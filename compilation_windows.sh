#!/bin/bash
NBPROC=$(nproc)
echo $NBPROC

#update pacman
pacman -Syuu

#installation default package for msys64
pacman -Ss --needed base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake 
pacman -Ss yasm 
pacman -Ss nasm 
pacman -Ss mingw-w64-x86_64-pango
pacman -Ss mingw-w64-x86_64-fftw 
pacman -Ss mingw-w64-x86_64-openexr 
pacman -Ss git 
pacman -Ss mingw-w64-x86_64-freeglut

#Install External Library
cd libextern
chmod +x compilation_library_window.sh
./compilation_library_window.sh
cd ..

/bin/sh -c '/usr/bin/make -j12 -e -f Makefile'
