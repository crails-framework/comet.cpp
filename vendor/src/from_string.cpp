#include "from_string.hpp"
#include "exception.hpp"
#include <map>
#include <climits>
#include <algorithm>

#define declare_std_converter(type, func) \
  template<> type from_string<type>(const std::string& str) \
  { \
    return std::func(str); \
  }

namespace Comet
{
  declare_std_converter(int,                stoi)
  declare_std_converter(double,             stod)
  declare_std_converter(long double,        stold)
  declare_std_converter(long,               stol)
  declare_std_converter(unsigned long,      stoul)
  declare_std_converter(long long,          stoll)
  declare_std_converter(unsigned long long, stoull)
  declare_std_converter(float,              stof)

  template<> unsigned int from_string<unsigned int>(const std::string& str)
  {
    unsigned long value = std::stoul(str);

    if (value > UINT_MAX)
      raise(std::out_of_range(str));
    return value;
  }

  template<> short from_string<short>(const std::string& str)
  {
    int value = std::stoi(str);

    if (value > SHRT_MAX || value < SHRT_MIN)
      raise(std::out_of_range(str));
    return value;
  }

  template<> unsigned short from_string<unsigned short>(const std::string& str)
  {
    unsigned long value = std::stoul(str);

    if (value > USHRT_MAX)
      raise(std::out_of_range(str));
    return value;
  }

  template<> char from_string<char>(const std::string& str)
  {
    if (str.length() > 1)
      raise(std::out_of_range(str));
    return *str.begin();
  }

  template<> unsigned char from_string<unsigned char>(const std::string& str)
  {
    if (str.length() > 1)
      raise(std::out_of_range(str));
    return *str.begin();
  }

  template<> bool from_string<bool>(const std::string& str)
  {
    static const std::map<std::string, bool> translation_table {
      {"true", true}, {"TRUE", true}, {"True", true}, {"t", true},
      {"false", false}, {"FALSE", false}, {"False", false}, {"f", false}
    };
    auto it = translation_table.find(str);

    if (it == translation_table.end())
    {
      bool is_number = false;

      if (!str.empty())
      {
        auto it = str.begin();

        if (*it == '-')
          ++it;
        is_number = std::find_if(it, str.end(), [](unsigned char c) { return !std::isdigit(c); }) == str.end();
      }
      if (!is_number)
      {
        std::string what = "cannot cast '" + str + "' to boolean";
        raise(std::runtime_error(what.c_str()));
      }
      return std::stold(str) > 0;
    }
    return it->second;
  }

  template<> std::string from_string<std::string>(const std::string& str)
  {
    return str;
  }
}
