#ifndef  COMET_DOCUMENT_HPP
# define COMET_DOCUMENT_HPP

# include "object.hpp"
# include "signal.hpp"
# include "events.hpp"
# include <list>

namespace Comet
{
  class Document : public ObjectImpl<client::Document>
  {
  public:
    enum ReadyState { Loading, Interactive, Complete };

    std::shared_ptr<JavascriptEvents> events;

    Document();

    ReadyState get_ready_state();
    void on_ready(std::function<void ()> callback);

  private:
    void on_ready_state_changed();
    void on_loading_ended();

    std::list<std::function<void()>> on_ready_callbacks;
  };
}

#endif
