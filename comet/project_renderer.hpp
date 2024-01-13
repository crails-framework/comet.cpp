#pragma once
#include <crails/renderer.hpp>
#include <crails/render_file.hpp>

class ProjectRenderer : public Crails::Renderer
{
public:
  ProjectRenderer();
  const std::vector<std::string>& get_mimetypes() const override;
  void render_template(const std::string& view, Crails::RenderTarget& target, Crails::SharedVars& vars) const override
  {
    auto tpl    = templates.find(view);
    (*tpl).second(*this, target, vars);
  }
};
