#include "new.hpp"
#include "../project_renderer.hpp"
#include <crails/cli/filesystem.hpp>
#include <crails/render_file.hpp>
#include <filesystem>

using namespace std;

class FileRenderer
{
public:
  ProjectRenderer    renderer;
  Crails::SharedVars vars;
  bool               should_overwrite = false;

  bool generate_file(std::string_view template_name) { return generate_file(template_name, template_name); }
  bool generate_file(std::string_view template_name, std::string_view filepath)
  {
    std::filesystem::path path(filepath.data());

    return (should_overwrite || Crails::prompt_write_file("FILE", path.string()))
        && render_file(template_name, path);
  }
private:
  bool render_file(std::string_view template_name, const std::filesystem::path& path)
  {
    Crails::RenderFile render_target;

    if (renderer.can_render(std::string(template_name)))
    {
      if (Crails::require_folder("DIR", path.string()))
      {
        render_target.open(path.string());
        renderer.render_template(template_name.data(), render_target, vars);
        return true;
      }
    }
    else
      cout << "[FILE] `" << path.string() << "` could not be generated: template not found." << endl;
    return false;
  }
};

void New::options_description(boost::program_options::options_description& desc) const
{
  desc.add_options()
    ("name,n",        boost::program_options::value<string>(), "project name")
    ("target,o",      boost::program_options::value<string>(), "folder in which the project will be generated (equal to `name` by default)")
    ("build-dir,b",   boost::program_options::value<string>(), "in which path should `comet build` build the application (defaults to ./build)")
    ("cheerp-path",   boost::program_options::value<string>(), "path where cheerp is installed (defaults to /opt/cheerp)")
    ("toolchain,t",   boost::program_options::value<string>(), "options: cmake or build2 (defaults to cmake)")
    ("html-config",   boost::program_options::value<string>(), "comet-html config path (defaults to config.json)")
    ("html-output",   boost::program_options::value<string>(), "configure comet-html to store generated Comet elements in a custom folder")
    ("include-src",   boost::program_options::value<vector<string>>()->multitoken(), "external source files to add to the project")
    ("rpath,l",       boost::program_options::value<string>(), "library path for javascript libraries (defaults to /usr/local/lib/genericjs)")
    ("filename",      boost::program_options::value<string>(), "name of the javascript file generated by comet")
    ("force,f", "overwrite existing files without asking")
    ("skip-index-html", "skips generation of example index.html file");
}

bool New::generate_project_structure()
{
  FileRenderer renderer;
  vector<string> external_sources;

  if (options.count("include-src"))
    external_sources = options["include-src"].as<vector<string>>();
  renderer.should_overwrite = options.count("force");
  renderer.vars["project_name"] = options["name"].as<string>();
  renderer.vars["generated_files_dir"] = html_output_path;
  renderer.vars["external_sources"] = &external_sources;
  if (options.count("rpath"))
    renderer.vars["rpath"] = options["rpath"].as<string>();
  if (options.count("filename"))
    renderer.vars["output_name"] = options["filename"].as<string>();
  if (build_system == "cmake")
    renderer.generate_file("CMakeLists.txt");
  else if (build_system == "build2")
  {
    cerr << "toolchain build2 not yet supported" << endl;
    return false;
  }
  else
  {
    cerr << "toolchain " << build_system << " unknown" << endl;
    return false;
  }
  renderer.generate_file("application.hpp");
  renderer.generate_file("main.cpp");
  renderer.generate_file("routes.cpp");
  renderer.generate_file("config.json", html_config_path);
  if (!options.count("skip-index-html"))
    renderer.generate_file("index.html");
  return true;
}

bool New::move_to_project_directory()
{
  filesystem::path path(target().data());

  if (!filesystem::is_directory(path))
  {
    error_code ec;

    filesystem::create_directories(path, ec);
    if (ec)
    {
      cout << "Failed to reach project directory: " << ec.message() << endl;
      return false;
    }
  }
  filesystem::current_path(path);
  return true;
}

int New::run()
{
  if (!options.count("name"))
    cerr << "option --name required" << endl;
  else if (move_to_project_directory())
  {
    html_output_path = html_config_path = filesystem::canonical(filesystem::current_path()).string() + '/';
    if (options.count("build-dir"))
    {
      filesystem::create_directories(options["build-dir"].as<string>());
      build_path = filesystem::canonical(options["build-dir"].as<string>());
    }
    html_config_path += (options.count("html-config") ? options["html-config"].as<string>() : string("config.json"));
    html_output_path += (options.count("html-output") ? options["html-output"].as<string>() : string("lib"));
    html_config_path = filesystem::relative(html_config_path, filesystem::current_path());
    html_output_path = filesystem::relative(html_output_path, filesystem::current_path());
    if (options.count("toolchain"))
      build_system = options["toolchain"].as<string>();
    if (options.count("cheerp-path"))
      cheerp_path = options["cheerp-path"].as<string>();
    configuration.variable("name",         options["name"].as<string>());
    configuration.variable("toolchain",    build_system);
    configuration.variable("cheerp-path",  cheerp_path);
    configuration.variable("html-config",  html_config_path);
    configuration.variable("local-libdir", html_output_path);
    if (build_path.length() > 0)
      configuration.variable("build-dir", filesystem::relative(build_path));
    configuration.save();
    return generate_project_structure() ? 0 : 1;
  }
  return -2;
}
