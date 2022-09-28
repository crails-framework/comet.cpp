#pragma once
#include <pugixml/pugixml.hpp>
#include <regex>
#include "class.hpp"

class Binding
{
  pugi::xml_node         element;
  std::shared_ptr<Class> parent;
  bool                   is_cpp;
  bool                   plain_setter = false;
  std::string            attribute_name;
  std::string            bind_mode;
  std::string            code;
public:
  Binding(pugi::xml_node element, Class& parent_, const std::string& name, const std::string& value) : element(element), parent(parent_.shared_from_this())
  {
    auto value_and_bind_mode = extract_bind_mode_from(value);

    code           = value_and_bind_mode.first;
    bind_mode      = value_and_bind_mode.second;
    is_cpp         = name.substr(0, 5) == "cpp::";
    attribute_name = name;
    plain_setter = name.find(".bind") == std::string::npos;
    if (!plain_setter)
      attribute_name.erase(name.length() - std::string(".bind").length());
    if (is_cpp)
      attribute_name.erase(0, 5);
    if (parent_.find_reference_for(element) == nullptr)
      parent_.create_reference(element);
  }

  const pugi::xml_node& get_element() const { return element; }
  const std::string&    get_attribute_name() const { return attribute_name; }
  const std::string&    get_code() const { return code; }
  const std::string&    get_bind_mode() const { return bind_mode; }

  bool binds_to_cpp_property() const { return is_cpp; }
  bool is_plain_setter() const { return plain_setter; }

  static std::pair<std::string, std::string> extract_bind_mode_from(std::string value)
  {
    static const std::regex bind_mode_pattern("\\s*&\\s*(throttle|signal):([a-zA-Z0-9_-]+)$", std::regex_constants::optimize);
    std::string bind_mode;
    auto        match = std::sregex_iterator(value.begin(), value.end(), bind_mode_pattern);

    if (match != std::sregex_iterator())
    {
      std::string bind_type   = value.substr(match->position(1), match->length(1));
      std::string bind_target = value.substr(match->position(2), match->length(2));

      value.erase(match->position(0));
      if      (bind_type == "signal")   bind_type = "SignalBind";
      else if (bind_type == "throttle") bind_type = "ThrottleBind";
      else                              bind_type = "StaticBind";
      bind_mode = ".use_mode(Comet::Bindable::" + bind_type + ", \"" + bind_target + "\")";
    }
    return {value, bind_mode};
  }
};
