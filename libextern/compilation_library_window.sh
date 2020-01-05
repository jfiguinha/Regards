#!/bin/bash
NBPROC=$(nproc)
echo $NBPROC

#decompression
pacman -S unzip
#Get libjasper
wget http://www.ece.uvic.ca/~frodo/jasper/software/jasper-2.0.14.tar.gz
tar xf jasper-2.0.14.tar.gz

#get lib poppler
wget https://poppler.freedesktop.org/poppler-0.81.0.tar.xz
tar xf poppler-0.81.0.tar.xz

tar xf  MediaInfo_DLL_GNU_FromSource.tar.gz

export PKG_CONFIG_PATH=$HOME/ffmpeg_build/lib/pkgconfig

#get Tesseract
wget https://github.com/tesseract-ocr/tesseract/archive/4.1.0.zip
unzip 4.1.0.zip

#compile tesseract
cd tesseract-4.1.0
./autogen.sh
./configure --prefix="$HOME/ffmpeg_build"
make -j$NBPROC
make install
cd ..
cd ..

#Generate package FreeImage
cd FreeImage3180
makepkg -sCLf
cd ..

#Compile exiv2-0.26 :
cd exiv2-0.26 
chmod +x configure
./configure --enable-video
make -j$NBPROC
cd ..

#Compile glew-2.1.0
cd glew-2.1.0
mkdir -p lib
mkdir -p bin
cd build/cmake/
cmake ./ -G"MSYS Makefiles" 
make -j$NBPROC
cp lib/*.* ../../lib
cp bin/*.* ../../bin
cd ..
cd ..
cd ..

#Compile heif-master
cd heif-master/srcs 
cmake -G "MSYS Makefiles" ../srcs 
make -j$NBPROC
cd ..
cd ..

#Compile libde265-master
cd libde265-master
mkdir build
cd build
cmake -G "MSYS Makefiles" ../
make -j$NBPROC
cd ..
cd ..

#Compile SDL2-2.0.5
cd SDL2-2.0.5 
./configure
make -j$NBPROC
cd ..

#Compile wxWidgets-master
cd wxWidgets-master
chmod +x configure
chmod +x src/stc/gen_iface.py
./configure --with-libjpeg=builtin --with-libpng=builtin --with-libtiff=builtin --enable-monolithic --enable-unicode --disable-shared --disable-log --disable-debug 
make -j$NBPROC
cd ..

#Compile x265_2.5
cd x265_2.5/build/msys
chmod +x multilib.sh
./multilib.sh
cd .. 
cd .. 
cd .. 

#compile jasper
cd jasper-2.0.14
mkdir build
cd build
cmake ../  -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build"
make -j$NBPROC
make install
cd ..
cd ..

#Compile libpoppler
cd poppler-0.81.0
mkdir build
cd build
cmake ../ -G "MSYS Makefiles" -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build"
make -j$NBPROC
sudo make install
cd ..
cd ..

#Compile Zen From MediaInfo_DLL_GNU_FromSource
cd MediaInfo_DLL_GNU_FromSource/ZenLib/Project/CMake
cmake ../CMake/ -G "MSYS Makefiles" -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build"
make -j$NBPROC
make install
cd .. 
cd .. 
cd .. 
cd .. 

#Compile MediaInfoLib From MediaInfo_DLL_GNU_FromSource
cd MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/GNU/Library
./configure
make -j$NBPROC
cd ..
cd ..
cd ..
cd ..
cd ..

#Compille ffmpeg
./ffmpeg_build_windows.sh
