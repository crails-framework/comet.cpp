#pragma once
#include <crails/cli/scaffold_model.hpp>
#include <crails/cli/conventions.hpp>
#include <crails/utils/semantics.hpp>
#include "../file_renderer.hpp"

class ViewScaffold : public Crails::ScaffoldModel
{
  FileRenderer renderer;
  std::string name = "application";
  std::string target_folder = ".";

public:
  void options_description(boost::program_options::options_description& desc) const override
  {
    desc.add_options()
      ("name,n",     boost::program_options::value<std::string>(), "view name")
      ("target,t",   boost::program_options::value<std::string>(), "target folder")
      ("property,p", boost::program_options::value<std::vector<std::string>>()->multitoken(), properties_help());
  }

  int create(boost::program_options::variables_map& options) override
  {
    std::string comet_html_namespace = "HtmlTemplate";
    std::map<std::string, std::string> properties = properties_option(options);

    if (options.count("name"))
      name = Crails::naming_convention.filenames(options["name"].as<std::string>());
    else
      name = Crails::naming_convention.filenames("application");
    if (options.count("target"))
      target_folder = options["target"].as<std::string>();
    renderer.vars["include"] = "templates/" + (target_folder.length() > 0 ? target_folder + '/' : "") + name + ".hpp";
    renderer.vars["element"] = comet_html_namespace + "::" + Crails::camelize(name);
    renderer.vars["classname"] = Crails::naming_convention.classnames(name);
    renderer.vars["properties"] = &properties;
    renderer.generate_file("scaffolds/view.html", "views/" + target_folder + '/' + name + ".html");
    renderer.generate_file("scaffolds/view.hpp",  "views/" + target_folder + '/' + name + ".hpp");
    return 0;
  }
};
