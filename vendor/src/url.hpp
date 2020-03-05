#ifndef  COMET_HTTP_HPP
# define COMET_HTTP_HPP

# include <string>

namespace Comet
{
  namespace Url
  {
    std::string encode(const std::string&);
    std::string decode(const std::string&);
  }
}

#endif
