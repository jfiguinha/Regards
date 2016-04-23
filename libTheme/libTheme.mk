##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libTheme
ConfigurationName      :=Debug
WorkspacePath          := "/home/figuinha/dev/Regards"
ProjectPath            := "/home/figuinha/dev/Regards/libTheme"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=root
Date                   :=22/09/15
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
ObjectsFileList        :="libTheme.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../../libextern/libRapidXML $(IncludeSwitch)../libParameter $(IncludeSwitch)../include $(IncludeSwitch)../../libextern/wxWidgets-3.0.2/include $(IncludeSwitch)../../libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0 
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
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/ThemeInit.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThemeParam.cpp$(ObjectSuffix) $(IntermediateDirectory)/Theme.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/home/figuinha/dev/Regards/.build-debug/libTheme"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/ThemeInit.cpp$(ObjectSuffix): ThemeInit.cpp $(IntermediateDirectory)/ThemeInit.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libTheme/ThemeInit.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThemeInit.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThemeInit.cpp$(DependSuffix): ThemeInit.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThemeInit.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThemeInit.cpp$(DependSuffix) -MM "ThemeInit.cpp"

$(IntermediateDirectory)/ThemeInit.cpp$(PreprocessSuffix): ThemeInit.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThemeInit.cpp$(PreprocessSuffix) "ThemeInit.cpp"

$(IntermediateDirectory)/ThemeParam.cpp$(ObjectSuffix): ThemeParam.cpp $(IntermediateDirectory)/ThemeParam.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libTheme/ThemeParam.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThemeParam.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThemeParam.cpp$(DependSuffix): ThemeParam.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThemeParam.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThemeParam.cpp$(DependSuffix) -MM "ThemeParam.cpp"

$(IntermediateDirectory)/ThemeParam.cpp$(PreprocessSuffix): ThemeParam.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThemeParam.cpp$(PreprocessSuffix) "ThemeParam.cpp"

$(IntermediateDirectory)/Theme.cpp$(ObjectSuffix): Theme.cpp $(IntermediateDirectory)/Theme.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libTheme/Theme.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Theme.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Theme.cpp$(DependSuffix): Theme.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Theme.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Theme.cpp$(DependSuffix) -MM "Theme.cpp"

$(IntermediateDirectory)/Theme.cpp$(PreprocessSuffix): Theme.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Theme.cpp$(PreprocessSuffix) "Theme.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


