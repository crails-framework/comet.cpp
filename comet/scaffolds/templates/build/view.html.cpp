#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <map>
using namespace std;

class render_ScaffoldViewHtml : public Crails::Template
{
public:
  render_ScaffoldViewHtml(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    properties(reinterpret_cast<map<string,string>&>(*Crails::cast< map<string,string>* >(vars, "properties")))
  {}

  void render()
  {
ecpp_stream << "<template>";
 if (properties.size() > 0){
  ecpp_stream << "\n  <head>";
   for (auto it = properties.begin() ; it != properties.end() ; ++it){
  ecpp_stream << "\n    <attribute name=\"" << ( it->first );
  ecpp_stream << "\" type=\"" << ( it->second );
  ecpp_stream << "\" />";
 };
  ecpp_stream << "\n  </head>";
 };
  ecpp_stream << "\n  <body>\n  </body>\n</template>\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
  map<string,string>& properties;
};

void render_scaffold_view_html(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ScaffoldViewHtml(renderer, target, vars).render();
}