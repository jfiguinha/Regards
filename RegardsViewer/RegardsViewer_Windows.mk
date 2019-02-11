##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=RegardsViewer_Windows
ConfigurationName      :=Release
WorkspacePath          :=C:/Regards
ProjectPath            :=C:/Regards/RegardsViewer
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=jfigu_000
Date                   :=11/02/2019
CodeLitePath           :="C:/Program Files/CodeLite"
LinkerName             :=C:/msys64/usr/bin/ccache.exe C:/msys64/mingw64/bin/g++.exe
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
OutputFile             :=$(IntermediateDirectory)/RegardsViewer.exe
Preprocessors          :=$(PreprocessorSwitch)VIEWER $(PreprocessorSwitch)OPENGL $(PreprocessorSwitch)FFMPEG $(PreprocessorSwitch)SDL2 $(PreprocessorSwitch)LIBBPG $(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="RegardsViewer_Windows.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := $(shell wx-config --rcflags)
RcCompilerName         :=C:/msys64/mingw64/bin/windres.exe
LinkOptions            := $(shell ../libextern/wxWidgets-master/wx-config   --libs --unicode=yes) -Wl,--allow-multiple-definition regards.rc.o 
IncludePath            := $(IncludeSwitch)"C:\Program Files (x86)\AMD APP SDK\3.0_mingw\include" $(IncludeSwitch)/mingw64/include/libxml2 $(IncludeSwitch)/mingw64/include/OpenEXR  $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libTheme $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libextern/ffmpeg-4.1 $(IncludeSwitch)../libextern/glew-2.1.0/include $(IncludeSwitch)../libextern/SDL2-2.0.5/include $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libParameter $(IncludeSwitch)../libUtility $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libResource $(IncludeSwitch)../libSqlData $(IncludeSwitch)../libOpenGL $(IncludeSwitch)../libRenderBitmap $(IncludeSwitch)../libSqlEngine $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libFiltre $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../Otherlib/wxSVG/include $(IncludeSwitch)../libWindow $(IncludeSwitch)../libPrint $(IncludeSwitch)../libPicture $(IncludeSwitch)../libTheme $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libControl $(IncludeSwitch)../libIntro $(IncludeSwitch)../libDialog $(IncludeSwitch)../libViewer $(IncludeSwitch)../libExplorer $(IncludeSwitch)../libOpenCL $(IncludeSwitch)../libWindowMain $(IncludeSwitch)../libVideoFFMpeg $(IncludeSwitch)../libextern/dlfcn-win32-master 
IncludePCH             :=  -include ../include/Release/header.h 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)Viewer $(LibrarySwitch)Intro $(LibrarySwitch)VideoFFMpeg $(LibrarySwitch)wxCheckTree $(LibrarySwitch)Control $(LibrarySwitch)Email $(LibrarySwitch)Exif_regards $(LibrarySwitch)Picture $(LibrarySwitch)FaceRecognition $(LibrarySwitch)Window $(LibrarySwitch)WindowMain $(LibrarySwitch)Geolocalisation $(LibrarySwitch)Utility $(LibrarySwitch)Dialog $(LibrarySwitch)Resource $(LibrarySwitch)SqlData $(LibrarySwitch)DataStructure $(LibrarySwitch)SqlEngine $(LibrarySwitch)Sqlite $(LibrarySwitch)wx_mswu_gl-3.1 $(LibrarySwitch)wx_mswu-3.1 $(LibrarySwitch)wxscintilla-3.1 $(LibrarySwitch)wxregexu-3.1 $(LibrarySwitch)z $(LibrarySwitch)rpcrt4 $(LibrarySwitch)oleaut32 $(LibrarySwitch)ole32 $(LibrarySwitch)uuid $(LibrarySwitch)winspool $(LibrarySwitch)winmm $(LibrarySwitch)shell32 $(LibrarySwitch)shlwapi $(LibrarySwitch)comctl32 $(LibrarySwitch)comdlg32 $(LibrarySwitch)advapi32 $(LibrarySwitch)version $(LibrarySwitch)wsock32 $(LibrarySwitch)gdi32 $(LibrarySwitch)expat $(LibrarySwitch)z $(LibrarySwitch)rpcrt4 $(LibrarySwitch)oleaut32 $(LibrarySwitch)ole32 $(LibrarySwitch)uuid $(LibrarySwitch)winspool $(LibrarySwitch)winmm $(LibrarySwitch)shell32 $(LibrarySwitch)shlwapi $(LibrarySwitch)comctl32 $(LibrarySwitch)comdlg32 $(LibrarySwitch)advapi32 $(LibrarySwitch)version $(LibrarySwitch)wsock32 $(LibrarySwitch)gdi32 $(LibrarySwitch)Compress $(LibrarySwitch)Utility $(LibrarySwitch)Picture $(LibrarySwitch)Dialog $(LibrarySwitch)Geolocalisation $(LibrarySwitch)Exif_regards $(LibrarySwitch)RegardsMediaInfo $(LibrarySwitch)MediaInfo $(LibrarySwitch)SqlData $(LibrarySwitch)Print $(LibrarySwitch)Filtre $(LibrarySwitch)Theme $(LibrarySwitch)DataStructure $(LibrarySwitch)Utility $(LibrarySwitch)Resource $(LibrarySwitch)Parameter $(LibrarySwitch)OpenCL $(LibrarySwitch)CxImage $(LibrarySwitch)wxSVG $(LibrarySwitch)RegardsRaw $(LibrarySwitch)OpenGL $(LibrarySwitch)RegardsCL $(LibrarySwitch)SqlData $(LibrarySwitch)freeglut $(LibrarySwitch)glu32 $(LibrarySwitch)glew32 $(LibrarySwitch)exiv2 $(LibrarySwitch)cairo $(LibrarySwitch)iconv $(LibrarySwitch)expat $(LibrarySwitch)jasper $(LibrarySwitch)Jbig $(LibrarySwitch)mng $(LibrarySwitch)gomp $(LibrarySwitch)avformat $(LibrarySwitch)avcodec $(LibrarySwitch)swresample $(LibrarySwitch)avdevice $(LibrarySwitch)avfilter $(LibrarySwitch)avcodec $(LibrarySwitch)avutil $(LibrarySwitch)swresample $(LibrarySwitch)swscale $(LibrarySwitch)glib-2.0 $(LibrarySwitch)Psd $(LibrarySwitch)turbojpeg $(LibrarySwitch)wxjpeg-3.1 $(LibrarySwitch)wxpng-3.1 $(LibrarySwitch)wxtiff-3.1 $(LibrarySwitch)xml2 $(LibrarySwitch)webp $(LibrarySwitch)lzma $(LibrarySwitch)SDL2 $(LibrarySwitch)z $(LibrarySwitch)rpcrt4 $(LibrarySwitch)oleaut32 $(LibrarySwitch)ole32 $(LibrarySwitch)uuid $(LibrarySwitch)winspool $(LibrarySwitch)winmm $(LibrarySwitch)shell32 $(LibrarySwitch)shlwapi $(LibrarySwitch)comctl32 $(LibrarySwitch)comdlg32 $(LibrarySwitch)advapi32 $(LibrarySwitch)version $(LibrarySwitch)wsock32 $(LibrarySwitch)gdi32 $(LibrarySwitch)Imm32 $(LibrarySwitch)D3d9 $(LibrarySwitch)opengl32 $(LibrarySwitch)OpenCL $(LibrarySwitch)Strmiids $(LibrarySwitch)Quartz $(LibrarySwitch)bz2 $(LibrarySwitch)intl $(LibrarySwitch)psapi $(LibrarySwitch)ws2_32 $(LibrarySwitch)Secur32 $(LibrarySwitch)avicap32 $(LibrarySwitch)winpthread $(LibrarySwitch)libde265 $(LibrarySwitch)zen $(LibrarySwitch)curl $(LibrarySwitch)RegardsBpg $(LibrarySwitch)IlmImf $(LibrarySwitch)IexMath $(LibrarySwitch)Half $(LibrarySwitch)aom $(LibrarySwitch)Bcrypt $(LibrarySwitch)heif_shared $(LibrarySwitch)VideoThumbnail $(LibrarySwitch)fftw3f $(LibrarySwitch)exif 
ArLibs                 :=  "Viewer" "Intro" "VideoFFMpeg" "wxCheckTree" "Control" "Email" "Exif_regards" "Picture" "FaceRecognition" "Window" "WindowMain" "Geolocalisation" "Utility" "Dialog" "Resource" "SqlData" "DataStructure" "SqlEngine" "Sqlite" "wx_mswu_gl-3.1" "wx_mswu-3.1" "wxscintilla-3.1" "wxregexu-3.1" "z" "rpcrt4" "oleaut32" "ole32" "uuid" "winspool" "winmm" "shell32" "shlwapi" "comctl32" "comdlg32" "advapi32" "version" "wsock32" "gdi32" "expat" "z" "rpcrt4" "oleaut32" "ole32" "uuid" "winspool" "winmm" "shell32" "shlwapi" "comctl32" "comdlg32" "advapi32" "version" "wsock32" "gdi32" "Compress" "Utility" "Picture" "Dialog" "Geolocalisation" "Exif_regards" "RegardsMediaInfo" "MediaInfo" "SqlData" "Print" "Filtre" "Theme" "DataStructure" "Utility" "Resource" "Parameter" "OpenCL" "CxImage" "wxSVG" "RegardsRaw" "OpenGL" "RegardsCL" "SqlData" "freeglut" "glu32" "glew32" "exiv2" "cairo" "iconv" "expat" "jasper" "Jbig" "mng" "gomp" "avformat" "avcodec" "swresample" "avdevice" "avfilter" "avcodec" "avutil" "swresample" "swscale" "glib-2.0" "Psd" "turbojpeg" "wxjpeg-3.1" "wxpng-3.1" "wxtiff-3.1" "xml2" "webp" "lzma" "SDL2" "z" "rpcrt4" "oleaut32" "ole32" "uuid" "winspool" "winmm" "shell32" "shlwapi" "comctl32" "comdlg32" "advapi32" "version" "wsock32" "gdi32" "Imm32" "D3d9" "opengl32" "OpenCL" "Strmiids" "Quartz" "bz2" "intl" "psapi" "ws2_32" "Secur32" "avicap32" "winpthread" "liblibde265.dll" "zen" "curl" "RegardsBpg" "IlmImf" "IexMath" "Half" "aom" "Bcrypt" "libheif_shared" "VideoThumbnail" "fftw3f" "exif" 
LibPath                :=$(LibraryPathSwitch)"C:\Program Files (x86)\AMD APP SDK\3.0_mingw\lib\x86_64"  $(LibraryPathSwitch)../libextern/glew-2.1.0/lib $(LibraryPathSwitch)../libextern/jasper-1.900.1/src/libjasper/.libs $(LibraryPathSwitch)../libextern/SDL2-2.0.5/build/.libs $(LibraryPathSwitch)../libextern/exiv2-0.26/src/.libs $(LibraryPathSwitch)../libextern/wxWidgets-master/lib $(LibraryPathSwitch)../libextern/libmng-2.0.3/.libs $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libavcodec $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libavformat $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libavdevice $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libavfilter $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libavutil $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libswresample $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libswscale $(LibraryPathSwitch)../libextern/libde265-master/libde265 $(LibraryPathSwitch)../libextern/heif-master/lib $(LibraryPathSwitch)../libextern/libexif-master/libexif/.libs $(LibraryPathSwitch)../libextern/libwebp-0.6.0/src/.libs $(LibraryPathSwitch)../libextern/libjpeg-turbo-1.5.1/.libs $(LibraryPathSwitch)../libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/CMake/ZenLib $(LibraryPathSwitch)../libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/CMake $(LibraryPathSwitch)../libextern/aom_build $(LibraryPathSwitch)../libPrint/Release $(LibraryPathSwitch)../CxImage/Release $(LibraryPathSwitch)../libOpenGL/Release $(LibraryPathSwitch)../libMediaInfo/Release $(LibraryPathSwitch)../wxCheckTree/Release $(LibraryPathSwitch)../libSqlite/Release $(LibraryPathSwitch)../libViewer/Release $(LibraryPathSwitch)../libExplorer/Release $(LibraryPathSwitch)../libIntro/Release $(LibraryPathSwitch)../libSqlEngine/Release $(LibraryPathSwitch)../libResource/Release $(LibraryPathSwitch)../libCompress/Release $(LibraryPathSwitch)../libDialog/Release $(LibraryPathSwitch)../libDataStructure/Release $(LibraryPathSwitch)../libPicture/Release $(LibraryPathSwitch)../libUtility/Release $(LibraryPathSwitch)../libGeolocalisation/Release $(LibraryPathSwitch)../libSqlData/Release $(LibraryPathSwitch)../libVideoThumbnail/Release $(LibraryPathSwitch)../Otherlib/RawSpeed/Release $(LibraryPathSwitch)../libFiltre/Release $(LibraryPathSwitch)../libFiltreInterface/Release $(LibraryPathSwitch)../libWindow/Release $(LibraryPathSwitch)../libControl/Release $(LibraryPathSwitch)../libTheme/Release $(LibraryPathSwitch)../libParameter/Release $(LibraryPathSwitch)../Otherlib/RegardsRaw/Release $(LibraryPathSwitch)../libRenderBitmap/Release $(LibraryPathSwitch)../libOpenCL/Release $(LibraryPathSwitch)../libVideoFFMpeg/Release $(LibraryPathSwitch)../libEmail/Release $(LibraryPathSwitch)../libExif/Release $(LibraryPathSwitch)../libPiccante/Release $(LibraryPathSwitch)../libWindowMain/Release $(LibraryPathSwitch)../Otherlib/wxSVG/Release $(LibraryPathSwitch)../Otherlib/libPsd/Release $(LibraryPathSwitch)../Otherlib/libJbig/Release $(LibraryPathSwitch)../FaceRecognition_windows/Release $(LibraryPathSwitch)../Otherlib/libbpg/libRegardsBpg_windows/Release 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/msys64/mingw64/bin/ar.exe rcu
CXX      := C:/msys64/usr/bin/ccache.exe C:/msys64/mingw64/bin/g++.exe
CC       := C:/msys64/usr/bin/ccache.exe  C:/msys64/mingw64/bin/gcc.exe
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags --libs)  -fopenmp -std=gnu++11 -Wall -O2 -msse4.1  $(Preprocessors)
CFLAGS   :=  -Wall  -O2 -msse4.1  $(Preprocessors)
ASFLAGS  := 
AS       := C:/msys64/mingw64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
PATH:=C:\msys64\usr\bin;$PATH
WXWIN:=C:\Regards\libextern\wxwidgets-3.1.2
WXCFG:=lib/mswud
Objects0=$(IntermediateDirectory)/up_Regards_Regards.cpp$(ObjectSuffix) 



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

