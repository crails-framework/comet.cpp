#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>

using namespace std;

bool read_file(const string& filepath, string& out)
{
  ifstream file(filepath.c_str());

  if (file.is_open())
  {
    size_t size, begin, end;

    begin = file.tellg();
    file.seekg(0, ios::end);
    end = file.tellg();
    file.seekg(0, ios::beg);
    size = end - begin;
    out.resize(size);
    file.read(&out[0], size);
    file.close();
    return true;
  }
  return false;
}

bool write_file(const string& filepath, const string& contents)
{
  string original_source;
  bool   exists = read_file(filepath, original_source);

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
