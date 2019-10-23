#!/bin/bash
NBPROC=$(nproc)
echo $NBPROC

#Get libjasper
wget http://www.ece.uvic.ca/~frodo/jasper/software/jasper-2.0.14.tar.gz
tar xf jasper-2.0.14.tar.gz

#get lib poppler
wget https://poppler.freedesktop.org/poppler-0.81.0.tar.xz
tar xf poppler-0.81.0.tar.xz

#get Tesseract
wget https://github.com/tesseract-ocr/tesseract/archive/4.1.0.zip
unzip 4.1.0.zip

#compile tesseract
cd tesseract-4.1.0
mkdir build
cd build
cmake ../  -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build"
make -j$NBPROC
cd ..
cd ..

#decompression
sudo apt install unzip
tar xf  MediaInfo_DLL_GNU_FromSource.tar.gz

#compile jasper
cd jasper-2.0.14
mkdir build
cd build
cmake ../  -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build"
make -j$NBPROC
cd ..
cd ..

#Compile libpoppler
cd poppler-0.81.0
mkdir build
cd build
cmake ../  -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build"
make -j$NBPROC
cd ..
cd ..

#Compile qpdf
cd qpdf-master
chmod +x configure
./configure
make -j$NBPROC
cd ..

#Compile exiv2-0.26 :
cd exiv2-0.26 
chmod +x configure
./configure --enable-video --enable-webready
make -j$NBPROC
cd ..

#Compile fftw-3.3.8  :
cd fftw-3.3.8 
chmod +x configure
./configure --enable-float
make -j$NBPROC
cd ..

#Compile heif-master
cd heif-master/srcs 
cmake ../srcs 
make -j$NBPROC
cd ..
cd ..

#Compile libde265-master
cd libde265-master
cmake ../libde265-master 
make -j$NBPROC
cd ..

#Compile MediaInfo_DLL_GNU_FromSource
cd MediaInfo_DLL_GNU_FromSource
chmod +x SO_Compile.sh
 ./SO_Compile.sh
cd .. 

#Compile SDL2-2.0.5
cd SDL2-2.0.5 
./configure
make -j$NBPROC
cd ..

#Compile wxWidgets-master
cd wxWidgets-master
chmod +x configure
chmod +x src/stc/gen_iface.py
./configure --with-libjpeg=builtin --with-libpng=builtin --with-libtiff=builtin --enable-monolithic --enable-unicode --disable-shared --disable-log --disable-debug 
make -j$NBPROC
cd ..

#Compile x265_2.5
cd x265_2.5/build/linux
chmod +x multilib.sh
./multilib.sh
cd .. 
cd .. 
cd .. 

#Compile libexif
cd libexif-master
chmod +x configure
./configure
make -j$NBPROC
cd ..

#Compille ffmpeg
./ffmpeg_build.sh
