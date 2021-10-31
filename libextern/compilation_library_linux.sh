#/bin/bash
NBPROC=$(nproc)
echo $NBPROC

export PKG_CONFIG_PATH=$HOME/ffmpeg_build/lib/pkgconfig


#Get libjasper
FILE=jasper-2.0.14.tar.gz
if [ ! -f FILE ]; then
    wget http://www.ece.uvic.ca/~frodo/jasper/software/jasper-2.0.14.tar.gz
    tar xf jasper-2.0.14.tar.gz
fi

#compile jasper
cd jasper-2.0.14
cd build
cmake ../  -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build" -DBUILD_SHARED_LIBS=OFF -DBUILD_STATIC_LIBS=ON
make -j$NBPROC
sudo make install
cd ..
cd ..

#Compile LibRaw
unzip LibRaw-0.20.0.zip
cd LibRaw-0.20.0
./configure --prefix="$HOME/ffmpeg_build"
make -j$NBPROC
sudo make install
cd ..

FILE=leptonica-1.82.0.tar.gz
if [ ! -f FILE ]; then
    wget https://github.com/DanBloomberg/leptonica/releases/download/1.82.0/leptonica-1.82.0.tar.gz
    tar xf leptonica-1.82.0.tar.gz
fi

cd leptonica-1.82.0
./autogen.sh
./configure --prefix="$HOME/ffmpeg_build" --disable-shared
make -j$NBPROC
sudo make install
cd ..

#compile tesseract
FILE=4.1.1.zip
if [ ! -f FILE ]; then
    wget https://github.com/tesseract-ocr/tesseract/archive/refs/tags/4.1.1.zip
    unzip 4.1.1.zip
fi

cd tesseract-4.1.1
./autogen.sh
./configure --prefix="$HOME/ffmpeg_build" --disable-shared
make -j$NBPROC
sudo make install
cd ..

FILE=v2.4.0.zip
if [ ! -f FILE ]; then
    wget https://github.com/uclouvain/openjpeg/archive/refs/tags/v2.4.0.zip
    unzip v2.4.0.zip
fi

cd openjpeg-2.4.0
mkdir build
cd build
cmake ../  -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build"
make -j$NBPROC
sudo make install
cd ..
cd ..

#Compile heif-master
unzip heif-master.zip
mv heif-3.6.2 heif-master
cd heif-master/srcs 
cmake ../srcs -DBUILD_SHARED_LIBS=OFF -DBUILD_STATIC_LIBS=ON
make -j$NBPROC
cd ..
cd ..


FILE=opencv/opencv_contrib-4.5.1.zip
if [ ! -f FILE ]; then
    mkdir opencv

    wget https://github.com/opencv/opencv_contrib/archive/4.5.1.zip
    mv 4.5.1.zip opencv/opencv_contrib-4.5.1.zip

    wget https://github.com/opencv/opencv/archive/4.5.1.zip
    mv 4.5.1.zip opencv/opencv-4.5.1.zip
fi

#compile opencv
cd opencv
unzip opencv-4.5.1.zip
unzip opencv_contrib-4.5.1.zip
cd opencv-4.5.1
mkdir build
cd build
cmake -DBUILD_SHARED_LIBS=ON -DBUILD_STATIC_LIBS=OFF -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build" -DBUILD_opencv_python=OFF -DOPENCV_EXTRA_MODULES_PATH:PATH="../../opencv_contrib-4.5.1/modules" -DBUILD_opencv_freetype=OFF -DCMAKE_BUILD_TYPE=RELEASE -DBUILD_PERF_TESTS=OFF -DBUILD_TESTS=OFF -DBUILD_opencv_apps=OFF -DBUILD_EXAMPLES=OFF -DCMAKE_CXX_FLAGS="-std=gnu++14 -I ../../../eigen/Eigen" -DOPENCV_ALLOCATOR_STATS_COUNTER_TYPE=int64_t ../
make -j$NBPROC
sudo make install
cd ..
cd ..
cd ..

#Compile qpdf
FILE=4.5.1.zip
if [ ! -f FILE ]; then
    wget https://github.com/qpdf/qpdf/archive/refs/tags/release-qpdf-10.3.2.zip
    unzip release-qpdf-10.3.2.zip
fi

cd qpdf-release-qpdf-10.3.2
./autogen.sh
./configure --prefix="$HOME/ffmpeg_build" --disable-shared
make -j$NBPROC
sudo make install
cd ..

wget -O vcpkg.tar.gz https://github.com/microsoft/vcpkg/archive/master.tar.gz
tar xf vcpkg.tar.gz
cd vcpkg-master
./bootstrap-vcpkg.sh
./vcpkg install exiv2
./vcpkg install libmediainfo
./vcpkg install opencl
./vcpkg install libde265
./vcpkg install fftw3
./vcpkg install libexif
./vcpkg install tbb
./vcpkg install glew
./vcpkg install x265
./vcpkg install libwebp
cd ..

#Compile libpoppler
FILE=poppler-21.10.0.tar.xz
if [ ! -f FILE ]; then
    wget https://poppler.freedesktop.org/poppler-21.10.0.tar.xz
    tar xf poppler-21.10.0.tar.xz
fi

cd poppler-21.10.0 
mkdir build
cd build
cmake ../  -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build" -DBUILD_SHARED_LIBS=OFF -DBUILD_STATIC_LIBS=ON -DENABLE_QT5=OFF -DENABLE_BOOST=OFF
make -j$NBPROC 
sudo make install
cd ..
cd ..

#Compile wxWidgets-master
unzip wxWidgets-master.zip
cd wxWidgets-master
./configure --prefix="$HOME/ffmpeg_build" --enable-monolithic --enable-unicode --disable-shared --disable-log --disable-debug --with-gtk=3 --with-libtiff=no
make -j$NBPROC
sudo make install
cd ..

unzip rav1e-0.5.0-beta.2-ubuntu.zip

#tiff
unzip tiff-4.3.0.zip
cd tiff-4.3.0
./autogen.sh
./configure --prefix="$HOME/ffmpeg_build" --disable-shared
make -j$NBPROC
sudo make install
cd ..
sudo rm -r tiff-4.3.0

chmod +x ffmpeg_linux.sh
./ffmpeg_linux.sh

sudo cp tesscallback.h ~/ffmpeg_build/include/tesseract
