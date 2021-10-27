#!/bin/bash
NBPROC=$(nproc)
echo $NBPROC

#Install Depedency
sudo pacman -S gcc
sudo pacman -S cmake
sudo pacman -S yasm
sudo pacman -S nasm
sudo pacman -S meson
sudo pacman -S make
sudo pacman -S wget
sudo pacman -S autoconf
sudo pacman -S automake
sudo pacman -S pkg-config
sudo pacman -S patchelf
sudo pacman -S libgnutls28
sudo pacman -S libtool
sudo pacman -S sndio
sudo pacman -S libunistring
sudo pacman -S opencl-headers
sudo pacman -S gtk3
sudo pacman -S webkit2gtk
sudo pacman -S freeglut
sudo pacman -S mesa
sudo pacman -S sdl2
sudo pacman -S libjpeg-turbo
sudo pacman -S sane
sudo pacman -S curl4-openssl
sudo pacman -S ssh
sudo pacman -S lzma
#sudo pacman -S x265
#sudo pacman -S tesseract
#sudo pacman -S poppler
#sudo pacman -S qpdf
#sudo pacman -S openexr
#sudo pacman -S ocl-icd-opencl

#Get AMD SDK
#wget https://github.com/microsoft/LightGBM/releases/download/v2.0.12/AMD-APP-SDKInstaller-v3.0.130.136-GA-linux64.tar.bz2
#Install AMD SDK
#tar -xvf AMD-APP-SDKInstaller-v3.0.130.136-GA-linux64.tar.bz2
#chmod +x AMD-APP-SDK-v3.0.130.136-GA-linux64.sh
#sudo bash AMD-APP-SDK-v3.0.130.136-GA-linux64.sh


#Install External Library
cd libextern
chmod +x compilation_library_linux.sh
./compilation_library_linux.sh
cd ..
