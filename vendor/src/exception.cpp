#include "exception.hpp"
#include "globals.hpp"

namespace Comet
{
  void raise(const std::exception& e)
  {
    String what(e.what());

    window.set("_comet_exception", what);
    __asm__("throw window._comet_exception");
  }
}
