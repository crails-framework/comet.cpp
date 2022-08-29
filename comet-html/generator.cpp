#include "generator.hpp"
#include "probe.hpp"
#include "header_generator.hpp"
#include <set>
#include <crails/utils/semantics.hpp>

using namespace std;

void Generator::prepare()
{
  auto html = document.select_node("template");
  auto head = document.select_node("//head");
  auto body = document.select_node("//body");

  if (html.node().type() == pugi::node_null) throw LoadError("could not find <template> element");
  if (body.node().type() == pugi::node_null) throw LoadError("missing <body> element");
  this->head = head.node();
  this->body = body.node();
  this->head.select_nodes("//attribute");

  if (config.if_contains("elements"))
    Context::global.load_global_element_types(config.at("elements").as_array());
  if (head.node().type() != pugi::node_null)
    load_includes();
  root = Context::make<Class>(body.node());
  initialize_classname(html.node());
  initialize_superclass(html.node());
  initialize_attributes();
  Probe::probe(*root);
}

void Generator::set_inline_code(const string& inline_code)
{
  root->inline_code = inline_code;
}

string Generator::generate_header()
{
  stringstream stream;
  string guard_name = "__COMET_CUSTOM_ELEMENT_" + root->get_typename() + "__";
  auto classes = generated_classes();

  stream << "#ifndef " << guard_name << endl
          << "# define " << guard_name << endl
          << "# include <comet/bindable.hpp>" << endl
          << "# include <comet/repeater.hpp>" << endl
          << "# include <comet/slot_element.hpp>" << endl
          << "# include <comet/comment_element.hpp>" << endl
          << "# include <comet/custom_element.hpp>" << endl
          << "# include <comet/signal.hpp>" << endl
          << prepare_includes() << endl
          << "namespace HtmlTemplate" << endl
          << '{' << endl;
  for (const auto& class_ : classes)
    stream << "  class " << class_->get_typename() << ';' << endl;
  for (auto it = classes.rbegin() ; it != classes.rend() ; ++it)
    stream << HeaderGenerator(**it).generate() << endl;
  stream << '}' << endl << "#endif";
  return stream.str();
}

string Generator::generate_source()
{
  stringstream stream;

  stream << "#include <comet/lexical_cast.hpp>" << endl
          << "#include \"" << compiled_header_path() << '"' << endl << endl;
  for (const auto& class_ : generated_classes())
    stream << SourceGenerator(*class_).generate() << endl;
  return stream.str();
}

vector<shared_ptr<Class>> Generator::generated_classes()
{
  vector<shared_ptr<Class>> classes;

  for (const auto& class_ : Context::global.classes)
  {
    if (!class_->should_skip()) classes.push_back(class_);
  }
  return classes;
}

void Generator::initialize_classname(const pugi::xml_node& node)
{
  pugi::xml_attribute attribute = node.attribute("classname");

  if (attribute.empty())
  {
    auto filename = Context::global.file_path.filename();
    auto classname = filename.string().substr(0, filename.string().length() - filename.extension().string().length());
    root->type_name = Crails::camelize(classname);
  }
  else
    root->type_name = attribute.value();
}

void Generator::initialize_superclass(const pugi::xml_node& node)
{
  pugi::xml_attribute attribute = node.attribute("extends");

  if (!attribute.empty())
    root->super_class = attribute.value();
}

void Generator::initialize_attributes()
{
  for (pugi::xpath_node xpath : head.select_nodes("//attribute"))
  {
    pugi::xml_node attribute = xpath.node();
    pugi::xml_attribute type = attribute.attribute("type");
    pugi::xml_attribute name = attribute.attribute("name");
    pugi::xml_attribute value = attribute.attribute("value");

    root->references.push_back(make_shared<CppReference>(attribute, type.value(), name.value(), value.value()));
  }
}

void Generator::load_includes()
{
  pugi::xpath_node_set includes = head.select_nodes("include");

  for (const pugi::xpath_node& xpath_node : includes)
  {
    pugi::xml_node node = xpath_node.node();
    pugi::xml_attribute require = node.attribute("require");
    pugi::xml_attribute tag_attribute = node.attribute("tag-name");

    if (!require.empty())
    {
      string type = require.value();
      string tag_name;

      if (tag_attribute.empty())
        tag_name = Crails::dasherize(type); // TODO: replace "::" with "_", remove anything after '<'
      else
        tag_name = tag_attribute.value();
      Context::global.element_types.emplace(tag_name, type);
    }
  }
}

string Generator::prepare_includes()
{
  stringstream stream;
  set<string> paths;
  const auto& referenced_types = Context::global.referenced_types;

  // Explicits includes in <head>
  for (const pugi::xpath_node& xpath_node : head.select_nodes("include"))
    paths.insert(xpath_node.node().attribute("src").value());
  // Implicits includes from JSON config
  if (config.if_contains("elements"))
  {
    for (const boost::json::value& element_value : config.at("elements").as_array())
    {
      const boost::json::object element = element_value.as_object();
      string require = boost::json::value_to<string>(element.at("require"));

      if (find(referenced_types.begin(), referenced_types.end(), require) != referenced_types.end())
        paths.insert(boost::json::value_to<string>(element.at("include")));
    }
  }
  for (const string& path : paths)
    stream << "# include \"" + path + "\"" << endl;
  return stream.str();
}

static string replace_extension(const string& path, const string& extension) { return path.substr(0, path.find_last_of(".")) + '.' + extension; }

string Generator::relative_filepath() const
{
  return std::filesystem::relative(Context::global.file_path, Context::global.input_folder).string();
}

string Generator::compiled_header_path() const
{
  return replace_extension(relative_filepath(), "hpp");
}

string Generator::compiled_source_path() const
{
  return replace_extension(relative_filepath(), "cpp");
}
