#include "window.hpp"
#include "globals.hpp"

using namespace Comet;
using namespace std;

Comet::Window Comet::window;

Window::Window() : ObjectImpl((client::Window*)&client::window), events(make_shared<JavascriptEvents>((client::EventTarget*)&client::window))
{
  events->on("resize", [this]() { resized.trigger(); });
}

void Window::set_timeout(function<void()> callback, double interval) const
{
  client::window.setTimeout(cheerp::Callback(callback), interval);
}

void Window::set_interval(function<void()> callback, double interval) const
{
  client::window.setInterval(cheerp::Callback(callback), interval);
}
