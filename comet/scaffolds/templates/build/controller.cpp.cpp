#include <crails/template_stream.hpp>
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
    ecpp_stream.reserve(482);
    // BEGIN TEMPLATE BODY
ecpp_stream << "#include \"" << ( filename );
  ecpp_stream << ".hpp\"\n\nusing namespace std;\n" << ( classname );
  ecpp_stream << "::" << ( classname );
  ecpp_stream << "(const Comet::Params& params) : Comet::Controller(params)\n{\n}\n";
    // END TEMPLATE BODY
    std::string _out_buffer = std::move(ecpp_stream).extract();
    this->target.set_body(this->apply_post_render_filters(std::move(_out_buffer)));
  }
private:
  Crails::TemplateStream ecpp_stream;
  std::string filename;
  std::string classname;
};

void render_scaffold_controller_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ScaffoldControllerCpp(renderer, target, vars).render();
}