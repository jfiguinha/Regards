##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=RegardsRaw
ConfigurationName      :=Release
WorkspacePath          := "/home/figuinha/dev/Regards"
ProjectPath            := "/home/figuinha/dev/Regards/Otherlib/RegardsRaw"
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=24/04/16
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
OutputFile             :=$(IntermediateDirectory)/lib$(ProjectName).a
Preprocessors          :=$(PreprocessorSwitch)NDEBUG $(PreprocessorSwitch)LIBRAW_LIBRARY_BUILD $(PreprocessorSwitch)LIBRAW $(PreprocessorSwitch)__WXGTK__ 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="RegardsRaw.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -O2
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../../libextern/LibRaw-0.17.0/ $(IncludeSwitch)../../libextern/LibRaw-0.17.0/libraw $(IncludeSwitch)../../libDataStructure $(IncludeSwitch)../../libUtility $(IncludeSwitch)../../CxImage $(IncludeSwitch)../../libextern/wxWidgets-3.0.2/include $(IncludeSwitch)../../libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0 
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
CXXFLAGS :=  -O3 -fopenmp -std=c++11 -Wall  $(Preprocessors)
CFLAGS   :=  -g $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/RegardsRaw.cpp$(ObjectSuffix) $(IntermediateDirectory)/raw.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/home/figuinha/dev/Regards/.build-release/RegardsRaw"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


./Release:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/RegardsRaw.cpp$(ObjectSuffix): RegardsRaw.cpp $(IntermediateDirectory)/RegardsRaw.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/RegardsRaw/RegardsRaw.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RegardsRaw.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RegardsRaw.cpp$(DependSuffix): RegardsRaw.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RegardsRaw.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RegardsRaw.cpp$(DependSuffix) -MM "RegardsRaw.cpp"

$(IntermediateDirectory)/RegardsRaw.cpp$(PreprocessSuffix): RegardsRaw.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RegardsRaw.cpp$(PreprocessSuffix) "RegardsRaw.cpp"

$(IntermediateDirectory)/raw.cpp$(ObjectSuffix): raw.cpp $(IntermediateDirectory)/raw.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/RegardsRaw/raw.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/raw.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/raw.cpp$(DependSuffix): raw.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/raw.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/raw.cpp$(DependSuffix) -MM "raw.cpp"

$(IntermediateDirectory)/raw.cpp$(PreprocessSuffix): raw.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/raw.cpp$(PreprocessSuffix) "raw.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


