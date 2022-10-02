#include "header_generator.hpp"
#include "repeater.hpp"
#include "slot.hpp"
#include "event_listener.hpp"
#include "binding.hpp"
#include <list>
#include <sstream>
#include <algorithm>
#include <crails/utils/semantics.hpp>

using namespace std;

string make_attrs_from_element(const pugi::xml_node& node);
void dom_generator(stringstream& stream, const pugi::xml_node& node, Class& object, unsigned short depth);

const string parent_symbol = "__comet_parent";

const std::map<AnchorMode, std::string> anchor_symbols{
  {ChildrenAnchor, "Comet::ChildrenAnchor"},
  {AppendAnchor,   "Comet::AppendAnchor"},
  {PrependAnchor,  "Comet::PrependAnchor"}
};

static std::string root_ptr_for(Class& object) { return object.is_root() ? "this" : "root"; }

static std::string tag_name_for(Class& object)
{
  if (object.is_root())
  {
    auto attribute = object.get_element().attribute("tag-name");
    return attribute.empty() ? Crails::dasherize(object.get_typename()) : string(attribute.value());
  }
  return object.get_element().name();
}

static std::string get_path_of_parents_until(const Class& object, std::function<bool(const Class&)> function)
{
  if (!function(object))
  {
    auto   current_object = object.get_parent();
    string result(parent_symbol);

    while (!function(*current_object))
    {
      result += "->parent";
      current_object = current_object->get_parent();
    }
    return result;
  }
  return "this";
}

static std::string get_root_from_parent_code(const Class& object)
{
  return get_path_of_parents_until(object, [](const Class& entry) { return entry.is_root(); });
}

static std::string get_reference_holder_from_parent_code(const Class& object)
{
  return get_path_of_parents_until(object, [](const Class& entry) { return entry.blocks_remote_references(); });
}

static std::string get_parent_from_parent_code(const Class& object)
{
  return get_path_of_parents_until(object, [&object](const Class& entry) { return &entry == &object; });
}

static list<string> initializers_for(Class& object)
{
  list<string> initializers;
  string root_getter = get_root_from_parent_code(object);
  string reference_holder_getter = get_reference_holder_from_parent_code(object);

  if (object.get_superclass() == Context::global.template_base_type())
    initializers.push_back(object.get_superclass() + "(\"" + tag_name_for(object) + "\")");
  initializers.push_back("root(" + root_getter + ")");
  for (const auto& reference : object.get_references())
  {
    if (reference->has_initializer())
      initializers.push_back(reference->get_name() + '(' + reference->get_initializer(reference_holder_getter) + ')');
  }
  if (!object.is_root())
  {
    if (!object.implements_ibindable_view())
      initializers.push_back("signaler(" + parent_symbol + "->signaler)");
    initializers.push_back("parent(" + parent_symbol + ')');
    if (object.is_repeater())
      initializers.push_back(reinterpret_cast<const Repeater&>(object).get_value_name() + "(__i)");
    for (const auto& slot_ : object.get_slots())
    {
      const Slot& slot = reinterpret_cast<const Slot&>(*slot_.get());
      initializers.push_back(slot.get_slot_ref() + '(' + reference_holder_getter + "->" + slot.get_slot_ref() + ')');
    }
  }
  return initializers;
}

static void generate_constructor_header(stringstream& stream, Class& object)
{
  auto initializers = initializers_for(object);

  stream << "HtmlTemplate::" << object.get_typename() << "::" << object.get_typename() << '(';
  if (!object.is_root())
  {
    stream << object.get_parent()->get_typename() << "* " << parent_symbol;
    if (object.is_repeater())
      stream << ", " << reinterpret_cast<const Repeater&>(object).get_value_type() << " __i";
  }
  stream << ')';
  if (initializers.size() > 0)
  {
    stream << " :" << endl;
    for (auto it = initializers.begin() ; it != initializers.end() ; ++it)
    {
      if (it != initializers.begin()) stream << ',' << endl;
      stream << "  " << *it;
    }
  }
  stream << endl;
}

static void generate_element_initializers(stringstream& stream, Class& object)
{
  for (const auto& reference : object.get_references())
  {
    if (reference->get_type() == "Comet::Element")
      stream << "  " << reference->get_name() << " = Comet::Element(\"" << reference->get_element().name() << "\");" << endl;
  }
}

