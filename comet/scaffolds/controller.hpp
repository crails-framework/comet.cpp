#pragma once
#include <crails/cli/scaffold_model.hpp>
#include <crails/utils/string.hpp>
#include "../file_renderer.hpp"

class ControllerScaffold : public Crails::ScaffoldModel
{
  FileRenderer renderer;
  std::string name;
  std::string target_folder = ".";
public:
  void options_description(boost::program_options::options_description& desc) const override
  {
    desc.add_options()
      ("name,n",   boost::program_options::value<std::string>(), "controller name")
      ("target,t", boost::program_options::value<std::string>(), "target folder (defaults to ./)");
  }

  int create(boost::program_options::variables_map& options) override
  {
    std::string filename;

    if (!options.count("name"))
    {
      return -1;
    }
    name = options["name"].as<std::string>();
    filename = Crails::underscore(name);
    if (options.count("target"))
      target_folder = options["target"].as<std::string>();
    renderer.vars["classname"] = name + "Controller";
    renderer.vars["filename"] = filename;
    renderer.generate_file("scaffolds/controller.hpp", target_folder + "/controllers/" + filename + ".hpp");
    renderer.generate_file("scaffolds/controller.cpp", target_folder + "/controllers/" + filename + ".cpp");
    return 0;
  }
};
