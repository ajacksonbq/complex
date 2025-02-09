#include "FindSlipTransmissionMetricsFilter.hpp"

#include "OrientationAnalysis/Filters/Algorithms/FindSlipTransmissionMetrics.hpp"

#include "complex/DataStructure/DataPath.hpp"
#include "complex/DataStructure/NeighborList.hpp"
#include "complex/Filter/Actions/CreateNeighborListAction.hpp"
#include "complex/Parameters/ArraySelectionParameter.hpp"
#include "complex/Parameters/DataObjectNameParameter.hpp"
#include "complex/Parameters/NeighborListSelectionParameter.hpp"

using namespace complex;

namespace complex
{
//------------------------------------------------------------------------------
std::string FindSlipTransmissionMetricsFilter::name() const
{
  return FilterTraits<FindSlipTransmissionMetricsFilter>::name.str();
}

//------------------------------------------------------------------------------
std::string FindSlipTransmissionMetricsFilter::className() const
{
  return FilterTraits<FindSlipTransmissionMetricsFilter>::className;
}

//------------------------------------------------------------------------------
Uuid FindSlipTransmissionMetricsFilter::uuid() const
{
  return FilterTraits<FindSlipTransmissionMetricsFilter>::uuid;
}

//------------------------------------------------------------------------------
std::string FindSlipTransmissionMetricsFilter::humanName() const
{
  return "Find Neighbor Slip Transmission Metrics";
}

//------------------------------------------------------------------------------
std::vector<std::string> FindSlipTransmissionMetricsFilter::defaultTags() const
{
  return {className(), "Statistics", "Crystallography"};
}

//------------------------------------------------------------------------------
Parameters FindSlipTransmissionMetricsFilter::parameters() const
{
  Parameters params;

  // Create the parameter descriptors that are needed for this filter
  params.insertSeparator(Parameters::Separator{"Feature Data"});
  params.insert(std::make_unique<NeighborListSelectionParameter>(k_NeighborListArrayPath_Key, "Neighbor List", "List of the contiguous neighboring Features for a given Feature", DataPath{},
                                                                 NeighborListSelectionParameter::AllowedTypes{DataType::int32}));

  params.insertSeparator(Parameters::Separator{"Required Cell Feature Data"});
  params.insert(std::make_unique<ArraySelectionParameter>(k_AvgQuatsArrayPath_Key, "Average Quaternions",
                                                          "Data Array that specifies the average orientation of each Feature in quaternion representation", DataPath{},
                                                          ArraySelectionParameter::AllowedTypes{DataType::float32}, ArraySelectionParameter::AllowedComponentShapes{{4}}));
  params.insert(std::make_unique<ArraySelectionParameter>(k_FeaturePhasesArrayPath_Key, "Phases", "Data Array that specifies to which Ensemble each Feature belongs", DataPath{},
                                                          ArraySelectionParameter::AllowedTypes{DataType::int32}, ArraySelectionParameter::AllowedComponentShapes{{1}}));

  params.insertSeparator(Parameters::Separator{"Required Cell Ensemble Data"});
  params.insert(std::make_unique<ArraySelectionParameter>(k_CrystalStructuresArrayPath_Key, "Crystal Structures", "Enumeration representing the crystal structure for each phase", DataPath{},
                                                          ArraySelectionParameter::AllowedTypes{DataType::uint32}, ArraySelectionParameter::AllowedComponentShapes{{1}}));

  params.insertSeparator(Parameters::Separator{"Created Feature Data"});
  params.insert(std::make_unique<DataObjectNameParameter>(k_F1ListArrayName_Key, "F1 List", "DataArray Name to store the calculated F1s Values", "F1 List"));
  params.insert(std::make_unique<DataObjectNameParameter>(k_F1sptListArrayName_Key, "F1spt List", "DataArray Name to store the calculated F1spts Values", "F1spt List"));
  params.insert(std::make_unique<DataObjectNameParameter>(k_F7ListArrayName_Key, "F7 List", "DataArray Name to store the calculated F7s Values", "F7 List"));
  params.insert(std::make_unique<DataObjectNameParameter>(k_mPrimeListArrayName_Key, "mPrime List", "DataArray Name to store the calculated mPrimes Values", "mPrime List"));

  return params;
}

//------------------------------------------------------------------------------
IFilter::UniquePointer FindSlipTransmissionMetricsFilter::clone() const
{
  return std::make_unique<FindSlipTransmissionMetricsFilter>();
}

//------------------------------------------------------------------------------
IFilter::PreflightResult FindSlipTransmissionMetricsFilter::preflightImpl(const DataStructure& dataStructure, const Arguments& filterArgs, const MessageHandler& messageHandler,
                                                                          const std::atomic_bool& shouldCancel) const
{
  auto pNeighborListPathValue = filterArgs.value<DataPath>(k_NeighborListArrayPath_Key);
  auto pFeaturePhasesPathValue = filterArgs.value<DataPath>(k_FeaturePhasesArrayPath_Key);
  auto pF1ListNameValue = filterArgs.value<std::string>(k_F1ListArrayName_Key);
  auto pF1sptListNameValue = filterArgs.value<std::string>(k_F1sptListArrayName_Key);
  auto pF7ListNameValue = filterArgs.value<std::string>(k_F7ListArrayName_Key);
  auto pmPrimeListNameValue = filterArgs.value<std::string>(k_mPrimeListArrayName_Key);

  PreflightResult preflightResult;
  complex::Result<OutputActions> resultOutputActions;
  std::vector<PreflightValue> preflightUpdatedValues;

  usize tupShape = dataStructure.getDataAs<Int32NeighborList>(pNeighborListPathValue)->getNumberOfTuples();

  {
    auto action = std::make_unique<CreateNeighborListAction>(DataType::float32, tupShape, pNeighborListPathValue.getParent().createChildPath(pF1ListNameValue));
    resultOutputActions.value().appendAction(std::move(action));
  }

  {
    auto action = std::make_unique<CreateNeighborListAction>(DataType::float32, tupShape, pNeighborListPathValue.getParent().createChildPath(pF1sptListNameValue));
    resultOutputActions.value().appendAction(std::move(action));
  }

  {
    auto action = std::make_unique<CreateNeighborListAction>(DataType::float32, tupShape, pNeighborListPathValue.getParent().createChildPath(pF7ListNameValue));
    resultOutputActions.value().appendAction(std::move(action));
  }

  {
    auto action = std::make_unique<CreateNeighborListAction>(DataType::float32, tupShape, pNeighborListPathValue.getParent().createChildPath(pmPrimeListNameValue));
    resultOutputActions.value().appendAction(std::move(action));
  }

  // Return both the resultOutputActions and the preflightUpdatedValues via std::move()
  return {std::move(resultOutputActions), std::move(preflightUpdatedValues)};
}

//------------------------------------------------------------------------------
Result<> FindSlipTransmissionMetricsFilter::executeImpl(DataStructure& dataStructure, const Arguments& filterArgs, const PipelineFilter* pipelineNode, const MessageHandler& messageHandler,
                                                        const std::atomic_bool& shouldCancel) const
{
  FindSlipTransmissionMetricsInputValues inputValues;

  inputValues.NeighborListArrayPath = filterArgs.value<DataPath>(k_NeighborListArrayPath_Key);
  inputValues.AvgQuatsArrayPath = filterArgs.value<DataPath>(k_AvgQuatsArrayPath_Key);
  inputValues.FeaturePhasesArrayPath = filterArgs.value<DataPath>(k_FeaturePhasesArrayPath_Key);
  inputValues.CrystalStructuresArrayPath = filterArgs.value<DataPath>(k_CrystalStructuresArrayPath_Key);
  inputValues.F1ListArrayName = inputValues.NeighborListArrayPath.getParent().createChildPath(filterArgs.value<std::string>(k_F1ListArrayName_Key));
  inputValues.F1sptListArrayName = inputValues.NeighborListArrayPath.getParent().createChildPath(filterArgs.value<std::string>(k_F1sptListArrayName_Key));
  inputValues.F7ListArrayName = inputValues.NeighborListArrayPath.getParent().createChildPath(filterArgs.value<std::string>(k_F7ListArrayName_Key));
  inputValues.mPrimeListArrayName = inputValues.NeighborListArrayPath.getParent().createChildPath(filterArgs.value<std::string>(k_mPrimeListArrayName_Key));

  return FindSlipTransmissionMetrics(dataStructure, messageHandler, shouldCancel, &inputValues)();
}
} // namespace complex
