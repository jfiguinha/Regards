##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=RegardsSelectApplication
ConfigurationName      :=Release
WorkspacePath          :=/home/figuinha/developpement/git/Regards
ProjectPath            :=/home/figuinha/developpement/git/Regards/RegardsSelectApplication
IntermediateDirectory  :=$(ConfigurationName)
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=08/04/21
CodeLitePath           :=/home/figuinha/.codelite
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
OutputFile             :=$(IntermediateDirectory)/RegardsViewer
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="RegardsSelectApplication.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            := -Wl,--allow-multiple-definition  
IncludePath            := $(IncludeSwitch)$${HOME}/ffmpeg_build/include $(IncludeSwitch)$${HOME}/ffmpeg_build/include/poppler $(IncludeSwitch)$${HOME}/ffmpeg_build/include/poppler/cpp $(IncludeSwitch)$${HOME}/ffmpeg_build/include/opencv4 $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include $(IncludeSwitch)/usr/include/harfbuzz  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                :=$(LibraryPathSwitch)$${HOME}/ffmpeg_build/lib  $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS := -fopenmp -std=gnu++14 -Wall -flto -O2 -Wall $(Preprocessors)
CFLAGS   :=  -O2 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/RegardsSelectApplication.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d $(ConfigurationName) || $(MakeDirCommand) $(ConfigurationName)


$(IntermediateDirectory)/.d:
	@test -d $(ConfigurationName) || $(MakeDirCommand) $(ConfigurationName)

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/RegardsSelectApplication.cpp$(ObjectSuffix): RegardsSelectApplication.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RegardsSelectApplication.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RegardsSelectApplication.cpp$(DependSuffix) -MM RegardsSelectApplication.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/RegardsSelectApplication/RegardsSelectApplication.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RegardsSelectApplication.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RegardsSelectApplication.cpp$(PreprocessSuffix): RegardsSelectApplication.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RegardsSelectApplication.cpp$(PreprocessSuffix) RegardsSelectApplication.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(ConfigurationName)/


