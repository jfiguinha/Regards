##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libWindow
ConfigurationName      :=Debug
WorkspacePath          :=C:/developpement/git_gcc/Regards
ProjectPath            :=C:/developpement/git_gcc/Regards/libWindow
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=jfigu_000
Date                   :=14/05/2019
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
OutputFile             :=$(IntermediateDirectory)/$(ProjectName).a
Preprocessors          :=$(PreprocessorSwitch)FFMPEG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libWindow.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/msys64/mingw64/bin/windres.exe
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)"C:\Program Files (x86)\AMD APP SDK\3.0_mingw\include" $(IncludeSwitch)/mingw64/include/libxml2 $(IncludeSwitch)/mingw64/include/OpenEXR  $(IncludeSwitch)../Otherlib/RegardsRaw $(IncludeSwitch). $(IncludeSwitch)../libextern/glew-2.1.0/include $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libTheme $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libextern/ffmpeg-2.6.2 $(IncludeSwitch)../libextern/SDL2-2.0.3/include $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libParameter $(IncludeSwitch)../libUtility $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libResource $(IncludeSwitch)../libSqlData $(IncludeSwitch)../libRenderBitmap $(IncludeSwitch)../libSqlEngine $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libFiltre $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../Otherlib/wxSVG/include $(IncludeSwitch)../libPicture $(IncludeSwitch)../libWindowMain $(IncludeSwitch)../libOpenCL $(IncludeSwitch)../libOpenGL $(IncludeSwitch)../libExif $(IncludeSwitch)../libPiccante $(IncludeSwitch)../libextern/OpenCL-Headers 
IncludePCH             :=  -include ../include/Debug/header.h 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                :=$(LibraryPathSwitch)"C:\Program Files (x86)\AMD APP SDK\3.0_mingw\lib\x86_64"  $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/msys64/mingw64/bin/ar.exe rcu
CXX      := C:/msys64/usr/bin/ccache.exe C:/msys64/mingw64/bin/g++.exe
CC       := C:/msys64/usr/bin/ccache.exe  C:/msys64/mingw64/bin/gcc.exe
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread  -fopenmp -std=gnu++11 -Wall -ggdb -O0 $(Preprocessors)
CFLAGS   :=  -Wall -ggdb -O0 $(Preprocessors)
ASFLAGS  := 
AS       := C:/msys64/mingw64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
PATH:=C:\msys64\usr\bin;$PATH
WXWIN:=C:\Regards\libextern\wxwidgets-3.1.2
WXCFG:=lib/mswud
Objects0=$(IntermediateDirectory)/BitmapInfosCheckBox.cpp$(ObjectSuffix) $(IntermediateDirectory)/IconeList.cpp$(ObjectSuffix) $(IntermediateDirectory)/BitmapInfos.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeElementTexteClick.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeElementTriangle.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeElementDelete.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeControl.cpp$(ObjectSuffix) $(IntermediateDirectory)/Pane.cpp$(ObjectSuffix) $(IntermediateDirectory)/ToolbarWindow.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/PositionElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/ToolbarElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/TitleBar.cpp$(ObjectSuffix) $(IntermediateDirectory)/BitmapInfosGps.cpp$(ObjectSuffix) $(IntermediateDirectory)/Slider.cpp$(ObjectSuffix) $(IntermediateDirectory)/ToolbarButton.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeElementTexte.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeWindow.cpp$(ObjectSuffix) $(IntermediateDirectory)/ScrollbarVerticalWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/Icone.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/ScrollbarWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/ScrollbarHorizontalWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/InfosSeparationBar.cpp$(ObjectSuffix) $(IntermediateDirectory)/TabWindow.cpp$(ObjectSuffix) $(IntermediateDirectory)/ToolbarSlide.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeElementSlide.cpp$(ObjectSuffix) $(IntermediateDirectory)/ClickToolbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/CalendarSelect.cpp$(ObjectSuffix) $(IntermediateDirectory)/SeparationBar.cpp$(ObjectSuffix) $(IntermediateDirectory)/ToolbarTexte.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/BitmapWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/RenderPreviewBitmap.cpp$(ObjectSuffix) $(IntermediateDirectory)/ScrollInterface.cpp$(ObjectSuffix) $(IntermediateDirectory)/Splitter.cpp$(ObjectSuffix) $(IntermediateDirectory)/MapDialogBox.cpp$(ObjectSuffix) $(IntermediateDirectory)/Thumbnail.cpp$(ObjectSuffix) $(IntermediateDirectory)/PanelWithClickToolbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeElementCheckBox.cpp$(ObjectSuffix) $(IntermediateDirectory)/EmptyWindow.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/TreeElementListBox.cpp$(ObjectSuffix) $(IntermediateDirectory)/MapSelect.cpp$(ObjectSuffix) $(IntermediateDirectory)/CalendarDialogBox.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "C:\developpement\git_gcc\Regards/.build-debug"
	@echo rebuilt > "C:\developpement\git_gcc\Regards/.build-debug/libWindow"

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Debug"


