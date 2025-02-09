#pragma once

#include "complex/Common/Array.hpp"
#include "complex/Common/StringLiteral.hpp"
#include "complex/DataStructure/AttributeMatrix.hpp"
#include "complex/DataStructure/Geometry/IGeometry.hpp"

#include "complex/Common/StringLiteral.hpp"

namespace complex
{
class COMPLEX_EXPORT IGridGeometry : public IGeometry
{
public:
  static inline constexpr StringLiteral k_CellDataName = "Cell Data";
  static inline constexpr StringLiteral k_TypeName = "IGridGeometry";

  IGridGeometry() = delete;
  IGridGeometry(const IGridGeometry&) = default;
  IGridGeometry(IGridGeometry&&) = default;

  IGridGeometry& operator=(const IGridGeometry&) = delete;
  IGridGeometry& operator=(IGridGeometry&&) noexcept = delete;

  ~IGridGeometry() noexcept override = default;

  /**
   * @brief
   * @return SizeVec3
   */
  virtual SizeVec3 getDimensions() const = 0;

  /**
   * @brief
   * @param dims
   */
  virtual void setDimensions(const SizeVec3& dims) = 0;

  /**
   * @brief
   * @return usize
   */
  virtual usize getNumXCells() const = 0;

  /**
   * @brief
   * @return usize
   */
  virtual usize getNumYCells() const = 0;

  /**
   * @brief
   * @return usize
   */
  virtual usize getNumZCells() const = 0;

  /**
   * @brief
   * @param idx
   * @return Point3D<float32>
   */
  virtual Point3D<float32> getPlaneCoordsf(usize idx[3]) const = 0;

  /**
   * @brief
   * @param x
   * @param y
   * @param z
   * @return Point3D<float32>
   */
  virtual Point3D<float32> getPlaneCoordsf(usize x, usize y, usize z) const = 0;

  /**
   * @brief
   * @param idx
   * @return Point3D<float32>
   */
  virtual Point3D<float32> getPlaneCoordsf(usize idx) const = 0;

  /**
   * @brief
   * @param idx
   * @return Point3D<float64>
   */
  virtual Point3D<float64> getPlaneCoords(usize idx[3]) const = 0;

  /**
   * @brief
   * @param x
   * @param y
   * @param z
   * @return Point3D<float64>
   */
  virtual Point3D<float64> getPlaneCoords(usize x, usize y, usize z) const = 0;

  /**
   * @brief
   * @param idx
   * @return Point3D<float64>
   */
  virtual Point3D<float64> getPlaneCoords(usize idx) const = 0;

  /**
   * @brief
   * @param idx
   * @return Point3D<float32>
   */
  virtual Point3D<float32> getCoordsf(usize idx[3]) const = 0;

  /**
   * @brief
   * @param x
   * @param y
   * @param z
   * @preturn Point3D<float32>
   */
  virtual Point3D<float32> getCoordsf(usize x, usize y, usize z) const = 0;

  /**
   * @brief
   * @param idx
   * @return
   */
  virtual Point3D<float32> getCoordsf(usize idx) const = 0;

  /**
   * @brief
   * @param idx
   * @return Point3D<float64>
   */
  virtual Point3D<float64> getCoords(usize idx[3]) const = 0;

  /**
   * @brief
   * @param x
   * @param y
   * @param z
   * @return Point3D<float64>
   */
  virtual Point3D<float64> getCoords(usize x, usize y, usize z) const = 0;

  /**
   * @brief
   * @param idx
   * @return Point3D<float64>
   */
  virtual Point3D<float64> getCoords(usize idx) const = 0;

  /**
   * @brief
   * @param xCoord
   * @param yCoord
   * @param zCoord
   * @return std::optional<usize>
   */
  virtual std::optional<usize> getIndex(float32 xCoord, float32 yCoord, float32 zCoord) const = 0;

  /**
   * @brief
   * @param xCoord
   * @param yCoord
   * @param zCoord
   * @return std::optional<usize>
   */
  virtual std::optional<usize> getIndex(float64 xCoord, float64 yCoord, float64 zCoord) const = 0;

  /**
   * @brief
   * @return
   */
  const std::optional<IdType>& getCellDataId() const;

  /**
   * @brief
   * @return
   */
  AttributeMatrix* getCellData();

  /**
   * @brief
   * @return
   */
  const AttributeMatrix* getCellData() const;

  /**
   * @brief
   * @return
   */
  AttributeMatrix& getCellDataRef();

  /**
   * @brief
   * @return
   */
  const AttributeMatrix& getCellDataRef() const;

  /**
   * @brief
   * @return
   */
  DataPath getCellDataPath() const;

  /**
   * @brief
   * @param attributeMatrix
   */
  void setCellData(const AttributeMatrix& attributeMatrix);

  /**
   * @brief
   * @param id
   */
  void setCellData(OptionalId id);

protected:
  IGridGeometry(DataStructure& dataStructure, std::string name);

  IGridGeometry(DataStructure& dataStructure, std::string name, IdType importId);

  /**
   * @brief Updates the array IDs. Should only be called by DataObject::checkUpdatedIds.
   * @param updatedIds
   */
  void checkUpdatedIdsImpl(const std::vector<std::pair<IdType, IdType>>& updatedIds) override;

  std::optional<IdType> m_CellDataId;
};
} // namespace complex
