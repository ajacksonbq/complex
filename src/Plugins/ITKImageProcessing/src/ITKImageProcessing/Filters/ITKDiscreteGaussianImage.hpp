#pragma once

#include "ITKImageProcessing/ITKImageProcessing_export.hpp"

#include "complex/Filter/FilterTraits.hpp"
#include "complex/Filter/IFilter.hpp"

namespace complex
{
/**
 * @class ITKDiscreteGaussianImage
 * @brief Blurs an image by separable convolution with discrete gaussian kernels. This filter performs Gaussian blurring by separable convolution of an image and a discrete Gaussian operator (kernel).
 *
 * The Gaussian operator used here was described by Tony Lindeberg (Discrete Scale-Space Theory and the Scale-Space Primal Sketch. Dissertation. Royal Institute of Technology, Stockholm, Sweden. May
 * 1991.) The Gaussian kernel used here was designed so that smoothing and derivative operations commute after discretization.
 *
 * The variance or standard deviation (sigma) will be evaluated as pixel units if SetUseImageSpacing is off (false) or as physical units if SetUseImageSpacing is on (true, default). The variance can
 * be set independently in each dimension.
 *
 * When the Gaussian kernel is small, this filter tends to run faster than itk::RecursiveGaussianImageFilter .
 *
 * @see GaussianOperator
 *
 *
 * @see Image
 *
 *
 * @see Neighborhood
 *
 *
 * @see NeighborhoodOperator
 *
 *
 * @see RecursiveGaussianImageFilter
 *
 * ITK Module: ITKSmoothing
 * ITK Group: Smoothing
 */
class ITKIMAGEPROCESSING_EXPORT ITKDiscreteGaussianImage : public IFilter
{
public:
  ITKDiscreteGaussianImage() = default;
  ~ITKDiscreteGaussianImage() noexcept override = default;

  ITKDiscreteGaussianImage(const ITKDiscreteGaussianImage&) = delete;
  ITKDiscreteGaussianImage(ITKDiscreteGaussianImage&&) noexcept = delete;

  ITKDiscreteGaussianImage& operator=(const ITKDiscreteGaussianImage&) = delete;
  ITKDiscreteGaussianImage& operator=(ITKDiscreteGaussianImage&&) noexcept = delete;

  // Parameter Keys
  static inline constexpr StringLiteral k_SelectedImageGeomPath_Key = "selected_image_geom_path";
  static inline constexpr StringLiteral k_SelectedImageDataPath_Key = "input_image_data_path";
  static inline constexpr StringLiteral k_OutputImageDataPath_Key = "output_image_data_path";
  static inline constexpr StringLiteral k_Variance_Key = "variance";
  static inline constexpr StringLiteral k_MaximumKernelWidth_Key = "maximum_kernel_width";
  static inline constexpr StringLiteral k_MaximumError_Key = "maximum_error";
  static inline constexpr StringLiteral k_UseImageSpacing_Key = "use_image_spacing";

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
   * @param shouldCancel Boolean that gets set if the filter should stop executing and return
   * @return Returns a Result object with error or warning values if any of those occurred during execution of this function
   */
  PreflightResult preflightImpl(const DataStructure& dataStructure, const Arguments& filterArgs, const MessageHandler& messageHandler, const std::atomic_bool& shouldCancel) const override;

  /**
   * @brief Applies the filter's algorithm to the DataStructure with the given arguments. Returns any warnings/errors.
   * On failure, there is no guarantee that the DataStructure is in a correct state.
   * @param dataStructure The input DataStructure instance
   * @param filterArgs These are the input values for each parameter that is required for the filter
   * @param messageHandler The MessageHandler object
   * @param shouldCancel Boolean that gets set if the filter should stop executing and return
   * @return Returns a Result object with error or warning values if any of those occurred during execution of this function
   */
  Result<> executeImpl(DataStructure& dataStructure, const Arguments& filterArgs, const PipelineFilter* pipelineNode, const MessageHandler& messageHandler,
                       const std::atomic_bool& shouldCancel) const override;
};
} // namespace complex

COMPLEX_DEF_FILTER_TRAITS(complex, ITKDiscreteGaussianImage, "025edc1a-986d-4005-92d1-545dfdc13abd");
