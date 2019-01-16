##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=RegardsViewer_linux
ConfigurationName      :=Release
WorkspacePath          :=/home/figuinha/Developpement/Regards
ProjectPath            :=/home/figuinha/Developpement/Regards/RegardsViewer_linux
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=15/01/19
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
OutputFile             :=$(IntermediateDirectory)/RegardsViewer
Preprocessors          :=$(PreprocessorSwitch)VIEWER $(PreprocessorSwitch)OPENGL $(PreprocessorSwitch)FFMPEG $(PreprocessorSwitch)SDL2 $(PreprocessorSwitch)LIBBPG $(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="RegardsViewer_linux.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            := -Wl,-rpath,'$$ORIGIN' -Wl,-z,origin -lexpat $(shell ../libextern/wxWidgets-master/wx-config   --libs --unicode=yes) -lwebkitgtk-3.0 -Wl,--allow-multiple-definition 
IncludePath            := $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2  $(IncludeSwitch). $(IncludeSwitch)../libextern/fftw-3.3.8/api $(IncludeSwitch)../libextern/wxWidgets-master/src/png $(IncludeSwitch)../libextern/wxWidgets-master/src/tiff/libtiff $(IncludeSwitch)../libextern/wxWidgets-master/src/jpeg $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libTheme $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libextern/ffmpeg-4.1 $(IncludeSwitch)../libextern/glew-2.1.0/include $(IncludeSwitch)../libextern/SDL2-2.0.5/include $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libParameter $(IncludeSwitch)../libUtility $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libResource $(IncludeSwitch)../libSqlData $(IncludeSwitch)../libOpenGL $(IncludeSwitch)../libRenderBitmap $(IncludeSwitch)../libSqlEngine $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libFiltre $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../Otherlib/wxSVG/include $(IncludeSwitch)../libWindow $(IncludeSwitch)../libPrint $(IncludeSwitch)../libPicture $(IncludeSwitch)../libTheme $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libControl $(IncludeSwitch)../libIntro $(IncludeSwitch)../libDialog $(IncludeSwitch)../libViewer $(IncludeSwitch)../libExplorer $(IncludeSwitch)../libOpenCL $(IncludeSwitch)../libWindowMain $(IncludeSwitch)../libVideoFFMpeg 
IncludePCH             :=  -include ../include/Release/header.h 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)Viewer $(LibrarySwitch)Intro $(LibrarySwitch)VideoFFMpeg $(LibrarySwitch)wxCheckTree $(LibrarySwitch)Control $(LibrarySwitch)Exif_regards $(LibrarySwitch)Email $(LibrarySwitch)Picture $(LibrarySwitch)FaceRecognition $(LibrarySwitch)Window $(LibrarySwitch)WindowMain $(LibrarySwitch)Geolocalisation $(LibrarySwitch)Utility $(LibrarySwitch)Dialog $(LibrarySwitch)Resource $(LibrarySwitch)SqlData $(LibrarySwitch)DataStructure $(LibrarySwitch)SqlEngine $(LibrarySwitch)Sqlite $(LibrarySwitch)Compress $(LibrarySwitch)Utility $(LibrarySwitch)heif_static $(LibrarySwitch)de265 $(LibrarySwitch)Picture $(LibrarySwitch)Dialog $(LibrarySwitch)Geolocalisation $(LibrarySwitch)RegardsMediaInfo $(LibrarySwitch)Exif_regards $(LibrarySwitch)mediainfo $(LibrarySwitch)SqlData $(LibrarySwitch)Print $(LibrarySwitch)Filtre $(LibrarySwitch)VideoThumbnail $(LibrarySwitch)Theme $(LibrarySwitch)DataStructure $(LibrarySwitch)Utility $(LibrarySwitch)Resource $(LibrarySwitch)Parameter $(LibrarySwitch)RegardsRaw $(LibrarySwitch)RegardsCL $(LibrarySwitch)OpenCL $(LibrarySwitch)CxImage $(LibrarySwitch)wxSVG $(LibrarySwitch)OpenGL $(LibrarySwitch)wx_gtk3u_gl-3.1 $(LibrarySwitch)GL $(LibrarySwitch)GLU $(LibrarySwitch)glut $(LibrarySwitch)GLEW $(LibrarySwitch)cairo $(LibrarySwitch)exiv2 $(LibrarySwitch)expat $(LibrarySwitch)Jbig $(LibrarySwitch)Psd $(LibrarySwitch)jasper $(LibrarySwitch)mng $(LibrarySwitch)turbojpeg $(LibrarySwitch)gomp $(LibrarySwitch)avformat $(LibrarySwitch)avcodec $(LibrarySwitch)avdevice $(LibrarySwitch)avfilter $(LibrarySwitch)avutil $(LibrarySwitch)swresample $(LibrarySwitch)swscale $(LibrarySwitch)xcb $(LibrarySwitch)xcb-shm $(LibrarySwitch)xcb-xfixes $(LibrarySwitch)xcb-shape $(LibrarySwitch)SM $(LibrarySwitch)xml2 $(LibrarySwitch)webp $(LibrarySwitch)SDL2 $(LibrarySwitch)FaceRecognition $(LibrarySwitch)va $(LibrarySwitch)va-x11 $(LibrarySwitch)va-drm $(LibrarySwitch)Xext $(LibrarySwitch)lzma $(LibrarySwitch)asound $(LibrarySwitch)sndio $(LibrarySwitch)Xv $(LibrarySwitch)IlmImf $(LibrarySwitch)IexMath $(LibrarySwitch)Half $(LibrarySwitch)curl $(LibrarySwitch)dl $(LibrarySwitch)gobject-2.0 $(LibrarySwitch)X11 $(LibrarySwitch)z $(LibrarySwitch)pango-1.0 $(LibrarySwitch)pangocairo-1.0 $(LibrarySwitch)wxjpeg-3.1 $(LibrarySwitch)wxpng-3.1 $(LibrarySwitch)wxtiff-3.1 $(LibrarySwitch)fftw3f $(LibrarySwitch)aom $(LibrarySwitch)exif 
ArLibs                 :=  "Viewer" "Intro" "VideoFFMpeg" "wxCheckTree" "Control" "Exif_regards" "Email" "Picture" "FaceRecognition" "Window" "WindowMain" "Geolocalisation" "Utility" "Dialog" "Resource" "SqlData" "DataStructure" "SqlEngine" "Sqlite" "Compress" "Utility" "heif_static" "de265" "Picture" "Dialog" "Geolocalisation" "RegardsMediaInfo" "Exif_regards" "mediainfo" "SqlData" "Print" "Filtre" "VideoThumbnail" "Theme" "DataStructure" "Utility" "Resource" "Parameter" "RegardsRaw" "RegardsCL" "OpenCL" "CxImage" "wxSVG" "OpenGL" "wx_gtk3u_gl-3.1.a" "GL" "GLU" "glut" "GLEW" "cairo" "exiv2" "expat" "Jbig" "Psd" "jasper" "mng" "turbojpeg" "gomp" "avformat" "avcodec" "avdevice" "avfilter" "avutil" "swresample" "swscale" "xcb" "xcb-shm" "xcb-xfixes" "xcb-shape" "SM" "xml2" "webp" "SDL2" "FaceRecognition" "va" "va-x11" "va-drm" "Xext" "lzma" "asound" "sndio" "Xv" "IlmImf" "IexMath" "Half" "curl" "dl" "gobject-2.0" "X11" "z" "pango-1.0" "pangocairo-1.0" "wxjpeg-3.1" "wxpng-3.1" "wxtiff-3.1" "fftw3f" "aom" "exif" 
LibPath                :=$(LibraryPathSwitch)/opt/AMDAPPSDK-3.0/lib  $(LibraryPathSwitch)../libextern/jasper-1.900.1/src/libjasper/.libs $(LibraryPathSwitch)../libextern/fftw-3.3.8/.libs $(LibraryPathSwitch)../libextern/SDL2-2.0.5/build/.libs $(LibraryPathSwitch)../libextern/exiv2-0.26/src/.libs $(LibraryPathSwitch)../libextern/wxWidgets-master/lib $(LibraryPathSwitch)../libextern/libmng-2.0.3/.libs $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libavcodec $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libavformat $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libavdevice $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libavfilter $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libavutil $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libswresample $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libswscale $(LibraryPathSwitch)../libextern/libde265-master/libde265 $(LibraryPathSwitch)../libextern/heif-master/srcs/lib $(LibraryPathSwitch)../libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/GNU/Library/.libs $(LibraryPathSwitch)../libextern/x265_2.5/build/linux/8bit $(LibraryPathSwitch)../libPrint/Release $(LibraryPathSwitch)../CxImage/Release $(LibraryPathSwitch)../libOpenGL/Release $(LibraryPathSwitch)../libMediaInfo/Release $(LibraryPathSwitch)../wxCheckTree/Release $(LibraryPathSwitch)../libSqlite/Release $(LibraryPathSwitch)../libViewer/Release $(LibraryPathSwitch)../libExplorer/Release $(LibraryPathSwitch)../libIntro/Release $(LibraryPathSwitch)../libSqlEngine/Release $(LibraryPathSwitch)../libResource/Release $(LibraryPathSwitch)../libCompress/Release $(LibraryPathSwitch)../libDialog/Release $(LibraryPathSwitch)../libDataStructure/Release $(LibraryPathSwitch)../libPicture/Release $(LibraryPathSwitch)../libUtility/Release $(LibraryPathSwitch)../libGeolocalisation/Release $(LibraryPathSwitch)../libSqlData/Release $(LibraryPathSwitch)../libVideoThumbnail/Release $(LibraryPathSwitch)../Otherlib/RawSpeed/Release $(LibraryPathSwitch)../libFiltre/Release $(LibraryPathSwitch)../libWindow/Release $(LibraryPathSwitch)../libControl/Release $(LibraryPathSwitch)../libTheme/Release $(LibraryPathSwitch)../libParameter/Release $(LibraryPathSwitch)../Otherlib/RegardsRaw/Release $(LibraryPathSwitch)../libRenderBitmap/Release $(LibraryPathSwitch)../libOpenCL/Release $(LibraryPathSwitch)../libVideoFFMpeg/Release $(LibraryPathSwitch)../libEmail/Release $(LibraryPathSwitch)../libExif/Release $(LibraryPathSwitch)../libWindowMain/Release $(LibraryPathSwitch)../Otherlib/wxSVG/Release $(LibraryPathSwitch)../Otherlib/libPsd/Release $(LibraryPathSwitch)../Otherlib/libJbig/Release $(LibraryPathSwitch)../Otherlib/libbpg/libRegardsBpg_unix/Release $(LibraryPathSwitch)../FaceRecognition_unix/Release $(LibraryPathSwitch)../libPiccante/Release 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/ccache /usr/bin/g++
CC       := /usr/bin/ccache /usr/bin/gcc
CXXFLAGS := -fopenmp -Wall $(shell ../libextern/wxWidgets-master/wx-config --cppflags --libs) -std=gnu++11 -pthread -finstrument-functions -O2 -msse4.1 $(Preprocessors)
CFLAGS   :=  -Wall $(shell ../libextern/wxWidgets-master/wx-config --cppflags --libs) -O2 -msse4.1 $(Preprocessors)
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

