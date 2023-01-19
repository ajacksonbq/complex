#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>

#include "fmt/format.h"

#include "nlohmann/json.hpp"

#include "PRObserver.hpp"
#include "complex/Core/Application.hpp"
#include "complex/Pipeline/Pipeline.hpp"

namespace fs = std::filesystem;
using namespace complex;

#if 0
#define TEST_PIPELINE
#else
#undef TEST_PIPELINE
#endif

void LoadApp(complex::Application& app)
{
#if defined(__APPLE__)
  {
    fs::path appPath = app.getCurrentDir();
    app.loadPlugins(appPath, true);
    appPath = appPath.parent_path();

    // Check if there is a Plugins Folder inside the app package
    if(fs::exists(appPath / "Plugins"))
    {
      appPath = appPath / "Plugins";
      app.loadPlugins(appPath, true);
    }
    else // Climb out of the app package and look in the build directory
    {
      appPath = appPath.parent_path().parent_path();
      app.loadPlugins(appPath, true);
    }
  }
#elif defined(_MSC_VER)
#else
  fs::path appPath = app.getCurrentDir();
  appPath = appPath.parent_path();

  // Check if there is a Plugins Folder inside the app package
  if(fs::exists(appPath / "Plugins"))
  {
    appPath = appPath / "Plugins";
    app.loadPlugins(appPath, true);
  }
#endif
  app.loadPlugins(app.getCurrentDir(), true);
}

#ifdef TEST_PIPELINE
Pipeline createTestPipeline()
{
  Uuid pluginId = Uuid::FromString("05cc618b-781f-4ac0-b9ac-43f26ce1854f").value();

  FilterHandle createGroupHandle(Uuid::FromString("e7d2f9b8-4131-4b28-a843-ea3c6950f101").value(), pluginId);
  FilterHandle createArrayHandle(Uuid::FromString("67041f9b-bdc6-4122-acc6-c9fe9280e90d").value(), pluginId);

  Arguments groupArgs;
  groupArgs.insert("Data_Object_Path", DataPath({"Foo"}));

  Arguments arrayArgs;
  arrayArgs.insert("numeric_type", NumericType::float32);
  arrayArgs.insert("component_count", std::make_any<uint64>(3));
  arrayArgs.insert("tuple_count", std::make_any<uint64>(5));
  arrayArgs.insert("output_data_array", DataPath({"Foo", "Bar"}));

  Pipeline pipeline;
  pipeline.push_back(createGroupHandle, groupArgs);
  pipeline.push_back(createArrayHandle, arrayArgs);

  return pipeline;
}
#endif

bool ShouldPreflight(int argc, char* argv[])
{
  if(argc < 3)
  {
    return false;
  }

  const std::string arg(argv[2]);
  if(arg == "-p" || arg == "--preflight")
  {
    return true;
  }
  return false;
}

int PreflightPipeline(Pipeline& pipeline)
{
  const PipelineRunner::PipelineObserver obs(&pipeline);
  std::cout << "\n-------------------------" << std::endl;

  if(!pipeline.preflight())
  {
    std::cout << "Error preflighting pipeline" << std::endl;
    return -2;
  }

  std::cout << "Finished preflighting pipeline" << std::endl;
  return 0;
}

bool PrintPipelineLoadMessages(const fs::path& pipelinePath, const Result<Pipeline>& result)
{
  if(result.invalid())
  {
    std::cout << fmt::format("Could not load pipeline at path: '{}'", pipelinePath.string()) << std::endl;
    for(const auto& warning : result.warnings())
    {
      std::cout << fmt::format("Warning {}: {}", warning.code, warning.message) << std::endl;
    }
    for(const auto& error : result.errors())
    {
      std::cout << fmt::format("Error {}: {}", error.code, error.message) << std::endl;
    }
    return false;
  }
  return true;
}

int PreflightPipelinePath(const fs::path& pipelinePath)
{
  auto result = Pipeline::FromFile(pipelinePath);
  if(!PrintPipelineLoadMessages(pipelinePath, result))
  {
    return -1;
  }

  std::cout << fmt::format("Preflighting pipeline at path: '{}'\n", pipelinePath.string()) << std::endl;

  Pipeline pipeline = result.value();
  return PreflightPipeline(pipeline);
}

int ExecutePipeline(Pipeline& pipeline)
{
  const PipelineRunner::PipelineObserver obs(&pipeline);
  std::cout << "\n-------------------------" << std::endl;

  if(!pipeline.execute())
  {
    std::cout << "Error executing pipeline" << std::endl;
    return -2;
  }
  std::cout << "Finished executing pipeline" << std::endl;
  return 0;
}

int ExecutePipelinePath(const fs::path& pipelinePath)
{
  auto result = Pipeline::FromFile(pipelinePath);
  if(!PrintPipelineLoadMessages(pipelinePath, result))
  {
    return -1;
  }

  std::cout << fmt::format("Executing pipeline at path: '{}'\n", pipelinePath.string()) << std::endl;

  Pipeline pipeline = result.value();
  return ExecutePipeline(pipeline);
}

int main(int argc, char* argv[])
{
  std::cout << "PipelineRunner Version 7" << std::endl;
  complex::Application app;
  LoadApp(app);

#ifdef TEST_PIPELINE
  Pipeline pipeline = createTestPipeline();
  return executePipeline(pipeline);
#endif
  if(argc < 2)
  {
    std::cout << "PipelineRunner requires a filepath to run" << std::endl;
    return 0;
  }

  const fs::path targetPath = argv[1];
  if(!fs::exists(targetPath))
  {
    fmt::print("Input file does not exist.\n  '{}'\n", targetPath.string());
    return -1;
  }

  if(ShouldPreflight(argc, argv))
  {
    return PreflightPipelinePath(targetPath);
  }
  return ExecutePipelinePath(targetPath);
}
