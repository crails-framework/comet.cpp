#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <algorithm>
using namespace std;

class render_ScaffoldModelHpp : public Crails::Template
{
public:
  render_ScaffoldModelHpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    classname(Crails::cast<string>(vars, "classname")), 
    properties(Crails::cast<map<string, string>>(vars, "properties")), 
    scalar_types( {"bool","char","unsigned char","char16_t","char32_t","wchar_t","signed char","int","unsigned int","short","unsigned short","long","long long","unsigned long","unsigned long long","double","long double","float","std::size_t","std::time_t"})
  {}

  void render()
  {
ecpp_stream << "#pragma once\n#include <crails/mvc/model.hpp>\n\nclass " << ( classname );
  ecpp_stream << " : public Comet::JsonModel\n{\npublic:\n  std::string get_url() const override;\n  std::string to_json() const override;\n  void from_json(Data) override;\n";
 for (auto it = properties.begin() ; it != properties.end() ; ++it){
  ecpp_stream << "";
   if (it->second == "DataTree"){
  ecpp_stream << "\n  DataTree& get_" << ( it->first );
  ecpp_stream << "() { return " << ( it->first );
  ecpp_stream << "; }\n  const DataTree& get_" << ( it->first );
  ecpp_stream << "() const { return " << ( it->first );
  ecpp_stream << "; }\n  void set_" << ( it->first );
  ecpp_stream << "(Data value) { this->" << ( it->first );
  ecpp_stream << ".clear(); this->" << ( it->first );
  ecpp_stream << ".as_data().merge(value); }";
 }else if (std::find(scalar_types.begin(), scalar_types.end(), it->second) != scalar_types.end()){
  ecpp_stream << "\n  void set_" << ( it->first );
  ecpp_stream << "(" << ( it->second );
  ecpp_stream << " value) { this->" << ( it->first );
  ecpp_stream << " = value; }\n  " << ( it->second );
  ecpp_stream << " get_" << ( it->first );
  ecpp_stream << "() const { return " << ( it->first );
  ecpp_stream << "; }";
 }else{
  ecpp_stream << "\n  void set_" << ( it->first );
  ecpp_stream << "(const " << ( it->second );
  ecpp_stream << "& value) { this->" << ( it->first );
  ecpp_stream << " = value; }\n  const " << ( it->second );
  ecpp_stream << "& get_" << ( it->first );
  ecpp_stream << "() const { return " << ( it->first );
  ecpp_stream << "; }";
 };
  ecpp_stream << "";
 };
  ecpp_stream << "\n\nprivate:";
 for (auto it = properties.begin() ; it != properties.end() ; ++it){
  ecpp_stream << "\n  " << ( it->second );
  ecpp_stream << " " << ( it->first );
  ecpp_stream << ";";
 };
  ecpp_stream << "\n};\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
  string classname;
  map<string, string> properties;
  vector<string> scalar_types;
};

void render_scaffold_model_hpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ScaffoldModelHpp(renderer, target, vars).render();
}