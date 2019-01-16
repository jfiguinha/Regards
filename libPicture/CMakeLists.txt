cmake_minimum_required(VERSION 2.8.11)

project(libPicture)



include_directories(
    /home/figuinha/dev/Regards/libPicture/
    /home/figuinha/dev/Regards/libPicture/
    /home/figuinha/dev/Regards/libSqlite
    /home/figuinha/dev/Regards/libDataStructure
    /home/figuinha/dev/Regards/libFiltreInterface
    /home/figuinha/dev/Regards/libOpenCL
    /home/figuinha/dev/Regards/libResource
    /home/figuinha/dev/Regards/include
    /home/figuinha/dev/Regards/Otherlib/RegardsRaw
    /home/figuinha/dev/Regards/CxImage
    /home/figuinha/dev/Regards/libFiltre
    /home/figuinha/dev/Regards/Otherlib/libRegardsBpg
    /home/figuinha/dev/Regards/libUtility
    /home/figuinha/dev/Regards/libVideoThumbnail
    /home/figuinha/dev/Regards/libExif
    /home/figuinha/dev/Regards/libDialog
    /home/figuinha/dev/Regards/Otherlib/wxSVG/include
    /home/figuinha/dev/Regards/Otherlib/RegardsBpg
    /home/figuinha/dev/Regards/libextern/wxWidgets-3.0.2/include
    /home/figuinha/dev/Regards/libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0

)

add_definitions(
    -DNDEBUG
    -D__WXGTK__
    -D_LINUX
    -DLIBRAW
    -DEXIV2
    -DFFMPEG
)

# Linker options
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS}  -s ")

# Library path
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L\".\" ")

# Define the CXX sources
set ( CXX_SRCS
    CPCD.cpp
    libPicture.cpp
    ScaleThumbnail.cpp
)

set_source_files_properties(
    ${CXX_SRCS} PROPERTIES COMPILE_FLAGS 
    " -fopenmp -std=c++11 -Wall")

add_library(libPicture ${CXX_SRCS} ${C_SRCS})

