##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libWindow
ConfigurationName      :=Release
WorkspacePath          :=/Users/jacques/developpement/git/Regards
ProjectPath            :=/Users/jacques/developpement/git/Regards/libWindow
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha jacques
Date                   :=29/11/2021
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
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libWindow.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s
IncludePath            := $(IncludeSwitch)/opt/homebrew/include $(IncludeSwitch)/opt/homebrew/include/poppler/cpp $(IncludeSwitch)/opt/homebrew/include/SDL2 $(IncludeSwitch)/opt/homebrew/include/openjpeg-2.4 $(IncludeSwitch)/opt/homebrew/include/libde265 $(IncludeSwitch)/opt/homebrew/include/opencv4 $(IncludeSwitch)/opt/homebrew/include/OpenEXR $(IncludeSwitch)/opt/homebrew/opt/jpeg-turbo/include  $(IncludeSwitch). $(IncludeSwitch)../include $(IncludeSwitch)../libextern/glew-2.1.0/include $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libextern/SDL2-2.0.3/include $(IncludeSwitch)../libFiltre $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libParameter $(IncludeSwitch)../libPicture $(IncludeSwitch)../libResource $(IncludeSwitch)../libSqlData $(IncludeSwitch)../libOpenGL $(IncludeSwitch)../libOpenCLEngine $(IncludeSwitch)../libTheme $(IncludeSwitch)../libUtility $(IncludeSwitch)../libWindowMain $(IncludeSwitch)../Otherlib/wxSVG/include $(IncludeSwitch)../libextern/libOpenCL/include 
IncludePCH             :=  -include ../include/Release/header.h 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                :=$(LibraryPathSwitch)/opt/homebrew/lib $(LibraryPathSwitch)/opt/homebrew/opt/jpeg-turbo/lib  $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/clang++ -arch arm64
CC       := /usr/bin/clang -arch arm64
CXXFLAGS := $(shell ../libextern/wxconfig_cpp.sh) -Wall -pthread -std=gnu++14 -Wall -O3 $(Preprocessors)
CFLAGS   :=  -Wall -O3 $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/Applications/codelite.app/Contents/SharedSupport/
Objects0=$(IntermediateDirectory)/BitmapWndRender.cpp$(ObjectSuffix) $(IntermediateDirectory)/BitmapWnd3d.cpp$(ObjectSuffix) $(IntermediateDirectory)/BitmapWnd2d.cpp$(ObjectSuffix) $(IntermediateDirectory)/MapSelect.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeData.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeElementListBox.cpp$(ObjectSuffix) $(IntermediateDirectory)/ScrollbarWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/ScrollbarVerticalWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeElementTexteClick.cpp$(ObjectSuffix) $(IntermediateDirectory)/IconeList.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/ScrollbarHorizontalWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/MapDialogBox.cpp$(ObjectSuffix) $(IntermediateDirectory)/BitmapInfosCheckBox.cpp$(ObjectSuffix) $(IntermediateDirectory)/BitmapInfos.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeDataStars.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeElementTriangle.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeElementSlide.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeWindow.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeControl.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/ToolbarWindow.cpp$(ObjectSuffix) $(IntermediateDirectory)/ToolbarTexte.cpp$(ObjectSuffix) $(IntermediateDirectory)/PanelWithClickToolbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/wxImagePanel.cpp$(ObjectSuffix) $(IntermediateDirectory)/Thumbnail.cpp$(ObjectSuffix) $(IntermediateDirectory)/Icone.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeDataCategory.cpp$(ObjectSuffix) $(IntermediateDirectory)/SeparationBar.cpp$(ObjectSuffix) $(IntermediateDirectory)/Pane.cpp$(ObjectSuffix) $(IntermediateDirectory)/ToolbarSlide.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/PositionElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/EmptyWindow.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeElementDelete.cpp$(ObjectSuffix) $(IntermediateDirectory)/TabWindow.cpp$(ObjectSuffix) $(IntermediateDirectory)/WindowManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/CalendarDialogBox.cpp$(ObjectSuffix) $(IntermediateDirectory)/WindowToAdd.cpp$(ObjectSuffix) $(IntermediateDirectory)/CalendarSelect.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeDataKeyWord.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeElementStar.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/KeywordDialogBox.cpp$(ObjectSuffix) $(IntermediateDirectory)/ToolbarButton.cpp$(ObjectSuffix) $(IntermediateDirectory)/TitleBar.cpp$(ObjectSuffix) $(IntermediateDirectory)/Slider.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeElementCheckBox.cpp$(ObjectSuffix) 

