#include <filesystem>
#include <boost/process.hpp>
#include <crails/cli/with_path.hpp>
#include "project_configuration.hpp"

class CMakeBuilder : private Crails::WithPath
{
  std::filesystem::path project_directory;
  std::stringstream options;
public:
  CMakeBuilder(const std::filesystem::path& project_directory, const std::filesystem::path& build_directory) :
    Crails::WithPath(build_directory),
    project_directory(project_directory)
  {
  }

  CMakeBuilder& option(const std::string& name, const std::string& value)
  {
    options << "-D" << name << '=' << value << ' ';
    return *this;
  }

  bool configure()
  {
    boost::process::child cmake("cmake " + options.str() + project_directory.string());

    cmake.wait();
    return cmake.exit_code() == 0;
  }

  bool make()
  {
    boost::process::child make("make");

    make.wait();
    return make.exit_code() == 0;
  }

  bool build()
  {
    return configure() && make();
  }
};

bool comet_cmake_builder(const ProjectConfiguration& configuration)
{
  return CMakeBuilder(
    configuration.project_directory(),
    configuration.application_build_path()
  ).option("CMAKE_TOOLCHAIN_FILE", configuration.variable("cheerp-path") + "/share/cmake/Modules/CheerpToolchain.cmake")
   .option("CMAKE_BUILD_TYPE", configuration.build_type())
   .build();
}
