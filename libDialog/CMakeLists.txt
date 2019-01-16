cmake_minimum_required(VERSION 2.8.11)

project(libDialog)



include_directories(
    /home/figuinha/dev/Regards/libDialog/
    /home/figuinha/dev/Regards/libDialog/
    /home/figuinha/dev/Regards/libSqlite
    /home/figuinha/dev/Regards/libUtility
    /home/figuinha/dev/Regards/libResource
    /home/figuinha/dev/Regards/libDataStructure
    /home/figuinha/dev/Regards/libOpenCL
    /home/figuinha/dev/Regards/libPicture
    /home/figuinha/dev/Regards/libParameter
    /home/figuinha/dev/Regards/libextern/libRapidXML
    /home/figuinha/dev/Regards/include
    /home/figuinha/dev/Regards/libSqlData
    /home/figuinha/dev/Regards/libSqlEngine
    /home/figuinha/dev/Regards/libSqlite
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
    CompressionOption.cpp
    ConfigRegards.cpp
    CopyFileDlg.cpp
    ExportFile.cpp
    GifOption.cpp
    JpegOption.cpp
    PngOption.cpp
    SaveFileFormat.cpp
    TiffOption.cpp
)

set_source_files_properties(
    ${CXX_SRCS} PROPERTIES COMPILE_FLAGS 
    " -fopenmp -std=c++11 -Wall")

add_library(libDialog ${CXX_SRCS} ${C_SRCS})

