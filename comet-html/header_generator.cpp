#include "header_generator.hpp"
#include "repeater.hpp"
#include "slot.hpp"
#include <sstream>
#include <iostream>

using namespace std;

const char* indent = "  ";

static void generate_friends(stringstream& stream, Class& object)
{
  for (const auto& klass : Context::global.classes)
  {
    if (klass.get() != &object)
      stream << indent << "  friend class " << klass->get_typename() << ';' << endl;
  }
}

static void generate_bound_attributes_definitions(stringstream& stream, Class& object)
{
  stream << indent << "  Comet::BoundAttributes bound_attributes;" << endl;
}

static void generate_anchors_definitions(stringstream& stream, Class& object)
{
  unsigned short anchor_count = 0;

  for (const auto& child : object.collect_children())
  {
    if (child->is_anchorable())
    {
      stringstream anchor_name;

      anchor_name << "anchor_" << ++anchor_count;
      child->set_anchor_name(anchor_name.str());
      stream << indent << "  Comet::CommentElement " << anchor_name.str() << ';' << endl;
    }
  }
}

static void generate_signaler_definition(stringstream& stream, Class& object)
{
  stream << indent << "  Comet::Signal<std::string>";
  if (object.get_parent() != nullptr) stream << '&';
  stream << " signaler;" << endl;
}

static void append_reference(stringstream& stream, const ReferenceBase& reference)
{
  auto type = Context::global.find_cpp_type(reference.get_element().name());
  stream << indent << "  " << reference.get_type() << ' ' << reference.get_name();
  if (reference.has_default_value())
    stream << " = " << reference.get_default_value();
  stream << ';' << endl;
}

static void generate_private_properties(stringstream& stream, Class& object)
{
  for (const auto& reference : object.get_references())
  {
    if (reference->is_implicit())
      append_reference(stream, *reference);
  }
}

static void generate_protected_properties(stringstream& stream, Class& object)
{
  stream << indent << "  " << object.root()->get_typename() << "* root;" << endl;
  for (const auto& reference : object.get_references())
  {
    if (reference->is_explicit())
      append_reference(stream, *reference);
  }
  for (const auto& repeater_ptr : object.get_repeaters())
  {
    const Repeater& repeater = reinterpret_cast<const Repeater&>(*repeater_ptr.get());

    stream << indent << "  " << "Comet::Repeater<" << repeater.get_list_type() << ", " << repeater.get_typename() << "> " << repeater.get_name() << ';' << endl;
  }
}

static void generate_public_properties(stringstream& stream, const Class& object)
{
  if (!object.is_root())
    stream << indent << "  " << object.get_parent()->get_typename() << "* parent;" << endl;
  if (object.is_repeater())
  {
    const Repeater& repeater = reinterpret_cast<const Repeater&>(object);

    stream << indent << "  " << repeater.get_value_type() << ' ' << repeater.get_value_name() << ';' << endl;
  }
}

static void generate_root_slots(stringstream& stream, const Class& object)
{
  std::vector<const Slot*> slots;

  for (const auto& slot : object.get_slots())
    slots.push_back(reinterpret_cast<const Slot*>(slot.get()));
  for (const auto& child : object.recursively_collect_children())
  {
    for (const auto& slot : child->get_slots())
      slots.push_back(reinterpret_cast<const Slot*>(slot.get()));
  }
  for (const Slot* slot : slots)
    stream << indent << "  " << "Comet::SlotElement " << slot->get_slot_ref() << ';' << endl;
}

static void generate_child_slots(stringstream& stream, const Class& object)
{
  for (const auto& child : object.get_slots())
  {
    const Slot* slot = reinterpret_cast<const Slot*>(child.get());

    stream << indent << "  " << "Comet::SlotElement& " << slot->get_slot_ref() << ';' << endl;
  }
}

static void generate_getter_setters(stringstream& stream, const Class& object)
{
  for (const auto& reference : object.get_references())
  {
    bool is_ptr = *reference->get_type().rbegin() == '*';

    if (reference->has_getter())
    {
      if (is_ptr)
        stream << indent << "  " << reference->get_type() << " get_" << reference->get_name() << "() const { return " << reference->get_name() << "; }" << endl;
      else
        stream << indent << "  const " << reference->get_type() << "& get_" << reference->get_name() << "() const { return " << reference->get_name() << "; }" << endl;
    }
    if (reference->has_setter())
    {
      if (is_ptr)
        stream << indent << "  virtual void set_" << reference->get_name() << '(' << reference->get_type() << " __v) { " << reference->get_name() << " = __v; }" << endl;
      else
        stream << indent << "  virtual void set_" << reference->get_name() << "(const " << reference->get_type() << "& __v) { " << reference->get_name() << " = __v; }" << endl;
    }
  }
}

string HeaderGenerator::generate()
{
  stringstream stream;

  stream << indent << "class " << object.get_typename() << " : public " << object.get_superclass() << endl
         << indent << '{' << endl;
  generate_friends(stream, object);
  stream << indent << "protected:" << endl;
  if (!object.implements_ibindable_view())
    generate_bound_attributes_definitions(stream, object);
  generate_protected_properties(stream, object);
  stream << indent << "private:" << endl;
  generate_anchors_definitions(stream, object);
  generate_private_properties(stream, object);
  stream << indent << "public:" << endl;
  if (!object.implements_ibindable_view())
    generate_signaler_definition(stream, object);
  generate_public_properties(stream, object);
  if (object.is_root())
    generate_root_slots(stream, object);
  else
    generate_child_slots(stream, object);
  stream << indent << "  " << object.constructor_declaration() << endl
         << indent << "  " << "virtual ~" << object.get_typename() << "() {}" << endl
         << endl
         << indent << "  " << "void bind_attributes();" << endl
         << indent << "  " << "void trigger_binding_updates();" << endl;
  generate_getter_setters(stream, object);
  stream << object.get_inline_code();

  stream << indent << "};" << endl;
  return stream.str();
}
