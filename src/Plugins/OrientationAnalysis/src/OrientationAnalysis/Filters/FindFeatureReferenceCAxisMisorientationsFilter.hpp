#pragma once

#include "OrientationAnalysis/OrientationAnalysis_export.hpp"

#include "complex/Filter/FilterTraits.hpp"
#include "complex/Filter/IFilter.hpp"

namespace complex
{
/**
 * @class FindFeatureReferenceCAxisMisorientationsFilter
 * @brief This filter calculates the misorientation angle between the C-axis of each Cell within a Feature and the average C-axis for that Feature and stores that value for each Cell.
 */
class ORIENTATIONANALYSIS_EXPORT FindFeatureReferenceCAxisMisorientationsFilter : public IFilter
{
public:
  FindFeatureReferenceCAxisMisorientationsFilter() = default;
  ~FindFeatureReferenceCAxisMisorientationsFilter() noexcept override = default;

  FindFeatureReferenceCAxisMisorientationsFilter(const FindFeatureReferenceCAxisMisorientationsFilter&) = delete;
  FindFeatureReferenceCAxisMisorientationsFilter(FindFeatureReferenceCAxisMisorientationsFilter&&) noexcept = delete;

  FindFeatureReferenceCAxisMisorientationsFilter& operator=(const FindFeatureReferenceCAxisMisorientationsFilter&) = delete;
  FindFeatureReferenceCAxisMisorientationsFilter& operator=(FindFeatureReferenceCAxisMisorientationsFilter&&) noexcept = delete;

  // Parameter Keys
  static inline constexpr StringLiteral k_ImageGeometryPath_Key = "image_geometry_path";
  static inline constexpr StringLiteral k_FeatureIdsArrayPath_Key = "feature_ids_array_path";
  static inline constexpr StringLiteral k_CellPhasesArrayPath_Key = "cell_phases_array_path";
  static inline constexpr StringLiteral k_QuatsArrayPath_Key = "quats_array_path";
  static inline constexpr StringLiteral k_AvgCAxesArrayPath_Key = "avg_c_axes_array_path";
  static inline constexpr StringLiteral k_CrystalStructuresArrayPath_Key = "crystal_structures_array_path";
  static inline constexpr StringLiteral k_FeatureAvgCAxisMisorientationsArrayName_Key = "feature_avg_c_axis_misorientations_array_name";
  static inline constexpr StringLiteral k_FeatureStdevCAxisMisorientationsArrayName_Key = "feature_stdev_c_axis_misorientations_array_name";
  static inline constexpr StringLiteral k_FeatureReferenceCAxisMisorientationsArrayName_Key = "feature_reference_c_axis_misorientations_array_name";

  /**
   * @brief Returns the name of the filter.
   * @return
   */
  std::string name() const override;

  /**
   * @brief Returns the C++ classname of this filter.
   * @return
   */
  std::string className() const override;

  /**
   * @brief Returns the uuid of the filter.
   * @return
   */
  Uuid uuid() const override;

  /**
   * @brief Returns the human readable name of the filter.
   * @return
   */
  std::string humanName() const override;

  /**
   * @brief Returns the default tags for this filter.
   * @return
   */
  std::vector<std::string> defaultTags() const override;

  /**
   * @brief Returns the parameters of the filter (i.e. its inputs)
   * @return
   */
  Parameters parameters() const override;

  /**
   * @brief Returns a copy of the filter.
   * @return
   */
  UniquePointer clone() const override;

protected:
  /**
   * @brief Takes in a DataStructure and checks that the filter can be run on it with the given arguments.
   * Returns any warnings/errors. Also returns the changes that would be applied to the DataStructure.
   * Some parts of the actions may not be completely filled out if all the required information is not available at preflight time.
   * @param ds The input DataStructure instance
   * @param filterArgs These are the input values for each parameter that is required for the filter
   * @param messageHandler The MessageHandler object
   * @return Returns a Result object with error or warning values if any of those occurred during execution of this function
   */
  PreflightResult preflightImpl(const DataStructure& ds, const Arguments& filterArgs, const MessageHandler& messageHandler, const std::atomic_bool& shouldCancel) const override;

  /**
   * @brief Applies the filter's algorithm to the DataStructure with the given arguments. Returns any warnings/errors.
   * On failure, there is no guarantee that the DataStructure is in a correct state.
   * @param ds The input DataStructure instance
   * @param filterArgs These are the input values for each parameter that is required for the filter
   * @param messageHandler The MessageHandler object
   * @return Returns a Result object with error or warning values if any of those occurred during execution of this function
   */
  Result<> executeImpl(DataStructure& data, const Arguments& filterArgs, const PipelineFilter* pipelineNode, const MessageHandler& messageHandler, const std::atomic_bool& shouldCancel) const override;
};
} // namespace complex

COMPLEX_DEF_FILTER_TRAITS(complex, FindFeatureReferenceCAxisMisorientationsFilter, "16c487d2-8f99-4fb5-a4df-d3f70a8e6b25");
/* LEGACY UUID FOR THIS FILTER 1a0848da-2edd-52c0-b111-62a4dc6d2886 */
