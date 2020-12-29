#!/bin/bash
NBPROC=$(sysctl -n hw.ncpu)
echo $NBPROC

#update pacman
brew update 
brew upgrade
xcode-select --install
brew install -v pkgconfig
brew install -v pango 
brew install -v libexif 
brew install -v OpenEXR
brew install -v glew
brew install -v coreutils
brew install -v leptonica
brew install -v wget
brew install -v unzip
brew install -v cmake
brew install -v yasm
brew install -v tbb
brew install -v sdl2
brew install -v tesseract
brew install -v qpdf
brew install -v jasper
brew install -v mediainfo
brew install -v libraw
brew install -v libde265
brew install -v aom
brew install -v fdk-aac
brew install -v libvorbis
brew install -v libvpx
brew install -v opus
brew install -v theora
brew install -v x264
brew install -v x265
brew install -v xvid
brew install -v dav1d
brew install -v nasm
brew install -v libjpeg-turbo
brew install -v poppler
#Install External Library
cd libextern
chmod +x compilation_library_macos.sh
./compilation_library_macos.sh
cd ..

#/bin/sh -c '/usr/bin/make -j12 -e -f Makefile.macos'
