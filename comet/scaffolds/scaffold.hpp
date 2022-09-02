#pragma once
#include <crails/cli/scaffold.hpp>
#include "controller.hpp"
#include "view.hpp"
#include "layout.hpp"
#include <iostream>

class Scaffold : public Crails::Scaffold
{
public:
  Scaffolds available_scaffolds() const override
  {
    return Scaffolds{
      {"controller", []() { return new ControllerScaffold; }},
      {"view",       []() { return new ViewScaffold; }},
      {"layout",     []() { return new LayoutScaffold; }}
    };
  }
};
