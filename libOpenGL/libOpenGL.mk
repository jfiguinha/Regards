##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libOpenGL
ConfigurationName      :=Debug
WorkspacePath          :=/home/figuinha/Developpement/Regards
ProjectPath            :=/home/figuinha/Developpement/Regards/libOpenGL
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
ObjectsFileList        :="libOpenGL.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2  $(IncludeSwitch). $(IncludeSwitch)../libFiltre $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libResource $(IncludeSwitch)../libOpenCL $(IncludeSwitch)../libextern/glew-2.1.0/include $(IncludeSwitch)../libUtility 
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
CXXFLAGS := -fopenmp -std=gnu++11 $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread  -ggdb -O0 $(Preprocessors)
CFLAGS   :=  -Wall  -ggdb -O0 $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/GLSLShader.cpp$(ObjectSuffix) $(IntermediateDirectory)/GLSLParameter.cpp$(ObjectSuffix) $(IntermediateDirectory)/GLTexture.cpp$(ObjectSuffix) $(IntermediateDirectory)/RenderOpenGL.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/home/figuinha/Developpement/Regards/.build-debug/libOpenGL"

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
$(IntermediateDirectory)/GLSLShader.cpp$(ObjectSuffix): GLSLShader.cpp $(IntermediateDirectory)/GLSLShader.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libOpenGL/GLSLShader.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GLSLShader.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GLSLShader.cpp$(DependSuffix): GLSLShader.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GLSLShader.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GLSLShader.cpp$(DependSuffix) -MM GLSLShader.cpp

$(IntermediateDirectory)/GLSLShader.cpp$(PreprocessSuffix): GLSLShader.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GLSLShader.cpp$(PreprocessSuffix) GLSLShader.cpp

$(IntermediateDirectory)/GLSLParameter.cpp$(ObjectSuffix): GLSLParameter.cpp $(IntermediateDirectory)/GLSLParameter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libOpenGL/GLSLParameter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GLSLParameter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GLSLParameter.cpp$(DependSuffix): GLSLParameter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GLSLParameter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GLSLParameter.cpp$(DependSuffix) -MM GLSLParameter.cpp

$(IntermediateDirectory)/GLSLParameter.cpp$(PreprocessSuffix): GLSLParameter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GLSLParameter.cpp$(PreprocessSuffix) GLSLParameter.cpp

$(IntermediateDirectory)/GLTexture.cpp$(ObjectSuffix): GLTexture.cpp $(IntermediateDirectory)/GLTexture.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libOpenGL/GLTexture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GLTexture.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GLTexture.cpp$(DependSuffix): GLTexture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GLTexture.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GLTexture.cpp$(DependSuffix) -MM GLTexture.cpp

$(IntermediateDirectory)/GLTexture.cpp$(PreprocessSuffix): GLTexture.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GLTexture.cpp$(PreprocessSuffix) GLTexture.cpp

$(IntermediateDirectory)/RenderOpenGL.cpp$(ObjectSuffix): RenderOpenGL.cpp $(IntermediateDirectory)/RenderOpenGL.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libOpenGL/RenderOpenGL.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RenderOpenGL.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RenderOpenGL.cpp$(DependSuffix): RenderOpenGL.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RenderOpenGL.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RenderOpenGL.cpp$(DependSuffix) -MM RenderOpenGL.cpp

$(IntermediateDirectory)/RenderOpenGL.cpp$(PreprocessSuffix): RenderOpenGL.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RenderOpenGL.cpp$(PreprocessSuffix) RenderOpenGL.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/
	$(RM) ../include/Debug/header.h.gch


