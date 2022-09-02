#include <iostream>
#include <boost/program_options.hpp>
#include <crails/cli/command_index.hpp>
#include <crails/renderer.hpp>
#include "new/new.hpp"
#include "build/build.hpp"
#include "scaffolds/scaffold.hpp"

using namespace std;

const string Crails::Renderer::default_format = "*";

class Index : public Crails::CommandIndex
{
public:
  Index()
  {
    add_command("new",      []() { return make_shared<New>(); });
    add_command("build",    []() { return make_shared<Build>(); });
    add_command("scaffold", []() { return make_shared<Scaffold>(); });
  }
};

int main(int argc, char** argv)
{
  Index index;

  if (index.initialize(argc, const_cast<const char**>(argv)))
    return index.run();
  return -1;
}
