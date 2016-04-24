##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libWindow
ConfigurationName      :=Debug
WorkspacePath          := "/home/figuinha/dev/Regards"
ProjectPath            := "/home/figuinha/dev/Regards/libWindow"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=23/04/16
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
Preprocessors          :=$(PreprocessorSwitch)__WXGTK__ $(PreprocessorSwitch)GLEW_MX 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libWindow.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libTheme $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libextern/ffmpeg-2.6.2 $(IncludeSwitch)../libextern/SDL2-2.0.3/include $(IncludeSwitch)../libextern/glew-1.13.0/include $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libParameter $(IncludeSwitch)../libextern/exiv2-0.24/src $(IncludeSwitch)../libextern/exiv2-0.24/xmpsdk/include $(IncludeSwitch)../libUtility $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libResource $(IncludeSwitch)../libSqlData $(IncludeSwitch)../libOpenGL $(IncludeSwitch)../libRenderBitmap $(IncludeSwitch)../libSqlEngine $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libFiltre $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../Otherlib/wxSVG/include $(IncludeSwitch)../libPicture $(IncludeSwitch)../libWindowMain $(IncludeSwitch)../libextern/wxWidgets-3.0.2/include $(IncludeSwitch)../libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0 
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
Objects0=$(IntermediateDirectory)/BitmapWindowContext.cpp$(ObjectSuffix) $(IntermediateDirectory)/BitmapWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/EmptyWindow.cpp$(ObjectSuffix) $(IntermediateDirectory)/Icone.cpp$(ObjectSuffix) $(IntermediateDirectory)/InfosSeparationBar.cpp$(ObjectSuffix) $(IntermediateDirectory)/Pane.cpp$(ObjectSuffix) $(IntermediateDirectory)/PositionElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/ScrollbarHorizontalWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/ScrollbarVerticalWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/ScrollbarWnd.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/ScrollInterface.cpp$(ObjectSuffix) $(IntermediateDirectory)/SeparationBar.cpp$(ObjectSuffix) $(IntermediateDirectory)/Slider.cpp$(ObjectSuffix) $(IntermediateDirectory)/Splitter.cpp$(ObjectSuffix) $(IntermediateDirectory)/Thumbnail.cpp$(ObjectSuffix) $(IntermediateDirectory)/TitleBar.cpp$(ObjectSuffix) $(IntermediateDirectory)/ToolbarButton.cpp$(ObjectSuffix) $(IntermediateDirectory)/ToolbarElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/ToolbarSlide.cpp$(ObjectSuffix) $(IntermediateDirectory)/ToolbarTexte.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/ToolbarWindow.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeControl.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeElementCheckBox.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeElementDelete.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeElementSlide.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeElementTexte.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeElementTriangle.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeWindow.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeElementTexteClick.cpp$(ObjectSuffix) \
	



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
	@echo rebuilt > "/home/figuinha/dev/Regards/.build-debug/libWindow"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/BitmapWindowContext.cpp$(ObjectSuffix): BitmapWindowContext.cpp $(IntermediateDirectory)/BitmapWindowContext.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/BitmapWindowContext.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BitmapWindowContext.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BitmapWindowContext.cpp$(DependSuffix): BitmapWindowContext.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BitmapWindowContext.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BitmapWindowContext.cpp$(DependSuffix) -MM "BitmapWindowContext.cpp"

$(IntermediateDirectory)/BitmapWindowContext.cpp$(PreprocessSuffix): BitmapWindowContext.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BitmapWindowContext.cpp$(PreprocessSuffix) "BitmapWindowContext.cpp"

$(IntermediateDirectory)/BitmapWnd.cpp$(ObjectSuffix): BitmapWnd.cpp $(IntermediateDirectory)/BitmapWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/BitmapWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BitmapWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BitmapWnd.cpp$(DependSuffix): BitmapWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BitmapWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BitmapWnd.cpp$(DependSuffix) -MM "BitmapWnd.cpp"

$(IntermediateDirectory)/BitmapWnd.cpp$(PreprocessSuffix): BitmapWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BitmapWnd.cpp$(PreprocessSuffix) "BitmapWnd.cpp"

