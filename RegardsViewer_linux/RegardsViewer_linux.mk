##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=RegardsViewer_linux
ConfigurationName      :=Debug
WorkspacePath          :=/home/figuinha/developpement/Regards
ProjectPath            :=/home/figuinha/developpement/Regards/RegardsViewer_linux
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=10/02/19
CodeLitePath           :=/home/figuinha/.codelite
LinkerName             :=ccache /usr/bin/x86_64-linux-gnu-g++
SharedObjectLinkerName :=ccache /usr/bin/x86_64-linux-gnu-g++ -shared -fPIC
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
Preprocessors          :=$(PreprocessorSwitch)VIEWER $(PreprocessorSwitch)OPENGL $(PreprocessorSwitch)FFMPEG $(PreprocessorSwitch)SDL2 $(PreprocessorSwitch)LIBBPG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="RegardsViewer_linux.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            := -Wl,-rpath,'$$ORIGIN' -Wl,-z,origin -lexpat $(shell ../libextern/wxWidgets-master/wx-config   --libs --unicode=yes) -lwebkitgtk-3.0 -Wl,--allow-multiple-definition 
IncludePath            := $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2 $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include  $(IncludeSwitch). $(IncludeSwitch)../libextern/fftw-3.3.8/api $(IncludeSwitch)../libextern/wxWidgets-master/src/png $(IncludeSwitch)../libextern/wxWidgets-master/src/tiff/libtiff $(IncludeSwitch)../libextern/wxWidgets-master/src/jpeg $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libTheme $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libextern/ffmpeg-4.1 $(IncludeSwitch)../libextern/glew-2.1.0/include $(IncludeSwitch)../libextern/SDL2-2.0.5/include $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libParameter $(IncludeSwitch)../libUtility $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libResource $(IncludeSwitch)../libSqlData $(IncludeSwitch)../libOpenGL $(IncludeSwitch)../libRenderBitmap $(IncludeSwitch)../libSqlEngine $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libFiltre $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../Otherlib/wxSVG/include $(IncludeSwitch)../libWindow $(IncludeSwitch)../libPrint $(IncludeSwitch)../libPicture $(IncludeSwitch)../libTheme $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libControl $(IncludeSwitch)../libIntro $(IncludeSwitch)../libDialog $(IncludeSwitch)../libViewer $(IncludeSwitch)../libExplorer $(IncludeSwitch)../libOpenCL $(IncludeSwitch)../libWindowMain $(IncludeSwitch)../libVideoFFMpeg 
IncludePCH             :=  -include ../include/Debug/header.h 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)Viewer $(LibrarySwitch)Intro $(LibrarySwitch)VideoFFMpeg $(LibrarySwitch)wxCheckTree $(LibrarySwitch)Control $(LibrarySwitch)Exif_regards $(LibrarySwitch)Email $(LibrarySwitch)Picture $(LibrarySwitch)FaceRecognition $(LibrarySwitch)Window $(LibrarySwitch)WindowMain $(LibrarySwitch)Geolocalisation $(LibrarySwitch)Utility $(LibrarySwitch)Dialog $(LibrarySwitch)Resource $(LibrarySwitch)SqlData $(LibrarySwitch)DataStructure $(LibrarySwitch)SqlEngine $(LibrarySwitch)Sqlite $(LibrarySwitch)Compress $(LibrarySwitch)Utility $(LibrarySwitch)heif_static $(LibrarySwitch)de265 $(LibrarySwitch)Picture $(LibrarySwitch)Dialog $(LibrarySwitch)Geolocalisation $(LibrarySwitch)RegardsMediaInfo $(LibrarySwitch)Exif_regards $(LibrarySwitch)mediainfo $(LibrarySwitch)Print $(LibrarySwitch)Filtre $(LibrarySwitch)VideoThumbnail $(LibrarySwitch)Theme $(LibrarySwitch)DataStructure $(LibrarySwitch)Utility $(LibrarySwitch)Resource $(LibrarySwitch)Parameter $(LibrarySwitch)RegardsRaw $(LibrarySwitch)RegardsCL $(LibrarySwitch)OpenCL $(LibrarySwitch)SqlData $(LibrarySwitch)CxImage $(LibrarySwitch)wxSVG $(LibrarySwitch)OpenGL $(LibrarySwitch)wx_gtk3u_gl-3.1 $(LibrarySwitch)GL $(LibrarySwitch)GLU $(LibrarySwitch)glut $(LibrarySwitch)GLEW $(LibrarySwitch)cairo $(LibrarySwitch)exiv2 $(LibrarySwitch)expat $(LibrarySwitch)Jbig $(LibrarySwitch)Psd $(LibrarySwitch)jasper $(LibrarySwitch)mng $(LibrarySwitch)turbojpeg $(LibrarySwitch)gomp $(LibrarySwitch)avformat $(LibrarySwitch)avcodec $(LibrarySwitch)avdevice $(LibrarySwitch)avfilter $(LibrarySwitch)avutil $(LibrarySwitch)swresample $(LibrarySwitch)swscale $(LibrarySwitch)xcb $(LibrarySwitch)xcb-shm $(LibrarySwitch)xcb-xfixes $(LibrarySwitch)xcb-shape $(LibrarySwitch)SM $(LibrarySwitch)xml2 $(LibrarySwitch)webp $(LibrarySwitch)SDL2 $(LibrarySwitch)FaceRecognition $(LibrarySwitch)va $(LibrarySwitch)va-x11 $(LibrarySwitch)va-drm $(LibrarySwitch)Xext $(LibrarySwitch)lzma $(LibrarySwitch)asound $(LibrarySwitch)sndio $(LibrarySwitch)Xv $(LibrarySwitch)IlmImf $(LibrarySwitch)IexMath $(LibrarySwitch)Half $(LibrarySwitch)curl $(LibrarySwitch)dl $(LibrarySwitch)gobject-2.0 $(LibrarySwitch)X11 $(LibrarySwitch)z $(LibrarySwitch)pango-1.0 $(LibrarySwitch)pangocairo-1.0 $(LibrarySwitch)wxjpeg-3.1 $(LibrarySwitch)wxpng-3.1 $(LibrarySwitch)wxtiff-3.1 $(LibrarySwitch)fftw3f $(LibrarySwitch)aom $(LibrarySwitch)exif 
ArLibs                 :=  "Viewer" "Intro" "VideoFFMpeg" "wxCheckTree" "Control" "Exif_regards" "Email" "Picture" "FaceRecognition" "Window" "WindowMain" "Geolocalisation" "Utility" "Dialog" "Resource" "SqlData" "DataStructure" "SqlEngine" "Sqlite" "Compress" "Utility" "heif_static" "de265" "Picture" "Dialog" "Geolocalisation" "RegardsMediaInfo" "Exif_regards" "mediainfo" "Print" "Filtre" "VideoThumbnail" "Theme" "DataStructure" "Utility" "Resource" "Parameter" "RegardsRaw" "RegardsCL" "OpenCL" "SqlData" "CxImage" "wxSVG" "OpenGL" "wx_gtk3u_gl-3.1.a" "GL" "GLU" "glut" "GLEW" "cairo" "exiv2" "expat" "Jbig" "Psd" "jasper" "mng" "turbojpeg" "gomp" "avformat" "avcodec" "avdevice" "avfilter" "avutil" "swresample" "swscale" "xcb" "xcb-shm" "xcb-xfixes" "xcb-shape" "SM" "xml2" "webp" "SDL2" "FaceRecognition" "va" "va-x11" "va-drm" "Xext" "lzma" "asound" "sndio" "Xv" "IlmImf" "IexMath" "Half" "curl" "dl" "gobject-2.0" "X11" "z" "pango-1.0" "pangocairo-1.0" "wxjpeg-3.1" "wxpng-3.1" "wxtiff-3.1" "fftw3f" "aom" "exif" 
LibPath                :=$(LibraryPathSwitch)/opt/AMDAPPSDK-3.0/lib/x86_64/sdk  $(LibraryPathSwitch)../libextern/jasper-1.900.1/src/libjasper/.libs $(LibraryPathSwitch)../libextern/fftw-3.3.8/.libs $(LibraryPathSwitch)../libextern/SDL2-2.0.5/build/.libs $(LibraryPathSwitch)../libextern/exiv2-0.26/src/.libs $(LibraryPathSwitch)../libextern/wxWidgets-master/lib $(LibraryPathSwitch)../libextern/libmng-2.0.3/.libs $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libavcodec $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libavformat $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libavdevice $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libavfilter $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libavutil $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libswresample $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libswscale $(LibraryPathSwitch)../libextern/libde265-master/libde265 $(LibraryPathSwitch)../libextern/heif-master/lib $(LibraryPathSwitch)../libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/GNU/Library/.libs $(LibraryPathSwitch)../libextern/x265_2.5/build/linux/8bit $(LibraryPathSwitch)../libPrint/Debug $(LibraryPathSwitch)../CxImage/Debug $(LibraryPathSwitch)../libOpenGL/Debug $(LibraryPathSwitch)../libMediaInfo/Debug $(LibraryPathSwitch)../wxCheckTree/Debug $(LibraryPathSwitch)../libSqlite/Debug $(LibraryPathSwitch)../libViewer/Debug $(LibraryPathSwitch)../libExplorer/Debug $(LibraryPathSwitch)../libIntro/Debug $(LibraryPathSwitch)../libSqlEngine/Debug $(LibraryPathSwitch)../libResource/Debug $(LibraryPathSwitch)../libCompress/Debug $(LibraryPathSwitch)../libDialog/Debug $(LibraryPathSwitch)../libDataStructure/Debug $(LibraryPathSwitch)../libPicture/Debug $(LibraryPathSwitch)../libUtility/Debug $(LibraryPathSwitch)../libGeolocalisation/Debug $(LibraryPathSwitch)../libSqlData/Debug $(LibraryPathSwitch)../libVideoThumbnail/Debug $(LibraryPathSwitch)../Otherlib/RawSpeed/Debug $(LibraryPathSwitch)../libFiltre/Debug $(LibraryPathSwitch)../libFiltreInterface/Debug $(LibraryPathSwitch)../libWindow/Debug $(LibraryPathSwitch)../libControl/Debug $(LibraryPathSwitch)../libTheme/Debug $(LibraryPathSwitch)../libParameter/Debug $(LibraryPathSwitch)../Otherlib/RegardsRaw/Debug $(LibraryPathSwitch)../libRenderBitmap/Debug $(LibraryPathSwitch)../libOpenCL/Debug $(LibraryPathSwitch)../libVideoFFMpeg/Debug $(LibraryPathSwitch)../libEmail/Debug $(LibraryPathSwitch)../libExif/Debug $(LibraryPathSwitch)../libWindowMain/Debug $(LibraryPathSwitch)../Otherlib/wxSVG/Debug $(LibraryPathSwitch)../Otherlib/libPsd/Debug $(LibraryPathSwitch)../Otherlib/libJbig/Debug $(LibraryPathSwitch)../FaceRecognition_unix/Debug $(LibraryPathSwitch)../Otherlib/libbpg/libRegardsBpg_unix/Debug $(LibraryPathSwitch)../libPiccante/Debug 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/x86_64-linux-gnu-ar rcu
CXX      := ccache /usr/bin/x86_64-linux-gnu-g++
CC       := ccache /usr/bin/x86_64-linux-gnu-gcc
CXXFLAGS := -fopenmp -Wall $(shell ../libextern/wxWidgets-master/wx-config --cppflags --libs) -std=gnu++11 -pthread -finstrument-functions -ggdb -O0 $(Preprocessors)
CFLAGS   :=  -Wall $(shell ../libextern/wxWidgets-master/wx-config --cppflags --libs) -ggdb -O0 $(Preprocessors)
ASFLAGS  := 
AS       := ccache /usr/bin/x86_64-linux-gnu-as


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

