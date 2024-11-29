#!/bin/bash
NBPROC=$(sysctl -n hw.ncpu)
echo $NBPROC

#update pacman
brew update 
brew upgrade
xcode-select --install
brew install -v cmake
brew install -v nasm
brew install -v pkg-config
brew install -v autoconf
brew install -v automake
brew install -v libtool
brew install -v qpdf
brew install -v ncnn
brew install -v autoconf-archive

#Install External Library
cd libextern
chmod +x compilation_library_macos.sh
./compilation_library_macos.sh
cd ..

#/bin/sh -c '/usr/bin/make -j12 -e -f Makefile.macos'
