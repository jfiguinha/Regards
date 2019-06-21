#!/bin/bash
NBPROC=$(sysctl -n hw.physicalcpu)
echo $NBPROC

#update pacman
brew update 
brew upgrade 
xcode-select --install
brew install llvm@7
brew install pango 
brew install git 
brew install libexif 
brew install OpenEXR

#Install External Library
#cd libextern
#chmod +x compilation_library_macos.sh
#./compilation_library_macos.sh
#cd ..

#/bin/sh -c '/usr/bin/make -j12 -e -f Makefile.macos'
