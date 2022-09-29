#include <iostream>
#include <pugixml/pugixml.hpp>
#include <fstream>
#include <map>
#include <memory>
#include <vector>
#include <sstream>
#include <regex>
#include "generator.hpp"
#include "parse_error.hpp"
#include <boost/json.hpp>
#include <boost/program_options.hpp>
#include <crails/read_file.hpp>
#include <filesystem>

using namespace std;

bool is_valid_cpp_variable_name(const std::string& value)
{
  if (value.length() < 255)
    return std::regex_match(value.begin(), value.end(), std::regex("^[a-zA-Z_][a-zA-Z0-9_]*$"));
  return false;
}

// filesystem.cpp
bool write_file(const string& filepath, const string& contents);
// options.cpp
boost::program_options::options_description options_description();
boost::program_options::variables_map       get_options(int argc, char** argv);

boost::json::object load_config(const std::string& path)
{
  boost::json::object        result;
  boost::json::parse_options options;
  std::string                raw_json;

  options.allow_comments = options.allow_trailing_commas = true;
  if (Crails::read_file(path, raw_json))
    return boost::json::parse(raw_json, {}, options).as_object();
  return result;
}

static void collect_template_files(const filesystem::path& directory, vector<filesystem::path>& path_list)
{
  for (const auto& entry : filesystem::recursive_directory_iterator(directory))
  {
    if (filesystem::is_directory(entry))
      continue ;
    else if (entry.path().extension() == ".html")
      path_list.push_back(entry.path());
  }
}

static void output_parse_error(const std::filesystem::path& path, pugi::xml_parse_result result, const std::string& source)
{
  unsigned short line_count = 1;
  unsigned short relative_offset = 0;
  unsigned short line_begin = 0;
  unsigned short line_end = 0;

  for (unsigned short i = 0 ; i < source.length() ; ++i)
  {
    if (source[i] == '\n')
    {
      if (i > result.offset)
      {
        line_end = i;
        break ;
      }
      line_begin = i + 1;
      line_count++;
      relative_offset = 0;
    }
    else if (i < result.offset)
      relative_offset++;
  }
  if (line_end == 0) line_end = result.offset;
  std::cerr << "parsing error: " << result.description() << " in `" << path.string() << "` at line " << line_count << " and offset " << relative_offset << endl
            << "  " << source.substr(line_begin, line_end - line_begin) << endl;
}

static std::string extract_inline_code(std::string& html)
{
  static const string tag_name = "script";
  static const regex  open_regex("^<\\s*" + tag_name + "(\\s+[^>]*)?>", regex_constants::icase | regex_constants::optimize);
  static const regex  close_regex("^</\\s*" + tag_name + "\\s*>",        regex_constants::icase | regex_constants::optimize);
  bool within_inline = false;
  string inline_code;

  for (unsigned short i = 0 ; i < html.length() ; ++i)
  {
    auto match = cregex_iterator(&html[i], &(*html.end()), within_inline ? close_regex : open_regex);

    if (match != cregex_iterator())
    {
      within_inline = !within_inline;
      i += match->length() - 1;
      inline_code += '\n';
    }
    else if (within_inline)
    {
      inline_code += html[i];
      if (html[i] != '\n')
        html[i] = ' ';
    }
  }
  if (within_inline)
    throw LoadError("Could not find closing tag for <" + tag_name + '>');
  return inline_code;
}

static int generate_template(const filesystem::path& output_folder, const filesystem::path& template_path, const boost::json::object& config)
{
  string                 html;
  pugi::xml_document     document;
  Generator              generator(document, config);

  try
  {
    if (Crails::read_file(template_path.string(), html))
    {
      string inline_code = extract_inline_code(html);
      pugi::xml_parse_result result = document.load_string(html.c_str(), pugi::parse_cdata | pugi::parse_trim_pcdata);

      if (result)
      {
        Context::global.reset();
        Context::global.file_path = template_path;
        bool success = true;

        generator.prepare();
        generator.set_inline_code(inline_code);
        filesystem::create_directories(filesystem::path(output_folder.string() + '/' + generator.compiled_header_path()).parent_path());
        success = success && write_file(output_folder.string() + '/' + generator.compiled_header_path(), generator.generate_header());
        success = success && write_file(output_folder.string() + '/' + generator.compiled_source_path(), generator.generate_source());
        if (!success) return 6;
      }
      else
      {
        output_parse_error(template_path, result, html);
        return 2;
      }
    }
    else
    {
      cerr << "cannot open file: " << template_path.string() << endl;
      return 1;
    }
  }
  catch (const LoadError& exception)
  {
    cerr << "could not load `" << template_path.string() << "`: " << exception.what() << endl;
    return 5;
  }
  catch (const ParseError& exception)
  {
    cerr << "error while probing `" << template_path.string() << "`: " << exception.what() << endl;
    exception.element.print(cerr);
    return 4;
  }
  catch (const std::exception& exception)
  {
    cerr << "exception while generating `" << template_path.string() << "`: " << exception.what() << endl;
    return 3;
  }
  return 0;
}

int main(int argc, char* argv[])
{
  using namespace std;
  auto options = get_options(argc, argv);

  if (options.count("help"))
  {
    cout << "usage: " << argv[0] << " [options]" << endl << options_description();
    return 0;
  }
  if (options.count("input") && options.count("output"))
  {
    boost::json::object config;
    filesystem::path input_folder(options["input"].as<string>());
    filesystem::path output_folder(options["output"].as<string>());
    vector<filesystem::path> html_templates;

    if (!filesystem::is_directory(input_folder))
    {
      cerr << "input is not a directory: " << input_folder.string() << endl;
      return -2;
    }
    if (options.count("config"))
      config = load_config(options["config"].as<std::string>());
    collect_template_files(input_folder, html_templates);
    Context::global.input_folder = input_folder;
    for (const auto& template_path : html_templates)
    {
      int result = generate_template(output_folder, template_path, config);

      if (result != 0)
        return result;
    }
  }
  else
  {
    cerr << "missing input and/or output arguments" << endl;
    return -1;
  }
  return 0;
}
