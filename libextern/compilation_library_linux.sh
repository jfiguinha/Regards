#/bin/bash
NBPROC=$(nproc)
echo $NBPROC

LOCALPATH=$(pwd)
echo $LOCALPATH

export PKG_CONFIG_PATH=$HOME/ffmpeg_build/lib/pkgconfig

unzip vcpkg-2022.08.15.zip
cd vcpkg-2022.08.15
./bootstrap-vcpkg.sh
./vcpkg install wxwidgets
cd ..


unzip vcpkg-2024.01.12.zip
mv vcpkg-2024.01.12 vcpkg

cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg install x265
./vcpkg install libde265
./vcpkg install opencv4[contrib,core,dnn,ffmpeg,ipp,jpeg,tbb,png,tiff,webp,openexr]
./vcpkg install opencl
./vcpkg install dav1d
./vcpkg install exiv2[video,xmp,bmff]
./vcpkg install libavif[aom,dav1d]
./vcpkg install libmediainfo
./vcpkg install libexif
./vcpkg install jasper
./vcpkg install libraw[dng-lossy,openmp]
./vcpkg install poppler
./vcpkg install sqlite3
./vcpkg install freeimage
./vcpkg install libjxl
./vcpkg install libepoxy
cd ..

#Compile heif-master
unzip heif-master.zip
mv heif-3.6.2 heif-master
cd heif-master/srcs 
cmake ../srcs -DBUILD_SHARED_LIBS=OFF -DBUILD_STATIC_LIBS=ON
make -j$NBPROC
cd ..
cd ..

unzip ffmpeg-master_linux.zip

#Compile qpdf
FILE=release-qpdf-10.3.2.zip
if [ ! -f FILE ]; then
    wget https://github.com/qpdf/qpdf/archive/refs/tags/release-qpdf-10.3.2.zip
    unzip release-qpdf-10.3.2.zip
fi

#LOCALPATH=$HOME/developpement/git/Regards/libextern
LOCALPATH=$(pwd)
echo $LOCALPATH


export pc_libjpeg_LIBS=$LOCALPATH/vcpkg/installed/x64-linux/lib/libjpeg.a


cd qpdf-release-qpdf-10.3.2
./autogen.sh
./configure --prefix="$HOME/ffmpeg_build" --disable-shared --enable-crypto-native --disable-crypto-openssl --disable-crypto-gnutls 
make -j$NBPROC CXXFLAGS="-I$LOCALPATH/vcpkg/installed/x64-linux/include"

sudo make install
cd ..

rm $LOCALPATH/vcpkg-2022.08.15/installed/x64-linux/lib/libpng.a
rm $LOCALPATH/vcpkg-2022.08.15/installed/x64-linux/include/nanosvg.h
rm $LOCALPATH/vcpkg-2022.08.15/installed/x64-linux/include/nanosvgrast.h
rm $LOCALPATH/vcpkg/installed/x64-linux/lib/libpng.a

