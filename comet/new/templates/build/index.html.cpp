#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class ProjectIndexHtml : public Crails::Template
{
public:
  ProjectIndexHtml(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars)
  {}

  std::string render()
  {
ecpp_stream << "<html>\n  <head>\n    <title>Comet Application</title>\n    <script src=\"build/application.js\"></script>\n  </head>\n  <body>\n  </body>\n</html>\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
};

std::string render_project_index_html(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ProjectIndexHtml(renderer, vars).render();
}