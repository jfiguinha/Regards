//********************************************************************************
//Installation Wiki Windows
//********************************************************************************

Compilation under Windows :
1 - Install MSYS 2
Download and MSYS :
http://www.msys2.org/
2 - Install Compiler :
Open MSYS2 Shell and Execute :
pacman -Syuu
3 Close the MSYS2 shell once you're asked to And Reopen MSYS2
Execute :
pacman -Syuu
4 - Install Base Developpement Tools :
pacman -S --needed base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake
pacman -S yasm
pacman -S nasm
pacman -S pango-1.0
pacman -S fftw
pacman -S openEXR
pacman -S git
pacman -S autotools
5 - Install OpenGL Extension and Lib
pacman -S freeglut
6 - Download and Install CodeLite
https://codelite.org/
7 - Download OpenCL SDK from Amd :
APP SDK :
http://developer.amd.com/amd-accelerated-parallel-processing-app-sdk/
Install it and remove all OpenGL lib and include
8 - Download Regards source code from git :
https://github.com/jfiguinha/Regards/tree/master
9 - Compilation of all libextern
Go to libextern folder

Compile exiv2-0.26 : 
Enter into exiv2-0.26 folder
Tape ./configure
And make

Compile ffmpeg-4.1
Enter into ffmpeg-4.1 folder
Tape ./configure
And make

Compile heif-master
Enter into heif-master folder
Tape cmake -G "MSYS Makefiles" ../heif-master
And make

Compile jasper-1.900.1
Enter into jasper-1.900.1 folder
Tape ./configure
And make

Compile libde265-master
Enter into libde265-master folder
Tape cmake -G "MSYS Makefiles" ../libde265-master
And make

Compile libjpeg-turbo-1.5.1
Enter into libjpeg-turbo-1.5.1 folder
Tape ./configure
And make

Compile libmng-2.0.3
Enter into libmng-2.0.3 folder
Tape ./configure
And make

Compile libwebp-0.6.0
Enter into libwebp-0.6.0 folder
Tape cmake -G "MSYS Makefiles" ../libwebp-0.6.0
And make

Compile MediaInfo_DLL_GNU_FromSource
Enter MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/GNU Folder
Tape ./configure
And make

Compile SDL2-2.0.5
Enter into SDL2-2.0.5 folder
Tape ./configure
And make

Compile wxWidgets-master
Enter into wxWidgets-master folder
./configure --with-libjpeg=builtin --with-libpng=builtin --with-libtiff=builtin --enable-monolithic --enable-unicode --disable-shared --disable-log --disable-debug
And make

Compile x265_2.5
Enter into x265_2.5/build/msys folder
Tape ./multilib.sh

Compile libexif
Enter into libexif
Tape ./configure

10 - Compilation of Regards
On the root of the source code Folder, open Regards.wokspace under CodeLite
Compile under RegardsViewer_exe RegardsViewer_Windows Project

Installation Wiki Mac OS
1 - Install and open Xcode
2 - Install Xcode extension
3 - Install Homebrew
https://brew.sh/index_fr.html

4 - Install and Update package
Open a Terminal :
brew update
brew upgrade
brew install llvm
brew install pango
brew install git
brew install libexif
brew install fftw --with-openmp --without-fortran

5 - Download and Install CodeLite
https://codelite.org/
Open CodeLite
Start Wizard to find C++ compiler

6 - Download Regards and Libextern source code :
git clone https://github.com/jfiguinha/Regards/tree/master

7 - Compilation of all libextern
Open a Terminal and Go to libextern folder

Compile libaom-master
Enter into libaom-master/libaom folder
Tape cmake path/to/aom
And make
and make install

Compile exiv2-0.26 : 
Enter into exiv2-0.26 folder
Tape ./configure
And make

Compile ffmpeg-4.1
Enter into ffmpeg-4.1 folder
Tape ./configure --enable-libaom
And make

Compile heif-master
Enter into heif-master folder
Tape cmake ../heif-master
And make

Compile jasper-1.900.1
Enter into jasper-1.900.1 folder
Tape ./configure
And make

Compile libde265-master
Enter into libde265-master folder
Tape cmake ../libde265-master
And make

Compile libjpeg-turbo-1.5.1
Enter into libjpeg-turbo-1.5.1 folder
Tape ./configure
And make

Compile libmng-2.0.3
Enter into libmng-2.0.3 folder
Tape ./configure
And make

Compile libwebp-0.6.0
Enter into libwebp-0.6.0 folder
Tape cmake ../libwebp-0.6.0
And make

