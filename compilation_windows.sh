#!/bin/bash
NBPROC=$(nproc)
echo $NBPROC

#update pacman
pacman -Syuu

#installation default package for msys64
pacman -S --needed base-devel mingw-w64-x86_64-toolchain
pacman -S mingw-w64-x86_64-cmake 
pacman -S yasm 
pacman -S nasm 
pacman -S mingw-w64-x86_64-pango
pacman -S mingw-w64-x86_64-fftw 
pacman -S mingw-w64-x86_64-openexr 
pacman -S git 
pacman -S mingw-w64-x86_64-freeglut
pacman -S libxml2-devel
pacman -S mingw-w64-x86_64-glew

#Install External Library
cd libextern
chmod +x compilation_library_window.sh
./compilation_library_window.sh
cd ..

/bin/sh -c '/usr/bin/make -j12 -e -f Makefile'
