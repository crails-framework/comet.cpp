#include "class.hpp"
#include <iostream>
using namespace std;

Class::Class(pugi::xml_node element, shared_ptr<Class> parent) : element(element), parent(parent)
{
  super_class = is_root() ? Context::global.template_base_type() : Context::global.template_base_subtype();
}

string Class::constructor_declaration() const
{
  stringstream stream;

  stream << type_name << '(';
  if (parent != nullptr)
    stream << parent->type_name << '*';
  stream << ')' << ';';
  return stream.str();
}

shared_ptr<ReferenceBase> Class::find_reference_for(pugi::xml_node element) const
{
  for (const auto& reference : references)
  {
    if (reference->get_element().path() == element.path())
      return reference;
  }
  return element.path() == this->element.path() ? make_shared<ThisReference>() : nullptr;
}

shared_ptr<Class> Class::find_class_for(pugi::xml_node element) const
{
  for (ClassPtr child : collect_children())
  {
    if (child->element.path() == element.path())
      return child;
  }
  return nullptr;
}

vector<shared_ptr<Class>> Class::collect_children() const
{
  Children children;

  for (ClassPtr class_ : Context::global.classes)
  {
    if (class_->parent.get() == this)
      children.push_back(class_);
  }
  return children;
}

vector<shared_ptr<Class>> Class::recursively_collect_children() const
{
  Children children;

  for (ClassPtr child : collect_children())
  {
    Children child_children = child->recursively_collect_children();

    children.push_back(child);
    children.insert(children.end(), child_children.begin(), child_children.end());
  }
  return children;
}

shared_ptr<Class> Class::root()
{
  ClassPtr current_class = shared_from_this();

  while (!current_class->is_root())
    current_class = current_class->parent;
  return current_class;
}

bool Class::is_node_a_subtype_of_this(const pugi::xml_node& element) const
{
  for (ClassPtr child : collect_children())
  {
    if (child->get_element() == element)
      return true;
  }
  return false;
}

bool Class::inherits_binding_methods() const
{
  return Context::global.template_base_subtype() != super_class && Context::global.template_base_type() != super_class;
}
