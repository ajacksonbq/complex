#include <catch2/catch.hpp>

#include "ITKImageProcessing/Filters/ITKImportFijiMontageFilter.hpp"
#include "ITKImageProcessing/ITKImageProcessing_test_dirs.hpp"

#include "complex/DataStructure/DataArray.hpp"
#include "complex/Parameters/ChoicesParameter.hpp"
#include "complex/Parameters/FileSystemPathParameter.hpp"
#include "complex/Parameters/VectorParameter.hpp"
#include "complex/UnitTest/UnitTestCommon.hpp"
#include "complex/Utilities/DataGroupUtilities.hpp"

#include <filesystem>
#include <sstream>

using namespace complex;

namespace fs = std::filesystem;

namespace
{
const std::string k_SmallZeissZenDir = fmt::format("{}/fiji_montage/small_zeiss_zen", unit_test::k_TestFilesDir);

const std::string k_DataGroupName = "Zen DataGroup";
const DataPath k_DataGroupPath = {{k_DataGroupName}};
} // namespace

TEST_CASE("ITKImageProcessing::ITKImportFijiMontage: Basic 2x2 Grid Montage", "[ITKImageProcessing][ITKImportFijiMontage]")
{
  const complex::UnitTest::TestFileSentinel testDataSentinel(complex::unit_test::k_CMakeExecutable, complex::unit_test::k_TestFilesDir, "fiji_montage.tar.gz", "fiji_montage");

  auto app = Application::GetOrCreateInstance();
  app->loadPlugins(unit_test::k_BuildDir.view(), true);

  DataStructure exemplarDataStructure = UnitTest::LoadDataStructure(fs::path(fmt::format("{}/fiji_montage/2x2_fiji_montage_test.dream3d", unit_test::k_TestFilesDir)));

  // Instantiate the filter, a DataStructure object and an Arguments Object
  ITKImportFijiMontageFilter filter;
  DataStructure dataStructure;
  Arguments args;

  std::stringstream path;
  path << k_SmallZeissZenDir << "/TileConfiguration.registered.txt";
  const fs::path smallInputFile = fs::path(path.str());

  args.insertOrAssign(ITKImportFijiMontageFilter::k_InputFile_Key, std::make_any<FileSystemPathParameter::ValueType>(smallInputFile));
  args.insertOrAssign(ITKImportFijiMontageFilter::k_DataGroupName_Key, std::make_any<std::string>(k_DataGroupName));
  args.insertOrAssign(ITKImportFijiMontageFilter::k_LengthUnit_Key, std::make_any<ChoicesParameter::ValueType>(to_underlying(IGeometry::LengthUnit::Micrometer)));
  args.insertOrAssign(ITKImportFijiMontageFilter::k_ChangeOrigin_Key, std::make_any<bool>(false));
  args.insertOrAssign(ITKImportFijiMontageFilter::k_ConvertToGrayScale_Key, std::make_any<bool>(false));
  args.insertOrAssign(ITKImportFijiMontageFilter::k_ParentDataGroup_Key, std::make_any<bool>(true));
  // args.insertOrAssign(ITKImportFijiMontageFilter::k_ColorWeights_Key, std::make_any<VectorFloat32Parameter::ValueType>({0.2125f, 0.7154f, 0.0721f}));
  args.insertOrAssign(ITKImportFijiMontageFilter::k_DataContainerPath_Key, std::make_any<std::string>("Mosaic-"));
  args.insertOrAssign(ITKImportFijiMontageFilter::k_CellAttributeMatrixName_Key, std::make_any<std::string>("Tile Data"));
  args.insertOrAssign(ITKImportFijiMontageFilter::k_ImageDataArrayName_Key, std::make_any<std::string>("Image"));

  // Preflight the filter and check result
  auto preflightResult = filter.preflight(dataStructure, args);
  COMPLEX_RESULT_REQUIRE_VALID(preflightResult.outputActions);

  // Execute the filter and check the result
  auto executeResult = filter.execute(dataStructure, args);
  COMPLEX_RESULT_REQUIRE_VALID(executeResult.result);

  std::vector<DataPath> generatedGroup = GetAllChildDataPaths(dataStructure, k_DataGroupPath, DataObject::Type::ImageGeom).value();
  std::vector<DataPath> exemplarGroup = GetAllChildDataPaths(exemplarDataStructure, k_DataGroupPath, DataObject::Type::ImageGeom).value();
  REQUIRE(generatedGroup.size() == exemplarGroup.size());
  for(usize i = 0; i < generatedGroup.size(); i++)
  {
    UnitTest::CompareImageGeometry(exemplarDataStructure.getDataAs<ImageGeom>(exemplarGroup[i]), dataStructure.getDataAs<ImageGeom>(generatedGroup[i]));
  }
}
