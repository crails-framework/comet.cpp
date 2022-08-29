#pragma once
#include <crails/cli/project_variables.hpp>
#include <filesystem>

class ProjectConfiguration : public Crails::ProjectVariables
{
public:
  ProjectConfiguration() : Crails::ProjectVariables(".comet") {}

  static std::string project_directory() { return ProjectConfiguration().lookup_variable_path(); }
  static void move_to_project_directory() { std::filesystem::current_path(project_directory); }
  static std::string comet_bin_path() { return std::filesystem::canonical("/proc/self/exe").parent_path().string(); }

  std::string build_type() const { return variable_or("build-type", "Release"); }
  std::string application_build_path() const { return variable_or("build-dir", "build"); }
};