./Debug:
	@$(MakeDirCommand) "./Debug"

PreBuild:

# PreCompiled Header
../include/Debug/header.h.gch: ../include/Debug/header.h
	$(CXX) $(SourceSwitch) ../include/Debug/header.h $(PCHCompileFlags) $(CXXFLAGS) $(IncludePath)



##
## Objects
##
$(IntermediateDirectory)/BitmapInfosCheckBox.cpp$(ObjectSuffix): BitmapInfosCheckBox.cpp $(IntermediateDirectory)/BitmapInfosCheckBox.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/BitmapInfosCheckBox.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BitmapInfosCheckBox.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BitmapInfosCheckBox.cpp$(DependSuffix): BitmapInfosCheckBox.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BitmapInfosCheckBox.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BitmapInfosCheckBox.cpp$(DependSuffix) -MM BitmapInfosCheckBox.cpp

$(IntermediateDirectory)/BitmapInfosCheckBox.cpp$(PreprocessSuffix): BitmapInfosCheckBox.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BitmapInfosCheckBox.cpp$(PreprocessSuffix) BitmapInfosCheckBox.cpp

$(IntermediateDirectory)/IconeList.cpp$(ObjectSuffix): IconeList.cpp $(IntermediateDirectory)/IconeList.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/IconeList.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IconeList.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IconeList.cpp$(DependSuffix): IconeList.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IconeList.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IconeList.cpp$(DependSuffix) -MM IconeList.cpp

$(IntermediateDirectory)/IconeList.cpp$(PreprocessSuffix): IconeList.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IconeList.cpp$(PreprocessSuffix) IconeList.cpp

$(IntermediateDirectory)/BitmapInfos.cpp$(ObjectSuffix): BitmapInfos.cpp $(IntermediateDirectory)/BitmapInfos.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/BitmapInfos.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BitmapInfos.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BitmapInfos.cpp$(DependSuffix): BitmapInfos.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BitmapInfos.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BitmapInfos.cpp$(DependSuffix) -MM BitmapInfos.cpp

$(IntermediateDirectory)/BitmapInfos.cpp$(PreprocessSuffix): BitmapInfos.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BitmapInfos.cpp$(PreprocessSuffix) BitmapInfos.cpp

$(IntermediateDirectory)/TreeElementTexteClick.cpp$(ObjectSuffix): TreeElementTexteClick.cpp $(IntermediateDirectory)/TreeElementTexteClick.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/TreeElementTexteClick.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeElementTexteClick.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeElementTexteClick.cpp$(DependSuffix): TreeElementTexteClick.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeElementTexteClick.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeElementTexteClick.cpp$(DependSuffix) -MM TreeElementTexteClick.cpp

