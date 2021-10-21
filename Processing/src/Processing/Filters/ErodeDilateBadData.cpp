#include "ErodeDilateBadData.hpp"

#include "complex/DataStructure/DataPath.hpp"
#include "complex/Parameters/ArraySelectionParameter.hpp"
#include "complex/Parameters/BoolParameter.hpp"
#include "complex/Parameters/ChoicesParameter.hpp"
#include "complex/Parameters/MultiArraySelectionParameter.hpp"
#include "complex/Parameters/NumberParameter.hpp"

using namespace complex;

namespace complex
{
//------------------------------------------------------------------------------
std::string ErodeDilateBadData::name() const
{
  return FilterTraits<ErodeDilateBadData>::name.str();
}

//------------------------------------------------------------------------------
std::string ErodeDilateBadData::className() const
{
  return FilterTraits<ErodeDilateBadData>::className;
}

//------------------------------------------------------------------------------
Uuid ErodeDilateBadData::uuid() const
{
  return FilterTraits<ErodeDilateBadData>::uuid;
}

//------------------------------------------------------------------------------
std::string ErodeDilateBadData::humanName() const
{
  return "Erode/Dilate Bad Data";
}

//------------------------------------------------------------------------------
std::vector<std::string> ErodeDilateBadData::defaultTags() const
{
  return {"#Processing", "#Cleanup"};
}

//------------------------------------------------------------------------------
Parameters ErodeDilateBadData::parameters() const
{
  Parameters params;
  // Create the parameter descriptors that are needed for this filter
  params.insert(std::make_unique<ChoicesParameter>(k_Direction_Key, "Operation", "", 0, ChoicesParameter::Choices{"Option 1", "Option 2", "Option 3"}));
  params.insert(std::make_unique<Int32Parameter>(k_NumIterations_Key, "Number of Iterations", "", 1234356));
  params.insert(std::make_unique<BoolParameter>(k_XDirOn_Key, "X Direction", "", false));
  params.insert(std::make_unique<BoolParameter>(k_YDirOn_Key, "Y Direction", "", false));
  params.insert(std::make_unique<BoolParameter>(k_ZDirOn_Key, "Z Direction", "", false));
  params.insertSeparator(Parameters::Separator{"Cell Data"});
  params.insert(std::make_unique<ArraySelectionParameter>(k_FeatureIdsArrayPath_Key, "Feature Ids", "", DataPath{}));
  params.insert(
      std::make_unique<MultiArraySelectionParameter>(k_IgnoredDataArrayPaths_Key, "Attribute Arrays to Ignore", "", MultiArraySelectionParameter::ValueType{DataPath(), DataPath(), DataPath()}));

  return params;
}

//------------------------------------------------------------------------------
IFilter::UniquePointer ErodeDilateBadData::clone() const
{
  return std::make_unique<ErodeDilateBadData>();
}

//------------------------------------------------------------------------------
Result<OutputActions> ErodeDilateBadData::preflightImpl(const DataStructure& ds, const Arguments& filterArgs, const MessageHandler& messageHandler) const
{
  /****************************************************************************
   * Write any preflight sanity checking codes in this function
   ***************************************************************************/
  auto pDirectionValue = filterArgs.value<ChoicesParameter::ValueType>(k_Direction_Key);
  auto pNumIterationsValue = filterArgs.value<int32>(k_NumIterations_Key);
  auto pXDirOnValue = filterArgs.value<bool>(k_XDirOn_Key);
  auto pYDirOnValue = filterArgs.value<bool>(k_YDirOn_Key);
  auto pZDirOnValue = filterArgs.value<bool>(k_ZDirOn_Key);
  auto pFeatureIdsArrayPathValue = filterArgs.value<DataPath>(k_FeatureIdsArrayPath_Key);
  auto pIgnoredDataArrayPathsValue = filterArgs.value<MultiArraySelectionParameter::ValueType>(k_IgnoredDataArrayPaths_Key);

  OutputActions actions;
#if 0
  // Define a custom class that generates the changes to the DataStructure.
  auto action = std::make_unique<ErodeDilateBadDataAction>();
  actions.actions.push_back(std::move(action));
#endif
  return {std::move(actions)};
}

//------------------------------------------------------------------------------
Result<> ErodeDilateBadData::executeImpl(DataStructure& ds, const Arguments& filterArgs, const MessageHandler& messageHandler) const
{
  /****************************************************************************
   * Extract the actual input values from the 'filterArgs' object
   ***************************************************************************/
  auto pDirectionValue = filterArgs.value<ChoicesParameter::ValueType>(k_Direction_Key);
  auto pNumIterationsValue = filterArgs.value<int32>(k_NumIterations_Key);
  auto pXDirOnValue = filterArgs.value<bool>(k_XDirOn_Key);
  auto pYDirOnValue = filterArgs.value<bool>(k_YDirOn_Key);
  auto pZDirOnValue = filterArgs.value<bool>(k_ZDirOn_Key);
  auto pFeatureIdsArrayPathValue = filterArgs.value<DataPath>(k_FeatureIdsArrayPath_Key);
  auto pIgnoredDataArrayPathsValue = filterArgs.value<MultiArraySelectionParameter::ValueType>(k_IgnoredDataArrayPaths_Key);

  /****************************************************************************
   * Write your algorithm implementation in this function
   ***************************************************************************/

  return {};
}
} // namespace complex
