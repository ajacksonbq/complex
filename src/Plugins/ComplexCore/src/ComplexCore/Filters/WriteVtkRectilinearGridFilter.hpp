#pragma once

#include "ComplexCore/ComplexCore_export.hpp"

#include "complex/Filter/FilterTraits.hpp"
#include "complex/Filter/IFilter.hpp"

namespace complex
{
/**
 * @class WriteVtkRectilinearGridFilter
 * @brief This filter will ....
 */
class COMPLEXCORE_EXPORT WriteVtkRectilinearGridFilter : public IFilter
{
public:
  WriteVtkRectilinearGridFilter() = default;
  ~WriteVtkRectilinearGridFilter() noexcept override = default;

  WriteVtkRectilinearGridFilter(const WriteVtkRectilinearGridFilter&) = delete;
  WriteVtkRectilinearGridFilter(WriteVtkRectilinearGridFilter&&) noexcept = delete;

  WriteVtkRectilinearGridFilter& operator=(const WriteVtkRectilinearGridFilter&) = delete;
  WriteVtkRectilinearGridFilter& operator=(WriteVtkRectilinearGridFilter&&) noexcept = delete;

  // Parameter Keys
  static inline constexpr StringLiteral k_OutputFile_Key = "output_file";
  static inline constexpr StringLiteral k_WriteBinaryFile_Key = "write_binary_file";
  static inline constexpr StringLiteral k_ImageGeometryPath_Key = "image_geometry_path";
  static inline constexpr StringLiteral k_SelectedDataArrayPaths_Key = "selected_data_array_paths";

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

COMPLEX_DEF_FILTER_TRAITS(complex, WriteVtkRectilinearGridFilter, "58a4d758-c5ff-43d3-a394-1508ad00a0d2");
/* LEGACY UUID FOR THIS FILTER a043bd66-2681-5126-82e1-5fdc46694bf4 */
