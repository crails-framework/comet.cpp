#ifndef  COMET_EXCEPTION_HPP
# define COMET_EXCEPTION_HPP

# include <exception>

namespace Comet
{
  void raise(const std::exception& e);
}

#endif
