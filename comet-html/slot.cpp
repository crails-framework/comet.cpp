#include "slot.hpp"
#include <sstream>
#include <iostream>

using namespace std;

bool is_valid_cpp_variable_name(const std::string&);

Anchor find_anchorable_anchor(pugi::xml_node element)
{
  pugi::xml_node it = element;

  do { it = it.previous_sibling(); }
  while (it.type() != pugi::node_null && it.name() != string("text") && it.name() != string("comment"));
  if (it.type() != pugi::node_null)
    return {it, AppendAnchor};
  it = element;
  do { it = it.next_sibling(); }
  while (it.type() != pugi::node_null && it.name() != string("text") && it.name() != string("comment"));
  if (it.type() != pugi::node_null)
    return {it, PrependAnchor};
  return {element.parent(), ChildrenAnchor};
}

std::string SlotBase::generate_slot_typename(const std::string& slot_type)
{
  std::stringstream stream;

  stream << (*Context::global.classes.begin())->get_typename()
          << slot_type << '_' << Context::global.slot_count;
  Context::global.slot_count++;
  return stream.str();
}

Class::ClassPtr SlotBase::find_remote_reference_holder() const
{
  auto class_ = parent;

  while (!class_->is_root())
  {
    if (class_->blocks_remote_references()) break;
    class_ = class_->get_parent();
  }
  return class_;
}

std::string SlotBase::path_to_reference_holder(Class& context) const
{
  auto holder = find_remote_reference_holder();
  string path = "this";

  if (!holder)
    return "root";
  if (holder.get() != &context)
  {
    auto class_ = parent;

    while (class_ != holder && !class_->is_root())
    {
      path += "->parent";
      class_ = class_->get_parent();
    }
  }
  return path;
}

void SlotBase::create_reference(pugi::xml_node element, Reference::Mode mode)
{
  ClassPtr root_reference;

  if (mode == Reference::ImplicitReference || (root_reference = find_remote_reference_holder()) == nullptr)
    Class::create_reference(element, mode);
  else
  {
    shared_ptr<RemoteReference> remote_reference;

    root_reference->create_reference(element, mode);
    remote_reference = make_shared<RemoteReference>(
      element,
      this,
      root_reference->find_reference_for(element),
      mode
    );
    references.push_back(remote_reference);
  }
}

Slot::Slot(pugi::xml_node element, std::shared_ptr<Class> parent) : SlotBase(element, parent), anchor(find_anchorable_anchor(element))
{
  type_name = generate_slot_typename("Slot");
  slot_name = element.attribute("name").value();
  slot_ref  = "slot_" + slot_name;
  this->parent = parent->root();
  if (!is_valid_cpp_variable_name(slot_name))
    throw ParseError(element, "slot name `" + slot_name + "` is not a valid C++ variable name");
}

SlotPlugin::SlotPlugin(pugi::xml_node element, std::shared_ptr<Class> parent, pugi::xml_node on_element) : SlotBase(element, parent), on_element(on_element)
{
  ClassPtr ref_root = find_remote_reference_holder();

  slot_name   = element.attribute("slot").value();
  type_name   = generate_slot_typename("SlotPlugin");
  super_class = Context::global.find_cpp_type(element.name(), Context::global.template_base_subtype());
  if (is_custom_element())
    Context::global.use_cpp_type(element.name());
  if (has_reference())
  {

    if (!ref_root)
    {
      cerr << "[comet-html] ignoring ref at " << Context::global.file_path.string() << ':' << endl;
      element.print(cout);
    }
    else
    {
      auto reference = make_shared<CppReference>(element, type_name, element.attribute("ref").value());

      this->parent = ref_root;
      reference->set_initializer(path_to_reference_holder(*this->parent));
      reference->disable_setter();
      ref_root->append_reference(reference);
    }
  }
}

string SlotPlugin::constructor_declaration() const
{
  ClassPtr ref_root = find_remote_reference_holder();
  stringstream stream;

  stream << type_name << '(' << parent->get_typename() << '*' << ')' << ';';
  return stream.str();
}
