#include "build.hpp"
#include <crails/cli/process.hpp>
#include <filesystem>
#include <iostream>

using namespace std;

bool comet_cmake_builder(const ProjectConfiguration& configuration, bool verbose, bool clean);
bool comet_build2_builder(const ProjectConfiguration& configuration, bool verbose, bool clean);

bool Build::generate_html_elements()
{
  string template_folder = "views";

  if (filesystem::is_directory(template_folder))
  {
    Crails::ExecutableCommand command;

    command.path = ProjectConfiguration::comet_bin_path() + "/comet-html";
    command.arguments = {
      "-i", template_folder,
      "-o", (configuration.variable("local-libdir") + "/templates"),
      "-c", configuration.variable("html-config")
    };
    if (options.count("verbose"))
      cout << "+ " << command << endl;
    return Crails::run_command(command);
  }
  return true;
}

int Build::run()
{
  ProjectConfiguration::move_to_project_directory();

  configuration.initialize();
  if (options.count("mode"))
    configuration.variable("build-type", options["mode"].as<string>());
  if (!generate_html_elements())
    return 2;
  if (configuration.variable("toolchain") == "cmake")
    return comet_cmake_builder(configuration, options.count("verbose"), options.count("clean")) ? 0 : 1;
  else if (configuration.variable("toolchain") == "build2")
    return comet_build2_builder(configuration, options.count("verbose"), options.count("clean")) ? 0 : 1;
  else
    cerr << "Cannot build with toolchain `" << configuration.variable("toolchain") << '`' << endl;
  return -1;
}
