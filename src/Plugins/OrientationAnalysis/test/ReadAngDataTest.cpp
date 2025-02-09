#include "OrientationAnalysis/Filters/ReadAngDataFilter.hpp"
#include "OrientationAnalysis/OrientationAnalysis_test_dirs.hpp"

#include "complex/Parameters/FileSystemPathParameter.hpp"
#include "complex/UnitTest/UnitTestCommon.hpp"

#include <catch2/catch.hpp>

#include <filesystem>

namespace fs = std::filesystem;

using namespace complex;
using namespace complex::Constants;
using namespace complex::UnitTest;

TEST_CASE("OrientationAnalysis::ReadAngData: Valid Execution", "[OrientationAnalysis][ReadAngData]")
{
  Application::GetOrCreateInstance()->loadPlugins(unit_test::k_BuildDir.view(), true);

  const complex::UnitTest::TestFileSentinel testDataSentinel(complex::unit_test::k_CMakeExecutable, complex::unit_test::k_TestFilesDir, "6_6_read_ang_data.tar.gz", "6_6_read_ang_data");

  // Read Exemplar DREAM3D File
  auto exemplarFilePath = fs::path(fmt::format("{}/6_6_read_ang_data/6_6_read_ang_data.dream3d", unit_test::k_TestFilesDir));
  DataStructure exemplarDataStructure = LoadDataStructure(exemplarFilePath);

  // Instantiate the filter, a DataStructure object and an Arguments Object
  ReadAngDataFilter filter;
  DataStructure dataStructure;
  Arguments args;

  const fs::path inputAngFile(fmt::format("{}/6_6_read_ang_data/Slice_1.ang", unit_test::k_TestFilesDir));

  // Create default Parameters for the filter.
  args.insertOrAssign(ReadAngDataFilter::k_InputFile_Key, std::make_any<FileSystemPathParameter::ValueType>(inputAngFile));
  args.insertOrAssign(ReadAngDataFilter::k_DataContainerName_Key, std::make_any<DataPath>(k_DataContainerPath));
  args.insertOrAssign(ReadAngDataFilter::k_CellAttributeMatrixName_Key, std::make_any<std::string>(k_CellData));
  args.insertOrAssign(ReadAngDataFilter::k_CellEnsembleAttributeMatrixName_Key, std::make_any<std::string>(k_EnsembleAttributeMatrix));

  // Preflight the filter and check result
  auto preflightResult = filter.preflight(dataStructure, args);
  COMPLEX_RESULT_REQUIRE_VALID(preflightResult.outputActions)

  // Execute the filter and check the result
  auto executeResult = filter.execute(dataStructure, args);
  COMPLEX_RESULT_REQUIRE_VALID(executeResult.result)

  CompareExemplarToGeneratedData(dataStructure, exemplarDataStructure, k_CellAttributeMatrix, k_ExemplarDataContainer);
}
