##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libParameter
ConfigurationName      :=Debug
WorkspacePath          := "/home/figuinha/dev/Regards"
ProjectPath            := "/home/figuinha/dev/Regards/libParameter"
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
Preprocessors          :=$(PreprocessorSwitch)__WXGTK__ 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libParameter.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libextern/wxWidgets-3.0.2/include $(IncludeSwitch)../libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0 
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
Objects0=$(IntermediateDirectory)/ConfigParam.cpp$(ObjectSuffix) $(IntermediateDirectory)/ParamInit.cpp$(ObjectSuffix) $(IntermediateDirectory)/RegardsConfigParam.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/home/figuinha/dev/Regards/.build-debug/libParameter"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/ConfigParam.cpp$(ObjectSuffix): ConfigParam.cpp $(IntermediateDirectory)/ConfigParam.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libParameter/ConfigParam.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ConfigParam.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ConfigParam.cpp$(DependSuffix): ConfigParam.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ConfigParam.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ConfigParam.cpp$(DependSuffix) -MM "ConfigParam.cpp"

$(IntermediateDirectory)/ConfigParam.cpp$(PreprocessSuffix): ConfigParam.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ConfigParam.cpp$(PreprocessSuffix) "ConfigParam.cpp"

$(IntermediateDirectory)/ParamInit.cpp$(ObjectSuffix): ParamInit.cpp $(IntermediateDirectory)/ParamInit.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libParameter/ParamInit.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ParamInit.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ParamInit.cpp$(DependSuffix): ParamInit.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ParamInit.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ParamInit.cpp$(DependSuffix) -MM "ParamInit.cpp"

$(IntermediateDirectory)/ParamInit.cpp$(PreprocessSuffix): ParamInit.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ParamInit.cpp$(PreprocessSuffix) "ParamInit.cpp"

$(IntermediateDirectory)/RegardsConfigParam.cpp$(ObjectSuffix): RegardsConfigParam.cpp $(IntermediateDirectory)/RegardsConfigParam.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libParameter/RegardsConfigParam.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RegardsConfigParam.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RegardsConfigParam.cpp$(DependSuffix): RegardsConfigParam.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RegardsConfigParam.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RegardsConfigParam.cpp$(DependSuffix) -MM "RegardsConfigParam.cpp"

$(IntermediateDirectory)/RegardsConfigParam.cpp$(PreprocessSuffix): RegardsConfigParam.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RegardsConfigParam.cpp$(PreprocessSuffix) "RegardsConfigParam.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