$(IntermediateDirectory)/TreeElementTexteClick.cpp$(PreprocessSuffix): TreeElementTexteClick.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeElementTexteClick.cpp$(PreprocessSuffix) TreeElementTexteClick.cpp

$(IntermediateDirectory)/TreeElementTriangle.cpp$(ObjectSuffix): TreeElementTriangle.cpp $(IntermediateDirectory)/TreeElementTriangle.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/TreeElementTriangle.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeElementTriangle.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeElementTriangle.cpp$(DependSuffix): TreeElementTriangle.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeElementTriangle.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeElementTriangle.cpp$(DependSuffix) -MM TreeElementTriangle.cpp

$(IntermediateDirectory)/TreeElementTriangle.cpp$(PreprocessSuffix): TreeElementTriangle.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeElementTriangle.cpp$(PreprocessSuffix) TreeElementTriangle.cpp

$(IntermediateDirectory)/TreeElementDelete.cpp$(ObjectSuffix): TreeElementDelete.cpp $(IntermediateDirectory)/TreeElementDelete.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/TreeElementDelete.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeElementDelete.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeElementDelete.cpp$(DependSuffix): TreeElementDelete.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeElementDelete.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeElementDelete.cpp$(DependSuffix) -MM TreeElementDelete.cpp

$(IntermediateDirectory)/TreeElementDelete.cpp$(PreprocessSuffix): TreeElementDelete.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeElementDelete.cpp$(PreprocessSuffix) TreeElementDelete.cpp

$(IntermediateDirectory)/TreeElement.cpp$(ObjectSuffix): TreeElement.cpp $(IntermediateDirectory)/TreeElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/TreeElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeElement.cpp$(DependSuffix): TreeElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeElement.cpp$(DependSuffix) -MM TreeElement.cpp

$(IntermediateDirectory)/TreeElement.cpp$(PreprocessSuffix): TreeElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeElement.cpp$(PreprocessSuffix) TreeElement.cpp

$(IntermediateDirectory)/TreeControl.cpp$(ObjectSuffix): TreeControl.cpp $(IntermediateDirectory)/TreeControl.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/TreeControl.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeControl.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeControl.cpp$(DependSuffix): TreeControl.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeControl.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeControl.cpp$(DependSuffix) -MM TreeControl.cpp

$(IntermediateDirectory)/TreeControl.cpp$(PreprocessSuffix): TreeControl.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeControl.cpp$(PreprocessSuffix) TreeControl.cpp

$(IntermediateDirectory)/Pane.cpp$(ObjectSuffix): Pane.cpp $(IntermediateDirectory)/Pane.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/Pane.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Pane.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Pane.cpp$(DependSuffix): Pane.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Pane.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Pane.cpp$(DependSuffix) -MM Pane.cpp

$(IntermediateDirectory)/Pane.cpp$(PreprocessSuffix): Pane.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Pane.cpp$(PreprocessSuffix) Pane.cpp

$(IntermediateDirectory)/ToolbarWindow.cpp$(ObjectSuffix): ToolbarWindow.cpp $(IntermediateDirectory)/ToolbarWindow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/ToolbarWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ToolbarWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ToolbarWindow.cpp$(DependSuffix): ToolbarWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ToolbarWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ToolbarWindow.cpp$(DependSuffix) -MM ToolbarWindow.cpp

$(IntermediateDirectory)/ToolbarWindow.cpp$(PreprocessSuffix): ToolbarWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ToolbarWindow.cpp$(PreprocessSuffix) ToolbarWindow.cpp

$(IntermediateDirectory)/PositionElement.cpp$(ObjectSuffix): PositionElement.cpp $(IntermediateDirectory)/PositionElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/PositionElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PositionElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PositionElement.cpp$(DependSuffix): PositionElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PositionElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PositionElement.cpp$(DependSuffix) -MM PositionElement.cpp

