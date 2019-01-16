##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=RegardsViewer_Windows
ConfigurationName      :=Debug
WorkspacePath          :=/home/figuinha/Developpement/Regards
ProjectPath            :=/home/figuinha/Developpement/Regards/RegardsViewer
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=14/01/19
CodeLitePath           :=/home/figuinha/.codelite
LinkerName             :=/usr/bin/ccache /usr/bin/g++
SharedObjectLinkerName :=/usr/bin/ccache /usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/RegardsViewer.exe
Preprocessors          :=$(PreprocessorSwitch)VIEWER $(PreprocessorSwitch)OPENGL $(PreprocessorSwitch)FFMPEG $(PreprocessorSwitch)SDL2 $(PreprocessorSwitch)LIBBPG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="RegardsViewer_Windows.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            := $(shell ../libextern/wxWidgets-master/wx-config   --libs --unicode=yes) -Wl,--allow-multiple-definition regards.rc.o 
IncludePath            := $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2  $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libTheme $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libextern/ffmpeg-4.1 $(IncludeSwitch)../libextern/glew-2.1.0/include $(IncludeSwitch)../libextern/SDL2-2.0.5/include $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libParameter $(IncludeSwitch)../libUtility $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libResource $(IncludeSwitch)../libSqlData $(IncludeSwitch)../libOpenGL $(IncludeSwitch)../libRenderBitmap $(IncludeSwitch)../libSqlEngine $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libFiltre $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../Otherlib/wxSVG/include $(IncludeSwitch)../libWindow $(IncludeSwitch)../libPrint $(IncludeSwitch)../libPicture $(IncludeSwitch)../libTheme $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libControl $(IncludeSwitch)../libIntro $(IncludeSwitch)../libDialog $(IncludeSwitch)../libViewer $(IncludeSwitch)../libExplorer $(IncludeSwitch)../libOpenCL $(IncludeSwitch)../libWindowMain $(IncludeSwitch)../libVideoFFMpeg $(IncludeSwitch)../libextern/dlfcn-win32-master 
IncludePCH             :=  -include ../include/Debug/header.h 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)Viewer $(LibrarySwitch)Intro $(LibrarySwitch)VideoFFMpeg $(LibrarySwitch)wxCheckTree $(LibrarySwitch)Control $(LibrarySwitch)Email $(LibrarySwitch)Exif_regards $(LibrarySwitch)Picture $(LibrarySwitch)FaceRecognition $(LibrarySwitch)Window $(LibrarySwitch)WindowMain $(LibrarySwitch)Geolocalisation $(LibrarySwitch)Utility $(LibrarySwitch)Dialog $(LibrarySwitch)Resource $(LibrarySwitch)SqlData $(LibrarySwitch)DataStructure $(LibrarySwitch)SqlEngine $(LibrarySwitch)Sqlite $(LibrarySwitch)wx_mswu_gl-3.1 $(LibrarySwitch)wx_mswu-3.1 $(LibrarySwitch)wxscintilla-3.1 $(LibrarySwitch)wxregexu-3.1 $(LibrarySwitch)z $(LibrarySwitch)rpcrt4 $(LibrarySwitch)oleaut32 $(LibrarySwitch)ole32 $(LibrarySwitch)uuid $(LibrarySwitch)winspool $(LibrarySwitch)winmm $(LibrarySwitch)shell32 $(LibrarySwitch)shlwapi $(LibrarySwitch)comctl32 $(LibrarySwitch)comdlg32 $(LibrarySwitch)advapi32 $(LibrarySwitch)version $(LibrarySwitch)wsock32 $(LibrarySwitch)gdi32 $(LibrarySwitch)expat $(LibrarySwitch)z $(LibrarySwitch)rpcrt4 $(LibrarySwitch)oleaut32 $(LibrarySwitch)ole32 $(LibrarySwitch)uuid $(LibrarySwitch)winspool $(LibrarySwitch)winmm $(LibrarySwitch)shell32 $(LibrarySwitch)shlwapi $(LibrarySwitch)comctl32 $(LibrarySwitch)comdlg32 $(LibrarySwitch)advapi32 $(LibrarySwitch)version $(LibrarySwitch)wsock32 $(LibrarySwitch)gdi32 $(LibrarySwitch)Compress $(LibrarySwitch)Utility $(LibrarySwitch)Picture $(LibrarySwitch)Dialog $(LibrarySwitch)Geolocalisation $(LibrarySwitch)Exif_regards $(LibrarySwitch)RegardsMediaInfo $(LibrarySwitch)MediaInfo $(LibrarySwitch)SqlData $(LibrarySwitch)Print $(LibrarySwitch)Filtre $(LibrarySwitch)Theme $(LibrarySwitch)DataStructure $(LibrarySwitch)Utility $(LibrarySwitch)Resource $(LibrarySwitch)Parameter $(LibrarySwitch)OpenCL $(LibrarySwitch)CxImage $(LibrarySwitch)wxSVG $(LibrarySwitch)RegardsRaw $(LibrarySwitch)OpenGL $(LibrarySwitch)RegardsCL $(LibrarySwitch)freeglut $(LibrarySwitch)glu32 $(LibrarySwitch)glew32 $(LibrarySwitch)exiv2 $(LibrarySwitch)cairo $(LibrarySwitch)iconv $(LibrarySwitch)expat $(LibrarySwitch)jasper $(LibrarySwitch)Jbig $(LibrarySwitch)mng $(LibrarySwitch)gomp $(LibrarySwitch)avformat $(LibrarySwitch)avcodec $(LibrarySwitch)swresample $(LibrarySwitch)avdevice $(LibrarySwitch)avfilter $(LibrarySwitch)avcodec $(LibrarySwitch)avutil $(LibrarySwitch)swresample $(LibrarySwitch)swscale $(LibrarySwitch)glib-2.0 $(LibrarySwitch)Psd $(LibrarySwitch)turbojpeg $(LibrarySwitch)wxjpeg-3.1 $(LibrarySwitch)wxpng-3.1 $(LibrarySwitch)wxtiff-3.1 $(LibrarySwitch)xml2 $(LibrarySwitch)webp $(LibrarySwitch)lzma $(LibrarySwitch)SDL2 $(LibrarySwitch)z $(LibrarySwitch)rpcrt4 $(LibrarySwitch)oleaut32 $(LibrarySwitch)ole32 $(LibrarySwitch)uuid $(LibrarySwitch)winspool $(LibrarySwitch)winmm $(LibrarySwitch)shell32 $(LibrarySwitch)shlwapi $(LibrarySwitch)comctl32 $(LibrarySwitch)comdlg32 $(LibrarySwitch)advapi32 $(LibrarySwitch)version $(LibrarySwitch)wsock32 $(LibrarySwitch)gdi32 $(LibrarySwitch)Imm32 $(LibrarySwitch)D3d9 $(LibrarySwitch)opengl32 $(LibrarySwitch)OpenCL $(LibrarySwitch)Strmiids $(LibrarySwitch)Quartz $(LibrarySwitch)bz2 $(LibrarySwitch)intl $(LibrarySwitch)psapi $(LibrarySwitch)ws2_32 $(LibrarySwitch)Secur32 $(LibrarySwitch)avicap32 $(LibrarySwitch)winpthread $(LibrarySwitch)libde265 $(LibrarySwitch)zen $(LibrarySwitch)curl $(LibrarySwitch)RegardsBpg $(LibrarySwitch)IlmImf $(LibrarySwitch)IexMath $(LibrarySwitch)Half $(LibrarySwitch)aom $(LibrarySwitch)Bcrypt $(LibrarySwitch)heif_shared $(LibrarySwitch)VideoThumbnail $(LibrarySwitch)fftw3f $(LibrarySwitch)exif 
ArLibs                 :=  "Viewer" "Intro" "VideoFFMpeg" "wxCheckTree" "Control" "Email" "Exif_regards" "Picture" "FaceRecognition" "Window" "WindowMain" "Geolocalisation" "Utility" "Dialog" "Resource" "SqlData" "DataStructure" "SqlEngine" "Sqlite" "wx_mswu_gl-3.1" "wx_mswu-3.1" "wxscintilla-3.1" "wxregexu-3.1" "z" "rpcrt4" "oleaut32" "ole32" "uuid" "winspool" "winmm" "shell32" "shlwapi" "comctl32" "comdlg32" "advapi32" "version" "wsock32" "gdi32" "expat" "z" "rpcrt4" "oleaut32" "ole32" "uuid" "winspool" "winmm" "shell32" "shlwapi" "comctl32" "comdlg32" "advapi32" "version" "wsock32" "gdi32" "Compress" "Utility" "Picture" "Dialog" "Geolocalisation" "Exif_regards" "RegardsMediaInfo" "MediaInfo" "SqlData" "Print" "Filtre" "Theme" "DataStructure" "Utility" "Resource" "Parameter" "OpenCL" "CxImage" "wxSVG" "RegardsRaw" "OpenGL" "RegardsCL" "freeglut" "glu32" "glew32" "exiv2" "cairo" "iconv" "expat" "jasper" "Jbig" "mng" "gomp" "avformat" "avcodec" "swresample" "avdevice" "avfilter" "avcodec" "avutil" "swresample" "swscale" "glib-2.0" "Psd" "turbojpeg" "wxjpeg-3.1" "wxpng-3.1" "wxtiff-3.1" "xml2" "webp" "lzma" "SDL2" "z" "rpcrt4" "oleaut32" "ole32" "uuid" "winspool" "winmm" "shell32" "shlwapi" "comctl32" "comdlg32" "advapi32" "version" "wsock32" "gdi32" "Imm32" "D3d9" "opengl32" "OpenCL" "Strmiids" "Quartz" "bz2" "intl" "psapi" "ws2_32" "Secur32" "avicap32" "winpthread" "liblibde265.dll" "zen" "curl" "RegardsBpg" "IlmImf" "IexMath" "Half" "aom" "Bcrypt" "libheif_shared" "VideoThumbnail" "fftw3f" "exif" 
LibPath                :=$(LibraryPathSwitch)/opt/AMDAPPSDK-3.0/lib  $(LibraryPathSwitch)../libextern/glew-2.1.0/lib $(LibraryPathSwitch)../libextern/jasper-1.900.1/src/libjasper/.libs $(LibraryPathSwitch)../libextern/SDL2-2.0.5/build/.libs $(LibraryPathSwitch)../libextern/exiv2-0.26/src/.libs $(LibraryPathSwitch)../libextern/wxWidgets-master/lib $(LibraryPathSwitch)../libextern/libmng-2.0.3/.libs $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libavcodec $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libavformat $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libavdevice $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libavfilter $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libavutil $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libswresample $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libswscale $(LibraryPathSwitch)../libextern/libde265-master/libde265 $(LibraryPathSwitch)../libextern/heif-master/lib $(LibraryPathSwitch)../libextern/libexif-master/libexif/.libs $(LibraryPathSwitch)../libextern/libwebp-0.6.0/src/.libs $(LibraryPathSwitch)../libextern/libjpeg-turbo-1.5.1/.libs $(LibraryPathSwitch)../libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/CMake/ZenLib $(LibraryPathSwitch)../libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/CMake $(LibraryPathSwitch)../libextern/aom_build $(LibraryPathSwitch)../libPrint/Debug $(LibraryPathSwitch)../CxImage/Debug $(LibraryPathSwitch)../libOpenGL/Debug $(LibraryPathSwitch)../libMediaInfo/Debug $(LibraryPathSwitch)../wxCheckTree/Debug $(LibraryPathSwitch)../libSqlite/Debug $(LibraryPathSwitch)../libViewer/Debug $(LibraryPathSwitch)../libExplorer/Debug $(LibraryPathSwitch)../libIntro/Debug $(LibraryPathSwitch)../libSqlEngine/Debug $(LibraryPathSwitch)../libResource/Debug $(LibraryPathSwitch)../libCompress/Debug $(LibraryPathSwitch)../libDialog/Debug $(LibraryPathSwitch)../libDataStructure/Debug $(LibraryPathSwitch)../libPicture/Debug $(LibraryPathSwitch)../libUtility/Debug $(LibraryPathSwitch)../libGeolocalisation/Debug $(LibraryPathSwitch)../libSqlData/Debug $(LibraryPathSwitch)../libVideoThumbnail/Debug $(LibraryPathSwitch)../Otherlib/RawSpeed/Debug $(LibraryPathSwitch)../libFiltre/Debug $(LibraryPathSwitch)../libFiltreInterface/Debug $(LibraryPathSwitch)../libWindow/Debug $(LibraryPathSwitch)../libControl/Debug $(LibraryPathSwitch)../libTheme/Debug $(LibraryPathSwitch)../libParameter/Debug $(LibraryPathSwitch)../Otherlib/RegardsRaw/Debug $(LibraryPathSwitch)../libRenderBitmap/Debug $(LibraryPathSwitch)../libOpenCL/Debug $(LibraryPathSwitch)../libVideoFFMpeg/Debug $(LibraryPathSwitch)../libEmail/Debug $(LibraryPathSwitch)../libExif/Debug $(LibraryPathSwitch)../libWindowMain/Debug $(LibraryPathSwitch)../Otherlib/wxSVG/Debug $(LibraryPathSwitch)../Otherlib/libPsd/Debug $(LibraryPathSwitch)../Otherlib/libJbig/Debug $(LibraryPathSwitch)../FaceRecognition_windows/Release $(LibraryPathSwitch)../Otherlib/libbpg/libRegardsBpg_windows/Debug $(LibraryPathSwitch)../libPiccante/Debug 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/ccache /usr/bin/g++
CC       := /usr/bin/ccache /usr/bin/gcc
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags --libs)  -fopenmp -std=gnu++11 -Wall -ggdb -O0 $(Preprocessors)
CFLAGS   :=  -Wall  -ggdb -O0 $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/up_Regards_Regards.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d "../.build-debug/include" "../.build-debug/libWindowMain" "../.build-debug/libWindow" "../.build-debug/libViewer" "../.build-debug/libVideoThumbnail" "../.build-debug/libVideoFFMpeg" "../.build-debug/libUtility" "../.build-debug/libTheme" "../.build-debug/libSqlite" "../.build-debug/libSqlEngine" "../.build-debug/libSqlData" "../.build-debug/libResource" "../.build-debug/libPrint" "../.build-debug/libPicture" "../.build-debug/libParameter" "../.build-debug/libOpenCL" "../.build-debug/libIntro" "../.build-debug/libGeolocalisation" "../.build-debug/libFiltre" "../.build-debug/libExif" "../.build-debug/libEmail" "../.build-debug/libDialog" "../.build-debug/libDataStructure" "../.build-debug/libCompress" "../.build-debug/RegardsRaw" "../.build-debug/CxImage" "../.build-debug/libControl" "../.build-debug/wxCheckTree" "../.build-debug/wxSVG" "../.build-debug/libMediaInfo" "../.build-debug/libPsd" "../.build-debug/libOpenGL" "../.build-debug/libJbig" "../.build-debug/libavcodec" "../.build-debug/libbpg" "../.build-debug/libavutil" "../.build-debug/libRegardsBpg_windows" $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

