#!/bin/bash
NBPROC=$(nproc)
echo $NBPROC

#Get libjasper
wget http://www.ece.uvic.ca/~frodo/jasper/software/jasper-2.0.14.tar.gz
tar xf jasper-2.0.14.tar.gz

#decompression
tar xf  MediaInfo_DLL_GNU_FromSource.tar.gz

#compile jasper
cd jasper-2.0.14
mkdir build
cd build
cmake ../  -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build"
make -j$NBPROC
sudo make install
cd ..
cd ..

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
cd MediaInfo_DLL_GNU_FromSource
chmod +x SO_Compile.sh
 ./SO_Compile.sh
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
./configure --enable-monolithic --enable-unicode --disable-shared --disable-log --disable-debug --with-gtk=3
make -j$NBPROC
cd ..


#compile opencv
cd opencv
unzip opencv-4.3.0.zip
unzip opencv_contrib-4.3.0.zip
cd opencv-4.3.0
mkdir build
cd build
cmake -DBUILD_SHARED_LIBS=ON -DBUILD_STATIC_LIBS=OFF -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build" -DBUILD_opencv_python=OFF -DOPENCV_EXTRA_MODULES_PATH:PATH="../../opencv_contrib-4.3.0/modules" -DBUILD_opencv_freetype=OFF -DCMAKE_BUILD_TYPE=RELEASE -DBUILD_PERF_TESTS=OFF -DBUILD_TESTS=OFF -DBUILD_opencv_apps=OFF -DBUILD_EXAMPLES=OFF -DCMAKE_CXX_FLAGS="-std=gnu++14 -I ../../../eigen/Eigen" -DOPENCV_ALLOCATOR_STATS_COUNTER_TYPE=int64_t ../
make -j$NBPROC
sudo make install
cd ..
cd ..
cd ..

# libaom
git clone https://aomedia.googlesource.com/aom
cd aom
cd build
cmake ../../aom -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build"
make -j$NBPROC
sudo make install
cd ..
cd ..

#Compille ffmpeg
tar xf ffmpeg-4.3.1.tar.xz
mv ffmpeg-4.3.1 ffmpeg-master
cd ffmpeg-master
export PKG_CONFIG_PATH=$HOME/ffmpeg_build/lib/pkgconfig
./configure --prefix="$HOME/ffmpeg_build" --extra-cflags="-I$HOME/ffmpeg_build/include" --extra-ldflags="-L$HOME/ffmpeg_build/lib" --bindir="/usr/bin" --enable-gpl --enable-nonfree --enable-libaom
make -j$NBPROC
cd ..

#compile tesseract
unzip tesseract-4.1.1.zip
cd tesseract-4.1.1
mkdir build
cd build
cmake ../  -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build"
make -j$NBPROC
sudo make install
cd ..
cd ..

#Compile libpoppler
tar xf poppler-20.11.0.tar.xz
cd poppler-20.11.0
mkdir build
cd build
cmake ../  -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build"
make -j$NBPROC
sudo make install
cd ..
cd ..

#Compile qpdf
tar xf qpdf-10.0.3.tar.gz
cd qpdf-10.0.3
./configure --prefix="$HOME/ffmpeg_build"
make -j$NBPROC
sudo make install
cd ..

#compile x265
tar xf x265_3.2.tar.gz
cd x265_3.2/source
mkdir build
cd build
cmake ../  -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build"
sudo make install
cd .. 
cd .. 
cd .. 

#compile OpenEXR
unzip openexr-2.5.2.zip
cd openexr-2.5.2
mkdir build
cd build
cmake ../  -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build"
sudo make install
cd .. 
cd .. 



