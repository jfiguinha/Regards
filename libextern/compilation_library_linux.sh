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

tar xf vcpkg.tar.gz
cd vcpkg-master
./bootstrap-vcpkg.sh
./vcpkg install wxWidgets
./vcpkg install ffmpeg[all-gpl]
./vcpkg install exiv2[video,xmp]
./vcpkg install libmediainfo
./vcpkg install libde265
./vcpkg install libexif
./vcpkg install tbb
./vcpkg install x265
./vcpkg install jasper
./vcpkg install libraw
./vcpkg install poppler
./vcpkg install sqlite3
./vcpkg install freeimage
./vcpkg install tesseract
cd ..

git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg install opencv[contrib,ipp,openmp,ffmpeg]
cd ..

unzip rav1e-0.5.0-beta.2-ubuntu.zip

#Compile wxWidgets
FILE=wxWidgets-3.1.6.zip
if [ ! -f FILE ]; then
    wget https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.6/wxWidgets-3.1.6.zip
    mkdir wxWidgets
    cp wxWidgets-3.1.6.zip wxWidgets/wxWidgets-3.1.6.zip 
    cd wxWidgets
    unzip wxWidgets-3.1.6.zip
    cd ..
fi

cd wxWidgets
./configure --disable-glcanvasegl
make -j$NBPROC 



