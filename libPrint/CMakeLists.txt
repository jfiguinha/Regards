cmake_minimum_required(VERSION 2.8.11)

project(libPrint)



include_directories(
    /home/figuinha/dev/Regards/libPrint/
    /home/figuinha/dev/Regards/libPrint/
    /home/figuinha/dev/Regards/libSqlite
    /home/figuinha/dev/Regards/libDataStructure
    /home/figuinha/dev/Regards/libFiltreInterface
    /home/figuinha/dev/Regards/libextern/libRapidXML
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
    PrintEngine.cpp
)

set_source_files_properties(
    ${CXX_SRCS} PROPERTIES COMPILE_FLAGS 
    " -fopenmp -std=c++11 -Wall")

add_library(libPrint ${CXX_SRCS} ${C_SRCS})

