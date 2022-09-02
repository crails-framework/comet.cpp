#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class ScaffoldControllerCpp : public Crails::Template
{
public:
  ScaffoldControllerCpp(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars), 
    filename(Crails::cast<std::string>(vars, "filename")), 
    classname(Crails::cast<std::string>(vars, "classname"))
  {}

  std::string render()
  {
ecpp_stream << "#include \"" << ( filename );
  ecpp_stream << ".hpp\"\n\nusing namespace std;\n" << ( classname );
  ecpp_stream << "::" << ( classname );
  ecpp_stream << "(const Comet::Params& params) : Comet::Controller(params)\n{\n}\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
  std::string filename;
  std::string classname;
};

std::string render_scaffold_controller_cpp(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ScaffoldControllerCpp(renderer, vars).render();
}