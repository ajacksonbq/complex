#include "FindVertexToTriangleDistancesFilter.hpp"

#include "ComplexCore/Filters/Algorithms/FindVertexToTriangleDistances.hpp"

#include "complex/DataStructure/DataPath.hpp"
#include "complex/DataStructure/Geometry/TriangleGeom.hpp"
#include "complex/DataStructure/Geometry/VertexGeom.hpp"
#include "complex/Filter/Actions/CreateArrayAction.hpp"
#include "complex/Filter/Actions/DeleteDataAction.hpp"
#include "complex/Parameters/ArraySelectionParameter.hpp"
#include "complex/Parameters/DataObjectNameParameter.hpp"
#include "complex/Parameters/GeometrySelectionParameter.hpp"

using namespace complex;

namespace
{
inline constexpr StringLiteral k_TriangleBounds("Triangle Bounds");
}

namespace complex
{
//------------------------------------------------------------------------------
std::string FindVertexToTriangleDistancesFilter::name() const
{
  return FilterTraits<FindVertexToTriangleDistancesFilter>::name.str();
}

//------------------------------------------------------------------------------
std::string FindVertexToTriangleDistancesFilter::className() const
{
  return FilterTraits<FindVertexToTriangleDistancesFilter>::className;
}

//------------------------------------------------------------------------------
Uuid FindVertexToTriangleDistancesFilter::uuid() const
{
  return FilterTraits<FindVertexToTriangleDistancesFilter>::uuid;
}

//------------------------------------------------------------------------------
std::string FindVertexToTriangleDistancesFilter::humanName() const
{
  return "Find Vertex to Triangle Distances";
}

//------------------------------------------------------------------------------
std::vector<std::string> FindVertexToTriangleDistancesFilter::defaultTags() const
{
  return {className(), "#Sampling", "#Spatial"};
}

//------------------------------------------------------------------------------
Parameters FindVertexToTriangleDistancesFilter::parameters() const
{
  Parameters params;

  // Create the parameter descriptors that are needed for this filter
  params.insertSeparator(Parameters::Separator{"Required Input Arrays"});
  params.insert(std::make_unique<GeometrySelectionParameter>(k_VertexDataContainer_Key, "Source Vertex Geometry", "The Vertex Geometry point cloud to map to triangles", DataPath{},
                                                             GeometrySelectionParameter::AllowedTypes{IGeometry::Type::Vertex}));
  params.insert(std::make_unique<GeometrySelectionParameter>(k_TriangleDataContainer_Key, "Target Triangle Geometry", "The triangle geometry to compare against", DataPath{},
                                                             GeometrySelectionParameter::AllowedTypes{IGeometry::Type::Triangle}));
  params.insert(
      std::make_unique<ArraySelectionParameter>(k_TriangleNormalsArrayPath_Key, "Triangle Normals", "The triangle geometry's normals array", DataPath{}, std::set<DataType>{DataType::float64}));

  params.insertSeparator(Parameters::Separator{"Created Output Arrays"});
  params.insert(std::make_unique<DataObjectNameParameter>(k_DistancesArrayPath_Key, "Distances Array", "The array to store distance between vertex and triangle", ""));
  params.insert(std::make_unique<DataObjectNameParameter>(k_ClosestTriangleIdArrayPath_Key, "Closest Triangle Ids Array", "The array to store the ID of the closest triangle", ""));

  return params;
}

//------------------------------------------------------------------------------
IFilter::UniquePointer FindVertexToTriangleDistancesFilter::clone() const
{
  return std::make_unique<FindVertexToTriangleDistancesFilter>();
}

//------------------------------------------------------------------------------
IFilter::PreflightResult FindVertexToTriangleDistancesFilter::preflightImpl(const DataStructure& dataStructure, const Arguments& filterArgs, const MessageHandler& messageHandler,
                                                                            const std::atomic_bool& shouldCancel) const
{
  auto pVertexGeometryDataPath = filterArgs.value<DataPath>(k_VertexDataContainer_Key);
  auto pTriangleGeometryDataPath = filterArgs.value<DataPath>(k_TriangleDataContainer_Key);
  auto pTriangleNormalsArrayDataPath = filterArgs.value<DataPath>(k_TriangleNormalsArrayPath_Key);
  auto pDistancesDataName = filterArgs.value<std::string>(k_DistancesArrayPath_Key);
  auto pClosestTriangleIdDataName = filterArgs.value<std::string>(k_ClosestTriangleIdArrayPath_Key);

  PreflightResult preflightResult;

  complex::Result<OutputActions> resultOutputActions;

  std::vector<PreflightValue> preflightUpdatedValues;

  auto* vertexGeomPtr = dataStructure.getDataAs<VertexGeom>(pVertexGeometryDataPath);
  if(vertexGeomPtr == nullptr)
  {
    return {MakeErrorResult<OutputActions>(-4530, fmt::format("The DataPath {} is not a valid VertexGeometry.", pVertexGeometryDataPath.toString()))};
  }

  auto* triangleGeomPtr = dataStructure.getDataAs<TriangleGeom>(pTriangleGeometryDataPath);
  if(triangleGeomPtr == nullptr)
  {
    return {MakeErrorResult<OutputActions>(-4531, fmt::format("The DataPath {} is not a valid TriangleGeometry.", pTriangleGeometryDataPath.toString()))};
  }

  const DataPath vertexDataPath = pVertexGeometryDataPath.createChildPath(vertexGeomPtr->getVertexAttributeMatrix()->getName());

  auto createDistancesArrayAction = std::make_unique<CreateArrayAction>(complex::DataType::float32, std::vector<usize>{vertexGeomPtr->getNumberOfVertices()}, std::vector<usize>{1},
                                                                        vertexDataPath.createChildPath(pDistancesDataName));
  resultOutputActions.value().appendAction(std::move(createDistancesArrayAction));

  auto createClosestTriangleIdArrayAction = std::make_unique<CreateArrayAction>(complex::DataType::int64, std::vector<usize>{vertexGeomPtr->getNumberOfVertices()}, std::vector<usize>{1},
                                                                                vertexDataPath.createChildPath(pClosestTriangleIdDataName));
  resultOutputActions.value().appendAction(std::move(createClosestTriangleIdArrayAction));

  // Create temp array then deferred delete
  auto tempTriBoundsDataPath = DataPath({::k_TriangleBounds});
  auto createTriBoundsArrayAction =
      std::make_unique<CreateArrayAction>(complex::DataType::float32, std::vector<usize>{triangleGeomPtr->getNumberOfFaces()}, std::vector<usize>{6}, tempTriBoundsDataPath);
  resultOutputActions.value().appendAction(std::move(createTriBoundsArrayAction));

  auto removeTempArrayAction = std::make_unique<DeleteDataAction>(tempTriBoundsDataPath);
  resultOutputActions.value().appendDeferredAction(std::move(removeTempArrayAction));

  // Return both the resultOutputActions and the preflightUpdatedValues via std::move()
  return {std::move(resultOutputActions), std::move(preflightUpdatedValues)};
}

//------------------------------------------------------------------------------
Result<> FindVertexToTriangleDistancesFilter::executeImpl(DataStructure& dataStructure, const Arguments& filterArgs, const PipelineFilter* pipelineNode, const MessageHandler& messageHandler,
                                                          const std::atomic_bool& shouldCancel) const
{
  FindVertexToTriangleDistancesInputValues inputValues;

  inputValues.VertexDataContainer = filterArgs.value<DataPath>(k_VertexDataContainer_Key);
  inputValues.TriangleDataContainer = filterArgs.value<DataPath>(k_TriangleDataContainer_Key);
  inputValues.TriangleNormalsArrayPath = filterArgs.value<DataPath>(k_TriangleNormalsArrayPath_Key);

  auto vertexGeom = dataStructure.getDataRefAs<VertexGeom>(inputValues.VertexDataContainer);
  const DataPath vertexDataPath = inputValues.VertexDataContainer.createChildPath(vertexGeom.getVertexAttributeMatrix()->getName());
  inputValues.DistancesArrayPath = vertexDataPath.createChildPath(filterArgs.value<std::string>(k_DistancesArrayPath_Key));
  inputValues.ClosestTriangleIdArrayPath = vertexDataPath.createChildPath(filterArgs.value<std::string>(k_ClosestTriangleIdArrayPath_Key));

  inputValues.TriBoundsDataPath = DataPath({::k_TriangleBounds});

  return FindVertexToTriangleDistances(dataStructure, messageHandler, shouldCancel, &inputValues)();
}
} // namespace complex
