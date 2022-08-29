#include "context.hpp"
#include <sstream>
#include <crails/utils/semantics.hpp>

Context Context::global;

void Context::reset()
{
  file_path = std::filesystem::path();
  element_types.clear();
  classes.clear();
  referenced_types.clear();
  implicit_reference_count = slot_count = repeater_count = 0;
}

std::string Context::find_cpp_type(const std::string& name, const std::string& fallback) const
{
  if (element_types.find(name) != element_types.end())
    return element_types.at(name);
  else if (fallback.length() > 0)
    return fallback;
  return element_base_type();
}

void Context::use_cpp_type(const std::string& name)
{
  referenced_types.push_back(element_types.at(name));
}

std::string Context::generate_new_reference_name()
{
  std::stringstream stream;

  implicit_reference_count++;
  stream << "implicit_reference_" << implicit_reference_count;
  return stream.str();
}

void Context::load_global_element_types(const boost::json::array& elements)
{
  for (auto it = elements.begin() ; it != elements.end() ; ++it)
  {
    boost::json::object element_data = it->as_object();
    std::string         require = boost::json::value_to<std::string>(element_data["require"]);
    std::string         name;

    if (element_data["tagName"].is_string())
      name = boost::json::value_to<std::string>(element_data["tagName"]);
    else
      name = Crails::dasherize(require);
    element_types.emplace(name, require);
  }
}
