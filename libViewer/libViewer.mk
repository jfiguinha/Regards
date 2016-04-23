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
User                   :=root
Date                   :=22/09/15
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
Preprocessors          :=$(PreprocessorSwitch)__WXGTK__ 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libViewer.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libTheme $(IncludeSwitch)../../libextern/libRapidXML $(IncludeSwitch)../../libextern/ffmpeg-2.6.2 $(IncludeSwitch)../../libextern/SDL2-2.0.3/include $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libParameter $(IncludeSwitch)../../libextern/exiv2-0.24/src $(IncludeSwitch)../../libextern/exiv2-0.24/xmpsdk/include $(IncludeSwitch)../libUtility $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libResource $(IncludeSwitch)../libSqlData $(IncludeSwitch)../libOpenGL $(IncludeSwitch)../libRenderBitmap $(IncludeSwitch)../libSqlEngine $(IncludeSwitch)../libSqlite $(IncludeSwitch)../../libextern/glew-1.13.0/include $(IncludeSwitch)../libFiltre $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../Otherlib/wxSVG/include $(IncludeSwitch)../libWindow $(IncludeSwitch)../libPrint $(IncludeSwitch)../libPicture $(IncludeSwitch)../libTheme $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libControl $(IncludeSwitch)../libIntro $(IncludeSwitch)../libDialog $(IncludeSwitch)../libWindowMain $(IncludeSwitch)../libVideoFFMpeg $(IncludeSwitch)../../libextern/wxWidgets-3.0.2/include $(IncludeSwitch)../../libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0 
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
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CentralWnd.cpp$(PreprocessSuffix) "CentralWnd.cpp"

$(IntermediateDirectory)/FiltreEffect.cpp$(ObjectSuffix): FiltreEffect.cpp $(IntermediateDirectory)/FiltreEffect.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/FiltreEffect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FiltreEffect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FiltreEffect.cpp$(DependSuffix): FiltreEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FiltreEffect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FiltreEffect.cpp$(DependSuffix) -MM "FiltreEffect.cpp"

$(IntermediateDirectory)/FiltreEffect.cpp$(PreprocessSuffix): FiltreEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FiltreEffect.cpp$(PreprocessSuffix) "FiltreEffect.cpp"

$(IntermediateDirectory)/FiltreToolbar.cpp$(ObjectSuffix): FiltreToolbar.cpp $(IntermediateDirectory)/FiltreToolbar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/FiltreToolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FiltreToolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FiltreToolbar.cpp$(DependSuffix): FiltreToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FiltreToolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FiltreToolbar.cpp$(DependSuffix) -MM "FiltreToolbar.cpp"

$(IntermediateDirectory)/FiltreToolbar.cpp$(PreprocessSuffix): FiltreToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FiltreToolbar.cpp$(PreprocessSuffix) "FiltreToolbar.cpp"

$(IntermediateDirectory)/InfoEffect.cpp$(ObjectSuffix): InfoEffect.cpp $(IntermediateDirectory)/InfoEffect.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/InfoEffect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InfoEffect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InfoEffect.cpp$(DependSuffix): InfoEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InfoEffect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InfoEffect.cpp$(DependSuffix) -MM "InfoEffect.cpp"

$(IntermediateDirectory)/InfoEffect.cpp$(PreprocessSuffix): InfoEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InfoEffect.cpp$(PreprocessSuffix) "InfoEffect.cpp"

$(IntermediateDirectory)/MainWindow.cpp$(ObjectSuffix): MainWindow.cpp $(IntermediateDirectory)/MainWindow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/MainWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MainWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MainWindow.cpp$(DependSuffix): MainWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MainWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MainWindow.cpp$(DependSuffix) -MM "MainWindow.cpp"

$(IntermediateDirectory)/MainWindow.cpp$(PreprocessSuffix): MainWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MainWindow.cpp$(PreprocessSuffix) "MainWindow.cpp"

$(IntermediateDirectory)/ModificationManager.cpp$(ObjectSuffix): ModificationManager.cpp $(IntermediateDirectory)/ModificationManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/ModificationManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ModificationManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ModificationManager.cpp$(DependSuffix): ModificationManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ModificationManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ModificationManager.cpp$(DependSuffix) -MM "ModificationManager.cpp"

