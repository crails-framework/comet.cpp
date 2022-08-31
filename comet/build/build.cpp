#include "build.hpp"
#include <boost/process.hpp>
#include <filesystem>
#include <iostream>

using namespace std;

bool comet_cmake_builder(const ProjectConfiguration& configuration);

static bool run_command(const string& command)
{
  boost::process::child process(command);

  process.wait();
  return process.exit_code() == 0;
}

bool Build::generate_html_elements()
{
  string template_folder = "views";

  if (filesystem::is_directory(template_folder))
  {
    stringstream command;

    command << ProjectConfiguration::comet_bin_path() + "/comet-html"
      << " -i " << template_folder
      << " -o " << configuration.variable("local-libdir")
      << " -c " << configuration.variable("html-config");
    return run_command(command.str());
  }
  return true;
}

int Build::run()
{
  ProjectConfiguration::move_to_project_directory();
  configuration.initialize();
  if (!generate_html_elements())
    return 2;
  if (configuration.variable("toolchain") == "cmake")
    return comet_cmake_builder(configuration) ? 0 : 1;
  else
    cerr << "Cannot build with toolchain `" << configuration.variable("toolchain") << '`' << endl;
  return -1;
}
