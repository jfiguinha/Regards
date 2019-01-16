cmake_minimum_required(VERSION 2.8.11)

project(libVideoFFMpeg)



include_directories(
    /home/figuinha/dev/Regards/libVideoFFMpeg/
    /home/figuinha/dev/Regards/libVideoFFMpeg/
    /home/figuinha/dev/Regards/libTheme
    /home/figuinha/dev/Regards/libextern/libRapidXML
    /home/figuinha/dev/Regards/libextern/ffmpeg-2.6.2
    /home/figuinha/dev/Regards/libextern/SDL2-2.0.3/include
    /home/figuinha/dev/Regards/libextern/glew-1.13.0/include
    /home/figuinha/dev/Regards/libWindowMain
    /home/figuinha/dev/Regards/libDataStructure
    /home/figuinha/dev/Regards/libResource
    /home/figuinha/dev/Regards/libOpenGL
    /home/figuinha/dev/Regards/libParameter
    /home/figuinha/dev/Regards/libFiltre
    /home/figuinha/dev/Regards/libUtility
    /home/figuinha/dev/Regards/include
    /home/figuinha/dev/Regards/libFiltreInterface
    /home/figuinha/dev/Regards/Otherlib/wxSVG/include
    /home/figuinha/dev/Regards/libextern/wxWidgets-3.0.2/include
    /home/figuinha/dev/Regards/libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0
    /home/figuinha/dev/Regards/libOpenCL

)

add_definitions(
    -DNDEBUG
    -D__WXGTK__
    -DSDL2
    -DGLEW_MX
)

# Linker options
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS}  -s ")

# Library path
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L\".\" ")

# Define the CXX sources
set ( CXX_SRCS
    cmdutils.cpp
    ffplaycore.cpp
    ShowVideo.cpp
    SliderVideo.cpp
    VideoControl.cpp
    VideoWindowContext.cpp
)

set_source_files_properties(
    ${CXX_SRCS} PROPERTIES COMPILE_FLAGS 
    " -fopenmp -std=c++11 -Wall")

add_library(libVideoFFMpeg ${CXX_SRCS} ${C_SRCS})

