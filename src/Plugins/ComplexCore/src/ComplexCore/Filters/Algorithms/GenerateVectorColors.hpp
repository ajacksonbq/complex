#pragma once

#include "ComplexCore/ComplexCore_export.hpp"

#include "complex/DataStructure/DataPath.hpp"
#include "complex/DataStructure/DataStructure.hpp"
#include "complex/Filter/IFilter.hpp"
#include "complex/Parameters/ArrayCreationParameter.hpp"
#include "complex/Parameters/ArraySelectionParameter.hpp"

namespace complex
{

struct COMPLEXCORE_EXPORT GenerateVectorColorsInputValues
{
  bool UseMask;
  DataPath VectorsArrayPath;
  DataPath MaskArrayPath;
  DataPath CellVectorColorsArrayPath;
};

/**
 * @class ConditionalSetValue
 * @brief This filter replaces values in the target array with a user specified value
 * where a bool mask array specifies.
 */

class COMPLEXCORE_EXPORT GenerateVectorColors
{
public:
  GenerateVectorColors(DataStructure& dataStructure, const IFilter::MessageHandler& mesgHandler, const std::atomic_bool& shouldCancel, GenerateVectorColorsInputValues* inputValues);
  ~GenerateVectorColors() noexcept;

  GenerateVectorColors(const GenerateVectorColors&) = delete;
  GenerateVectorColors(GenerateVectorColors&&) noexcept = delete;
  GenerateVectorColors& operator=(const GenerateVectorColors&) = delete;
  GenerateVectorColors& operator=(GenerateVectorColors&&) noexcept = delete;

  Result<> operator()();

  const std::atomic_bool& getCancel();

private:
  DataStructure& m_DataStructure;
  const GenerateVectorColorsInputValues* m_InputValues = nullptr;
  const std::atomic_bool& m_ShouldCancel;
  const IFilter::MessageHandler& m_MessageHandler;
};

} // namespace complex
