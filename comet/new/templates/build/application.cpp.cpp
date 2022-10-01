#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class ProjectApplicationHpp : public Crails::Template
{
public:
  ProjectApplicationHpp(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars)
  {}

  std::string render()
  {
ecpp_stream << "#pragma once\n#include <comet/router.hpp>\n\nclass Application\n{\n  Application() {}\n  Application(const Application&) = delete;\n  void operator=(const Application&) = delete;\npublic:\n  static Application& get()\n  {\n    static Application instance;\n    return instance;\n  }\n\n  static void start()\n  {\n    auto& app = get();\n\n    app.router.start();\n  }\n\n  Comet::Router router;\n};\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
};

std::string render_project_application_hpp(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ProjectApplicationHpp(renderer, vars).render();
}