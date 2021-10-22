#/bin/bash
NBPROC=$(nproc)
echo $NBPROC

unzip heif-master.zip

#Compile heif-master
mv heif-3.6.2 heif-master
cd heif-master/srcs 
cmake ../srcs 
make -j$NBPROC
cd ..
cd ..

mkdir opencv

wget https://github.com/opencv/opencv_contrib/archive/4.5.1.zip
mv 4.5.1.zip opencv/opencv_contrib-4.5.1.zip

wget https://github.com/opencv/opencv/archive/4.5.1.zip
mv 4.5.1.zip opencv/opencv-4.5.1.zip

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
tar xf qpdf-10.0.3.tar.gz
cd qpdf-10.0.3
./configure --prefix="$HOME/ffmpeg_build"
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
./vcpkg install tesseract
./vcpkg install libraw
./vcpkg install fftw3
./vcpkg install poppler
./vcpkg install libexif
./vcpkg install tbb
./vcpkg install glew
./vcpkg install x265
cd ..

#Compile wxWidgets-master
unzip wxWidgets-master.zip
cd wxWidgets-master
./configure --prefix="$HOME/ffmpeg_build" --enable-monolithic --enable-unicode --disable-shared --disable-log --disable-debug --with-gtk=3 --with-libtiff=no
make -j$NBPROC
sudo make install
cd ..


chmod +x ffmpeg_linux.sh
./ffmpeg_linux.sh

