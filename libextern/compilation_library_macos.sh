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

git clone https://github.com/oneapi-src/oneTBB.git
cd oneTBB
mkdir build
cd build
cmake ../ -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build" -DBUILD_SHARED_LIBS=ON -DTBB_TEST=FALSE
make -j$NBPROC
make install
cd ..
cd ..

