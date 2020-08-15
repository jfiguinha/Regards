#!/bin/bash
NBPROC=$(nproc)
echo $NBPROC

#Install Depedency
sudo apt install gcc
sudo apt install g++
sudo apt install build-essential
sudo apt install cmake
sudo apt install binutils-gold
sudo apt install yasm
sudo apt install nasm
sudo apt install autoconf
sudo apt install libexif-dev
sudo apt install libgtk-3-dev
sudo apt install libwebkit2gtk-4.0-dev
sudo apt install freeglut3-dev
sudo apt install mesa-common-dev
sudo apt install git
sudo apt install unzip
sudo apt install libturbojpeg-dev
sudo apt install libsdl2-dev
sudo apt install libglew-dev
sudo apt install libcurl4-openssl-dev
sudo apt install libssh-dev
sudo apt install libexpat-dev
sudo apt install liblzma-dev
sudo apt install libfreeimage-dev
sudo apt install libva-dev
sudo apt install libsane-dev
sudo apt install libleptonica-dev
sudo apt install ocl-icd-opencl-dev
sudo apt install libopenjp2-7-dev
sudo apt install libfftw3-dev
sudo apt install libwebp-dev
sudo apt install libtbb-dev
sudo apt install libvdpau-dev
sudo apt install libbz2-dev
#sudo apt install libopenexr-dev
#sudo apt install ocl-icd-opencl-dev

#Get AMD SDK
#wget https://github.com/microsoft/LightGBM/releases/download/v2.0.12/AMD-APP-SDKInstaller-v3.0.130.136-GA-linux64.tar.bz2
#Install AMD SDK
#tar -xvf AMD-APP-SDKInstaller-v3.0.130.136-GA-linux64.tar.bz2
#chmod +x AMD-APP-SDK-v3.0.130.136-GA-linux64.sh
#sudo bash AMD-APP-SDK-v3.0.130.136-GA-linux64.sh

#install clinfo
sudo apt install clinfo

#Install External Library
cd libextern
chmod +x compilation_library_linux.sh
./compilation_library_linux.sh
cd ..

sudo cp libextern/tesseract-4.1.0/tesscallback.h ~/ffmpeg_build/include/tesseract

unzip project_linux.zip

export PROJECTPATH=$PWD
make -j$NBPROC

mkdir build

cp -R ./RegardsViewer_linux/Release ./build
