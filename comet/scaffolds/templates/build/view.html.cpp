#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class ScaffoldViewHtml : public Crails::Template
{
public:
  ScaffoldViewHtml(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars)
  {}

  std::string render()
  {
ecpp_stream << "<template>\n  <body>\n  </body>\n</template>\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
};

std::string render_scaffold_view_html(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ScaffoldViewHtml(renderer, vars).render();
}