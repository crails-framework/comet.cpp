#include <crails/template_stream.hpp>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ScaffoldLayoutHtml : public Crails::Template
{
public:
  render_ScaffoldLayoutHtml(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
    ecpp_stream.reserve(200);
    // BEGIN TEMPLATE BODY
ecpp_stream << "<template>\n  <body>\n    <slot name=\"content\"></slot>\n  </body>\n</template>\n";
    // END TEMPLATE BODY
    std::string _out_buffer = std::move(ecpp_stream).extract();
    this->target.set_body(this->apply_post_render_filters(std::move(_out_buffer)));
  }
private:
  Crails::TemplateStream ecpp_stream;
};

void render_scaffold_layout_html(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ScaffoldLayoutHtml(renderer, target, vars).render();
}