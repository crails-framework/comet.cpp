#include <crails/template_stream.hpp>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ScaffoldControllerHpp : public Crails::Template
{
public:
  render_ScaffoldControllerHpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    classname(Crails::cast< std::string >(vars, "classname"))
  {}

  void render()
  {
    ecpp_stream.reserve(750);
    // BEGIN TEMPLATE BODY
ecpp_stream << "#pragma once\n#include <comet/mvc/controller.hpp>\n\nclass " << ( classname );
  ecpp_stream << " : public Comet::Controller\n{\npublic:\n  " << ( classname );
  ecpp_stream << "(const Comet::Params&);\n\n  Comet::Promise initialize() { return Comet::Controller::initialize(); }\n  Comet::Promise finalize()   { return Comet::Controller::finalize(); }\n};\n";
    // END TEMPLATE BODY
    std::string _out_buffer = std::move(ecpp_stream).extract();
    this->target.set_body(this->apply_post_render_filters(std::move(_out_buffer)));
  }
private:
  Crails::TemplateStream ecpp_stream;
  std::string classname;
};

void render_scaffold_controller_hpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ScaffoldControllerHpp(renderer, target, vars).render();
}