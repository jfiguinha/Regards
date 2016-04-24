##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libVideoFFMpeg
ConfigurationName      :=Debug
WorkspacePath          := "/home/figuinha/dev/Regards"
ProjectPath            := "/home/figuinha/dev/Regards/libVideoFFMpeg"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=23/04/16
CodeLitePath           :="/home/figuinha/.codelite"
LinkerName             :=/usr/bin/g++-4.8
SharedObjectLinkerName :=/usr/bin/g++-4.8 -shared -fPIC
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
Preprocessors          :=$(PreprocessorSwitch)__WXGTK__ $(PreprocessorSwitch)SDL2 $(PreprocessorSwitch)GLEW_MX 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libVideoFFMpeg.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../libTheme $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libextern/ffmpeg-2.6.2 $(IncludeSwitch)../libextern/glew-1.13.0/include $(IncludeSwitch)../libextern/SDL2-2.0.3/include $(IncludeSwitch)../libWindowMain $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libResource $(IncludeSwitch)../libOpenGL $(IncludeSwitch)../libParameter $(IncludeSwitch)../libFiltre $(IncludeSwitch)../libUtility $(IncludeSwitch)../include $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../Otherlib/wxSVG/include $(IncludeSwitch)../libextern/wxWidgets-3.0.2/include $(IncludeSwitch)../libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0 
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
CXX      := /usr/bin/g++-4.8
CC       := /usr/bin/gcc-4.8
CXXFLAGS :=  -g -O0 -fopenmp -std=c++11 -Wall  $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall  $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/cmdutils.cpp$(ObjectSuffix) $(IntermediateDirectory)/ffplaycore.cpp$(ObjectSuffix) $(IntermediateDirectory)/ShowVideo.cpp$(ObjectSuffix) $(IntermediateDirectory)/SliderVideo.cpp$(ObjectSuffix) $(IntermediateDirectory)/VideoControl.cpp$(ObjectSuffix) $(IntermediateDirectory)/VideoWindowContext.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/home/figuinha/dev/Regards/.build-debug/libVideoFFMpeg"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/cmdutils.cpp$(ObjectSuffix): cmdutils.cpp $(IntermediateDirectory)/cmdutils.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libVideoFFMpeg/cmdutils.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/cmdutils.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/cmdutils.cpp$(DependSuffix): cmdutils.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/cmdutils.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/cmdutils.cpp$(DependSuffix) -MM "cmdutils.cpp"

$(IntermediateDirectory)/cmdutils.cpp$(PreprocessSuffix): cmdutils.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/cmdutils.cpp$(PreprocessSuffix) "cmdutils.cpp"

$(IntermediateDirectory)/ffplaycore.cpp$(ObjectSuffix): ffplaycore.cpp $(IntermediateDirectory)/ffplaycore.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libVideoFFMpeg/ffplaycore.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ffplaycore.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ffplaycore.cpp$(DependSuffix): ffplaycore.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ffplaycore.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ffplaycore.cpp$(DependSuffix) -MM "ffplaycore.cpp"

$(IntermediateDirectory)/ffplaycore.cpp$(PreprocessSuffix): ffplaycore.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ffplaycore.cpp$(PreprocessSuffix) "ffplaycore.cpp"

$(IntermediateDirectory)/ShowVideo.cpp$(ObjectSuffix): ShowVideo.cpp $(IntermediateDirectory)/ShowVideo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libVideoFFMpeg/ShowVideo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ShowVideo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ShowVideo.cpp$(DependSuffix): ShowVideo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ShowVideo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ShowVideo.cpp$(DependSuffix) -MM "ShowVideo.cpp"

$(IntermediateDirectory)/ShowVideo.cpp$(PreprocessSuffix): ShowVideo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ShowVideo.cpp$(PreprocessSuffix) "ShowVideo.cpp"

$(IntermediateDirectory)/SliderVideo.cpp$(ObjectSuffix): SliderVideo.cpp $(IntermediateDirectory)/SliderVideo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libVideoFFMpeg/SliderVideo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SliderVideo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SliderVideo.cpp$(DependSuffix): SliderVideo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SliderVideo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SliderVideo.cpp$(DependSuffix) -MM "SliderVideo.cpp"

$(IntermediateDirectory)/SliderVideo.cpp$(PreprocessSuffix): SliderVideo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SliderVideo.cpp$(PreprocessSuffix) "SliderVideo.cpp"

$(IntermediateDirectory)/VideoControl.cpp$(ObjectSuffix): VideoControl.cpp $(IntermediateDirectory)/VideoControl.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libVideoFFMpeg/VideoControl.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/VideoControl.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/VideoControl.cpp$(DependSuffix): VideoControl.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/VideoControl.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/VideoControl.cpp$(DependSuffix) -MM "VideoControl.cpp"

$(IntermediateDirectory)/VideoControl.cpp$(PreprocessSuffix): VideoControl.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/VideoControl.cpp$(PreprocessSuffix) "VideoControl.cpp"

$(IntermediateDirectory)/VideoWindowContext.cpp$(ObjectSuffix): VideoWindowContext.cpp $(IntermediateDirectory)/VideoWindowContext.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libVideoFFMpeg/VideoWindowContext.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/VideoWindowContext.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/VideoWindowContext.cpp$(DependSuffix): VideoWindowContext.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/VideoWindowContext.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/VideoWindowContext.cpp$(DependSuffix) -MM "VideoWindowContext.cpp"

$(IntermediateDirectory)/VideoWindowContext.cpp$(PreprocessSuffix): VideoWindowContext.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/VideoWindowContext.cpp$(PreprocessSuffix) "VideoWindowContext.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


