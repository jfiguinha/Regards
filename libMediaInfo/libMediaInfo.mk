##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libMediaInfo
ConfigurationName      :=Release
WorkspacePath          :=/home/figuinha/Developpement/Regards
ProjectPath            :=/home/figuinha/Developpement/Regards/libMediaInfo
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=15/01/19
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
OutputFile             :=$(IntermediateDirectory)/libRegardsMediaInfo.a
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libMediaInfo.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2  $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libextern/MediaInfo_DLL_GNU_FromSource/MediaInfoLib/Source $(IncludeSwitch)../libextern/MediaInfo_DLL_GNU_FromSource/ZenLib/Source $(IncludeSwitch)../libUtility $(IncludeSwitch). 
IncludePCH             :=  -include ../include/Release/header.h 
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
CXXFLAGS := -std=gnu++11 $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread -fopenmp -O2 -msse4.1 $(Preprocessors)
CFLAGS   :=  -Wall  -O2 -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/MediaInfo.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "/home/figuinha/Developpement/Regards/.build-release"
	@echo rebuilt > "/home/figuinha/Developpement/Regards/.build-release/libMediaInfo"

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
$(IntermediateDirectory)/MediaInfo.cpp$(ObjectSuffix): MediaInfo.cpp $(IntermediateDirectory)/MediaInfo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libMediaInfo/MediaInfo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MediaInfo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MediaInfo.cpp$(DependSuffix): MediaInfo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MediaInfo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MediaInfo.cpp$(DependSuffix) -MM MediaInfo.cpp

$(IntermediateDirectory)/MediaInfo.cpp$(PreprocessSuffix): MediaInfo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MediaInfo.cpp$(PreprocessSuffix) MediaInfo.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/
	$(RM) ../include/Release/header.h.gch


