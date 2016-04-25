##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libViewer
ConfigurationName      :=Debug
WorkspacePath          := "/home/figuinha/dev/Regards"
ProjectPath            := "/home/figuinha/dev/Regards/libViewer"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=25/04/16
CodeLitePath           :="/home/figuinha/.codelite"
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
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
Preprocessors          :=$(PreprocessorSwitch)__WXGTK__ $(PreprocessorSwitch)VIEWER 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libViewer.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libTheme $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libextern/ffmpeg-2.6.2 $(IncludeSwitch)../libextern/SDL2-2.0.3/include $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libParameter $(IncludeSwitch)../libextern/exiv2-0.24/src $(IncludeSwitch)../libextern/exiv2-0.24/xmpsdk/include $(IncludeSwitch)../libUtility $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libResource $(IncludeSwitch)../libSqlData $(IncludeSwitch)../libOpenGL $(IncludeSwitch)../libRenderBitmap $(IncludeSwitch)../libSqlEngine $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libextern/glew-1.13.0/include $(IncludeSwitch)../libFiltre $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../Otherlib/wxSVG/include $(IncludeSwitch)../libWindow $(IncludeSwitch)../libPrint $(IncludeSwitch)../libPicture $(IncludeSwitch)../libTheme $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libControl $(IncludeSwitch)../libIntro $(IncludeSwitch)../libDialog $(IncludeSwitch)../libWindowMain $(IncludeSwitch)../libVideoFFMpeg $(IncludeSwitch)../libextern/wxWidgets-3.0.2/include $(IncludeSwitch)../libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -O0 -fopenmp -std=c++11 -Wall  $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall  $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/CentralWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/FiltreEffect.cpp$(ObjectSuffix) $(IntermediateDirectory)/FiltreToolbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/InfoEffect.cpp$(ObjectSuffix) $(IntermediateDirectory)/MainWindow.cpp$(ObjectSuffix) $(IntermediateDirectory)/ModificationManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/PanelInfosWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/PreviewInfosWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/PreviewToolbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/PreviewWnd.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/ThumbnailBitmap.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(ObjectSuffix) $(IntermediateDirectory)/toolbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/ToolbarInfos.cpp$(ObjectSuffix) $(IntermediateDirectory)/ViewerFrame.cpp$(ObjectSuffix) $(IntermediateDirectory)/ViewerParam.cpp$(ObjectSuffix) $(IntermediateDirectory)/ViewerParamInit.cpp$(ObjectSuffix) $(IntermediateDirectory)/ViewerTheme.cpp$(ObjectSuffix) $(IntermediateDirectory)/ViewerThemeInit.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/BrightAndContrastFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/CategoryFolderWindow.cpp$(ObjectSuffix) $(IntermediateDirectory)/CategoryWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/CloudsFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/ColorBalanceFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/FilterPreviewSplitter.cpp$(ObjectSuffix) $(IntermediateDirectory)/FilterWindowParam.cpp$(ObjectSuffix) $(IntermediateDirectory)/FiltreEffectWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/InfoEffectWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/LensFlareFilter.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/MotionBlurFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/PhotoFiltreFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/PosterisationFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/RotateFreeFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/SolarisationFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/SwirlFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "/home/figuinha/dev/Regards/.build-debug"
	@echo rebuilt > "/home/figuinha/dev/Regards/.build-debug/libViewer"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/CentralWnd.cpp$(ObjectSuffix): CentralWnd.cpp $(IntermediateDirectory)/CentralWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/CentralWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CentralWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CentralWnd.cpp$(DependSuffix): CentralWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CentralWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CentralWnd.cpp$(DependSuffix) -MM "CentralWnd.cpp"

$(IntermediateDirectory)/CentralWnd.cpp$(PreprocessSuffix): CentralWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CentralWnd.cpp$(PreprocessSuffix) "CentralWnd.cpp"

$(IntermediateDirectory)/FiltreEffect.cpp$(ObjectSuffix): FiltreEffect.cpp $(IntermediateDirectory)/FiltreEffect.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/FiltreEffect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FiltreEffect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FiltreEffect.cpp$(DependSuffix): FiltreEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FiltreEffect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FiltreEffect.cpp$(DependSuffix) -MM "FiltreEffect.cpp"

