cmake_minimum_required(VERSION 2.8.11)

project(libVideoThumbnail)



include_directories(
    /home/figuinha/dev/Regards/libVideoThumbnail/
    /home/figuinha/dev/Regards/libVideoThumbnail/
    /home/figuinha/dev/Regards/CxImage
    /home/figuinha/dev/Regards/libextern/ffmpeg-2.6.2
    /home/figuinha/dev/Regards/libDataStructure
    /home/figuinha/dev/Regards/libextern/wxWidgets-3.0.2/include
    /home/figuinha/dev/Regards/libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0

)

add_definitions(
    -DNDEBUG
    -DLATEST_GREATEST_FFMPEG
    -D__WXGTK__
)

# Linker options
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS}  -s")

# Library path
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L\".\" ")

# Define the CXX sources
set ( CXX_SRCS
    cximagewriter.cpp
    filmstripfilter.cpp
    Metadata.cpp
    moviedecoder.cpp
    stringoperations.cpp
    videothumbnailer.cpp
    videothumbnailerc.cpp
    videothumb.cpp
)

set_source_files_properties(
    ${CXX_SRCS} PROPERTIES COMPILE_FLAGS 
    " -fopenmp -std=c++11 -Wall")

add_library(libVideoThumbnail ${CXX_SRCS} ${C_SRCS})

