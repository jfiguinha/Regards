cmake_minimum_required(VERSION 2.8.11)

project(libWindow)



include_directories(
    /home/figuinha/dev/Regards/libWindow/
    /home/figuinha/dev/Regards/libWindow/
    /home/figuinha/dev/Regards/libSqlite
    /home/figuinha/dev/Regards/libTheme
    /home/figuinha/dev/Regards/libextern/glew-1.13.0/include
    /home/figuinha/dev/Regards/libextern/libRapidXML
    /home/figuinha/dev/Regards/libextern/ffmpeg-2.6.2
    /home/figuinha/dev/Regards/libextern/SDL2-2.0.3/include
    /home/figuinha/dev/Regards/libGeolocalisation
    /home/figuinha/dev/Regards/libParameter
    /home/figuinha/dev/Regards/libextern/exiv2-0.24/src
    /home/figuinha/dev/Regards/libextern/exiv2-0.24/xmpsdk/include
    /home/figuinha/dev/Regards/libUtility
    /home/figuinha/dev/Regards/libFiltreInterface
    /home/figuinha/dev/Regards/include
    /home/figuinha/dev/Regards/libDataStructure
    /home/figuinha/dev/Regards/libResource
    /home/figuinha/dev/Regards/libSqlData
    /home/figuinha/dev/Regards/libOpenGL
    /home/figuinha/dev/Regards/libRenderBitmap
    /home/figuinha/dev/Regards/libSqlEngine
    /home/figuinha/dev/Regards/libSqlite
    /home/figuinha/dev/Regards/libFiltre
    /home/figuinha/dev/Regards/include
    /home/figuinha/dev/Regards/libDataStructure
    /home/figuinha/dev/Regards/Otherlib/wxSVG/include
    /home/figuinha/dev/Regards/libPicture
    /home/figuinha/dev/Regards/libWindowMain
    /home/figuinha/dev/Regards/libextern/wxWidgets-3.0.2/include
    /home/figuinha/dev/Regards/libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0
    /home/figuinha/dev/Regards/libOpenCL

)

add_definitions(
    -DNDEBUG
    -D__WXGTK__
    -DGLEW_MX
)

# Linker options
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS}  -s")

# Library path
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L\".\" ")

# Define the CXX sources
set ( CXX_SRCS
    BitmapWindowContext.cpp
    BitmapWnd.cpp
    EmptyWindow.cpp
    Icone.cpp
    InfosSeparationBar.cpp
    Pane.cpp
    PositionElement.cpp
    ScrollbarHorizontalWnd.cpp
    ScrollbarVerticalWnd.cpp
    ScrollbarWnd.cpp
    ScrollInterface.cpp
    SeparationBar.cpp
    Slider.cpp
    Splitter.cpp
    Thumbnail.cpp
    TitleBar.cpp
    ToolbarButton.cpp
    ToolbarElement.cpp
    ToolbarSlide.cpp
    ToolbarTexte.cpp
    ToolbarWindow.cpp
    TreeControl.cpp
    TreeElement.cpp
    TreeElementCheckBox.cpp
    TreeElementDelete.cpp
    TreeElementSlide.cpp
    TreeElementTexte.cpp
    TreeElementTriangle.cpp
    TreeWindow.cpp
    TreeElementTexteClick.cpp
    BitmapInfos.cpp
    BitmapInfosCheckBox.cpp
    BitmapInfosGps.cpp
    CalendarDialogBox.cpp
    CalendarSelect.cpp
    MapDialogBox.cpp
    MapSelect.cpp
)

set_source_files_properties(
    ${CXX_SRCS} PROPERTIES COMPILE_FLAGS 
    " -Os -fopenmp -std=c++11 -Wall")

add_library(libWindow ${CXX_SRCS} ${C_SRCS})

