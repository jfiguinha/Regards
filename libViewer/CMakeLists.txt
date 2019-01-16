cmake_minimum_required(VERSION 2.8.11)

project(libViewer)



include_directories(
    /home/figuinha/dev/Regards/libViewer/
    /home/figuinha/dev/Regards/libViewer/
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
    /home/figuinha/dev/Regards/libWindowMain
    /home/figuinha/dev/Regards/libVideoFFMpeg
    /home/figuinha/dev/Regards/libextern/wxWidgets-3.0.2/include
    /home/figuinha/dev/Regards/libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0
    /home/figuinha/dev/Regards/libOpenCL

)

add_definitions(
    -DNDEBUG
    -D__WXGTK__
    -DVIEWER
)

# Linker options
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS}  -s ")

# Library path
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L\".\" ")

# Define the CXX sources
set ( CXX_SRCS
    CentralWnd.cpp
    FiltreEffect.cpp
    FiltreToolbar.cpp
    InfoEffect.cpp
    MainWindow.cpp
    ModificationManager.cpp
    PanelInfosWnd.cpp
    PreviewInfosWnd.cpp
    PreviewToolbar.cpp
    PreviewWnd.cpp
    ThumbnailBitmap.cpp
    ThumbnailViewerEffect.cpp
    ThumbnailViewerVideo.cpp
    toolbar.cpp
    ToolbarInfos.cpp
    ViewerFrame.cpp
    ViewerParam.cpp
    ViewerParamInit.cpp
    ViewerTheme.cpp
    ViewerThemeInit.cpp
    BrightAndContrastFilter.cpp
    CategoryFolderWindow.cpp
    CategoryWnd.cpp
    CloudsFilter.cpp
    ColorBalanceFilter.cpp
    FilterPreviewSplitter.cpp
    FilterWindowParam.cpp
    FiltreEffectWnd.cpp
    InfoEffectWnd.cpp
    LensFlareFilter.cpp
    MotionBlurFilter.cpp
    PhotoFiltreFilter.cpp
    PosterisationFilter.cpp
    RotateFreeFilter.cpp
    SolarisationFilter.cpp
    SwirlFilter.cpp
    ThumbnailViewerEffectWnd.cpp
    CriteriaFolderSplitter.cpp
    PreviewThumbnailSplitter.cpp
    ThumbnailFolder.cpp
    InfosSeparationBarExplorer.cpp
    ListPicture.cpp
    ThumbnailToolBar.cpp
    ThumbnailToolBarZoom.cpp
)

set_source_files_properties(
    ${CXX_SRCS} PROPERTIES COMPILE_FLAGS 
    " -fopenmp -std=c++11 -Wall")

add_library(libViewer ${CXX_SRCS} ${C_SRCS})

