#include <filesystem>
#include <boost/process.hpp>
#include <crails/cli/cmake_builder.hpp>
#include "project_configuration.hpp"

bool comet_cmake_builder(const ProjectConfiguration& configuration, bool verbose, bool clean)
{
  int options = 0;

  if (verbose)
    options += CMakeVerbose;
  if (clean)
    options += CMakeClean;
  return CMakeBuilder(
    configuration.project_directory(),
    configuration.application_build_path(),
    options
  ).option("CMAKE_TOOLCHAIN_FILE", configuration.variable("cheerp-path") + "/share/cmake/Modules/CheerpToolchain.cmake")
   .option("CMAKE_BUILD_TYPE", configuration.build_type())
   .build();
}
