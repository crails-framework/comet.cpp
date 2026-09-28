#include <crails/template_stream.hpp>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
using namespace std;

class render_ScaffoldLayoutHpp : public Crails::Template
{
public:
  render_ScaffoldLayoutHpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    classname(Crails::cast< string >(vars, "classname")), 
    element(Crails::cast< string >(vars, "element")), 
    include(Crails::cast< string >(vars, "include"))
  {}

  void render()
  {
    ecpp_stream.reserve(464);
    // BEGIN TEMPLATE BODY
ecpp_stream << "#pragma once\n#include <comet/mvc/layout.hpp>\n#include \"" << ( include );
  ecpp_stream << "\"\n\nclass " << ( classname );
  ecpp_stream << " : public Comet::Layout<" << ( element );
  ecpp_stream << ">\n{\n};\n";
    // END TEMPLATE BODY
    std::string _out_buffer = std::move(ecpp_stream).extract();
    this->target.set_body(this->apply_post_render_filters(std::move(_out_buffer)));
  }
private:
  Crails::TemplateStream ecpp_stream;
  string classname;
  string element;
  string include;
};

void render_scaffold_layout_hpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ScaffoldLayoutHpp(renderer, target, vars).render();
}