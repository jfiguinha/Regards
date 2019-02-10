##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libVideoThumbnail
ConfigurationName      :=Debug
WorkspacePath          :=/home/figuinha/developpement/Regards
ProjectPath            :=/home/figuinha/developpement/Regards/libVideoThumbnail
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=10/02/19
CodeLitePath           :=/home/figuinha/.codelite
LinkerName             :=ccache /usr/bin/x86_64-linux-gnu-g++
SharedObjectLinkerName :=ccache /usr/bin/x86_64-linux-gnu-g++ -shared -fPIC
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
Preprocessors          :=$(PreprocessorSwitch)LATEST_GREATEST_FFMPEG $(PreprocessorSwitch)FFMPEG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libVideoThumbnail.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2 $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include  $(IncludeSwitch). $(IncludeSwitch)../CxImage $(IncludeSwitch)../libextern/ffmpeg-4.1 $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libUtility 
IncludePCH             :=  -include ../include/Debug/header.h 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                :=$(LibraryPathSwitch)/opt/AMDAPPSDK-3.0/lib/x86_64/sdk  $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/x86_64-linux-gnu-ar rcu
CXX      := ccache /usr/bin/x86_64-linux-gnu-g++
CC       := ccache /usr/bin/x86_64-linux-gnu-gcc
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread  -fopenmp -std=gnu++11 -Wall -ggdb -O0 $(Preprocessors)
CFLAGS   :=  -Wall  -ggdb -O0 $(Preprocessors)
ASFLAGS  := 
AS       := ccache /usr/bin/x86_64-linux-gnu-as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/videothumb.cpp$(ObjectSuffix) $(IntermediateDirectory)/stringoperations.cpp$(ObjectSuffix) $(IntermediateDirectory)/filmstripfilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/moviestreaminfo.cpp$(ObjectSuffix) $(IntermediateDirectory)/moviedecoder.cpp$(ObjectSuffix) $(IntermediateDirectory)/videostream.cpp$(ObjectSuffix) $(IntermediateDirectory)/videothumbnailer.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "/home/figuinha/developpement/Regards/.build-debug"
	@echo rebuilt > "/home/figuinha/developpement/Regards/.build-debug/libVideoThumbnail"

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
$(IntermediateDirectory)/videothumb.cpp$(ObjectSuffix): videothumb.cpp $(IntermediateDirectory)/videothumb.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libVideoThumbnail/videothumb.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/videothumb.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/videothumb.cpp$(DependSuffix): videothumb.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/videothumb.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/videothumb.cpp$(DependSuffix) -MM videothumb.cpp

$(IntermediateDirectory)/videothumb.cpp$(PreprocessSuffix): videothumb.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/videothumb.cpp$(PreprocessSuffix) videothumb.cpp

$(IntermediateDirectory)/stringoperations.cpp$(ObjectSuffix): stringoperations.cpp $(IntermediateDirectory)/stringoperations.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libVideoThumbnail/stringoperations.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/stringoperations.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/stringoperations.cpp$(DependSuffix): stringoperations.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/stringoperations.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/stringoperations.cpp$(DependSuffix) -MM stringoperations.cpp

$(IntermediateDirectory)/stringoperations.cpp$(PreprocessSuffix): stringoperations.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/stringoperations.cpp$(PreprocessSuffix) stringoperations.cpp

$(IntermediateDirectory)/filmstripfilter.cpp$(ObjectSuffix): filmstripfilter.cpp $(IntermediateDirectory)/filmstripfilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libVideoThumbnail/filmstripfilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/filmstripfilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/filmstripfilter.cpp$(DependSuffix): filmstripfilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/filmstripfilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/filmstripfilter.cpp$(DependSuffix) -MM filmstripfilter.cpp

$(IntermediateDirectory)/filmstripfilter.cpp$(PreprocessSuffix): filmstripfilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/filmstripfilter.cpp$(PreprocessSuffix) filmstripfilter.cpp

$(IntermediateDirectory)/moviestreaminfo.cpp$(ObjectSuffix): moviestreaminfo.cpp $(IntermediateDirectory)/moviestreaminfo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libVideoThumbnail/moviestreaminfo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/moviestreaminfo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/moviestreaminfo.cpp$(DependSuffix): moviestreaminfo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/moviestreaminfo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/moviestreaminfo.cpp$(DependSuffix) -MM moviestreaminfo.cpp

$(IntermediateDirectory)/moviestreaminfo.cpp$(PreprocessSuffix): moviestreaminfo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/moviestreaminfo.cpp$(PreprocessSuffix) moviestreaminfo.cpp

$(IntermediateDirectory)/moviedecoder.cpp$(ObjectSuffix): moviedecoder.cpp $(IntermediateDirectory)/moviedecoder.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libVideoThumbnail/moviedecoder.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/moviedecoder.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/moviedecoder.cpp$(DependSuffix): moviedecoder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/moviedecoder.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/moviedecoder.cpp$(DependSuffix) -MM moviedecoder.cpp

$(IntermediateDirectory)/moviedecoder.cpp$(PreprocessSuffix): moviedecoder.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/moviedecoder.cpp$(PreprocessSuffix) moviedecoder.cpp

$(IntermediateDirectory)/videostream.cpp$(ObjectSuffix): videostream.cpp $(IntermediateDirectory)/videostream.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libVideoThumbnail/videostream.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/videostream.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/videostream.cpp$(DependSuffix): videostream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/videostream.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/videostream.cpp$(DependSuffix) -MM videostream.cpp

$(IntermediateDirectory)/videostream.cpp$(PreprocessSuffix): videostream.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/videostream.cpp$(PreprocessSuffix) videostream.cpp

$(IntermediateDirectory)/videothumbnailer.cpp$(ObjectSuffix): videothumbnailer.cpp $(IntermediateDirectory)/videothumbnailer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libVideoThumbnail/videothumbnailer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/videothumbnailer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/videothumbnailer.cpp$(DependSuffix): videothumbnailer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/videothumbnailer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/videothumbnailer.cpp$(DependSuffix) -MM videothumbnailer.cpp

$(IntermediateDirectory)/videothumbnailer.cpp$(PreprocessSuffix): videothumbnailer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/videothumbnailer.cpp$(PreprocessSuffix) videothumbnailer.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/
	$(RM) ../include/Debug/header.h.gch


