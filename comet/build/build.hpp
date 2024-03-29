#pragma once
#include <crails/cli/command.hpp>
#include "../project_configuration.hpp"

class Build : public Crails::Command
{
  ProjectConfiguration configuration;
public:
  std::string_view description() const override { return "Builds a Comet application."; }
  int              run() override;
  void options_description(boost::program_options::options_description& desc) const override
  {
    desc.add_options()
      ("verbose,v", "enable verbose mode")
      ("mode,m", boost::program_options::value<std::string>(), "Release or Debug: defaults to build-type value in the .comet file")
      ("clean,c", "cleanup before building");
  }

private:
  bool generate_html_elements();
};
