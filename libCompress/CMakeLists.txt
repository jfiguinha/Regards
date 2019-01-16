cmake_minimum_required(VERSION 2.8.11)

project(libCompress)



include_directories(
    /home/figuinha/dev/Regards/libCompress/
    /home/figuinha/dev/Regards/libCompress/
    /home/figuinha/dev/Regards/libCompress

)

add_definitions(
    -DNDEBUG
)

# Linker options
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS}  -s ")

# Library path
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L\".\" ")

# Define the C sources
set ( C_SRCS
    lz4.c
)

# Define the CXX sources
set ( CXX_SRCS
    jpgd.cpp
    jpge.cpp
)

set_source_files_properties(
    ${CXX_SRCS} PROPERTIES COMPILE_FLAGS 
    " -fopenmp -std=c++11 -Wall")

set_source_files_properties(
    ${C_SRCS} PROPERTIES COMPILE_FLAGS 
    " -O2 -Wall")

add_library(libCompress ${CXX_SRCS} ${C_SRCS})

