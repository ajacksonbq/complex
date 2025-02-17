#include "SilhouetteFilter.hpp"

#include "ComplexCore/Filters/Algorithms/Silhouette.hpp"

#include "complex/Common/TypeTraits.hpp"
#include "complex/DataStructure/DataArray.hpp"
#include "complex/DataStructure/DataPath.hpp"
#include "complex/DataStructure/IDataArray.hpp"
#include "complex/Filter/Actions/CreateArrayAction.hpp"
#include "complex/Filter/Actions/DeleteDataAction.hpp"
#include "complex/Parameters/ArrayCreationParameter.hpp"
#include "complex/Parameters/ArraySelectionParameter.hpp"
#include "complex/Parameters/BoolParameter.hpp"
#include "complex/Parameters/ChoicesParameter.hpp"
#include "complex/Utilities/KUtilities.hpp"

using namespace complex;

namespace
{
const std::string k_MaskName = "temp_mask";
}

namespace complex
{
//------------------------------------------------------------------------------
std::string SilhouetteFilter::name() const
{
  return FilterTraits<SilhouetteFilter>::name.str();
}

//------------------------------------------------------------------------------
std::string SilhouetteFilter::className() const
{
  return FilterTraits<SilhouetteFilter>::className;
}

//------------------------------------------------------------------------------
Uuid SilhouetteFilter::uuid() const
{
  return FilterTraits<SilhouetteFilter>::uuid;
}

//------------------------------------------------------------------------------
std::string SilhouetteFilter::humanName() const
{
  return "Silhouette";
}

//------------------------------------------------------------------------------
std::vector<std::string> SilhouetteFilter::defaultTags() const
{
  return {className(), "DREAM3D Review", "Clustering"};
}

//------------------------------------------------------------------------------
Parameters SilhouetteFilter::parameters() const
{
  Parameters params;

  // Create the parameter descriptors that are needed for this filter
  params.insertSeparator(Parameters::Separator{"Optional Parameters"});
  params.insertLinkableParameter(std::make_unique<BoolParameter>(k_UseMask_Key, "Use Mask", "Specifies whether or not to use a mask array", false));
  params.insert(std::make_unique<ArraySelectionParameter>(k_MaskArrayPath_Key, "Mask Array", "DataPath to the boolean mask array. Values that are true will mark that cell/point as usable.",
                                                          DataPath{}, ArraySelectionParameter::AllowedTypes{DataType::boolean}));

  params.insertSeparator(Parameters::Separator{"Input Parameters"});
  params.insert(
      std::make_unique<ChoicesParameter>(k_DistanceMetric_Key, "Distance Metric", "Distance Metric type to be used for calculations", to_underlying(KUtilities::DistanceMetric::Euclidean),
                                         ChoicesParameter::Choices{"Euclidean", "Squared Euclidean", "Manhattan", "Cosine", "Pearson", "Squared Pearson"})); // sequence dependent DO NOT REORDER

  params.insertSeparator(Parameters::Separator{"Required Objects"});
  params.insert(std::make_unique<ArraySelectionParameter>(k_SelectedArrayPath_Key, "Attribute Array to Silhouette", "The DataPath to the input DataArray", DataPath{}, complex::GetAllNumericTypes()));
  params.insert(std::make_unique<ArraySelectionParameter>(k_FeatureIdsArrayPath_Key, "Cluster Ids", "The DataPath to the DataArray that specifies which cluster each point belongs", DataPath{},
                                                          ArraySelectionParameter::AllowedTypes{DataType::int32}));

  params.insertSeparator(Parameters::Separator{"Created Objects"});
  params.insert(std::make_unique<ArrayCreationParameter>(k_SilhouetteArrayPath_Key, "Silhouette", "The DataPath to the calculated output Silhouette array values", DataPath{}));

  // Associate the Linkable Parameter(s) to the children parameters that they control
  params.linkParameters(k_UseMask_Key, k_MaskArrayPath_Key, true);

  return params;
}

//------------------------------------------------------------------------------
IFilter::UniquePointer SilhouetteFilter::clone() const
{
  return std::make_unique<SilhouetteFilter>();
}

//------------------------------------------------------------------------------
IFilter::PreflightResult SilhouetteFilter::preflightImpl(const DataStructure& dataStructure, const Arguments& filterArgs, const MessageHandler& messageHandler,
                                                         const std::atomic_bool& shouldCancel) const
{
  auto pUseMaskValue = filterArgs.value<bool>(k_UseMask_Key);
  auto pSelectedArrayPathValue = filterArgs.value<DataPath>(k_SelectedArrayPath_Key);
  auto pMaskArrayPathValue = filterArgs.value<DataPath>(k_MaskArrayPath_Key);
  auto pFeatureIdsArrayPathValue = filterArgs.value<DataPath>(k_FeatureIdsArrayPath_Key);
  auto pSilhouetteArrayPathValue = filterArgs.value<DataPath>(k_SilhouetteArrayPath_Key);

  PreflightResult preflightResult;
  complex::Result<OutputActions> resultOutputActions;
  std::vector<PreflightValue> preflightUpdatedValues;

  auto clusterArray = dataStructure.getDataAs<IDataArray>(pSelectedArrayPathValue);
  auto clusterIds = dataStructure.getDataAs<IDataArray>(pFeatureIdsArrayPathValue);
  if(clusterArray->getNumberOfTuples() != clusterIds->getNumberOfTuples())
  {
    return MakePreflightErrorResult(-8976, fmt::format("The the number of tuples for {} ({}) do not match the number of tuples for {} ({})", clusterArray->getName(), clusterArray->getNumberOfTuples(),
                                                       clusterIds->getName(), clusterIds->getNumberOfTuples()));
  }

  if(!pUseMaskValue)
  {
    DataPath tempPath = DataPath({k_MaskName});
    {
      auto createAction = std::make_unique<CreateArrayAction>(DataType::boolean, clusterArray->getTupleShape(), std::vector<usize>{1}, tempPath);
      resultOutputActions.value().appendAction(std::move(createAction));
    }

    resultOutputActions.value().appendDeferredAction(std::make_unique<DeleteDataAction>(tempPath));
  }

  {
    auto createAction = std::make_unique<CreateArrayAction>(DataType::float64, clusterArray->getTupleShape(), std::vector<usize>{1}, pSilhouetteArrayPathValue);
    resultOutputActions.value().appendAction(std::move(createAction));
  }

  // Return both the resultOutputActions and the preflightUpdatedValues via std::move()
  return {std::move(resultOutputActions), std::move(preflightUpdatedValues)};
}

//------------------------------------------------------------------------------
Result<> SilhouetteFilter::executeImpl(DataStructure& dataStructure, const Arguments& filterArgs, const PipelineFilter* pipelineNode, const MessageHandler& messageHandler,
                                       const std::atomic_bool& shouldCancel) const
{
  auto maskPath = filterArgs.value<DataPath>(k_MaskArrayPath_Key);
  if(!filterArgs.value<bool>(k_UseMask_Key))
  {
    maskPath = DataPath({k_MaskName});
    dataStructure.getDataRefAs<BoolArray>(maskPath).fill(true);
  }

  SilhouetteInputValues inputValues;

  inputValues.DistanceMetric = static_cast<KUtilities::DistanceMetric>(filterArgs.value<ChoicesParameter::ValueType>(k_DistanceMetric_Key));
  inputValues.ClusteringArrayPath = filterArgs.value<DataPath>(k_SelectedArrayPath_Key);
  inputValues.MaskArrayPath = maskPath;
  inputValues.FeatureIdsArrayPath = filterArgs.value<DataPath>(k_FeatureIdsArrayPath_Key);
  inputValues.SilhouetteArrayPath = filterArgs.value<DataPath>(k_SilhouetteArrayPath_Key);

  return Silhouette(dataStructure, messageHandler, shouldCancel, &inputValues)();
}
} // namespace complex
