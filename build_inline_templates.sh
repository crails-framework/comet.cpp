#!/bin/bash -ex
ECPP=ecpp
PROJECT_TPL=comet/new/templates
SCAFFOLD_TPL=comet/scaffolds/templates
##
## AppTemplate
##
mkdir -p $PROJECT_TPL/build
$ECPP \
  -n ProjectCmakelistsTxt \
  -i $PROJECT_TPL/CMakeLists.txt.ecpp \
  >  $PROJECT_TPL/build/CMakeLists.txt.cpp
$ECPP \
  -n ProjectMainCpp \
  -i $PROJECT_TPL/main.cpp.ecpp \
  >  $PROJECT_TPL/build/main.cpp.cpp
$ECPP \
  -n ProjectRoutesCpp \
  -i $PROJECT_TPL/routes.cpp.ecpp \
  >  $PROJECT_TPL/build/routes.cpp.cpp
$ECPP \
  -n ProjectApplicationHpp \
  -i $PROJECT_TPL/application.hpp.ecpp \
  >  $PROJECT_TPL/build/exception.html.cpp
$ECPP \
  -n ProjectConfigJson \
  -i $PROJECT_TPL/config.json.ecpp \
  >  $PROJECT_TPL/build/config.json.cpp

##
## Scaffolds
##
$ECPP \
  -n ScaffoldLayoutHtml \
  -i $SCAFFOLD_TPL/layout.html.ecpp \
  >  $SCAFFOLD_TPL/build/layout.html.cpp
$ECPP \
  -n ScaffoldLayoutHpp \
  -i $SCAFFOLD_TPL/layout.hpp.ecpp \
  >  $SCAFFOLD_TPL/build/layout.hpp.cpp
$ECPP \
  -n ScaffoldViewHtml \
  -i $SCAFFOLD_TPL/view.html.ecpp \
  >  $SCAFFOLD_TPL/build/view.html.cpp
$ECPP \
  -n ScaffoldViewHpp \
  -i $SCAFFOLD_TPL/view.hpp.ecpp \
  >  $SCAFFOLD_TPL/build/view.hpp.cpp
$ECPP \
  -n ScaffoldControllerHpp \
  -i $SCAFFOLD_TPL/controller.hpp.ecpp \
  >  $SCAFFOLD_TPL/build/controller.hpp.cpp
$ECPP \
  -n ScaffoldControllerCpp \
  -i $SCAFFOLD_TPL/controller.cpp.ecpp \
  >  $SCAFFOLD_TPL/build/controller.cpp.cpp
