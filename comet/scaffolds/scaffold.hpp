#pragma once
#include <crails/cli/scaffold.hpp>
#include "model.hpp"
#include "view.hpp"
#include "controller.hpp"
#include "layout.hpp"
#include <iostream>

class Scaffold : public Crails::Scaffold
{
public:
  Scaffolds available_scaffolds() const override
  {
    return Scaffolds{
      {"controller", []() { return new ControllerScaffold; }},
      {"model",      []() { return new ModelScaffold; }},
      {"view",       []() { return new ViewScaffold; }},
      {"layout",     []() { return new LayoutScaffold; }}
    };
  }
};