$(OutputFile): $(IntermediateDirectory)/.d "../.build-debug/include" "../.build-debug/libWindowMain" "../.build-debug/libWindow" "../.build-debug/libViewer" "../.build-debug/libVideoThumbnail" "../.build-debug/libVideoFFMpeg" "../.build-debug/libUtility" "../.build-debug/libTheme" "../.build-debug/libSqlite" "../.build-debug/libSqlEngine" "../.build-debug/libSqlData" "../.build-debug/libResource" "../.build-debug/libPrint" "../.build-debug/libPicture" "../.build-debug/libParameter" "../.build-debug/libOpenCL" "../.build-debug/libIntro" "../.build-debug/libGeolocalisation" "../.build-debug/libFiltre" "../.build-debug/libExif" "../.build-debug/libEmail" "../.build-debug/libDialog" "../.build-debug/libDataStructure" "../.build-debug/libCompress" "../.build-debug/RegardsRaw" "../.build-debug/CxImage" "../.build-debug/libControl" "../.build-debug/wxCheckTree" "../.build-debug/wxSVG" "../.build-debug/libMediaInfo" "../.build-debug/libPsd" "../.build-debug/libOpenGL" "../.build-debug/libJbig" "../.build-debug/libavutil" "../.build-debug/libbpg" "../.build-debug/libavcodec" "../.build-debug/FaceRecognition" "../.build-debug/FaceRecognition_unix" "../.build-debug/libRegardsBpg_unix" $(Objects) 
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


