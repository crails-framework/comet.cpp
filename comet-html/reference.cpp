#include "reference.hpp"
#include "class.hpp"
#include <iostream>

bool is_valid_cpp_variable_name(const std::string& value);

CppReference::CppReference(pugi::xml_node element, const std::string& type, const std::string& name, const std::string& default_value)
  : ReferenceBase(element, type, name)
{
  this->default_value = default_value;
  if (type.length() == 0 || name.length() == 0)
    throw ParseError(element, "incomplete attribute definition");
  if (!is_valid_cpp_variable_name(name))
    throw ParseError(element, "attribute name `" + name + "` is not a valid C++ variable name");
}

Reference::Reference(pugi::xml_node element, Class* parent, Mode mode) : ReferenceBase(element, "", ""), parent(parent)
{
  type = Context::global.find_cpp_type(element.name());
  set_mode(mode);
}

void Reference::set_mode(Mode mode)
{
  if ((mode == UnspecifiedReferenceMode && !element.attribute("ref").empty()) || mode == ExplicitReference)
  {
    this->mode = ExplicitReference;
    name = element.attribute("ref").value();
    if (!is_valid_cpp_variable_name(name))
      throw ParseError(element, "ref `" + name + "` is not a valid C++ variable name.");
  }
  else
  {
    this->mode = ImplicitReference;
    name = "element_" + parent->generate_new_reference_name();
  }
}

std::string RemoteReference::get_initializer(const std::string& root_getter) const
{
  if (reference_to == nullptr)
    throw std::runtime_error("RemoteReference has null reference_to");
  return root_getter + "->" + reference_to->get_name();
}
