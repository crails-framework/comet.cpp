#pragma once
#include <pugixml/pugixml.hpp>
#include "class.hpp"

class EventListener
{
  pugi::xml_node         element;
  std::shared_ptr<Class> parent;
  bool                   is_cpp;
  std::string            attribute_name;
  std::string            code;
public:
  EventListener(pugi::xml_node element, Class& parent, const std::string& name, const std::string& value) : element(element), parent(parent.shared_from_this())
  {
    is_cpp         = name.substr(0, 5) == "cpp::";
    attribute_name = name.substr(0, name.length() - std::string(".trigger").length());
    if (is_cpp)
      attribute_name.erase(0, 5);
    code = value;
    if (parent.find_reference_for(element) == nullptr)
      parent.create_reference(element);
  }

  bool                  binds_to_cpp() const { return is_cpp; }
  const pugi::xml_node& get_element() const { return element; }
  const std::string&    get_attribute_name() const { return attribute_name; }
  const std::string&    get_code() const { return code; }
};
