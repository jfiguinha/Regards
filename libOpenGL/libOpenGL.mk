##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libOpenGL
ConfigurationName      :=Release
WorkspacePath          :=/home/figuinha/developpement/Regards
ProjectPath            :=/home/figuinha/developpement/Regards/libOpenGL
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
ObjectsFileList        :="libOpenGL.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2 $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include  $(IncludeSwitch). $(IncludeSwitch)../libFiltre $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libResource $(IncludeSwitch)../libOpenCL $(IncludeSwitch)../libextern/glew-2.1.0/include $(IncludeSwitch)../libUtility $(IncludeSwitch)../libextern/OpenCL-Headers 
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
CXXFLAGS := -fopenmp -std=gnu++11 $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread  -O2 -msse4.1 $(Preprocessors)
CFLAGS   :=  -Wall  -O2 -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := ccache /usr/bin/x86_64-linux-gnu-as


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
	@$(MakeDirCommand) "/home/figuinha/developpement/Regards/.build-release"
	@echo rebuilt > "/home/figuinha/developpement/Regards/.build-release/libOpenGL"

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
$(IntermediateDirectory)/GLSLShader.cpp$(ObjectSuffix): GLSLShader.cpp $(IntermediateDirectory)/GLSLShader.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libOpenGL/GLSLShader.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GLSLShader.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GLSLShader.cpp$(DependSuffix): GLSLShader.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GLSLShader.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GLSLShader.cpp$(DependSuffix) -MM GLSLShader.cpp

$(IntermediateDirectory)/GLSLShader.cpp$(PreprocessSuffix): GLSLShader.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GLSLShader.cpp$(PreprocessSuffix) GLSLShader.cpp

$(IntermediateDirectory)/GLSLParameter.cpp$(ObjectSuffix): GLSLParameter.cpp $(IntermediateDirectory)/GLSLParameter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libOpenGL/GLSLParameter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GLSLParameter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GLSLParameter.cpp$(DependSuffix): GLSLParameter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GLSLParameter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GLSLParameter.cpp$(DependSuffix) -MM GLSLParameter.cpp

$(IntermediateDirectory)/GLSLParameter.cpp$(PreprocessSuffix): GLSLParameter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GLSLParameter.cpp$(PreprocessSuffix) GLSLParameter.cpp

$(IntermediateDirectory)/GLTexture.cpp$(ObjectSuffix): GLTexture.cpp $(IntermediateDirectory)/GLTexture.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libOpenGL/GLTexture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GLTexture.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GLTexture.cpp$(DependSuffix): GLTexture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GLTexture.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GLTexture.cpp$(DependSuffix) -MM GLTexture.cpp

$(IntermediateDirectory)/GLTexture.cpp$(PreprocessSuffix): GLTexture.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GLTexture.cpp$(PreprocessSuffix) GLTexture.cpp

$(IntermediateDirectory)/RenderOpenGL.cpp$(ObjectSuffix): RenderOpenGL.cpp $(IntermediateDirectory)/RenderOpenGL.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libOpenGL/RenderOpenGL.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RenderOpenGL.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RenderOpenGL.cpp$(DependSuffix): RenderOpenGL.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RenderOpenGL.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RenderOpenGL.cpp$(DependSuffix) -MM RenderOpenGL.cpp

$(IntermediateDirectory)/RenderOpenGL.cpp$(PreprocessSuffix): RenderOpenGL.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RenderOpenGL.cpp$(PreprocessSuffix) RenderOpenGL.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/
	$(RM) ../include/Release/header.h.gch


