cmake_minimum_required(VERSION 2.8.11)

project(libDataStructure)



include_directories(
    /home/figuinha/dev/Regards/libDataStructure/
    /home/figuinha/dev/Regards/libDataStructure/
    /home/figuinha/dev/Regards/libSqlite
    /home/figuinha/dev/Regards/libUtility
    /home/figuinha/dev/Regards/libResource
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
    Catalog.cpp
    Country.cpp
    Criteria.cpp
    FolderCatalog.cpp
    GeoPluginValue.cpp
    PhotoCategorie.cpp
    PhotoCriteria.cpp
    Photos.cpp
    RegardsBitmap.cpp
    RGBAQuad.cpp
    ThumbnailData.cpp
    ThumbnailDataStorage.cpp
    TreeData.cpp
    TreeDataEffect.cpp
    videothumbnail.cpp
    TreeDataLink.cpp
    PhotoGps.cpp
    TreeDataCategory.cpp
)

set_source_files_properties(
    ${CXX_SRCS} PROPERTIES COMPILE_FLAGS 
    " -fopenmp -std=c++11 -Wall")

add_library(libDataStructure ${CXX_SRCS} ${C_SRCS})