$(IntermediateDirectory)/EmptyWindow.cpp$(ObjectSuffix): EmptyWindow.cpp $(IntermediateDirectory)/EmptyWindow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/EmptyWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/EmptyWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/EmptyWindow.cpp$(DependSuffix): EmptyWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/EmptyWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/EmptyWindow.cpp$(DependSuffix) -MM "EmptyWindow.cpp"

$(IntermediateDirectory)/EmptyWindow.cpp$(PreprocessSuffix): EmptyWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/EmptyWindow.cpp$(PreprocessSuffix) "EmptyWindow.cpp"

$(IntermediateDirectory)/Icone.cpp$(ObjectSuffix): Icone.cpp $(IntermediateDirectory)/Icone.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/Icone.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Icone.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Icone.cpp$(DependSuffix): Icone.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Icone.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Icone.cpp$(DependSuffix) -MM "Icone.cpp"

$(IntermediateDirectory)/Icone.cpp$(PreprocessSuffix): Icone.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Icone.cpp$(PreprocessSuffix) "Icone.cpp"

$(IntermediateDirectory)/InfosSeparationBar.cpp$(ObjectSuffix): InfosSeparationBar.cpp $(IntermediateDirectory)/InfosSeparationBar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/InfosSeparationBar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InfosSeparationBar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InfosSeparationBar.cpp$(DependSuffix): InfosSeparationBar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InfosSeparationBar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InfosSeparationBar.cpp$(DependSuffix) -MM "InfosSeparationBar.cpp"

$(IntermediateDirectory)/InfosSeparationBar.cpp$(PreprocessSuffix): InfosSeparationBar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InfosSeparationBar.cpp$(PreprocessSuffix) "InfosSeparationBar.cpp"

$(IntermediateDirectory)/Pane.cpp$(ObjectSuffix): Pane.cpp $(IntermediateDirectory)/Pane.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/Pane.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Pane.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Pane.cpp$(DependSuffix): Pane.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Pane.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Pane.cpp$(DependSuffix) -MM "Pane.cpp"

$(IntermediateDirectory)/Pane.cpp$(PreprocessSuffix): Pane.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Pane.cpp$(PreprocessSuffix) "Pane.cpp"

$(IntermediateDirectory)/PositionElement.cpp$(ObjectSuffix): PositionElement.cpp $(IntermediateDirectory)/PositionElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/PositionElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PositionElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PositionElement.cpp$(DependSuffix): PositionElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PositionElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PositionElement.cpp$(DependSuffix) -MM "PositionElement.cpp"

$(IntermediateDirectory)/PositionElement.cpp$(PreprocessSuffix): PositionElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PositionElement.cpp$(PreprocessSuffix) "PositionElement.cpp"

$(IntermediateDirectory)/ScrollbarHorizontalWnd.cpp$(ObjectSuffix): ScrollbarHorizontalWnd.cpp $(IntermediateDirectory)/ScrollbarHorizontalWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/ScrollbarHorizontalWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ScrollbarHorizontalWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ScrollbarHorizontalWnd.cpp$(DependSuffix): ScrollbarHorizontalWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ScrollbarHorizontalWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ScrollbarHorizontalWnd.cpp$(DependSuffix) -MM "ScrollbarHorizontalWnd.cpp"

$(IntermediateDirectory)/ScrollbarHorizontalWnd.cpp$(PreprocessSuffix): ScrollbarHorizontalWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ScrollbarHorizontalWnd.cpp$(PreprocessSuffix) "ScrollbarHorizontalWnd.cpp"

$(IntermediateDirectory)/ScrollbarVerticalWnd.cpp$(ObjectSuffix): ScrollbarVerticalWnd.cpp $(IntermediateDirectory)/ScrollbarVerticalWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/ScrollbarVerticalWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ScrollbarVerticalWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ScrollbarVerticalWnd.cpp$(DependSuffix): ScrollbarVerticalWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ScrollbarVerticalWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ScrollbarVerticalWnd.cpp$(DependSuffix) -MM "ScrollbarVerticalWnd.cpp"

$(IntermediateDirectory)/ScrollbarVerticalWnd.cpp$(PreprocessSuffix): ScrollbarVerticalWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ScrollbarVerticalWnd.cpp$(PreprocessSuffix) "ScrollbarVerticalWnd.cpp"

