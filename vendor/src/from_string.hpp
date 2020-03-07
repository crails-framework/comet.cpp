#ifndef  COMET_FROM_STRING_HPP
# define COMET_FROM_STRING_HPP

# include <string>
# include <sstream>

namespace Comet
{
  template<typename TYPE>
  TYPE from_string(const std::string& str)
  {
    std::stringstream stream;
    TYPE value;

    stream << str;
    stream >> value;
    return value;
  }

  template<> int                from_string<int>(const std::string& str);
  template<> unsigned int       from_string<unsigned int>(const std::string& str);
  template<> double             from_string<double>(const std::string& str);
  template<> long double        from_string<long double>(const std::string& str);
  template<> long               from_string<long>(const std::string& str);
  template<> unsigned long      from_string<unsigned long>(const std::string& str);
  template<> long long          from_string<long long>(const std::string& str);
  template<> unsigned long long from_string<unsigned long long>(const std::string& str);
  template<> float              from_string<float>(const std::string& str);
  template<> short              from_string<short>(const std::string& str);
  template<> unsigned short     from_string<unsigned short>(const std::string& str);
  template<> char               from_string<char>(const std::string& str);
  template<> unsigned char      from_string<unsigned char>(const std::string& str);
  template<> bool               from_string<bool>(const std::string& str);
  template<> std::string        from_string<std::string>(const std::string& str);
}

#endif
