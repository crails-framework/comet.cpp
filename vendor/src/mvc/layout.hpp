#ifndef  COMET_MVC_LAYOUT_HPP
# define COMET_MVC_LAYOUT_HPP

# include "../globals.hpp"

namespace Comet
{
  template<typename ELEMENT>
  class Layout : public ELEMENT
  {
  public:
    void attach_as_layout()
    {
      body.html("").inner({*this});
      ELEMENT::bind_attributes();
      ELEMENT::trigger_binding_updates();
    }

    template<typename VIEW_ELEMENT>
    void render(std::shared_ptr<VIEW_ELEMENT> el)
    {
      ELEMENT::slot_content.set_element(el);
    }
  };
}

#endif
