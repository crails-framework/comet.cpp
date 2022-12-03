#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class ScaffoldControllerHpp : public Crails::Template
{
public:
  ScaffoldControllerHpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    classname(Crails::cast<std::string>(vars, "classname"))
  {}

  void render()
  {
ecpp_stream << "#pragma once\n#include <comet/mvc/controller.hpp>\n\nclass " << ( classname );
  ecpp_stream << " : public Comet::Controller\n{\npublic:\n  " << ( classname );
  ecpp_stream << "(const Comet::Params&);\n\n  Comet::Promise initialize() { return Comet::Controller::initialize(); }\n  Comet::Promise finalize()   { return Comet::Controller::finalize(); }\n};\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
  std::string classname;
};

void render_scaffold_controller_hpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  ScaffoldControllerHpp(renderer, target, vars).render();
}