#/bin/bash
NBPROC=$(nproc)
echo $NBPROC

#LOCALPATH=$HOME/developpement/git/Regards/libextern
LOCALPATH=$(pwd)
echo $LOCALPATH

export PKG_CONFIG_PATH=$HOME/ffmpeg_build/lib/pkgconfig

unzip vcpkg-2024.01.12.zip
mv vcpkg-2024.01.12 vcpkg

cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg install libavif[aom,core]

cd ..
unzip SVT-AV1-v1.8.0.zip
cd SVT-AV1-v1.8.0/Build
PATH="$HOME/bin:$PATH" cmake -DCMAKE_INSTALL_PREFIX="$LOCALPATH/vcpkg/installed/arm64-osx" -DCMAKE_BUILD_TYPE=Release -DBUILD_DEC=OFF -DBUILD_SHARED_LIBS=OFF ..
PATH="$HOME/bin:$PATH" make -j
make install
PATH="$HOME/bin:$PATH" cmake -DCMAKE_INSTALL_PREFIX="$LOCALPATH/vcpkg/installed/arm64-osx/debug" -DCMAKE_BUILD_TYPE=Debug -DBUILD_DEC=OFF -DBUILD_SHARED_LIBS=OFF ..
PATH="$HOME/bin:$PATH" make -j
make install
cd ..
cd ..

cd vcpkg
./vcpkg install ffmpeg[gpl,aom,dav1d,x265,x264,openh264,vpx,webp,vorbis,mp3lame,opencl,openjpeg,opus,fdk-aac] --allow-unsupported
./vcpkg install opencv4[contrib,core,dnn,ffmpeg,ipp,jpeg,openmp,png,tiff,webp]
./vcpkg install tbb
./vcpkg install exiv2[video,xmp]
./vcpkg install libmediainfo
./vcpkg install libde265
./vcpkg install libexif
./vcpkg install sdl2
./vcpkg install jasper
./vcpkg install libraw
./vcpkg install poppler
./vcpkg install sqlite3
./vcpkg install freeimage
./vcpkg install libjxl
./vcpkg install libepoxy
./vcpkg install wxwidgets[webview]
cd ..

#Compile heif-master
unzip heif-master.zip
mv heif-3.6.2 heif-master
cd heif-master/srcs 
cmake ../srcs -DBUILD_SHARED_LIBS=OFF -DBUILD_STATIC_LIBS=ON
make -j$NBPROC
cd ..
cd ..

rm $LOCALPATH/vcpkg/installed/x64-linux/lib/libpng.a