"../.build-debug/libavutil":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libavutil"


"../.build-debug/libbpg":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libbpg"


"../.build-debug/libavcodec":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libavcodec"


"../.build-debug/FaceRecognition":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/FaceRecognition"


"../.build-debug/FaceRecognition_unix":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/FaceRecognition_unix"


"../.build-debug/libRegardsBpg_unix":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/libRegardsBpg_unix"




PostBuild:
	@echo Executing Post Build commands ...
	cp -R ../Regards/Resources ./Debug
	cp ../libextern/x265_2.5/build/linux/8bit/libx265.so.130 ./Debug/libx265.so.130
	cp ../libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/GNU/Library/.libs/libmediainfo.so.0 ./Debug/libmediainfo.so.0
	cp ../Otherlib/libbpg/libRegardsBpg_unix/Debug/libRegardsBpg.so ./Debug/libRegardsBpg.so
	cp ../FaceRecognition_unix/Debug/libFaceRecognition.so ./Debug/libFaceRecognition.so
	cp ../libextern/exiv2-0.26/src/.libs/libexiv2.so.26 ./Debug/libexiv2.so.26
	cp ../libextern/libde265-master/libde265/libde265.so ./Debug/libde265.so
	@echo Done

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:

# PreCompiled Header
../include/Debug/header.h.gch: ../include/Debug/header.h
	$(CXX) $(SourceSwitch) ../include/Debug/header.h $(PCHCompileFlags) $(CXXFLAGS) $(IncludePath)



##
## Objects
##
$(IntermediateDirectory)/up_Regards_Regards.cpp$(ObjectSuffix): ../Regards/Regards.cpp $(IntermediateDirectory)/up_Regards_Regards.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Regards/Regards.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_Regards_Regards.cpp$(ObjectSuffix) $(IncludePath)
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


