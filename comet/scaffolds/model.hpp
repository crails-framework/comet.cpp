#pragma once
#include <crails/cli/scaffold_model.hpp>
#include <crails/cli/conventions.hpp>
#include "../file_renderer.hpp"

class ModelScaffold : public Crails::ScaffoldModel
{
  FileRenderer renderer;
  std::map<std::string, std::string> properties;
  std::string classname;
  std::string path_name;
  std::string target_folder = "models";
public:
  void options_description(boost::program_options::options_description& desc) const override
  {
    desc.add_options()
      ("model,m",    boost::program_options::value<std::string>(), "classname")
      ("target,t",   boost::program_options::value<std::string>(), "target folder (defaults to `app/models`)")
      ("property,p", boost::program_options::value<std::vector<std::string>>()->multitoken(), properties_help());
  }

  int create(boost::program_options::variables_map& options) override
  {
    if (!options.count("model"))
    {
      std::cerr << "Option --model required" << std::endl;
      return -1;
    }
    classname = Crails::naming_convention.classnames(options["model"].as<std::string>());
    path_name = Crails::naming_convention.filenames(options["model"].as<std::string>());
    properties = properties_option(options);
    renderer.vars["classname"]  = classname;
    renderer.vars["properties"] = &properties;
    renderer.vars["filename"]   = path_name;
    renderer.generate_file("scaffolds/model.hpp", target_folder + '/' + path_name + ".hpp");
    renderer.generate_file("scaffolds/model.cpp", target_folder + '/' + path_name + ".cpp");
    return 0;
  }
};
