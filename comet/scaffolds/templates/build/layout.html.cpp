#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class ScaffoldLayoutHtml : public Crails::Template
{
public:
  ScaffoldLayoutHtml(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars)
  {}

  std::string render()
  {
ecpp_stream << "<template>\n  <body>\n    <slot name=\"content\"></slot>\n  </body>\n</template>\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
};

std::string render_scaffold_layout_html(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ScaffoldLayoutHtml(renderer, vars).render();
}