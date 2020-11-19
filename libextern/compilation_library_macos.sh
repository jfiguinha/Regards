#!/bin/bash
NBPROC=$(sysctl -n hw.ncpu)
echo $NBPROC


#decompression
tar xf  MediaInfo_DLL_GNU_FromSource.tar.gz
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

#Compile LibRaw
unzip LibRaw-0.20.0.zip
cd LibRaw-0.20.0
./configure
make -j$NBPROC
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

#compile opencv
cd opencv
unzip opencv-4.3.0.zip
unzip opencv_contrib-4.3.0.zip
cd opencv-4.3.0
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build" -DBUILD_opencv_python=OFF -DOPENCV_EXTRA_MODULES_PATH:PATH="../../opencv_contrib-4.3.0/modules" -DBUILD_opencv_freetype=OFF -DCMAKE_BUILD_TYPE=RELEASE -DBUILD_PERF_TESTS=OFF -DBUILD_TESTS=OFF -DBUILD_opencv_apps=OFF -DBUILD_EXAMPLES=OFF -DCMAKE_CXX_FLAGS="-std=gnu++14 -I ../../../eigen/Eigen" -DOPENCV_ALLOCATOR_STATS_COUNTER_TYPE=int64_t ../
make -j$NBPROC
sudo make install
cd ..
cd ..
cd ..

#Compille ffmpeg
chmod +x ffmpeg_build_macos.sh
./ffmpeg_build_macos.sh