$(IntermediateDirectory)/FiltreEffect.cpp$(PreprocessSuffix): FiltreEffect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FiltreEffect.cpp$(PreprocessSuffix) "FiltreEffect.cpp"

$(IntermediateDirectory)/FiltreToolbar.cpp$(ObjectSuffix): FiltreToolbar.cpp $(IntermediateDirectory)/FiltreToolbar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/FiltreToolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FiltreToolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FiltreToolbar.cpp$(DependSuffix): FiltreToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FiltreToolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FiltreToolbar.cpp$(DependSuffix) -MM "FiltreToolbar.cpp"

$(IntermediateDirectory)/FiltreToolbar.cpp$(PreprocessSuffix): FiltreToolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FiltreToolbar.cpp$(PreprocessSuffix) "FiltreToolbar.cpp"

$(IntermediateDirectory)/InfoEffect.cpp$(ObjectSuffix): InfoEffect.cpp $(IntermediateDirectory)/InfoEffect.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/InfoEffect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InfoEffect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InfoEffect.cpp$(DependSuffix): InfoEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InfoEffect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InfoEffect.cpp$(DependSuffix) -MM "InfoEffect.cpp"

$(IntermediateDirectory)/InfoEffect.cpp$(PreprocessSuffix): InfoEffect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InfoEffect.cpp$(PreprocessSuffix) "InfoEffect.cpp"

$(IntermediateDirectory)/MainWindow.cpp$(ObjectSuffix): MainWindow.cpp $(IntermediateDirectory)/MainWindow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/MainWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MainWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MainWindow.cpp$(DependSuffix): MainWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MainWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MainWindow.cpp$(DependSuffix) -MM "MainWindow.cpp"

$(IntermediateDirectory)/MainWindow.cpp$(PreprocessSuffix): MainWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MainWindow.cpp$(PreprocessSuffix) "MainWindow.cpp"

$(IntermediateDirectory)/ModificationManager.cpp$(ObjectSuffix): ModificationManager.cpp $(IntermediateDirectory)/ModificationManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/ModificationManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ModificationManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ModificationManager.cpp$(DependSuffix): ModificationManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ModificationManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ModificationManager.cpp$(DependSuffix) -MM "ModificationManager.cpp"

$(IntermediateDirectory)/ModificationManager.cpp$(PreprocessSuffix): ModificationManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ModificationManager.cpp$(PreprocessSuffix) "ModificationManager.cpp"

$(IntermediateDirectory)/PanelInfosWnd.cpp$(ObjectSuffix): PanelInfosWnd.cpp $(IntermediateDirectory)/PanelInfosWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/PanelInfosWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PanelInfosWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PanelInfosWnd.cpp$(DependSuffix): PanelInfosWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PanelInfosWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PanelInfosWnd.cpp$(DependSuffix) -MM "PanelInfosWnd.cpp"

$(IntermediateDirectory)/PanelInfosWnd.cpp$(PreprocessSuffix): PanelInfosWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PanelInfosWnd.cpp$(PreprocessSuffix) "PanelInfosWnd.cpp"

$(IntermediateDirectory)/PreviewInfosWnd.cpp$(ObjectSuffix): PreviewInfosWnd.cpp $(IntermediateDirectory)/PreviewInfosWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/PreviewInfosWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PreviewInfosWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PreviewInfosWnd.cpp$(DependSuffix): PreviewInfosWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PreviewInfosWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PreviewInfosWnd.cpp$(DependSuffix) -MM "PreviewInfosWnd.cpp"

$(IntermediateDirectory)/PreviewInfosWnd.cpp$(PreprocessSuffix): PreviewInfosWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PreviewInfosWnd.cpp$(PreprocessSuffix) "PreviewInfosWnd.cpp"

$(IntermediateDirectory)/PreviewToolbar.cpp$(ObjectSuffix): PreviewToolbar.cpp $(IntermediateDirectory)/PreviewToolbar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/PreviewToolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PreviewToolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PreviewToolbar.cpp$(DependSuffix): PreviewToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PreviewToolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PreviewToolbar.cpp$(DependSuffix) -MM "PreviewToolbar.cpp"

$(IntermediateDirectory)/PreviewToolbar.cpp$(PreprocessSuffix): PreviewToolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PreviewToolbar.cpp$(PreprocessSuffix) "PreviewToolbar.cpp"

