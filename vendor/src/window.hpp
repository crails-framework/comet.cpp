#ifndef  CRAILS_FRONT_WINDOW_HPP
# define CRAILS_FRONT_WINDOW_HPP

# include "object.hpp"
# include "signal.hpp"
# include "events.hpp"

namespace Comet
{
  class Window : public ObjectImpl<client::Window>
  {
  public:
    std::shared_ptr<JavascriptEvents> events;

    Signal<void> resized;

    Window();

    void set_timeout(std::function<void()>, double interval) const;
    void set_interval(std::function<void()>, double interval) const;
  };
}

#endif
