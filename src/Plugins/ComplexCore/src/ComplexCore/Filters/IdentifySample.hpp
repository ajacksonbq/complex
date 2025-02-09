#pragma once

#include "ComplexCore/ComplexCore_export.hpp"

#include "complex/Common/StringLiteral.hpp"
#include "complex/Filter/FilterTraits.hpp"
#include "complex/Filter/IFilter.hpp"

namespace complex
{
class COMPLEXCORE_EXPORT IdentifySample : public IFilter
{
public:
  IdentifySample() = default;
  ~IdentifySample() noexcept override = default;

  IdentifySample(const IdentifySample&) = delete;
  IdentifySample(IdentifySample&&) noexcept = delete;

  IdentifySample& operator=(const IdentifySample&) = delete;
  IdentifySample& operator=(IdentifySample&&) noexcept = delete;

  // Parameter Keys
  static inline constexpr StringLiteral k_FillHoles_Key = "fill_holes";
  static inline constexpr StringLiteral k_ImageGeom_Key = "image_geometry";
  static inline constexpr StringLiteral k_MaskArrayPath_Key = "mask_array_path";

  /**
   * @brief
   * @return
   */
  std::string name() const override;

  /**
   * @brief Returns the C++ classname of this filter.
   * @return
   */
  std::string className() const override;

  /**
   * @brief
   * @return
   */
  Uuid uuid() const override;

  /**
   * @brief
   * @return
   */
  std::string humanName() const override;

  /**
   * @brief Returns the default tags for this filter.
   * @return
   */
  std::vector<std::string> defaultTags() const override;

  /**
   * @brief
   * @return
   */
  Parameters parameters() const override;

  /**
   * @brief
   * @return
   */
  UniquePointer clone() const override;

protected:
  /**
   * @brief
   * @param data
   * @param args
   * @param messageHandler
   * @return PreflightResult
   */
  PreflightResult preflightImpl(const DataStructure& data, const Arguments& args, const MessageHandler& messageHandler, const std::atomic_bool& shouldCancel) const override;

  /**
   * @brief
   * @param data
   * @param args
   * @param pipelineNode
   * @param messageHandler
   * @return Result<>
   */
  Result<> executeImpl(DataStructure& data, const Arguments& args, const PipelineFilter* pipelineNode, const MessageHandler& messageHandler, const std::atomic_bool& shouldCancel) const override;
};
} // namespace complex

COMPLEX_DEF_FILTER_TRAITS(complex, IdentifySample, "94d47495-5a89-4c7f-a0ee-5ff20e6bd273");
