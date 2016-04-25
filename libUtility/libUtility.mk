##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libUtility
ConfigurationName      :=Release
WorkspacePath          := "/home/figuinha/dev/Regards"
ProjectPath            := "/home/figuinha/dev/Regards/libUtility"
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=24/04/16
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
Preprocessors          :=$(PreprocessorSwitch)NDEBUG $(PreprocessorSwitch)__WXGTK__ 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libUtility.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s 
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libextern/exiv2-0.24/xmpsdk/include $(IncludeSwitch)../libResource $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libextern/wxWidgets-3.0.2/include $(IncludeSwitch)../libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0 
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
CXXFLAGS :=  -fopenmp -std=c++11 -Wall $(Preprocessors)
CFLAGS   :=  -O2 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/ConvertUtility.cpp$(ObjectSuffix) $(IntermediateDirectory)/FileUtility.cpp$(ObjectSuffix) $(IntermediateDirectory)/LoadingResource.cpp$(ObjectSuffix) $(IntermediateDirectory)/wxmd5.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/home/figuinha/dev/Regards/.build-release/libUtility"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


./Release:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/ConvertUtility.cpp$(ObjectSuffix): ConvertUtility.cpp $(IntermediateDirectory)/ConvertUtility.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libUtility/ConvertUtility.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ConvertUtility.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ConvertUtility.cpp$(DependSuffix): ConvertUtility.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ConvertUtility.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ConvertUtility.cpp$(DependSuffix) -MM "ConvertUtility.cpp"

$(IntermediateDirectory)/ConvertUtility.cpp$(PreprocessSuffix): ConvertUtility.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ConvertUtility.cpp$(PreprocessSuffix) "ConvertUtility.cpp"

$(IntermediateDirectory)/FileUtility.cpp$(ObjectSuffix): FileUtility.cpp $(IntermediateDirectory)/FileUtility.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libUtility/FileUtility.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FileUtility.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FileUtility.cpp$(DependSuffix): FileUtility.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FileUtility.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FileUtility.cpp$(DependSuffix) -MM "FileUtility.cpp"

$(IntermediateDirectory)/FileUtility.cpp$(PreprocessSuffix): FileUtility.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FileUtility.cpp$(PreprocessSuffix) "FileUtility.cpp"

$(IntermediateDirectory)/LoadingResource.cpp$(ObjectSuffix): LoadingResource.cpp $(IntermediateDirectory)/LoadingResource.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libUtility/LoadingResource.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LoadingResource.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LoadingResource.cpp$(DependSuffix): LoadingResource.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LoadingResource.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LoadingResource.cpp$(DependSuffix) -MM "LoadingResource.cpp"

$(IntermediateDirectory)/LoadingResource.cpp$(PreprocessSuffix): LoadingResource.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LoadingResource.cpp$(PreprocessSuffix) "LoadingResource.cpp"

$(IntermediateDirectory)/wxmd5.cpp$(ObjectSuffix): wxmd5.cpp $(IntermediateDirectory)/wxmd5.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libUtility/wxmd5.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wxmd5.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wxmd5.cpp$(DependSuffix): wxmd5.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/wxmd5.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/wxmd5.cpp$(DependSuffix) -MM "wxmd5.cpp"

$(IntermediateDirectory)/wxmd5.cpp$(PreprocessSuffix): wxmd5.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wxmd5.cpp$(PreprocessSuffix) "wxmd5.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