Objects1=$(IntermediateDirectory)/BitmapInfosGps.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeWithScrollbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeDataLink.cpp$(ObjectSuffix) $(IntermediateDirectory)/Splitter.cpp$(ObjectSuffix) $(IntermediateDirectory)/ToolbarElement.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/InfosSeparationBar.cpp$(ObjectSuffix) $(IntermediateDirectory)/ClickToolbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeElementTexte.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeDataEffect.cpp$(ObjectSuffix) 



Objects=$(Objects0) $(Objects1) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(IntermediateDirectory) $(OutputFile)

$(OutputFile): $(Objects)
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	@echo $(Objects1) >> $(ObjectsFileList)
	$(AR) $(ArchiveOutputSwitch)$(OutputFile) $(Objects)
	@$(MakeDirCommand) "/Users/jacques/developpement/git/Regards/.build-release"
	@echo rebuilt > "/Users/jacques/developpement/git/Regards/.build-release/libWindow"

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
$(IntermediateDirectory)/BitmapWndRender.cpp$(ObjectSuffix): BitmapWndRender.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BitmapWndRender.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BitmapWndRender.cpp$(DependSuffix) -MM BitmapWndRender.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/BitmapWndRender.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BitmapWndRender.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BitmapWndRender.cpp$(PreprocessSuffix): BitmapWndRender.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BitmapWndRender.cpp$(PreprocessSuffix) BitmapWndRender.cpp

$(IntermediateDirectory)/BitmapWnd3d.cpp$(ObjectSuffix): BitmapWnd3d.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BitmapWnd3d.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BitmapWnd3d.cpp$(DependSuffix) -MM BitmapWnd3d.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/BitmapWnd3d.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BitmapWnd3d.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BitmapWnd3d.cpp$(PreprocessSuffix): BitmapWnd3d.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BitmapWnd3d.cpp$(PreprocessSuffix) BitmapWnd3d.cpp

$(IntermediateDirectory)/BitmapWnd2d.cpp$(ObjectSuffix): BitmapWnd2d.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BitmapWnd2d.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BitmapWnd2d.cpp$(DependSuffix) -MM BitmapWnd2d.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/BitmapWnd2d.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BitmapWnd2d.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BitmapWnd2d.cpp$(PreprocessSuffix): BitmapWnd2d.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BitmapWnd2d.cpp$(PreprocessSuffix) BitmapWnd2d.cpp

$(IntermediateDirectory)/MapSelect.cpp$(ObjectSuffix): MapSelect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MapSelect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MapSelect.cpp$(DependSuffix) -MM MapSelect.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/MapSelect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MapSelect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MapSelect.cpp$(PreprocessSuffix): MapSelect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MapSelect.cpp$(PreprocessSuffix) MapSelect.cpp

$(IntermediateDirectory)/TreeData.cpp$(ObjectSuffix): TreeData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeData.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeData.cpp$(DependSuffix) -MM TreeData.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/TreeData.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeData.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeData.cpp$(PreprocessSuffix): TreeData.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeData.cpp$(PreprocessSuffix) TreeData.cpp

$(IntermediateDirectory)/TreeElementListBox.cpp$(ObjectSuffix): TreeElementListBox.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeElementListBox.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeElementListBox.cpp$(DependSuffix) -MM TreeElementListBox.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/TreeElementListBox.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeElementListBox.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeElementListBox.cpp$(PreprocessSuffix): TreeElementListBox.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeElementListBox.cpp$(PreprocessSuffix) TreeElementListBox.cpp

