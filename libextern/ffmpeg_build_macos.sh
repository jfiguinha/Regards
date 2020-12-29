#!/bin/bash
NBPROC=$(sysctl -n hw.ncpu)
echo $NBPROC

brew install aom fdk-aac libvorbis libvpx opus sdl2 theora x264 x265 xvid nasm

tar xf ffmpeg-4.3.1.tar.xz

export PKG_CONFIG_PATH=$HOME/ffmpeg_build/lib/pkgconfig

# ffmpeg
#git clone --depth 1 git://source.ffmpeg.org/ffmpeg
mv ffmpeg-4.3.1 ffmpeg-master
cd ffmpeg-master
./configure --prefix="$HOME/ffmpeg_build" --extra-cflags="-I$HOME/ffmpeg_build/include" --extra-ldflags="-L$HOME/ffmpeg_build/lib" --bindir="$HOME/ffmpeg_build/bin" --enable-gpl --enable-nonfree --enable-libaom --enable-libfdk-aac --enable-libmp3lame --enable-libtheora --enable-libvorbis --enable-libvpx --enable-libx264 --enable-libx265 --enable-libopus --enable-libxvid --enable-libdav1d
make -j$NBPROC
#make install

rm VERSION

cd ..
