#pragma once
#include "class.hpp"
#include "anchor.hpp"
#include "binding.hpp"

bool is_valid_cpp_variable_name(const std::string&);

class Repeater : public Class
{
  std::string repeater_name;
  std::string value_type, value_name;
  std::string list_type, list_name;
  std::string bind_mode;
  //Anchor      anchor; // TODO is this used ?

  std::pair<std::string,std::string> generate_type_and_name()
  {
    std::stringstream stream_type, stream_name;

    stream_type << (*Context::global.classes.begin())->get_typename()
                << "Repeatable_" << Context::global.repeater_count;
    stream_name << "repeater_" << Context::global.repeater_count;
    Context::global.repeater_count++;
    return {stream_type.str(), stream_name.str()};
  }
public:
  Repeater(pugi::xml_node element, ClassPtr parent) : Class(element, parent)
  {
    auto type_and_name       = generate_type_and_name();
    auto value_and_bind_mode = Binding::extract_bind_mode_from(element.attribute("repeat.for").value());

    type_name     = type_and_name.first;
    repeater_name = type_and_name.second;
    bind_mode     = value_and_bind_mode.second;
    extract_symbols(value_and_bind_mode.first);
    if (!is_valid_cpp_variable_name(value_name))
      throw ParseError(element, "invalid variable name `" + value_name + '`');
  }

  void extract_symbols(const std::string& value)
  {
    static const std::regex pattern("^\\s*(\\[([^\\]]+)\\])?\\s*([^\\s]+)\\s+of\\s+\\[([^\\]]+)\\]\\s*(.*)$", std::regex_constants::ECMAScript | std::regex_constants::optimize);
    auto match = std::sregex_iterator(value.begin(), value.end(), pattern);

    if (match != std::sregex_iterator())
    {
      value_name = value.substr(match->position(3), match->length(3));
      list_type  = value.substr(match->position(4), match->length(4));
      list_name  = value.substr(match->position(5), match->length(5));
      value_type = match->length(2) == 0 ? list_type + "::value_type" : value.substr(match->position(2), match->length(2));
    }
    else
      throw ParseError(element, "invalid repeater definition `" + value + '`');
  }

  bool        is_anchorable() const override { return true; }
  bool        is_repeater() const override { return true; }
  bool        blocks_remote_references() const override { return true; }
  std::string constructor_declaration() const override { return get_typename() + '(' + parent->get_typename() + "*, " + value_type + ");"; }

  const std::string& get_list_type() const { return list_type; }
  const std::string& get_list_name() const { return list_name; }
  const std::string& get_value_type() const { return value_type; }
  const std::string& get_value_name() const { return value_name; }
  const std::string& get_name() const { return repeater_name; }
  const std::string& get_bind_mode() const { return bind_mode; }
};
