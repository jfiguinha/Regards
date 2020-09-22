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
pacman -S mingw-w64-x86_64-glew
pacman -S mingw-w64-x86_64-openjpeg2
#pacman -S mingw-w64-x86_64-freeimage
pacman -S unzip
pacman -S zip
pacman -S mingw-w64-x86_64-qpdf
pacman -S mingw-w64-x86_64-libexif
pacman -S mingw-w64-x86_64-gnutls
pacman -S mingw-w64-x86_64-leptonica
pacman -S mingw-w64-x86_64-icu
pacman -S mingw-w64-x86_64-intel-tbb
#install OpenCL SDK
#wget https://github.com/GPUOpen-LibrariesAndSDKs/OCL-SDK/releases/download/1.0/OCL_SDK_Light_AMD.exe

#Install External Library
cd libextern
chmod +x compilation_library_window.sh
./compilation_library_window.sh
cd ..

cp libextern/tesseract-4.1.0/tesscallback.h ~/ffmpeg_build/include/tesseract

# /bin/sh -c '/usr/bin/make -j12 -e -f Makefile'
#cd libextern/
#mkdir build
#cp ~/ffmpeg_build/bin/*.dll build
