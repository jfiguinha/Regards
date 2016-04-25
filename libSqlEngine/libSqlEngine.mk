##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libSqlEngine
ConfigurationName      :=Debug
WorkspacePath          := "/home/figuinha/dev/Regards"
ProjectPath            := "/home/figuinha/dev/Regards/libSqlEngine"
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
Preprocessors          :=$(PreprocessorSwitch)__WXGTK__ 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libSqlEngine.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libextern/wxWidgets-3.0.2/include $(IncludeSwitch)../libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0 
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
Objects0=$(IntermediateDirectory)/SqlEngine.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlExecuteRequest.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlLib.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlResult.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/home/figuinha/dev/Regards/.build-debug/libSqlEngine"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/SqlEngine.cpp$(ObjectSuffix): SqlEngine.cpp $(IntermediateDirectory)/SqlEngine.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libSqlEngine/SqlEngine.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlEngine.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlEngine.cpp$(DependSuffix): SqlEngine.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlEngine.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlEngine.cpp$(DependSuffix) -MM "SqlEngine.cpp"

$(IntermediateDirectory)/SqlEngine.cpp$(PreprocessSuffix): SqlEngine.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlEngine.cpp$(PreprocessSuffix) "SqlEngine.cpp"

$(IntermediateDirectory)/SqlExecuteRequest.cpp$(ObjectSuffix): SqlExecuteRequest.cpp $(IntermediateDirectory)/SqlExecuteRequest.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libSqlEngine/SqlExecuteRequest.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlExecuteRequest.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlExecuteRequest.cpp$(DependSuffix): SqlExecuteRequest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlExecuteRequest.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlExecuteRequest.cpp$(DependSuffix) -MM "SqlExecuteRequest.cpp"

$(IntermediateDirectory)/SqlExecuteRequest.cpp$(PreprocessSuffix): SqlExecuteRequest.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlExecuteRequest.cpp$(PreprocessSuffix) "SqlExecuteRequest.cpp"

$(IntermediateDirectory)/SqlLib.cpp$(ObjectSuffix): SqlLib.cpp $(IntermediateDirectory)/SqlLib.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libSqlEngine/SqlLib.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlLib.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlLib.cpp$(DependSuffix): SqlLib.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlLib.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlLib.cpp$(DependSuffix) -MM "SqlLib.cpp"

$(IntermediateDirectory)/SqlLib.cpp$(PreprocessSuffix): SqlLib.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlLib.cpp$(PreprocessSuffix) "SqlLib.cpp"

$(IntermediateDirectory)/SqlResult.cpp$(ObjectSuffix): SqlResult.cpp $(IntermediateDirectory)/SqlResult.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libSqlEngine/SqlResult.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlResult.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlResult.cpp$(DependSuffix): SqlResult.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlResult.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlResult.cpp$(DependSuffix) -MM "SqlResult.cpp"

$(IntermediateDirectory)/SqlResult.cpp$(PreprocessSuffix): SqlResult.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlResult.cpp$(PreprocessSuffix) "SqlResult.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


