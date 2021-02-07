#!/bin/bash
NBPROC=$(sysctl -n hw.ncpu)
echo $NBPROC

#decompression
tar xf  fftw-3.3.8.tar.gz

#Compile exiv2-0.27.3 :
cd exiv2-0.27.3
mkdir build
cd build 
cmake ../
make -j$NBPROC
cd ..
cd ..


#Compile heif-master
cd heif-master/srcs 
cmake ../srcs 
make -j$NBPROC
cd ..
cd ..


#Compile wxWidgets-master
unzip wxWidgets-master.zip
cd wxWidgets-master
./configure --enable-unicode --enable-monolithic --disable-log --disable-debug --disable-shared --with-cxx=14
make -j$NBPROC
cd ..

cd fftw-3.3.8
./configure --enable-float
make -j$NBPROC
cd ..

<<<<<<< HEAD
=======

>>>>>>> b36816eed99bc9797e34373a8f97026d28bbb79e
export PKG_CONFIG_PATH=$HOME/ffmpeg_build/lib/pkgconfig

# ffmpeg
tar xf ffmpeg-4.3.1.tar.xz
mv ffmpeg-4.3.1 ffmpeg-master
cd ffmpeg-master
./configure --prefix="$HOME/ffmpeg_build" --extra-cflags="-I$HOME/ffmpeg_build/include -I/opt/homebrew/include" --extra-ldflags="-L$HOME/ffmpeg_build/lib -L/opt/homebrew/lib" --bindir="$HOME/ffmpeg_build/bin" --enable-gpl --enable-nonfree --enable-libaom --enable-libfdk-aac --enable-libmp3lame --enable-libtheora --enable-libvorbis --enable-libvpx --enable-libx264 --enable-libx265 --enable-libopus --enable-libxvid --enable-libdav1d --enable-shared
make -j$NBPROC
sudo make install 
<<<<<<< HEAD
sudo rm -r $HOME/ffmpeg_build/include/libavcodec
sudo rm -r $HOME/ffmpeg_build/include/libavdevice
sudo rm -r $HOME/ffmpeg_build/include/libavfilter
sudo rm -r $HOME/ffmpeg_build/include/libavformat
sudo rm -r $HOME/ffmpeg_build/include/libavutil
sudo rm -r $HOME/ffmpeg_build/include/libpostproc
sudo rm -r $HOME/ffmpeg_build/include/libswresample
sudo rm -r $HOME/ffmpeg_build/include/libswscale
=======
>>>>>>> b36816eed99bc9797e34373a8f97026d28bbb79e
rm VERSION

cd ..

wget https://github.com/opencv/opencv_contrib/archive/4.5.1.zip
mv 4.5.1.zip opencv/opencv_contrib-4.5.1.zip

wget https://github.com/opencv/opencv/archive/4.5.1.zip
mv 4.5.1.zip opencv/opencv-4.5.1.zip

#compile opencv
cd opencv
unzip opencv-4.5.1.zip
unzip opencv_contrib-4.5.1.zip
cd opencv-4.5.1
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX:PATH="$HOME/ffmpeg_build" -DBUILD_opencv_python=OFF -DOPENCV_EXTRA_MODULES_PATH:PATH="../../opencv_contrib-4.5.1/modules" -DBUILD_opencv_freetype=OFF -DCMAKE_BUILD_TYPE=RELEASE -DBUILD_PERF_TESTS=OFF -DBUILD_TESTS=OFF -DBUILD_opencv_apps=OFF -DBUILD_EXAMPLES=OFF -DCMAKE_CXX_FLAGS="-std=gnu++14 -I ../../../eigen/Eigen" -DOPENCV_ALLOCATOR_STATS_COUNTER_TYPE=int64_t ../
make -j$NBPROC
sudo make install
cd ..
cd ..
cd ..

<<<<<<< HEAD

=======
>>>>>>> b36816eed99bc9797e34373a8f97026d28bbb79e
