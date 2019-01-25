##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libCompress
ConfigurationName      :=Release
WorkspacePath          :=/home/figuinha/developpement/Regards
ProjectPath            :=/home/figuinha/developpement/Regards/libCompress
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
ObjectsFileList        :="libCompress.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s 
IncludePath            := $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2 $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include  $(IncludeSwitch). $(IncludeSwitch)../libCompress 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                :=$(LibraryPathSwitch)/opt/AMDAPPSDK-3.0/lib/x86_64/sdk  $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/x86_64-linux-gnu-ar rcu
CXX      := ccache /usr/bin/x86_64-linux-gnu-g++
CC       := ccache /usr/bin/x86_64-linux-gnu-gcc
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread -fopenmp -std=gnu++11 -fpermissive -O2 -msse4.1 $(Preprocessors)
CFLAGS   := $(shell ../libextern/wxWidgets-master/wx-config --cflags) -Wall  -O2 -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := ccache /usr/bin/x86_64-linux-gnu-as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/lz4.c$(ObjectSuffix) $(IntermediateDirectory)/jpgd.cpp$(ObjectSuffix) $(IntermediateDirectory)/jpge.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/home/figuinha/developpement/Regards/.build-release/libCompress"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


./Release:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/lz4.c$(ObjectSuffix): lz4.c $(IntermediateDirectory)/lz4.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/libCompress/lz4.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lz4.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lz4.c$(DependSuffix): lz4.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lz4.c$(ObjectSuffix) -MF$(IntermediateDirectory)/lz4.c$(DependSuffix) -MM lz4.c

$(IntermediateDirectory)/lz4.c$(PreprocessSuffix): lz4.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lz4.c$(PreprocessSuffix) lz4.c

$(IntermediateDirectory)/jpgd.cpp$(ObjectSuffix): jpgd.cpp $(IntermediateDirectory)/jpgd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libCompress/jpgd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/jpgd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/jpgd.cpp$(DependSuffix): jpgd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/jpgd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/jpgd.cpp$(DependSuffix) -MM jpgd.cpp

$(IntermediateDirectory)/jpgd.cpp$(PreprocessSuffix): jpgd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/jpgd.cpp$(PreprocessSuffix) jpgd.cpp

$(IntermediateDirectory)/jpge.cpp$(ObjectSuffix): jpge.cpp $(IntermediateDirectory)/jpge.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libCompress/jpge.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/jpge.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/jpge.cpp$(DependSuffix): jpge.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/jpge.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/jpge.cpp$(DependSuffix) -MM jpge.cpp

$(IntermediateDirectory)/jpge.cpp$(PreprocessSuffix): jpge.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/jpge.cpp$(PreprocessSuffix) jpge.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