PostBuild:
	@echo Executing Post Build commands ...
	cp -R ../Regards/Resources ./Release
	cp C:/msys64/mingw64/bin/libbz2-1.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libcairo-2.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libcurl-4.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libexpat-1.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libffi-6.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libfontconfig-1.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libfreeglut.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libfreetype-6.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libgcc_s_seh-1.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libglib-2.0-0.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libgmp-10.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libgomp-1.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libgraphite2.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libHalf-2_3.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libharfbuzz-0.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libhogweed-4.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libiconv-2.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libIex-2_3.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libIexMath-2_3.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libIlmImf-2_3.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libIlmImfUtil-2_3.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libIlmThread-2_3.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libImath-2_3.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libintl-8.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/liblzma-5.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libnettle-6.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libnghttp2-14.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libp11-kit-0.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libpcre-1.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libpixman-1-0.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libpng16-16.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/librtmp-1.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libssh2-1.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libstdc++-6.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libtasn1-6.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libunistring-2.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libwinpthread-1.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libxml2-2.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/zlib1.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libgnutls-30.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libbrotlidec.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libpsl-5.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libbrotlicommon.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libfftw3f-3.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libfftw3-3.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libcrypto-1_1-x64.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libssl-1_1-x64.dll C:/Regards/RegardsViewer/Release
	cp C:/msys64/mingw64/bin/libidn2-4.dll C:/Regards/RegardsViewer/Release
	cp ../libextern/libjpeg-turbo-1.5.1/.libs/libturbojpeg-0.dll C:/Regards/RegardsViewer/Release
	cp ../libextern/SDL2-2.0.5/build/.libs/SDL2.dll C:/Regards/RegardsViewer/Release
	cp ../libextern/glew-2.1.0/lib/glew32.dll C:/Regards/RegardsViewer/Release
	cp ../libextern/x265_2.5/build/msys/8bit/libx265.dll ./Release/libx265.dll
	cp ../Otherlib/libbpg/libRegardsBpg_windows/Release/libRegardsBpg.dll ./Release/libRegardsBpg.dll
	cp ../FaceRecognition_windows/Release/libFaceRecognition.dll ./Release/libFaceRecognition.dll
	cp ../libextern/libde265-master/libde265/liblibde265.dll ./Release/liblibde265.dll
	cp ../libextern/libexif-master/libexif/.libs/libexif-12.dll ./Release/libexif-12.dll
	cp ../libextern/heif-master/lib/libheif_shared.dll ./Release/libheif_shared.dll
	
	
	@echo Done

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Release"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Release"

