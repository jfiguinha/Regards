cmake_minimum_required(VERSION 2.8.11)

project(libEmail)



include_directories(
    /home/figuinha/dev/Regards/libEmail/
    /home/figuinha/dev/Regards/libEmail/
    /home/figuinha/dev/Regards/libextern/wxWidgets-3.0.2/include
    /home/figuinha/dev/Regards/libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0

)

add_definitions(
    -DNDEBUG
    -D__WXGTK__
)

# Linker options
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS}  -s")

# Library path
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L\".\" ")

# Define the CXX sources
set ( CXX_SRCS
    email.cpp
    mime.cpp
)

set_source_files_properties(
    ${CXX_SRCS} PROPERTIES COMPILE_FLAGS 
    " -fopenmp -std=c++11 -Wall")

add_library(libEmail ${CXX_SRCS} ${C_SRCS})

