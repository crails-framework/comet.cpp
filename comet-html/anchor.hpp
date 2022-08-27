#pragma once
#include <pugixml/pugixml.hpp>
#include <utility>

enum AnchorMode
{
  ChildrenAnchor,
  AppendAnchor,
  PrependAnchor
};

struct Anchor : public std::pair<pugi::xml_node, AnchorMode>
{
  typedef AnchorMode Mode;

  Anchor(pugi::xml_node element, Mode mode) : std::pair<pugi::xml_node, Mode>(element, mode)
  {
  }
};
