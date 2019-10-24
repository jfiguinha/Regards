#!/bin/bash
NBPROC=$(sysctl -n hw.physicalcpu)
echo $NBPROC

#update pacman
brew update 
brew upgrade
xcode-select --install
brew install -v HOMEBREW_MAKE_JOBS=$NBPROC llvm@7
brew install -v HOMEBREW_MAKE_JOBS=$NBPROC pango 
brew install -v HOMEBREW_MAKE_JOBS=$NBPROC git 
brew install -v HOMEBREW_MAKE_JOBS=$NBPROC libexif 
brew install -v HOMEBREW_MAKE_JOBS=$NBPROC OpenEXR
brew install -v HOMEBREW_MAKE_JOBS=$NBPROC glew
brew install -v HOMEBREW_MAKE_JOBS=$NBPROC coreutils
brew install -v HOMEBREW_MAKE_JOBS=$NBPROC sane-backends
brew install -v HOMEBREW_MAKE_JOBS=$NBPROC leptonica

#Install External Library
cd libextern
chmod +x compilation_library_macos.sh
./compilation_library_macos.sh
cd ..

#/bin/sh -c '/usr/bin/make -j12 -e -f Makefile.macos'
