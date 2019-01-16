cmake_minimum_required(VERSION 2.8.11)

project(libSqlite)



include_directories(
    /home/figuinha/dev/Regards/libSqlite/
    /home/figuinha/dev/Regards/libSqlite/

)

add_definitions(
    -DNDEBUG
    -DSQLITE_THREADSAFE=1
)

# Linker options
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS}  -s ")

# Library path
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L\".\" ")

# Define the C sources
set ( C_SRCS
    sqlite3.c
)

set_source_files_properties(
    ${C_SRCS} PROPERTIES COMPILE_FLAGS 
    " -O3 -Wall")

add_library(libSqlite ${CXX_SRCS} ${C_SRCS})

