#pragma once

#include "ParallelAlgorithmUtilities.hpp"
#include "complex/Common/Result.hpp"
#include "complex/Common/Types.hpp"
#include "complex/complex_export.hpp"
#include <complex/Filter/Output.hpp>
#include <complex/Utilities/DataGroupUtilities.hpp>

#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;

namespace complex
{
template <template <class> class ClassT, class ArrayTypeOptions = ArrayUseAllTypes, class... ArgsT>
auto RunTemplateClass(DataType dataType, ArgsT&&... args)
{
  if constexpr(ArrayTypeOptions::UsingBoolean)
  {
    if(dataType == DataType::boolean)
    {
      return ClassT<bool>(std::forward<ArgsT>(args)...)();
    }
  }
  if constexpr(ArrayTypeOptions::UsingInt8)
  {
    if(dataType == DataType::int8)
    {
      return ClassT<int8>(std::forward<ArgsT>(args)...)();
    }
  }
  if constexpr(ArrayTypeOptions::UsingInt16)
  {
    if(dataType == DataType::int16)
    {
      return ClassT<int16>(std::forward<ArgsT>(args)...)();
    }
  }
  if constexpr(ArrayTypeOptions::UsingInt32)
  {
    if(dataType == DataType::int32)
    {
      return ClassT<int32>(std::forward<ArgsT>(args)...)();
    }
  }
  if constexpr(ArrayTypeOptions::UsingInt64)
  {
    if(dataType == DataType::int64)
    {
      return ClassT<int64>(std::forward<ArgsT>(args)...)();
    }
  }
  if constexpr(ArrayTypeOptions::UsingUInt8)
  {
    if(dataType == DataType::uint8)
    {
      return ClassT<uint8>(std::forward<ArgsT>(args)...)();
    }
  }
  if constexpr(ArrayTypeOptions::UsingUInt16)
  {
    if(dataType == DataType::uint16)
    {
      return ClassT<uint16>(std::forward<ArgsT>(args)...)();
    }
  }
  if constexpr(ArrayTypeOptions::UsingUInt32)
  {
    if(dataType == DataType::uint32)
    {
      return ClassT<uint32>(std::forward<ArgsT>(args)...)();
    }
  }
  if constexpr(ArrayTypeOptions::UsingUInt64)
  {
    if(dataType == DataType::uint64)
    {
      return ClassT<uint64>(std::forward<ArgsT>(args)...)();
    }
  }
  if constexpr(ArrayTypeOptions::UsingFloat32)
  {
    if(dataType == DataType::float32)
    {
      return ClassT<float32>(std::forward<ArgsT>(args)...)();
    }
  }
  if constexpr(ArrayTypeOptions::UsingFloat64)
  {
    if(dataType == DataType::float64)
    {
      return ClassT<float64>(std::forward<ArgsT>(args)...)();
    }
  }
}

template <class FuncT, class... ArgsT>
auto ExecuteDataFunction(FuncT&& func, DataType dataType, ArgsT&&... args)
{
  switch(dataType)
  {
  case DataType::boolean: {
    return func.template operator()<bool>(std::forward<ArgsT>(args)...);
  }
  case DataType::int8: {
    return func.template operator()<int8>(std::forward<ArgsT>(args)...);
  }
  case DataType::int16: {
    return func.template operator()<int16>(std::forward<ArgsT>(args)...);
  }
  case DataType::int32: {
    return func.template operator()<int32>(std::forward<ArgsT>(args)...);
  }
  case DataType::int64: {
    return func.template operator()<int64>(std::forward<ArgsT>(args)...);
  }
  case DataType::uint8: {
    return func.template operator()<uint8>(std::forward<ArgsT>(args)...);
  }
  case DataType::uint16: {
    return func.template operator()<uint16>(std::forward<ArgsT>(args)...);
  }
  case DataType::uint32: {
    return func.template operator()<uint32>(std::forward<ArgsT>(args)...);
  }
  case DataType::uint64: {
    return func.template operator()<uint64>(std::forward<ArgsT>(args)...);
  }
  case DataType::float32: {
    return func.template operator()<float32>(std::forward<ArgsT>(args)...);
  }
  case DataType::float64: {
    return func.template operator()<float64>(std::forward<ArgsT>(args)...);
  }
  default: {
    throw std::runtime_error("complex::ExecuteDataFunction<...>(FuncT&& func, DataType dataType, ArgsT&&... args). Error: Invalid DataType");
  }
  }
}

template <class FuncT, class... ArgsT>
auto ExecuteNeighborFunction(FuncT&& func, DataType dataType, ArgsT&&... args)
{
  switch(dataType)
  {
  case DataType::int8: {
    return func.template operator()<int8>(std::forward<ArgsT>(args)...);
  }
  case DataType::int16: {
    return func.template operator()<int16>(std::forward<ArgsT>(args)...);
  }
  case DataType::int32: {
    return func.template operator()<int32>(std::forward<ArgsT>(args)...);
  }
  case DataType::int64: {
    return func.template operator()<int64>(std::forward<ArgsT>(args)...);
  }
  case DataType::uint8: {
    return func.template operator()<uint8>(std::forward<ArgsT>(args)...);
  }
  case DataType::uint16: {
    return func.template operator()<uint16>(std::forward<ArgsT>(args)...);
  }
  case DataType::uint32: {
    return func.template operator()<uint32>(std::forward<ArgsT>(args)...);
  }
  case DataType::uint64: {
    return func.template operator()<uint64>(std::forward<ArgsT>(args)...);
  }
  case DataType::float32: {
    return func.template operator()<float32>(std::forward<ArgsT>(args)...);
  }
  case DataType::float64: {
    return func.template operator()<float64>(std::forward<ArgsT>(args)...);
  }
  case DataType::boolean: {
    throw std::runtime_error("Cannot create a NeighborList of booleans.");
  }
  default: {
    throw std::runtime_error("complex::ExecuteDataFunction<...>(FuncT&& func, DataType dataType, ArgsT&&... args). Error: Invalid DataType");
  }
  }
}

/**
 * @brief Creates all intermediate directories for a given path.
 * @param outputPath The path that gets created
 * @return
 */
COMPLEX_EXPORT Result<> CreateOutputDirectories(const fs::path& outputPath);

/**
 * @brief Creates a delimiters vector from the given delimiters booleans
 * @return
 */
COMPLEX_EXPORT std::vector<char> CreateDelimitersVector(bool tabAsDelimiter, bool semicolonAsDelimiter, bool commaAsDelimiter, bool spaceAsDelimiter);

/**
 * @brief This will create DataModifiedActions for each child of the parent path given
 * @param dataStructure The DataStructure object
 * @param modifiedActions The std::vector<> of DataModifiedAction
 * @param parentPath The parent path
 * @param ignoredDataPaths And specific DataPath to ignore.
 */
COMPLEX_EXPORT void AppendDataObjectModifications(const DataStructure& dataStructure, std::vector<DataObjectModification>& modifiedActions, const DataPath& parentPath,
                                                  const std::vector<DataPath>& ignoredDataPaths);
} // namespace complex