Compile MediaInfo_DLL_GNU_FromSource
Enter MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/GNU Folder
Tape ./configure
And make

Compile SDL2-2.0.5
Enter into SDL2-2.0.5 folder
Tape ./configure
And make

Compile wxWidgets-master
Enter into wxWidgets-master folder
./configure --with-libjpeg=builtin --with-libpng=builtin --with-libtiff=builtin --enable-monolithic --enable-unicode --disable-shared --disable-log --disable-debug --with-cxx=11
And make

Compile x265_2.5
Enter into x265_2.5/source folder
Tape cmake ../source
And make

8 - Open Regards Project

On the root of the source code Folder, open Regards.wokspace under CodeLite
Set llvm as default compiler for MinGW 64 bit Compilers
Change option for this compiler under Tools Panel
Add -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk/
for c and c++ compiler tools at the end of the line
In Advanced Panel :
Include Path : 
/usr/local/llvm/include/c++/v1;/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include;/usr/local/opt/cairo/include;/usr/local/llvm/include;/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/libxml2
Librairies Path :
/usr/local/llvm/lib;/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/lib;/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library

9 - Compilation of Regards
Compile under RegardsViewer_exe RegardsViewer_Mac Project


Installation Wiki Linux Mint

1 - Open a terminal
Run this command to be up to date
sudo apt-get update
sudo apt-get upgrade
2 - Install Compiler and library tools :
sudo apt-get install gcc
sudo apt-get install g++
sudo apt-get install build-essential
sudo apt-get install cmake
sudo apt-get install binutils-gold
sudo apt-get install yasm
sudo apt-get install nasm
sudo apt-get install libpango-1.0
sudo apt-get install libwebkit-dev
sudo apt-get install freeglut3
sudo apt-get install freeglut3-dev
sudo apt-get install mesa-common-dev
sudo apt-get install git
sudo apt-get install libturbojpeg-dev
sudo apt-get install libSDL2-dev
sudo apt-get install libglew-dev
sudo apt-get install libopenexr-dev
sudo apt-get install libfftw3-dev

3 - Download and Install CodeLite
https://codelite.org/

4 - Download OpenCL SDK from Amd :
APP SDK :
http://developer.amd.com/amd-accelerated-parallel-processing-app-sdk/

5 - Download Regards from github :
Source Code :
git clone https://github.com/jfiguinha/Regards/tree/master

6 - Compilation of all libextern
Go to libextern folder

Compile libaom-master
Enter into libaom-master/libaom folder
Tape cmake path/to/aom
And make
and make install

Compile exiv2-0.26 : 
Enter into exiv2-0.26 folder
Tape ./configure
And make

Compile fftw-3.3.8 : 
Enter into fftw-3.3.8 folder
Tape ./configure
And make

Compile ffmpeg-4.1
Enter into ffmpeg-4.1 folder
Tape ./configure --enable-libaom
And make

Compile heif-master
Enter into heif-master folder
Tape cmake ../heif-master
And make

Compile jasper-1.900.1
Enter into jasper-1.900.1 folder
Tape ./configure
And make

Compile libde265-master
Enter into libde265-master folder
Tape cmake ../libde265-master
And make

Compile libmng-2.0.3
Enter into libmng-2.0.3 folder
Tape ./configure
And make

Compile libwebp-0.6.0
Enter into libwebp-0.6.0 folder
Tape cmake ../libwebp-0.6.0
And make

Compile MediaInfo_DLL_GNU_FromSource
Enter MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/GNU Folder
Tape ./configure
And make

Compile wxWidgets-master
Enter into wxWidgets-master folder
./configure --with-libjpeg=builtin --with-libpng=builtin --with-libtiff=builtin --enable-monolithic --enable-unicode --disable-shared --disable-log --disable-debug --with-gtk=3
And make

Compile x265_2.5
Enter into x265_2.5/build/linux folder
Tape ./multilib.sh

7 - Open Regards Project

On the root of the source code Folder, open Regards.wokspace under CodeLite
Set gcc as default compiler for MinGW 64 bit Compilers
Change option for this compiler under advanced Panel
Include Path : 
/usr/local/include;/usr/include/libxml2;/usr/include/pango-1.0;/usr/include/glib-2.0;/usr/include/cairo;/opt/AMDAPPSDK-3.0/opencl/include;/usr/lib/x86_64-linux-gnu/glib-2.0/include

8 - Compilation of Regards

On the root of the source code Folder, open Regards.wokspace under CodeLite
Compile under RegardsViewer_exe RegardsViewer_linux Project
