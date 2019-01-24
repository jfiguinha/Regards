##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=include
ConfigurationName      :=Release
WorkspacePath          :=/home/figuinha/developpement/Regards
ProjectPath            :=/home/figuinha/developpement/Regards/include
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
OutputFile             :=$(IntermediateDirectory)/lib$(ProjectName).a
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="include.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2  $(IncludeSwitch). $(IncludeSwitch)../libCompress $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libUtility $(IncludeSwitch)../libResource $(IncludeSwitch)../libPicture $(IncludeSwitch)../CxImage $(IncludeSwitch)../libFiltre $(IncludeSwitch)../libextern/libjpeg-turbo-1.5.1 $(IncludeSwitch)../libExif $(IncludeSwitch)../include $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch). 
IncludePCH             :=  -include Release/header.h 
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
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread  -fopenmp -std=gnu++11 -O2 -msse4.1 $(Preprocessors)
CFLAGS   :=  -O2 -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := ccache /usr/bin/x86_64-linux-gnu-as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/header.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/home/figuinha/developpement/Regards/.build-release/include"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


./Release:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:
	@echo Executing Pre Build commands ...
	if [ -d Release ]; then echo "Exists"; else mkdir Release;cp ../include/header.h Release/header.h;fi
	@echo Done

# PreCompiled Header
Release/header.h.gch: Release/header.h
	$(CXX) $(SourceSwitch) Release/header.h $(PCHCompileFlags) $(CXXFLAGS) $(IncludePath)



##
## Objects
##
$(IntermediateDirectory)/header.cpp$(ObjectSuffix): header.cpp $(IntermediateDirectory)/header.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/include/header.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/header.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/header.cpp$(DependSuffix): header.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/header.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/header.cpp$(DependSuffix) -MM header.cpp

$(IntermediateDirectory)/header.cpp$(PreprocessSuffix): header.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/header.cpp$(PreprocessSuffix) header.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/
	$(RM) Release/header.h.gch


