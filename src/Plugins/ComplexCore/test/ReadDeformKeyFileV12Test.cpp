#include <catch2/catch.hpp>

#include "complex/Parameters/FileSystemPathParameter.hpp"
#include "complex/UnitTest/UnitTestCommon.hpp"
#include "complex/Utilities/StringUtilities.hpp"

#include "ComplexCore/ComplexCore_test_dirs.hpp"
#include "ComplexCore/Filters/ReadDeformKeyFileV12Filter.hpp"

#include <filesystem>
namespace fs = std::filesystem;

using namespace complex;
using namespace complex::Constants;
using namespace complex::UnitTest;

/**
 * The commented out code in this filter is there to generate a file to test should the need arise down the line.
 */
namespace
{
const DataPath k_QuadGeomPath = DataPath({k_DataContainer});
const DataPath k_CellPath = k_QuadGeomPath.createChildPath(k_CellData);
const DataPath k_VertexPath = k_QuadGeomPath.createChildPath(k_VertexData);

// std::array<size_t, 3> dims = {48, 48, 1};
// std::array<float, 3> spacing = {1.0f, 1.0f, 1.0f};
// std::array<float, 3> origin = {0.0f, 0.0f, 0.0f};
} // namespace

// void MakeNodeFile(std::string nodeFilePath, std::string sectionTitle, int numComp)
//{
//   size_t numNodes = dims[0] * dims[1] * dims[2];
//   FloatVec3 center0(12.0f, 12.0f, 0.0f);
//   FloatVec3 center1(36.0f, 36.0f, 0.0f);
//
//   int index = 1;
//   std::ofstream nodes(nodeFilePath, std::ios_base::out);
//   nodes << fmt::format("{:<8}       1    {}    0.0000000000E+000      {}", sectionTitle, numNodes, numComp) << std::endl;
//   for(size_t z = 0; z < dims[2]; z++)
//   {
//     for(size_t y = 0; y < dims[1]; y++)
//     {
//       for(size_t x = 0; x < dims[1]; x++)
//       {
//         nodes << fmt::format("{:>8}    ", index);
//
//         FloatVec3 point(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
//
//         float r0 = (center0 - point).magnitude();
//         float r1 = (center1 - point).magnitude();
//
//         for(int c = 0; c < numComp; c++)
//         {
//           nodes << fmt::format("{:>17E}   ", std::abs(r0 - r1));
//           if(c % 5 == 0 && c > 0)
//           {
//             nodes << std::endl << "            ";
//           }
//         }
//
//         nodes << std::endl;
//         index++;
//       }
//     }
//   }
// }
//
// void MakeElementFile(std::string nodeFilePath, std::string sectionTitle, int numComp)
//{
//   size_t numElements = (dims[0] - 1) * (dims[1] - 1) * dims[2];
//   FloatVec3 center0(24.0f, 12.0f, 0.0f);
//   FloatVec3 center1(36.0f, 36.0f, 0.0f);
//
//   int index = 1;
//   std::ofstream nodes(nodeFilePath, std::ios_base::out);
//   nodes << fmt::format("{:<8}       1    {}    0.0000000000E+000      {}", sectionTitle, numElements, numComp) << std::endl;
//   for(size_t z = 0; z < dims[2]; z++)
//   {
//     for(size_t y = 0; y < dims[1] - 1; y++)
//     {
//       for(size_t x = 0; x < dims[1] - 1; x++)
//       {
//         nodes << fmt::format("{:>8}    ", index);
//
//         FloatVec3 point(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
//
//         float r0 = (center0 - point).magnitude();
//         float r1 = (center1 - point).magnitude();
//
//         for(int c = 0; c < numComp; c++)
//         {
//           nodes << fmt::format("{:>17E}   ", (c + 1) * std::abs(r0 - r1));
//           if(c % 5 == 0 && c > 0)
//           {
//             nodes << std::endl << "            ";
//           }
//         }
//
//         nodes << std::endl;
//         index++;
//       }
//     }
//   }
// }
//
// TEST_CASE("Generate_Quad_Geom", "")
//{
//   MakeNodeFile("/tmp/USRNOD.txt", "USRNOD", 1);
//   MakeElementFile("/tmp/NDTMP.txt", "DAMAGE", 1);
//
//   size_t numNodes = dims[0] * dims[1] * dims[2];
//   int index = 1;
//   std::ofstream nodes("/tmp/nodes.csv", std::ios_base::out);
//   nodes << fmt::format("RZ           1    {}", numNodes) << std::endl;
//   for(size_t z = 0; z < dims[2]; z++)
//   {
//     for(size_t y = 0; y < dims[1]; y++)
//     {
//       for(size_t x = 0; x < dims[1]; x++)
//       {
//         float xpos = (x * spacing[0] + origin[0]);
//         float ypos = (y * spacing[1] + origin[1]);
//
//         std::string output = fmt::format("{:>8}   {:>17.10E}   {:>17.10E}", index, xpos, ypos);
//         output = StringUtilities::replace(output, "E+", "E+0");
//         nodes << output << std::endl;
//         index++;
//       }
//     }
//   }
//
//   std::ofstream elecon("/tmp/elecon.csv", std::ios_base::out);
//   size_t numElements = (dims[0] - 1) * (dims[1] - 1) * dims[2];
//
//   elecon << fmt::format("ELMCON       1    {}", numElements) << std::endl;
//   index = 1;
//   for(size_t z = 0; z < dims[2]; z++)
//   {
//     for(size_t y = 0; y < dims[1] - 1; y++)
//     {
//       for(size_t x = 0; x < dims[1] - 1; x++)
//       {
//         size_t idx0 = (y * dims[0]) + x;
//         size_t idx1 = (y * dims[0]) + x + 1;
//
//         size_t idx2 = ((y + 1) * dims[0]) + x + 1;
//         size_t idx3 = ((y + 1) * dims[0]) + x;
//         std::string output = fmt::format("{:>8}{:>8}{:>8}{:>8}{:>8}", index, idx0 + 1, idx1 + 1, idx2 + 1, idx3 + 1);
//
//         elecon << output << std::endl;
//         index++;
//       }
//     }
//   }
// }