"../.build-debug/include":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/include"


"../.build-debug/libWindowMain":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libWindowMain"


"../.build-debug/libWindow":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libWindow"


"../.build-debug/libViewer":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libViewer"


"../.build-debug/libVideoThumbnail":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libVideoThumbnail"


"../.build-debug/libVideoFFMpeg":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libVideoFFMpeg"


"../.build-debug/libUtility":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libUtility"


"../.build-debug/libTheme":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libTheme"


"../.build-debug/libSqlite":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libSqlite"


"../.build-debug/libSqlEngine":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libSqlEngine"


"../.build-debug/libSqlData":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libSqlData"


"../.build-debug/libResource":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libResource"


"../.build-debug/libPrint":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libPrint"


"../.build-debug/libPicture":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libPicture"


"../.build-debug/libParameter":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libParameter"


"../.build-debug/libOpenCL":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libOpenCL"


"../.build-debug/libIntro":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libIntro"


"../.build-debug/libGeolocalisation":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libGeolocalisation"


"../.build-debug/libFiltre":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libFiltre"


"../.build-debug/libExif":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libExif"


"../.build-debug/libEmail":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libEmail"


"../.build-debug/libDialog":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libDialog"


"../.build-debug/libDataStructure":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libDataStructure"


"../.build-debug/libCompress":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libCompress"


