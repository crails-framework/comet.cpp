#include <crails/template_stream.hpp>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ProjectConfigJson : public Crails::Template
{
public:
  render_ProjectConfigJson(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
    ecpp_stream.reserve(452);
    // BEGIN TEMPLATE BODY
ecpp_stream << "{\n  // Custom elements specified here are included by default in all html templates\n  \"elements\": [\n    // { \"require\": \"Classname\", \"include\": \"header.hpp\", \"tagName\", \"html-tag\" }\n  ]\n}\n";
    // END TEMPLATE BODY
    std::string _out_buffer = std::move(ecpp_stream).extract();
    this->target.set_body(this->apply_post_render_filters(std::move(_out_buffer)));
  }
private:
  Crails::TemplateStream ecpp_stream;
};

void render_project_config_json(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectConfigJson(renderer, target, vars).render();
}