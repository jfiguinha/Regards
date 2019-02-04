##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libVideoThumbnail
ConfigurationName      :=Debug
WorkspacePath          :=/home/figuinha/dev/Regards
ProjectPath            :=/home/figuinha/dev/Regards/libVideoThumbnail
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=19/11/16
CodeLitePath           :=/home/figuinha/.codelite
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
Preprocessors          :=$(PreprocessorSwitch)LATEST_GREATEST_FFMPEG $(PreprocessorSwitch)__WXGTK__ $(PreprocessorSwitch)FFMPEG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libVideoThumbnail.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../CxImage $(IncludeSwitch)../libextern/ffmpeg-2.6.2 $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libextern/wxWidgets-3.1.0/include $(IncludeSwitch)../libextern/wxWidgets-3.1.0/lib/wx/include/gtk2-unicode-static-3.1 
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
Objects0=$(IntermediateDirectory)/cximagewriter.cpp$(ObjectSuffix) $(IntermediateDirectory)/filmstripfilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/Metadata.cpp$(ObjectSuffix) $(IntermediateDirectory)/moviedecoder.cpp$(ObjectSuffix) $(IntermediateDirectory)/stringoperations.cpp$(ObjectSuffix) $(IntermediateDirectory)/videothumbnailer.cpp$(ObjectSuffix) $(IntermediateDirectory)/videothumbnailerc.cpp$(ObjectSuffix) $(IntermediateDirectory)/videothumb.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/home/figuinha/dev/Regards/.build-debug/libVideoThumbnail"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/cximagewriter.cpp$(ObjectSuffix): cximagewriter.cpp $(IntermediateDirectory)/cximagewriter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libVideoThumbnail/cximagewriter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/cximagewriter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/cximagewriter.cpp$(DependSuffix): cximagewriter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/cximagewriter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/cximagewriter.cpp$(DependSuffix) -MM cximagewriter.cpp

$(IntermediateDirectory)/cximagewriter.cpp$(PreprocessSuffix): cximagewriter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/cximagewriter.cpp$(PreprocessSuffix)cximagewriter.cpp

$(IntermediateDirectory)/filmstripfilter.cpp$(ObjectSuffix): filmstripfilter.cpp $(IntermediateDirectory)/filmstripfilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libVideoThumbnail/filmstripfilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/filmstripfilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/filmstripfilter.cpp$(DependSuffix): filmstripfilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/filmstripfilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/filmstripfilter.cpp$(DependSuffix) -MM filmstripfilter.cpp

$(IntermediateDirectory)/filmstripfilter.cpp$(PreprocessSuffix): filmstripfilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/filmstripfilter.cpp$(PreprocessSuffix)filmstripfilter.cpp

$(IntermediateDirectory)/Metadata.cpp$(ObjectSuffix): Metadata.cpp $(IntermediateDirectory)/Metadata.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libVideoThumbnail/Metadata.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Metadata.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Metadata.cpp$(DependSuffix): Metadata.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Metadata.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Metadata.cpp$(DependSuffix) -MM Metadata.cpp

$(IntermediateDirectory)/Metadata.cpp$(PreprocessSuffix): Metadata.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Metadata.cpp$(PreprocessSuffix)Metadata.cpp

$(IntermediateDirectory)/moviedecoder.cpp$(ObjectSuffix): moviedecoder.cpp $(IntermediateDirectory)/moviedecoder.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libVideoThumbnail/moviedecoder.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/moviedecoder.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/moviedecoder.cpp$(DependSuffix): moviedecoder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/moviedecoder.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/moviedecoder.cpp$(DependSuffix) -MM moviedecoder.cpp

$(IntermediateDirectory)/moviedecoder.cpp$(PreprocessSuffix): moviedecoder.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/moviedecoder.cpp$(PreprocessSuffix)moviedecoder.cpp

$(IntermediateDirectory)/stringoperations.cpp$(ObjectSuffix): stringoperations.cpp $(IntermediateDirectory)/stringoperations.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libVideoThumbnail/stringoperations.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/stringoperations.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/stringoperations.cpp$(DependSuffix): stringoperations.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/stringoperations.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/stringoperations.cpp$(DependSuffix) -MM stringoperations.cpp

$(IntermediateDirectory)/stringoperations.cpp$(PreprocessSuffix): stringoperations.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/stringoperations.cpp$(PreprocessSuffix)stringoperations.cpp

$(IntermediateDirectory)/videothumbnailer.cpp$(ObjectSuffix): videothumbnailer.cpp $(IntermediateDirectory)/videothumbnailer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libVideoThumbnail/videothumbnailer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/videothumbnailer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/videothumbnailer.cpp$(DependSuffix): videothumbnailer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/videothumbnailer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/videothumbnailer.cpp$(DependSuffix) -MM videothumbnailer.cpp

$(IntermediateDirectory)/videothumbnailer.cpp$(PreprocessSuffix): videothumbnailer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/videothumbnailer.cpp$(PreprocessSuffix)videothumbnailer.cpp

$(IntermediateDirectory)/videothumbnailerc.cpp$(ObjectSuffix): videothumbnailerc.cpp $(IntermediateDirectory)/videothumbnailerc.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libVideoThumbnail/videothumbnailerc.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/videothumbnailerc.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/videothumbnailerc.cpp$(DependSuffix): videothumbnailerc.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/videothumbnailerc.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/videothumbnailerc.cpp$(DependSuffix) -MM videothumbnailerc.cpp

$(IntermediateDirectory)/videothumbnailerc.cpp$(PreprocessSuffix): videothumbnailerc.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/videothumbnailerc.cpp$(PreprocessSuffix)videothumbnailerc.cpp

$(IntermediateDirectory)/videothumb.cpp$(ObjectSuffix): videothumb.cpp $(IntermediateDirectory)/videothumb.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libVideoThumbnail/videothumb.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/videothumb.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/videothumb.cpp$(DependSuffix): videothumb.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/videothumb.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/videothumb.cpp$(DependSuffix) -MM videothumb.cpp

$(IntermediateDirectory)/videothumb.cpp$(PreprocessSuffix): videothumb.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/videothumb.cpp$(PreprocessSuffix)videothumb.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