$(OutputFile): $(IntermediateDirectory)/.d "../.build-release/include" "../.build-release/CxImage" "../.build-release/RegardsRaw" "../.build-release/libCompress" "../.build-release/libControl" "../.build-release/libWindowMain" "../.build-release/libWindow" "../.build-release/libViewer" "../.build-release/libVideoThumbnail" "../.build-release/libVideoFFMpeg" "../.build-release/libUtility" "../.build-release/libSqlEngine" "../.build-release/libSqlite" "../.build-release/libTheme" "../.build-release/libSqlData" "../.build-release/libResource" "../.build-release/libPrint" "../.build-release/libPicture" "../.build-release/libOpenCL" "../.build-release/libParameter" "../.build-release/libIntro" "../.build-release/libGeolocalisation" "../.build-release/libFiltre" "../.build-release/libExif" "../.build-release/libEmail" "../.build-release/libDialog" "../.build-release/libDataStructure" "../.build-release/wxCheckTree" "../.build-release/wxSVG" "../.build-release/libOpenGL" "../.build-release/libMediaInfo" "../.build-release/libJbig" "../.build-release/libPsd" "../.build-release/libavutil" "../.build-release/libbpg" "../.build-release/libavcodec" "../.build-release/libRegardsBpg_unix" "../.build-release/FaceRecognition" "../.build-release/FaceRecognition_unix" $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

