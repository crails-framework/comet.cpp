#include "window.hpp"
#include "globals.hpp"

using namespace Comet;
using namespace std;

Comet::Window Comet::window;

Window::Window() : ObjectImpl((client::Window*)&client::window), events(make_shared<JavascriptEvents>((client::EventTarget*)&client::window))
{
  events->on("resize", [this]() { resized.trigger(); });
}