static void generate_binding_initializers(stringstream& stream, Class& object)
{
  for (const auto& binding : object.get_bindings())
  {
    const auto reference = object.find_reference_for(binding->get_element());

    if (binding->is_plain_setter())
    {
      stream << "  " << reference->get_name() << ".set_" << binding->get_attribute_name() << '(' << binding->get_code() << ");" << endl;
      continue ;
    }
    stream << "  bound_attributes.push_back("
           << "Comet::Bindable(";
    if (binding->binds_to_cpp_property())
      stream << "[this]() { " << reference->get_name() << ".set_" << binding->get_attribute_name() << '(' << binding->get_code() << "); }";
    else if (binding->get_attribute_name() == "checked")
      stream << "[this]() { " << reference->get_name() << ".checked(" << binding->get_code() << "); }";
    else if (binding->get_attribute_name() == "selected")
      stream << "[this]() { " << reference->get_name() << ".selected(" << binding->get_code() << "); }";
    else if (binding->get_attribute_name() == "show")
      stream << "[this]() { " << reference->get_name() << ".visible(" << binding->get_code() << "); }";
    else if (binding->get_attribute_name() == "text")
      stream << "[this]() { " << reference->get_name() << ".text(" << binding->get_code() << "); }";
    else if (binding->get_attribute_name() == "innerhtml")
      stream << "[this]() { " << reference->get_name() << ".html(" << binding->get_code() << "); }";
    else
      stream << reference->get_name() << ", \"" << binding->get_attribute_name() << "\", [this]() { return Comet::lexical_cast<std::string>(" << binding->get_code() << "); }";
    stream << ')' << binding->get_bind_mode()
           << ");" << endl;
  }
}

static void generate_repeater_initializers(stringstream& stream, Class& object)
{
  for (const auto& repeater_ : object.get_repeaters())
  {
    const Repeater& repeater = reinterpret_cast<const Repeater&>(*repeater_.get());

    stream << "  bound_attributes.push_back("
           << "Comet::Bindable([this]() { "
           << repeater.get_name() << ".refresh(this, " << repeater.get_list_name() << ");"
           << " })"
           << repeater.get_bind_mode()
           << ");" << endl;
  }
}

static void generate_event_listener_initializers(stringstream& stream, Class& object)
{
  for (const auto& event_listener : object.get_event_listeners())
  {
    const auto reference = object.find_reference_for(event_listener->get_element());

    if (event_listener->binds_to_cpp())
    {
      stream << "  " << reference->get_name() << ".signaler.connect([this](std::string signal_name)" << endl
             << "  {" << endl
             << "    if (signal_name == \"" << event_listener->get_attribute_name() << "\")" << endl
             << "    {" << endl
             << "      " << event_listener->get_code() << ';' << endl
             << "    }" << endl
             << "  });" << endl;
    }
    else
      stream << "  " << reference->get_name() << ".events->on(\"" << event_listener->get_attribute_name() << "\", [this](client::Event* _event) { " << event_listener->get_code() << "; });" << endl;
  }
}

static void generate_dom_constructor(stringstream& stream, Class& object)
{
  pugi::xml_node source_el = object.get_element();
  string         html_attributes = make_attrs_from_element(source_el);
  stringstream   dom_stream;

  dom_generator(dom_stream, source_el, object, 2);
  if (html_attributes.length() > 0)
    stream << "  attr({" << html_attributes << "});" << endl;
  if (dom_stream.rdbuf()->in_avail())
    stream << "  inner(std::vector<Comet::Element>{" << dom_stream.str() << endl << "  });" << endl;
}

