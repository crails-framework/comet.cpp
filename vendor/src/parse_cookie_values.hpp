#ifndef  PARSE_COOKIE_VALUES_HPP
# define PARSE_COOKIE_VALUES_HPP

# include <string>
# include <functional>

namespace Comet
{
  void parse_cookie_values(const std::string&, std::function<bool (const std::string&, const std::string&)> callback);
}

#endif
