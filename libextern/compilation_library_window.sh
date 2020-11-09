#!/bin/bash
NBPROC=$(nproc)
echo $NBPROC

tar xf  MediaInfo_DLL_GNU_FromSource.tar.gz

export PKG_CONFIG_PATH=$HOME/ffmpeg_build/lib/pkgconfig

#Compile exiv2-0.27.3 :
cd exiv2-0.27.3
mkdir build
cd build 
cmake -G "MSYS Makefiles" ../
make -j$NBPROC
cd ..
cd ..

#Compile heif-master
cd heif-master/srcs 
cmake -G "MSYS Makefiles" ../srcs 
make -j$NBPROC
cd ..
cd ..

#Compile libde265-master
cd libde265-master
mkdir build
cd build
cmake -G "MSYS Makefiles" ../
make -j$NBPROC
make install
cd ..
cd ..

#Compile wxWidgets-master
unzip wxWidgets-master.zip
cd wxWidgets-master
./configure --enable-monolithic --enable-unicode --disable-shared --disable-log --disable-debug
make -j$NBPROC
cd ..

#Compile Zen From MediaInfo_DLL_GNU_FromSource
cd MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake
cmake ../CMake/ -G "MSYS Makefiles" -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build"
make -j$NBPROC
make install
cd .. 
cd .. 
cd .. 
cd .. 

#Compile MediaInfoLib From MediaInfo_DLL_GNU_FromSource
cd MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/GNU/Library
./configure
make -j$NBPROC
cd ..
cd ..
cd ..
cd ..
cd ..