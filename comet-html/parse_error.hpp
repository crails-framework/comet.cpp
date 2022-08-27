#pragma once

struct ParseError : public std::exception
{
  ParseError(pugi::xml_node element, const std::string& description) : element(element), description(description) {}
  const char* what() const noexcept override { return description.c_str(); }

  pugi::xml_node     element;
  const std::string description;
};

struct LoadError : public std::exception
{
  LoadError(const std::string& message) : message(message) {}
  const char* what() const noexcept override { return message.c_str(); }
  const std::string message;
};
