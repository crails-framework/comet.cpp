#include "document.hpp"
#include "globals.hpp"
#include <iostream>

using namespace Comet;
using namespace std;

Comet::Document Comet::document;

Document::Document() : ObjectImpl(&(client::document)), events(make_shared<JavascriptEvents>((client::EventTarget*)&client::document))
{
  auto callback = [this]() { on_ready_state_changed(); };

  events->on("readystatechange", callback);
  client::window.setTimeout(cheerp::Callback(callback), 0);
}

Document::ReadyState Document::get_ready_state()
{
  client::String* str = (*this)->get_readyState();
  std::string ready_state = (std::string)(*(str));

  if (ready_state == "complete")
    return Complete;
  else if (ready_state == "interactive")
    return Interactive;
  return Loading;
}

void Document::on_ready(function<void ()> callback)
{
  switch (get_ready_state())
  {
  case Loading:
    on_ready_callbacks.push_back(callback);
    break ;
  default:
    client::window.setTimeout(cheerp::Callback(callback), 0);
    break ;
  }
}

void Document::on_ready_state_changed()
{
  switch (get_ready_state())
  {
  case Interactive:
  case Complete:
    on_loading_ended();
    break ;
  default:
    break ;
  }
}

void Document::on_loading_ended()
{
  body = Element((*this)->get_body());
  for (auto callback : on_ready_callbacks)
    callback();
  on_ready_callbacks.empty();
}
