#/bin/bash
NBPROC=$(nproc)
echo $NBPROC

export PKG_CONFIG_PATH=$HOME/ffmpeg_build/lib/pkgconfig

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

FILE=vcpkg.tar.gz
if [ ! -f FILE ]; then
    wget -O vcpkg.tar.gz https://github.com/microsoft/vcpkg/archive/master.tar.gz
    tar xf vcpkg.tar.gz
fi
cd vcpkg-master
./bootstrap-vcpkg.sh
./vcpkg install wxWidgets
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
./vcpkg install jasper
./vcpkg install libraw
./vcpkg install tesseract
./vcpkg install openjpeg
./vcpkg install poppler
./vcpkg install sqlite3
./vcpkg install freeimage
./vcpkg install sdl2
cd ..

unzip rav1e-0.5.0-beta.2-ubuntu.zip

rm vcpkg-master/installed/x64-linux/lib/libpng.a

chmod +x ffmpeg_linux.sh
./ffmpeg_linux.sh
