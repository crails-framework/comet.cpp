#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class ProjectRoutesCpp : public Crails::Template
{
public:
  ProjectRoutesCpp(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars)
  {}

  std::string render()
  {
ecpp_stream << "#include <comet/router.hpp>\n\nusing namespace Comet;\n\nvoid Router::initialize()\n{\n}\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
};

std::string render_project_routes_cpp(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ProjectRoutesCpp(renderer, vars).render();
}