#pragma once

#include "ComplexCore/ComplexCore_export.hpp"

#include "complex/DataStructure/DataPath.hpp"
#include "complex/DataStructure/DataStructure.hpp"
#include "complex/Filter/IFilter.hpp"
#include "complex/Parameters/DataGroupSelectionParameter.hpp"

namespace complex
{

struct COMPLEXCORE_EXPORT NearestPointFuseRegularGridsInputValues
{
  DataPath ReferenceGeometryPath;
  DataPath SamplingGeometryPath;
  DataPath ReferenceCellAttributeMatrixPath;
  DataPath SamplingCellAttributeMatrixPath;
  float64 fillValue;
};

/**
 * @class ConditionalSetValue
 * @brief This filter replaces values in the target array with a user specified value
 * where a bool mask array specifies.
 */

class COMPLEXCORE_EXPORT NearestPointFuseRegularGrids
{
public:
  NearestPointFuseRegularGrids(DataStructure& dataStructure, const IFilter::MessageHandler& mesgHandler, const std::atomic_bool& shouldCancel, NearestPointFuseRegularGridsInputValues* inputValues);
  ~NearestPointFuseRegularGrids() noexcept;

  NearestPointFuseRegularGrids(const NearestPointFuseRegularGrids&) = delete;
  NearestPointFuseRegularGrids(NearestPointFuseRegularGrids&&) noexcept = delete;
  NearestPointFuseRegularGrids& operator=(const NearestPointFuseRegularGrids&) = delete;
  NearestPointFuseRegularGrids& operator=(NearestPointFuseRegularGrids&&) noexcept = delete;

  Result<> operator()();

  const std::atomic_bool& getCancel();

private:
  DataStructure& m_DataStructure;
  const NearestPointFuseRegularGridsInputValues* m_InputValues = nullptr;
  const std::atomic_bool& m_ShouldCancel;
  const IFilter::MessageHandler& m_MessageHandler;
};

} // namespace complex