$(IntermediateDirectory)/ScrollbarWnd.cpp$(ObjectSuffix): ScrollbarWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ScrollbarWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ScrollbarWnd.cpp$(DependSuffix) -MM ScrollbarWnd.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/ScrollbarWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ScrollbarWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ScrollbarWnd.cpp$(PreprocessSuffix): ScrollbarWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ScrollbarWnd.cpp$(PreprocessSuffix) ScrollbarWnd.cpp

$(IntermediateDirectory)/ScrollbarVerticalWnd.cpp$(ObjectSuffix): ScrollbarVerticalWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ScrollbarVerticalWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ScrollbarVerticalWnd.cpp$(DependSuffix) -MM ScrollbarVerticalWnd.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/ScrollbarVerticalWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ScrollbarVerticalWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ScrollbarVerticalWnd.cpp$(PreprocessSuffix): ScrollbarVerticalWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ScrollbarVerticalWnd.cpp$(PreprocessSuffix) ScrollbarVerticalWnd.cpp

$(IntermediateDirectory)/TreeElementTexteClick.cpp$(ObjectSuffix): TreeElementTexteClick.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeElementTexteClick.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeElementTexteClick.cpp$(DependSuffix) -MM TreeElementTexteClick.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/TreeElementTexteClick.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeElementTexteClick.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeElementTexteClick.cpp$(PreprocessSuffix): TreeElementTexteClick.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeElementTexteClick.cpp$(PreprocessSuffix) TreeElementTexteClick.cpp

$(IntermediateDirectory)/IconeList.cpp$(ObjectSuffix): IconeList.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IconeList.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IconeList.cpp$(DependSuffix) -MM IconeList.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/IconeList.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IconeList.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IconeList.cpp$(PreprocessSuffix): IconeList.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IconeList.cpp$(PreprocessSuffix) IconeList.cpp

$(IntermediateDirectory)/ScrollbarHorizontalWnd.cpp$(ObjectSuffix): ScrollbarHorizontalWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ScrollbarHorizontalWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ScrollbarHorizontalWnd.cpp$(DependSuffix) -MM ScrollbarHorizontalWnd.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/ScrollbarHorizontalWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ScrollbarHorizontalWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ScrollbarHorizontalWnd.cpp$(PreprocessSuffix): ScrollbarHorizontalWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ScrollbarHorizontalWnd.cpp$(PreprocessSuffix) ScrollbarHorizontalWnd.cpp

$(IntermediateDirectory)/MapDialogBox.cpp$(ObjectSuffix): MapDialogBox.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MapDialogBox.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MapDialogBox.cpp$(DependSuffix) -MM MapDialogBox.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/MapDialogBox.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MapDialogBox.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MapDialogBox.cpp$(PreprocessSuffix): MapDialogBox.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MapDialogBox.cpp$(PreprocessSuffix) MapDialogBox.cpp

$(IntermediateDirectory)/BitmapInfosCheckBox.cpp$(ObjectSuffix): BitmapInfosCheckBox.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BitmapInfosCheckBox.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BitmapInfosCheckBox.cpp$(DependSuffix) -MM BitmapInfosCheckBox.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/BitmapInfosCheckBox.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BitmapInfosCheckBox.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BitmapInfosCheckBox.cpp$(PreprocessSuffix): BitmapInfosCheckBox.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BitmapInfosCheckBox.cpp$(PreprocessSuffix) BitmapInfosCheckBox.cpp

$(IntermediateDirectory)/BitmapInfos.cpp$(ObjectSuffix): BitmapInfos.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BitmapInfos.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BitmapInfos.cpp$(DependSuffix) -MM BitmapInfos.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/BitmapInfos.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BitmapInfos.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BitmapInfos.cpp$(PreprocessSuffix): BitmapInfos.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BitmapInfos.cpp$(PreprocessSuffix) BitmapInfos.cpp

$(IntermediateDirectory)/TreeDataStars.cpp$(ObjectSuffix): TreeDataStars.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeDataStars.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeDataStars.cpp$(DependSuffix) -MM TreeDataStars.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/TreeDataStars.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeDataStars.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeDataStars.cpp$(PreprocessSuffix): TreeDataStars.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeDataStars.cpp$(PreprocessSuffix) TreeDataStars.cpp