"../.build-release/include":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/include"


"../.build-release/CxImage":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/CxImage"


"../.build-release/RegardsRaw":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/RegardsRaw"


"../.build-release/libCompress":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libCompress"


"../.build-release/libControl":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libControl"


"../.build-release/libWindowMain":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libWindowMain"


"../.build-release/libWindow":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libWindow"


"../.build-release/libViewer":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libViewer"


"../.build-release/libVideoThumbnail":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libVideoThumbnail"


"../.build-release/libVideoFFMpeg":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libVideoFFMpeg"


"../.build-release/libUtility":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libUtility"


"../.build-release/libSqlEngine":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libSqlEngine"


"../.build-release/libSqlite":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libSqlite"


"../.build-release/libTheme":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libTheme"


"../.build-release/libSqlData":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libSqlData"


"../.build-release/libResource":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libResource"


"../.build-release/libPrint":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libPrint"


"../.build-release/libPicture":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libPicture"


"../.build-release/libOpenCL":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libOpenCL"


"../.build-release/libParameter":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libParameter"


"../.build-release/libIntro":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libIntro"


"../.build-release/libGeolocalisation":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libGeolocalisation"


"../.build-release/libFiltre":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libFiltre"


"../.build-release/libExif":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libExif"


"../.build-release/libEmail":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libEmail"


