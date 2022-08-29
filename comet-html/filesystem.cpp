#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <crails/read_file.hpp>

using namespace std;

bool write_file(const string& filepath, const string& contents)
{
  string original_source;
  bool   exists = Crails::read_file(filepath, original_source);

  if (contents != original_source)
  {
    ofstream file(filepath.c_str());
    filesystem::create_directories(filesystem::path(filepath).parent_path());
    if (file.is_open())
    {
      file.write(contents.c_str(), contents.length());
      file.close();
      cout << "[comet-html] " << filepath << ": " << (exists ? "updated" : "generated") << endl;
      return true;
    }
    else
      cerr << "cannot write in file `" << filepath << '`' << endl;
    return false;
  }
  return true;
}