$(IntermediateDirectory)/TreeElementTriangle.cpp$(ObjectSuffix): TreeElementTriangle.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeElementTriangle.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeElementTriangle.cpp$(DependSuffix) -MM TreeElementTriangle.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/TreeElementTriangle.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeElementTriangle.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeElementTriangle.cpp$(PreprocessSuffix): TreeElementTriangle.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeElementTriangle.cpp$(PreprocessSuffix) TreeElementTriangle.cpp

$(IntermediateDirectory)/TreeElementSlide.cpp$(ObjectSuffix): TreeElementSlide.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeElementSlide.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeElementSlide.cpp$(DependSuffix) -MM TreeElementSlide.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/TreeElementSlide.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeElementSlide.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeElementSlide.cpp$(PreprocessSuffix): TreeElementSlide.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeElementSlide.cpp$(PreprocessSuffix) TreeElementSlide.cpp

$(IntermediateDirectory)/TreeElement.cpp$(ObjectSuffix): TreeElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeElement.cpp$(DependSuffix) -MM TreeElement.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/TreeElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeElement.cpp$(PreprocessSuffix): TreeElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeElement.cpp$(PreprocessSuffix) TreeElement.cpp

$(IntermediateDirectory)/TreeWindow.cpp$(ObjectSuffix): TreeWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeWindow.cpp$(DependSuffix) -MM TreeWindow.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/TreeWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeWindow.cpp$(PreprocessSuffix): TreeWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeWindow.cpp$(PreprocessSuffix) TreeWindow.cpp

$(IntermediateDirectory)/TreeControl.cpp$(ObjectSuffix): TreeControl.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeControl.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeControl.cpp$(DependSuffix) -MM TreeControl.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/TreeControl.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeControl.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeControl.cpp$(PreprocessSuffix): TreeControl.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeControl.cpp$(PreprocessSuffix) TreeControl.cpp

$(IntermediateDirectory)/ToolbarWindow.cpp$(ObjectSuffix): ToolbarWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ToolbarWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ToolbarWindow.cpp$(DependSuffix) -MM ToolbarWindow.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/ToolbarWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ToolbarWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ToolbarWindow.cpp$(PreprocessSuffix): ToolbarWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ToolbarWindow.cpp$(PreprocessSuffix) ToolbarWindow.cpp

$(IntermediateDirectory)/ToolbarTexte.cpp$(ObjectSuffix): ToolbarTexte.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ToolbarTexte.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ToolbarTexte.cpp$(DependSuffix) -MM ToolbarTexte.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/ToolbarTexte.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ToolbarTexte.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ToolbarTexte.cpp$(PreprocessSuffix): ToolbarTexte.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ToolbarTexte.cpp$(PreprocessSuffix) ToolbarTexte.cpp

$(IntermediateDirectory)/PanelWithClickToolbar.cpp$(ObjectSuffix): PanelWithClickToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PanelWithClickToolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PanelWithClickToolbar.cpp$(DependSuffix) -MM PanelWithClickToolbar.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/PanelWithClickToolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PanelWithClickToolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PanelWithClickToolbar.cpp$(PreprocessSuffix): PanelWithClickToolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PanelWithClickToolbar.cpp$(PreprocessSuffix) PanelWithClickToolbar.cpp

$(IntermediateDirectory)/wxImagePanel.cpp$(ObjectSuffix): wxImagePanel.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/wxImagePanel.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/wxImagePanel.cpp$(DependSuffix) -MM wxImagePanel.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/wxImagePanel.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wxImagePanel.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wxImagePanel.cpp$(PreprocessSuffix): wxImagePanel.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wxImagePanel.cpp$(PreprocessSuffix) wxImagePanel.cpp

$(IntermediateDirectory)/Thumbnail.cpp$(ObjectSuffix): Thumbnail.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Thumbnail.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Thumbnail.cpp$(DependSuffix) -MM Thumbnail.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/Thumbnail.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Thumbnail.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Thumbnail.cpp$(PreprocessSuffix): Thumbnail.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Thumbnail.cpp$(PreprocessSuffix) Thumbnail.cpp

