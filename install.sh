#!/bin/bash -e

DEFAULT_CRAILS_VERSION=master
DEFAULT_INSTALL_ROOT=/usr/local
CPPGET_FINGERPRINT="70:64:FE:E4:E0:F3:60:F1:B4:51:E1:FA:12:5C:E0:B3:DB:DF:96:33:39:B9:2E:E5:C2:68:63:4C:A6:47:39:43"

echo ""
echo '   ______                     __             '     
echo '  / ____/___  ____ ___  ___  / /_ _________  ____ '
echo ' / /   / __ \/ __ `__ \/ _ \/ __// ___/ __ \/ __ \'
echo '/ /___/ /_/ / / / / / /  __/ /__/ /__/ /_/ / /_/ /'
echo '\____/\____/_/ /_/ /_/\___/\__(_)___/ .___/ .___/ '
echo '                                   /_/   /_/      '
echo ""

crails_packages=(
  libcrails-semantics
  libcrails-router
  libtext-archive
  libcomet
)

if [ -z "$CRAILS_VERSION" ] ; then
  echo -n "> Which version of comet.cpp would you like to install (default: $DEFAULT_CRAILS_VERSION): "
  read CRAILS_VERSION
  if [ -z "$CRAILS_VERSION" ] ; then CRAILS_VERSION="$DEFAULT_CRAILS_VERSION" ; fi
fi

if [ -d "/opt/cheerp" ] ; then
  CHEERP_PATH="/opt/cheerp"
else
  echo -n "> Location for Cheerp: "
  read CHEERP_PATH
fi

if [ -z "$INSTALL_ROOT" ] ; then
  echo -n "> Install path (default: $DEFAULT_INSTALL_ROOT): "
  read INSTALL_ROOT
  if [ -z "$INSTALL_ROOT" ] ; then export INSTALL_ROOT="$DEFAULT_INSTALL_ROOT" ; fi
fi

if [ -z "$INSTALL_CLI" ] ; then
  echo -n "> Install CLI (default: y): "
  read INSTALL_CLI
  if [ -z "$INSTALL_CLI" ] ; then export INSTALL_CLI="y" ; fi
fi

touch "$INSTALL_ROOT/toto" && rm "$INSTALL_ROOT/toto" && USE_SUDO=n || USE_SUDO=y
if [ "$USE_SUDO" = "y" ] ; then
  SUDO_OPTION="config.install.sudo=sudo"
fi

##
## Prepare build2
##
if ! which bpkg ; then
  echo "+ bpkg does not appear to be installed. Installing build2:"
  BUILD2_VERSION="0.15.0"
  curl -sSfO https://download.build2.org/$BUILD2_VERSION/build2-install-$BUILD2_VERSION.sh
  chmod +x build2-install-$BUILD2_VERSION.sh
  sh build2-install-$BUILD2_VERSION.sh
fi

##
## Build libcomet
##
COMPILER_VERSION=`$CHEERP_PATH/bin/clang++ --version | sed -n 1p | cut -d' ' -f5`
BUILD_DIR="build-cheerp-$COMPILER_VERSION"

if [ -d "$BUILD_DIR" ] ; then
  echo "++ removing previously existing package at $BUILD_DIR"
  rm -rf "$BUILD_DIR"
fi

echo "+ creating package at $BUILD_DIR"
bpkg create -d "$BUILD_DIR" cc \
  config.cxx="$CHEERP_PATH/bin/clang++" \
  config.cxx.poptions="-target cheerp-genericjs -fexceptions -D__CHEERP_CLIENT__ -I/usr/local/include" \
  config.c{,xx}.version=$COMPILER_VERSION \
  $SUDO_OPTION \
  config.install.root=$INSTALL_ROOT \
  config.install.lib=exec_root/lib/genericjs

cd "$BUILD_DIR"

echo "+ fetching dependencies"
bpkg add "https://github.com/crails-framework/libtext-archive.git#$CRAILS_VERSION"
bpkg add "https://github.com/crails-framework/libcrails-semantics.git#$CRAILS_VERSION"
bpkg add "https://github.com/crails-framework/libcrails-router.git#$CRAILS_VERSION"
bpkg add "https://github.com/crails-framework/libcomet.git#$CRAILS_VERSION"
bpkg fetch

for package in ${crails_packages[@]} ; do
  echo "+ building $package"
  bpkg build $package --yes
  echo "+ installing $package at $INSTALL_ROOT"
  bpkg install $package
done

##
## Build Comet CLI
##
if [ "$INSTALL_CLI" = "y" ] ; then
  cd ..
  if which clang++ ; then
    cpp_compiler=clang++
  elif which g++ ; then
    cpp_compiler=g++
  else
    echo "+ C++ compiler not found"
    exit -1
  fi
  BUILD_DIR="build-comet-cli"
  if [ -d "$BUILD_DIR" ] ; then
    echo "++ removing previously existing package at $BUILD_DIR"
    rm -rf "$BUILD_DIR"
  fi
  echo "+ creating CLI package at $BUILD_DIR"
  bpkg create -d "$BUILD_DIR" cc \
    config.cxx="$cpp_compiler" \
    config.cc.coptions=-O3 \
    config.bin.rpath="$INSTALL_ROOT/lib"
  cd "$BUILD_DIR"
  bpkg add https://pkg.cppget.org/1/beta --trust "$CPPGET_FINGERPRINT"
  bpkg add "https://github.com/crails-framework/comet.cpp.git#$CRAILS_VERSION"
  bpkg fetch --trust "$CPPGET_FINGERPRINT"
  bpkg build comet --yes
  bpkg install comet config.install.root="$INSTALL_ROOT" $SUDO_OPTION
fi
