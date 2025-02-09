#include "WriteGBCDGMTFileFilter.hpp"

#include "complex/DataStructure/DataArray.hpp"
#include "complex/DataStructure/DataPath.hpp"
#include "complex/Parameters/ArraySelectionParameter.hpp"
#include "complex/Parameters/FileSystemPathParameter.hpp"
#include "complex/Parameters/NumberParameter.hpp"
#include "complex/Parameters/VectorParameter.hpp"

#include "OrientationAnalysis/Filters/Algorithms/WriteGBCDGMTFile.hpp"

#include <filesystem>

namespace fs = std::filesystem;

using namespace complex;

namespace complex
{
//------------------------------------------------------------------------------
std::string WriteGBCDGMTFileFilter::name() const
{
  return FilterTraits<WriteGBCDGMTFileFilter>::name.str();
}

//------------------------------------------------------------------------------
std::string WriteGBCDGMTFileFilter::className() const
{
  return FilterTraits<WriteGBCDGMTFileFilter>::className;
}

//------------------------------------------------------------------------------
Uuid WriteGBCDGMTFileFilter::uuid() const
{
  return FilterTraits<WriteGBCDGMTFileFilter>::uuid;
}

//------------------------------------------------------------------------------
std::string WriteGBCDGMTFileFilter::humanName() const
{
  return "Write GBCD Pole Figure (GMT 5)";
}

//------------------------------------------------------------------------------
std::vector<std::string> WriteGBCDGMTFileFilter::defaultTags() const
{
  return {className(), "IO", "Output", "Write", "Export"};
}

//------------------------------------------------------------------------------
Parameters WriteGBCDGMTFileFilter::parameters() const
{
  Parameters params;
  // Create the parameter descriptors that are needed for this filter
  params.insertSeparator(Parameters::Separator{"Input Parameters"});
  params.insert(std::make_unique<Int32Parameter>(k_PhaseOfInterest_Key, "Phase of Interest", "Index of the Ensemble for which to plot the pole figure", 1));
  params.insert(std::make_unique<VectorFloat32Parameter>(k_MisorientationRotation_Key, "Misorientation Axis-Angle", "Axis-Angle pair values for drawing GBCD", std::vector<float32>(4, 0),
                                                         std::vector<std::string>{"Angle", "h", "k", "l"}));
  params.insert(std::make_unique<FileSystemPathParameter>(k_OutputFile_Key, "Output GMT File", "The output .dat file path", FileSystemPathParameter::ValueType{},
                                                          FileSystemPathParameter::ExtensionsType{".dat"}, FileSystemPathParameter::PathType::OutputFile, false));
  params.insertSeparator(Parameters::Separator{"Required Face Ensemble Data"});
  params.insert(std::make_unique<ArraySelectionParameter>(k_GBCDArrayPath_Key, "GBCD",
                                                          "5 parameter GBCD data. The 6th component is used internally to track the northern vs. southern hemisphere of the Lambert sphere",
                                                          DataPath({"FaceEnsembleData", "GBCD"}), ArraySelectionParameter::AllowedTypes{DataType::float64}));
  params.insert(std::make_unique<ArraySelectionParameter>(k_CrystalStructuresArrayPath_Key, "Crystal Structures", "Enumeration representing the crystal structure for each Ensemble",
                                                          DataPath({"Ensemble Data", "CrystalStructures"}), ArraySelectionParameter::AllowedTypes{DataType::uint32},
                                                          ArraySelectionParameter::AllowedComponentShapes{{1}}));

  return params;
}

//------------------------------------------------------------------------------
IFilter::UniquePointer WriteGBCDGMTFileFilter::clone() const
{
  return std::make_unique<WriteGBCDGMTFileFilter>();
}

//------------------------------------------------------------------------------
IFilter::PreflightResult WriteGBCDGMTFileFilter::preflightImpl(const DataStructure& dataStructure, const Arguments& filterArgs, const MessageHandler& messageHandler,
                                                               const std::atomic_bool& shouldCancel) const
{
  auto pPhaseOfInterestValue = filterArgs.value<int32>(k_PhaseOfInterest_Key);
  auto pMisorientationRotationValue = filterArgs.value<VectorFloat32Parameter::ValueType>(k_MisorientationRotation_Key);
  auto pOutputFileValue = filterArgs.value<FileSystemPathParameter::ValueType>(k_OutputFile_Key);
  auto pGBCDArrayPathValue = filterArgs.value<DataPath>(k_GBCDArrayPath_Key);
  auto pCrystalStructuresArrayPathValue = filterArgs.value<DataPath>(k_CrystalStructuresArrayPath_Key);

  PreflightResult preflightResult;
  complex::Result<OutputActions> resultOutputActions;
  std::vector<PreflightValue> preflightUpdatedValues;

  // Make sure the file name ends with _1 so the GMT scripts work correctly
  std::string fileName = pOutputFileValue.stem().string();
  if(fileName.substr(fileName.size() - 2) == "_1")
  {
    fileName = fileName + "_1.dat";
    pOutputFileValue = pOutputFileValue.parent_path() / fileName;
  }

  if(dataStructure.getDataAs<UInt32Array>(pCrystalStructuresArrayPathValue) == nullptr)
  {
    return {MakeErrorResult<OutputActions>(-96710, fmt::format("Could not find crystal structures array at path '{}'", pCrystalStructuresArrayPathValue.toString()))};
  }

  const auto* gbcd = dataStructure.getDataAs<Float64Array>(pGBCDArrayPathValue);
  if(gbcd == nullptr)
  {
    return {MakeErrorResult<OutputActions>(-96711, fmt::format("Could not find GBCD array at path '{}'", pGBCDArrayPathValue.toString()))};
  }

  auto numEnsembles = gbcd->getNumberOfTuples();
  if(pPhaseOfInterestValue >= numEnsembles)
  {
    return {MakeErrorResult<OutputActions>(-96712, fmt::format("The phase index {} is larger than the number of Ensembles {}", pPhaseOfInterestValue, numEnsembles))};
  }

  return {std::move(resultOutputActions), std::move(preflightUpdatedValues)};
}

//------------------------------------------------------------------------------
Result<> WriteGBCDGMTFileFilter::executeImpl(DataStructure& dataStructure, const Arguments& filterArgs, const PipelineFilter* pipelineNode, const MessageHandler& messageHandler,
                                             const std::atomic_bool& shouldCancel) const
{
  WriteGBCDGMTFileInputValues inputValues;
  inputValues.PhaseOfInterest = filterArgs.value<int32>(k_PhaseOfInterest_Key);
  inputValues.MisorientationRotation = filterArgs.value<VectorFloat32Parameter::ValueType>(k_MisorientationRotation_Key);
  inputValues.OutputFile = filterArgs.value<FileSystemPathParameter::ValueType>(k_OutputFile_Key);
  inputValues.GBCDArrayPath = filterArgs.value<DataPath>(k_GBCDArrayPath_Key);
  inputValues.CrystalStructuresArrayPath = filterArgs.value<DataPath>(k_CrystalStructuresArrayPath_Key);

  return WriteGBCDGMTFile(dataStructure, messageHandler, shouldCancel, &inputValues)();
}
} // namespace complex
