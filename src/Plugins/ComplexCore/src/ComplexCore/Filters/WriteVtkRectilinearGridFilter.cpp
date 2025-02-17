#include "WriteVtkRectilinearGridFilter.hpp"

#include "ComplexCore/Filters/Algorithms/WriteVtkRectilinearGrid.hpp"

#include "complex/DataStructure/DataPath.hpp"
#include "complex/DataStructure/Geometry/ImageGeom.hpp"
#include "complex/Parameters/BoolParameter.hpp"
#include "complex/Parameters/FileSystemPathParameter.hpp"
#include "complex/Parameters/GeometrySelectionParameter.hpp"
#include "complex/Parameters/MultiArraySelectionParameter.hpp"

#include <filesystem>
namespace fs = std::filesystem;

using namespace complex;

namespace complex
{
//------------------------------------------------------------------------------
std::string WriteVtkRectilinearGridFilter::name() const
{
  return FilterTraits<WriteVtkRectilinearGridFilter>::name.str();
}

//------------------------------------------------------------------------------
std::string WriteVtkRectilinearGridFilter::className() const
{
  return FilterTraits<WriteVtkRectilinearGridFilter>::className;
}

//------------------------------------------------------------------------------
Uuid WriteVtkRectilinearGridFilter::uuid() const
{
  return FilterTraits<WriteVtkRectilinearGridFilter>::uuid;
}

//------------------------------------------------------------------------------
std::string WriteVtkRectilinearGridFilter::humanName() const
{
  return "Write Vtk Rectilinear Grid";
}

//------------------------------------------------------------------------------
std::vector<std::string> WriteVtkRectilinearGridFilter::defaultTags() const
{
  return {className(), "IO", "Output", "Write", "Export"};
}

//------------------------------------------------------------------------------
Parameters WriteVtkRectilinearGridFilter::parameters() const
{
  Parameters params;

  // Create the parameter descriptors that are needed for this filter
  params.insert(std::make_unique<FileSystemPathParameter>(k_OutputFile_Key, "Output File", "The output vtk file in which the geometry data is written", fs::path("Data/Output/RectilinearGrid.vtk"),
                                                          FileSystemPathParameter::ExtensionsType{".vtk"}, FileSystemPathParameter::PathType::OutputFile));
  params.insert(std::make_unique<BoolParameter>(k_WriteBinaryFile_Key, "Write Binary File", "Whether or not to write the vtk file in binary", false));
  params.insert(std::make_unique<GeometrySelectionParameter>(k_ImageGeometryPath_Key, "Image Geometry", "The path to the image geometry in which to write out to the vtk file", DataPath{},
                                                             GeometrySelectionParameter::AllowedTypes{IGeometry::Type::Image}));
  params.insert(std::make_unique<MultiArraySelectionParameter>(k_SelectedDataArrayPaths_Key, "Cell Data Arrays to Write", "The paths to the cell data arrays to write out with the geometry",
                                                               MultiArraySelectionParameter::ValueType{}, MultiArraySelectionParameter::AllowedTypes{IArray::ArrayType::DataArray}, GetAllDataTypes()));

  return params;
}

//------------------------------------------------------------------------------
IFilter::UniquePointer WriteVtkRectilinearGridFilter::clone() const
{
  return std::make_unique<WriteVtkRectilinearGridFilter>();
}

//------------------------------------------------------------------------------
IFilter::PreflightResult WriteVtkRectilinearGridFilter::preflightImpl(const DataStructure& dataStructure, const Arguments& filterArgs, const MessageHandler& messageHandler,
                                                                      const std::atomic_bool& shouldCancel) const
{
  auto pOutputFileValue = filterArgs.value<FileSystemPathParameter::ValueType>(k_OutputFile_Key);
  auto pWriteBinaryFileValue = filterArgs.value<bool>(k_WriteBinaryFile_Key);
  auto pImageGeometryPathValue = filterArgs.value<DataPath>(k_ImageGeometryPath_Key);
  auto pSelectedDataArrayPathsValue = filterArgs.value<MultiArraySelectionParameter::ValueType>(k_SelectedDataArrayPaths_Key);

  PreflightResult preflightResult;
  complex::Result<OutputActions> resultOutputActions;
  std::vector<PreflightValue> preflightUpdatedValues;

  if(pSelectedDataArrayPathsValue.empty())
  {
    return MakePreflightErrorResult(-2070, "No cell data arrays are selected. You must select at least one array.");
  }

  auto tupleValidityCheck = dataStructure.validateNumberOfTuples(pSelectedDataArrayPathsValue);
  if(!tupleValidityCheck)
  {
    return {MakeErrorResult<OutputActions>(-2071, fmt::format("The following DataArrays all must have equal number of tuples but this was not satisfied.\n{}", tupleValidityCheck.error()))};
  }

  usize numTuples = dataStructure.getDataRefAs<IDataArray>(pSelectedDataArrayPathsValue[0]).getNumberOfTuples();
  usize numCells = dataStructure.getDataRefAs<ImageGeom>(pImageGeometryPathValue).getNumberOfCells();
  if(numCells != numTuples)
  {
    return MakePreflightErrorResult(
        -2072,
        fmt::format("The selected data arrays do not have the same number of tuples as the number of cells in the selected geometry ({}). Make sure your selected arrays are cell level data arrays",
                    pImageGeometryPathValue.toString()));
  }

  return {std::move(resultOutputActions), std::move(preflightUpdatedValues)};
}

//------------------------------------------------------------------------------
Result<> WriteVtkRectilinearGridFilter::executeImpl(DataStructure& dataStructure, const Arguments& filterArgs, const PipelineFilter* pipelineNode, const MessageHandler& messageHandler,
                                                    const std::atomic_bool& shouldCancel) const
{
  WriteVtkRectilinearGridInputValues inputValues;

  inputValues.OutputFile = filterArgs.value<FileSystemPathParameter::ValueType>(k_OutputFile_Key);
  inputValues.WriteBinaryFile = filterArgs.value<bool>(k_WriteBinaryFile_Key);
  inputValues.ImageGeometryPath = filterArgs.value<DataPath>(k_ImageGeometryPath_Key);
  inputValues.SelectedDataArrayPaths = filterArgs.value<MultiArraySelectionParameter::ValueType>(k_SelectedDataArrayPaths_Key);

  return WriteVtkRectilinearGrid(dataStructure, messageHandler, shouldCancel, &inputValues)();
}
} // namespace complex
