#pragma once

#include "ComplexCore/ComplexCore_export.hpp"

#include "complex/Filter/FilterTraits.hpp"
#include "complex/Filter/IFilter.hpp"

namespace complex
{
/**
 * @class RegularGridSampleSurfaceMeshFilter
 * @brief This filter will ....
 */
class COMPLEXCORE_EXPORT RegularGridSampleSurfaceMeshFilter : public IFilter
{
public:
  RegularGridSampleSurfaceMeshFilter() = default;
  ~RegularGridSampleSurfaceMeshFilter() noexcept override = default;

  RegularGridSampleSurfaceMeshFilter(const RegularGridSampleSurfaceMeshFilter&) = delete;
  RegularGridSampleSurfaceMeshFilter(RegularGridSampleSurfaceMeshFilter&&) noexcept = delete;

  RegularGridSampleSurfaceMeshFilter& operator=(const RegularGridSampleSurfaceMeshFilter&) = delete;
  RegularGridSampleSurfaceMeshFilter& operator=(RegularGridSampleSurfaceMeshFilter&&) noexcept = delete;

  // Parameter Keys
  static inline constexpr StringLiteral k_SurfaceMeshFaceLabelsArrayPath_Key = "surface_mesh_face_labels_array_path";
  static inline constexpr StringLiteral k_Dimensions_Key = "dimensions";
  static inline constexpr StringLiteral k_Spacing_Key = "spacing";
  static inline constexpr StringLiteral k_Origin_Key = "origin";
  static inline constexpr StringLiteral k_LengthUnit_Key = "length_unit";
  static inline constexpr StringLiteral k_ImageGeomPath_Key = "image_geom_path";
  static inline constexpr StringLiteral k_TriangleGeometryPath_Key = "triangle_geometry_path";
  static inline constexpr StringLiteral k_FeatureIdsArrayName_Key = "feature_ids_array_name";
  static inline constexpr StringLiteral k_CellAMName_Key = "cell_attribute_matrix_name";

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

COMPLEX_DEF_FILTER_TRAITS(complex, RegularGridSampleSurfaceMeshFilter, "7d061fe1-7c88-45ef-a7ee-f8ab027c8e85");
/* LEGACY UUID FOR THIS FILTER 0df3da89-9106-538e-b1a9-6bbf1cf0aa92 */
