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

FILE=wxWidgets-3.1.5.tar.bz2
if [ ! -f FILE ]; then
    wget https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.5/wxWidgets-3.1.5.tar.bz2
    tar xf wxWidgets-3.1.5.tar.bz2
fi
cd wxWidgets-3.1.5
./configure --with-libjpeg=builtin --with-libpng=builtin --with-libtiff=builtin --enable-unicode --disable-debug --with-cxx=11
make -j$NBPROC
cd ..

