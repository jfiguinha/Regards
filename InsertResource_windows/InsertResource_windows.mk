##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=InsertResource_windows
ConfigurationName      :=Debug
WorkspacePath          :=C:/Regards
ProjectPath            :=C:/Regards/InsertResource_windows
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=jacques
Date                   :=12/12/2017
CodeLitePath           :="C:/Program Files/CodeLite"
LinkerName             :=C:/msys64/mingw64/bin/g++.exe
SharedObjectLinkerName :=C:/msys64/mingw64/bin/g++.exe -shared -fPIC
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
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=$(PreprocessorSwitch)VIEWER $(PreprocessorSwitch)OPENGL $(PreprocessorSwitch)FFMPEG $(PreprocessorSwitch)SDL2 $(PreprocessorSwitch)LIBBPG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="InsertResource_windows.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/msys64/mingw64/bin/windres.exe
LinkOptions            := $(shell ../libextern/wxWidgets-master/wx-config   --libs --unicode=yes) -Wl,--allow-multiple-definition  
IncludePath            := $(IncludeSwitch)"C:\Program Files (x86)\AMD APP SDK\3.0_mingw\include" $(IncludeSwitch)/mingw64/include/libxml2 $(IncludeSwitch)/mingw64/include/openEXR $(IncludeSwitch)/mingw64/include/ilmbase  $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libTheme $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libextern/ffmpeg-3.3.4 $(IncludeSwitch)../libextern/glew-2.1.0/include $(IncludeSwitch)../libextern/SDL2-2.0.5/include $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libParameter $(IncludeSwitch)../libUtility $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libResource $(IncludeSwitch)../libSqlData $(IncludeSwitch)../libOpenGL $(IncludeSwitch)../libRenderBitmap $(IncludeSwitch)../libSqlEngine $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libFiltre $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../Otherlib/wxSVG/include $(IncludeSwitch)../libWindow $(IncludeSwitch)../libPrint $(IncludeSwitch)../libPicture $(IncludeSwitch)../libTheme $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libControl $(IncludeSwitch)../libIntro $(IncludeSwitch)../libDialog $(IncludeSwitch)../libViewer $(IncludeSwitch)../libExplorer $(IncludeSwitch)../libOpenCL $(IncludeSwitch)../libWindowMain $(IncludeSwitch)../libVideoFFMpeg $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)Viewer $(LibrarySwitch)Intro $(LibrarySwitch)VideoFFMpeg $(LibrarySwitch)wxCheckTree $(LibrarySwitch)Control $(LibrarySwitch)Email $(LibrarySwitch)Exif $(LibrarySwitch)Picture $(LibrarySwitch)FaceRecognition $(LibrarySwitch)Window $(LibrarySwitch)WindowMain $(LibrarySwitch)Geolocalisation $(LibrarySwitch)Utility $(LibrarySwitch)Dialog $(LibrarySwitch)Resource $(LibrarySwitch)SqlData $(LibrarySwitch)DataStructure $(LibrarySwitch)SqlEngine $(LibrarySwitch)Sqlite $(LibrarySwitch)wx_mswu_gl-3.1 $(LibrarySwitch)wx_mswu-3.1 $(LibrarySwitch)wxscintilla-3.1 $(LibrarySwitch)wxregexu-3.1 $(LibrarySwitch)z $(LibrarySwitch)rpcrt4 $(LibrarySwitch)oleaut32 $(LibrarySwitch)ole32 $(LibrarySwitch)uuid $(LibrarySwitch)winspool $(LibrarySwitch)winmm $(LibrarySwitch)shell32 $(LibrarySwitch)shlwapi $(LibrarySwitch)comctl32 $(LibrarySwitch)comdlg32 $(LibrarySwitch)advapi32 $(LibrarySwitch)version $(LibrarySwitch)wsock32 $(LibrarySwitch)gdi32 $(LibrarySwitch)expat $(LibrarySwitch)z $(LibrarySwitch)rpcrt4 $(LibrarySwitch)oleaut32 $(LibrarySwitch)ole32 $(LibrarySwitch)uuid $(LibrarySwitch)winspool $(LibrarySwitch)winmm $(LibrarySwitch)shell32 $(LibrarySwitch)shlwapi $(LibrarySwitch)comctl32 $(LibrarySwitch)comdlg32 $(LibrarySwitch)advapi32 $(LibrarySwitch)version $(LibrarySwitch)wsock32 $(LibrarySwitch)gdi32 $(LibrarySwitch)Compress $(LibrarySwitch)Utility $(LibrarySwitch)Picture $(LibrarySwitch)VideoThumbnail $(LibrarySwitch)Dialog $(LibrarySwitch)Geolocalisation $(LibrarySwitch)Exif $(LibrarySwitch)RegardsMediaInfo $(LibrarySwitch)MediaInfo $(LibrarySwitch)SqlData $(LibrarySwitch)Print $(LibrarySwitch)Filtre $(LibrarySwitch)FiltreInterface $(LibrarySwitch)Theme $(LibrarySwitch)DataStructure $(LibrarySwitch)Utility $(LibrarySwitch)Resource $(LibrarySwitch)Parameter $(LibrarySwitch)OpenCL $(LibrarySwitch)CxImage $(LibrarySwitch)wxSVG $(LibrarySwitch)RegardsRaw $(LibrarySwitch)OpenGL $(LibrarySwitch)RegardsCL $(LibrarySwitch)freeglut $(LibrarySwitch)glu32 $(LibrarySwitch)glew32 $(LibrarySwitch)exiv2 $(LibrarySwitch)cairo $(LibrarySwitch)iconv $(LibrarySwitch)expat $(LibrarySwitch)jasper $(LibrarySwitch)Jbig $(LibrarySwitch)mng $(LibrarySwitch)gomp $(LibrarySwitch)avformat $(LibrarySwitch)avcodec $(LibrarySwitch)swresample $(LibrarySwitch)avdevice $(LibrarySwitch)avfilter $(LibrarySwitch)avcodec $(LibrarySwitch)avutil $(LibrarySwitch)swresample $(LibrarySwitch)swscale $(LibrarySwitch)glib-2.0 $(LibrarySwitch)Psd $(LibrarySwitch)turbojpeg $(LibrarySwitch)wxjpeg-3.1 $(LibrarySwitch)wxpng-3.1 $(LibrarySwitch)wxtiff-3.1 $(LibrarySwitch)xml2 $(LibrarySwitch)webp $(LibrarySwitch)lzma $(LibrarySwitch)SDL2 $(LibrarySwitch)z $(LibrarySwitch)rpcrt4 $(LibrarySwitch)oleaut32 $(LibrarySwitch)ole32 $(LibrarySwitch)uuid $(LibrarySwitch)winspool $(LibrarySwitch)winmm $(LibrarySwitch)shell32 $(LibrarySwitch)shlwapi $(LibrarySwitch)comctl32 $(LibrarySwitch)comdlg32 $(LibrarySwitch)advapi32 $(LibrarySwitch)version $(LibrarySwitch)wsock32 $(LibrarySwitch)gdi32 $(LibrarySwitch)Imm32 $(LibrarySwitch)D3d9 $(LibrarySwitch)opengl32 $(LibrarySwitch)OpenCL $(LibrarySwitch)Strmiids $(LibrarySwitch)Quartz $(LibrarySwitch)bz2 $(LibrarySwitch)intl $(LibrarySwitch)psapi $(LibrarySwitch)ws2_32 $(LibrarySwitch)Secur32 $(LibrarySwitch)avicap32 $(LibrarySwitch)winpthread $(LibrarySwitch)heifreader $(LibrarySwitch)common $(LibrarySwitch)h265parser $(LibrarySwitch)avcparser $(LibrarySwitch)libde265 $(LibrarySwitch)zen $(LibrarySwitch)curl $(LibrarySwitch)RegardsBpg $(LibrarySwitch)IlmImf $(LibrarySwitch)IexMath $(LibrarySwitch)Half 
ArLibs                 :=  "Viewer" "Intro" "VideoFFMpeg" "wxCheckTree" "Control" "Email" "Exif" "Picture" "FaceRecognition" "Window" "WindowMain" "Geolocalisation" "Utility" "Dialog" "Resource" "SqlData" "DataStructure" "SqlEngine" "Sqlite" "wx_mswu_gl-3.1" "wx_mswu-3.1" "wxscintilla-3.1" "wxregexu-3.1" "z" "rpcrt4" "oleaut32" "ole32" "uuid" "winspool" "winmm" "shell32" "shlwapi" "comctl32" "comdlg32" "advapi32" "version" "wsock32" "gdi32" "expat" "z" "rpcrt4" "oleaut32" "ole32" "uuid" "winspool" "winmm" "shell32" "shlwapi" "comctl32" "comdlg32" "advapi32" "version" "wsock32" "gdi32" "Compress" "Utility" "Picture" "VideoThumbnail" "Dialog" "Geolocalisation" "Exif" "RegardsMediaInfo" "MediaInfo" "SqlData" "Print" "Filtre" "FiltreInterface" "Theme" "DataStructure" "Utility" "Resource" "Parameter" "OpenCL" "CxImage" "wxSVG" "RegardsRaw" "OpenGL" "RegardsCL" "freeglut" "glu32" "glew32" "exiv2" "cairo" "iconv" "expat" "jasper" "Jbig" "mng" "gomp" "avformat" "avcodec" "swresample" "avdevice" "avfilter" "avcodec" "avutil" "swresample" "swscale" "glib-2.0" "Psd" "turbojpeg" "wxjpeg-3.1" "wxpng-3.1" "wxtiff-3.1" "xml2" "webp" "lzma" "SDL2" "z" "rpcrt4" "oleaut32" "ole32" "uuid" "winspool" "winmm" "shell32" "shlwapi" "comctl32" "comdlg32" "advapi32" "version" "wsock32" "gdi32" "Imm32" "D3d9" "opengl32" "OpenCL" "Strmiids" "Quartz" "bz2" "intl" "psapi" "ws2_32" "Secur32" "avicap32" "winpthread" "heifreader" "common" "h265parser" "avcparser" "liblibde265.dll" "zen" "curl" "RegardsBpg" "IlmImf" "IexMath" "Half" 
LibPath                :=$(LibraryPathSwitch)"C:\Program Files (x86)\AMD APP SDK\3.0_mingw\lib\x86_64"  $(LibraryPathSwitch)../libextern/glew-2.1.0/lib $(LibraryPathSwitch)../libextern/jasper-1.900.1/src/libjasper/.libs $(LibraryPathSwitch)../libextern/SDL2-2.0.5/build/.libs $(LibraryPathSwitch)../libextern/exiv2-0.26/src/.libs $(LibraryPathSwitch)../libextern/wxWidgets-master/lib $(LibraryPathSwitch)../libextern/libmng-2.0.3/.libs $(LibraryPathSwitch)../libextern/ffmpeg-3.3.4/libavcodec $(LibraryPathSwitch)../libextern/ffmpeg-3.3.4/libavformat $(LibraryPathSwitch)../libextern/ffmpeg-3.3.4/libavdevice $(LibraryPathSwitch)../libextern/ffmpeg-3.3.4/libavfilter $(LibraryPathSwitch)../libextern/ffmpeg-3.3.4/libavutil $(LibraryPathSwitch)../libextern/ffmpeg-3.3.4/libswresample $(LibraryPathSwitch)../libextern/ffmpeg-3.3.4/libswscale $(LibraryPathSwitch)../libextern/libde265-master/libde265 $(LibraryPathSwitch)../libextern/heif-master/Srcs/reader $(LibraryPathSwitch)../libextern/heif-master/Srcs/common $(LibraryPathSwitch)../libextern/heif-master/Srcs/parser/h265parser $(LibraryPathSwitch)../libextern/heif-master/Srcs/parser/avcparser $(LibraryPathSwitch)../libextern/libwebp-0.6.0/src/.libs $(LibraryPathSwitch)../libextern/libjpeg-turbo-1.5.1/.libs $(LibraryPathSwitch)../libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/CMake/ZenLib $(LibraryPathSwitch)../libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/CMake $(LibraryPathSwitch)../libPrint/Debug $(LibraryPathSwitch)../CxImage/Debug $(LibraryPathSwitch)../libOpenGL/Debug $(LibraryPathSwitch)../libMediaInfo/Debug $(LibraryPathSwitch)../wxCheckTree/Debug $(LibraryPathSwitch)../libSqlite/Debug $(LibraryPathSwitch)../libViewer/Debug $(LibraryPathSwitch)../libExplorer/Debug $(LibraryPathSwitch)../libIntro/Debug $(LibraryPathSwitch)../libSqlEngine/Debug $(LibraryPathSwitch)../libResource/Debug $(LibraryPathSwitch)../libCompress/Debug $(LibraryPathSwitch)../libDialog/Debug $(LibraryPathSwitch)../libDataStructure/Debug $(LibraryPathSwitch)../libPicture/Debug $(LibraryPathSwitch)../libUtility/Debug $(LibraryPathSwitch)../libGeolocalisation/Debug $(LibraryPathSwitch)../libSqlData/Debug $(LibraryPathSwitch)../libVideoThumbnail/Debug $(LibraryPathSwitch)../Otherlib/RawSpeed/Debug $(LibraryPathSwitch)../libFiltre/Debug $(LibraryPathSwitch)../libFiltreInterface/Debug $(LibraryPathSwitch)../libWindow/Debug $(LibraryPathSwitch)../libControl/Debug $(LibraryPathSwitch)../libTheme/Debug $(LibraryPathSwitch)../libParameter/Debug $(LibraryPathSwitch)../Otherlib/RegardsRaw/Debug $(LibraryPathSwitch)../libRenderBitmap/Debug $(LibraryPathSwitch)../libOpenCL/Debug $(LibraryPathSwitch)../libVideoFFMpeg/Debug $(LibraryPathSwitch)../libEmail/Debug $(LibraryPathSwitch)../libExif/Debug $(LibraryPathSwitch)../libWindowMain/Debug $(LibraryPathSwitch)../Otherlib/wxSVG/Debug $(LibraryPathSwitch)../Otherlib/libPsd/Debug $(LibraryPathSwitch)../Otherlib/libJbig/Debug $(LibraryPathSwitch)../FaceRecognition_windows/Release $(LibraryPathSwitch)../Otherlib/libbpg/libRegardsBpg_windows/Debug 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/msys64/mingw64/bin/ar.exe rcu  --plugin=$(shell gcc --print-file-name=liblto_plugin-0.dll)
CXX      := C:/msys64/mingw64/bin/g++.exe
CC       := C:/msys64/mingw64/bin/gcc.exe
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags --libs)  -fopenmp -std=gnu++11 -Wall -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -Wall  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := C:/msys64/mingw64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
PATH:=C:\msys64\usr\bin;C:\msys64\mingw64\bin;$PATH
Objects0=$(IntermediateDirectory)/up_InsertResource_InsertResource.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Debug"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_InsertResource_InsertResource.cpp$(ObjectSuffix): ../InsertResource/InsertResource.cpp $(IntermediateDirectory)/up_InsertResource_InsertResource.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/InsertResource/InsertResource.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_InsertResource_InsertResource.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_InsertResource_InsertResource.cpp$(DependSuffix): ../InsertResource/InsertResource.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_InsertResource_InsertResource.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_InsertResource_InsertResource.cpp$(DependSuffix) -MM ../InsertResource/InsertResource.cpp

$(IntermediateDirectory)/up_InsertResource_InsertResource.cpp$(PreprocessSuffix): ../InsertResource/InsertResource.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_InsertResource_InsertResource.cpp$(PreprocessSuffix) ../InsertResource/InsertResource.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


