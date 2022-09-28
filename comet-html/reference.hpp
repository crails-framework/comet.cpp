#pragma once
#include <pugixml/pugixml.hpp>
#include <string>
#include <memory>
#include "parse_error.hpp"

class Class;

struct ReferenceBase : public std::enable_shared_from_this<ReferenceBase>
{
  const pugi::xml_node& get_element() const { return element; }
  virtual bool has_default_value() const { return false; }
  virtual bool has_initializer() const   { return false; }
  virtual bool has_getter() const        { return false; }
  virtual bool has_setter() const        { return false; }
  virtual bool is_explicit() const       { return false; }
  virtual bool is_implicit() const       { return false; }
  virtual std::string get_type() const   { return type; }
  virtual std::string get_name() const   { return name; }
  virtual std::string get_initializer(const std::string&) const { return initializer; }
  const std::string& get_default_value() const { return default_value; }

protected:
  ReferenceBase() {}
  ReferenceBase(pugi::xml_node element, std::string type, std::string name) : element(element), type(type), name(name) {}

  virtual ~ReferenceBase() {}
  pugi::xml_node element;
  std::string type, name, initializer, default_value;
};

struct CppReference : public ReferenceBase
{
  CppReference(pugi::xml_node element, const std::string& type, const std::string& name, const std::string& default_value = "");
  virtual ~CppReference() override {}

  bool is_explicit() const override { return true; }
  bool is_implicit() const override { return false; }
  bool has_default_value() const override { return default_value.length() > 0; }
  bool has_initializer() const override { return initializer.length() > 0; }
  bool has_getter() const override { return true; }
  bool has_setter() const override { return setter_enabled; }
  void disable_setter() { setter_enabled = false; }
  void set_initializer(const std::string& value) { initializer = value; }
private:
  bool        setter_enabled = true;
};

struct Reference : public ReferenceBase
{
  enum Mode
  {
    ImplicitReference,
    ExplicitReference,
    UnspecifiedReferenceMode
  };

  Reference(pugi::xml_node element, Class* parent, Mode mode = UnspecifiedReferenceMode);
  virtual ~Reference() override {}

  bool is_explicit() const override { return mode == ExplicitReference; }
  bool is_implicit() const override { return mode == ImplicitReference; }
  void set_mode(Mode mode);

protected:
  Mode mode = UnspecifiedReferenceMode;
  Class* parent;
};

struct RemoteReference : public Reference
{
  RemoteReference(pugi::xml_node element, Class* parent, Mode mode = UnspecifiedReferenceMode) : Reference(element, parent, mode) {}
  virtual ~RemoteReference() override {}
  std::string get_type() const override { return type + '&'; }
  bool has_initializer() const override { return true; }
  std::string get_initializer(const std::string & root_getter) const override { return root_getter + "->" + name; }
};

struct ThisReference : public ReferenceBase
{
  virtual ~ThisReference() override {}
  std::string get_name() const override { return "(*this)"; }
};
