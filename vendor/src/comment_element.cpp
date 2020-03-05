#include "comment_element.hpp"
#include "globals.hpp"

using namespace Comet;

client::HTMLElement* CommentElement::makeCommentElement(const client::String& str)
{
  client::Comment* comment = Comet::document->createComment(str);

  return reinterpret_cast<client::HTMLElement*>(comment);
}
