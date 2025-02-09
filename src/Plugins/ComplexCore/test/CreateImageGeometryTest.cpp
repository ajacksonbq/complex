#include "ComplexCore/Filters/CreateImageGeometry.hpp"

#include "complex/Parameters/VectorParameter.hpp"
#include "complex/UnitTest/UnitTestCommon.hpp"
#include "complex/unit_test/complex_test_dirs.hpp"

#include <catch2/catch.hpp>

using namespace complex;
using namespace complex::Constants;

namespace fs = std::filesystem;

namespace CreateImageGeometryUnitTest
{

const fs::path k_DataDir = "test/data";
const fs::path k_TestFile = "CreateImageGeometry_Test.dream3d";

} // namespace CreateImageGeometryUnitTest

TEST_CASE("ComplexCore::CreateImageGeometry", "[ComplexCore]")
{
  // Instantiate the filter, a DataStructure object and an Arguments Object

  std::vector<usize> imageDims = {40, 60, 80};
  std::vector<float32> imageSpacing = {0.10F, 2.0F, 33.0F};
  std::vector<float32> imageOrigin = {0.0F, 22.0F, 77.0F};

  VectorUInt64Parameter::ValueType inputDims = {imageDims[0], imageDims[1], imageDims[2]};

  DataStructure dataStructure = complex::UnitTest::CreateAllPrimitiveTypes(imageDims);
  //  DataGroup* levelZeroGroup = DataGroup::Create(dataStructure, Complexk_LevelZero);
  //  DataGroup* levelOneGroup = DataGroup::Create(dataStructure, Complexk_LevelOne, levelZeroGroup->getId());
  const std::string k_ImageGeometryName("[Image Geometry]");
  DataPath selectedDataGroupPath({k_LevelZero, k_LevelOne, k_ImageGeometryName});
  Arguments args;
  // Create default Parameters for the filter.
  args.insertOrAssign(CreateImageGeometry::k_GeometryDataPath_Key, std::make_any<DataPath>(selectedDataGroupPath));
  args.insertOrAssign(CreateImageGeometry::k_Dimensions_Key, std::make_any<std::vector<uint64_t>>(inputDims));
  args.insertOrAssign(CreateImageGeometry::k_Origin_Key, std::make_any<VectorFloat32Parameter::ValueType>(imageOrigin));
  args.insertOrAssign(CreateImageGeometry::k_Spacing_Key, std::make_any<VectorFloat32Parameter::ValueType>(imageSpacing));

  CreateImageGeometry filter;

  // Preflight the filter and check result
  auto preflightResult = filter.preflight(dataStructure, args);
  REQUIRE(preflightResult.outputActions.valid());

  // Execute the filter and check the result
  auto executeResult = filter.execute(dataStructure, args);
  REQUIRE(executeResult.result.valid());

  // Execute the filter and check the result
  executeResult = filter.execute(dataStructure, args);
  REQUIRE(executeResult.result.valid() == false);

#if 0
  fs::path filePath = COMPLEX_BUILD_DIR / CreateImageGeometryUnitTest::k_DataDir / CreateImageGeometryUnitTest::k_TestFile;

  H5::FileWriter::ResultType result = H5::FileWriter::CreateFile(filePath);
  REQUIRE(result.valid());

  H5::FileWriter& fileWriter = *(result.value());
  REQUIRE(fileWriter.isValid());

  herr_t err;
  err = dataStructure.writeHdf5(fileWriter);
  REQUIRE(err >= 0);
#endif
}
