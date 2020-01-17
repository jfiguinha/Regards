#!/bin/bash
NBPROC=$(sysctl -n hw.ncpu)
echo $NBPROC

#update pacman
brew update 
brew upgrade
xcode-select --install
HOMEBREW_MAKE_JOBS=$NBPROC brew install -v pkgconfig
HOMEBREW_MAKE_JOBS=$NBPROC brew install -v llvm@7
HOMEBREW_MAKE_JOBS=$NBPROC brew install -v pango 
HOMEBREW_MAKE_JOBS=$NBPROC brew install -v libexif 
HOMEBREW_MAKE_JOBS=$NBPROC brew install -v OpenEXR
HOMEBREW_MAKE_JOBS=$NBPROC brew install -v glew
HOMEBREW_MAKE_JOBS=$NBPROC brew install -v coreutils
HOMEBREW_MAKE_JOBS=$NBPROC brew install -v leptonica
HOMEBREW_MAKE_JOBS=$NBPROC brew install -v libjpeg-turbo
HOMEBREW_MAKE_JOBS=$NBPROC brew install -v wget
HOMEBREW_MAKE_JOBS=$NBPROC brew install -v unzip
HOMEBREW_MAKE_JOBS=$NBPROC brew install -v cmake
HOMEBREW_MAKE_JOBS=$NBPROC brew install -v yasm
HOMEBREW_MAKE_JOBS=$NBPROC brew install -v tbb
#bHOMEBREW_MAKE_JOBS=$NBPROC rew install -v fftw
#Install External Library
cd libextern
chmod +x compilation_library_macos.sh
./compilation_library_macos.sh
cd ..

#/bin/sh -c '/usr/bin/make -j12 -e -f Makefile.macos'
