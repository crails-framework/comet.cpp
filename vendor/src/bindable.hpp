#ifndef  COMET_BINDABLE_HPP
# define COMET_BINDABLE_HPP

# include "signal.hpp"
# include "element.hpp"

namespace Comet
{
  struct IBindableView : public Comet::Element
  {
    IBindableView() : Comet::Element("div") {}
    IBindableView(const client::String& tagName) : Comet::Element(tagName) {}

    virtual void bind_attributes() = 0;
    virtual void trigger_binding_updates() = 0;
  private:
    IBindableView(const IBindableView&) {}
  };

  struct BindableUpdater
  {
    std::function<void()> func;

    void execute()
    {
      if (func)
        func();
      else
        el.attr(target, getter());
    }

    Comet::Element el;
    const std::string target;
    std::function<std::string (void)> getter;
  };

  class Bindable : public Comet::Listener
  {
  public:
    typedef std::function<std::string (void)> Getter;

    enum BindMode
    {
      StaticBind,
      SignalBind,
      ThrottleBind
    };

  private:
    BindMode               bind_mode = StaticBind;
    Comet::Element element;
    std::string            target;
    Getter                 getter;
    bool                   has_updater = false;
    std::function<void()>  updater;

    std::string            signal_name;
    unsigned long          throttle_refresh;
    bool                   enabled = false;
    std::shared_ptr<bool>  enabled_ptr;

  public:
    Bindable() {}
    Bindable(Comet::Element el, const std::string& t, Getter g) : element(el), target(t), getter(g) {}
    Bindable(std::function<void ()> func) : has_updater(true), updater(func) {}

    ~Bindable() { disable(); }

    inline Bindable& bind_to(Comet::Element el) { element = el; return *this; }
    inline Bindable& on_attribute(const std::string& t, Getter g) { target = t; getter = g; return *this; }
    Bindable& use_mode(BindMode, const std::string& parameter);

    void enable(Comet::Signal<std::string>& signal);
    void disable();
    void update();

  private:
    std::string get_value() const;
    void throttle_schedule();
  };

  class BoundAttributes : public std::vector<Bindable>
  {
  public:
    void update()
    {
      for (Bindable& bindable : *this)
        bindable.update();
    }

    void enable(Comet::Signal<std::string>& signaler)
    {
      disable();
      for (Bindable& bindable : *this)
        bindable.enable(signaler);
    }

    void disable()
    {
      for (Bindable& bindable : *this)
        bindable.disable();
    }
  };
}

#endif
