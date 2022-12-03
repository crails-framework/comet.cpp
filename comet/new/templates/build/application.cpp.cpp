#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class ProjectApplicationHpp : public Crails::Template
{
public:
  ProjectApplicationHpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
ecpp_stream << "#pragma once\n#include <comet/router.hpp>\n\nclass Application\n{\n  Application() {}\n  Application(const Application&) = delete;\n  void operator=(const Application&) = delete;\npublic:\n  static Application& get()\n  {\n    static Application instance;\n    return instance;\n  }\n\n  static void start()\n  {\n    auto& app = get();\n\n    app.router.start();\n  }\n\n  Comet::Router router;\n};\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
};

void render_project_application_hpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  ProjectApplicationHpp(renderer, target, vars).render();
}