$(IntermediateDirectory)/PreviewWnd.cpp$(ObjectSuffix): PreviewWnd.cpp $(IntermediateDirectory)/PreviewWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/PreviewWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PreviewWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PreviewWnd.cpp$(DependSuffix): PreviewWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PreviewWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PreviewWnd.cpp$(DependSuffix) -MM "PreviewWnd.cpp"

$(IntermediateDirectory)/PreviewWnd.cpp$(PreprocessSuffix): PreviewWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PreviewWnd.cpp$(PreprocessSuffix) "PreviewWnd.cpp"

$(IntermediateDirectory)/ThumbnailBitmap.cpp$(ObjectSuffix): ThumbnailBitmap.cpp $(IntermediateDirectory)/ThumbnailBitmap.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/ThumbnailBitmap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailBitmap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailBitmap.cpp$(DependSuffix): ThumbnailBitmap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailBitmap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailBitmap.cpp$(DependSuffix) -MM "ThumbnailBitmap.cpp"

$(IntermediateDirectory)/ThumbnailBitmap.cpp$(PreprocessSuffix): ThumbnailBitmap.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailBitmap.cpp$(PreprocessSuffix) "ThumbnailBitmap.cpp"

$(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(ObjectSuffix): ThumbnailViewerEffect.cpp $(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/ThumbnailViewerEffect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(DependSuffix): ThumbnailViewerEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(DependSuffix) -MM "ThumbnailViewerEffect.cpp"

$(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(PreprocessSuffix): ThumbnailViewerEffect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(PreprocessSuffix) "ThumbnailViewerEffect.cpp"

$(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(ObjectSuffix): ThumbnailViewerVideo.cpp $(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/ThumbnailViewerVideo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(DependSuffix): ThumbnailViewerVideo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(DependSuffix) -MM "ThumbnailViewerVideo.cpp"

$(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(PreprocessSuffix): ThumbnailViewerVideo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(PreprocessSuffix) "ThumbnailViewerVideo.cpp"

$(IntermediateDirectory)/toolbar.cpp$(ObjectSuffix): toolbar.cpp $(IntermediateDirectory)/toolbar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/toolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/toolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/toolbar.cpp$(DependSuffix): toolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/toolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/toolbar.cpp$(DependSuffix) -MM "toolbar.cpp"

$(IntermediateDirectory)/toolbar.cpp$(PreprocessSuffix): toolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/toolbar.cpp$(PreprocessSuffix) "toolbar.cpp"

$(IntermediateDirectory)/ToolbarInfos.cpp$(ObjectSuffix): ToolbarInfos.cpp $(IntermediateDirectory)/ToolbarInfos.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/ToolbarInfos.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ToolbarInfos.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ToolbarInfos.cpp$(DependSuffix): ToolbarInfos.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ToolbarInfos.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ToolbarInfos.cpp$(DependSuffix) -MM "ToolbarInfos.cpp"

$(IntermediateDirectory)/ToolbarInfos.cpp$(PreprocessSuffix): ToolbarInfos.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ToolbarInfos.cpp$(PreprocessSuffix) "ToolbarInfos.cpp"

$(IntermediateDirectory)/ViewerFrame.cpp$(ObjectSuffix): ViewerFrame.cpp $(IntermediateDirectory)/ViewerFrame.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/ViewerFrame.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ViewerFrame.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ViewerFrame.cpp$(DependSuffix): ViewerFrame.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ViewerFrame.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ViewerFrame.cpp$(DependSuffix) -MM "ViewerFrame.cpp"

$(IntermediateDirectory)/ViewerFrame.cpp$(PreprocessSuffix): ViewerFrame.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ViewerFrame.cpp$(PreprocessSuffix) "ViewerFrame.cpp"

$(IntermediateDirectory)/ViewerParam.cpp$(ObjectSuffix): ViewerParam.cpp $(IntermediateDirectory)/ViewerParam.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/ViewerParam.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ViewerParam.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ViewerParam.cpp$(DependSuffix): ViewerParam.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ViewerParam.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ViewerParam.cpp$(DependSuffix) -MM "ViewerParam.cpp"

$(IntermediateDirectory)/ViewerParam.cpp$(PreprocessSuffix): ViewerParam.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ViewerParam.cpp$(PreprocessSuffix) "ViewerParam.cpp"

$(IntermediateDirectory)/ViewerParamInit.cpp$(ObjectSuffix): ViewerParamInit.cpp $(IntermediateDirectory)/ViewerParamInit.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/ViewerParamInit.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ViewerParamInit.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ViewerParamInit.cpp$(DependSuffix): ViewerParamInit.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ViewerParamInit.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ViewerParamInit.cpp$(DependSuffix) -MM "ViewerParamInit.cpp"

$(IntermediateDirectory)/ViewerParamInit.cpp$(PreprocessSuffix): ViewerParamInit.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ViewerParamInit.cpp$(PreprocessSuffix) "ViewerParamInit.cpp"

$(IntermediateDirectory)/ViewerTheme.cpp$(ObjectSuffix): ViewerTheme.cpp $(IntermediateDirectory)/ViewerTheme.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/ViewerTheme.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ViewerTheme.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ViewerTheme.cpp$(DependSuffix): ViewerTheme.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ViewerTheme.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ViewerTheme.cpp$(DependSuffix) -MM "ViewerTheme.cpp"

$(IntermediateDirectory)/ViewerTheme.cpp$(PreprocessSuffix): ViewerTheme.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ViewerTheme.cpp$(PreprocessSuffix) "ViewerTheme.cpp"

$(IntermediateDirectory)/ViewerThemeInit.cpp$(ObjectSuffix): ViewerThemeInit.cpp $(IntermediateDirectory)/ViewerThemeInit.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/ViewerThemeInit.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ViewerThemeInit.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ViewerThemeInit.cpp$(DependSuffix): ViewerThemeInit.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ViewerThemeInit.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ViewerThemeInit.cpp$(DependSuffix) -MM "ViewerThemeInit.cpp"

$(IntermediateDirectory)/ViewerThemeInit.cpp$(PreprocessSuffix): ViewerThemeInit.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ViewerThemeInit.cpp$(PreprocessSuffix) "ViewerThemeInit.cpp"

$(IntermediateDirectory)/BrightAndContrastFilter.cpp$(ObjectSuffix): BrightAndContrastFilter.cpp $(IntermediateDirectory)/BrightAndContrastFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/BrightAndContrastFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BrightAndContrastFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BrightAndContrastFilter.cpp$(DependSuffix): BrightAndContrastFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BrightAndContrastFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BrightAndContrastFilter.cpp$(DependSuffix) -MM "BrightAndContrastFilter.cpp"

$(IntermediateDirectory)/BrightAndContrastFilter.cpp$(PreprocessSuffix): BrightAndContrastFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BrightAndContrastFilter.cpp$(PreprocessSuffix) "BrightAndContrastFilter.cpp"

$(IntermediateDirectory)/CategoryFolderWindow.cpp$(ObjectSuffix): CategoryFolderWindow.cpp $(IntermediateDirectory)/CategoryFolderWindow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/CategoryFolderWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CategoryFolderWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CategoryFolderWindow.cpp$(DependSuffix): CategoryFolderWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CategoryFolderWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CategoryFolderWindow.cpp$(DependSuffix) -MM "CategoryFolderWindow.cpp"

$(IntermediateDirectory)/CategoryFolderWindow.cpp$(PreprocessSuffix): CategoryFolderWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CategoryFolderWindow.cpp$(PreprocessSuffix) "CategoryFolderWindow.cpp"

$(IntermediateDirectory)/CategoryWnd.cpp$(ObjectSuffix): CategoryWnd.cpp $(IntermediateDirectory)/CategoryWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/CategoryWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CategoryWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CategoryWnd.cpp$(DependSuffix): CategoryWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CategoryWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CategoryWnd.cpp$(DependSuffix) -MM "CategoryWnd.cpp"

$(IntermediateDirectory)/CategoryWnd.cpp$(PreprocessSuffix): CategoryWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CategoryWnd.cpp$(PreprocessSuffix) "CategoryWnd.cpp"

$(IntermediateDirectory)/CloudsFilter.cpp$(ObjectSuffix): CloudsFilter.cpp $(IntermediateDirectory)/CloudsFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/CloudsFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CloudsFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CloudsFilter.cpp$(DependSuffix): CloudsFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CloudsFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CloudsFilter.cpp$(DependSuffix) -MM "CloudsFilter.cpp"

$(IntermediateDirectory)/CloudsFilter.cpp$(PreprocessSuffix): CloudsFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CloudsFilter.cpp$(PreprocessSuffix) "CloudsFilter.cpp"

$(IntermediateDirectory)/ColorBalanceFilter.cpp$(ObjectSuffix): ColorBalanceFilter.cpp $(IntermediateDirectory)/ColorBalanceFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/ColorBalanceFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ColorBalanceFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ColorBalanceFilter.cpp$(DependSuffix): ColorBalanceFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ColorBalanceFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ColorBalanceFilter.cpp$(DependSuffix) -MM "ColorBalanceFilter.cpp"

$(IntermediateDirectory)/ColorBalanceFilter.cpp$(PreprocessSuffix): ColorBalanceFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ColorBalanceFilter.cpp$(PreprocessSuffix) "ColorBalanceFilter.cpp"

$(IntermediateDirectory)/FilterPreviewSplitter.cpp$(ObjectSuffix): FilterPreviewSplitter.cpp $(IntermediateDirectory)/FilterPreviewSplitter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/FilterPreviewSplitter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FilterPreviewSplitter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FilterPreviewSplitter.cpp$(DependSuffix): FilterPreviewSplitter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FilterPreviewSplitter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FilterPreviewSplitter.cpp$(DependSuffix) -MM "FilterPreviewSplitter.cpp"

$(IntermediateDirectory)/FilterPreviewSplitter.cpp$(PreprocessSuffix): FilterPreviewSplitter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FilterPreviewSplitter.cpp$(PreprocessSuffix) "FilterPreviewSplitter.cpp"

$(IntermediateDirectory)/FilterWindowParam.cpp$(ObjectSuffix): FilterWindowParam.cpp $(IntermediateDirectory)/FilterWindowParam.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/FilterWindowParam.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FilterWindowParam.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FilterWindowParam.cpp$(DependSuffix): FilterWindowParam.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FilterWindowParam.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FilterWindowParam.cpp$(DependSuffix) -MM "FilterWindowParam.cpp"

$(IntermediateDirectory)/FilterWindowParam.cpp$(PreprocessSuffix): FilterWindowParam.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FilterWindowParam.cpp$(PreprocessSuffix) "FilterWindowParam.cpp"

$(IntermediateDirectory)/FiltreEffectWnd.cpp$(ObjectSuffix): FiltreEffectWnd.cpp $(IntermediateDirectory)/FiltreEffectWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/FiltreEffectWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FiltreEffectWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FiltreEffectWnd.cpp$(DependSuffix): FiltreEffectWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FiltreEffectWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FiltreEffectWnd.cpp$(DependSuffix) -MM "FiltreEffectWnd.cpp"

$(IntermediateDirectory)/FiltreEffectWnd.cpp$(PreprocessSuffix): FiltreEffectWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FiltreEffectWnd.cpp$(PreprocessSuffix) "FiltreEffectWnd.cpp"

$(IntermediateDirectory)/InfoEffectWnd.cpp$(ObjectSuffix): InfoEffectWnd.cpp $(IntermediateDirectory)/InfoEffectWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/InfoEffectWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InfoEffectWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InfoEffectWnd.cpp$(DependSuffix): InfoEffectWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InfoEffectWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InfoEffectWnd.cpp$(DependSuffix) -MM "InfoEffectWnd.cpp"

$(IntermediateDirectory)/InfoEffectWnd.cpp$(PreprocessSuffix): InfoEffectWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InfoEffectWnd.cpp$(PreprocessSuffix) "InfoEffectWnd.cpp"

$(IntermediateDirectory)/LensFlareFilter.cpp$(ObjectSuffix): LensFlareFilter.cpp $(IntermediateDirectory)/LensFlareFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/LensFlareFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LensFlareFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LensFlareFilter.cpp$(DependSuffix): LensFlareFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LensFlareFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LensFlareFilter.cpp$(DependSuffix) -MM "LensFlareFilter.cpp"

$(IntermediateDirectory)/LensFlareFilter.cpp$(PreprocessSuffix): LensFlareFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LensFlareFilter.cpp$(PreprocessSuffix) "LensFlareFilter.cpp"

$(IntermediateDirectory)/MotionBlurFilter.cpp$(ObjectSuffix): MotionBlurFilter.cpp $(IntermediateDirectory)/MotionBlurFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/MotionBlurFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MotionBlurFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MotionBlurFilter.cpp$(DependSuffix): MotionBlurFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MotionBlurFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MotionBlurFilter.cpp$(DependSuffix) -MM "MotionBlurFilter.cpp"

$(IntermediateDirectory)/MotionBlurFilter.cpp$(PreprocessSuffix): MotionBlurFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MotionBlurFilter.cpp$(PreprocessSuffix) "MotionBlurFilter.cpp"

$(IntermediateDirectory)/PhotoFiltreFilter.cpp$(ObjectSuffix): PhotoFiltreFilter.cpp $(IntermediateDirectory)/PhotoFiltreFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/PhotoFiltreFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PhotoFiltreFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PhotoFiltreFilter.cpp$(DependSuffix): PhotoFiltreFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PhotoFiltreFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PhotoFiltreFilter.cpp$(DependSuffix) -MM "PhotoFiltreFilter.cpp"

$(IntermediateDirectory)/PhotoFiltreFilter.cpp$(PreprocessSuffix): PhotoFiltreFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PhotoFiltreFilter.cpp$(PreprocessSuffix) "PhotoFiltreFilter.cpp"

$(IntermediateDirectory)/PosterisationFilter.cpp$(ObjectSuffix): PosterisationFilter.cpp $(IntermediateDirectory)/PosterisationFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/PosterisationFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PosterisationFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PosterisationFilter.cpp$(DependSuffix): PosterisationFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PosterisationFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PosterisationFilter.cpp$(DependSuffix) -MM "PosterisationFilter.cpp"

$(IntermediateDirectory)/PosterisationFilter.cpp$(PreprocessSuffix): PosterisationFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PosterisationFilter.cpp$(PreprocessSuffix) "PosterisationFilter.cpp"

$(IntermediateDirectory)/RotateFreeFilter.cpp$(ObjectSuffix): RotateFreeFilter.cpp $(IntermediateDirectory)/RotateFreeFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/RotateFreeFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RotateFreeFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RotateFreeFilter.cpp$(DependSuffix): RotateFreeFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RotateFreeFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RotateFreeFilter.cpp$(DependSuffix) -MM "RotateFreeFilter.cpp"

$(IntermediateDirectory)/RotateFreeFilter.cpp$(PreprocessSuffix): RotateFreeFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RotateFreeFilter.cpp$(PreprocessSuffix) "RotateFreeFilter.cpp"

$(IntermediateDirectory)/SolarisationFilter.cpp$(ObjectSuffix): SolarisationFilter.cpp $(IntermediateDirectory)/SolarisationFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/SolarisationFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SolarisationFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SolarisationFilter.cpp$(DependSuffix): SolarisationFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SolarisationFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SolarisationFilter.cpp$(DependSuffix) -MM "SolarisationFilter.cpp"

$(IntermediateDirectory)/SolarisationFilter.cpp$(PreprocessSuffix): SolarisationFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SolarisationFilter.cpp$(PreprocessSuffix) "SolarisationFilter.cpp"

$(IntermediateDirectory)/SwirlFilter.cpp$(ObjectSuffix): SwirlFilter.cpp $(IntermediateDirectory)/SwirlFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/SwirlFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SwirlFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SwirlFilter.cpp$(DependSuffix): SwirlFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SwirlFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SwirlFilter.cpp$(DependSuffix) -MM "SwirlFilter.cpp"

$(IntermediateDirectory)/SwirlFilter.cpp$(PreprocessSuffix): SwirlFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SwirlFilter.cpp$(PreprocessSuffix) "SwirlFilter.cpp"

$(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(ObjectSuffix): ThumbnailViewerEffectWnd.cpp $(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/ThumbnailViewerEffectWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(DependSuffix): ThumbnailViewerEffectWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(DependSuffix) -MM "ThumbnailViewerEffectWnd.cpp"

$(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(PreprocessSuffix): ThumbnailViewerEffectWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(PreprocessSuffix) "ThumbnailViewerEffectWnd.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


