#include <crails/template_stream.hpp>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ProjectMainCpp : public Crails::Template
{
public:
  render_ProjectMainCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
    ecpp_stream.reserve(194);
    // BEGIN TEMPLATE BODY
ecpp_stream << "#include \"application.hpp\"\n\nvoid webMain()\n{\n  Application::start();\n}\n";
    // END TEMPLATE BODY
    std::string _out_buffer = std::move(ecpp_stream).extract();
    this->target.set_body(this->apply_post_render_filters(std::move(_out_buffer)));
  }
private:
  Crails::TemplateStream ecpp_stream;
};

void render_project_main_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectMainCpp(renderer, target, vars).render();
}