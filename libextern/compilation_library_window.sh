#!/bin/bash
NBPROC=$(nproc)
echo $NBPROC

#decompression
pacman -S unzip
tar xf libmng-2.0.3.tar.gz
unzip jasper-1.900.1.zip
tar xf  MediaInfo_DLL_GNU_FromSource.tar.gz

#Compile exiv2-0.26 :
cd exiv2-0.26 
chmod +x configure
./configure --enable-video
make -j$NBPROC
cd ..

#Compile heif-master
cd heif-master/srcs 
cmake -G "MSYS Makefiles" ../srcs 
make -j$NBPROC
cd ..
cd ..

#Compile jasper-1.900.1
cd jasper-1.900.1
./configure 
make -j$NBPROC
cd ..

#Compile libde265-master
cd libde265-master
cmake -G "MSYS Makefiles" ../libde265-master 
make -j$NBPROC
cd ..

#Compile libjpeg-turbo-1.5.1
cd libjpeg-turbo-1.5.1 
chmod +x configure
./configure
make -j$NBPROC
cd .. 

#Compile libmng-2.0.3
cd libmng-2.0.3 
./configure CFLAGS="-I../libjpeg"
make -j$NBPROC CFLAGS="-I../libjpeg -DNEED_BOOLEAN"
cd ..

#Compile libwebp-0.6.0
cd libwebp-0.6.0
cmake -G "MSYS Makefiles" ../libwebp-0.6.0
make -j$NBPROC
cd .. 

#Compile MediaInfo_DLL_GNU_FromSource
cd MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/GNU/Library
./configure 
make -j$NBPROC
cd .. 
cd .. 
cd .. 
cd .. 
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
cd x265_2.5/build/msys
chmod +x multilib.sh
./multilib.sh
cd .. 
cd .. 
cd .. 

#Compile libexif
cd libexif 
chmod +x configure
./configure
make -j$NBPROC
cd ..

#Compille ffmpeg
./ffmpeg_build_windows.sh
