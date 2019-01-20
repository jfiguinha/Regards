##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libViewer
ConfigurationName      :=Debug
WorkspacePath          :=/home/figuinha/Developpement/Regards
ProjectPath            :=/home/figuinha/Developpement/Regards/libViewer
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=20/01/19
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
OutputFile             :=$(IntermediateDirectory)/$(ProjectName).a
Preprocessors          :=$(PreprocessorSwitch)VIEWER $(PreprocessorSwitch)FFMPEG $(PreprocessorSwitch)OPENGL $(PreprocessorSwitch)WEBVIEWER 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libViewer.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2  $(IncludeSwitch)../wxCheckTree $(IncludeSwitch)../Otherlib/RegardsRaw $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libTheme $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libParameter $(IncludeSwitch)../libUtility $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libResource $(IncludeSwitch)../libSqlData $(IncludeSwitch)../libRenderBitmap $(IncludeSwitch)../libSqlEngine $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libFiltre $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../Otherlib/wxSVG/include $(IncludeSwitch)../libWindow $(IncludeSwitch)../libPrint $(IncludeSwitch)../libPicture $(IncludeSwitch)../libTheme $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libControl $(IncludeSwitch)../libIntro $(IncludeSwitch)../libDialog $(IncludeSwitch)../libWindowMain $(IncludeSwitch)../libVideoFFMpeg $(IncludeSwitch)../libOpenCL $(IncludeSwitch)../libOpenGL $(IncludeSwitch)../libVideoThumbnail $(IncludeSwitch)../FaceRecognition $(IncludeSwitch)../libCompress $(IncludeSwitch)../libAppleFunction $(IncludeSwitch)../libextern/glew-2.1.0/include 
IncludePCH             :=  -include ../include/Debug/header.h 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                :=$(LibraryPathSwitch)/opt/AMDAPPSDK-3.0/lib  $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/ccache /usr/bin/g++
CC       := /usr/bin/ccache /usr/bin/gcc
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread  -fopenmp -std=gnu++11 -Wall -ggdb -O0 $(Preprocessors)
CFLAGS   :=  -Wall -ggdb -O0 $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/FiltreEffectWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/ViewerParam.cpp$(ObjectSuffix) $(IntermediateDirectory)/ListFace.cpp$(ObjectSuffix) $(IntermediateDirectory)/InfosSeparationBarFace.cpp$(ObjectSuffix) $(IntermediateDirectory)/ListPicture.cpp$(ObjectSuffix) $(IntermediateDirectory)/PreviewThumbnailSplitter.cpp$(ObjectSuffix) $(IntermediateDirectory)/CategoryWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/ViewerTheme.cpp$(ObjectSuffix) $(IntermediateDirectory)/ViewerThemeInit.cpp$(ObjectSuffix) $(IntermediateDirectory)/InfoEffectWnd.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/CentralWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(ObjectSuffix) $(IntermediateDirectory)/FilterPreviewSplitter.cpp$(ObjectSuffix) $(IntermediateDirectory)/MainWindow.cpp$(ObjectSuffix) $(IntermediateDirectory)/PreviewToolbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/FiltreEffect.cpp$(ObjectSuffix) $(IntermediateDirectory)/PreviewInfosWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/ModificationManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/ViewerFrame.cpp$(ObjectSuffix) $(IntermediateDirectory)/InfoEffect.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/PreviewFaceSplitter.cpp$(ObjectSuffix) $(IntermediateDirectory)/PanelInfosWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/ImageList.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailFacePertinenceToolbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/AnimationToolbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/CriteriaFolderSplitter.cpp$(ObjectSuffix) $(IntermediateDirectory)/FiltreToolbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/PreviewWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/toolbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/CategoryFolderWindow.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/ThumbnailFolder.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailVideoPanel.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailFaceToolBar.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailToolBar.cpp$(ObjectSuffix) $(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailFace.cpp$(ObjectSuffix) $(IntermediateDirectory)/ViewerParamInit.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/ToolbarInfos.cpp$(ObjectSuffix) 



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
	$(AR) $(ArchiveOutputSwitch)$(OutputFile) @$(ObjectsFileList) $(ArLibs)
	@$(MakeDirCommand) "/home/figuinha/Developpement/Regards/.build-debug"
	@echo rebuilt > "/home/figuinha/Developpement/Regards/.build-debug/libViewer"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:

# PreCompiled Header
../include/Debug/header.h.gch: ../include/Debug/header.h
	$(CXX) $(SourceSwitch) ../include/Debug/header.h $(PCHCompileFlags) $(CXXFLAGS) $(IncludePath)



##
## Objects
##
$(IntermediateDirectory)/FiltreEffectWnd.cpp$(ObjectSuffix): FiltreEffectWnd.cpp $(IntermediateDirectory)/FiltreEffectWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/FiltreEffectWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FiltreEffectWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FiltreEffectWnd.cpp$(DependSuffix): FiltreEffectWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FiltreEffectWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FiltreEffectWnd.cpp$(DependSuffix) -MM FiltreEffectWnd.cpp

$(IntermediateDirectory)/FiltreEffectWnd.cpp$(PreprocessSuffix): FiltreEffectWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FiltreEffectWnd.cpp$(PreprocessSuffix) FiltreEffectWnd.cpp

$(IntermediateDirectory)/ViewerParam.cpp$(ObjectSuffix): ViewerParam.cpp $(IntermediateDirectory)/ViewerParam.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/ViewerParam.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ViewerParam.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ViewerParam.cpp$(DependSuffix): ViewerParam.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ViewerParam.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ViewerParam.cpp$(DependSuffix) -MM ViewerParam.cpp

$(IntermediateDirectory)/ViewerParam.cpp$(PreprocessSuffix): ViewerParam.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ViewerParam.cpp$(PreprocessSuffix) ViewerParam.cpp

$(IntermediateDirectory)/ListFace.cpp$(ObjectSuffix): ListFace.cpp $(IntermediateDirectory)/ListFace.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/ListFace.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ListFace.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ListFace.cpp$(DependSuffix): ListFace.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ListFace.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ListFace.cpp$(DependSuffix) -MM ListFace.cpp

$(IntermediateDirectory)/ListFace.cpp$(PreprocessSuffix): ListFace.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ListFace.cpp$(PreprocessSuffix) ListFace.cpp

$(IntermediateDirectory)/InfosSeparationBarFace.cpp$(ObjectSuffix): InfosSeparationBarFace.cpp $(IntermediateDirectory)/InfosSeparationBarFace.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/InfosSeparationBarFace.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InfosSeparationBarFace.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InfosSeparationBarFace.cpp$(DependSuffix): InfosSeparationBarFace.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InfosSeparationBarFace.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InfosSeparationBarFace.cpp$(DependSuffix) -MM InfosSeparationBarFace.cpp

$(IntermediateDirectory)/InfosSeparationBarFace.cpp$(PreprocessSuffix): InfosSeparationBarFace.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InfosSeparationBarFace.cpp$(PreprocessSuffix) InfosSeparationBarFace.cpp

$(IntermediateDirectory)/ListPicture.cpp$(ObjectSuffix): ListPicture.cpp $(IntermediateDirectory)/ListPicture.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/ListPicture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ListPicture.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ListPicture.cpp$(DependSuffix): ListPicture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ListPicture.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ListPicture.cpp$(DependSuffix) -MM ListPicture.cpp

$(IntermediateDirectory)/ListPicture.cpp$(PreprocessSuffix): ListPicture.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ListPicture.cpp$(PreprocessSuffix) ListPicture.cpp

$(IntermediateDirectory)/PreviewThumbnailSplitter.cpp$(ObjectSuffix): PreviewThumbnailSplitter.cpp $(IntermediateDirectory)/PreviewThumbnailSplitter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/PreviewThumbnailSplitter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PreviewThumbnailSplitter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PreviewThumbnailSplitter.cpp$(DependSuffix): PreviewThumbnailSplitter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PreviewThumbnailSplitter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PreviewThumbnailSplitter.cpp$(DependSuffix) -MM PreviewThumbnailSplitter.cpp

$(IntermediateDirectory)/PreviewThumbnailSplitter.cpp$(PreprocessSuffix): PreviewThumbnailSplitter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PreviewThumbnailSplitter.cpp$(PreprocessSuffix) PreviewThumbnailSplitter.cpp

$(IntermediateDirectory)/CategoryWnd.cpp$(ObjectSuffix): CategoryWnd.cpp $(IntermediateDirectory)/CategoryWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/CategoryWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CategoryWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CategoryWnd.cpp$(DependSuffix): CategoryWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CategoryWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CategoryWnd.cpp$(DependSuffix) -MM CategoryWnd.cpp

$(IntermediateDirectory)/CategoryWnd.cpp$(PreprocessSuffix): CategoryWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CategoryWnd.cpp$(PreprocessSuffix) CategoryWnd.cpp

$(IntermediateDirectory)/ViewerTheme.cpp$(ObjectSuffix): ViewerTheme.cpp $(IntermediateDirectory)/ViewerTheme.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/ViewerTheme.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ViewerTheme.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ViewerTheme.cpp$(DependSuffix): ViewerTheme.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ViewerTheme.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ViewerTheme.cpp$(DependSuffix) -MM ViewerTheme.cpp

$(IntermediateDirectory)/ViewerTheme.cpp$(PreprocessSuffix): ViewerTheme.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ViewerTheme.cpp$(PreprocessSuffix) ViewerTheme.cpp

$(IntermediateDirectory)/ViewerThemeInit.cpp$(ObjectSuffix): ViewerThemeInit.cpp $(IntermediateDirectory)/ViewerThemeInit.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/ViewerThemeInit.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ViewerThemeInit.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ViewerThemeInit.cpp$(DependSuffix): ViewerThemeInit.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ViewerThemeInit.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ViewerThemeInit.cpp$(DependSuffix) -MM ViewerThemeInit.cpp

$(IntermediateDirectory)/ViewerThemeInit.cpp$(PreprocessSuffix): ViewerThemeInit.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ViewerThemeInit.cpp$(PreprocessSuffix) ViewerThemeInit.cpp

$(IntermediateDirectory)/InfoEffectWnd.cpp$(ObjectSuffix): InfoEffectWnd.cpp $(IntermediateDirectory)/InfoEffectWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/InfoEffectWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InfoEffectWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InfoEffectWnd.cpp$(DependSuffix): InfoEffectWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InfoEffectWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InfoEffectWnd.cpp$(DependSuffix) -MM InfoEffectWnd.cpp

$(IntermediateDirectory)/InfoEffectWnd.cpp$(PreprocessSuffix): InfoEffectWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InfoEffectWnd.cpp$(PreprocessSuffix) InfoEffectWnd.cpp

$(IntermediateDirectory)/CentralWnd.cpp$(ObjectSuffix): CentralWnd.cpp $(IntermediateDirectory)/CentralWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/CentralWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CentralWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CentralWnd.cpp$(DependSuffix): CentralWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CentralWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CentralWnd.cpp$(DependSuffix) -MM CentralWnd.cpp

$(IntermediateDirectory)/CentralWnd.cpp$(PreprocessSuffix): CentralWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CentralWnd.cpp$(PreprocessSuffix) CentralWnd.cpp

$(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(ObjectSuffix): ThumbnailToolBarZoom.cpp $(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/ThumbnailToolBarZoom.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(DependSuffix): ThumbnailToolBarZoom.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(DependSuffix) -MM ThumbnailToolBarZoom.cpp

$(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(PreprocessSuffix): ThumbnailToolBarZoom.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(PreprocessSuffix) ThumbnailToolBarZoom.cpp

$(IntermediateDirectory)/FilterPreviewSplitter.cpp$(ObjectSuffix): FilterPreviewSplitter.cpp $(IntermediateDirectory)/FilterPreviewSplitter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/FilterPreviewSplitter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FilterPreviewSplitter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FilterPreviewSplitter.cpp$(DependSuffix): FilterPreviewSplitter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FilterPreviewSplitter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FilterPreviewSplitter.cpp$(DependSuffix) -MM FilterPreviewSplitter.cpp

$(IntermediateDirectory)/FilterPreviewSplitter.cpp$(PreprocessSuffix): FilterPreviewSplitter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FilterPreviewSplitter.cpp$(PreprocessSuffix) FilterPreviewSplitter.cpp

$(IntermediateDirectory)/MainWindow.cpp$(ObjectSuffix): MainWindow.cpp $(IntermediateDirectory)/MainWindow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/MainWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MainWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MainWindow.cpp$(DependSuffix): MainWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MainWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MainWindow.cpp$(DependSuffix) -MM MainWindow.cpp

$(IntermediateDirectory)/MainWindow.cpp$(PreprocessSuffix): MainWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MainWindow.cpp$(PreprocessSuffix) MainWindow.cpp

$(IntermediateDirectory)/PreviewToolbar.cpp$(ObjectSuffix): PreviewToolbar.cpp $(IntermediateDirectory)/PreviewToolbar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/PreviewToolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PreviewToolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PreviewToolbar.cpp$(DependSuffix): PreviewToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PreviewToolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PreviewToolbar.cpp$(DependSuffix) -MM PreviewToolbar.cpp

$(IntermediateDirectory)/PreviewToolbar.cpp$(PreprocessSuffix): PreviewToolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PreviewToolbar.cpp$(PreprocessSuffix) PreviewToolbar.cpp

$(IntermediateDirectory)/FiltreEffect.cpp$(ObjectSuffix): FiltreEffect.cpp $(IntermediateDirectory)/FiltreEffect.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/FiltreEffect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FiltreEffect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FiltreEffect.cpp$(DependSuffix): FiltreEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FiltreEffect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FiltreEffect.cpp$(DependSuffix) -MM FiltreEffect.cpp

$(IntermediateDirectory)/FiltreEffect.cpp$(PreprocessSuffix): FiltreEffect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FiltreEffect.cpp$(PreprocessSuffix) FiltreEffect.cpp

$(IntermediateDirectory)/PreviewInfosWnd.cpp$(ObjectSuffix): PreviewInfosWnd.cpp $(IntermediateDirectory)/PreviewInfosWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/PreviewInfosWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PreviewInfosWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PreviewInfosWnd.cpp$(DependSuffix): PreviewInfosWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PreviewInfosWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PreviewInfosWnd.cpp$(DependSuffix) -MM PreviewInfosWnd.cpp

$(IntermediateDirectory)/PreviewInfosWnd.cpp$(PreprocessSuffix): PreviewInfosWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PreviewInfosWnd.cpp$(PreprocessSuffix) PreviewInfosWnd.cpp

$(IntermediateDirectory)/ModificationManager.cpp$(ObjectSuffix): ModificationManager.cpp $(IntermediateDirectory)/ModificationManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/ModificationManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ModificationManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ModificationManager.cpp$(DependSuffix): ModificationManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ModificationManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ModificationManager.cpp$(DependSuffix) -MM ModificationManager.cpp

$(IntermediateDirectory)/ModificationManager.cpp$(PreprocessSuffix): ModificationManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ModificationManager.cpp$(PreprocessSuffix) ModificationManager.cpp

$(IntermediateDirectory)/ViewerFrame.cpp$(ObjectSuffix): ViewerFrame.cpp $(IntermediateDirectory)/ViewerFrame.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/ViewerFrame.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ViewerFrame.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ViewerFrame.cpp$(DependSuffix): ViewerFrame.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ViewerFrame.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ViewerFrame.cpp$(DependSuffix) -MM ViewerFrame.cpp

$(IntermediateDirectory)/ViewerFrame.cpp$(PreprocessSuffix): ViewerFrame.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ViewerFrame.cpp$(PreprocessSuffix) ViewerFrame.cpp

$(IntermediateDirectory)/InfoEffect.cpp$(ObjectSuffix): InfoEffect.cpp $(IntermediateDirectory)/InfoEffect.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/InfoEffect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InfoEffect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InfoEffect.cpp$(DependSuffix): InfoEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InfoEffect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InfoEffect.cpp$(DependSuffix) -MM InfoEffect.cpp

$(IntermediateDirectory)/InfoEffect.cpp$(PreprocessSuffix): InfoEffect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InfoEffect.cpp$(PreprocessSuffix) InfoEffect.cpp

$(IntermediateDirectory)/PreviewFaceSplitter.cpp$(ObjectSuffix): PreviewFaceSplitter.cpp $(IntermediateDirectory)/PreviewFaceSplitter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/PreviewFaceSplitter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PreviewFaceSplitter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PreviewFaceSplitter.cpp$(DependSuffix): PreviewFaceSplitter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PreviewFaceSplitter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PreviewFaceSplitter.cpp$(DependSuffix) -MM PreviewFaceSplitter.cpp

$(IntermediateDirectory)/PreviewFaceSplitter.cpp$(PreprocessSuffix): PreviewFaceSplitter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PreviewFaceSplitter.cpp$(PreprocessSuffix) PreviewFaceSplitter.cpp

$(IntermediateDirectory)/PanelInfosWnd.cpp$(ObjectSuffix): PanelInfosWnd.cpp $(IntermediateDirectory)/PanelInfosWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/PanelInfosWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PanelInfosWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PanelInfosWnd.cpp$(DependSuffix): PanelInfosWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PanelInfosWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PanelInfosWnd.cpp$(DependSuffix) -MM PanelInfosWnd.cpp

$(IntermediateDirectory)/PanelInfosWnd.cpp$(PreprocessSuffix): PanelInfosWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PanelInfosWnd.cpp$(PreprocessSuffix) PanelInfosWnd.cpp

$(IntermediateDirectory)/ImageList.cpp$(ObjectSuffix): ImageList.cpp $(IntermediateDirectory)/ImageList.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/ImageList.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ImageList.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ImageList.cpp$(DependSuffix): ImageList.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ImageList.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ImageList.cpp$(DependSuffix) -MM ImageList.cpp

$(IntermediateDirectory)/ImageList.cpp$(PreprocessSuffix): ImageList.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ImageList.cpp$(PreprocessSuffix) ImageList.cpp

$(IntermediateDirectory)/ThumbnailFacePertinenceToolbar.cpp$(ObjectSuffix): ThumbnailFacePertinenceToolbar.cpp $(IntermediateDirectory)/ThumbnailFacePertinenceToolbar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/ThumbnailFacePertinenceToolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailFacePertinenceToolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailFacePertinenceToolbar.cpp$(DependSuffix): ThumbnailFacePertinenceToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailFacePertinenceToolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailFacePertinenceToolbar.cpp$(DependSuffix) -MM ThumbnailFacePertinenceToolbar.cpp

$(IntermediateDirectory)/ThumbnailFacePertinenceToolbar.cpp$(PreprocessSuffix): ThumbnailFacePertinenceToolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailFacePertinenceToolbar.cpp$(PreprocessSuffix) ThumbnailFacePertinenceToolbar.cpp

$(IntermediateDirectory)/AnimationToolbar.cpp$(ObjectSuffix): AnimationToolbar.cpp $(IntermediateDirectory)/AnimationToolbar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/AnimationToolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/AnimationToolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/AnimationToolbar.cpp$(DependSuffix): AnimationToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/AnimationToolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/AnimationToolbar.cpp$(DependSuffix) -MM AnimationToolbar.cpp

$(IntermediateDirectory)/AnimationToolbar.cpp$(PreprocessSuffix): AnimationToolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/AnimationToolbar.cpp$(PreprocessSuffix) AnimationToolbar.cpp

$(IntermediateDirectory)/CriteriaFolderSplitter.cpp$(ObjectSuffix): CriteriaFolderSplitter.cpp $(IntermediateDirectory)/CriteriaFolderSplitter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/CriteriaFolderSplitter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CriteriaFolderSplitter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CriteriaFolderSplitter.cpp$(DependSuffix): CriteriaFolderSplitter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CriteriaFolderSplitter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CriteriaFolderSplitter.cpp$(DependSuffix) -MM CriteriaFolderSplitter.cpp

$(IntermediateDirectory)/CriteriaFolderSplitter.cpp$(PreprocessSuffix): CriteriaFolderSplitter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CriteriaFolderSplitter.cpp$(PreprocessSuffix) CriteriaFolderSplitter.cpp

$(IntermediateDirectory)/FiltreToolbar.cpp$(ObjectSuffix): FiltreToolbar.cpp $(IntermediateDirectory)/FiltreToolbar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/FiltreToolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FiltreToolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FiltreToolbar.cpp$(DependSuffix): FiltreToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FiltreToolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FiltreToolbar.cpp$(DependSuffix) -MM FiltreToolbar.cpp

$(IntermediateDirectory)/FiltreToolbar.cpp$(PreprocessSuffix): FiltreToolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FiltreToolbar.cpp$(PreprocessSuffix) FiltreToolbar.cpp

$(IntermediateDirectory)/PreviewWnd.cpp$(ObjectSuffix): PreviewWnd.cpp $(IntermediateDirectory)/PreviewWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/PreviewWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PreviewWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PreviewWnd.cpp$(DependSuffix): PreviewWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PreviewWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PreviewWnd.cpp$(DependSuffix) -MM PreviewWnd.cpp

$(IntermediateDirectory)/PreviewWnd.cpp$(PreprocessSuffix): PreviewWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PreviewWnd.cpp$(PreprocessSuffix) PreviewWnd.cpp

$(IntermediateDirectory)/toolbar.cpp$(ObjectSuffix): toolbar.cpp $(IntermediateDirectory)/toolbar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/toolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/toolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/toolbar.cpp$(DependSuffix): toolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/toolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/toolbar.cpp$(DependSuffix) -MM toolbar.cpp

$(IntermediateDirectory)/toolbar.cpp$(PreprocessSuffix): toolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/toolbar.cpp$(PreprocessSuffix) toolbar.cpp

$(IntermediateDirectory)/CategoryFolderWindow.cpp$(ObjectSuffix): CategoryFolderWindow.cpp $(IntermediateDirectory)/CategoryFolderWindow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/CategoryFolderWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CategoryFolderWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CategoryFolderWindow.cpp$(DependSuffix): CategoryFolderWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CategoryFolderWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CategoryFolderWindow.cpp$(DependSuffix) -MM CategoryFolderWindow.cpp

$(IntermediateDirectory)/CategoryFolderWindow.cpp$(PreprocessSuffix): CategoryFolderWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CategoryFolderWindow.cpp$(PreprocessSuffix) CategoryFolderWindow.cpp

$(IntermediateDirectory)/ThumbnailFolder.cpp$(ObjectSuffix): ThumbnailFolder.cpp $(IntermediateDirectory)/ThumbnailFolder.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/ThumbnailFolder.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailFolder.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailFolder.cpp$(DependSuffix): ThumbnailFolder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailFolder.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailFolder.cpp$(DependSuffix) -MM ThumbnailFolder.cpp

$(IntermediateDirectory)/ThumbnailFolder.cpp$(PreprocessSuffix): ThumbnailFolder.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailFolder.cpp$(PreprocessSuffix) ThumbnailFolder.cpp

$(IntermediateDirectory)/ThumbnailVideoPanel.cpp$(ObjectSuffix): ThumbnailVideoPanel.cpp $(IntermediateDirectory)/ThumbnailVideoPanel.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/ThumbnailVideoPanel.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailVideoPanel.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailVideoPanel.cpp$(DependSuffix): ThumbnailVideoPanel.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailVideoPanel.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailVideoPanel.cpp$(DependSuffix) -MM ThumbnailVideoPanel.cpp

$(IntermediateDirectory)/ThumbnailVideoPanel.cpp$(PreprocessSuffix): ThumbnailVideoPanel.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailVideoPanel.cpp$(PreprocessSuffix) ThumbnailVideoPanel.cpp

$(IntermediateDirectory)/ThumbnailFaceToolBar.cpp$(ObjectSuffix): ThumbnailFaceToolBar.cpp $(IntermediateDirectory)/ThumbnailFaceToolBar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/ThumbnailFaceToolBar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailFaceToolBar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailFaceToolBar.cpp$(DependSuffix): ThumbnailFaceToolBar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailFaceToolBar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailFaceToolBar.cpp$(DependSuffix) -MM ThumbnailFaceToolBar.cpp

$(IntermediateDirectory)/ThumbnailFaceToolBar.cpp$(PreprocessSuffix): ThumbnailFaceToolBar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailFaceToolBar.cpp$(PreprocessSuffix) ThumbnailFaceToolBar.cpp

$(IntermediateDirectory)/ThumbnailToolBar.cpp$(ObjectSuffix): ThumbnailToolBar.cpp $(IntermediateDirectory)/ThumbnailToolBar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/ThumbnailToolBar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailToolBar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailToolBar.cpp$(DependSuffix): ThumbnailToolBar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailToolBar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailToolBar.cpp$(DependSuffix) -MM ThumbnailToolBar.cpp

$(IntermediateDirectory)/ThumbnailToolBar.cpp$(PreprocessSuffix): ThumbnailToolBar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailToolBar.cpp$(PreprocessSuffix) ThumbnailToolBar.cpp

$(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(ObjectSuffix): InfosSeparationBarExplorer.cpp $(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/InfosSeparationBarExplorer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(DependSuffix): InfosSeparationBarExplorer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(DependSuffix) -MM InfosSeparationBarExplorer.cpp

$(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(PreprocessSuffix): InfosSeparationBarExplorer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(PreprocessSuffix) InfosSeparationBarExplorer.cpp

$(IntermediateDirectory)/ThumbnailFace.cpp$(ObjectSuffix): ThumbnailFace.cpp $(IntermediateDirectory)/ThumbnailFace.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/ThumbnailFace.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailFace.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailFace.cpp$(DependSuffix): ThumbnailFace.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailFace.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailFace.cpp$(DependSuffix) -MM ThumbnailFace.cpp

$(IntermediateDirectory)/ThumbnailFace.cpp$(PreprocessSuffix): ThumbnailFace.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailFace.cpp$(PreprocessSuffix) ThumbnailFace.cpp

$(IntermediateDirectory)/ViewerParamInit.cpp$(ObjectSuffix): ViewerParamInit.cpp $(IntermediateDirectory)/ViewerParamInit.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/ViewerParamInit.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ViewerParamInit.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ViewerParamInit.cpp$(DependSuffix): ViewerParamInit.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ViewerParamInit.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ViewerParamInit.cpp$(DependSuffix) -MM ViewerParamInit.cpp

$(IntermediateDirectory)/ViewerParamInit.cpp$(PreprocessSuffix): ViewerParamInit.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ViewerParamInit.cpp$(PreprocessSuffix) ViewerParamInit.cpp

$(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(ObjectSuffix): ThumbnailViewerEffect.cpp $(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/ThumbnailViewerEffect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(DependSuffix): ThumbnailViewerEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(DependSuffix) -MM ThumbnailViewerEffect.cpp

$(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(PreprocessSuffix): ThumbnailViewerEffect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(PreprocessSuffix) ThumbnailViewerEffect.cpp

$(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(ObjectSuffix): ThumbnailViewerEffectWnd.cpp $(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/ThumbnailViewerEffectWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(DependSuffix): ThumbnailViewerEffectWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(DependSuffix) -MM ThumbnailViewerEffectWnd.cpp

$(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(PreprocessSuffix): ThumbnailViewerEffectWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(PreprocessSuffix) ThumbnailViewerEffectWnd.cpp

$(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(ObjectSuffix): ThumbnailViewerVideo.cpp $(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/ThumbnailViewerVideo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(DependSuffix): ThumbnailViewerVideo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(DependSuffix) -MM ThumbnailViewerVideo.cpp

$(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(PreprocessSuffix): ThumbnailViewerVideo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(PreprocessSuffix) ThumbnailViewerVideo.cpp

$(IntermediateDirectory)/ToolbarInfos.cpp$(ObjectSuffix): ToolbarInfos.cpp $(IntermediateDirectory)/ToolbarInfos.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libViewer/ToolbarInfos.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ToolbarInfos.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ToolbarInfos.cpp$(DependSuffix): ToolbarInfos.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ToolbarInfos.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ToolbarInfos.cpp$(DependSuffix) -MM ToolbarInfos.cpp

$(IntermediateDirectory)/ToolbarInfos.cpp$(PreprocessSuffix): ToolbarInfos.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ToolbarInfos.cpp$(PreprocessSuffix) ToolbarInfos.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/
	$(RM) ../include/Debug/header.h.gch