$(IntermediateDirectory)/ModificationManager.cpp$(PreprocessSuffix): ModificationManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ModificationManager.cpp$(PreprocessSuffix) "ModificationManager.cpp"

$(IntermediateDirectory)/PanelInfosWnd.cpp$(ObjectSuffix): PanelInfosWnd.cpp $(IntermediateDirectory)/PanelInfosWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/PanelInfosWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PanelInfosWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PanelInfosWnd.cpp$(DependSuffix): PanelInfosWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PanelInfosWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PanelInfosWnd.cpp$(DependSuffix) -MM "PanelInfosWnd.cpp"

$(IntermediateDirectory)/PanelInfosWnd.cpp$(PreprocessSuffix): PanelInfosWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PanelInfosWnd.cpp$(PreprocessSuffix) "PanelInfosWnd.cpp"

$(IntermediateDirectory)/PreviewInfosWnd.cpp$(ObjectSuffix): PreviewInfosWnd.cpp $(IntermediateDirectory)/PreviewInfosWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/PreviewInfosWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PreviewInfosWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PreviewInfosWnd.cpp$(DependSuffix): PreviewInfosWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PreviewInfosWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PreviewInfosWnd.cpp$(DependSuffix) -MM "PreviewInfosWnd.cpp"

$(IntermediateDirectory)/PreviewInfosWnd.cpp$(PreprocessSuffix): PreviewInfosWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PreviewInfosWnd.cpp$(PreprocessSuffix) "PreviewInfosWnd.cpp"

$(IntermediateDirectory)/PreviewToolbar.cpp$(ObjectSuffix): PreviewToolbar.cpp $(IntermediateDirectory)/PreviewToolbar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/PreviewToolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PreviewToolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PreviewToolbar.cpp$(DependSuffix): PreviewToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PreviewToolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PreviewToolbar.cpp$(DependSuffix) -MM "PreviewToolbar.cpp"

$(IntermediateDirectory)/PreviewToolbar.cpp$(PreprocessSuffix): PreviewToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PreviewToolbar.cpp$(PreprocessSuffix) "PreviewToolbar.cpp"

$(IntermediateDirectory)/PreviewWnd.cpp$(ObjectSuffix): PreviewWnd.cpp $(IntermediateDirectory)/PreviewWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/PreviewWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PreviewWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PreviewWnd.cpp$(DependSuffix): PreviewWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PreviewWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PreviewWnd.cpp$(DependSuffix) -MM "PreviewWnd.cpp"

$(IntermediateDirectory)/PreviewWnd.cpp$(PreprocessSuffix): PreviewWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PreviewWnd.cpp$(PreprocessSuffix) "PreviewWnd.cpp"

$(IntermediateDirectory)/ThumbnailBitmap.cpp$(ObjectSuffix): ThumbnailBitmap.cpp $(IntermediateDirectory)/ThumbnailBitmap.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/ThumbnailBitmap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailBitmap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailBitmap.cpp$(DependSuffix): ThumbnailBitmap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailBitmap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailBitmap.cpp$(DependSuffix) -MM "ThumbnailBitmap.cpp"

$(IntermediateDirectory)/ThumbnailBitmap.cpp$(PreprocessSuffix): ThumbnailBitmap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailBitmap.cpp$(PreprocessSuffix) "ThumbnailBitmap.cpp"

