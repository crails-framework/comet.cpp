#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
using namespace std;

class render_ScaffoldViewHpp : public Crails::Template
{
public:
  render_ScaffoldViewHpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    classname(Crails::cast<string>(vars, "classname")), 
    element(Crails::cast<string>(vars, "element")), 
    include(Crails::cast<string>(vars, "include"))
  {}

  void render()
  {
ecpp_stream << "#pragma once\n#include \"" << ( include );
  ecpp_stream << "\"\n\nclass " << ( classname );
  ecpp_stream << " : public " << ( element );
  ecpp_stream << "\n{\n};\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
  string classname;
  string element;
  string include;
};

void render_scaffold_view_hpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ScaffoldViewHpp(renderer, target, vars).render();
}