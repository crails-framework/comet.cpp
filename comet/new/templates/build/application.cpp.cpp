#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ProjectApplicationHpp : public Crails::Template
{
public:
  render_ProjectApplicationHpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
ecpp_stream << "#pragma once\n#include <comet/router.hpp>\n\nclass Application\n{\n  Application() {}\n  Application(const Application&) = delete;\n  void operator=(const Application&) = delete;\npublic:\n  static Application& get()\n  {\n    static Application instance;\n    return instance;\n  }\n\n  static void start()\n  {\n    auto& app = get();\n\n    app.router.start();\n  }\n\n  Comet::Router router;\n};\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
};

void render_project_application_hpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectApplicationHpp(renderer, target, vars).render();
}