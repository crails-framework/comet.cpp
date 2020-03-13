#ifndef  COMET_MVC_CONTROLLER_HPP
# define COMET_MVC_CONTROLLER_HPP

# include "../promise.hpp"
# include "../http.hpp"
# include "../router.hpp"
# include "layout.hpp"

# define match_route(ROUTE, CONTROLLER, METHOD) \
  match(ROUTE, [](const Params& p) \
  { \
    auto controller = std::make_shared<CONTROLLER>(p); \
    controller->initialize().then([controller]() \
    { \
      controller->METHOD(); \
      controller->finalize().then([controller]() {}); \
    }); \
  });

namespace Comet
{
  class Controller
  {
  protected:
    const Params& params;

  public:
    Controller(const Params& p) : params(p)
    {
    }

    virtual std::string get_application_name()
    {
      return "main";
    }

    template<typename ELEMENT>
    Layout<ELEMENT>& use_layout()
    {
      auto& layout = get_layout<ELEMENT>();

      layout.attach_as_layout(get_application_name());
      return layout;
    }

    template<typename ELEMENT>
    Layout<ELEMENT>& get_layout()
    {
      static Layout<ELEMENT> layout;

      return layout;
    }

    Comet::Promise initialize() { return Promise::solved_promise(); }
    Comet::Promise finalize()   { return Promise::solved_promise(); }
  };
}

#endif
