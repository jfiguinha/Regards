#!/bin/bash
NBPROC=$(sysctl -n hw.ncpu)
echo $NBPROC

#decompression
unzip heif-master.zip

#Compile heif-master
mv heif-3.6.2 heif-master
cd heif-master/srcs 
cmake ../srcs 
make -j$NBPROC
cd ..
cd ..

unzip tiff-4.3.0.zip
cd tiff-4.3.0
./autogen.sh
./configure --prefix="$HOME/ffmpeg_build" --disable-shared
rm VERSION
make -j$NBPROC
sudo make install
cd ..
sudo rm -r tiff-4.3.0

#Compile wxWidgets-master
#unzip wxWidgets-master.zip
#cd wxWidgets-master
#./configure --enable-unicode --enable-monolithic --disable-log --disable-debug --disable-shared --with-cxx=14
#make -j$NBPROC
#cd ..

