#include "probe.hpp"
#include "slot.hpp"
#include "repeater.hpp"
#include "binding.hpp"
#include "event_listener.hpp"
#include <regex>
#include <iostream>

using namespace std;

static bool is_slot       (const pugi::xml_node& element) { return element.name() == string("slot"); }
static bool is_slot_plugin(const pugi::xml_node& element) { return !element.attribute("slot").empty(); }
static bool is_repeater   (const pugi::xml_node& element) { return !element.attribute("repeat.for").empty(); }
static bool has_cpp_class_attribute(const pugi::xml_node& element) { return !element.attribute("_cheerp_class").empty(); }

void Probe::probe(Class& object)
{
  probe_subtypes(object);
  probe_slots_plugins(object);
  probe_references(object);
  probe_bindings(object);
}

void Probe::probe(SlotPlugin& object)
{
  probe(reinterpret_cast<Class&>(object));
  probe_bindings_for(object, object.element);
}

void Probe::probe(Repeater& object)
{
  probe(reinterpret_cast<Class&>(object));
  probe_bindings_for(object, object.element);
}

void Probe::probe_subtypes(Class& object, const pugi::xml_node* root)
{
  if (!root) root = &object.element;
  for (const pugi::xml_node& element : root->children())
  {
    if (Context::global.has_cpp_type(element))
      Context::global.use_cpp_type(element.name());
    if (is_slot(element))
    {
      auto slot = Context::make<Slot>(element, object.shared_from_this());

      object.slots.push_back(slot);
      probe(*slot);
    }
    else if (is_repeater(element))
    {
      auto repeater = Context::make<Repeater>(element, object.shared_from_this());

      object.repeaters.push_back(repeater);
      probe(*repeater);
    }
    else if (!Context::global.has_cpp_type(element))
      probe_subtypes(object, &element);
  }
}

void Probe::probe_slots_plugins(Class& object, const pugi::xml_node* root)
{
  if (!root) root = &object.element;
  for (pugi::xml_node& element : root->children())
  {
    if (has_cpp_class_attribute(element)) continue ;
    if (Context::global.has_cpp_type(element))
    {
      for (const pugi::xml_node& candidate : element.children())
      {
        if (is_slot_plugin(candidate))
        {
          auto slot_plugin = Context::make<SlotPlugin>(candidate, object.shared_from_this(), element);

          object.slot_plugins.push_back(slot_plugin);
          probe(*slot_plugin);
        }
        else
          element.remove_child(candidate); // Non-slot children for cpp custom elements must be ignored
      }
    }
    else
      probe_slots_plugins(object, &element);
  }
}

void Probe::probe_references(Class& object, const pugi::xml_node* root)
{
  if (!root) root = &object.element;
  for (const pugi::xml_node& element : root->children())
  {
    if (has_cpp_class_attribute(element)) continue ;
    if (object.find_reference_for(element) != nullptr) continue ;
    if (!element.attribute("ref").empty())
      object.create_reference(element, Reference::ExplicitReference);
    else if (Context::global.has_cpp_type(element))
      object.create_reference(element, Reference::ImplicitReference);
    if (!Context::global.has_cpp_type(element))
      probe_references(object, &element);
  }
}

void Probe::probe_bindings(Class& object, const pugi::xml_node* root)
{
  if (!root) root = &object.element;
  for (const pugi::xml_node& child : root->children())
  {
    if (has_cpp_class_attribute(child) || object.is_node_a_subtype_of_this(child)) continue ;
    probe_bindings_for(object, child);
    if (!Context::global.has_cpp_type(child))
      probe_bindings(object, &child);
  }
}

void Probe::probe_bindings_for(Class& object, const pugi::xml_node& element)
{
  static const regex listener_regex("\\.trigger$", std::regex_constants::optimize);
  static const regex binding_regex("\\.bind$", std::regex_constants::optimize);
  static const regex cpp_setter_regex("^cpp::", std::regex_constants::optimize);

  for (const pugi::xml_attribute& attribute : element.attributes())
  {
    string name(attribute.name());

    if (sregex_iterator(name.begin(), name.end(), listener_regex) != sregex_iterator())
      object.event_listeners.push_back(make_shared<EventListener>(element, object, attribute.name(), attribute.value()));
    else if (sregex_iterator(name.begin(), name.end(), binding_regex) != sregex_iterator()
         || (sregex_iterator(name.begin(), name.end(), cpp_setter_regex) != sregex_iterator())
    )
      object.bindings.push_back(make_shared<Binding>(element, object, attribute.name(), attribute.value()));
  }
}
