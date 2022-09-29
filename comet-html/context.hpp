#pragma once
#include <pugixml/pugixml.hpp>
#include <boost/json.hpp>
#include <memory>
#include <map>
#include <vector>
#include <filesystem>

class Class;

struct Context
{
  static Context global;

  std::filesystem::path               input_folder, file_path;
  std::map<std::string, std::string>  element_types;
  std::vector<std::shared_ptr<Class>> classes;
  std::vector<std::string>            referenced_types;
  static unsigned short               implicit_reference_count;
  unsigned short                      slot_count = 0;
  unsigned short                      repeater_count = 0;

  virtual std::string template_base_type() const    { return "Comet::IBindableView"; }
  virtual std::string template_base_subtype() const { return template_base_type(); }
  virtual std::string element_base_type() const     { return "Comet::Element"; }

  void        reset();
  bool        has_cpp_type(pugi::xml_node node) const { return element_types.find(node.name()) != element_types.end() || !node.attribute("slot").empty(); }
  void        use_cpp_type(const std::string& name);
  std::string find_cpp_type(const std::string& name, const std::string& fallback = "") const;
  std::string generate_new_reference_name();
  void        load_global_element_types(const boost::json::array& elements);

  virtual ~Context() {}

  template<typename TYPE, typename... ARGS>
  static std::shared_ptr<TYPE> make(ARGS... args)
  {
    auto ptr = std::make_shared<TYPE>(args...);

    global.classes.push_back(ptr);
    if (ptr->get_parent())
      ptr->get_parent()->children.push_back(ptr);
    return ptr;
  }
};
