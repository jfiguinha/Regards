#!/bin/bash
NBPROC=$(nproc)
echo $NBPROC

#Install Depedency
sudo pacman -S gcc
sudo pacman -S g++
sudo pacman -S build-essential
sudo pacman -S cmake
sudo pacman -S binutils-gold
sudo pacman -S yasm
sudo pacman -S nasm
sudo pacman -S wget
sudo pacman -S autoconf
sudo pacman -S libexif
sudo pacman -S gtk3
sudo pacman -S webkit2gtk
sudo pacman -S freeglut
sudo pacman -S mesa
sudo pacman -S git
sudo pacman -S libjpeg-turbo
sudo pacman -S sdl2
sudo pacman -S glew
sudo pacman -S curl
sudo pacman -S expat
sudo pacman -S libva
sudo pacman -S sane
sudo pacman -S ocl-icd
sudo pacman -S openjpeg2
sudo pacman -S fftw
sudo pacman -S leptonica
sudo pacman -S libwebp
sudo pacman -S intel-tbb
sudo pacman -S bzip2
sudo pacman -S mesa-vdpau
#sudo pacman -S ocl-icd-opencl

#Get AMD SDK
#wget https://github.com/microsoft/LightGBM/releases/download/v2.0.12/AMD-APP-SDKInstaller-v3.0.130.136-GA-linux64.tar.bz2
#Install AMD SDK
#tar -xvf AMD-APP-SDKInstaller-v3.0.130.136-GA-linux64.tar.bz2
#chmod +x AMD-APP-SDK-v3.0.130.136-GA-linux64.sh
#sudo bash AMD-APP-SDK-v3.0.130.136-GA-linux64.sh

#install clinfo
sudo pacman -S clinfo

#Install External Library
cd libextern
chmod +x compilation_library_linux.sh
./compilation_library_linux.sh
cd ..