$(IntermediateDirectory)/PositionElement.cpp$(PreprocessSuffix): PositionElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PositionElement.cpp$(PreprocessSuffix) PositionElement.cpp

$(IntermediateDirectory)/ToolbarElement.cpp$(ObjectSuffix): ToolbarElement.cpp $(IntermediateDirectory)/ToolbarElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/ToolbarElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ToolbarElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ToolbarElement.cpp$(DependSuffix): ToolbarElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ToolbarElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ToolbarElement.cpp$(DependSuffix) -MM ToolbarElement.cpp

$(IntermediateDirectory)/ToolbarElement.cpp$(PreprocessSuffix): ToolbarElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ToolbarElement.cpp$(PreprocessSuffix) ToolbarElement.cpp

$(IntermediateDirectory)/TitleBar.cpp$(ObjectSuffix): TitleBar.cpp $(IntermediateDirectory)/TitleBar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/TitleBar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TitleBar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TitleBar.cpp$(DependSuffix): TitleBar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TitleBar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TitleBar.cpp$(DependSuffix) -MM TitleBar.cpp

$(IntermediateDirectory)/TitleBar.cpp$(PreprocessSuffix): TitleBar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TitleBar.cpp$(PreprocessSuffix) TitleBar.cpp

$(IntermediateDirectory)/BitmapInfosGps.cpp$(ObjectSuffix): BitmapInfosGps.cpp $(IntermediateDirectory)/BitmapInfosGps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/BitmapInfosGps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BitmapInfosGps.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BitmapInfosGps.cpp$(DependSuffix): BitmapInfosGps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BitmapInfosGps.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BitmapInfosGps.cpp$(DependSuffix) -MM BitmapInfosGps.cpp

$(IntermediateDirectory)/BitmapInfosGps.cpp$(PreprocessSuffix): BitmapInfosGps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BitmapInfosGps.cpp$(PreprocessSuffix) BitmapInfosGps.cpp

$(IntermediateDirectory)/Slider.cpp$(ObjectSuffix): Slider.cpp $(IntermediateDirectory)/Slider.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/Slider.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Slider.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Slider.cpp$(DependSuffix): Slider.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Slider.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Slider.cpp$(DependSuffix) -MM Slider.cpp

$(IntermediateDirectory)/Slider.cpp$(PreprocessSuffix): Slider.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Slider.cpp$(PreprocessSuffix) Slider.cpp

$(IntermediateDirectory)/ToolbarButton.cpp$(ObjectSuffix): ToolbarButton.cpp $(IntermediateDirectory)/ToolbarButton.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/ToolbarButton.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ToolbarButton.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ToolbarButton.cpp$(DependSuffix): ToolbarButton.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ToolbarButton.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ToolbarButton.cpp$(DependSuffix) -MM ToolbarButton.cpp

$(IntermediateDirectory)/ToolbarButton.cpp$(PreprocessSuffix): ToolbarButton.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ToolbarButton.cpp$(PreprocessSuffix) ToolbarButton.cpp

$(IntermediateDirectory)/TreeElementTexte.cpp$(ObjectSuffix): TreeElementTexte.cpp $(IntermediateDirectory)/TreeElementTexte.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/TreeElementTexte.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeElementTexte.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeElementTexte.cpp$(DependSuffix): TreeElementTexte.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeElementTexte.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeElementTexte.cpp$(DependSuffix) -MM TreeElementTexte.cpp

$(IntermediateDirectory)/TreeElementTexte.cpp$(PreprocessSuffix): TreeElementTexte.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeElementTexte.cpp$(PreprocessSuffix) TreeElementTexte.cpp

$(IntermediateDirectory)/TreeWindow.cpp$(ObjectSuffix): TreeWindow.cpp $(IntermediateDirectory)/TreeWindow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/TreeWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeWindow.cpp$(DependSuffix): TreeWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeWindow.cpp$(DependSuffix) -MM TreeWindow.cpp

