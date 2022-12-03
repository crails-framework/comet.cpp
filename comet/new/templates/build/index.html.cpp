#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class ProjectIndexHtml : public Crails::Template
{
public:
  ProjectIndexHtml(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
ecpp_stream << "<html>\n  <head>\n    <title>Comet Application</title>\n    <script src=\"build/application.js\"></script>\n  </head>\n  <body>\n  </body>\n</html>\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
};

void render_project_index_html(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  ProjectIndexHtml(renderer, target, vars).render();
}