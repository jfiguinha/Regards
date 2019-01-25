##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libResource
ConfigurationName      :=Release
WorkspacePath          :=/home/figuinha/developpement/Regards
ProjectPath            :=/home/figuinha/developpement/Regards/libResource
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
ObjectsFileList        :="libResource.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s 
IncludePath            := $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2 $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include  $(IncludeSwitch)../libPicture $(IncludeSwitch). $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libParameter $(IncludeSwitch)../libFiltre $(IncludeSwitch)../libOpenGL $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../libUtility $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libResource $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libSqlEngine $(IncludeSwitch)../libCompress 
IncludePCH             :=  -include ../include/Release/header.h 
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
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread  -fopenmp -std=gnu++11 -Wall -Wno-narrowing -O2 -msse4.1 $(Preprocessors)
CFLAGS   :=  -Wall  -O2 -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := ccache /usr/bin/x86_64-linux-gnu-as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/LibResource.cpp$(ObjectSuffix) $(IntermediateDirectory)/ClosedHandCursor.cpp$(ObjectSuffix) $(IntermediateDirectory)/PictureData.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlLibResource.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlResource.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/home/figuinha/developpement/Regards/.build-release/libResource"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


./Release:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:
	@echo Executing Pre Build commands ...
	
	@echo Done

# PreCompiled Header
../include/Release/header.h.gch: ../include/Release/header.h
	$(CXX) $(SourceSwitch) ../include/Release/header.h $(PCHCompileFlags) $(CXXFLAGS) $(IncludePath)



##
## Objects
##
$(IntermediateDirectory)/LibResource.cpp$(ObjectSuffix): LibResource.cpp $(IntermediateDirectory)/LibResource.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libResource/LibResource.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LibResource.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LibResource.cpp$(DependSuffix): LibResource.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LibResource.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LibResource.cpp$(DependSuffix) -MM LibResource.cpp

$(IntermediateDirectory)/LibResource.cpp$(PreprocessSuffix): LibResource.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LibResource.cpp$(PreprocessSuffix) LibResource.cpp

$(IntermediateDirectory)/ClosedHandCursor.cpp$(ObjectSuffix): ClosedHandCursor.cpp $(IntermediateDirectory)/ClosedHandCursor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libResource/ClosedHandCursor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ClosedHandCursor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ClosedHandCursor.cpp$(DependSuffix): ClosedHandCursor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ClosedHandCursor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ClosedHandCursor.cpp$(DependSuffix) -MM ClosedHandCursor.cpp

$(IntermediateDirectory)/ClosedHandCursor.cpp$(PreprocessSuffix): ClosedHandCursor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ClosedHandCursor.cpp$(PreprocessSuffix) ClosedHandCursor.cpp

$(IntermediateDirectory)/PictureData.cpp$(ObjectSuffix): PictureData.cpp $(IntermediateDirectory)/PictureData.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libResource/PictureData.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PictureData.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PictureData.cpp$(DependSuffix): PictureData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PictureData.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PictureData.cpp$(DependSuffix) -MM PictureData.cpp

$(IntermediateDirectory)/PictureData.cpp$(PreprocessSuffix): PictureData.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PictureData.cpp$(PreprocessSuffix) PictureData.cpp

$(IntermediateDirectory)/SqlLibResource.cpp$(ObjectSuffix): SqlLibResource.cpp $(IntermediateDirectory)/SqlLibResource.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libResource/SqlLibResource.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlLibResource.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlLibResource.cpp$(DependSuffix): SqlLibResource.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlLibResource.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlLibResource.cpp$(DependSuffix) -MM SqlLibResource.cpp

$(IntermediateDirectory)/SqlLibResource.cpp$(PreprocessSuffix): SqlLibResource.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlLibResource.cpp$(PreprocessSuffix) SqlLibResource.cpp

$(IntermediateDirectory)/SqlResource.cpp$(ObjectSuffix): SqlResource.cpp $(IntermediateDirectory)/SqlResource.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libResource/SqlResource.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlResource.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlResource.cpp$(DependSuffix): SqlResource.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlResource.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlResource.cpp$(DependSuffix) -MM SqlResource.cpp

$(IntermediateDirectory)/SqlResource.cpp$(PreprocessSuffix): SqlResource.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlResource.cpp$(PreprocessSuffix) SqlResource.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/
	$(RM) ../include/Release/header.h.gch


