#include "document.hpp"
#include "globals.hpp"

using namespace Comet;
using namespace std;

Comet::Document Comet::document;

Document::Document() : ObjectImpl(&(client::document))
{
  operator*()->addEventListener("DOMContentLoaded", cheerp::Callback([](client::Event*)
  {
    body = Element(client::document.get_body());
  }));
}

void Document::on_ready(function<void ()> callback)
{
  std::string readyState = get<client::String>("readyState");

  if (readyState != "loading")
    callback();
  else
  {
    operator*()->addEventListener("DOMContentLoaded", cheerp::Callback([callback](client::Event* event_ptr)
    {
      callback();
    }));
  }
}
