##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libIntro
ConfigurationName      :=Debug
WorkspacePath          :=/home/figuinha/Developpement/Regards
ProjectPath            :=/home/figuinha/Developpement/Regards/libIntro
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
ObjectsFileList        :="libIntro.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2  $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libTheme $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libParameter $(IncludeSwitch)../libUtility $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libResource $(IncludeSwitch)../libSqlData $(IncludeSwitch)../libOpenGL $(IncludeSwitch)../libRenderBitmap $(IncludeSwitch)../libSqlEngine $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libFiltre $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../Otherlib/wxSVG/include $(IncludeSwitch)../libWindow $(IncludeSwitch)../libPrint $(IncludeSwitch)../libPicture $(IncludeSwitch)../libTheme $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libControl $(IncludeSwitch)../libIntro $(IncludeSwitch)../libWindowMain 
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
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread  -fopenmp -std=gnu++11 -Wall -ggdb -O0 $(Preprocessors)
CFLAGS   :=  -Wall  -ggdb -O0 $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/MyCentralWindowIntro.cpp$(ObjectSuffix) $(IntermediateDirectory)/IntroTheme.cpp$(ObjectSuffix) $(IntermediateDirectory)/MyFrameIntro.cpp$(ObjectSuffix) $(IntermediateDirectory)/TitleIntro.cpp$(ObjectSuffix) $(IntermediateDirectory)/infoAbout.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/home/figuinha/Developpement/Regards/.build-debug/libIntro"

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
$(IntermediateDirectory)/MyCentralWindowIntro.cpp$(ObjectSuffix): MyCentralWindowIntro.cpp $(IntermediateDirectory)/MyCentralWindowIntro.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libIntro/MyCentralWindowIntro.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MyCentralWindowIntro.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MyCentralWindowIntro.cpp$(DependSuffix): MyCentralWindowIntro.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MyCentralWindowIntro.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MyCentralWindowIntro.cpp$(DependSuffix) -MM MyCentralWindowIntro.cpp

$(IntermediateDirectory)/MyCentralWindowIntro.cpp$(PreprocessSuffix): MyCentralWindowIntro.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MyCentralWindowIntro.cpp$(PreprocessSuffix) MyCentralWindowIntro.cpp

$(IntermediateDirectory)/IntroTheme.cpp$(ObjectSuffix): IntroTheme.cpp $(IntermediateDirectory)/IntroTheme.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libIntro/IntroTheme.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IntroTheme.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IntroTheme.cpp$(DependSuffix): IntroTheme.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IntroTheme.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IntroTheme.cpp$(DependSuffix) -MM IntroTheme.cpp

$(IntermediateDirectory)/IntroTheme.cpp$(PreprocessSuffix): IntroTheme.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IntroTheme.cpp$(PreprocessSuffix) IntroTheme.cpp

$(IntermediateDirectory)/MyFrameIntro.cpp$(ObjectSuffix): MyFrameIntro.cpp $(IntermediateDirectory)/MyFrameIntro.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libIntro/MyFrameIntro.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MyFrameIntro.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MyFrameIntro.cpp$(DependSuffix): MyFrameIntro.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MyFrameIntro.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MyFrameIntro.cpp$(DependSuffix) -MM MyFrameIntro.cpp

$(IntermediateDirectory)/MyFrameIntro.cpp$(PreprocessSuffix): MyFrameIntro.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MyFrameIntro.cpp$(PreprocessSuffix) MyFrameIntro.cpp

$(IntermediateDirectory)/TitleIntro.cpp$(ObjectSuffix): TitleIntro.cpp $(IntermediateDirectory)/TitleIntro.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libIntro/TitleIntro.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TitleIntro.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TitleIntro.cpp$(DependSuffix): TitleIntro.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TitleIntro.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TitleIntro.cpp$(DependSuffix) -MM TitleIntro.cpp

$(IntermediateDirectory)/TitleIntro.cpp$(PreprocessSuffix): TitleIntro.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TitleIntro.cpp$(PreprocessSuffix) TitleIntro.cpp

$(IntermediateDirectory)/infoAbout.cpp$(ObjectSuffix): infoAbout.cpp $(IntermediateDirectory)/infoAbout.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libIntro/infoAbout.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/infoAbout.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/infoAbout.cpp$(DependSuffix): infoAbout.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/infoAbout.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/infoAbout.cpp$(DependSuffix) -MM infoAbout.cpp

$(IntermediateDirectory)/infoAbout.cpp$(PreprocessSuffix): infoAbout.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/infoAbout.cpp$(PreprocessSuffix) infoAbout.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/
	$(RM) ../include/Debug/header.h.gch