$(IntermediateDirectory)/Icone.cpp$(ObjectSuffix): Icone.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Icone.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Icone.cpp$(DependSuffix) -MM Icone.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/Icone.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Icone.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Icone.cpp$(PreprocessSuffix): Icone.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Icone.cpp$(PreprocessSuffix) Icone.cpp

$(IntermediateDirectory)/TreeDataCategory.cpp$(ObjectSuffix): TreeDataCategory.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeDataCategory.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeDataCategory.cpp$(DependSuffix) -MM TreeDataCategory.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/TreeDataCategory.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeDataCategory.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeDataCategory.cpp$(PreprocessSuffix): TreeDataCategory.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeDataCategory.cpp$(PreprocessSuffix) TreeDataCategory.cpp

$(IntermediateDirectory)/SeparationBar.cpp$(ObjectSuffix): SeparationBar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SeparationBar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SeparationBar.cpp$(DependSuffix) -MM SeparationBar.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/SeparationBar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SeparationBar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SeparationBar.cpp$(PreprocessSuffix): SeparationBar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SeparationBar.cpp$(PreprocessSuffix) SeparationBar.cpp

$(IntermediateDirectory)/Pane.cpp$(ObjectSuffix): Pane.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Pane.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Pane.cpp$(DependSuffix) -MM Pane.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/Pane.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Pane.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Pane.cpp$(PreprocessSuffix): Pane.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Pane.cpp$(PreprocessSuffix) Pane.cpp

$(IntermediateDirectory)/ToolbarSlide.cpp$(ObjectSuffix): ToolbarSlide.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ToolbarSlide.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ToolbarSlide.cpp$(DependSuffix) -MM ToolbarSlide.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/ToolbarSlide.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ToolbarSlide.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ToolbarSlide.cpp$(PreprocessSuffix): ToolbarSlide.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ToolbarSlide.cpp$(PreprocessSuffix) ToolbarSlide.cpp

$(IntermediateDirectory)/PositionElement.cpp$(ObjectSuffix): PositionElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PositionElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PositionElement.cpp$(DependSuffix) -MM PositionElement.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/PositionElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PositionElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PositionElement.cpp$(PreprocessSuffix): PositionElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PositionElement.cpp$(PreprocessSuffix) PositionElement.cpp

$(IntermediateDirectory)/EmptyWindow.cpp$(ObjectSuffix): EmptyWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/EmptyWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/EmptyWindow.cpp$(DependSuffix) -MM EmptyWindow.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/EmptyWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/EmptyWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/EmptyWindow.cpp$(PreprocessSuffix): EmptyWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/EmptyWindow.cpp$(PreprocessSuffix) EmptyWindow.cpp

$(IntermediateDirectory)/TreeElementDelete.cpp$(ObjectSuffix): TreeElementDelete.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeElementDelete.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeElementDelete.cpp$(DependSuffix) -MM TreeElementDelete.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/TreeElementDelete.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeElementDelete.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeElementDelete.cpp$(PreprocessSuffix): TreeElementDelete.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeElementDelete.cpp$(PreprocessSuffix) TreeElementDelete.cpp

$(IntermediateDirectory)/TabWindow.cpp$(ObjectSuffix): TabWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TabWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TabWindow.cpp$(DependSuffix) -MM TabWindow.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/TabWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TabWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TabWindow.cpp$(PreprocessSuffix): TabWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TabWindow.cpp$(PreprocessSuffix) TabWindow.cpp

$(IntermediateDirectory)/WindowManager.cpp$(ObjectSuffix): WindowManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/WindowManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/WindowManager.cpp$(DependSuffix) -MM WindowManager.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/WindowManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/WindowManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/WindowManager.cpp$(PreprocessSuffix): WindowManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/WindowManager.cpp$(PreprocessSuffix) WindowManager.cpp

