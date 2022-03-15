#!/bin/bash
NBPROC=$(sysctl -n hw.ncpu)
echo $NBPROC

#decompression
unzip heif-master.zip

#Compile heif-master
mv heif-3.6.2 heif-master
cd heif-master/srcs 
cmake ../srcs 
make -j$NBPROC
cd ..
cd ..

#update pacman
brew update 
brew upgrade
xcode-select --install
brew install -v cmake
brew install -v opencv
brew install -v jasper
brew install -v jpeg-turbo
brew install -v libde265
brew install -v libraw
brew install -v poppler
brew install -v mediainfo
brew install -v libexif
brew install -v qpdf
brew install -v exiv2
brew install -v freeimage
brew install -v wxWidgets