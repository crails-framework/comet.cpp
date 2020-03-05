#include "url.hpp"
#include "globals.hpp"

using namespace std;
using namespace Comet;

string Url::encode(const string& src)
{
  Comet::String js_string(src.c_str());

  return window.apply("encodeURIComponent", js_string);
}

string Url::decode(const string& src)
{
  Comet::String js_string(src.c_str());

  return window.apply("decodeURIComponent", js_string);
}
