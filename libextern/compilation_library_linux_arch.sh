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

#Compile qpdf
FILE=release-qpdf-10.3.2.zip
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
./vcpkg install opencv[contrib]
./vcpkg install wxWidgets
./vcpkg install exiv2
./vcpkg install libmediainfo
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
./vcpkg install boost-compute
./vcpkg install opencl
cd ..

rm vcpkg-master/installed/x64-linux/lib/libpng.a
