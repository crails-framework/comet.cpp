#ifndef  COMET_DOCUMENT_HPP
# define COMET_DOCUMENT_HPP

# include "object.hpp"

namespace Comet
{
  class Document : public ObjectImpl<client::Document>
  {
  public:
    Document();

    void on_ready(std::function<void ()> callback);
  };
}

#endif
