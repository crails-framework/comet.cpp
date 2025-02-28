#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ScaffoldControllerCpp : public Crails::Template
{
public:
  render_ScaffoldControllerCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    filename(Crails::cast< std::string >(vars, "filename")), 
    classname(Crails::cast< std::string >(vars, "classname"))
  {}

  void render()
  {
ecpp_stream << "#include \"" << ( filename );
  ecpp_stream << ".hpp\"\n\nusing namespace std;\n" << ( classname );
  ecpp_stream << "::" << ( classname );
  ecpp_stream << "(const Comet::Params& params) : Comet::Controller(params)\n{\n}\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
  std::string filename;
  std::string classname;
};

void render_scaffold_controller_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ScaffoldControllerCpp(renderer, target, vars).render();
}