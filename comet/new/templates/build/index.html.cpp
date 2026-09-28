#include <crails/template_stream.hpp>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ProjectIndexHtml : public Crails::Template
{
public:
  render_ProjectIndexHtml(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
    ecpp_stream.reserve(334);
    // BEGIN TEMPLATE BODY
ecpp_stream << "<html>\n  <head>\n    <title>Comet Application</title>\n    <script src=\"build/application.js\"></script>\n  </head>\n  <body>\n  </body>\n</html>\n";
    // END TEMPLATE BODY
    std::string _out_buffer = std::move(ecpp_stream).extract();
    this->target.set_body(this->apply_post_render_filters(std::move(_out_buffer)));
  }
private:
  Crails::TemplateStream ecpp_stream;
};

void render_project_index_html(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectIndexHtml(renderer, target, vars).render();
}