$(IntermediateDirectory)/CalendarDialogBox.cpp$(ObjectSuffix): CalendarDialogBox.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CalendarDialogBox.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CalendarDialogBox.cpp$(DependSuffix) -MM CalendarDialogBox.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/CalendarDialogBox.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CalendarDialogBox.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CalendarDialogBox.cpp$(PreprocessSuffix): CalendarDialogBox.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CalendarDialogBox.cpp$(PreprocessSuffix) CalendarDialogBox.cpp

$(IntermediateDirectory)/WindowToAdd.cpp$(ObjectSuffix): WindowToAdd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/WindowToAdd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/WindowToAdd.cpp$(DependSuffix) -MM WindowToAdd.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/WindowToAdd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/WindowToAdd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/WindowToAdd.cpp$(PreprocessSuffix): WindowToAdd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/WindowToAdd.cpp$(PreprocessSuffix) WindowToAdd.cpp

$(IntermediateDirectory)/CalendarSelect.cpp$(ObjectSuffix): CalendarSelect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CalendarSelect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CalendarSelect.cpp$(DependSuffix) -MM CalendarSelect.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/CalendarSelect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CalendarSelect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CalendarSelect.cpp$(PreprocessSuffix): CalendarSelect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CalendarSelect.cpp$(PreprocessSuffix) CalendarSelect.cpp

$(IntermediateDirectory)/TreeDataKeyWord.cpp$(ObjectSuffix): TreeDataKeyWord.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeDataKeyWord.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeDataKeyWord.cpp$(DependSuffix) -MM TreeDataKeyWord.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/TreeDataKeyWord.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeDataKeyWord.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeDataKeyWord.cpp$(PreprocessSuffix): TreeDataKeyWord.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeDataKeyWord.cpp$(PreprocessSuffix) TreeDataKeyWord.cpp

$(IntermediateDirectory)/TreeElementStar.cpp$(ObjectSuffix): TreeElementStar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeElementStar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeElementStar.cpp$(DependSuffix) -MM TreeElementStar.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/TreeElementStar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeElementStar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeElementStar.cpp$(PreprocessSuffix): TreeElementStar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeElementStar.cpp$(PreprocessSuffix) TreeElementStar.cpp

$(IntermediateDirectory)/KeywordDialogBox.cpp$(ObjectSuffix): KeywordDialogBox.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/KeywordDialogBox.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/KeywordDialogBox.cpp$(DependSuffix) -MM KeywordDialogBox.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/KeywordDialogBox.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/KeywordDialogBox.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/KeywordDialogBox.cpp$(PreprocessSuffix): KeywordDialogBox.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/KeywordDialogBox.cpp$(PreprocessSuffix) KeywordDialogBox.cpp

$(IntermediateDirectory)/ToolbarButton.cpp$(ObjectSuffix): ToolbarButton.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ToolbarButton.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ToolbarButton.cpp$(DependSuffix) -MM ToolbarButton.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/ToolbarButton.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ToolbarButton.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ToolbarButton.cpp$(PreprocessSuffix): ToolbarButton.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ToolbarButton.cpp$(PreprocessSuffix) ToolbarButton.cpp

$(IntermediateDirectory)/TitleBar.cpp$(ObjectSuffix): TitleBar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TitleBar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TitleBar.cpp$(DependSuffix) -MM TitleBar.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/TitleBar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TitleBar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TitleBar.cpp$(PreprocessSuffix): TitleBar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TitleBar.cpp$(PreprocessSuffix) TitleBar.cpp

$(IntermediateDirectory)/Slider.cpp$(ObjectSuffix): Slider.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Slider.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Slider.cpp$(DependSuffix) -MM Slider.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/Slider.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Slider.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Slider.cpp$(PreprocessSuffix): Slider.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Slider.cpp$(PreprocessSuffix) Slider.cpp

$(IntermediateDirectory)/TreeElementCheckBox.cpp$(ObjectSuffix): TreeElementCheckBox.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeElementCheckBox.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeElementCheckBox.cpp$(DependSuffix) -MM TreeElementCheckBox.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/TreeElementCheckBox.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeElementCheckBox.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeElementCheckBox.cpp$(PreprocessSuffix): TreeElementCheckBox.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeElementCheckBox.cpp$(PreprocessSuffix) TreeElementCheckBox.cpp

