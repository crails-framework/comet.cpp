#pragma once
#include <crails/cli/scaffold_model.hpp>
#include <crails/cli/conventions.hpp>
#include <crails/utils/semantics.hpp>
#include "../file_renderer.hpp"

class LayoutScaffold : public ScaffoldModel
{
  FileRenderer renderer;
  std::string name = "application";
  std::string target_folder = "layouts";

public:
  void options_description(boost::program_options::options_description& desc) const override
  {
    desc.add_options()
      ("name,n",   boost::program_options::value<std::string>(), "layout name (defaults to application)")
      ("target,t", boost::program_options::value<std::string>(), "target folder (defaults to layouts)");
  }

  int create(boost::program_options::variables_map& options) override
  {
    std::string comet_html_namespace = "HtmlTemplate";

    if (options.count("name"))
      name = Crails::naming_convention.filenames(options["name"].as<std::string>());
    if (options.count("target"))
      target_folder = options["target"].as<std::string>();
    renderer.vars["classname"] = Crails::naming_convention.classnames(name);
    renderer.vars["element"] = comet_html_namespace + "::" + Crails::camelize(name);
    renderer.vars["include"] = "templates/" + (target_folder.length() > 0 ? target_folder + '/' : "") + name + ".hpp";
    renderer.generate_file("scaffolds/layout.html", "views/" + target_folder + '/' + name + ".html");
    renderer.generate_file("scaffolds/layout.hpp",  "views/" + target_folder + '/' + name + ".hpp");
    return 0;
  }
};