$(IntermediateDirectory)/TreeWindow.cpp$(PreprocessSuffix): TreeWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeWindow.cpp$(PreprocessSuffix) TreeWindow.cpp

$(IntermediateDirectory)/ScrollbarVerticalWnd.cpp$(ObjectSuffix): ScrollbarVerticalWnd.cpp $(IntermediateDirectory)/ScrollbarVerticalWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/ScrollbarVerticalWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ScrollbarVerticalWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ScrollbarVerticalWnd.cpp$(DependSuffix): ScrollbarVerticalWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ScrollbarVerticalWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ScrollbarVerticalWnd.cpp$(DependSuffix) -MM ScrollbarVerticalWnd.cpp

$(IntermediateDirectory)/ScrollbarVerticalWnd.cpp$(PreprocessSuffix): ScrollbarVerticalWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ScrollbarVerticalWnd.cpp$(PreprocessSuffix) ScrollbarVerticalWnd.cpp

$(IntermediateDirectory)/Icone.cpp$(ObjectSuffix): Icone.cpp $(IntermediateDirectory)/Icone.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/Icone.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Icone.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Icone.cpp$(DependSuffix): Icone.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Icone.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Icone.cpp$(DependSuffix) -MM Icone.cpp

$(IntermediateDirectory)/Icone.cpp$(PreprocessSuffix): Icone.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Icone.cpp$(PreprocessSuffix) Icone.cpp

$(IntermediateDirectory)/ScrollbarWnd.cpp$(ObjectSuffix): ScrollbarWnd.cpp $(IntermediateDirectory)/ScrollbarWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/ScrollbarWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ScrollbarWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ScrollbarWnd.cpp$(DependSuffix): ScrollbarWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ScrollbarWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ScrollbarWnd.cpp$(DependSuffix) -MM ScrollbarWnd.cpp

$(IntermediateDirectory)/ScrollbarWnd.cpp$(PreprocessSuffix): ScrollbarWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ScrollbarWnd.cpp$(PreprocessSuffix) ScrollbarWnd.cpp

$(IntermediateDirectory)/ScrollbarHorizontalWnd.cpp$(ObjectSuffix): ScrollbarHorizontalWnd.cpp $(IntermediateDirectory)/ScrollbarHorizontalWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/ScrollbarHorizontalWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ScrollbarHorizontalWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ScrollbarHorizontalWnd.cpp$(DependSuffix): ScrollbarHorizontalWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ScrollbarHorizontalWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ScrollbarHorizontalWnd.cpp$(DependSuffix) -MM ScrollbarHorizontalWnd.cpp

$(IntermediateDirectory)/ScrollbarHorizontalWnd.cpp$(PreprocessSuffix): ScrollbarHorizontalWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ScrollbarHorizontalWnd.cpp$(PreprocessSuffix) ScrollbarHorizontalWnd.cpp

$(IntermediateDirectory)/InfosSeparationBar.cpp$(ObjectSuffix): InfosSeparationBar.cpp $(IntermediateDirectory)/InfosSeparationBar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/InfosSeparationBar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InfosSeparationBar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InfosSeparationBar.cpp$(DependSuffix): InfosSeparationBar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InfosSeparationBar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InfosSeparationBar.cpp$(DependSuffix) -MM InfosSeparationBar.cpp

$(IntermediateDirectory)/InfosSeparationBar.cpp$(PreprocessSuffix): InfosSeparationBar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InfosSeparationBar.cpp$(PreprocessSuffix) InfosSeparationBar.cpp

$(IntermediateDirectory)/TabWindow.cpp$(ObjectSuffix): TabWindow.cpp $(IntermediateDirectory)/TabWindow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/TabWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TabWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TabWindow.cpp$(DependSuffix): TabWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TabWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TabWindow.cpp$(DependSuffix) -MM TabWindow.cpp

