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


