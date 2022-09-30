#pragma once
#include "class.hpp"
#include "anchor.hpp"

class SlotBase : public Class
{
protected:
  static std::string generate_slot_typename(const std::string& slot_type);
public:
  SlotBase(pugi::xml_node element, std::shared_ptr<Class> parent = nullptr) : Class(element, parent) {}
  virtual ~SlotBase() override {}

  ClassPtr find_remote_reference_holder() const;
  void     create_reference(pugi::xml_node element, Reference::Mode mode) override;
  std::string path_to_reference_holder(Class& context) const;
};

class Slot : public SlotBase
{
  std::string slot_name, slot_ref;
  Anchor      anchor;
public:
  Slot(pugi::xml_node element, std::shared_ptr<Class> parent);

  bool is_anchorable() const override { return true; }
  const std::string& get_slot_name() const { return slot_name; }
  const std::string& get_slot_ref() const { return slot_ref; }
};

class SlotPlugin : public SlotBase
{
  pugi::xml_node on_element;
  std::string    slot_name;

public:
  SlotPlugin(pugi::xml_node element, std::shared_ptr<Class> parent, pugi::xml_node on_element);

  bool        implements_ibindable_view() const override { return !is_custom_element(); }
  bool        is_custom_element() const { return Context::global.has_cpp_type(element); }
  bool        has_reference() const { return !element.attribute("ref").empty(); }
  std::string constructor_params() const { return "this"; }
  std::string constructor_declaration() const override;
  const pugi::xml_node& get_on_element() const { return on_element; }
  const std::string&    get_slot_name() const { return slot_name; }
};
