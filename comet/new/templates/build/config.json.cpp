#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class ProjectConfigJson : public Crails::Template
{
public:
  ProjectConfigJson(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars)
  {}

  std::string render()
  {
ecpp_stream << "{\n  // Custom elements specified here are included by default in all html templates\n  \"elements\": [\n    // { \"require\": \"Classname\", \"include\": \"header.hpp\", \"tagName\", \"html-tag\" }\n  ]\n}\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
};

std::string render_project_config_json(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ProjectConfigJson(renderer, vars).render();
}