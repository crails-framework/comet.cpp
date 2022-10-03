#include <pugixml/pugixml.hpp>
#include <boost/json.hpp>
#include <sstream>
#include <map>
#include <regex>
#include "class.hpp"

using namespace std;

typedef void (*dom_generator_method)(stringstream&, const pugi::xml_node&, Class& object, unsigned short depth);
typedef std::map<pugi::xml_node_type, dom_generator_method> DomGenerators;
static void dom_element_generator(stringstream&, const pugi::xml_node&, Class&, unsigned short);
static void dom_text_generator(stringstream&, const pugi::xml_node&, Class&, unsigned short);
void        dom_generator(std::stringstream&, const pugi::xml_node&, Class&, unsigned short);

DomGenerators dom_generators{
  {pugi::node_element, &dom_element_generator},
  {pugi::node_pcdata,  &dom_text_generator}
};

static string indent(unsigned short i = 1)
{
  string result;
  result.resize(i * 2, ' ');
  return result;
}


string make_attrs_from_element(const pugi::xml_node& node)
{
  static const vector<string> reserved_keywords{"ref", "slot"};
  static const regex binding_pattern("\\.(bind|trigger|for)$");
  stringstream stream;
  unsigned short i = 0;

  for (const pugi::xml_attribute& attribute : node.attributes())
  {
    string name(attribute.name());

    if (find(reserved_keywords.begin(), reserved_keywords.end(), name) == reserved_keywords.end()
      && sregex_iterator(name.begin(), name.end(), binding_pattern) == sregex_iterator()
      && name.substr(0, 5) != "cpp::"
      && name != "_cheerp_class")
    {
      if (i > 0) stream << ',';
      stream << "{\"" << attribute.name() << "\",\"" << attribute.value() << "\"}";
      ++i;
    }
  }
  return stream.str();
}

static void dom_anchorable_generator(std::stringstream& stream, Class& object, Class& context, unsigned short depth)
{
  string prefix = "";

  if (object.get_parent() == context.root())
    prefix = "root->";
  // else if (object.get_parent().get() != &context)
  // TODO: mayhaps try to find a path to reach the anchorable actual container
  stream << endl << indent(depth) << prefix << object.get_anchor_name();
}

static void dom_element_generator(stringstream& stream, const pugi::xml_node& node, Class& object, unsigned short depth)
{
  auto reference = object.find_reference_for(node);
  auto attributes = make_attrs_from_element(node);
  stringstream children_stream;

  stream << endl << indent(depth);
  if (reference == nullptr)
    stream << "Comet::Element(\"" << node.name() << "\")";
  else
    stream << reference->get_name();
  if (attributes.length() > 0)
    stream << ".attr({" << attributes << "})";
  if (!Context::global.has_cpp_type(node))
    dom_generator(children_stream, node, object, depth + 1);
  if (children_stream.rdbuf()->in_avail())
    stream << " > std::vector<Comet::Element>{" << children_stream.str() << endl << indent(depth) << "}";
}

static void dom_text_generator(stringstream& stream, const pugi::xml_node& node, Class& object, unsigned short depth)
{
  string text = node.text().as_string();

  stream << endl << indent(depth) << "Comet::Element(\"span\").text(" << boost::json::serialize(text) << ')';
}

void dom_generator(stringstream& stream, const pugi::xml_node& node, Class& object, unsigned short depth)
{
  unsigned short count = 0;

  for (const pugi::xml_node& child : node.children())
  {
    shared_ptr<Class>       sub_object = object.root()->find_class_for(child);
    DomGenerators::iterator generator = dom_generators.find(child.type());

    if (generator == dom_generators.end()) continue ;
    if (count++ > 0) stream << ',';
    if (sub_object && sub_object->is_anchorable())
      dom_anchorable_generator(stream, *sub_object, object, depth);
    else
      (*generator->second)(stream, child, object, depth);
  }
}
