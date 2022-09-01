#pragma once
#include <crails/cli/command.hpp>
#include <string_view>
#include "../project_configuration.hpp"

class New : public Crails::Command
{
  ProjectConfiguration configuration;
  std::string build_system     = "cmake";
  std::string cheerp_path      = "/opt/cheerp";
  std::string html_output_path;
  std::string html_config_path;
  std::string build_path;
public:
  std::string_view description() const override { return "Create a new comet project"; }
  void options_description(boost::program_options::options_description&) const override;
  int run() override;

private:
  std::string_view target() const { return (options.count("target") ? options["target"] : options["name"]).as<std::string>(); }
  bool generate_project_structure();
  bool move_to_project_directory();
};

