#!/bin/bash
NBPROC=$(sysctl -n hw.ncpu)
echo $NBPROC

#decompression
tar xf  fftw-3.3.8.tar.gz
tar xf  exiv2-0.27.4.tar.gz
unzip heif-master.zip

#Compile exiv2-0.27.3 :
mv exiv2-0.27.4-Source exiv2-0.27.4
cd exiv2-0.27.4
mkdir build
cd build 
cmake ../ -DEXIV2_ENABLE_BMFF=ON -DEXIV2_ENABLE_VIDEO=ON
make -j$NBPROC
cd ..
cd ..


#Compile heif-master
mv heif-3.6.2 heif-master
cd heif-master/srcs 
cmake ../srcs 
make -j$NBPROC
cd ..
cd ..


#Compile wxWidgets-master
unzip wxWidgets-master.zip
cd wxWidgets-master
./configure --enable-unicode --enable-monolithic --disable-log --disable-debug --disable-shared --with-cxx=14
make -j$NBPROC
cd ..

cd fftw-3.3.8
./configure --enable-float
make -j$NBPROC
cd ..

tar xf ffmpeg-4.4.tar.xz
mv ffmpeg-4.4 ffmpeg-master
cd ffmpeg-master
./configure
make -j$NBPROC
cd ..
