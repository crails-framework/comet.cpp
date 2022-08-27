#include <string>

static bool alphanumerical(char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'); }

std::string dasherize(const std::string& source)
{
  std::string result;
  bool dash_on_next_character = true;

  for (std::size_t i = 0 ; i < source.length() ; ++i)
  {
    if (source[i] == ' ' || source[i] == '_' || source[i] == '/')
      dash_on_next_character = true;
    else if (dash_on_next_character || (source[i] >= 'A' && source[i] <= 'Z'))
    {
      result += '-';
      result += std::tolower(source[i]);
      dash_on_next_character = false;
    }
    else
      result += source[i];
  }
  if (result.length() > 0 && result[0] == '-')
    result.erase(0, 1);
  return result;
}

std::string camelize(const std::string& source, bool lowerCamelCase = false)
{
  std::string result;
  bool uppercase_on_next_character = !lowerCamelCase;

  for (std::size_t i = 0 ; i < source.length() ; ++i)
  {
    if (alphanumerical(source[i]))
    {
      if (uppercase_on_next_character)
        result += std::toupper(source[i]);
      else
        result += source[i];
      uppercase_on_next_character = false;
    }
    else
      uppercase_on_next_character = true;
  }
  return result;
}
