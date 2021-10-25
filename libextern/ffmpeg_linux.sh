#Compile dav1d-0.9.2.zip
wget https://code.videolan.org/videolan/dav1d/-/archive/0.9.2/dav1d-0.9.2.zip
unzip dav1d-0.9.2.zip
cd dav1d-0.9.2
mkdir build
cd build
meson setup -Denable_tools=false -Denable_tests=false --default-library=static .. --prefix "$HOME/ffmpeg_build" --libdir="$HOME/ffmpeg_build/lib"
ninja
sudo ninja install
cd ..
cd ..

# libaom
git clone https://aomedia.googlesource.com/aom
cd aom
cd build
cmake ../../aom -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build"
make -j$NBPROC
sudo make install
cd ..
cd ..

#compile x265
wget https://github.com/videolan/x265/archive/refs/tags/3.4.zip
unzip 3.4.zip
cd x265-3.4/source
mkdir build
cd build
cmake ../  -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build"
make -j$NBPROC
sudo make install
cd .. 
cd .. 
cd .. 

#compile x264
git clone --depth 1 https://code.videolan.org/videolan/x264.git
cd x264
export PKG_CONFIG_PATH=$HOME/ffmpeg_build/lib/pkgconfig
./configure --prefix="$HOME/ffmpeg_build" --bindir="$HOME/ffmpeg_build/bin" --enable-pic --enable-static
make -j$NBPROC
sudo make install
cd ..

#compile vp8 - vp9
git clone --depth 1 https://chromium.googlesource.com/webm/libvpx.git
cd libvpx
export PKG_CONFIG_PATH=$HOME/ffmpeg_build/lib/pkgconfig
./configure --prefix="$HOME/ffmpeg_build" --disable-examples --disable-unit-tests --enable-vp9-highbitdepth --as=yasm
make -j$NBPROC
sudo make install
cd ..

#compile libfdk-aac
git clone --depth 1 https://github.com/mstorsjo/fdk-aac
cd fdk-aac
autoreconf -fiv
./configure --prefix="$HOME/ffmpeg_build" --disable-shared
make -j$NBPROC
sudo make install
cd ..

#compile libmp3lame
wget -O lame-3.100.tar.gz https://downloads.sourceforge.net/project/lame/lame/3.100/lame-3.100.tar.gz
tar xzvf lame-3.100.tar.gz
cd lame-3.100
./configure --prefix="$HOME/ffmpeg_build" --bindir="$HOME/ffmpeg_build/bin" --disable-shared --enable-nasm
make -j$NBPROC
sudo make install
cd ..

wget https://ffmpeg.org/releases/ffmpeg-4.4.1.tar.xz
tar xf ffmpeg-4.4.tar.xz
mv ffmpeg-4.4 ffmpeg-master
cd ffmpeg-master
export PKG_CONFIG_PATH=$HOME/ffmpeg_build/lib/pkgconfig
./configure \
  --prefix="$HOME/ffmpeg_build" \
  --pkg-config-flags="--static" \
  --extra-cflags="-I$HOME/ffmpeg_build/include" \
  --extra-ldflags="-L$HOME/ffmpeg_build/lib" \
  --extra-libs="-lpthread -lm" \
  --bindir="$HOME/ffmpeg_build/bin" \
  --enable-gpl \
  --enable-gnutls \
  --enable-libaom \
  --enable-libfdk-aac \
  --enable-libfreetype \
  --enable-libmp3lame \
  --enable-libvpx \
  --enable-libx264 \
  --enable-libx265 \
  --disable-indevs \
  --enable-libdav1d \
  --enable-nonfree
make -j$NBPROC
sudo make install
cd ..
