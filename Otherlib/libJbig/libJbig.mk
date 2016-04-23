##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libJbig
ConfigurationName      :=Release
WorkspacePath          := "/home/figuinha/dev/Regards/Otherlib"
ProjectPath            := "/home/figuinha/dev/Regards/Otherlib/libJbig"
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=root
Date                   :=21/09/15
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
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libJbig.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
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
CXXFLAGS :=   $(Preprocessors)
CFLAGS   :=   $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/jbig.c$(ObjectSuffix) $(IntermediateDirectory)/jbig_tab.c$(ObjectSuffix) $(IntermediateDirectory)/tstcodec.c$(ObjectSuffix) 



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
	@$(MakeDirCommand) "/home/figuinha/dev/Regards/Otherlib/.build-release"
	@echo rebuilt > "/home/figuinha/dev/Regards/Otherlib/.build-release/libJbig"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


./Release:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/jbig.c$(ObjectSuffix): jbig.c $(IntermediateDirectory)/jbig.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libJbig/jbig.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/jbig.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/jbig.c$(DependSuffix): jbig.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/jbig.c$(ObjectSuffix) -MF$(IntermediateDirectory)/jbig.c$(DependSuffix) -MM "jbig.c"

$(IntermediateDirectory)/jbig.c$(PreprocessSuffix): jbig.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/jbig.c$(PreprocessSuffix) "jbig.c"

$(IntermediateDirectory)/jbig_tab.c$(ObjectSuffix): jbig_tab.c $(IntermediateDirectory)/jbig_tab.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libJbig/jbig_tab.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/jbig_tab.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/jbig_tab.c$(DependSuffix): jbig_tab.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/jbig_tab.c$(ObjectSuffix) -MF$(IntermediateDirectory)/jbig_tab.c$(DependSuffix) -MM "jbig_tab.c"

$(IntermediateDirectory)/jbig_tab.c$(PreprocessSuffix): jbig_tab.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/jbig_tab.c$(PreprocessSuffix) "jbig_tab.c"

$(IntermediateDirectory)/tstcodec.c$(ObjectSuffix): tstcodec.c $(IntermediateDirectory)/tstcodec.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libJbig/tstcodec.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tstcodec.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tstcodec.c$(DependSuffix): tstcodec.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/tstcodec.c$(ObjectSuffix) -MF$(IntermediateDirectory)/tstcodec.c$(DependSuffix) -MM "tstcodec.c"

$(IntermediateDirectory)/tstcodec.c$(PreprocessSuffix): tstcodec.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tstcodec.c$(PreprocessSuffix) "tstcodec.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


