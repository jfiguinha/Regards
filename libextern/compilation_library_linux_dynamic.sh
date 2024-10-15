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


unzip vcpkg-2024.08.23.zip
mv vcpkg-2024.08.23 vcpkg

cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg install x265:x64-linux-dynamic
./vcpkg install libde265:x64-linux-dynamic
./vcpkg install opencv4[contrib,core,dnn,ffmpeg,ipp,jpeg,tbb,png,tiff,webp,openexr]:x64-linux-dynamic
./vcpkg install opencl:x64-linux-dynamic
./vcpkg install dav1d:x64-linux-dynamic
./vcpkg install exiv2[video,xmp,bmff]:x64-linux-dynamic
./vcpkg install libavif[aom,dav1d]:x64-linux-dynamic
./vcpkg install libmediainfo:x64-linux-dynamic
./vcpkg install libexif:x64-linux-dynamic
./vcpkg install jasper:x64-linux-dynamic
./vcpkg install libraw[dng-lossy,openmp]:x64-linux-dynamic
./vcpkg install poppler:x64-linux-dynamic
./vcpkg install sqlite3:x64-linux-dynamic
./vcpkg install freeimage:x64-linux-dynamic
./vcpkg install libjxl:x64-linux-dynamic
./vcpkg install libepoxy:x64-linux-dynamic
./vcpkg install ncnn[vulkan]:x64-linux-dynamic
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

