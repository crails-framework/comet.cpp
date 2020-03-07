#ifndef  COMET_LEXICAL_CAST_HPP
# define COMET_LEXICAL_CAST_HPP

# include <string>
# include <sstream>
# include "from_string.hpp"

#include <iostream>

namespace Comet
{
  template<typename OUTPUT, typename INPUT, bool SAME = std::is_same<OUTPUT, INPUT>::value >
  struct lexical_caster
  {
    static inline OUTPUT _(INPUT input)
    {
      std::stringstream stream;
      OUTPUT output;

      stream << input;
      stream >> output;
      return output;
    }
  };

  template<typename OUTPUT, typename INPUT>
  struct lexical_caster<OUTPUT, INPUT, true>
  {
    static inline OUTPUT _(INPUT input) { return input; }
  };

  template<typename OUTPUT>
  struct lexical_caster<OUTPUT, std::string, false>
  {
    static inline OUTPUT _(const std::string& input) { return Comet::from_string<OUTPUT>(input); }
  };

  template<typename INPUT>
  struct lexical_caster<std::string, INPUT, false>
  {
    static inline std::string _(INPUT input) { return std::to_string(input); }
  };

  template<typename OUTPUT, typename INPUT>
  OUTPUT lexical_cast(INPUT input)
  {
    return lexical_caster<OUTPUT, INPUT, std::is_same<OUTPUT, INPUT>::value >::_(input);
  }
}

#endif
