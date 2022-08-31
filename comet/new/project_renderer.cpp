#include "project_renderer.hpp"
#define declare_renderer(name) \
  std::string render_##name(const Crails::Renderer*, Crails::SharedVars&);
#define add_renderer(path, name) \
  templates.insert(pair<string, Generator>(path, render_##name))

using namespace std;

declare_renderer(project_cmakelists_txt)
declare_renderer(project_main_cpp)
declare_renderer(project_routes_cpp)
declare_renderer(project_application_hpp)
declare_renderer(project_config_json)

ProjectRenderer::ProjectRenderer()
{
  add_renderer("CMakeLists.txt",  project_cmakelists_txt);
  add_renderer("main.cpp",        project_main_cpp);
  add_renderer("routes.cpp",      project_routes_cpp);
  add_renderer("application.hpp", project_application_hpp);
  add_renderer("config.json",     project_config_json);
}
