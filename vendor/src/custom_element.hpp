#ifndef  COMET_HTML_TEMPLATE_HPP
# define COMET_HTML_TEMPLATE_HPP

# include "bindable.hpp"
# include "element.hpp"

namespace Comet
{
  class BindableEngineCommon : public IBindableView
  {
  protected:
    Comet::BoundAttributes bound_attributes;
    virtual Comet::Signal<std::string>& get_signaler() = 0;

  public:
    BindableEngineCommon(const client::String& tagName) : IBindableView(tagName) {}
    BindableEngineCommon() {}

    virtual void bind_attributes()
    {
      bound_attributes.enable(get_signaler());
    }

    virtual void trigger_binding_updates()
    {
      bound_attributes.update();
    }
  };

  class BindableEngine : public BindableEngineCommon
  {
  public:
    BindableEngine(const client::String& tagName) : BindableEngineCommon(tagName) {}
    BindableEngine() {}

    Comet::Signal<std::string> signaler;
  protected:
    Comet::Signal<std::string>& get_signaler() { return signaler; }

  };

  class BindableEngineClient : public BindableEngineCommon
  {
  public:
    BindableEngineClient(const client::String& tagName, Comet::Signal<std::string>& s) : BindableEngineCommon(tagName), signaler(s) {}
    BindableEngineClient(Comet::Signal<std::string>& s) : signaler(s) {}

    Comet::Signal<std::string>& signaler;
  protected:
    Comet::Signal<std::string>& get_signaler() { return signaler; }
  };

  class CustomElement : public BindableEngine
  {
  public:
    CustomElement(const client::String& tagName) : BindableEngine(tagName) {}
    CustomElement() {}
  };
}

#endif
