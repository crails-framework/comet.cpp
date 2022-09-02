#include "file_renderer.hpp"
#include <boost/filesystem.hpp>
#include <crails/cli/filesystem.hpp>
#include <iostream>

using namespace std;

bool FileRenderer::render_file(string_view template_name, boost::filesystem::path path)
{
  Crails::RenderString render_target;

  if (renderer.can_render("", template_name.data()))
  {
    if (Crails::require_folder("DIR", path.string()))
    {
      renderer.render_template(template_name.data(), render_target, vars);
      Crails::write_file("FILE", path.string(), string(render_target.value()));
      return true;
    }
  }
  else
    cerr << "[FILE] `" << path.string() << "` could not be generated: template not found." << endl;
  return false;
}

bool FileRenderer::generate_file(string_view template_name, string_view filepath)
{
  boost::filesystem::path path(filepath.data());

  return (should_overwrite || Crails::prompt_write_file("FILE", path.string()))
      && render_file(template_name, path);
}
