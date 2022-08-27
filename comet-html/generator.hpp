#pragma once
#include <pugixml/pugixml.hpp>
#include <boost/json.hpp>
#include <memory>
#include <string>
#include "class.hpp"

class Generator
{
  pugi::xml_document&        document;
  std::shared_ptr<Class>     root;
  pugi::xml_node             head, body;
  const boost::json::object& config;
public:
  Generator(pugi::xml_document& document, const boost::json::object& config) : document(document), config(config)
  {
  }

  std::string relative_filepath() const;
  std::string compiled_header_path() const;
  std::string compiled_source_path() const;

  void        prepare();
  void        set_inline_code(const std::string&);
  std::string generate_header();
  std::string generate_source();

private:
  std::vector<std::shared_ptr<Class>> generated_classes();
  void initialize_classname(const pugi::xml_node& node);
  void initialize_superclass(const pugi::xml_node& node);
  void initialize_attributes();
  void load_includes();
  std::string prepare_includes();
};
