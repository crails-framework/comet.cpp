#!/bin/bash -ex
ECPP=ecpp
PROJECT_TPL=comet/new/templates
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
