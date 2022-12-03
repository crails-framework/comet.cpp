#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class ProjectConfigJson : public Crails::Template
{
public:
  ProjectConfigJson(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
ecpp_stream << "{\n  // Custom elements specified here are included by default in all html templates\n  \"elements\": [\n    // { \"require\": \"Classname\", \"include\": \"header.hpp\", \"tagName\", \"html-tag\" }\n  ]\n}\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
};

void render_project_config_json(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  ProjectConfigJson(renderer, target, vars).render();
}