$(IntermediateDirectory)/ScrollbarWnd.cpp$(ObjectSuffix): ScrollbarWnd.cpp $(IntermediateDirectory)/ScrollbarWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/ScrollbarWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ScrollbarWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ScrollbarWnd.cpp$(DependSuffix): ScrollbarWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ScrollbarWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ScrollbarWnd.cpp$(DependSuffix) -MM "ScrollbarWnd.cpp"

$(IntermediateDirectory)/ScrollbarWnd.cpp$(PreprocessSuffix): ScrollbarWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ScrollbarWnd.cpp$(PreprocessSuffix) "ScrollbarWnd.cpp"

$(IntermediateDirectory)/ScrollInterface.cpp$(ObjectSuffix): ScrollInterface.cpp $(IntermediateDirectory)/ScrollInterface.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/ScrollInterface.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ScrollInterface.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ScrollInterface.cpp$(DependSuffix): ScrollInterface.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ScrollInterface.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ScrollInterface.cpp$(DependSuffix) -MM "ScrollInterface.cpp"

$(IntermediateDirectory)/ScrollInterface.cpp$(PreprocessSuffix): ScrollInterface.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ScrollInterface.cpp$(PreprocessSuffix) "ScrollInterface.cpp"

$(IntermediateDirectory)/SeparationBar.cpp$(ObjectSuffix): SeparationBar.cpp $(IntermediateDirectory)/SeparationBar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/SeparationBar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SeparationBar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SeparationBar.cpp$(DependSuffix): SeparationBar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SeparationBar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SeparationBar.cpp$(DependSuffix) -MM "SeparationBar.cpp"

$(IntermediateDirectory)/SeparationBar.cpp$(PreprocessSuffix): SeparationBar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SeparationBar.cpp$(PreprocessSuffix) "SeparationBar.cpp"

$(IntermediateDirectory)/Slider.cpp$(ObjectSuffix): Slider.cpp $(IntermediateDirectory)/Slider.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/Slider.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Slider.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Slider.cpp$(DependSuffix): Slider.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Slider.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Slider.cpp$(DependSuffix) -MM "Slider.cpp"

$(IntermediateDirectory)/Slider.cpp$(PreprocessSuffix): Slider.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Slider.cpp$(PreprocessSuffix) "Slider.cpp"

$(IntermediateDirectory)/Splitter.cpp$(ObjectSuffix): Splitter.cpp $(IntermediateDirectory)/Splitter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/Splitter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Splitter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Splitter.cpp$(DependSuffix): Splitter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Splitter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Splitter.cpp$(DependSuffix) -MM "Splitter.cpp"

$(IntermediateDirectory)/Splitter.cpp$(PreprocessSuffix): Splitter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Splitter.cpp$(PreprocessSuffix) "Splitter.cpp"

$(IntermediateDirectory)/Thumbnail.cpp$(ObjectSuffix): Thumbnail.cpp $(IntermediateDirectory)/Thumbnail.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/Thumbnail.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Thumbnail.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Thumbnail.cpp$(DependSuffix): Thumbnail.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Thumbnail.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Thumbnail.cpp$(DependSuffix) -MM "Thumbnail.cpp"

$(IntermediateDirectory)/Thumbnail.cpp$(PreprocessSuffix): Thumbnail.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Thumbnail.cpp$(PreprocessSuffix) "Thumbnail.cpp"

$(IntermediateDirectory)/TitleBar.cpp$(ObjectSuffix): TitleBar.cpp $(IntermediateDirectory)/TitleBar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/TitleBar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TitleBar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TitleBar.cpp$(DependSuffix): TitleBar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TitleBar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TitleBar.cpp$(DependSuffix) -MM "TitleBar.cpp"

$(IntermediateDirectory)/TitleBar.cpp$(PreprocessSuffix): TitleBar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TitleBar.cpp$(PreprocessSuffix) "TitleBar.cpp"

