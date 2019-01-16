cmake_minimum_required(VERSION 2.8.11)

project(libGeolocalisation)



include_directories(
    /home/figuinha/dev/Regards/libGeolocalisation/
    /home/figuinha/dev/Regards/libGeolocalisation/
    /home/figuinha/dev/Regards/libSqlite
    /home/figuinha/dev/Regards/libDataStructure
    /home/figuinha/dev/Regards/libFiltreInterface
    /home/figuinha/dev/Regards/libUtility
    /home/figuinha/dev/Regards/libExif
    /home/figuinha/dev/Regards/libPicture
    /home/figuinha/dev/Regards/libSqlData
    /home/figuinha/dev/Regards/libSqlEngine
    /home/figuinha/dev/Regards/libextern/exiv2-0.24/src
    /home/figuinha/dev/Regards/libextern/wxWidgets-3.0.2/include
    /home/figuinha/dev/Regards/libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0
    /home/figuinha/dev/Regards/libResource

)

add_definitions(
    -DNDEBUG
    -D__WXGTK__
    -DEXIV2
)

# Linker options
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS}  -s ")

# Library path
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L\".\" ")

# Define the CXX sources
set ( CXX_SRCS
    FileGeolocation.cpp
    Gps.cpp
)

set_source_files_properties(
    ${CXX_SRCS} PROPERTIES COMPILE_FLAGS 
    " -fopenmp -std=c++11 -Wall")

add_library(libGeolocalisation ${CXX_SRCS} ${C_SRCS})