PreBuild:
	@echo Executing Pre Build commands ...
	$(RcCompilerName) -i regards.rc --use-temp-file --define WXMSW --define _UNICODE --include-dir ../libextern/wxWidgets-master/lib/wx/include/msw-unicode-static-3.1 --include-dir ../libextern/wxWidgets-master/include -o regards.rc.o
	@echo Done

# PreCompiled Header
../include/Release/header.h.gch: ../include/Release/header.h
	$(CXX) $(SourceSwitch) ../include/Release/header.h $(PCHCompileFlags) $(CXXFLAGS) $(IncludePath)



##
## Objects
##
$(IntermediateDirectory)/up_Regards_Regards.cpp$(ObjectSuffix): ../Regards/Regards.cpp $(IntermediateDirectory)/up_Regards_Regards.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Regards/Regards.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_Regards_Regards.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_Regards_Regards.cpp$(DependSuffix): ../Regards/Regards.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_Regards_Regards.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_Regards_Regards.cpp$(DependSuffix) -MM ../Regards/Regards.cpp

$(IntermediateDirectory)/up_Regards_Regards.cpp$(PreprocessSuffix): ../Regards/Regards.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_Regards_Regards.cpp$(PreprocessSuffix) ../Regards/Regards.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/
	$(RM) ../include/Release/header.h.gch


