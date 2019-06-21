#!/bin/bash
NBPROC=$(sysctl -n hw.ncpu)
echo $NBPROC

#decompression
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
cmake ../srcs 
make -j$NBPROC
cd ..
cd ..

#Compile fftw
cd fftw-3.3.8
 ./configure --enable-float 
make -j$NBPROC
cd ..

#Compile jasper-1.900.1
cd jasper-1.900.1
./configure 
make -j$NBPROC
cd ..

#Compile libde265-master
cd libde265-master
cmake ../libde265-master 
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
cmake ../libwebp-0.6.0
make -j$NBPROC
cd .. 


#Compile MediaInfo_DLL_GNU_FromSource
cd MediaInfo_DLL_GNU_FromSource/ZenLib/Project/GNU/Library
./configure
make -j$NBPROC
cd ..
cd ..
cd ..
cd ..
cd ..

cd MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/GNU/Library
./configure
make -j$NBPROC
cd ..
cd ..
cd ..
cd ..
cd ..
cd ..

#Compile SDL2-2.0.5
cd SDL2-2.0.5 
chmod +x configure
./configure
make -j$NBPROC
cd ..

#Compile wxWidgets-master
cd wxWidgets-master
chmod +x configure
chmod +x src/stc/gen_iface.py
./configure --with-libjpeg=builtin --with-libpng=builtin --with-libtiff=builtin --enable-monolithic --enable-unicode --disable-shared --disable-log --disable-debug --with-cxx=11
make -j$NBPROC
cd ..

#Compile x265_2.5
cd x265_2.5/source 
cmake ../source
make -j$NBPROC
cd .. 
cd .. 

#Compille ffmpeg
./ffmpeg_build_macos.sh