$(IntermediateDirectory)/ToolbarButton.cpp$(ObjectSuffix): ToolbarButton.cpp $(IntermediateDirectory)/ToolbarButton.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/ToolbarButton.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ToolbarButton.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ToolbarButton.cpp$(DependSuffix): ToolbarButton.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ToolbarButton.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ToolbarButton.cpp$(DependSuffix) -MM "ToolbarButton.cpp"

$(IntermediateDirectory)/ToolbarButton.cpp$(PreprocessSuffix): ToolbarButton.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ToolbarButton.cpp$(PreprocessSuffix) "ToolbarButton.cpp"

$(IntermediateDirectory)/ToolbarElement.cpp$(ObjectSuffix): ToolbarElement.cpp $(IntermediateDirectory)/ToolbarElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/ToolbarElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ToolbarElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ToolbarElement.cpp$(DependSuffix): ToolbarElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ToolbarElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ToolbarElement.cpp$(DependSuffix) -MM "ToolbarElement.cpp"

$(IntermediateDirectory)/ToolbarElement.cpp$(PreprocessSuffix): ToolbarElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ToolbarElement.cpp$(PreprocessSuffix) "ToolbarElement.cpp"

$(IntermediateDirectory)/ToolbarSlide.cpp$(ObjectSuffix): ToolbarSlide.cpp $(IntermediateDirectory)/ToolbarSlide.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/ToolbarSlide.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ToolbarSlide.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ToolbarSlide.cpp$(DependSuffix): ToolbarSlide.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ToolbarSlide.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ToolbarSlide.cpp$(DependSuffix) -MM "ToolbarSlide.cpp"

$(IntermediateDirectory)/ToolbarSlide.cpp$(PreprocessSuffix): ToolbarSlide.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ToolbarSlide.cpp$(PreprocessSuffix) "ToolbarSlide.cpp"

$(IntermediateDirectory)/ToolbarTexte.cpp$(ObjectSuffix): ToolbarTexte.cpp $(IntermediateDirectory)/ToolbarTexte.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/ToolbarTexte.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ToolbarTexte.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ToolbarTexte.cpp$(DependSuffix): ToolbarTexte.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ToolbarTexte.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ToolbarTexte.cpp$(DependSuffix) -MM "ToolbarTexte.cpp"

$(IntermediateDirectory)/ToolbarTexte.cpp$(PreprocessSuffix): ToolbarTexte.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ToolbarTexte.cpp$(PreprocessSuffix) "ToolbarTexte.cpp"

$(IntermediateDirectory)/ToolbarWindow.cpp$(ObjectSuffix): ToolbarWindow.cpp $(IntermediateDirectory)/ToolbarWindow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/ToolbarWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ToolbarWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ToolbarWindow.cpp$(DependSuffix): ToolbarWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ToolbarWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ToolbarWindow.cpp$(DependSuffix) -MM "ToolbarWindow.cpp"

$(IntermediateDirectory)/ToolbarWindow.cpp$(PreprocessSuffix): ToolbarWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ToolbarWindow.cpp$(PreprocessSuffix) "ToolbarWindow.cpp"

$(IntermediateDirectory)/TreeControl.cpp$(ObjectSuffix): TreeControl.cpp $(IntermediateDirectory)/TreeControl.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/TreeControl.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeControl.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeControl.cpp$(DependSuffix): TreeControl.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeControl.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeControl.cpp$(DependSuffix) -MM "TreeControl.cpp"

$(IntermediateDirectory)/TreeControl.cpp$(PreprocessSuffix): TreeControl.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeControl.cpp$(PreprocessSuffix) "TreeControl.cpp"

$(IntermediateDirectory)/TreeElement.cpp$(ObjectSuffix): TreeElement.cpp $(IntermediateDirectory)/TreeElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/TreeElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeElement.cpp$(DependSuffix): TreeElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeElement.cpp$(DependSuffix) -MM "TreeElement.cpp"

$(IntermediateDirectory)/TreeElement.cpp$(PreprocessSuffix): TreeElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeElement.cpp$(PreprocessSuffix) "TreeElement.cpp"

$(IntermediateDirectory)/TreeElementCheckBox.cpp$(ObjectSuffix): TreeElementCheckBox.cpp $(IntermediateDirectory)/TreeElementCheckBox.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/TreeElementCheckBox.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeElementCheckBox.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeElementCheckBox.cpp$(DependSuffix): TreeElementCheckBox.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeElementCheckBox.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeElementCheckBox.cpp$(DependSuffix) -MM "TreeElementCheckBox.cpp"

