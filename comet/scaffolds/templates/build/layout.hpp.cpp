#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
using namespace std;

class ScaffoldLayoutHpp : public Crails::Template
{
public:
  ScaffoldLayoutHpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    classname(Crails::cast<string>(vars, "classname")), 
    element(Crails::cast<string>(vars, "element")), 
    include(Crails::cast<string>(vars, "include"))
  {}

  void render()
  {
ecpp_stream << "#pragma once\n#include <comet/mvc/layout.hpp>\n#include \"" << ( include );
  ecpp_stream << "\"\n\nclass " << ( classname );
  ecpp_stream << " : public Comet::Layout<" << ( element );
  ecpp_stream << ">\n{\n};\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
  string classname;
  string element;
  string include;
};

void render_scaffold_layout_hpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  ScaffoldLayoutHpp(renderer, target, vars).render();
}