#!/bin/bash
NBPROC=$(sysctl -n hw.ncpu)
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

#install leptonica
tar xf  leptonica-1.79.0.tar.gz
cd leptonica-1.79.0
mkdir build
cd build
cmake ../  -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build" -DCMAKE_C_FLAGS="-I /usr/local/opt/openjpeg/include/openjpeg-2.3"
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

#Compile libpoppler
cd poppler-0.81.0
mkdir build
cd build
cmake ../  -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build"
make -j$NBPROC
sudo make install
cd ..
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

#Compile qpdf
cd qpdf-master
chmod +x configure
./configure --prefix="$HOME/ffmpeg_build"
make -j$NBPROC
chmod +x mkinstalldirs
chmod +x install-sh
sudo make install
cd ..


#decompression
tar xf  MediaInfo_DLL_GNU_FromSource.tar.gz
tar xf  fftw-3.3.8.tar.gz


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
./configure --enable-monolithic --enable-unicode --disable-shared --disable-log --disable-debug --with-cxx=11
make -j$NBPROC
cd ..

#Compile x265_2.5
cd x265_2.5/source 
cmake ../source
make -j$NBPROC
cd .. 
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
./ffmpeg_build_macos.sh
