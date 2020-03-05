#ifndef  COMET_COMMENT_ELEMENT_HPP
# define COMET_COMMENT_ELEMENT_HPP

# include "element.hpp"

namespace Comet
{
  class CommentElement : public Comet::Element
  {
    static client::HTMLElement* makeCommentElement(const client::String&);
  public:
    CommentElement(const client::String& str) : Element(makeCommentElement(str)) {}
    CommentElement() : Element(makeCommentElement("")) {}
  };
}

#endif
