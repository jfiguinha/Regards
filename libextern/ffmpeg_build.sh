#!/bin/bash
NBPROC=$(nproc)
echo $NBPROC

sudo apt-get update
sudo apt-get -y install autoconf automake build-essential git libass-dev libfaac-dev libgpac-dev libmp3lame-dev libopus-dev libtheora-dev libtool libvorbis-dev libvpx-dev pkg-config texi2html yasm zlib1g-dev

# x264
git clone --depth 1 git://git.videolan.org/x264.git
cd x264
./configure --prefix="$HOME/ffmpeg_build" --bindir="/usr/bin" --enable-static
make -j$NBPROC
sudo make install
cd ..

# fdk-aac
git clone --depth 1 git://github.com/mstorsjo/fdk-aac.git
cd fdk-aac
autoreconf -fiv
./configure --disable-shared --prefix="$HOME/ffmpeg_build"
make -j$NBPROC
sudo make install
cd ..

# libaom
git clone https://aomedia.googlesource.com/aom
cd aom
mkdir build
cd build
cmake ../../aom -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build"
make -j$NBPROC
sudo make install
cd ..
cd ..

export PKG_CONFIG_PATH=$HOME/ffmpeg_build/lib/pkgconfig

# ffmpeg
#git clone --depth 1 git://source.ffmpeg.org/ffmpeg
cd ffmpeg-4.1
./configure --prefix="$HOME/ffmpeg_build" --extra-cflags="-I$HOME/ffmpeg_build/include" \
  --extra-ldflags="-L$HOME/ffmpeg_build/lib" --bindir="/usr/bin" --enable-gpl --enable-libass \
  --enable-libfdk-aac --enable-libmp3lame --enable-libopus --enable-libtheora \
  --enable-libvorbis --enable-libvpx --enable-libx264 --enable-nonfree --enable-libaom
make -j$NBPROC
#make install
cd ..
