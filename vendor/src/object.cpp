#include "object.hpp"
#include "globals.hpp"

using namespace Comet;
using namespace std;

static void prepare_is_undefined()
{
  static bool is_prepared = false;
  
  if (!is_prepared)
  {
    client::eval("window.isUndefined = function(a) { return a == undefined ? 'y' : 'n'; };");
    is_prepared = true;
  }
}

bool Object::is_of_type(const char* type) const
{
  if (is_undefined())
    return false;
  return ptr->operator[]("constructor") == *(window[type]);
}

bool Object::is_undefined() const
{
  prepare_is_undefined();
  if (ptr != nullptr)
  {
    Object      answer = window.apply("isUndefined", *this);
    std::string result = (std::string)(*static_cast<client::String*>(answer.ptr));
    return result == "y";
  }
  return true;
}

void Object::set_global(const std::string& key, Object object)
{
  window.set(key, object);
}

template<>
vector<wstring> Object::to_vector() const
{
  vector<wstring> result;

  if (is_of_type("Array"))
  {
    const client::Array& array = *(static_cast<client::Array*>(ptr));
    result.resize(array.get_length());
    for (int i = 0 ; i < array.get_length() ; ++i)
      result.push_back((wstring)(Object(array[i])));
  }
  return result;
}

namespace Comet
{
  std::wstring to_wstring(client::String* value)
  {
    std::wstring wide_string;
    auto         length = value->get_length();
    wchar_t*     result = new wchar_t[length + 1];

    for(int i = 0; i < value->get_length() ; i++)
      result[i] = (wchar_t)value->charCodeAt(i);
    wide_string = std::wstring(result, length);
    delete[] result;
    return wide_string;
  }
}
