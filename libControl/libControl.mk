##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libControl
ConfigurationName      :=Release
WorkspacePath          :=/Users/jacques/developpement/git/Regards
ProjectPath            :=/Users/jacques/developpement/git/Regards/libControl
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha jacques
Date                   :=10/06/2021
CodeLitePath           :="/Users/jacques/Library/Application Support/CodeLite"
LinkerName             :=/usr/bin/clang++ -arch arm64
SharedObjectLinkerName :=/usr/bin/clang++ -dynamiclib -fPIC -arch arm64
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
OutputFile             :=$(IntermediateDirectory)/$(ProjectName).a
Preprocessors          :=$(PreprocessorSwitch)OPENGL $(PreprocessorSwitch)GLEW_STATIC $(PreprocessorSwitch)DXVA2 $(PreprocessorSwitch)OPENCL $(PreprocessorSwitch)YUV $(PreprocessorSwitch)SDL2 $(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libControl.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s 
IncludePath            := $(IncludeSwitch)/opt/homebrew/include $(IncludeSwitch)/opt/homebrew/include/poppler/cpp $(IncludeSwitch)/opt/homebrew/include/SDL2 $(IncludeSwitch)/opt/homebrew/include/openjpeg-2.4 $(IncludeSwitch)/opt/homebrew/include/libde265 $(IncludeSwitch)/opt/homebrew/include/opencv4 $(IncludeSwitch)/opt/homebrew/opt/jpeg-turbo/include $(IncludeSwitch)/opt/homebrew/Cellar/x265/3.5/include $(IncludeSwitch)/opt/homebrew/Cellar/openexr/3.0.1_1/include/OpenEXR $(IncludeSwitch)/opt/homebrew/Cellar/imath/3.0.2/include/Imath  $(IncludeSwitch). $(IncludeSwitch)../include $(IncludeSwitch)../libTheme $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libParameter $(IncludeSwitch)../libExif $(IncludeSwitch)../libUtility $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../libResource $(IncludeSwitch)../libSqlData $(IncludeSwitch)../libFiltre $(IncludeSwitch)../include $(IncludeSwitch)../Otherlib/wxSVG/include $(IncludeSwitch)../libWindow $(IncludeSwitch)../libPicture $(IncludeSwitch)../libTheme $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libWindowMain $(IncludeSwitch)../libEmail $(IncludeSwitch)../libOpenCL $(IncludeSwitch)../libOpenCLEngine $(IncludeSwitch)../libOpenGL $(IncludeSwitch)../CxImage $(IncludeSwitch)../libDialog $(IncludeSwitch)../libAppleFunction $(IncludeSwitch)../libextern/libOpenCL/include $(IncludeSwitch)../libextern/ffmpeg-master $(IncludeSwitch)../libextern/ffmpeg-master/include $(IncludeSwitch)../libDeepLearning 
IncludePCH             :=  -include ../include/Release/header.h 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                :=$(LibraryPathSwitch)/opt/homebrew/Cellar/openexr/3.0.1_1/lib $(LibraryPathSwitch)/opt/homebrew/Cellar/imath/3.0.2/lib $(LibraryPathSwitch)/opt/homebrew/lib $(LibraryPathSwitch)/opt/homebrew/opt/jpeg-turbo/lib  $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/clang++ -arch arm64
CC       := /usr/bin/clang -arch arm64
CXXFLAGS := -Wall $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -pthread -std=gnu++14 -fpermissive -O3  $(Preprocessors)
CFLAGS   :=  -Wall  -O3 $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/Applications/codelite.app/Contents/SharedSupport/
Objects0=$(IntermediateDirectory)/PageCurlFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/BitmapFusionFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailHorizontal.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailVertical.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailEffect.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/FiltreToolbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/InfoEffect.cpp$(ObjectSuffix) $(IntermediateDirectory)/BitmapToolbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/ffmpeg_transcoding.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/InfosFileWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/BitmapWndViewer.cpp$(ObjectSuffix) $(IntermediateDirectory)/ffmpegToBitmap.cpp$(ObjectSuffix) $(IntermediateDirectory)/SplitterWithPanel.cpp$(ObjectSuffix) $(IntermediateDirectory)/ffplaycore.cpp$(ObjectSuffix) $(IntermediateDirectory)/FiltreEffectWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/InfosSeparationBarEffect.cpp$(ObjectSuffix) $(IntermediateDirectory)/FiltreEffect.cpp$(ObjectSuffix) $(IntermediateDirectory)/FFmpegTranscodingPimpl.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailVideo.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/ShowBitmap.cpp$(ObjectSuffix) $(IntermediateDirectory)/InfosFile.cpp$(ObjectSuffix) $(IntermediateDirectory)/ModificationManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailVerticalListFile.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(ObjectSuffix) $(IntermediateDirectory)/SliderVideo.cpp$(ObjectSuffix) $(IntermediateDirectory)/PrintEngine.cpp$(ObjectSuffix) $(IntermediateDirectory)/ffmfcpimpl.cpp$(ObjectSuffix) $(IntermediateDirectory)/SlideToolbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/InfoEffectWnd.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/CriteriaTree.cpp$(ObjectSuffix) $(IntermediateDirectory)/VideoControl_soft.cpp$(ObjectSuffix) $(IntermediateDirectory)/BitmapPrintout.cpp$(ObjectSuffix) $(IntermediateDirectory)/ShowVideo.cpp$(ObjectSuffix) $(IntermediateDirectory)/CriteriaTreeWnd.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(IntermediateDirectory) $(OutputFile)

$(OutputFile): $(Objects)
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(AR) $(ArchiveOutputSwitch)$(OutputFile) $(Objects)
	@$(MakeDirCommand) "/Users/jacques/developpement/git/Regards/.build-release"
	@echo rebuilt > "/Users/jacques/developpement/git/Regards/.build-release/libControl"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


./Release:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:

# PreCompiled Header
../include/Release/header.h.gch: ../include/Release/header.h
	$(CXX) $(SourceSwitch) ../include/Release/header.h $(PCHCompileFlags) $(CXXFLAGS) $(IncludePath)



##
## Objects
##
$(IntermediateDirectory)/PageCurlFilter.cpp$(ObjectSuffix): PageCurlFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PageCurlFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PageCurlFilter.cpp$(DependSuffix) -MM PageCurlFilter.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/PageCurlFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PageCurlFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PageCurlFilter.cpp$(PreprocessSuffix): PageCurlFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PageCurlFilter.cpp$(PreprocessSuffix) PageCurlFilter.cpp

$(IntermediateDirectory)/BitmapFusionFilter.cpp$(ObjectSuffix): BitmapFusionFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BitmapFusionFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BitmapFusionFilter.cpp$(DependSuffix) -MM BitmapFusionFilter.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/BitmapFusionFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BitmapFusionFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BitmapFusionFilter.cpp$(PreprocessSuffix): BitmapFusionFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BitmapFusionFilter.cpp$(PreprocessSuffix) BitmapFusionFilter.cpp

$(IntermediateDirectory)/ThumbnailHorizontal.cpp$(ObjectSuffix): ThumbnailHorizontal.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailHorizontal.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailHorizontal.cpp$(DependSuffix) -MM ThumbnailHorizontal.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/ThumbnailHorizontal.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailHorizontal.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailHorizontal.cpp$(PreprocessSuffix): ThumbnailHorizontal.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailHorizontal.cpp$(PreprocessSuffix) ThumbnailHorizontal.cpp

$(IntermediateDirectory)/ThumbnailVertical.cpp$(ObjectSuffix): ThumbnailVertical.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailVertical.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailVertical.cpp$(DependSuffix) -MM ThumbnailVertical.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/ThumbnailVertical.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailVertical.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailVertical.cpp$(PreprocessSuffix): ThumbnailVertical.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailVertical.cpp$(PreprocessSuffix) ThumbnailVertical.cpp

$(IntermediateDirectory)/ThumbnailEffect.cpp$(ObjectSuffix): ThumbnailEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailEffect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailEffect.cpp$(DependSuffix) -MM ThumbnailEffect.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/ThumbnailEffect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailEffect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailEffect.cpp$(PreprocessSuffix): ThumbnailEffect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailEffect.cpp$(PreprocessSuffix) ThumbnailEffect.cpp

$(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(ObjectSuffix): ThumbnailViewerEffectWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(DependSuffix) -MM ThumbnailViewerEffectWnd.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/ThumbnailViewerEffectWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(PreprocessSuffix): ThumbnailViewerEffectWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(PreprocessSuffix) ThumbnailViewerEffectWnd.cpp

$(IntermediateDirectory)/FiltreToolbar.cpp$(ObjectSuffix): FiltreToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FiltreToolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FiltreToolbar.cpp$(DependSuffix) -MM FiltreToolbar.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/FiltreToolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FiltreToolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FiltreToolbar.cpp$(PreprocessSuffix): FiltreToolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FiltreToolbar.cpp$(PreprocessSuffix) FiltreToolbar.cpp

$(IntermediateDirectory)/InfoEffect.cpp$(ObjectSuffix): InfoEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InfoEffect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InfoEffect.cpp$(DependSuffix) -MM InfoEffect.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/InfoEffect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InfoEffect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InfoEffect.cpp$(PreprocessSuffix): InfoEffect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InfoEffect.cpp$(PreprocessSuffix) InfoEffect.cpp

$(IntermediateDirectory)/BitmapToolbar.cpp$(ObjectSuffix): BitmapToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BitmapToolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BitmapToolbar.cpp$(DependSuffix) -MM BitmapToolbar.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/BitmapToolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BitmapToolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BitmapToolbar.cpp$(PreprocessSuffix): BitmapToolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BitmapToolbar.cpp$(PreprocessSuffix) BitmapToolbar.cpp

$(IntermediateDirectory)/ffmpeg_transcoding.cpp$(ObjectSuffix): ffmpeg_transcoding.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ffmpeg_transcoding.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ffmpeg_transcoding.cpp$(DependSuffix) -MM ffmpeg_transcoding.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/ffmpeg_transcoding.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ffmpeg_transcoding.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ffmpeg_transcoding.cpp$(PreprocessSuffix): ffmpeg_transcoding.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ffmpeg_transcoding.cpp$(PreprocessSuffix) ffmpeg_transcoding.cpp

$(IntermediateDirectory)/InfosFileWnd.cpp$(ObjectSuffix): InfosFileWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InfosFileWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InfosFileWnd.cpp$(DependSuffix) -MM InfosFileWnd.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/InfosFileWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InfosFileWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InfosFileWnd.cpp$(PreprocessSuffix): InfosFileWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InfosFileWnd.cpp$(PreprocessSuffix) InfosFileWnd.cpp

$(IntermediateDirectory)/BitmapWndViewer.cpp$(ObjectSuffix): BitmapWndViewer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BitmapWndViewer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BitmapWndViewer.cpp$(DependSuffix) -MM BitmapWndViewer.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/BitmapWndViewer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BitmapWndViewer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BitmapWndViewer.cpp$(PreprocessSuffix): BitmapWndViewer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BitmapWndViewer.cpp$(PreprocessSuffix) BitmapWndViewer.cpp

$(IntermediateDirectory)/ffmpegToBitmap.cpp$(ObjectSuffix): ffmpegToBitmap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ffmpegToBitmap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ffmpegToBitmap.cpp$(DependSuffix) -MM ffmpegToBitmap.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/ffmpegToBitmap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ffmpegToBitmap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ffmpegToBitmap.cpp$(PreprocessSuffix): ffmpegToBitmap.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ffmpegToBitmap.cpp$(PreprocessSuffix) ffmpegToBitmap.cpp

$(IntermediateDirectory)/SplitterWithPanel.cpp$(ObjectSuffix): SplitterWithPanel.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SplitterWithPanel.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SplitterWithPanel.cpp$(DependSuffix) -MM SplitterWithPanel.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/SplitterWithPanel.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SplitterWithPanel.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SplitterWithPanel.cpp$(PreprocessSuffix): SplitterWithPanel.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SplitterWithPanel.cpp$(PreprocessSuffix) SplitterWithPanel.cpp

$(IntermediateDirectory)/ffplaycore.cpp$(ObjectSuffix): ffplaycore.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ffplaycore.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ffplaycore.cpp$(DependSuffix) -MM ffplaycore.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/ffplaycore.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ffplaycore.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ffplaycore.cpp$(PreprocessSuffix): ffplaycore.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ffplaycore.cpp$(PreprocessSuffix) ffplaycore.cpp

$(IntermediateDirectory)/FiltreEffectWnd.cpp$(ObjectSuffix): FiltreEffectWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FiltreEffectWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FiltreEffectWnd.cpp$(DependSuffix) -MM FiltreEffectWnd.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/FiltreEffectWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FiltreEffectWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FiltreEffectWnd.cpp$(PreprocessSuffix): FiltreEffectWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FiltreEffectWnd.cpp$(PreprocessSuffix) FiltreEffectWnd.cpp

$(IntermediateDirectory)/InfosSeparationBarEffect.cpp$(ObjectSuffix): InfosSeparationBarEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InfosSeparationBarEffect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InfosSeparationBarEffect.cpp$(DependSuffix) -MM InfosSeparationBarEffect.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/InfosSeparationBarEffect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InfosSeparationBarEffect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InfosSeparationBarEffect.cpp$(PreprocessSuffix): InfosSeparationBarEffect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InfosSeparationBarEffect.cpp$(PreprocessSuffix) InfosSeparationBarEffect.cpp

$(IntermediateDirectory)/FiltreEffect.cpp$(ObjectSuffix): FiltreEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FiltreEffect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FiltreEffect.cpp$(DependSuffix) -MM FiltreEffect.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/FiltreEffect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FiltreEffect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FiltreEffect.cpp$(PreprocessSuffix): FiltreEffect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FiltreEffect.cpp$(PreprocessSuffix) FiltreEffect.cpp

$(IntermediateDirectory)/FFmpegTranscodingPimpl.cpp$(ObjectSuffix): FFmpegTranscodingPimpl.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FFmpegTranscodingPimpl.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FFmpegTranscodingPimpl.cpp$(DependSuffix) -MM FFmpegTranscodingPimpl.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/FFmpegTranscodingPimpl.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FFmpegTranscodingPimpl.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FFmpegTranscodingPimpl.cpp$(PreprocessSuffix): FFmpegTranscodingPimpl.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FFmpegTranscodingPimpl.cpp$(PreprocessSuffix) FFmpegTranscodingPimpl.cpp

$(IntermediateDirectory)/ThumbnailVideo.cpp$(ObjectSuffix): ThumbnailVideo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailVideo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailVideo.cpp$(DependSuffix) -MM ThumbnailVideo.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/ThumbnailVideo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailVideo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailVideo.cpp$(PreprocessSuffix): ThumbnailVideo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailVideo.cpp$(PreprocessSuffix) ThumbnailVideo.cpp

$(IntermediateDirectory)/ShowBitmap.cpp$(ObjectSuffix): ShowBitmap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ShowBitmap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ShowBitmap.cpp$(DependSuffix) -MM ShowBitmap.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/ShowBitmap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ShowBitmap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ShowBitmap.cpp$(PreprocessSuffix): ShowBitmap.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ShowBitmap.cpp$(PreprocessSuffix) ShowBitmap.cpp

$(IntermediateDirectory)/InfosFile.cpp$(ObjectSuffix): InfosFile.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InfosFile.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InfosFile.cpp$(DependSuffix) -MM InfosFile.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/InfosFile.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InfosFile.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InfosFile.cpp$(PreprocessSuffix): InfosFile.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InfosFile.cpp$(PreprocessSuffix) InfosFile.cpp

$(IntermediateDirectory)/ModificationManager.cpp$(ObjectSuffix): ModificationManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ModificationManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ModificationManager.cpp$(DependSuffix) -MM ModificationManager.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/ModificationManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ModificationManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ModificationManager.cpp$(PreprocessSuffix): ModificationManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ModificationManager.cpp$(PreprocessSuffix) ModificationManager.cpp

$(IntermediateDirectory)/ThumbnailVerticalListFile.cpp$(ObjectSuffix): ThumbnailVerticalListFile.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailVerticalListFile.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailVerticalListFile.cpp$(DependSuffix) -MM ThumbnailVerticalListFile.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/ThumbnailVerticalListFile.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailVerticalListFile.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailVerticalListFile.cpp$(PreprocessSuffix): ThumbnailVerticalListFile.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailVerticalListFile.cpp$(PreprocessSuffix) ThumbnailVerticalListFile.cpp

$(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(ObjectSuffix): ThumbnailViewerEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(DependSuffix) -MM ThumbnailViewerEffect.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/ThumbnailViewerEffect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(PreprocessSuffix): ThumbnailViewerEffect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(PreprocessSuffix) ThumbnailViewerEffect.cpp

$(IntermediateDirectory)/SliderVideo.cpp$(ObjectSuffix): SliderVideo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SliderVideo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SliderVideo.cpp$(DependSuffix) -MM SliderVideo.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/SliderVideo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SliderVideo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SliderVideo.cpp$(PreprocessSuffix): SliderVideo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SliderVideo.cpp$(PreprocessSuffix) SliderVideo.cpp

$(IntermediateDirectory)/PrintEngine.cpp$(ObjectSuffix): PrintEngine.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PrintEngine.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PrintEngine.cpp$(DependSuffix) -MM PrintEngine.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/PrintEngine.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PrintEngine.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PrintEngine.cpp$(PreprocessSuffix): PrintEngine.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PrintEngine.cpp$(PreprocessSuffix) PrintEngine.cpp

$(IntermediateDirectory)/ffmfcpimpl.cpp$(ObjectSuffix): ffmfcpimpl.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ffmfcpimpl.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ffmfcpimpl.cpp$(DependSuffix) -MM ffmfcpimpl.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/ffmfcpimpl.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ffmfcpimpl.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ffmfcpimpl.cpp$(PreprocessSuffix): ffmfcpimpl.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ffmfcpimpl.cpp$(PreprocessSuffix) ffmfcpimpl.cpp

$(IntermediateDirectory)/SlideToolbar.cpp$(ObjectSuffix): SlideToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SlideToolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SlideToolbar.cpp$(DependSuffix) -MM SlideToolbar.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/SlideToolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SlideToolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SlideToolbar.cpp$(PreprocessSuffix): SlideToolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SlideToolbar.cpp$(PreprocessSuffix) SlideToolbar.cpp

$(IntermediateDirectory)/InfoEffectWnd.cpp$(ObjectSuffix): InfoEffectWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InfoEffectWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InfoEffectWnd.cpp$(DependSuffix) -MM InfoEffectWnd.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/InfoEffectWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InfoEffectWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InfoEffectWnd.cpp$(PreprocessSuffix): InfoEffectWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InfoEffectWnd.cpp$(PreprocessSuffix) InfoEffectWnd.cpp

$(IntermediateDirectory)/CriteriaTree.cpp$(ObjectSuffix): CriteriaTree.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CriteriaTree.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CriteriaTree.cpp$(DependSuffix) -MM CriteriaTree.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/CriteriaTree.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CriteriaTree.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CriteriaTree.cpp$(PreprocessSuffix): CriteriaTree.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CriteriaTree.cpp$(PreprocessSuffix) CriteriaTree.cpp

$(IntermediateDirectory)/VideoControl_soft.cpp$(ObjectSuffix): VideoControl_soft.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/VideoControl_soft.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/VideoControl_soft.cpp$(DependSuffix) -MM VideoControl_soft.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/VideoControl_soft.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/VideoControl_soft.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/VideoControl_soft.cpp$(PreprocessSuffix): VideoControl_soft.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/VideoControl_soft.cpp$(PreprocessSuffix) VideoControl_soft.cpp

$(IntermediateDirectory)/BitmapPrintout.cpp$(ObjectSuffix): BitmapPrintout.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BitmapPrintout.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BitmapPrintout.cpp$(DependSuffix) -MM BitmapPrintout.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/BitmapPrintout.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BitmapPrintout.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BitmapPrintout.cpp$(PreprocessSuffix): BitmapPrintout.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BitmapPrintout.cpp$(PreprocessSuffix) BitmapPrintout.cpp

$(IntermediateDirectory)/ShowVideo.cpp$(ObjectSuffix): ShowVideo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ShowVideo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ShowVideo.cpp$(DependSuffix) -MM ShowVideo.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/ShowVideo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ShowVideo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ShowVideo.cpp$(PreprocessSuffix): ShowVideo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ShowVideo.cpp$(PreprocessSuffix) ShowVideo.cpp

$(IntermediateDirectory)/CriteriaTreeWnd.cpp$(ObjectSuffix): CriteriaTreeWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CriteriaTreeWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CriteriaTreeWnd.cpp$(DependSuffix) -MM CriteriaTreeWnd.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libControl/CriteriaTreeWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CriteriaTreeWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CriteriaTreeWnd.cpp$(PreprocessSuffix): CriteriaTreeWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CriteriaTreeWnd.cpp$(PreprocessSuffix) CriteriaTreeWnd.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/
	$(RM) ../include/Release/header.h.gch


