#/bin/bash
NBPROC=$(nproc)
echo $NBPROC

#LOCALPATH=$HOME/developpement/git/Regards/libextern
LOCALPATH=$(pwd)
echo $LOCALPATH

export PKG_CONFIG_PATH=$HOME/ffmpeg_build/lib/pkgconfig

unzip vcpkg-2024.08.23_mac.zip
mv vcpkg-2024.08.23 vcpkg

cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg install wxwidgets[webview]
./vcpkg install sdl2
cd ..

unzip ports_vcpkg_mac.zip

cp -r ports_vcpkg/opencv4 vcpkg/ports

unzip SVT-AV1-v2.2.1.zip
cd SVT-AV1-v2.2.1/Build
PATH="$HOME/bin:$PATH" cmake -DCMAKE_INSTALL_PREFIX="$LOCALPATH/vcpkg/installed/arm64-osx" -DCMAKE_BUILD_TYPE=Release -DBUILD_DEC=OFF -DBUILD_SHARED_LIBS=OFF ..
PATH="$HOME/bin:$PATH" make -j
make install
PATH="$HOME/bin:$PATH" cmake -DCMAKE_INSTALL_PREFIX="$LOCALPATH/vcpkg/installed/arm64-osx/debug" -DCMAKE_BUILD_TYPE=Debug -DBUILD_DEC=OFF -DBUILD_SHARED_LIBS=OFF ..
PATH="$HOME/bin:$PATH" make -j
make install
cd ..
cd ..

cd vcpkg
./vcpkg install ffmpeg[gpl,aom,dav1d,x265,x264,openh264,vpx,webp,vorbis,mp3lame,openjpeg,opus,fdk-aac,ffmpeg] --allow-unsupported
./vcpkg install opencv4[contrib,core,dnn,ffmpeg,ipp,jpeg,tbb,png,tiff,webp,openexr,opengl]
./vcpkg install libde265
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
./vcpkg install openal-soft
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

rm $LOCALPATH/vcpkg/installed/arm64-osx/lib/libpng.a
rm -r $LOCALPATH/vcpkg/installed/arm64-osx/include/OpenCL





