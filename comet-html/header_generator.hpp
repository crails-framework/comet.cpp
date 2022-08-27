#pragma once
#include "class.hpp"

class HeaderGenerator
{
  Class& object;
public:
  HeaderGenerator(Class& klass) : object(klass) {}
  std::string generate();
};

class SourceGenerator
{
  Class& object;
public:
  SourceGenerator(Class& klass) : object(klass) {}
  std::string generate();
};
