#pragma once

#include "ComplexCore/ComplexCore_export.hpp"

#include "complex/Filter/FilterTraits.hpp"
#include "complex/Filter/IFilter.hpp"

namespace complex
{
/**
 * @class AlignSectionsFeatureCentroidFilter
 *
 * @brief This filter will ....
 */
class COMPLEXCORE_EXPORT AlignSectionsFeatureCentroidFilter : public IFilter
{
public:
  AlignSectionsFeatureCentroidFilter() = default;
  ~AlignSectionsFeatureCentroidFilter() noexcept override = default;

  AlignSectionsFeatureCentroidFilter(const AlignSectionsFeatureCentroidFilter&) = delete;
  AlignSectionsFeatureCentroidFilter(AlignSectionsFeatureCentroidFilter&&) noexcept = delete;

  AlignSectionsFeatureCentroidFilter& operator=(const AlignSectionsFeatureCentroidFilter&) = delete;
  AlignSectionsFeatureCentroidFilter& operator=(AlignSectionsFeatureCentroidFilter&&) noexcept = delete;

  // Parameter Keys
  static inline constexpr StringLiteral k_WriteAlignmentShifts_Key = "write_alignment_shifts";
  static inline constexpr StringLiteral k_AlignmentShiftFileName_Key = "alignment_shift_file_name";
  static inline constexpr StringLiteral k_UseReferenceSlice_Key = "use_reference_slice";
  static inline constexpr StringLiteral k_ReferenceSlice_Key = "reference_slice";
  static inline constexpr StringLiteral k_MaskArrayPath_Key = "mask_array_path";
  static inline constexpr StringLiteral k_SelectedImageGeometry_Key = "selected_image_geometry_path";
  static inline constexpr StringLiteral k_SelectedCellDataGroup_Key = "selected_cell_data_path";

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
   * @param dataStructure The input DataStructure instance
   * @param filterArgs These are the input values for each parameter that is required for the filter
   * @param messageHandler The MessageHandler object
   * @return Returns a Result object with error or warning values if any of those occurred during execution of this function
   */
  PreflightResult preflightImpl(const DataStructure& dataStructure, const Arguments& filterArgs, const MessageHandler& messageHandler, const std::atomic_bool& shouldCancel) const override;

  /**
   * @brief Applies the filter's algorithm to the DataStructure with the given arguments. Returns any warnings/errors.
   * On failure, there is no guarantee that the DataStructure is in a correct state.
   * @param dataStructure The input DataStructure instance
   * @param filterArgs These are the input values for each parameter that is required for the filter
   * @param messageHandler The MessageHandler object
   * @return Returns a Result object with error or warning values if any of those occurred during execution of this function
   */
  Result<> executeImpl(DataStructure& data, const Arguments& filterArgs, const PipelineFilter* pipelineNode, const MessageHandler& messageHandler, const std::atomic_bool& shouldCancel) const override;
};
} // namespace complex

COMPLEX_DEF_FILTER_TRAITS(complex, AlignSectionsFeatureCentroidFilter, "b83f9bae-9ccf-4932-96c3-7f2fdb091452");
