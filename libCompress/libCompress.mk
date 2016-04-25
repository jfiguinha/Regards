##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libCompress
ConfigurationName      :=Debug
WorkspacePath          := "/home/figuinha/dev/Regards"
ProjectPath            := "/home/figuinha/dev/Regards/libCompress"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=25/04/16
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
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libCompress.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../libCompress 
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
CXXFLAGS :=  -g -O0 -fopenmp -std=c++11 -Wall -fpermissive  $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall  $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/jpgd.cpp$(ObjectSuffix) $(IntermediateDirectory)/jpge.cpp$(ObjectSuffix) $(IntermediateDirectory)/lz4.c$(ObjectSuffix) 



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
	@echo rebuilt > "/home/figuinha/dev/Regards/.build-debug/libCompress"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/jpgd.cpp$(ObjectSuffix): jpgd.cpp $(IntermediateDirectory)/jpgd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libCompress/jpgd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/jpgd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/jpgd.cpp$(DependSuffix): jpgd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/jpgd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/jpgd.cpp$(DependSuffix) -MM "jpgd.cpp"

$(IntermediateDirectory)/jpgd.cpp$(PreprocessSuffix): jpgd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/jpgd.cpp$(PreprocessSuffix) "jpgd.cpp"

$(IntermediateDirectory)/jpge.cpp$(ObjectSuffix): jpge.cpp $(IntermediateDirectory)/jpge.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libCompress/jpge.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/jpge.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/jpge.cpp$(DependSuffix): jpge.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/jpge.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/jpge.cpp$(DependSuffix) -MM "jpge.cpp"

$(IntermediateDirectory)/jpge.cpp$(PreprocessSuffix): jpge.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/jpge.cpp$(PreprocessSuffix) "jpge.cpp"

$(IntermediateDirectory)/lz4.c$(ObjectSuffix): lz4.c $(IntermediateDirectory)/lz4.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/libCompress/lz4.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lz4.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lz4.c$(DependSuffix): lz4.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lz4.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lz4.c$(DependSuffix) -MM "lz4.c"

$(IntermediateDirectory)/lz4.c$(PreprocessSuffix): lz4.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lz4.c$(PreprocessSuffix) "lz4.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


