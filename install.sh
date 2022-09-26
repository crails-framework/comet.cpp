#!/bin/bash -e

DEFAULT_CRAILS_VERSION=master
DEFAULT_INSTALL_ROOT=/usr/local

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

if [[ -z "$CRAILS_VERSION" ]] ; then
  echo -n "> Which version of comet.cpp would you like to install (default: $DEFAULT_CRAILS_VERSION): "
  read CRAILS_VERSION
  if [[ -z "$CRAILS_VERSION" ]] ; then CRAILS_VERSION="$DEFAULT_CRAILS_VERSION" ; fi
fi

if [[ -d "/opt/cheerp" ]] ; then
  CHEERP_PATH="/opt/cheerp"
else
  echo -n "> Location for Cheerp: "
  read CHEERP_PATH
fi

if [[ -z "$INSTALL_ROOT" ]] ; then
  echo -n "> Install path (default: $DEFAULT_INSTALL_ROOT): "
  read INSTALL_ROOT
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

COMPILER_VERSION=`$CHEERP_PATH/bin/clang++ --version | sed -n 1p | cut -d' ' -f5`
BUILD_DIR="build-cheerp-$COMPILER_VERSION"

echo "+ creating package at $BUILD_DIR"
bpkg create -d "$BUILD_DIR" cc \
  config.cxx="$CHEERP_PATH/bin/clang++" \
  config.cxx.poptions="-target cheerp-genericjs -D__CHEERP_CLIENT__ -I/usr/local/include" \
  config.c{,xx}.version=$COMPILER_VERSION \
  config.install.sudo=sudo \
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
