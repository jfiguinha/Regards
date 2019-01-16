##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=RegardsViewer_mac
ConfigurationName      :=Release
WorkspacePath          :=/Users/jacques/developpement/codelite/Regards
ProjectPath            :=/Users/jacques/developpement/codelite/Regards/RegardsViewer_mac
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha jacques
Date                   :=11/01/2019
CodeLitePath           :="/Users/jacques/Library/Application Support/codelite"
LinkerName             :=/usr/local/opt/ccache/bin/ccache /usr/local/opt/llvm/bin/clang++ -stdlib=libc++ -mmacosx-version-min=10.10
SharedObjectLinkerName :=/usr/local/opt/ccache/bin/ccache /usr/local/opt/llvm/bin/clang++ -dynamiclib -fPIC
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
ObjectsFileList        :="RegardsViewer_mac.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            := $(shell ../libextern/wxWidgets-master/wx-config   --libs --unicode=yes) -headerpad_max_install_names -framework IOKit -framework Carbon -framework Cocoa -framework AudioToolbox -framework System -framework OpenCL -framework CoreAudio -framework CoreVideo -framework AudioUnit -framework AVKit -framework ForceFeedback -framework WebKit -framework security -framework VideoToolbox -framework AVFoundation -framework VideoDecodeAcceleration -framework CoreMedia -framework CoreImage -framework OpenGL -framework GLUT 
IncludePath            := $(IncludeSwitch)/usr/local/opt/cairo/include $(IncludeSwitch)/usr/local/opt/openexr/include/OpenEXR $(IncludeSwitch)/usr/local/opt/ilmbase/include/openexr $(IncludeSwitch)/usr/local/opt/libxml2/include/libxml2 $(IncludeSwitch)/usr/local/opt/libexif/include  $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libTheme $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libextern/ffmpeg-4.1 $(IncludeSwitch)../libextern/glew-2.1.0/include $(IncludeSwitch)../libextern/SDL2-2.0.5/include $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libParameter $(IncludeSwitch)../libUtility $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libResource $(IncludeSwitch)../libSqlData $(IncludeSwitch)../libOpenGL $(IncludeSwitch)../libRenderBitmap $(IncludeSwitch)../libSqlEngine $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libFiltre $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../Otherlib/wxSVG/include $(IncludeSwitch)../libWindow $(IncludeSwitch)../libPrint $(IncludeSwitch)../libPicture $(IncludeSwitch)../libTheme $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libControl $(IncludeSwitch)../libIntro $(IncludeSwitch)../libDialog $(IncludeSwitch)../libViewer $(IncludeSwitch)../libExplorer $(IncludeSwitch)../libOpenCL $(IncludeSwitch)../libWindowMain $(IncludeSwitch)../libVideoFFMpeg 
IncludePCH             :=  -include ../include/Release/header.h 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)Viewer $(LibrarySwitch)Intro $(LibrarySwitch)VideoFFMpeg $(LibrarySwitch)wxCheckTree $(LibrarySwitch)Control $(LibrarySwitch)Email $(LibrarySwitch)Exif_regards $(LibrarySwitch)Picture $(LibrarySwitch)FaceRecognition $(LibrarySwitch)Window $(LibrarySwitch)WindowMain $(LibrarySwitch)Geolocalisation $(LibrarySwitch)Utility $(LibrarySwitch)Dialog $(LibrarySwitch)Resource $(LibrarySwitch)SqlData $(LibrarySwitch)DataStructure $(LibrarySwitch)SqlEngine $(LibrarySwitch)Sqlite $(LibrarySwitch)dl $(LibrarySwitch)z $(LibrarySwitch)expat $(LibrarySwitch)z $(LibrarySwitch)Compress $(LibrarySwitch)Utility $(LibrarySwitch)Picture $(LibrarySwitch)VideoThumbnail $(LibrarySwitch)Dialog $(LibrarySwitch)Geolocalisation $(LibrarySwitch)Exif_regards $(LibrarySwitch)RegardsMediaInfo $(LibrarySwitch)mediainfo $(LibrarySwitch)SqlData $(LibrarySwitch)Print $(LibrarySwitch)Filtre $(LibrarySwitch)Theme $(LibrarySwitch)DataStructure $(LibrarySwitch)Utility $(LibrarySwitch)Resource $(LibrarySwitch)Parameter $(LibrarySwitch)CxImage $(LibrarySwitch)wxSVG $(LibrarySwitch)RegardsRaw $(LibrarySwitch)wx_osx_cocoau_gl-3.1 $(LibrarySwitch)GLEW $(LibrarySwitch)OpenGL $(LibrarySwitch)RegardsCL $(LibrarySwitch)exiv2 $(LibrarySwitch)cairo $(LibrarySwitch)iconv $(LibrarySwitch)expat $(LibrarySwitch)jasper $(LibrarySwitch)Jbig $(LibrarySwitch)mng $(LibrarySwitch)gomp $(LibrarySwitch)avformat $(LibrarySwitch)avcodec $(LibrarySwitch)swresample $(LibrarySwitch)avdevice $(LibrarySwitch)avfilter $(LibrarySwitch)avcodec $(LibrarySwitch)avutil $(LibrarySwitch)swresample $(LibrarySwitch)swscale $(LibrarySwitch)glib-2.0 $(LibrarySwitch)Psd $(LibrarySwitch)turbojpeg $(LibrarySwitch)xml2 $(LibrarySwitch)webp $(LibrarySwitch)lzma $(LibrarySwitch)SDL2 $(LibrarySwitch)z $(LibrarySwitch)bz2 $(LibrarySwitch)heif_static $(LibrarySwitch)de265 $(LibrarySwitch)dl $(LibrarySwitch)curl $(LibrarySwitch)AppleFunction $(LibrarySwitch)IlmImf $(LibrarySwitch)IexMath $(LibrarySwitch)Half $(LibrarySwitch)wxjpeg-3.1 $(LibrarySwitch)wxpng-3.1 $(LibrarySwitch)wxtiff-3.1 $(LibrarySwitch)fftw3f $(LibrarySwitch)aom $(LibrarySwitch)exif 
ArLibs                 :=  "Viewer" "Intro" "VideoFFMpeg" "wxCheckTree" "Control" "Email" "Exif_regards" "Picture" "FaceRecognition" "Window" "WindowMain" "Geolocalisation" "Utility" "Dialog" "Resource" "SqlData" "DataStructure" "SqlEngine" "Sqlite" "dl" "z" "expat" "z" "Compress" "Utility" "Picture" "VideoThumbnail" "Dialog" "Geolocalisation" "Exif_regards" "RegardsMediaInfo" "mediainfo" "SqlData" "Print" "Filtre" "Theme" "DataStructure" "Utility" "Resource" "Parameter" "CxImage" "wxSVG" "RegardsRaw" "wx_osx_cocoau_gl-3.1" "GLEW" "OpenGL" "RegardsCL" "exiv2" "cairo" "iconv" "expat" "jasper" "Jbig" "mng" "gomp" "avformat" "avcodec" "swresample" "avdevice" "avfilter" "avcodec" "avutil" "swresample" "swscale" "glib-2.0" "Psd" "turbojpeg" "xml2" "webp" "lzma" "SDL2" "z" "bz2" "heif_static" "de265" "dl" "curl" "AppleFunction" "IlmImf" "IexMath" "Half" "wxjpeg-3.1" "wxpng-3.1" "wxtiff-3.1" "fftw3f" "aom" "exif" 
LibPath                :=$(LibraryPathSwitch)/usr/local/opt/llvm/lib $(LibraryPathSwitch)/usr/local/opt/openexr/lib $(LibraryPathSwitch)/usr/local/opt/ilmbase/lib $(LibraryPathSwitch)/usr/local/opt/glib/lib $(LibraryPathSwitch)/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/lib $(LibraryPathSwitch)/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library $(LibraryPathSwitch)/usr/lib/system  $(LibraryPathSwitch)../libextern/jasper-1.900.1/src/libjasper/.libs $(LibraryPathSwitch)../libextern/SDL2-2.0.5/build/.libs $(LibraryPathSwitch)../libextern/exiv2-0.26/src/.libs $(LibraryPathSwitch)../libextern/wxWidgets-master/lib $(LibraryPathSwitch)../libextern/libmng-2.0.3/.libs $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libavcodec $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libavformat $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libavdevice $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libavfilter $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libavutil $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libswresample $(LibraryPathSwitch)../libextern/ffmpeg-4.1/libswscale $(LibraryPathSwitch)../libextern/libde265-master/libde265 $(LibraryPathSwitch)../libextern/heif-master/lib $(LibraryPathSwitch)../libextern/libwebp-0.6.0 $(LibraryPathSwitch)../libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/GNU/Library/.libs $(LibraryPathSwitch)../libextern/libjpeg-turbo-1.5.1/.libs $(LibraryPathSwitch)../libextern/glew-2.1.0/lib $(LibraryPathSwitch)../libextern/fftw-3.3.8/.libs $(LibraryPathSwitch)../libPrint/Release $(LibraryPathSwitch)../CxImage/Release $(LibraryPathSwitch)../libOpenGL/Release $(LibraryPathSwitch)../libMediaInfo/Release $(LibraryPathSwitch)../wxCheckTree/Release $(LibraryPathSwitch)../libSqlite/Release $(LibraryPathSwitch)../libViewer/Release $(LibraryPathSwitch)../libIntro/Release $(LibraryPathSwitch)../libSqlEngine/Release $(LibraryPathSwitch)../libResource/Release $(LibraryPathSwitch)../libCompress/Release $(LibraryPathSwitch)../libDialog/Release $(LibraryPathSwitch)../libDataStructure/Release $(LibraryPathSwitch)../libPicture/Release $(LibraryPathSwitch)../libUtility/Release $(LibraryPathSwitch)../libGeolocalisation/Release $(LibraryPathSwitch)../libSqlData/Release $(LibraryPathSwitch)../libVideoThumbnail/Release $(LibraryPathSwitch)../libFiltre/Release $(LibraryPathSwitch)../libWindow/Release $(LibraryPathSwitch)../libControl/Release $(LibraryPathSwitch)../libTheme/Release $(LibraryPathSwitch)../libParameter/Release $(LibraryPathSwitch)../Otherlib/RegardsRaw/Release $(LibraryPathSwitch)../libOpenCL/Release $(LibraryPathSwitch)../libVideoFFMpeg/Release $(LibraryPathSwitch)../libEmail/Release $(LibraryPathSwitch)../libExif/Release $(LibraryPathSwitch)../libWindowMain/Release $(LibraryPathSwitch)../Otherlib/wxSVG/Release $(LibraryPathSwitch)../Otherlib/libPsd/Release $(LibraryPathSwitch)../Otherlib/libJbig/Release $(LibraryPathSwitch)../FaceRecognition_mac/Release $(LibraryPathSwitch)../libAppleFunction/Release $(LibraryPathSwitch)../Otherlib/RawSpeed/Release 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/local/opt/ccache/bin/ccache /usr/local/opt/llvm/bin/clang++ -stdlib=libc++ -mmacosx-version-min=10.10 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/
CC       := /usr/local/opt/ccache/bin/ccache /usr/local/opt/llvm/bin/clang -mmacosx-version-min=10.10 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/
CXXFLAGS := -fopenmp -std=gnu++11 $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread -O2 -msse4.1 $(Preprocessors)
CFLAGS   :=  -Wall  -O2 -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := /usr//bin/as


