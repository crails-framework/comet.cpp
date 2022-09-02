#pragma once
#include "project_renderer.hpp"
#include <crails/shared_vars.hpp>
#include <boost/filesystem.hpp>

class FileRenderer
{
public:
  ProjectRenderer    renderer;
  Crails::SharedVars vars;
  bool               should_overwrite = false;

  bool generate_file(std::string_view template_name) { return generate_file(template_name, template_name); }
  bool generate_file(std::string_view template_name, std::string_view filepath);
private:
  bool render_file(std::string_view template_name, boost::filesystem::path);
};