$(IntermediateDirectory)/TreeElementCheckBox.cpp$(PreprocessSuffix): TreeElementCheckBox.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeElementCheckBox.cpp$(PreprocessSuffix) "TreeElementCheckBox.cpp"

$(IntermediateDirectory)/TreeElementDelete.cpp$(ObjectSuffix): TreeElementDelete.cpp $(IntermediateDirectory)/TreeElementDelete.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/TreeElementDelete.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeElementDelete.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeElementDelete.cpp$(DependSuffix): TreeElementDelete.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeElementDelete.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeElementDelete.cpp$(DependSuffix) -MM "TreeElementDelete.cpp"

$(IntermediateDirectory)/TreeElementDelete.cpp$(PreprocessSuffix): TreeElementDelete.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeElementDelete.cpp$(PreprocessSuffix) "TreeElementDelete.cpp"

$(IntermediateDirectory)/TreeElementSlide.cpp$(ObjectSuffix): TreeElementSlide.cpp $(IntermediateDirectory)/TreeElementSlide.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/TreeElementSlide.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeElementSlide.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeElementSlide.cpp$(DependSuffix): TreeElementSlide.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeElementSlide.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeElementSlide.cpp$(DependSuffix) -MM "TreeElementSlide.cpp"

$(IntermediateDirectory)/TreeElementSlide.cpp$(PreprocessSuffix): TreeElementSlide.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeElementSlide.cpp$(PreprocessSuffix) "TreeElementSlide.cpp"

$(IntermediateDirectory)/TreeElementTexte.cpp$(ObjectSuffix): TreeElementTexte.cpp $(IntermediateDirectory)/TreeElementTexte.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/TreeElementTexte.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeElementTexte.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeElementTexte.cpp$(DependSuffix): TreeElementTexte.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeElementTexte.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeElementTexte.cpp$(DependSuffix) -MM "TreeElementTexte.cpp"

$(IntermediateDirectory)/TreeElementTexte.cpp$(PreprocessSuffix): TreeElementTexte.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeElementTexte.cpp$(PreprocessSuffix) "TreeElementTexte.cpp"

$(IntermediateDirectory)/TreeElementTriangle.cpp$(ObjectSuffix): TreeElementTriangle.cpp $(IntermediateDirectory)/TreeElementTriangle.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/TreeElementTriangle.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeElementTriangle.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeElementTriangle.cpp$(DependSuffix): TreeElementTriangle.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeElementTriangle.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeElementTriangle.cpp$(DependSuffix) -MM "TreeElementTriangle.cpp"

$(IntermediateDirectory)/TreeElementTriangle.cpp$(PreprocessSuffix): TreeElementTriangle.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeElementTriangle.cpp$(PreprocessSuffix) "TreeElementTriangle.cpp"

$(IntermediateDirectory)/TreeWindow.cpp$(ObjectSuffix): TreeWindow.cpp $(IntermediateDirectory)/TreeWindow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/TreeWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeWindow.cpp$(DependSuffix): TreeWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeWindow.cpp$(DependSuffix) -MM "TreeWindow.cpp"

$(IntermediateDirectory)/TreeWindow.cpp$(PreprocessSuffix): TreeWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeWindow.cpp$(PreprocessSuffix) "TreeWindow.cpp"

$(IntermediateDirectory)/TreeElementTexteClick.cpp$(ObjectSuffix): TreeElementTexteClick.cpp $(IntermediateDirectory)/TreeElementTexteClick.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libWindow/TreeElementTexteClick.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeElementTexteClick.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeElementTexteClick.cpp$(DependSuffix): TreeElementTexteClick.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeElementTexteClick.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeElementTexteClick.cpp$(DependSuffix) -MM "TreeElementTexteClick.cpp"

$(IntermediateDirectory)/TreeElementTexteClick.cpp$(PreprocessSuffix): TreeElementTexteClick.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeElementTexteClick.cpp$(PreprocessSuffix) "TreeElementTexteClick.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


