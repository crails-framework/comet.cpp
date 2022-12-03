#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class ScaffoldControllerCpp : public Crails::Template
{
public:
  ScaffoldControllerCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    filename(Crails::cast<std::string>(vars, "filename")), 
    classname(Crails::cast<std::string>(vars, "classname"))
  {}

  void render()
  {
ecpp_stream << "#include \"" << ( filename );
  ecpp_stream << ".hpp\"\n\nusing namespace std;\n" << ( classname );
  ecpp_stream << "::" << ( classname );
  ecpp_stream << "(const Comet::Params& params) : Comet::Controller(params)\n{\n}\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
  std::string filename;
  std::string classname;
};

void render_scaffold_controller_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  ScaffoldControllerCpp(renderer, target, vars).render();
}