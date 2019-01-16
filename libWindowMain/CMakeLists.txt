cmake_minimum_required(VERSION 2.8.11)

project(libWindowMain)



include_directories(
    /home/figuinha/dev/Regards/libWindowMain/
    /home/figuinha/dev/Regards/libWindowMain/
    /home/figuinha/dev/Regards/libTheme
    /home/figuinha/dev/Regards/libextern/libRapidXML
    /home/figuinha/dev/Regards/libextern/wxWidgets-3.0.2/include
    /home/figuinha/dev/Regards/libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0

)

add_definitions(
    -DNDEBUG
    -D__WXGTK__
)

# Linker options
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS}  -s ")

# Library path
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L\".\" ")

# Define the CXX sources
set ( CXX_SRCS
    WindowUtility.cpp
)

set_source_files_properties(
    ${CXX_SRCS} PROPERTIES COMPILE_FLAGS 
    " -fopenmp -std=c++11 -Wall")

add_library(libWindowMain ${CXX_SRCS} ${C_SRCS})

