##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libParameter
ConfigurationName      :=Release
WorkspacePath          :=/home/figuinha/developpement/Regards
ProjectPath            :=/home/figuinha/developpement/Regards/libParameter
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=24/01/19
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
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libParameter.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s
IncludePath            := $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2  $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../include $(IncludeSwitch)../libUtility 
IncludePCH             :=  -include ../include/Release/header.h 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                :=$(LibraryPathSwitch)/opt/AMDAPPSDK-3.0/lib/x86_64/sdk/  $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/x86_64-linux-gnu-ar rcu
CXX      := ccache /usr/bin/x86_64-linux-gnu-g++
CC       := ccache /usr/bin/x86_64-linux-gnu-gcc
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread  -fopenmp -std=gnu++11 -Wall -O2 -msse4.1 $(Preprocessors)
CFLAGS   :=  -Wall  -O2 -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := ccache /usr/bin/x86_64-linux-gnu-as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/RegardsConfigParam.cpp$(ObjectSuffix) $(IntermediateDirectory)/ConfigParam.cpp$(ObjectSuffix) $(IntermediateDirectory)/ParamInit.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "/home/figuinha/developpement/Regards/.build-release"
	@echo rebuilt > "/home/figuinha/developpement/Regards/.build-release/libParameter"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


./Release:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:

# PreCompiled Header
../include/Release/header.h.gch: ../include/Release/header.h
	$(CXX) $(SourceSwitch) ../include/Release/header.h $(PCHCompileFlags) $(CXXFLAGS) $(IncludePath)



##
## Objects
##
$(IntermediateDirectory)/RegardsConfigParam.cpp$(ObjectSuffix): RegardsConfigParam.cpp $(IntermediateDirectory)/RegardsConfigParam.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libParameter/RegardsConfigParam.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RegardsConfigParam.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RegardsConfigParam.cpp$(DependSuffix): RegardsConfigParam.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RegardsConfigParam.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RegardsConfigParam.cpp$(DependSuffix) -MM RegardsConfigParam.cpp

$(IntermediateDirectory)/RegardsConfigParam.cpp$(PreprocessSuffix): RegardsConfigParam.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RegardsConfigParam.cpp$(PreprocessSuffix) RegardsConfigParam.cpp

$(IntermediateDirectory)/ConfigParam.cpp$(ObjectSuffix): ConfigParam.cpp $(IntermediateDirectory)/ConfigParam.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libParameter/ConfigParam.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ConfigParam.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ConfigParam.cpp$(DependSuffix): ConfigParam.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ConfigParam.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ConfigParam.cpp$(DependSuffix) -MM ConfigParam.cpp

$(IntermediateDirectory)/ConfigParam.cpp$(PreprocessSuffix): ConfigParam.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ConfigParam.cpp$(PreprocessSuffix) ConfigParam.cpp

$(IntermediateDirectory)/ParamInit.cpp$(ObjectSuffix): ParamInit.cpp $(IntermediateDirectory)/ParamInit.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libParameter/ParamInit.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ParamInit.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ParamInit.cpp$(DependSuffix): ParamInit.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ParamInit.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ParamInit.cpp$(DependSuffix) -MM ParamInit.cpp

$(IntermediateDirectory)/ParamInit.cpp$(PreprocessSuffix): ParamInit.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ParamInit.cpp$(PreprocessSuffix) ParamInit.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/
	$(RM) ../include/Release/header.h.gch


