#include <crails/template_stream.hpp>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ProjectRoutesCpp : public Crails::Template
{
public:
  render_ProjectRoutesCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
    ecpp_stream.reserve(276);
    // BEGIN TEMPLATE BODY
ecpp_stream << "#include <comet/router.hpp>\n#include <comet/globals.hpp>\n\nusing namespace Comet;\n\nvoid Router::initialize()\n{\n}\n";
    // END TEMPLATE BODY
    std::string _out_buffer = std::move(ecpp_stream).extract();
    this->target.set_body(this->apply_post_render_filters(std::move(_out_buffer)));
  }
private:
  Crails::TemplateStream ecpp_stream;
};

void render_project_routes_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectRoutesCpp(renderer, target, vars).render();
}