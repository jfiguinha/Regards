#/bin/bash
NBPROC=$(nproc)
echo $NBPROC

export PKG_CONFIG_PATH=$HOME/ffmpeg_build/lib/pkgconfig

unzip vcpkg-2022.08.15.zip
cd vcpkg-2022.08.15
./bootstrap-vcpkg.sh
./vcpkg install wxWidgets
cd ..

git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg install ffnvcodec
cd ..
unzip SVT-AV1-master.zip
cd SVT-AV1-master/Build
PATH="$HOME/bin:$PATH" cmake -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX="$HOME/developpement/git/Regards/libextern/vcpkg/installed/x64-linux" -DCMAKE_BUILD_TYPE=Release -DBUILD_DEC=OFF -DBUILD_SHARED_LIBS=OFF ..
PATH="$HOME/bin:$PATH" make -j
make install
PATH="$HOME/bin:$PATH" cmake -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX="$HOME/developpement/git/Regards/libextern/vcpkg/installed/x64-linux/debug" -DCMAKE_BUILD_TYPE=Debug -DBUILD_DEC=OFF -DBUILD_SHARED_LIBS=OFF ..
PATH="$HOME/bin:$PATH" make -j
make install
cd ..
cd ..
cd vcpkg
./vcpkg install libavif
./vcpkg install ffmpeg[gpl,aom,dav1d,x265,x264,openh264,vpx,webp,vorbis,mp3lame,nvcodec,opencl,openjpeg,opus]
./vcpkg install opencv4[contrib,core,dnn,ffmpeg,ipp,jpeg,openmp,png,tiff,webp]
./vcpkg install tbb
./vcpkg install exiv2[video,xmp]
./vcpkg install libmediainfo
./vcpkg install libde265
./vcpkg install libexif
./vcpkg install jasper
./vcpkg install libraw
./vcpkg install poppler
./vcpkg install sqlite3
./vcpkg install freeimage
./vcpkg install libjxl
./vcpkg install libepoxy
cd ..

#decompress tbb
tar -xf oneapi-tbb-2021.7.0.tar.gz

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
rm $LOCALPATH/vcpkg/installed/x64-linux/lib/libhardfbuzz.a
rm $LOCALPATH/vcpkg/installed/x64-linux/lib/libhardfbuzz-subset.a

unzip rav1e-0.5.0-beta.2-ubuntu.zip



