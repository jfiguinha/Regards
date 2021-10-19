#!/bin/bash
NBPROC=$(sysctl -n hw.ncpu)
echo $NBPROC

#update pacman
brew update 
brew upgrade
xcode-select --install
brew install -v cmake
brew install -v opencv
brew install -v libomp
brew install -v jasper
brew install -v jpeg-turbo
brew install -v libde265
brew install -v libraw
brew install -v poppler
brew install -v mediainfo
brew install -v libexif
brew install -v qpdf
brew install -v fftw
brew install -v wxWidgets
brew install -v exiv2

#Install External Library
#cd libextern
#chmod +x compilation_library_macos.sh
#./compilation_library_macos.sh
#cd ..

#/bin/sh -c '/usr/bin/make -j12 -e -f Makefile.macos'
