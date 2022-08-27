#pragma once
#include "class.hpp"

class SlotPlugin;
class Repeater;

class Probe
{
public:
  static void probe(SlotPlugin&);
  static void probe(Repeater&);
  static void probe(Class&);
private:
  static void probe_subtypes(Class&, const pugi::xml_node* root = nullptr);
  static void probe_slots_plugins(Class&, const pugi::xml_node* root = nullptr);
  static void probe_references(Class&, const pugi::xml_node* root = nullptr);
  static void probe_bindings(Class&, const pugi::xml_node* root = nullptr);
  static void probe_bindings_for(Class&, const pugi::xml_node& element);
};