$(IntermediateDirectory)/BitmapInfosGps.cpp$(ObjectSuffix): BitmapInfosGps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BitmapInfosGps.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BitmapInfosGps.cpp$(DependSuffix) -MM BitmapInfosGps.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/BitmapInfosGps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BitmapInfosGps.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BitmapInfosGps.cpp$(PreprocessSuffix): BitmapInfosGps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BitmapInfosGps.cpp$(PreprocessSuffix) BitmapInfosGps.cpp

$(IntermediateDirectory)/TreeWithScrollbar.cpp$(ObjectSuffix): TreeWithScrollbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeWithScrollbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeWithScrollbar.cpp$(DependSuffix) -MM TreeWithScrollbar.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/TreeWithScrollbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeWithScrollbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeWithScrollbar.cpp$(PreprocessSuffix): TreeWithScrollbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeWithScrollbar.cpp$(PreprocessSuffix) TreeWithScrollbar.cpp

$(IntermediateDirectory)/TreeDataLink.cpp$(ObjectSuffix): TreeDataLink.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeDataLink.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeDataLink.cpp$(DependSuffix) -MM TreeDataLink.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/TreeDataLink.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeDataLink.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeDataLink.cpp$(PreprocessSuffix): TreeDataLink.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeDataLink.cpp$(PreprocessSuffix) TreeDataLink.cpp

$(IntermediateDirectory)/Splitter.cpp$(ObjectSuffix): Splitter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Splitter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Splitter.cpp$(DependSuffix) -MM Splitter.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/Splitter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Splitter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Splitter.cpp$(PreprocessSuffix): Splitter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Splitter.cpp$(PreprocessSuffix) Splitter.cpp

$(IntermediateDirectory)/ToolbarElement.cpp$(ObjectSuffix): ToolbarElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ToolbarElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ToolbarElement.cpp$(DependSuffix) -MM ToolbarElement.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/ToolbarElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ToolbarElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ToolbarElement.cpp$(PreprocessSuffix): ToolbarElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ToolbarElement.cpp$(PreprocessSuffix) ToolbarElement.cpp

$(IntermediateDirectory)/InfosSeparationBar.cpp$(ObjectSuffix): InfosSeparationBar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InfosSeparationBar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InfosSeparationBar.cpp$(DependSuffix) -MM InfosSeparationBar.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/InfosSeparationBar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InfosSeparationBar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InfosSeparationBar.cpp$(PreprocessSuffix): InfosSeparationBar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InfosSeparationBar.cpp$(PreprocessSuffix) InfosSeparationBar.cpp

$(IntermediateDirectory)/ClickToolbar.cpp$(ObjectSuffix): ClickToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ClickToolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ClickToolbar.cpp$(DependSuffix) -MM ClickToolbar.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/ClickToolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ClickToolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ClickToolbar.cpp$(PreprocessSuffix): ClickToolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ClickToolbar.cpp$(PreprocessSuffix) ClickToolbar.cpp

$(IntermediateDirectory)/TreeElementTexte.cpp$(ObjectSuffix): TreeElementTexte.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeElementTexte.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeElementTexte.cpp$(DependSuffix) -MM TreeElementTexte.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/TreeElementTexte.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeElementTexte.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeElementTexte.cpp$(PreprocessSuffix): TreeElementTexte.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeElementTexte.cpp$(PreprocessSuffix) TreeElementTexte.cpp

$(IntermediateDirectory)/TreeDataEffect.cpp$(ObjectSuffix): TreeDataEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeDataEffect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeDataEffect.cpp$(DependSuffix) -MM TreeDataEffect.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/git/Regards/libWindow/TreeDataEffect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeDataEffect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeDataEffect.cpp$(PreprocessSuffix): TreeDataEffect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeDataEffect.cpp$(PreprocessSuffix) TreeDataEffect.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/
	$(RM) ../include/Release/header.h.gch


