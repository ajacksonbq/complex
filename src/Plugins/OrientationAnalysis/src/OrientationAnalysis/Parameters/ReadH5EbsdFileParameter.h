#pragma once

#include "complex/Filter/ParameterTraits.hpp"
#include "complex/Filter/ValueParameter.hpp"
#include "complex/Utilities/FilePathGenerator.hpp"
#include "complex/complex_export.hpp"

#include "EbsdLib/Core/EbsdLibConstants.h"

#include <string>

namespace complex
{
class COMPLEX_EXPORT ReadH5EbsdFileParameter : public ValueParameter
{

public:
  /**
   * @brief This struct holds all of the data necessary to generate a list of file paths.
   */
  struct ValueType
  {
    std::string inputFilePath;
    int32 startSlice = 0;
    int32 endSlice = 0;
    int32 eulerRepresentation = EbsdLib::AngleRepresentation::Radians;
    std::vector<std::string> selectedArrayNames = {};
    bool useRecommendedTransform = {true};
  };

  ReadH5EbsdFileParameter() = delete;

  /**
   * @brief Constructor
   * @param name internal string key for the parameter
   * @param humanName Human facing string for the parameter
   * @param helpText The help text that should be displayed to a user
   * @param defaultValue The default value for the parameter
   */
  ReadH5EbsdFileParameter(const std::string& name, const std::string& humanName, const std::string& helpText, const ValueType& defaultValue);

  ~ReadH5EbsdFileParameter() override = default;

  ReadH5EbsdFileParameter(const ReadH5EbsdFileParameter&) = delete;
  ReadH5EbsdFileParameter(ReadH5EbsdFileParameter&&) noexcept = delete;
  ReadH5EbsdFileParameter& operator=(const ReadH5EbsdFileParameter&) = delete;
  ReadH5EbsdFileParameter& operator=(ReadH5EbsdFileParameter&&) noexcept = delete;

  /**
   * @brief
   * @return
   */
  Uuid uuid() const override;

  /**
   * @brief
   * @return
   */
  AcceptedTypes acceptedTypes() const override;

  /**
   * @brief
   * @param value
   */
  nlohmann::json toJson(const std::any& value) const override;

  /**
   * @brief
   * @return
   */
  Result<std::any> fromJson(const nlohmann::json& json) const override;

  /**
   * @brief
   * @return
   */
  UniquePointer clone() const override;

  /**
   * @brief
   * @return
   */
  std::any defaultValue() const override;

  /**
   * @brief
   * @param value
   * @return
   */
  Result<> validate(const std::any& value) const override;

private:
  ValueType m_DefaultValue = {};
};
} // namespace complex

COMPLEX_DEF_PARAMETER_TRAITS(complex::ReadH5EbsdFileParameter, "FAC15aa6-b367-508e-bf73-94ab6be0058b");
