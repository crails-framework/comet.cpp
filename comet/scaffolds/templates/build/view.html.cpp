#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <map>
using namespace std;

class ScaffoldViewHtml : public Crails::Template
{
public:
  ScaffoldViewHtml(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars), 
    properties(reinterpret_cast<map<string,string>&>(*Crails::cast<map<string,string>*>(vars, "properties")))
  {}

  std::string render()
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
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
  map<string,string>& properties;
};

std::string render_scaffold_view_html(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ScaffoldViewHtml(renderer, vars).render();
}