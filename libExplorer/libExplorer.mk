##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libExplorer
ConfigurationName      :=Release
WorkspacePath          := "/home/figuinha/dev/Regards"
ProjectPath            := "/home/figuinha/dev/Regards/libExplorer"
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=24/04/16
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
Preprocessors          :=$(PreprocessorSwitch)NDEBUG $(PreprocessorSwitch)__WXGTK__ 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libExplorer.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libTheme $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libextern/ffmpeg-2.6.2 $(IncludeSwitch)../libextern/SDL2-2.0.3/include $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libParameter $(IncludeSwitch)../libextern/exiv2-0.24/src $(IncludeSwitch)../libextern/exiv2-0.24/xmpsdk/include $(IncludeSwitch)../libUtility $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libResource $(IncludeSwitch)../libSqlData $(IncludeSwitch)../libOpenGL $(IncludeSwitch)../libRenderBitmap $(IncludeSwitch)../libSqlEngine $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libextern/glew-1.13.0/include $(IncludeSwitch)../libFiltre $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../Otherlib/wxSVG/include $(IncludeSwitch)../libWindow $(IncludeSwitch)../libPrint $(IncludeSwitch)../libPicture $(IncludeSwitch)../libTheme $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libControl $(IncludeSwitch)../libIntro $(IncludeSwitch)../libDialog $(IncludeSwitch)../libWindowMain $(IncludeSwitch)../libVideoFFMpeg $(IncludeSwitch)../libResource $(IncludeSwitch)../libextern/wxWidgets-3.0.2/include $(IncludeSwitch)../libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0 
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
CXXFLAGS :=  -fopenmp -std=c++11 -Wall  $(Preprocessors)
CFLAGS   :=  -O2 -Wall  $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/CategoryFolderWindow.cpp$(ObjectSuffix) $(IntermediateDirectory)/CategoryWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/CentralWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/ExplorerFrame.cpp$(ObjectSuffix) $(IntermediateDirectory)/ExplorerParam.cpp$(ObjectSuffix) $(IntermediateDirectory)/ExplorerParamInit.cpp$(ObjectSuffix) $(IntermediateDirectory)/ExplorerTheme.cpp$(ObjectSuffix) $(IntermediateDirectory)/ExplorerThemeInit.cpp$(ObjectSuffix) $(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(ObjectSuffix) $(IntermediateDirectory)/InfosToolbar.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/ListPicture.cpp$(ObjectSuffix) $(IntermediateDirectory)/MainToolbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/MainWindow.cpp$(ObjectSuffix) $(IntermediateDirectory)/PanelInfosWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/PanelPreview.cpp$(ObjectSuffix) $(IntermediateDirectory)/PreviewThumbnail.cpp$(ObjectSuffix) $(IntermediateDirectory)/PreviewToolbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/PreviewWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailCriteria.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailToolBar.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeDataCategory.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "/home/figuinha/dev/Regards/.build-release"
	@echo rebuilt > "/home/figuinha/dev/Regards/.build-release/libExplorer"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


./Release:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/CategoryFolderWindow.cpp$(ObjectSuffix): CategoryFolderWindow.cpp $(IntermediateDirectory)/CategoryFolderWindow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libExplorer/CategoryFolderWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CategoryFolderWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CategoryFolderWindow.cpp$(DependSuffix): CategoryFolderWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CategoryFolderWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CategoryFolderWindow.cpp$(DependSuffix) -MM "CategoryFolderWindow.cpp"

$(IntermediateDirectory)/CategoryFolderWindow.cpp$(PreprocessSuffix): CategoryFolderWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CategoryFolderWindow.cpp$(PreprocessSuffix) "CategoryFolderWindow.cpp"

$(IntermediateDirectory)/CategoryWnd.cpp$(ObjectSuffix): CategoryWnd.cpp $(IntermediateDirectory)/CategoryWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libExplorer/CategoryWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CategoryWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CategoryWnd.cpp$(DependSuffix): CategoryWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CategoryWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CategoryWnd.cpp$(DependSuffix) -MM "CategoryWnd.cpp"

$(IntermediateDirectory)/CategoryWnd.cpp$(PreprocessSuffix): CategoryWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CategoryWnd.cpp$(PreprocessSuffix) "CategoryWnd.cpp"

$(IntermediateDirectory)/CentralWnd.cpp$(ObjectSuffix): CentralWnd.cpp $(IntermediateDirectory)/CentralWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libExplorer/CentralWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CentralWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CentralWnd.cpp$(DependSuffix): CentralWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CentralWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CentralWnd.cpp$(DependSuffix) -MM "CentralWnd.cpp"

$(IntermediateDirectory)/CentralWnd.cpp$(PreprocessSuffix): CentralWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CentralWnd.cpp$(PreprocessSuffix) "CentralWnd.cpp"

$(IntermediateDirectory)/ExplorerFrame.cpp$(ObjectSuffix): ExplorerFrame.cpp $(IntermediateDirectory)/ExplorerFrame.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libExplorer/ExplorerFrame.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ExplorerFrame.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ExplorerFrame.cpp$(DependSuffix): ExplorerFrame.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ExplorerFrame.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ExplorerFrame.cpp$(DependSuffix) -MM "ExplorerFrame.cpp"

$(IntermediateDirectory)/ExplorerFrame.cpp$(PreprocessSuffix): ExplorerFrame.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ExplorerFrame.cpp$(PreprocessSuffix) "ExplorerFrame.cpp"

$(IntermediateDirectory)/ExplorerParam.cpp$(ObjectSuffix): ExplorerParam.cpp $(IntermediateDirectory)/ExplorerParam.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libExplorer/ExplorerParam.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ExplorerParam.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ExplorerParam.cpp$(DependSuffix): ExplorerParam.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ExplorerParam.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ExplorerParam.cpp$(DependSuffix) -MM "ExplorerParam.cpp"

$(IntermediateDirectory)/ExplorerParam.cpp$(PreprocessSuffix): ExplorerParam.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ExplorerParam.cpp$(PreprocessSuffix) "ExplorerParam.cpp"

$(IntermediateDirectory)/ExplorerParamInit.cpp$(ObjectSuffix): ExplorerParamInit.cpp $(IntermediateDirectory)/ExplorerParamInit.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libExplorer/ExplorerParamInit.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ExplorerParamInit.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ExplorerParamInit.cpp$(DependSuffix): ExplorerParamInit.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ExplorerParamInit.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ExplorerParamInit.cpp$(DependSuffix) -MM "ExplorerParamInit.cpp"

$(IntermediateDirectory)/ExplorerParamInit.cpp$(PreprocessSuffix): ExplorerParamInit.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ExplorerParamInit.cpp$(PreprocessSuffix) "ExplorerParamInit.cpp"

$(IntermediateDirectory)/ExplorerTheme.cpp$(ObjectSuffix): ExplorerTheme.cpp $(IntermediateDirectory)/ExplorerTheme.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libExplorer/ExplorerTheme.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ExplorerTheme.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ExplorerTheme.cpp$(DependSuffix): ExplorerTheme.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ExplorerTheme.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ExplorerTheme.cpp$(DependSuffix) -MM "ExplorerTheme.cpp"

$(IntermediateDirectory)/ExplorerTheme.cpp$(PreprocessSuffix): ExplorerTheme.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ExplorerTheme.cpp$(PreprocessSuffix) "ExplorerTheme.cpp"

$(IntermediateDirectory)/ExplorerThemeInit.cpp$(ObjectSuffix): ExplorerThemeInit.cpp $(IntermediateDirectory)/ExplorerThemeInit.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libExplorer/ExplorerThemeInit.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ExplorerThemeInit.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ExplorerThemeInit.cpp$(DependSuffix): ExplorerThemeInit.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ExplorerThemeInit.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ExplorerThemeInit.cpp$(DependSuffix) -MM "ExplorerThemeInit.cpp"

$(IntermediateDirectory)/ExplorerThemeInit.cpp$(PreprocessSuffix): ExplorerThemeInit.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ExplorerThemeInit.cpp$(PreprocessSuffix) "ExplorerThemeInit.cpp"

$(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(ObjectSuffix): InfosSeparationBarExplorer.cpp $(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libExplorer/InfosSeparationBarExplorer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(DependSuffix): InfosSeparationBarExplorer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(DependSuffix) -MM "InfosSeparationBarExplorer.cpp"

$(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(PreprocessSuffix): InfosSeparationBarExplorer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(PreprocessSuffix) "InfosSeparationBarExplorer.cpp"

$(IntermediateDirectory)/InfosToolbar.cpp$(ObjectSuffix): InfosToolbar.cpp $(IntermediateDirectory)/InfosToolbar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libExplorer/InfosToolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InfosToolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InfosToolbar.cpp$(DependSuffix): InfosToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InfosToolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InfosToolbar.cpp$(DependSuffix) -MM "InfosToolbar.cpp"

$(IntermediateDirectory)/InfosToolbar.cpp$(PreprocessSuffix): InfosToolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InfosToolbar.cpp$(PreprocessSuffix) "InfosToolbar.cpp"

$(IntermediateDirectory)/ListPicture.cpp$(ObjectSuffix): ListPicture.cpp $(IntermediateDirectory)/ListPicture.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libExplorer/ListPicture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ListPicture.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ListPicture.cpp$(DependSuffix): ListPicture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ListPicture.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ListPicture.cpp$(DependSuffix) -MM "ListPicture.cpp"

$(IntermediateDirectory)/ListPicture.cpp$(PreprocessSuffix): ListPicture.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ListPicture.cpp$(PreprocessSuffix) "ListPicture.cpp"

$(IntermediateDirectory)/MainToolbar.cpp$(ObjectSuffix): MainToolbar.cpp $(IntermediateDirectory)/MainToolbar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libExplorer/MainToolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MainToolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MainToolbar.cpp$(DependSuffix): MainToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MainToolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MainToolbar.cpp$(DependSuffix) -MM "MainToolbar.cpp"

$(IntermediateDirectory)/MainToolbar.cpp$(PreprocessSuffix): MainToolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MainToolbar.cpp$(PreprocessSuffix) "MainToolbar.cpp"

$(IntermediateDirectory)/MainWindow.cpp$(ObjectSuffix): MainWindow.cpp $(IntermediateDirectory)/MainWindow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libExplorer/MainWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MainWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MainWindow.cpp$(DependSuffix): MainWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MainWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MainWindow.cpp$(DependSuffix) -MM "MainWindow.cpp"

$(IntermediateDirectory)/MainWindow.cpp$(PreprocessSuffix): MainWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MainWindow.cpp$(PreprocessSuffix) "MainWindow.cpp"

$(IntermediateDirectory)/PanelInfosWnd.cpp$(ObjectSuffix): PanelInfosWnd.cpp $(IntermediateDirectory)/PanelInfosWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libExplorer/PanelInfosWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PanelInfosWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PanelInfosWnd.cpp$(DependSuffix): PanelInfosWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PanelInfosWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PanelInfosWnd.cpp$(DependSuffix) -MM "PanelInfosWnd.cpp"

$(IntermediateDirectory)/PanelInfosWnd.cpp$(PreprocessSuffix): PanelInfosWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PanelInfosWnd.cpp$(PreprocessSuffix) "PanelInfosWnd.cpp"

$(IntermediateDirectory)/PanelPreview.cpp$(ObjectSuffix): PanelPreview.cpp $(IntermediateDirectory)/PanelPreview.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libExplorer/PanelPreview.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PanelPreview.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PanelPreview.cpp$(DependSuffix): PanelPreview.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PanelPreview.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PanelPreview.cpp$(DependSuffix) -MM "PanelPreview.cpp"

$(IntermediateDirectory)/PanelPreview.cpp$(PreprocessSuffix): PanelPreview.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PanelPreview.cpp$(PreprocessSuffix) "PanelPreview.cpp"

$(IntermediateDirectory)/PreviewThumbnail.cpp$(ObjectSuffix): PreviewThumbnail.cpp $(IntermediateDirectory)/PreviewThumbnail.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libExplorer/PreviewThumbnail.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PreviewThumbnail.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PreviewThumbnail.cpp$(DependSuffix): PreviewThumbnail.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PreviewThumbnail.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PreviewThumbnail.cpp$(DependSuffix) -MM "PreviewThumbnail.cpp"

$(IntermediateDirectory)/PreviewThumbnail.cpp$(PreprocessSuffix): PreviewThumbnail.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PreviewThumbnail.cpp$(PreprocessSuffix) "PreviewThumbnail.cpp"

$(IntermediateDirectory)/PreviewToolbar.cpp$(ObjectSuffix): PreviewToolbar.cpp $(IntermediateDirectory)/PreviewToolbar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libExplorer/PreviewToolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PreviewToolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PreviewToolbar.cpp$(DependSuffix): PreviewToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PreviewToolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PreviewToolbar.cpp$(DependSuffix) -MM "PreviewToolbar.cpp"

$(IntermediateDirectory)/PreviewToolbar.cpp$(PreprocessSuffix): PreviewToolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PreviewToolbar.cpp$(PreprocessSuffix) "PreviewToolbar.cpp"

$(IntermediateDirectory)/PreviewWnd.cpp$(ObjectSuffix): PreviewWnd.cpp $(IntermediateDirectory)/PreviewWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libExplorer/PreviewWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PreviewWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PreviewWnd.cpp$(DependSuffix): PreviewWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PreviewWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PreviewWnd.cpp$(DependSuffix) -MM "PreviewWnd.cpp"

$(IntermediateDirectory)/PreviewWnd.cpp$(PreprocessSuffix): PreviewWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PreviewWnd.cpp$(PreprocessSuffix) "PreviewWnd.cpp"

$(IntermediateDirectory)/ThumbnailCriteria.cpp$(ObjectSuffix): ThumbnailCriteria.cpp $(IntermediateDirectory)/ThumbnailCriteria.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libExplorer/ThumbnailCriteria.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailCriteria.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailCriteria.cpp$(DependSuffix): ThumbnailCriteria.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailCriteria.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailCriteria.cpp$(DependSuffix) -MM "ThumbnailCriteria.cpp"

$(IntermediateDirectory)/ThumbnailCriteria.cpp$(PreprocessSuffix): ThumbnailCriteria.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailCriteria.cpp$(PreprocessSuffix) "ThumbnailCriteria.cpp"

$(IntermediateDirectory)/ThumbnailToolBar.cpp$(ObjectSuffix): ThumbnailToolBar.cpp $(IntermediateDirectory)/ThumbnailToolBar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libExplorer/ThumbnailToolBar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailToolBar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailToolBar.cpp$(DependSuffix): ThumbnailToolBar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailToolBar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailToolBar.cpp$(DependSuffix) -MM "ThumbnailToolBar.cpp"

$(IntermediateDirectory)/ThumbnailToolBar.cpp$(PreprocessSuffix): ThumbnailToolBar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailToolBar.cpp$(PreprocessSuffix) "ThumbnailToolBar.cpp"

$(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(ObjectSuffix): ThumbnailToolBarZoom.cpp $(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libExplorer/ThumbnailToolBarZoom.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(DependSuffix): ThumbnailToolBarZoom.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(DependSuffix) -MM "ThumbnailToolBarZoom.cpp"

$(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(PreprocessSuffix): ThumbnailToolBarZoom.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(PreprocessSuffix) "ThumbnailToolBarZoom.cpp"

$(IntermediateDirectory)/TreeDataCategory.cpp$(ObjectSuffix): TreeDataCategory.cpp $(IntermediateDirectory)/TreeDataCategory.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libExplorer/TreeDataCategory.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeDataCategory.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeDataCategory.cpp$(DependSuffix): TreeDataCategory.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeDataCategory.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeDataCategory.cpp$(DependSuffix) -MM "TreeDataCategory.cpp"

$(IntermediateDirectory)/TreeDataCategory.cpp$(PreprocessSuffix): TreeDataCategory.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeDataCategory.cpp$(PreprocessSuffix) "TreeDataCategory.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


