#include "bindable.hpp"
#include "globals.hpp"
#include "from_string.hpp"

using namespace Comet;

Bindable& Bindable::use_mode(BindMode mode, const std::string& parameter)
{
  bind_mode = mode;
  switch (mode)
  {
  case ThrottleBind:
    throttle_refresh = Comet::from_string<unsigned long>(parameter);
    break ;
  case SignalBind:
    signal_name = parameter;
    break ;
  case StaticBind:
    break ;
  }
  return *this;
}

void Bindable::enable(Comet::Signal<std::string>& signal)
{
  enabled = true;
  switch (bind_mode)
  {
  case StaticBind:
    break ;
  case SignalBind:
    listen_to(signal, [this](const std::string& name) {
      if (name == signal_name)
        update();
    });
    break ;
  case ThrottleBind:
    enabled_ptr = std::make_shared<bool>(true);
    throttle_schedule();
    break ;
  }
  update();
}

void Bindable::disable()
{
  enabled = false;
  switch (bind_mode)
  {
  case StaticBind:
  case ThrottleBind:
    (*enabled_ptr) = false;
    break ;
  case SignalBind:
    stop_listening();
    break ;
  }
}

void Bindable::update()
{
  if (has_updater)
  {
    __asm__("try {");
    updater();
    __asm__("} catch(error) { console.error('[Comet] failed to update Bindable:', error); }");
  }
  else if (!element.is_undefined())
  {
    if (target == "text")
      element.text(get_value());
    else
      element.attr(target, get_value());
  }
}

std::string Bindable::get_value() const
{
  std::string value;

  __asm__("try {");
  value = getter();
  __asm__("} catch (err) { console.warn('failed to update attribute', err); }");
  return value;
}

void Bindable::throttle_schedule()
{
  std::shared_ptr<bool> _enabled_ptr = enabled_ptr;

  Comet::window.set_timeout([this, _enabled_ptr]()
  {
    if (*_enabled_ptr)
    {
      update();
      throttle_schedule();
    }
  }, throttle_refresh);
}
