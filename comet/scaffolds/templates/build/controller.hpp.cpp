#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class ScaffoldControllerHpp : public Crails::Template
{
public:
  ScaffoldControllerHpp(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars), 
    classname(Crails::cast<std::string>(vars, "classname"))
  {}

  std::string render()
  {
ecpp_stream << "#pragma once\n#include <comet/mvc/controller.hpp>\n\nclass " << ( classname );
  ecpp_stream << " : public Comet::Controller\n{\npublic:\n  " << ( classname );
  ecpp_stream << "(const Comet::Params&);\n\n  Comet::Promise initialize() { return Comet::Controller::initialize(); }\n  Comet::Promise finalize()   { return Comet::Controller::finalize(); }\n};\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
  std::string classname;
};

std::string render_scaffold_controller_hpp(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ScaffoldControllerHpp(renderer, vars).render();
}