$(IntermediateDirectory)/TabWindow.cpp$(PreprocessSuffix): TabWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TabWindow.cpp$(PreprocessSuffix) TabWindow.cpp

$(IntermediateDirectory)/ToolbarSlide.cpp$(ObjectSuffix): ToolbarSlide.cpp $(IntermediateDirectory)/ToolbarSlide.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/ToolbarSlide.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ToolbarSlide.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ToolbarSlide.cpp$(DependSuffix): ToolbarSlide.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ToolbarSlide.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ToolbarSlide.cpp$(DependSuffix) -MM ToolbarSlide.cpp

$(IntermediateDirectory)/ToolbarSlide.cpp$(PreprocessSuffix): ToolbarSlide.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ToolbarSlide.cpp$(PreprocessSuffix) ToolbarSlide.cpp

$(IntermediateDirectory)/TreeElementSlide.cpp$(ObjectSuffix): TreeElementSlide.cpp $(IntermediateDirectory)/TreeElementSlide.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/TreeElementSlide.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeElementSlide.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeElementSlide.cpp$(DependSuffix): TreeElementSlide.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeElementSlide.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeElementSlide.cpp$(DependSuffix) -MM TreeElementSlide.cpp

$(IntermediateDirectory)/TreeElementSlide.cpp$(PreprocessSuffix): TreeElementSlide.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeElementSlide.cpp$(PreprocessSuffix) TreeElementSlide.cpp

$(IntermediateDirectory)/ClickToolbar.cpp$(ObjectSuffix): ClickToolbar.cpp $(IntermediateDirectory)/ClickToolbar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/ClickToolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ClickToolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ClickToolbar.cpp$(DependSuffix): ClickToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ClickToolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ClickToolbar.cpp$(DependSuffix) -MM ClickToolbar.cpp

$(IntermediateDirectory)/ClickToolbar.cpp$(PreprocessSuffix): ClickToolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ClickToolbar.cpp$(PreprocessSuffix) ClickToolbar.cpp

$(IntermediateDirectory)/CalendarSelect.cpp$(ObjectSuffix): CalendarSelect.cpp $(IntermediateDirectory)/CalendarSelect.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/CalendarSelect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CalendarSelect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CalendarSelect.cpp$(DependSuffix): CalendarSelect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CalendarSelect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CalendarSelect.cpp$(DependSuffix) -MM CalendarSelect.cpp

$(IntermediateDirectory)/CalendarSelect.cpp$(PreprocessSuffix): CalendarSelect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CalendarSelect.cpp$(PreprocessSuffix) CalendarSelect.cpp

$(IntermediateDirectory)/SeparationBar.cpp$(ObjectSuffix): SeparationBar.cpp $(IntermediateDirectory)/SeparationBar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/SeparationBar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SeparationBar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SeparationBar.cpp$(DependSuffix): SeparationBar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SeparationBar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SeparationBar.cpp$(DependSuffix) -MM SeparationBar.cpp

$(IntermediateDirectory)/SeparationBar.cpp$(PreprocessSuffix): SeparationBar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SeparationBar.cpp$(PreprocessSuffix) SeparationBar.cpp

$(IntermediateDirectory)/ToolbarTexte.cpp$(ObjectSuffix): ToolbarTexte.cpp $(IntermediateDirectory)/ToolbarTexte.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/ToolbarTexte.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ToolbarTexte.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ToolbarTexte.cpp$(DependSuffix): ToolbarTexte.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ToolbarTexte.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ToolbarTexte.cpp$(DependSuffix) -MM ToolbarTexte.cpp

$(IntermediateDirectory)/ToolbarTexte.cpp$(PreprocessSuffix): ToolbarTexte.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ToolbarTexte.cpp$(PreprocessSuffix) ToolbarTexte.cpp

