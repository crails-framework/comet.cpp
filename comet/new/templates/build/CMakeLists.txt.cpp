#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
using namespace std;

class ProjectCmakelistsTxt : public Crails::Template
{
public:
  ProjectCmakelistsTxt(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars), 
    project_name(Crails::cast<string>(vars, "project_name")), 
    output_name(Crails::cast<string>(vars, "output_name",  "application")), 
    rpath(Crails::cast<string>(vars, "rpath",  "/usr/local/lib/genericjs")), 
    generated_files_dir(Crails::cast<string>(vars, "generated_files_dir",  "lib"))
  {}

  std::string render()
  {
ecpp_stream << "# runs with the following option:\n# -DCMAKE_TOOLCHAIN_FILE \"$CHEERP_PATH/share/cmake/Modules/CheerpToolchain.cmake\"\n\ncmake_minimum_required(VERSION 3.0)\n\nproject(" << ( project_name );
  ecpp_stream << ")\ninclude_directories(/usr/local/include " << ( generated_files_dir );
  ecpp_stream << ")\nlink_directories(/usr/local/lib/genericjs /usr/lib/genericjs)\n\nset(CMAKE_CXX_FLAGS \"-target cheerp-genericjs -D__CHEERP_CLIENT__\")\nset(CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} -Wall -Wno-unknown-pragmas -pedantic\")\n\nif (CMAKE_BUILD_TYPE EQUAL \"DEBUG\")\n  set(CMAKE_EXE_LINKER_FLAGS \"${CMAKE_EXE_LINKER_FLAGS}\" -cheerp-sourcemap=" << ( output_name );
  ecpp_stream << ".js.map -cheerp-sourcemap-standalone)\nendif()\n\nfile(GLOB_RECURSE app_src *.cpp *.cxx)\n\nadd_executable(" << ( output_name );
  ecpp_stream << " ${app_src})\n\ntarget_link_libraries(" << ( output_name );
  ecpp_stream << "\n  crails-semantics\n  comet\n)\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
  string project_name;
  string output_name;
  string rpath;
  string generated_files_dir;
};

std::string render_project_cmakelists_txt(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ProjectCmakelistsTxt(renderer, vars).render();
}