cmake_minimum_required(VERSION 2.8.11)

project(CxImage)



include_directories(
    /home/figuinha/dev/Regards/CxImage/
    /home/figuinha/dev/Regards/CxImage/
    /home/figuinha/dev/Regards/libextern/libmng-2.0.3
    /home/figuinha/dev/Regards/libextern/jasper-1.900.1/src/libjasper/include
    /home/figuinha/dev/Regards/Otherlib/libJbig
    /home/figuinha/dev/Regards/Otherlib/libPsd
    /home/figuinha/dev/Regards/libextern/wxWidgets-3.0.2/src/jpeg
    /home/figuinha/dev/Regards/libextern/wxWidgets-3.0.2/src/tiff/libtiff
    /home/figuinha/dev/Regards/libextern/wxWidgets-3.0.2/src/png
    /home/figuinha/dev/Regards/libextern/wxWidgets-3.0.2/include
    /home/figuinha/dev/Regards/libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0

)

add_definitions(
    -D__WXGTK__
    -D_LINUX
    -DNDEBUG
)

# Linker options
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS}  -s")

# Library path
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L\".\" ")

# Define the CXX sources
set ( CXX_SRCS
    tif_xfile.cpp
    ximabmp.cpp
    ximadsp.cpp
    ximaenc.cpp
    ximaexif.cpp
    ximage.cpp
    ximagif.cpp
    ximahist.cpp
    ximaico.cpp
    ximainfo.cpp
    ximaint.cpp
    ximajas.cpp
    ximajbg.cpp
    ximajpg.cpp
    ximalpha.cpp
    ximalyr.cpp
    ximamng.cpp
    ximapal.cpp
    ximapcx.cpp
    ximapng.cpp
    ximapsd.cpp
    ximaraw.cpp
    ximasel.cpp
    ximaska.cpp
    ximatga.cpp
    ximath.cpp
    ximatif.cpp
    ximatran.cpp
    ximawbmp.cpp
    ximawmf.cpp
    ximawnd.cpp
    xmemfile.cpp
)

set_source_files_properties(
    ${CXX_SRCS} PROPERTIES COMPILE_FLAGS 
    " -fopenmp -std=c++11 -Wall -msse4")

add_library(CxImage ${CXX_SRCS} ${C_SRCS})

