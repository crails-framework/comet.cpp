#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class ProjectAppMainCpp : public Crails::Template
{
public:
  ProjectAppMainCpp(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars)
  {}

  std::string render()
  {
ecpp_stream << "#include \"application.hpp\"\n\nvoid webMain()\n{\n  Application::start();\n}\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
};

std::string render_project_app_main_cpp(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ProjectAppMainCpp(renderer, vars).render();
}