#!/bin/bash
NBPROC=$(sysctl -n hw.ncpu)
echo $NBPROC

#update pacman
brew update 
brew upgrade
xcode-select --install
brew install -v pkgconfig
brew install -v llvm
brew install -v pango 
brew install -v libexif 
brew install -v OpenEXR
brew install -v glew
brew install -v coreutils
brew install -v leptonica
brew install -v libjpeg-turbo
brew install -v wget
brew install -v unzip
brew install -v cmake
brew install -v yasm
brew install -v tbb
brew install -v libtiff
brew install -v libpng
brew install -v libjpeg
brew install -v x265
brew install -v sdl2
brew install -v tesseract
brew install -v poppler
brew install -v qpdf
brew install -v openexr
brew install -v jasper
brew install -v aom
#brew install -v freeimage
#brew install -v fftw
#Install External Library
#cd libextern
#chmod +x compilation_library_macos.sh
#./compilation_library_macos.sh
#cd ..

#/bin/sh -c '/usr/bin/make -j12 -e -f Makefile.macos'
