##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libSqlite
ConfigurationName      :=Release
WorkspacePath          :=/home/figuinha/developpement/Regards
ProjectPath            :=/home/figuinha/developpement/Regards/libSqlite
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=14/02/19
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
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libSqlite.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s 
IncludePath            := $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2 $(IncludeSwitch)/usr/include/libexif  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
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
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread -O2 -msse4.1 $(Preprocessors)
CFLAGS   := $(shell ../libextern/wxWidgets-master/wx-config --cflags) -O2 -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/yasm


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/sqlite3.c$(ObjectSuffix) 



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
	@echo rebuilt > "/home/figuinha/developpement/Regards/.build-release/libSqlite"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


./Release:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/sqlite3.c$(ObjectSuffix): sqlite3.c $(IntermediateDirectory)/sqlite3.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlite/sqlite3.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sqlite3.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sqlite3.c$(DependSuffix): sqlite3.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sqlite3.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sqlite3.c$(DependSuffix) -MM sqlite3.c

$(IntermediateDirectory)/sqlite3.c$(PreprocessSuffix): sqlite3.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sqlite3.c$(PreprocessSuffix) sqlite3.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


