#/bin/bash
NBPROC=$(nproc)
echo $NBPROC

export PKG_CONFIG_PATH=$HOME/ffmpeg_build/lib/pkgconfig



tar xf vcpkg.tar.gz
cd vcpkg-master
./bootstrap-vcpkg.sh
./vcpkg install wxWidgets
./vcpkg install ffnvcodec
./vcpkg install ffmpeg[all-gpl]
./vcpkg install opencv[contrib,ipp,openmp]
./vcpkg install exiv2[video,xmp]
./vcpkg install tbb
./vcpkg install libmediainfo
./vcpkg install libde265
./vcpkg install libexif
./vcpkg install jasper
./vcpkg install libraw
./vcpkg install poppler
./vcpkg install sqlite3
./vcpkg install freeimage
cd ..

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

#LOCALPATH=$HOME/developpement/git/Regards/libextern
LOCALPATH=$(pwd)
echo $LOCALPATH

export pc_libjpeg_CFLAGS=$LOCALPATH/vcpkg-master/installed/x64-linux/include

export pc_libjpeg_LIBS=$LOCALPATH/vcpkg-master/installed/x64-linux/lib/libjpeg.a


cd qpdf-release-qpdf-10.3.2
./autogen.sh
./configure --prefix="$HOME/ffmpeg_build" --disable-shared --enable-crypto-native --disable-crypto-openssl --disable-crypto-gnutls 
make -j$NBPROC CXXFLAGS="-I$LOCALPATH/vcpkg-master/installed/x64-linux/include"

sudo make install
cd ..

rm $LOCALPATH/vcpkg-master/installed/x64-linux/lib/libpng.a
rm $LOCALPATH/vcpkg-master/installed/x64-linux/lib/libhardfbuzz.a
rm $LOCALPATH/vcpkg-master/installed/x64-linux/lib/libhardfbuzz-subset.a

cp $LOCALPATH/tesscallback.h $LOCALPATH/vcpkg-master/installed/x64-linux/include

unzip rav1e-0.5.0-beta.2-ubuntu.zip