$(IntermediateDirectory)/BitmapWnd.cpp$(ObjectSuffix): BitmapWnd.cpp $(IntermediateDirectory)/BitmapWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/BitmapWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BitmapWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BitmapWnd.cpp$(DependSuffix): BitmapWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BitmapWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BitmapWnd.cpp$(DependSuffix) -MM BitmapWnd.cpp

$(IntermediateDirectory)/BitmapWnd.cpp$(PreprocessSuffix): BitmapWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BitmapWnd.cpp$(PreprocessSuffix) BitmapWnd.cpp

$(IntermediateDirectory)/RenderPreviewBitmap.cpp$(ObjectSuffix): RenderPreviewBitmap.cpp $(IntermediateDirectory)/RenderPreviewBitmap.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/RenderPreviewBitmap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RenderPreviewBitmap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RenderPreviewBitmap.cpp$(DependSuffix): RenderPreviewBitmap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RenderPreviewBitmap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RenderPreviewBitmap.cpp$(DependSuffix) -MM RenderPreviewBitmap.cpp

$(IntermediateDirectory)/RenderPreviewBitmap.cpp$(PreprocessSuffix): RenderPreviewBitmap.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RenderPreviewBitmap.cpp$(PreprocessSuffix) RenderPreviewBitmap.cpp

$(IntermediateDirectory)/ScrollInterface.cpp$(ObjectSuffix): ScrollInterface.cpp $(IntermediateDirectory)/ScrollInterface.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/ScrollInterface.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ScrollInterface.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ScrollInterface.cpp$(DependSuffix): ScrollInterface.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ScrollInterface.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ScrollInterface.cpp$(DependSuffix) -MM ScrollInterface.cpp

$(IntermediateDirectory)/ScrollInterface.cpp$(PreprocessSuffix): ScrollInterface.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ScrollInterface.cpp$(PreprocessSuffix) ScrollInterface.cpp

$(IntermediateDirectory)/Splitter.cpp$(ObjectSuffix): Splitter.cpp $(IntermediateDirectory)/Splitter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/Splitter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Splitter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Splitter.cpp$(DependSuffix): Splitter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Splitter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Splitter.cpp$(DependSuffix) -MM Splitter.cpp

$(IntermediateDirectory)/Splitter.cpp$(PreprocessSuffix): Splitter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Splitter.cpp$(PreprocessSuffix) Splitter.cpp

$(IntermediateDirectory)/MapDialogBox.cpp$(ObjectSuffix): MapDialogBox.cpp $(IntermediateDirectory)/MapDialogBox.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/MapDialogBox.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MapDialogBox.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MapDialogBox.cpp$(DependSuffix): MapDialogBox.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MapDialogBox.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MapDialogBox.cpp$(DependSuffix) -MM MapDialogBox.cpp

$(IntermediateDirectory)/MapDialogBox.cpp$(PreprocessSuffix): MapDialogBox.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MapDialogBox.cpp$(PreprocessSuffix) MapDialogBox.cpp

$(IntermediateDirectory)/Thumbnail.cpp$(ObjectSuffix): Thumbnail.cpp $(IntermediateDirectory)/Thumbnail.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/Thumbnail.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Thumbnail.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Thumbnail.cpp$(DependSuffix): Thumbnail.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Thumbnail.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Thumbnail.cpp$(DependSuffix) -MM Thumbnail.cpp

$(IntermediateDirectory)/Thumbnail.cpp$(PreprocessSuffix): Thumbnail.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Thumbnail.cpp$(PreprocessSuffix) Thumbnail.cpp

$(IntermediateDirectory)/PanelWithClickToolbar.cpp$(ObjectSuffix): PanelWithClickToolbar.cpp $(IntermediateDirectory)/PanelWithClickToolbar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/PanelWithClickToolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PanelWithClickToolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PanelWithClickToolbar.cpp$(DependSuffix): PanelWithClickToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PanelWithClickToolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PanelWithClickToolbar.cpp$(DependSuffix) -MM PanelWithClickToolbar.cpp

