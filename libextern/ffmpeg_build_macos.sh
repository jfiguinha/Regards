#!/bin/bash
NBPROC=$(sysctl -n hw.ncpu)
echo $NBPROC

export PKG_CONFIG_PATH=$HOME/ffmpeg_build/lib/pkgconfig

tar xf ffmpeg-4.2.2.tar.xz

# ffmpeg
#git clone --depth 1 git://source.ffmpeg.org/ffmpeg
cd ffmpeg-4.2.2
./configure --prefix="$HOME/ffmpeg_build" --extra-cflags="-I$HOME/ffmpeg_build/include" \
  --extra-ldflags="-L$HOME/ffmpeg_build/lib" --bindir="/usr/bin" --enable-gpl --enable-nonfree --enable-libaom
make -j$NBPROC
#make install
cd ..
