#!/bin/bash
NBPROC=$(sysctl -n hw.physicalcpu)
echo $NBPROC

#decompression
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

#Compile glew-2.1.0
cd glew-2.1.0
chmod +x config/config.guess
make -j$NBPROC
cd ..

#Compile libde265-master
cd libde265-master
cmake ../libde265-master 
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
