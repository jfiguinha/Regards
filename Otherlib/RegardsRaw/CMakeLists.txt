cmake_minimum_required(VERSION 2.8.11)

project(RegardsRaw)



include_directories(
    /home/figuinha/dev/Regards/Otherlib/RegardsRaw/
    /home/figuinha/dev/Regards/Otherlib/RegardsRaw/
    /home/figuinha/dev/Regards/libextern/LibRaw-0.17.0/
    /home/figuinha/dev/Regards/libextern/LibRaw-0.17.0/libraw
    /home/figuinha/dev/Regards/libDataStructure
    /home/figuinha/dev/Regards/libUtility
    /home/figuinha/dev/Regards/CxImage
    /home/figuinha/dev/Regards/libextern/wxWidgets-3.0.2/include
    /home/figuinha/dev/Regards/libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0

)

add_definitions(
    -DNDEBUG
    -DLIBRAW_LIBRARY_BUILD
    -DLIBRAW
    -D__WXGTK__
)

# Linker options
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS}  -O2")

# Library path
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L\".\" ")

# Define the CXX sources
set ( CXX_SRCS
    RegardsRaw.cpp
    raw.cpp
)

set_source_files_properties(
    ${CXX_SRCS} PROPERTIES COMPILE_FLAGS 
    " -O3 -fopenmp -std=c++11 -Wall")

add_library(RegardsRaw ${CXX_SRCS} ${C_SRCS})