"../.build-debug/RegardsRaw":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/RegardsRaw"


"../.build-debug/CxImage":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/CxImage"


"../.build-debug/libControl":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libControl"


"../.build-debug/wxCheckTree":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/wxCheckTree"


"../.build-debug/wxSVG":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/wxSVG"


"../.build-debug/libMediaInfo":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libMediaInfo"


"../.build-debug/libPsd":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libPsd"


"../.build-debug/libOpenGL":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libOpenGL"


"../.build-debug/libJbig":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libJbig"


"../.build-debug/libavcodec":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libavcodec"


"../.build-debug/libbpg":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libbpg"


"../.build-debug/libavutil":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libavutil"


"../.build-debug/libRegardsBpg_windows":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libRegardsBpg_windows"




PostBuild:
	@echo Executing Post Build commands ...
	cp -R ../Regards/Resources ./Debug
	cp C:/msys64/mingw64/bin/libbz2-1.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libcairo-2.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libcurl-4.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libexpat-1.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libffi-6.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libfontconfig-1.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libfreeglut.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libfreetype-6.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libgcc_s_seh-1.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libglib-2.0-0.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libgmp-10.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libgomp-1.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libgraphite2.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libHalf-2_3.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libharfbuzz-0.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libhogweed-4.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libiconv-2.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libidn2-4.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libIex-2_3.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libIexMath-2_3.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libIlmImf-2_3.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libIlmImfUtil-2_3.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libIlmThread-2_3.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libImath-2_3.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libintl-8.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/liblzma-5.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libnettle-6.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libnghttp2-14.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libp11-kit-0.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libpcre-1.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libpixman-1-0.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libpng16-16.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/librtmp-1.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libssh2-1.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libstdc++-6.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libtasn1-6.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libunistring-2.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libwinpthread-1.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libxml2-2.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/zlib1.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libgnutls-30.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libbrotlidec.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libpsl-5.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libbrotlicommon.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libcrypto-1_1-x64.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libfftw3f-3.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libfftw3-3.dll C:/Regards/RegardsViewer/Debug
	cp C:/msys64/mingw64/bin/libssl-1_1-x64.dll C:/Regards/RegardsViewer/Debug
	cp ../libextern/libjpeg-turbo-1.5.1/.libs/libturbojpeg-0.dll C:/Regards/RegardsViewer/Debug
	cp ../libextern/SDL2-2.0.5/build/.libs/SDL2.dll C:/Regards/RegardsViewer/Debug
	cp ../libextern/glew-2.1.0/lib/glew32.dll C:/Regards/RegardsViewer/Debug
	cp ../libextern/x265_2.5/build/msys/8bit/libx265.dll ./Debug/libx265.dll
	cp ../Otherlib/libbpg/libRegardsBpg_windows/Release/libRegardsBpg.dll ./Debug/libRegardsBpg.dll
	cp ../FaceRecognition_windows/Release/libFaceRecognition.dll ./Debug/libFaceRecognition.dll
	cp ../libextern/libde265-master/libde265/liblibde265.dll ./Debug/liblibde265.dll
	cp ../libextern/heif-master/lib/libheif_shared.dll ./Debug/libheif_shared.dll
	cp ../libextern/libexif-master/libexif/.libs/libexif-12.dll ./Debug/libexif-12.dll
	@echo Done

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:
	@echo Executing Pre Build commands ...
	$(RcCompilerName) -i regards.rc --use-temp-file --define WXMSW --define _UNICODE --include-dir ../libextern/wxWidgets-master/lib/wx/include/msw-unicode-static-3.1 --include-dir ../libextern/wxWidgets-master/include -o regards.rc.o
	@echo Done

# PreCompiled Header
../include/Debug/header.h.gch: ../include/Debug/header.h
	$(CXX) $(SourceSwitch) ../include/Debug/header.h $(PCHCompileFlags) $(CXXFLAGS) $(IncludePath)



##
## Objects
##
$(IntermediateDirectory)/up_Regards_Regards.cpp$(ObjectSuffix): ../Regards/Regards.cpp $(IntermediateDirectory)/up_Regards_Regards.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/Regards/Regards.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_Regards_Regards.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_Regards_Regards.cpp$(DependSuffix): ../Regards/Regards.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_Regards_Regards.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_Regards_Regards.cpp$(DependSuffix) -MM ../Regards/Regards.cpp

$(IntermediateDirectory)/up_Regards_Regards.cpp$(PreprocessSuffix): ../Regards/Regards.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_Regards_Regards.cpp$(PreprocessSuffix) ../Regards/Regards.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/
	$(RM) ../include/Debug/header.h.gch


