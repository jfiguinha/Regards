#!/bin/bash
NBPROC=$(nproc)
echo $NBPROC

#Get libjasper
wget http://www.ece.uvic.ca/~frodo/jasper/software/jasper-2.0.14.tar.gz
tar xf jasper-2.0.14.tar.gz

#get lib poppler
wget https://poppler.freedesktop.org/poppler-0.81.0.tar.xz
tar xf poppler-0.81.0.tar.xz

#get Tesseract
wget https://github.com/tesseract-ocr/tesseract/archive/4.1.1.zip
unzip 4.1.1.zip

#compile leptonica
tar xf leptonica-1.79.0.tar.gz
cd leptonica-1.79.0/
mkdir build
cd build
cmake ../  -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build"
make -j$NBPROC
sudo make install
cd ..
cd ..

#compile tesseract
cd tesseract-4.1.1
mkdir build
cd build
cmake ../  -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build"
make -j$NBPROC
sudo make install
cd ..
cd ..


#decompression
tar xf  MediaInfo_DLL_GNU_FromSource.tar.gz

#compile glew
cd glew-2.1.0
chmod +x config/config.guess
make
cd ..

#compile jasper
cd jasper-2.0.14
mkdir build
cd build
cmake ../  -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build"
make -j$NBPROC
sudo make install
cd ..
cd ..

#Compile libpoppler
cd poppler-0.81.0
mkdir build
cd build
cmake ../  -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build"
make -j$NBPROC
sudo make install
cd ..
cd ..

#Compile qpdf
cd qpdf-master
chmod +x configure
./configure --prefix="$HOME/ffmpeg_build"
make -j$NBPROC
chmod +x mkinstalldirs
chmod +x install-sh
sudo make install
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

#Compile SDL2-2.0.5
cd SDL2-2.0.5 
chmod +x configure
./configure
make -j$NBPROC
cd ..

#Compile wxWidgets-master
unzip wxWidgets-master.zip
cd wxWidgets-master
./configure --enable-monolithic --enable-unicode --disable-shared --disable-log --disable-debug --with-gtk=3
make -j$NBPROC
cd ..

#Compile x265_2.5
cd x265_2.5/build/linux
chmod +x multilib.sh
./multilib.sh
cd .. 
cd .. 
cd .. 

unzip openexr-2.5.2.zip
cd openexr-2.5.2
mkdir build
cd build
cmake -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=Release -DZLIB_ROOT=../../zlib ../ -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build"
make -j$NBPROC
sudo make install
cd .. 
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

#Compille ffmpeg
./ffmpeg_build.sh

cd libheif-1.8.0
mkdir build
cd build
cmake ../ -DAOM_INCLUDE_DIR="../../aom" -DLIBDE265_INCLUDE_DIR="../../libde265-master"
make -j$NBPROC
cd ..
cd ..
