##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libTheme
ConfigurationName      :=Release
WorkspacePath          :=/home/figuinha/developpement/Regards
ProjectPath            :=/home/figuinha/developpement/Regards/libTheme
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
ObjectsFileList        :="libTheme.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s 
IncludePath            := $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2  $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libParameter $(IncludeSwitch)../include $(IncludeSwitch)../libUtility 
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
CFLAGS   :=  -Wall -O2 -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := ccache /usr/bin/x86_64-linux-gnu-as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/theme.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThemeInit.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThemeParam.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/home/figuinha/developpement/Regards/.build-release/libTheme"

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
$(IntermediateDirectory)/theme.cpp$(ObjectSuffix): theme.cpp $(IntermediateDirectory)/theme.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libTheme/theme.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/theme.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/theme.cpp$(DependSuffix): theme.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/theme.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/theme.cpp$(DependSuffix) -MM theme.cpp

$(IntermediateDirectory)/theme.cpp$(PreprocessSuffix): theme.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/theme.cpp$(PreprocessSuffix) theme.cpp

$(IntermediateDirectory)/ThemeInit.cpp$(ObjectSuffix): ThemeInit.cpp $(IntermediateDirectory)/ThemeInit.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libTheme/ThemeInit.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThemeInit.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThemeInit.cpp$(DependSuffix): ThemeInit.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThemeInit.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThemeInit.cpp$(DependSuffix) -MM ThemeInit.cpp

$(IntermediateDirectory)/ThemeInit.cpp$(PreprocessSuffix): ThemeInit.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThemeInit.cpp$(PreprocessSuffix) ThemeInit.cpp

$(IntermediateDirectory)/ThemeParam.cpp$(ObjectSuffix): ThemeParam.cpp $(IntermediateDirectory)/ThemeParam.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libTheme/ThemeParam.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThemeParam.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThemeParam.cpp$(DependSuffix): ThemeParam.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThemeParam.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThemeParam.cpp$(DependSuffix) -MM ThemeParam.cpp

$(IntermediateDirectory)/ThemeParam.cpp$(PreprocessSuffix): ThemeParam.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThemeParam.cpp$(PreprocessSuffix) ThemeParam.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/
	$(RM) ../include/Release/header.h.gch


