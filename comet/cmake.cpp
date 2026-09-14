#include <filesystem>
#include <boost/process.hpp>
#include <crails/cli/cmake_builder.hpp>
#include <crails/cli/build2_builder.hpp>
#include <crails/cli/process.hpp>
#include <crails/utils/split.hpp>
#include "project_configuration.hpp"
#include <iostream>

using namespace std;

bool comet_cmake_builder(const ProjectConfiguration& configuration, bool verbose, bool clean)
{
  if (CMakeBuilder::installed())
  {
    int options = 0;

    if (verbose)
      options += BuildVerbose;
    if (clean)
      options += BuildClean;
    return CMakeBuilder(
      configuration.project_directory(),
      configuration.application_build_path(),
      options
    ).option("CMAKE_TOOLCHAIN_FILE", configuration.variable("cheerp-path") + "/share/cmake/Modules/CheerpToolchain.cmake")
    .option("CMAKE_BUILD_TYPE", configuration.build_type())
    .build();
  }
  else
    cerr << "cmake does not appear to be installed." << endl;
  return false;
}

std::string get_cheerp_clang_version(const std::string& path)
{
  Crails::ExecutableCommand command({path, {"--version"}});
  std::string output;

  if (Crails::run_command(command, output))
  {
    std::vector<std::string_view> lines = Crails::split<std::string_view, std::vector<std::string_view>>(std::string_view(output), '\n');
    std::vector<std::string_view> parts = Crails::split<std::string_view, std::vector<std::string_view>>(lines[0], ' ');
    if (parts.size() > 2)
      return std::string(parts[2]);
    else
      std::cerr << "Could not retrieve cheerp's clang++ version." << std::endl;
  }
  return "";
}

bool comet_build2_builder(const ProjectConfiguration& configuration, bool verbose, bool clean)
{
  int options = 0;

  if (verbose)
    options += BuildVerbose;
  if (clean)
    options += BuildClean;
  if (Build2Builder::installed())
  {
    Build2Builder build2(
      configuration.variable_or("name", "application"),
      configuration.project_directory(),
      configuration.application_build_path(),
      options
    );

    if (clean)
      std::filesystem::remove_all(configuration.application_build_path());
    if (!std::filesystem::is_directory(configuration.application_build_path()))
    {
      std::string compiler = configuration.variable("cheerp-path") + "/bin/clang++";
      std::string compiler_version = get_cheerp_clang_version(compiler);

      Build2Builder::create(configuration.application_build_path(), {
        {"config.cxx", configuration.variable("cheerp-path") + "/bin/g++"},
        {"config.cxx.version", compiler_version},
        {"config.c.version", compiler_version},
        {"config.poptions", "--target=cheerp-genericjs -D__CHEERP_CLIENT__"},
        {"config.loptions", "--target=cheerp-genericjs"}
      });
    }
    return build2.configure() && build2.build();
  }
  else
    cerr << "bpkg does not appear to be installed." << endl;
  return false;
}