$(IntermediateDirectory)/PanelWithClickToolbar.cpp$(PreprocessSuffix): PanelWithClickToolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PanelWithClickToolbar.cpp$(PreprocessSuffix) PanelWithClickToolbar.cpp

$(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(ObjectSuffix): RenderBitmapInterfaceOpenGL.cpp $(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/RenderBitmapInterfaceOpenGL.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(DependSuffix): RenderBitmapInterfaceOpenGL.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(DependSuffix) -MM RenderBitmapInterfaceOpenGL.cpp

$(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(PreprocessSuffix): RenderBitmapInterfaceOpenGL.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(PreprocessSuffix) RenderBitmapInterfaceOpenGL.cpp

$(IntermediateDirectory)/TreeElementCheckBox.cpp$(ObjectSuffix): TreeElementCheckBox.cpp $(IntermediateDirectory)/TreeElementCheckBox.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/TreeElementCheckBox.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeElementCheckBox.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeElementCheckBox.cpp$(DependSuffix): TreeElementCheckBox.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeElementCheckBox.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeElementCheckBox.cpp$(DependSuffix) -MM TreeElementCheckBox.cpp

$(IntermediateDirectory)/TreeElementCheckBox.cpp$(PreprocessSuffix): TreeElementCheckBox.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeElementCheckBox.cpp$(PreprocessSuffix) TreeElementCheckBox.cpp

$(IntermediateDirectory)/EmptyWindow.cpp$(ObjectSuffix): EmptyWindow.cpp $(IntermediateDirectory)/EmptyWindow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/EmptyWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/EmptyWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/EmptyWindow.cpp$(DependSuffix): EmptyWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/EmptyWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/EmptyWindow.cpp$(DependSuffix) -MM EmptyWindow.cpp

$(IntermediateDirectory)/EmptyWindow.cpp$(PreprocessSuffix): EmptyWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/EmptyWindow.cpp$(PreprocessSuffix) EmptyWindow.cpp

$(IntermediateDirectory)/TreeElementListBox.cpp$(ObjectSuffix): TreeElementListBox.cpp $(IntermediateDirectory)/TreeElementListBox.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/TreeElementListBox.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeElementListBox.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeElementListBox.cpp$(DependSuffix): TreeElementListBox.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeElementListBox.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeElementListBox.cpp$(DependSuffix) -MM TreeElementListBox.cpp

$(IntermediateDirectory)/TreeElementListBox.cpp$(PreprocessSuffix): TreeElementListBox.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeElementListBox.cpp$(PreprocessSuffix) TreeElementListBox.cpp

$(IntermediateDirectory)/MapSelect.cpp$(ObjectSuffix): MapSelect.cpp $(IntermediateDirectory)/MapSelect.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/MapSelect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MapSelect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MapSelect.cpp$(DependSuffix): MapSelect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MapSelect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MapSelect.cpp$(DependSuffix) -MM MapSelect.cpp

$(IntermediateDirectory)/MapSelect.cpp$(PreprocessSuffix): MapSelect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MapSelect.cpp$(PreprocessSuffix) MapSelect.cpp

$(IntermediateDirectory)/CalendarDialogBox.cpp$(ObjectSuffix): CalendarDialogBox.cpp $(IntermediateDirectory)/CalendarDialogBox.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libWindow/CalendarDialogBox.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CalendarDialogBox.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CalendarDialogBox.cpp$(DependSuffix): CalendarDialogBox.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CalendarDialogBox.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CalendarDialogBox.cpp$(DependSuffix) -MM CalendarDialogBox.cpp

$(IntermediateDirectory)/CalendarDialogBox.cpp$(PreprocessSuffix): CalendarDialogBox.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CalendarDialogBox.cpp$(PreprocessSuffix) CalendarDialogBox.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/
	$(RM) ../include/Debug/header.h.gch


