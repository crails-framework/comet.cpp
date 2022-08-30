#pragma once
#include <crails/cli/command.hpp>
#include "../project_configuration.hpp"

class Build : public Crails::Command
{
  ProjectConfiguration configuration;
public:
  std::string_view description() const override { return "Builds a Comet application."; }
  int              run() override;
private:
  bool generate_html_elements();
};
