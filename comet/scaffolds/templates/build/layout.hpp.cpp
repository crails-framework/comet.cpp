#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
using namespace std;

class ScaffoldLayoutHpp : public Crails::Template
{
public:
  ScaffoldLayoutHpp(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars), 
    classname(Crails::cast<string>(vars, "classname")), 
    element(Crails::cast<string>(vars, "element")), 
    include(Crails::cast<string>(vars, "include"))
  {}

  std::string render()
  {
ecpp_stream << "#pragma once\n#include <comet/mvc/layout.hpp>\n#include \"" << ( include );
  ecpp_stream << "\"\n\nclass " << ( classname );
  ecpp_stream << " : public Comet::Layout<" << ( element );
  ecpp_stream << ">\n{\n};\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
  string classname;
  string element;
  string include;
};

std::string render_scaffold_layout_hpp(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ScaffoldLayoutHpp(renderer, vars).render();
}