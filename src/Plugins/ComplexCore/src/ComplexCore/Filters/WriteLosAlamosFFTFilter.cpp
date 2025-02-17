#include "WriteLosAlamosFFTFilter.hpp"

#include "ComplexCore/Filters/Algorithms/WriteLosAlamosFFT.hpp"

#include "complex/DataStructure/DataPath.hpp"
#include "complex/Filter/Actions/EmptyAction.hpp"
#include "complex/Parameters/ArraySelectionParameter.hpp"
#include "complex/Parameters/FileSystemPathParameter.hpp"
#include "complex/Parameters/GeometrySelectionParameter.hpp"

#include <filesystem>

namespace fs = std::filesystem;
using namespace complex;

namespace complex
{
//------------------------------------------------------------------------------
std::string WriteLosAlamosFFTFilter::name() const
{
  return FilterTraits<WriteLosAlamosFFTFilter>::name.str();
}

//------------------------------------------------------------------------------
std::string WriteLosAlamosFFTFilter::className() const
{
  return FilterTraits<WriteLosAlamosFFTFilter>::className;
}

//------------------------------------------------------------------------------
Uuid WriteLosAlamosFFTFilter::uuid() const
{
  return FilterTraits<WriteLosAlamosFFTFilter>::uuid;
}

//------------------------------------------------------------------------------
std::string WriteLosAlamosFFTFilter::humanName() const
{
  return "Write Los Alamos FFT File";
}

//------------------------------------------------------------------------------
std::vector<std::string> WriteLosAlamosFFTFilter::defaultTags() const
{
  return {className(), "IO", "Output", "Write", "Export"};
}

//------------------------------------------------------------------------------
Parameters WriteLosAlamosFFTFilter::parameters() const
{
  Parameters params;

  // Create the parameter descriptors that are needed for this filter
  params.insertSeparator(Parameters::Separator{"Input Parameters"});
  params.insert(std::make_unique<FileSystemPathParameter>(k_OutputFile_Key, "Output File Path", "The path to the output file", fs::path(""), FileSystemPathParameter::ExtensionsType{},
                                                          FileSystemPathParameter::PathType::OutputFile));

  params.insertSeparator(Parameters::Separator{"Required Data Objects"});
  params.insert(std::make_unique<GeometrySelectionParameter>(k_ImageGeomPath, "Parent Image Geometry", "The parent image geometry holding the subsequent arrays", DataPath{},
                                                             GeometrySelectionParameter::AllowedTypes{IGeometry::Type::Image}));
  params.insert(std::make_unique<ArraySelectionParameter>(k_FeatureIdsArrayPath_Key, "Feature Ids", "Data Array that specifies to which Feature each Element belongs", DataPath{},
                                                          ArraySelectionParameter::AllowedTypes{DataType::int32}, ArraySelectionParameter::AllowedComponentShapes{{1}}));
  params.insert(std::make_unique<ArraySelectionParameter>(k_CellEulerAnglesArrayPath_Key, "Euler Angles",
                                                          "Data Array containing the three angles defining the orientation for each of the Cell in Bunge convention (Z-X-Z)", DataPath{},
                                                          ArraySelectionParameter::AllowedTypes{DataType::float32}, ArraySelectionParameter::AllowedComponentShapes{{3}}));
  params.insert(std::make_unique<ArraySelectionParameter>(k_CellPhasesArrayPath_Key, "Feature Phases", "Data Array that specifies to which Ensemble each Cell belongs", DataPath{},
                                                          ArraySelectionParameter::AllowedTypes{DataType::int32}, ArraySelectionParameter::AllowedComponentShapes{{1}}));

  return params;
}

//------------------------------------------------------------------------------
IFilter::UniquePointer WriteLosAlamosFFTFilter::clone() const
{
  return std::make_unique<WriteLosAlamosFFTFilter>();
}

//------------------------------------------------------------------------------
IFilter::PreflightResult WriteLosAlamosFFTFilter::preflightImpl(const DataStructure& dataStructure, const Arguments& filterArgs, const MessageHandler& messageHandler,
                                                                const std::atomic_bool& shouldCancel) const
{
  auto pOutputFileValue = filterArgs.value<FileSystemPathParameter::ValueType>(k_OutputFile_Key);
  auto pFeatureIdsArrayPathValue = filterArgs.value<DataPath>(k_FeatureIdsArrayPath_Key);
  auto pCellEulerAnglesArrayPathValue = filterArgs.value<DataPath>(k_CellEulerAnglesArrayPath_Key);
  auto pCellPhasesArrayPathValue = filterArgs.value<DataPath>(k_CellPhasesArrayPath_Key);

  PreflightResult preflightResult;
  complex::Result<OutputActions> resultOutputActions;
  std::vector<PreflightValue> preflightUpdatedValues;

  usize fIdsTupCount = dataStructure.getDataAs<IDataArray>(pFeatureIdsArrayPathValue)->getNumberOfTuples();
  usize eulersTupCount = dataStructure.getDataAs<IDataArray>(pCellEulerAnglesArrayPathValue)->getNumberOfTuples();
  usize phasesTupCount = dataStructure.getDataAs<IDataArray>(pCellPhasesArrayPathValue)->getNumberOfTuples();

  if(fIdsTupCount != eulersTupCount || fIdsTupCount != phasesTupCount)
  {
    return MakePreflightErrorResult(-73460, fmt::format("Tuple Dimensions don't match: Feature Ids - {} || Euler Angles - {} || Phases - {}", fIdsTupCount, eulersTupCount, phasesTupCount));
  }

  // Return both the resultOutputActions and the preflightUpdatedValues via std::move()
  return {std::move(resultOutputActions), std::move(preflightUpdatedValues)};
}

//------------------------------------------------------------------------------
Result<> WriteLosAlamosFFTFilter::executeImpl(DataStructure& dataStructure, const Arguments& filterArgs, const PipelineFilter* pipelineNode, const MessageHandler& messageHandler,
                                              const std::atomic_bool& shouldCancel) const
{
  WriteLosAlamosFFTInputValues inputValues;

  inputValues.OutputFile = filterArgs.value<FileSystemPathParameter::ValueType>(k_OutputFile_Key);
  inputValues.FeatureIdsArrayPath = filterArgs.value<DataPath>(k_FeatureIdsArrayPath_Key);
  inputValues.CellEulerAnglesArrayPath = filterArgs.value<DataPath>(k_CellEulerAnglesArrayPath_Key);
  inputValues.CellPhasesArrayPath = filterArgs.value<DataPath>(k_CellPhasesArrayPath_Key);
  inputValues.ImageGeomPath = filterArgs.value<DataPath>(k_ImageGeomPath);

  return WriteLosAlamosFFT(dataStructure, messageHandler, shouldCancel, &inputValues)();
}
} // namespace complex