##
## User defined environment variables
##
CodeLiteDir:=/Applications/codelite.app/Contents/SharedSupport/
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
	$(LinkerName) $(OutputSwitch)$(OutputFile) $(Objects) $(LibPath) $(Libs) $(LinkOptions)

PostBuild:
	@echo Executing Post Build commands ...
	SetFile -t APPL ./Release/RegardsViewer
	mkdir -p ./Release/RegardsViewer.app/Contents
	mkdir -p ./Release/RegardsViewer.app/Contents/Frameworks
	mkdir -p ./Release/RegardsViewer.app/Contents/MacOS
	mkdir -p ./Release/RegardsViewer.app/Contents/Resources
	sed -e "s/IDENTIFIER/`echo . | sed -e 's,\.\./,,g' | sed -e 's,/,.,g'`/" \
	-e "s/EXECUTABLE/RegardsViewer/" \
	-e "s/VERSION/2.18.1/" \
	../Regards/mac/Info.plist.in >./Release/RegardsViewer.app/Contents/Info.plist
	/bin/echo "APPL????" >./Release/RegardsViewer.app/Contents/PkgInfo
	ln -f ./Release/RegardsViewer ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
	cp -f ../Regards/mac/wxmac.icns ./Release/RegardsViewer.app/Contents/Resources/wxmac.icns
	cp -R ../Regards/Resources ./Release/RegardsViewer.app/Contents/
	cp ../Resource/theme/RegardsViewer.viewer.theme ./Release/RegardsViewer.app/Contents/MacOs/RegardsViewer.viewer.theme
	
	cp ../libextern/x265_2.5/source/libx265.130.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libx265.130.dylib
	cp ../libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Project/GNU/Library/.libs/libmediainfo.0.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libmediainfo.0.dylib
	cp ../Otherlib/libbpg/libRegardsBpg_mac/Release/libRegardsBpg.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libRegardsBpg.dylib
	cp ../libextern/libmng-2.0.3/.libs/libmng.2.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libmng.2.dylib
	cp ../FaceRecognition_mac/Release/libFaceRecognition.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libFaceRecognition.dylib
	cp ../libextern/exiv2-0.26/src/.libs/libexiv2.26.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libexiv2.26.dylib
	cp ../libextern/libde265-master/libde265/libde265.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libde265.dylib
	cp ../libextern/libjpeg-turbo-1.5.1/.libs/libturbojpeg.0.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libturbojpeg.0.dylib
	cp ../libextern/SDL2-2.0.5/build/.libs/libSDL2-2.0.0.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libSDL2-2.0.0.dylib
	cp /usr/local/opt/cairo/lib/libcairo.2.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libcairo.2.dylib
	cp /usr/local/opt/llvm/lib/libomp.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libomp.dylib
	cp /usr/local/opt/glib/lib/libglib-2.0.0.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libglib-2.0.0.dylib
	cp /usr/local/opt/llvm/lib/libc++.1.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libc++.1.dylib
	cp /usr/local/opt/openexr/lib/libIlmImf-2_2.22.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libIlmImf-2_2.22.dylib
	cp /usr/local/opt/ilmbase/lib/libIexMath-2_2.23.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libIexMath-2_2.23.dylib
	cp /usr/local/opt/ilmbase/lib/libIMath-2_2.23.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libIMath-2_2.23.dylib
	cp /usr/local/opt/ilmbase/lib/libIlmThread-2_2.23.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libIlmThread-2_2.23.dylib
	cp /usr/local/opt/ilmbase/lib/libIex-2_2.23.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libIex-2_2.23.dylib
	cp /usr/local/opt/ilmbase/lib/libHalf.23.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libHalf.23.dylib
	cp /usr/local/opt/libexif/lib/libexif.12.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libexif.12.dylib
	cp ../libextern/glew-2.1.0/lib/libGLEW.2.1.0.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libGLEW.2.1.0.dylib
	cp ../libextern/libaom/libaom.0.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libaom.0.dylib
	cp ../libextern/wxWidgets-master/lib/libwx_osx_cocoau-3.1.1.0.0.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libwx_osx_cocoau-3.1.1.0.0.dylib
	cp ../libextern/wxWidgets-master/lib/libwx_osx_cocoau_gl-3.1.1.0.0.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libwx_osx_cocoau_gl-3.1.1.0.0.dylib
	chmod ugo+rw ./Release/RegardsViewer.app/Contents/Frameworks/libcairo.2.dylib
	chmod ugo+rw ./Release/RegardsViewer.app/Contents/Frameworks/libomp.dylib
	chmod ugo+rw ./Release/RegardsViewer.app/Contents/Frameworks/libglib-2.0.0.dylib
	chmod ugo+rw ./Release/RegardsViewer.app/Contents/Frameworks/libc++.1.dylib
	chmod ugo+rw ./Release/RegardsViewer.app/Contents/Frameworks/libIlmImf-2_2.22.dylib
	chmod ugo+rw ./Release/RegardsViewer.app/Contents/Frameworks/libIexMath-2_2.23.dylib
	chmod ugo+rw ./Release/RegardsViewer.app/Contents/Frameworks/libIMath-2_2.23.dylib
	chmod ugo+rw ./Release/RegardsViewer.app/Contents/Frameworks/libIex-2_2.23.dylib
	chmod ugo+rw ./Release/RegardsViewer.app/Contents/Frameworks/libHalf.23.dylib
	chmod ugo+rw ./Release/RegardsViewer.app/Contents/Frameworks/libIlmThread-2_2.23.dylib
	chmod ugo+rw ./Release/RegardsViewer.app/Contents/Frameworks/libexif.12.dylib
	chmod ugo+rw ./Release/RegardsViewer.app/Contents/Frameworks/libaom.0.dylib
	chmod ugo+rw ./Release/RegardsViewer.app/Contents/Frameworks/libwx_osx_cocoau-3.1.1.0.0.dylib
	chmod ugo+rw ./Release/RegardsViewer.app/Contents/Frameworks/libwx_osx_cocoau_gl-3.1.1.0.0.dylib
	
	install_name_tool -change /Users/jacques/developpement/codelite/Regards/libextern/wxWidgets-master/lib/libwx_osx_cocoau_gl-3.1.1.0.0.dylib @executable_path/../Frameworks/libwx_osx_cocoau_gl-3.1.1.0.0.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
	install_name_tool -change /Users/jacques/developpement/codelite/Regards/libextern/wxWidgets-master/lib/libwx_osx_cocoau-3.1.1.0.0.dylib @executable_path/../Frameworks/libwx_osx_cocoau-3.1.1.0.0.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
	install_name_tool -change /usr/local/opt/libexif/lib/libexif.12.dylib @executable_path/../Frameworks/libexif.12.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
	install_name_tool -change @rpath/libaom.0.dylib @executable_path/../Frameworks/libaom.0.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
	install_name_tool -change /usr/local/lib/libGLEW.2.1.0.dylib @executable_path/../Frameworks/libGLEW.2.1.0.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
	install_name_tool -change ./Release/libFaceRecognition.dylib @executable_path/../Frameworks/libFaceRecognition.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
	install_name_tool -change /usr/local/lib/libmediainfo.0.dylib @executable_path/../Frameworks/libmediainfo.0.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
	install_name_tool -change /usr/local/lib/libexiv2.26.dylib @executable_path/../Frameworks/libexiv2.26.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
	install_name_tool -change /usr/local/lib/libmng.2.dylib @executable_path/../Frameworks/libmng.2.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
	install_name_tool -change /usr/local/lib/libSDL2-2.0.0.dylib @executable_path/../Frameworks/libSDL2-2.0.0.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
	install_name_tool -change @rpath/libx265.130.dylib @executable_path/../Frameworks/libx265.130.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libRegardsBpg.dylib
	install_name_tool -change /Users/jacques/developpement/codelite/Regards/libextern/libde265-master/libde265/libde265.dylib @executable_path/../Frameworks/libde265.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
	install_name_tool -change /opt/libjpeg-turbo/lib/libturbojpeg.0.dylib @executable_path/../Frameworks/libturbojpeg.0.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
	chmod ugo+rw ./Release/RegardsViewer.app/Contents/Frameworks/libIexMath-2_2.23.dylib
	install_name_tool -change /usr/local/Cellar/ilmbase/2.2.1/lib/libIex-2_2.23.dylib @executable_path/../Frameworks/libIex-2_2.23.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libIexMath-2_2.23.dylib
	install_name_tool -change /usr/local/lib/libcairo.2.dylib @executable_path/../Frameworks/libcairo.2.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
	install_name_tool -change /usr/local/opt/llvm/lib/libomp.dylib @executable_path/../Frameworks/libomp.dylib ./Release/RegardsViewer
	install_name_tool -change /usr/local/opt/glib/lib/libglib-2.0.0.dylib @executable_path/../Frameworks/libglib-2.0.0.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
	install_name_tool -change /usr/local/opt/llvm/lib/libc++.1.dylib @executable_path/../Frameworks/libc++.1.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
	install_name_tool -change /usr/local/opt/openexr/lib/libIlmImf-2_2.22.dylib @executable_path/../Frameworks/libIlmImf-2_2.22.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
	install_name_tool -change /usr/local/opt/ilmbase/lib/libIexMath-2_2.23.dylib @executable_path/../Frameworks/libIexMath-2_2.23.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
	install_name_tool -change /usr/local/opt/ilmbase/lib/libHalf.23.dylib @executable_path/../Frameworks/libHalf.23.dylib ./Release/RegardsViewer.app/Contents/MacOS/RegardsViewer
	install_name_tool -change /usr/local/Cellar/ilmbase/2.2.1/lib/libIex-2_2.23.dylib @executable_path/../Frameworks/libIex-2_2.23.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libIexMath-2_2.23.dylib
	install_name_tool -change /usr/local/Cellar/ilmbase/2.2.1/lib/libIlmImf-2_2.22.dylib @executable_path/../Frameworks/libIex-2_2.23.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libIlmImf-2_2.22.dylib
	install_name_tool -change /usr/local/opt/ilmbase/lib/libImath-2_2.23.dylib @executable_path/../Frameworks/libImath-2_2.23.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libIlmImf-2_2.22.dylib
	install_name_tool -change /usr/local/opt/ilmbase/lib/libHalf.23.dylib @executable_path/../Frameworks/libHalf.23.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libIlmImf-2_2.22.dylib
	install_name_tool -change /usr/local/opt/ilmbase/lib/libIex-2_2.23.dylib @executable_path/../Frameworks/libIex-2_2.23.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libIlmImf-2_2.22.dylib
	install_name_tool -change /usr/local/opt/ilmbase/lib/libIexMath-2_2.23.dylib @executable_path/../Frameworks/libIexMath-2_2.23.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libIlmImf-2_2.22.dylib
	install_name_tool -change /usr/local/opt/ilmbase/lib/libIlmThread-2_2.23.dylib @executable_path/../Frameworks/libIlmThread-2_2.23.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libIlmImf-2_2.22.dylib
	install_name_tool -change /usr/local/Cellar/ilmbase/2.2.1/lib/libIex-2_2.23.dylib @executable_path/../Frameworks/libIex-2_2.23.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libImath-2_2.23.dylib
	install_name_tool -change /usr/local/Cellar/ilmbase/2.2.1/lib/libIex-2_2.23.dylib @executable_path/../Frameworks/libIex-2_2.23.dylib ./Release/RegardsViewer.app/Contents/Frameworks/libIlmThread-2_2.23.dylib
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
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/codelite/Regards/Regards/Regards.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_Regards_Regards.cpp$(ObjectSuffix) $(IncludePath)
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


