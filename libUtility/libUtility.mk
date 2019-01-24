##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libUtility
ConfigurationName      :=Release
WorkspacePath          :=/home/figuinha/developpement/Regards
ProjectPath            :=/home/figuinha/developpement/Regards/libUtility
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
ObjectsFileList        :="libUtility.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s 
IncludePath            := $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2  $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libResource $(IncludeSwitch)../libDataStructure 
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
CFLAGS   := -Wall  -O2 -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := ccache /usr/bin/x86_64-linux-gnu-as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/FileUtility.cpp$(ObjectSuffix) $(IntermediateDirectory)/ConvertUtility.cpp$(ObjectSuffix) $(IntermediateDirectory)/LoadingResource.cpp$(ObjectSuffix) $(IntermediateDirectory)/wxmd5.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/home/figuinha/developpement/Regards/.build-release/libUtility"

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
$(IntermediateDirectory)/FileUtility.cpp$(ObjectSuffix): FileUtility.cpp $(IntermediateDirectory)/FileUtility.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libUtility/FileUtility.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FileUtility.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FileUtility.cpp$(DependSuffix): FileUtility.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FileUtility.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FileUtility.cpp$(DependSuffix) -MM FileUtility.cpp

$(IntermediateDirectory)/FileUtility.cpp$(PreprocessSuffix): FileUtility.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FileUtility.cpp$(PreprocessSuffix) FileUtility.cpp

$(IntermediateDirectory)/ConvertUtility.cpp$(ObjectSuffix): ConvertUtility.cpp $(IntermediateDirectory)/ConvertUtility.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libUtility/ConvertUtility.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ConvertUtility.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ConvertUtility.cpp$(DependSuffix): ConvertUtility.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ConvertUtility.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ConvertUtility.cpp$(DependSuffix) -MM ConvertUtility.cpp

$(IntermediateDirectory)/ConvertUtility.cpp$(PreprocessSuffix): ConvertUtility.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ConvertUtility.cpp$(PreprocessSuffix) ConvertUtility.cpp

$(IntermediateDirectory)/LoadingResource.cpp$(ObjectSuffix): LoadingResource.cpp $(IntermediateDirectory)/LoadingResource.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libUtility/LoadingResource.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LoadingResource.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LoadingResource.cpp$(DependSuffix): LoadingResource.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LoadingResource.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LoadingResource.cpp$(DependSuffix) -MM LoadingResource.cpp

$(IntermediateDirectory)/LoadingResource.cpp$(PreprocessSuffix): LoadingResource.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LoadingResource.cpp$(PreprocessSuffix) LoadingResource.cpp

$(IntermediateDirectory)/wxmd5.cpp$(ObjectSuffix): wxmd5.cpp $(IntermediateDirectory)/wxmd5.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libUtility/wxmd5.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wxmd5.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wxmd5.cpp$(DependSuffix): wxmd5.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/wxmd5.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/wxmd5.cpp$(DependSuffix) -MM wxmd5.cpp

$(IntermediateDirectory)/wxmd5.cpp$(PreprocessSuffix): wxmd5.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wxmd5.cpp$(PreprocessSuffix) wxmd5.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/
	$(RM) ../include/Release/header.h.gch


