#pragma once
#include <memory>
#include <sstream>
#include <vector>
#include <pugixml/pugixml.hpp>
#include "context.hpp"
#include "reference.hpp"

class Generator;
class Probe;
class EventListener;
class Binding;

class Class : public Context, public std::enable_shared_from_this<Class>
{
  friend struct Context;
  friend class Generator;
  friend class Probe;
protected:
  typedef std::shared_ptr<ReferenceBase> ReferencePtr;
  typedef std::shared_ptr<Class>         ClassPtr;
  typedef std::shared_ptr<EventListener> EventListenerPtr;
  typedef std::shared_ptr<Binding>       BindingPtr;
  typedef std::vector<ClassPtr>          Children;
  typedef std::vector<ReferencePtr>      References;
  typedef std::vector<EventListenerPtr>  EventListeners;
  typedef std::vector<BindingPtr>        Bindings;

  pugi::xml_node         element;
  std::string            type_name;
  Children               children;
  References             references;
  ClassPtr               parent;
  std::string            super_class;
  Children               slots, slot_plugins, repeaters;
  EventListeners         event_listeners;
  Bindings               bindings;
  std::string            anchor_name;
  std::string            inline_code;
public:
  Class(pugi::xml_node element, std::shared_ptr<Class> parent = nullptr);
  virtual ~Class() override {}

  bool                  is_root() const { return parent == nullptr; }
  virtual bool          is_anchorable() const { return false; }
  virtual bool          is_repeater() const { return false; }
  virtual bool          should_skip() const { return false; }
  virtual bool          blocks_remote_references() const { return false; }
  virtual bool          implements_ibindable_view() const { return false; }
  bool                  inherits_binding_methods() const;
  virtual std::string   constructor_declaration() const;
  ReferencePtr          find_reference_for(pugi::xml_node element) const;
  ClassPtr              find_class_for(pugi::xml_node element) const;
  Children              collect_children() const;
  Children              recursively_collect_children() const;
  ClassPtr              root();
  bool                  is_node_a_subtype_of_this(const pugi::xml_node&) const;
  const std::string&    get_typename() const { return type_name; }
  const std::string&    get_superclass() const { return super_class; }
  ClassPtr              get_parent() const { return parent; }
  const References&     get_references() const { return references; }
  const Bindings&       get_bindings() const { return bindings; }
  const EventListeners& get_event_listeners() const { return event_listeners; }
  const Children&       get_slots() const { return slots; }
  const Children&       get_slot_plugins() const { return slot_plugins; }
  const Children&       get_repeaters() const { return repeaters; }
  const pugi::xml_node& get_element() const { return element; }
  const std::string&    get_anchor_name() const { return anchor_name; }
  const std::string&    get_inline_code() const { return inline_code; }
  void                  append_reference(std::shared_ptr<ReferenceBase> value) { references.push_back(value); }

  virtual void create_reference(pugi::xml_node element, Reference::Mode mode = Reference::UnspecifiedReferenceMode)
  {
    if (find_reference_for(element) == nullptr)
      references.push_back(std::make_shared<Reference>(element, this, mode));
  }

  void set_anchor_name(const std::string& value) { anchor_name = value; }
};
