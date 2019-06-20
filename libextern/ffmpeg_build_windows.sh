#!/bin/bash

# libaom
git clone https://aomedia.googlesource.com/aom
cd aom
mkdir build
cd build
cmake ../../aom -G"MSYS Makefiles" -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build"
make -j`nproc`
make install
cd ..
cd ..

export PKG_CONFIG_PATH=$HOME/ffmpeg_build/lib/pkgconfig

# ffmpeg
#git clone --depth 1 git://source.ffmpeg.org/ffmpeg
cd ffmpeg-4.1
./configure --prefix="$HOME/ffmpeg_build" --extra-cflags="-I$HOME/ffmpeg_build/include" \
  --extra-ldflags="-L$HOME/ffmpeg_build/lib" --bindir="/usr/bin" --enable-gpl --enable-nonfree --enable-libaom
make -j`nproc`
#make install
cd ..
