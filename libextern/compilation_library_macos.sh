#!/bin/bash
NBPROC=$(sysctl -n hw.ncpu)
echo $NBPROC

#decompression
tar xf  fftw-3.3.8.tar.gz

#Compile exiv2-0.27.3 :
cd exiv2-0.27.3
mkdir build
cd build 
cmake ../
make -j$NBPROC
cd ..
cd ..


#Compile heif-master
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