static void generate_anchor_initializers(stringstream& stream, Class& object)
{
  string reference_holder_getter = get_reference_holder_from_parent_code(object);

  for (const auto& repeater_ : object.get_repeaters())
  {
    const Repeater& repeater = reinterpret_cast<const Repeater&>(*repeater_.get());

    stream << "  " << repeater.get_name() << ".set_anchor(" << repeater.get_anchor_name() << ", " << anchor_symbols.at(AppendAnchor) << ");" << endl;
  }
  for (const auto& slot_ : object.get_slots())
  {
    const Slot& slot = reinterpret_cast<const Slot&>(*slot_.get());

    stream << "  " << slot.get_slot_ref() << ".set_anchor(" + reference_holder_getter + "->" << slot.get_anchor_name() << ", " << anchor_symbols.at(AppendAnchor) << ");" << endl;
    stream << "  " << slot.get_slot_ref() << ".set_element(std::make_shared<" << slot.get_typename() << ">(" + reference_holder_getter + "));" << endl;
  }
  for (const auto& slot_plugin_ : object.get_slot_plugins())
  {
    const SlotPlugin& slot_plugin = reinterpret_cast<const SlotPlugin&>(*slot_plugin_.get());
    const auto& reference = object.find_reference_for(slot_plugin.get_on_element());

    stream << "  " << reference->get_name() << ".slot_" << slot_plugin.get_slot_name() << ".set_element(";
    if (slot_plugin.has_reference())
      stream << reference_holder_getter << "->" << slot_plugin.get_element().attribute("ref").value();
    else
      stream << "std::make_shared<" << slot_plugin.get_typename() << ">(" << get_parent_from_parent_code(slot_plugin) << ")";
    stream << ");" << endl;
  }
}

static void generate_constructor(stringstream& stream, Class& object)
{
  generate_constructor_header(stream, object);
  stream << '{' << endl;
  generate_element_initializers(stream, object);
  generate_binding_initializers(stream, object);
  generate_repeater_initializers(stream, object);
  generate_event_listener_initializers(stream, object);
  generate_dom_constructor(stream, object);
  generate_anchor_initializers(stream, object);
  stream << '}' << endl << endl;
}

static void generate_method_bind_attributes(stringstream& stream, Class& object)
{
  stream << "void HtmlTemplate::" << object.get_typename() << "::bind_attributes()" << endl
         << '{' << endl;
  if (object.inherits_binding_methods())
    stream << "  " << object.get_superclass() << "::bind_attributes();" << endl;
  else
    stream << "  bound_attributes.enable(signaler);" << endl;
  for (const auto& reference : object.get_references())
  {
    if (reference->get_element().type() != pugi::node_null && Context::global.has_registered_cpp_type(reference->get_element()))
    {
      stream << "  " << reference->get_name() << ".bind_attributes();" << endl;
      stream << "  " << "signaler.connect([this](std::string _event) { " << reference->get_name() << ".signaler.trigger(_event); });" << endl;
    }
  }
  for (const auto& repeater : object.get_repeaters())
    stream << "  " << reinterpret_cast<const Repeater&>(*repeater.get()).get_name() << ".bind_attributes();" << endl;
  for (const auto& slot : object.get_slots())
  {
    const string& slot_ref = reinterpret_cast<const Slot&>(*slot.get()).get_slot_ref();
    stream << "  " << "if (" << slot_ref << ".has_element())" << endl
           << "  " << "  " << slot_ref << ".get_element()->bind_attributes();" << endl;
  }
  stream << '}' << endl;
}

static void generate_method_trigger_binding_updates(stringstream& stream, Class& object)
{
  stream << "void HtmlTemplate::" << object.get_typename() << "::trigger_binding_updates()" << endl
         << '{' << endl;
  if (object.inherits_binding_methods())
    stream << "  " << object.get_superclass() << "::trigger_binding_updates();" << endl;
  else
    stream << "  " << "bound_attributes.update();" << endl;
  for (const auto& reference : object.get_references())
  {
    if (reference->get_element().type() != pugi::node_null && Context::global.has_registered_cpp_type(reference->get_element()))
      stream << "  " << reference->get_name() << ".trigger_binding_updates();" << endl;
  }
  for (const auto& repeater : object.get_repeaters())
    stream << "  " << reinterpret_cast<const Repeater&>(*repeater.get()).get_name() << ".trigger_binding_updates();" << endl;
  for (const auto& slot : object.get_slots())
  {
    const string& slot_ref = reinterpret_cast<const Slot&>(*slot.get()).get_slot_ref();
    stream << "  " << "if (" << slot_ref << ".has_element())" << endl
           << "  " << "  " << slot_ref << ".get_element()->trigger_binding_updates();" << endl;
  }
  stream << '}' << endl;
}

string SourceGenerator::generate()
{
  stringstream stream;

  generate_constructor(stream, object);
  stream << endl;
  generate_method_bind_attributes(stream, object);
  stream << endl;
  generate_method_trigger_binding_updates(stream, object);
  return stream.str();
}
