cmake_minimum_required(VERSION 2.8.11)

project(libSqlData)



include_directories(
    /home/figuinha/dev/Regards/libSqlData/
    /home/figuinha/dev/Regards/libSqlData/
    /home/figuinha/dev/Regards/libSqlite
    /home/figuinha/dev/Regards/libSqlEngine
    /home/figuinha/dev/Regards/libDataStructure
    /home/figuinha/dev/Regards/libResource
    /home/figuinha/dev/Regards/libPicture
    /home/figuinha/dev/Regards/libCompress
    /home/figuinha/dev/Regards/libUtility
    /home/figuinha/dev/Regards/libFiltre
    /home/figuinha/dev/Regards/libFiltreInterface
    /home/figuinha/dev/Regards/include
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
    SqlCatalog.cpp
    SqlCountry.cpp
    SqlCriteria.cpp
    SqlFindCatalog.cpp
    SqlFindCriteria.cpp
    SqlFindDay.cpp
    SqlFindFolderCatalog.cpp
    SqlFindLocalisation.cpp
    SqlFindMonth.cpp
    SqlFindPhotos.cpp
    SqlFindYear.cpp
    SqlFolderCatalog.cpp
    SqlInsertFile.cpp
    SqlLibExplorer.cpp
    SqlPhotoCategorie.cpp
    SqlPhotoCriteria.cpp
    SqlPhotos.cpp
    SQLRemoveData.cpp
    SqlThumbnail.cpp
    ThumbnailDataSQL.cpp
    SqlGps.cpp
    SqlVersion.cpp
)

set_source_files_properties(
    ${CXX_SRCS} PROPERTIES COMPILE_FLAGS 
    " -fopenmp -std=c++11 -Wall")

add_library(libSqlData ${CXX_SRCS} ${C_SRCS})

