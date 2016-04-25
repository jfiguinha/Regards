##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=RegardsBpg
ConfigurationName      :=Release
WorkspacePath          := "/home/figuinha/dev/Regards/Otherlib"
ProjectPath            := "/home/figuinha/dev/Regards/Otherlib/RegardsBpg"
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
OutputFile             :=$(IntermediateDirectory)/$(ProjectName).so
Preprocessors          :=$(PreprocessorSwitch)NDEBUG $(PreprocessorSwitch)USE_VAR_BIT_DEPTH 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="RegardsBpg.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -O2
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../../libextern/lpng1250 $(IncludeSwitch)../../libextern/libbpg-0.9.5 $(IncludeSwitch)../../libextern/x265_1.7/source $(IncludeSwitch)../../libextern/x265_1.7/include 
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
CXXFLAGS :=  -g -O2 -W -std=c++11 -Wall -fPIC $(Preprocessors)
CFLAGS   :=  -g -O0 -W -std=c99 -fPIC $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/bpg.cpp$(ObjectSuffix) $(IntermediateDirectory)/bpgenc.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(SharedObjectLinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)
	@$(MakeDirCommand) "/home/figuinha/dev/Regards/Otherlib/.build-release"
	@echo rebuilt > "/home/figuinha/dev/Regards/Otherlib/.build-release/RegardsBpg"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


$(IntermediateDirectory)/.d:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/bpg.cpp$(ObjectSuffix): bpg.cpp $(IntermediateDirectory)/bpg.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/RegardsBpg/bpg.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bpg.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bpg.cpp$(DependSuffix): bpg.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bpg.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bpg.cpp$(DependSuffix) -MM "bpg.cpp"

$(IntermediateDirectory)/bpg.cpp$(PreprocessSuffix): bpg.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bpg.cpp$(PreprocessSuffix) "bpg.cpp"

$(IntermediateDirectory)/bpgenc.c$(ObjectSuffix): bpgenc.c $(IntermediateDirectory)/bpgenc.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/RegardsBpg/bpgenc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bpgenc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bpgenc.c$(DependSuffix): bpgenc.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bpgenc.c$(ObjectSuffix) -MF$(IntermediateDirectory)/bpgenc.c$(DependSuffix) -MM "bpgenc.c"

$(IntermediateDirectory)/bpgenc.c$(PreprocessSuffix): bpgenc.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bpgenc.c$(PreprocessSuffix) "bpgenc.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


