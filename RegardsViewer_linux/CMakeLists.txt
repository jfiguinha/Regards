cmake_minimum_required(VERSION 2.8.11)

project(RegardsViewer)

# Add the build order directories
add_subdirectory( ../CxImage )
add_subdirectory( ../Otherlib/RegardsRaw )
add_subdirectory( ../libCompress )
add_subdirectory( ../libControl )
add_subdirectory( ../libWindowMain )
add_subdirectory( ../libWindow )
add_subdirectory( ../libViewer )
add_subdirectory( ../libVideoThumbnail )
add_subdirectory( ../libVideoFFMpeg )
add_subdirectory( ../libUtility )
add_subdirectory( ../libSqlEngine )
add_subdirectory( ../libSqlite )
add_subdirectory( ../libTheme )
add_subdirectory( ../libSqlData )
add_subdirectory( ../libResource )
add_subdirectory( ../libRenderBitmap )
add_subdirectory( ../libPrint )
add_subdirectory( ../libPicture )
add_subdirectory( ../libOpenCL )
add_subdirectory( ../libOpenGL )
add_subdirectory( ../libParameter )
add_subdirectory( ../libIntro )
add_subdirectory( ../libGeolocalisation )
add_subdirectory( ../libFiltre )
add_subdirectory( ../libExif )
add_subdirectory( ../libEmail )
add_subdirectory( ../libDialog )
add_subdirectory( ../libDataStructure )


include_directories(
    /home/figuinha/dev/Regards/RegardsViewer/
    /home/figuinha/dev/Regards/RegardsViewer/
    /home/figuinha/dev/Regards/libSqlite
    /home/figuinha/dev/Regards/libTheme
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
    /home/figuinha/dev/Regards/libextern/glew-1.13.0/include
    /home/figuinha/dev/Regards/libFiltre
    /home/figuinha/dev/Regards/include
    /home/figuinha/dev/Regards/libDataStructure
    /home/figuinha/dev/Regards/Otherlib/wxSVG/include
    /home/figuinha/dev/Regards/libWindow
    /home/figuinha/dev/Regards/libPrint
    /home/figuinha/dev/Regards/libPicture
    /home/figuinha/dev/Regards/libTheme
    /home/figuinha/dev/Regards/libGeolocalisation
    /home/figuinha/dev/Regards/libControl
    /home/figuinha/dev/Regards/libIntro
    /home/figuinha/dev/Regards/libDialog
    /home/figuinha/dev/Regards/libViewer
    /home/figuinha/dev/Regards/libExplorer
    /home/figuinha/dev/Regards/libOpenCL
    /home/figuinha/dev/Regards/libWindowMain
    /home/figuinha/dev/Regards/libVideoFFMpeg
    /home/figuinha/dev/Regards/libextern/wxWidgets-3.0.2/include
    /home/figuinha/dev/Regards/libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0

)

add_definitions(
    -DNDEBUG
    -DVIEWER
    -D__WXGTK__
    -DGLEW_MX
    -DGLEW_STATIC
)

# Linker options
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS}  -s ")

# Library path
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L\".\" -L\"../libextern/lib\" -L\"../libPrint/Release\" -L\"../CxImage/Release\" -L\"../libSqlite/Release\" -L\"../libViewer/Release\" -L\"../libExplorer/Release\" -L\"../libIntro/Release\" -L\"../libSqlEngine/Release\" -L\"../libResource/Release\" -L\"../libCompress/Release\" -L\"../libDialog/Release\" -L\"../libDataStructure/Release\" -L\"../libPicture/Release\" -L\"../libUtility/Release\" -L\"../libGeolocalisation/Release\" -L\"../libSqlData/Release\" -L\"../libVideoThumbnail/Release\" -L\"../libFiltre/Release\" -L\"../libFiltreInterface/Release\" -L\"../libWindow/Release\" -L\"../libControl/Release\" -L\"../libTheme/Release\" -L\"../libParameter/Release\" -L\"../libOpenGL/Release\" -L\"../Otherlib/RegardsRaw/Release\" -L\"../libRenderBitmap/Release\" -L\"../libOpenCL/Release\" -L\"/opt/intel/intel-opencl-1.2-6.0.0.1049/opencl-1.2-6.0.0.1049/lib64\" -L\"../libVideoFFMpeg/Release\" -L\"../libEmail/Release\" -L\"../libExif/Release\" -L\"../libWindowMain/Release\" -L\"../Otherlib/wxSVG/Release\" ")

