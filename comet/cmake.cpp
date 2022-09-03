#include <filesystem>
#include <boost/process.hpp>
#include <crails/cli/cmake_builder.hpp>
#include <crails/cli/build2_builder.hpp>
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
  boost::process::ipstream stream;
  boost::process::child process(path + " --version");
  std::string line;
  std::vector<std::string> parts;

  process.wait();
  if (process.exit_code() == 0)
  {
    getline(stream, line);
    parts = Crails::split<std::string, std::vector<std::string>>(line, ' ');
    if (parts.size() > 4)
      return parts[4];
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