$(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(ObjectSuffix): ThumbnailViewerEffect.cpp $(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/ThumbnailViewerEffect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(DependSuffix): ThumbnailViewerEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(DependSuffix) -MM "ThumbnailViewerEffect.cpp"

$(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(PreprocessSuffix): ThumbnailViewerEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(PreprocessSuffix) "ThumbnailViewerEffect.cpp"

$(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(ObjectSuffix): ThumbnailViewerVideo.cpp $(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/ThumbnailViewerVideo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(DependSuffix): ThumbnailViewerVideo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(DependSuffix) -MM "ThumbnailViewerVideo.cpp"

$(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(PreprocessSuffix): ThumbnailViewerVideo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(PreprocessSuffix) "ThumbnailViewerVideo.cpp"

$(IntermediateDirectory)/toolbar.cpp$(ObjectSuffix): toolbar.cpp $(IntermediateDirectory)/toolbar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/toolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/toolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/toolbar.cpp$(DependSuffix): toolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/toolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/toolbar.cpp$(DependSuffix) -MM "toolbar.cpp"

$(IntermediateDirectory)/toolbar.cpp$(PreprocessSuffix): toolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/toolbar.cpp$(PreprocessSuffix) "toolbar.cpp"

$(IntermediateDirectory)/ToolbarInfos.cpp$(ObjectSuffix): ToolbarInfos.cpp $(IntermediateDirectory)/ToolbarInfos.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/ToolbarInfos.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ToolbarInfos.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ToolbarInfos.cpp$(DependSuffix): ToolbarInfos.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ToolbarInfos.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ToolbarInfos.cpp$(DependSuffix) -MM "ToolbarInfos.cpp"

$(IntermediateDirectory)/ToolbarInfos.cpp$(PreprocessSuffix): ToolbarInfos.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ToolbarInfos.cpp$(PreprocessSuffix) "ToolbarInfos.cpp"

$(IntermediateDirectory)/ViewerFrame.cpp$(ObjectSuffix): ViewerFrame.cpp $(IntermediateDirectory)/ViewerFrame.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/ViewerFrame.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ViewerFrame.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ViewerFrame.cpp$(DependSuffix): ViewerFrame.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ViewerFrame.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ViewerFrame.cpp$(DependSuffix) -MM "ViewerFrame.cpp"

$(IntermediateDirectory)/ViewerFrame.cpp$(PreprocessSuffix): ViewerFrame.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ViewerFrame.cpp$(PreprocessSuffix) "ViewerFrame.cpp"

$(IntermediateDirectory)/ViewerParam.cpp$(ObjectSuffix): ViewerParam.cpp $(IntermediateDirectory)/ViewerParam.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/ViewerParam.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ViewerParam.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ViewerParam.cpp$(DependSuffix): ViewerParam.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ViewerParam.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ViewerParam.cpp$(DependSuffix) -MM "ViewerParam.cpp"

$(IntermediateDirectory)/ViewerParam.cpp$(PreprocessSuffix): ViewerParam.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ViewerParam.cpp$(PreprocessSuffix) "ViewerParam.cpp"

$(IntermediateDirectory)/ViewerParamInit.cpp$(ObjectSuffix): ViewerParamInit.cpp $(IntermediateDirectory)/ViewerParamInit.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/ViewerParamInit.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ViewerParamInit.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ViewerParamInit.cpp$(DependSuffix): ViewerParamInit.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ViewerParamInit.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ViewerParamInit.cpp$(DependSuffix) -MM "ViewerParamInit.cpp"

$(IntermediateDirectory)/ViewerParamInit.cpp$(PreprocessSuffix): ViewerParamInit.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ViewerParamInit.cpp$(PreprocessSuffix) "ViewerParamInit.cpp"

$(IntermediateDirectory)/ViewerTheme.cpp$(ObjectSuffix): ViewerTheme.cpp $(IntermediateDirectory)/ViewerTheme.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/ViewerTheme.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ViewerTheme.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ViewerTheme.cpp$(DependSuffix): ViewerTheme.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ViewerTheme.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ViewerTheme.cpp$(DependSuffix) -MM "ViewerTheme.cpp"

$(IntermediateDirectory)/ViewerTheme.cpp$(PreprocessSuffix): ViewerTheme.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ViewerTheme.cpp$(PreprocessSuffix) "ViewerTheme.cpp"

$(IntermediateDirectory)/ViewerThemeInit.cpp$(ObjectSuffix): ViewerThemeInit.cpp $(IntermediateDirectory)/ViewerThemeInit.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libViewer/ViewerThemeInit.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ViewerThemeInit.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ViewerThemeInit.cpp$(DependSuffix): ViewerThemeInit.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ViewerThemeInit.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ViewerThemeInit.cpp$(DependSuffix) -MM "ViewerThemeInit.cpp"

$(IntermediateDirectory)/ViewerThemeInit.cpp$(PreprocessSuffix): ViewerThemeInit.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ViewerThemeInit.cpp$(PreprocessSuffix) "ViewerThemeInit.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