# Define the CXX sources
set ( CXX_SRCS
    ../Regards/Regards.cpp
)

set_source_files_properties(
    ${CXX_SRCS} PROPERTIES COMPILE_FLAGS 
    " -Os -fopenmp -std=c++11 -Wall")

add_executable(RegardsViewer ${CXX_SRCS} ${C_SRCS})

target_link_libraries(RegardsViewer
    Explorer
    Viewer
    Intro
    VideoFFMpeg
    Control
    Exif
    Email
    Picture
    Window
    WindowMain
    Geolocalisation
    Utility
    Dialog
    Resource
    SqlData
    DataStructure
    SqlEngine
    Sqlite
    dl
    Compress
    Utility
    Picture
    VideoThumbnail
    Dialog
    Geolocalisation
    Exif
    SqlData
    Print
    Filtre
    FiltreInterface
    RenderBitmap
    Theme
    DataStructure
    Utility
    Resource
    Parameter
    RegardsCL
    OpenCL
    OpenGL
    GLEWmx
    CxImage
    wxSVG
    GL
    SDL2
    wx_gtk2u_gl-3.0
    wx_gtk2u-3.0
    X11
    gtk-x11-2.0
    gdk-x11-2.0
    GLU
    pango-1.0
    pangocairo-1.0
    cairo
    gobject-2.0
    exiv2
    expat
    Jbig
    Psd
    jasper
    mng
    wxtiff-3.0
    RegardsRaw
    raw_r
    z
    gomp
    avcodec
    avformat
    avdevice
    avfilter
    avutil
    swresample
    swscale
    xcb
    xcb-shm
    Xxf86vm
    pthread
    gdk_pixbuf-2.0
    glib-2.0
    SM
    webkitgtk-1.0
    wxjpeg-3.0
    wxpng-3.0
    jpeg
    png12
)

# Setup dependencies
add_dependencies( RegardsViewer CxImage )
add_dependencies( RegardsViewer RegardsRaw )
add_dependencies( RegardsViewer libCompress )
add_dependencies( RegardsViewer libControl )
add_dependencies( RegardsViewer libWindowMain )
add_dependencies( RegardsViewer libWindow )
add_dependencies( RegardsViewer libViewer )
add_dependencies( RegardsViewer libVideoThumbnail )
add_dependencies( RegardsViewer libVideoFFMpeg )
add_dependencies( RegardsViewer libUtility )
add_dependencies( RegardsViewer libSqlEngine )
add_dependencies( RegardsViewer libSqlite )
add_dependencies( RegardsViewer libTheme )
add_dependencies( RegardsViewer libSqlData )
add_dependencies( RegardsViewer libResource )
add_dependencies( RegardsViewer libRenderBitmap )
add_dependencies( RegardsViewer libPrint )
add_dependencies( RegardsViewer libPicture )
add_dependencies( RegardsViewer libOpenCL )
add_dependencies( RegardsViewer libOpenGL )
add_dependencies( RegardsViewer libParameter )
add_dependencies( RegardsViewer libIntro )
add_dependencies( RegardsViewer libGeolocalisation )
add_dependencies( RegardsViewer libFiltre )
add_dependencies( RegardsViewer libExif )
add_dependencies( RegardsViewer libEmail )
add_dependencies( RegardsViewer libDialog )
add_dependencies( RegardsViewer libDataStructure )