"../.build-release/libDialog":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libDialog"


"../.build-release/libDataStructure":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libDataStructure"


"../.build-release/wxCheckTree":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/wxCheckTree"


"../.build-release/wxSVG":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/wxSVG"


"../.build-release/libOpenGL":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libOpenGL"


"../.build-release/libMediaInfo":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libMediaInfo"


"../.build-release/libJbig":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libJbig"


"../.build-release/libPsd":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libPsd"


"../.build-release/libavutil":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libavutil"


"../.build-release/libbpg":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libbpg"


"../.build-release/libavcodec":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libavcodec"


"../.build-release/libRegardsBpg_unix":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/libRegardsBpg_unix"


"../.build-release/FaceRecognition":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/FaceRecognition"


"../.build-release/FaceRecognition_unix":
	@$(MakeDirCommand) "../.build-release"
	@echo stam > "../.build-release/FaceRecognition_unix"




PostBuild:
	@echo Executing Post Build commands ...
	cp -R ../Regards/Resources ./Release
	cp ../libextern/x265_2.5/build/linux/libx265.so.130 ./Release/libx265.so.130
	cp ../libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/GNU/Library/.libs/libmediainfo.so.0 ./Release/libmediainfo.so.0
	cp ../Otherlib/libbpg/libRegardsBpg_unix/Release/libRegardsBpg.so ./Release/libRegardsBpg.so
	cp ../FaceRecognition_unix/Release/libFaceRecognition.so ./Release/libFaceRecognition.so
	cp ../libextern/exiv2-0.26/src/.libs/libexiv2.so.26 ./Release/libexiv2.so.26
	cp ../libextern/libde265-master/libde265/libde265.so ./Release/libde265.so
	@echo Done

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


$(IntermediateDirectory)/.d:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:

# PreCompiled Header
../include/Release/header.h.gch: ../include/Release/header.h
	$(CXX) $(SourceSwitch) ../include/Release/header.h $(PCHCompileFlags) $(CXXFLAGS) $(IncludePath)



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
	$(RM) -r ./Release/
	$(RM) ../include/Release/header.h.gch


