#pragma once
#include <crails/cli/command.hpp>
#include "controller.hpp"
#include "view.hpp"
#include "layout.hpp"
#include <iostream>

class Scaffold : public Crails::Command
{
  std::string type;
  ScaffoldModel* model = nullptr;
public:
  std::string_view description() const override { return "Generates basic files forming the basic structure of Comet."; }

  bool initialize(int argc, const char** argv) override
  {
    if (argc > 0)
    {
      type = argv[1];
      if (type == "controller")
        model = new ControllerScaffold;
      else if (type == "view")
        model = new ViewScaffold;
      else if (type == "layout")
        model = new LayoutScaffold;
      else
      {
        std::cerr << "Unknown scaffold type " << type << std::endl;
        return false;
      }
      return Command::initialize(argc - 1, &argv[1]);
    }
    else
    {
      std::cout << "Available scaffolds:" << std::endl;
      for (const std::string& type : std::vector<std::string>{"controller","model","view","layout"})
        std::cout << " - " << type << std::endl;
    }
    return false;
  }

  void options_description(boost::program_options::options_description& desc) const override
  {
    if (model)
      model->options_description(desc);
  }

  int run()
  {
    model->create(options);
    return 0;
  }
};