TEST_CASE("ComplexCore::ReadDeformKeyFileV12: Case 0", "[Core][ReadDeformKeyFileV12]")
{
  const complex::UnitTest::TestFileSentinel testDataSentinel(complex::unit_test::k_CMakeExecutable, complex::unit_test::k_TestFilesDir, "6_6_import_deform_12.tar.gz", "6_6_import_deform_12");
  // Read Exemplar DREAM3D File Filter
  auto exemplarFilePath = fs::path(fmt::format("{}/6_6_import_deform_12/6_6_read_deform_12_0.dream3d", unit_test::k_TestFilesDir));
  DataStructure exemplarDataStructure = LoadDataStructure(exemplarFilePath);

  DataStructure dataStructure;
  {
    // Instantiate the filter and an Arguments Object
    ReadDeformKeyFileV12Filter filter;
    Arguments args;

    // Create default Parameters for the filter.
    args.insertOrAssign(ReadDeformKeyFileV12Filter::k_InputFilePath_Key,
                        std::make_any<FileSystemPathParameter::ValueType>(fs::path(fmt::format("{}/6_6_import_deform_12/deform_v12_0.key", unit_test::k_TestFilesDir))));

    args.insertOrAssign(ReadDeformKeyFileV12Filter::k_QuadGeomPath_Key, std::make_any<DataPath>(k_QuadGeomPath));
    args.insertOrAssign(ReadDeformKeyFileV12Filter::k_VertexAMName_Key, std::make_any<std::string>(k_VertexPath.getTargetName()));
    args.insertOrAssign(ReadDeformKeyFileV12Filter::k_CellAMName_Key, std::make_any<std::string>(k_CellPath.getTargetName()));

    // Preflight the filter and check result
    auto preflightResult = filter.preflight(dataStructure, args);
    COMPLEX_RESULT_REQUIRE_VALID(preflightResult.outputActions)

    // Execute the filter and check the result
    auto executeResult = filter.execute(dataStructure, args);
    COMPLEX_RESULT_REQUIRE_VALID(executeResult.result)
  }

  UnitTest::CompareExemplarToGeneratedData(dataStructure, exemplarDataStructure, k_VertexPath, k_QuadGeomPath.getTargetName());
  UnitTest::CompareExemplarToGeneratedData(dataStructure, exemplarDataStructure, k_CellPath, k_QuadGeomPath.getTargetName());

// Write the DataStructure out to the file system
#ifdef COMPLEX_WRITE_TEST_OUTPUT
  WriteTestDataStructure(dataStructure, fs::path(fmt::format("{}/7_0_import_deform_key_v12_0_test.dream3d", unit_test::k_BinaryTestOutputDir)));
#endif
}

