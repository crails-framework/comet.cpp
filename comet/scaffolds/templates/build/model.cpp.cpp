#include <crails/template_stream.hpp>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <algorithm>
using namespace std;

class render_ScaffoldModelCpp : public Crails::Template
{
public:
  render_ScaffoldModelCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    filename(Crails::cast< string >(vars, "filename")), 
    classname(Crails::cast< string >(vars, "classname")), 
    properties(Crails::cast< map<string, string> >(vars, "properties"))
  {}

  void render()
  {
    ecpp_stream.reserve(2510);
    // BEGIN TEMPLATE BODY
ecpp_stream << "#pragma once\n#include \"" << ( filename );
  ecpp_stream << ".hpp\"\n#include <sstream>\n\nusing namespace std;\n\nstring " << ( classname );
  ecpp_stream << "::get_url()\n{\n  stringstream url;\n\n  url << '/' << get_resource_name();\n  if (get_id() != COMET_MVC_NULL_ID)\n    url << '/' << get_id();\n  return url.str();\n}\n\nstring " << ( classname );
  ecpp_stream << "::to_json()\n{\n  DataTree object;\n";
 for (auto it = properties.begin() ; it != properties.end() ; ++it){
  ecpp_stream << "";
   if (it->second == "DataTree"){
  ecpp_stream << "\n  object[\"" << ( it->first );
  ecpp_stream << "\"].merge(" << ( it->first );
  ecpp_stream << ".as_data());";
 }else{
  ecpp_stream << "\n  object[\"" << ( it->first );
  ecpp_stream << "\"] = " << ( it->first );
  ecpp_stream << ";";
 };
  ecpp_stream << "";
 };
  ecpp_stream << "\n  return object.to_json();\n}\n\nvoid " << ( classname );
  ecpp_stream << "::from_json(Data params)\n{";
 for (auto it = properties.begin() ; it != properties.end() ; ++it){
  ecpp_stream << "\n  if (params[\"" << ( it->first );
  ecpp_stream << "\"].exists())\n    set_" << ( it->first );
  ecpp_stream << "(params[\"" << ( it->first );
  ecpp_stream << "\"]);";
 };
  ecpp_stream << "\n}\n";
    // END TEMPLATE BODY
    std::string _out_buffer = std::move(ecpp_stream).extract();
    this->target.set_body(this->apply_post_render_filters(std::move(_out_buffer)));
  }
private:
  Crails::TemplateStream ecpp_stream;
  string filename;
  string classname;
  map<string, string> properties;
};

void render_scaffold_model_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ScaffoldModelCpp(renderer, target, vars).render();
}