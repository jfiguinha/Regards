##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libSqlEngine
ConfigurationName      :=Debug
WorkspacePath          :=/home/figuinha/Developpement/Regards
ProjectPath            :=/home/figuinha/Developpement/Regards/libSqlEngine
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=20/01/19
CodeLitePath           :=/home/figuinha/.codelite
LinkerName             :=/usr/bin/ccache /usr/bin/g++
SharedObjectLinkerName :=/usr/bin/ccache /usr/bin/g++ -shared -fPIC
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
ObjectsFileList        :="libSqlEngine.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2  $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libUtility $(IncludeSwitch)../libResource 
IncludePCH             :=  -include ../include/Debug/header.h 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                :=$(LibraryPathSwitch)/opt/AMDAPPSDK-3.0/lib  $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/ccache /usr/bin/g++
CC       := /usr/bin/ccache /usr/bin/gcc
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread -fopenmp -std=gnu++11 -Wall -ggdb -O0 $(Preprocessors)
CFLAGS   :=  -Wall  -ggdb -O0 $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/SqlExecuteRequest.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlEngine.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlLib.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlResult.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "/home/figuinha/Developpement/Regards/.build-debug"
	@echo rebuilt > "/home/figuinha/Developpement/Regards/.build-debug/libSqlEngine"

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
$(IntermediateDirectory)/SqlExecuteRequest.cpp$(ObjectSuffix): SqlExecuteRequest.cpp $(IntermediateDirectory)/SqlExecuteRequest.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libSqlEngine/SqlExecuteRequest.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlExecuteRequest.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlExecuteRequest.cpp$(DependSuffix): SqlExecuteRequest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlExecuteRequest.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlExecuteRequest.cpp$(DependSuffix) -MM SqlExecuteRequest.cpp

$(IntermediateDirectory)/SqlExecuteRequest.cpp$(PreprocessSuffix): SqlExecuteRequest.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlExecuteRequest.cpp$(PreprocessSuffix) SqlExecuteRequest.cpp

$(IntermediateDirectory)/SqlEngine.cpp$(ObjectSuffix): SqlEngine.cpp $(IntermediateDirectory)/SqlEngine.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libSqlEngine/SqlEngine.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlEngine.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlEngine.cpp$(DependSuffix): SqlEngine.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlEngine.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlEngine.cpp$(DependSuffix) -MM SqlEngine.cpp

$(IntermediateDirectory)/SqlEngine.cpp$(PreprocessSuffix): SqlEngine.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlEngine.cpp$(PreprocessSuffix) SqlEngine.cpp

$(IntermediateDirectory)/SqlLib.cpp$(ObjectSuffix): SqlLib.cpp $(IntermediateDirectory)/SqlLib.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libSqlEngine/SqlLib.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlLib.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlLib.cpp$(DependSuffix): SqlLib.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlLib.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlLib.cpp$(DependSuffix) -MM SqlLib.cpp

$(IntermediateDirectory)/SqlLib.cpp$(PreprocessSuffix): SqlLib.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlLib.cpp$(PreprocessSuffix) SqlLib.cpp

$(IntermediateDirectory)/SqlResult.cpp$(ObjectSuffix): SqlResult.cpp $(IntermediateDirectory)/SqlResult.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libSqlEngine/SqlResult.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlResult.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlResult.cpp$(DependSuffix): SqlResult.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlResult.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlResult.cpp$(DependSuffix) -MM SqlResult.cpp

$(IntermediateDirectory)/SqlResult.cpp$(PreprocessSuffix): SqlResult.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlResult.cpp$(PreprocessSuffix) SqlResult.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/
	$(RM) ../include/Debug/header.h.gch