TEST_CASE("ComplexCore::ReadDeformKeyFileV12Filter: Case 1", "[Core][ReadDeformKeyFileV12Filter]")
{
  const complex::UnitTest::TestFileSentinel testDataSentinel(complex::unit_test::k_CMakeExecutable, complex::unit_test::k_TestFilesDir, "6_6_import_deform_12.tar.gz", "6_6_import_deform_12");
  // Read Exemplar DREAM3D File Filter
  auto exemplarFilePath = fs::path(fmt::format("{}/6_6_import_deform_12/6_6_read_deform_12_1.dream3d", unit_test::k_TestFilesDir));
  DataStructure exemplarDataStructure = LoadDataStructure(exemplarFilePath);

  DataStructure dataStructure;
  {
    // Instantiate the filter and an Arguments Object
    ReadDeformKeyFileV12Filter filter;
    Arguments args;

    // Create default Parameters for the filter.
    args.insertOrAssign(ReadDeformKeyFileV12Filter::k_InputFilePath_Key,
                        std::make_any<FileSystemPathParameter::ValueType>(fs::path(fmt::format("{}/6_6_import_deform_12/deform_v12_1.key", unit_test::k_TestFilesDir))));

    args.insertOrAssign(ReadDeformKeyFileV12Filter::k_QuadGeomPath_Key, std::make_any<DataPath>(k_QuadGeomPath));
    args.insertOrAssign(ReadDeformKeyFileV12Filter::k_VertexAMName_Key, std::make_any<std::string>(k_VertexPath.getTargetName()));
    args.insertOrAssign(ReadDeformKeyFileV12Filter::k_CellAMName_Key, std::make_any<std::string>(k_CellPath.getTargetName()));

    // Preflight the filter and check result
    auto preflightResult = filter.preflight(dataStructure, args);
    COMPLEX_RESULT_REQUIRE_VALID(preflightResult.outputActions)

    // Execute the filter and check the result
    auto executeResult = filter.execute(dataStructure, args);
    COMPLEX_RESULT_REQUIRE_VALID(executeResult.result)
  }

  UnitTest::CompareExemplarToGeneratedData(dataStructure, exemplarDataStructure, k_VertexPath, k_QuadGeomPath.getTargetName());
  UnitTest::CompareExemplarToGeneratedData(dataStructure, exemplarDataStructure, k_CellPath, k_QuadGeomPath.getTargetName());

// Write the DataStructure out to the file system
#ifdef COMPLEX_WRITE_TEST_OUTPUT
  WriteTestDataStructure(dataStructure, fs::path(fmt::format("{}/7_0_import_deform_key_v12_1_test.dream3d", unit_test::k_BinaryTestOutputDir)));
#endif
}
