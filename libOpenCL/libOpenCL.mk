##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libOpenCL
ConfigurationName      :=Release
WorkspacePath          :=/home/figuinha/developpement/Regards
ProjectPath            :=/home/figuinha/developpement/Regards/libOpenCL
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
OutputFile             :=$(IntermediateDirectory)/libRegardsCL.a
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libOpenCL.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s
IncludePath            := $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2 $(IncludeSwitch)/usr/include/libexif  $(IncludeSwitch). $(IncludeSwitch)../libFiltre $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../libUtility $(IncludeSwitch)../libResource $(IncludeSwitch)../libParameter $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../include $(IncludeSwitch)../CxImage $(IncludeSwitch)../libUtility $(IncludeSwitch)../libSqlData $(IncludeSwitch)../libSqlEngine 
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
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread  -fopenmp -std=gnu++11 -Wall -O2 -msse4.1 $(Preprocessors)
CFLAGS   := -Wall  -O2 -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/yasm


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/OpenCLInfos.cpp$(ObjectSuffix) $(IntermediateDirectory)/OpenCLExecuteProgram.cpp$(ObjectSuffix) $(IntermediateDirectory)/OpenCLEngine.cpp$(ObjectSuffix) $(IntermediateDirectory)/OpenCLBm3D.cpp$(ObjectSuffix) $(IntermediateDirectory)/OpenCLContext.cpp$(ObjectSuffix) $(IntermediateDirectory)/OpenCLEffect.cpp$(ObjectSuffix) $(IntermediateDirectory)/OpenCLEffectVideoNV12.cpp$(ObjectSuffix) $(IntermediateDirectory)/OpenCLParameter.cpp$(ObjectSuffix) $(IntermediateDirectory)/OpenCLProgram.cpp$(ObjectSuffix) $(IntermediateDirectory)/OpenCLEffectVideoYUV.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/OpenCLRawDev.cpp$(ObjectSuffix) $(IntermediateDirectory)/utility.cpp$(ObjectSuffix) $(IntermediateDirectory)/OpenCLEffectVideo.cpp$(ObjectSuffix) $(IntermediateDirectory)/OpenCLFilter.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/home/figuinha/developpement/Regards/.build-release/libOpenCL"

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
$(IntermediateDirectory)/OpenCLInfos.cpp$(ObjectSuffix): OpenCLInfos.cpp $(IntermediateDirectory)/OpenCLInfos.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libOpenCL/OpenCLInfos.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/OpenCLInfos.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OpenCLInfos.cpp$(DependSuffix): OpenCLInfos.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/OpenCLInfos.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/OpenCLInfos.cpp$(DependSuffix) -MM OpenCLInfos.cpp

$(IntermediateDirectory)/OpenCLInfos.cpp$(PreprocessSuffix): OpenCLInfos.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OpenCLInfos.cpp$(PreprocessSuffix) OpenCLInfos.cpp

$(IntermediateDirectory)/OpenCLExecuteProgram.cpp$(ObjectSuffix): OpenCLExecuteProgram.cpp $(IntermediateDirectory)/OpenCLExecuteProgram.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libOpenCL/OpenCLExecuteProgram.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/OpenCLExecuteProgram.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OpenCLExecuteProgram.cpp$(DependSuffix): OpenCLExecuteProgram.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/OpenCLExecuteProgram.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/OpenCLExecuteProgram.cpp$(DependSuffix) -MM OpenCLExecuteProgram.cpp

$(IntermediateDirectory)/OpenCLExecuteProgram.cpp$(PreprocessSuffix): OpenCLExecuteProgram.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OpenCLExecuteProgram.cpp$(PreprocessSuffix) OpenCLExecuteProgram.cpp

$(IntermediateDirectory)/OpenCLEngine.cpp$(ObjectSuffix): OpenCLEngine.cpp $(IntermediateDirectory)/OpenCLEngine.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libOpenCL/OpenCLEngine.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/OpenCLEngine.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OpenCLEngine.cpp$(DependSuffix): OpenCLEngine.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/OpenCLEngine.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/OpenCLEngine.cpp$(DependSuffix) -MM OpenCLEngine.cpp

$(IntermediateDirectory)/OpenCLEngine.cpp$(PreprocessSuffix): OpenCLEngine.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OpenCLEngine.cpp$(PreprocessSuffix) OpenCLEngine.cpp

$(IntermediateDirectory)/OpenCLBm3D.cpp$(ObjectSuffix): OpenCLBm3D.cpp $(IntermediateDirectory)/OpenCLBm3D.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libOpenCL/OpenCLBm3D.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/OpenCLBm3D.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OpenCLBm3D.cpp$(DependSuffix): OpenCLBm3D.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/OpenCLBm3D.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/OpenCLBm3D.cpp$(DependSuffix) -MM OpenCLBm3D.cpp

$(IntermediateDirectory)/OpenCLBm3D.cpp$(PreprocessSuffix): OpenCLBm3D.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OpenCLBm3D.cpp$(PreprocessSuffix) OpenCLBm3D.cpp

$(IntermediateDirectory)/OpenCLContext.cpp$(ObjectSuffix): OpenCLContext.cpp $(IntermediateDirectory)/OpenCLContext.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libOpenCL/OpenCLContext.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/OpenCLContext.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OpenCLContext.cpp$(DependSuffix): OpenCLContext.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/OpenCLContext.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/OpenCLContext.cpp$(DependSuffix) -MM OpenCLContext.cpp

$(IntermediateDirectory)/OpenCLContext.cpp$(PreprocessSuffix): OpenCLContext.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OpenCLContext.cpp$(PreprocessSuffix) OpenCLContext.cpp

$(IntermediateDirectory)/OpenCLEffect.cpp$(ObjectSuffix): OpenCLEffect.cpp $(IntermediateDirectory)/OpenCLEffect.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libOpenCL/OpenCLEffect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/OpenCLEffect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OpenCLEffect.cpp$(DependSuffix): OpenCLEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/OpenCLEffect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/OpenCLEffect.cpp$(DependSuffix) -MM OpenCLEffect.cpp

$(IntermediateDirectory)/OpenCLEffect.cpp$(PreprocessSuffix): OpenCLEffect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OpenCLEffect.cpp$(PreprocessSuffix) OpenCLEffect.cpp

$(IntermediateDirectory)/OpenCLEffectVideoNV12.cpp$(ObjectSuffix): OpenCLEffectVideoNV12.cpp $(IntermediateDirectory)/OpenCLEffectVideoNV12.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libOpenCL/OpenCLEffectVideoNV12.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/OpenCLEffectVideoNV12.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OpenCLEffectVideoNV12.cpp$(DependSuffix): OpenCLEffectVideoNV12.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/OpenCLEffectVideoNV12.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/OpenCLEffectVideoNV12.cpp$(DependSuffix) -MM OpenCLEffectVideoNV12.cpp

$(IntermediateDirectory)/OpenCLEffectVideoNV12.cpp$(PreprocessSuffix): OpenCLEffectVideoNV12.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OpenCLEffectVideoNV12.cpp$(PreprocessSuffix) OpenCLEffectVideoNV12.cpp

$(IntermediateDirectory)/OpenCLParameter.cpp$(ObjectSuffix): OpenCLParameter.cpp $(IntermediateDirectory)/OpenCLParameter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libOpenCL/OpenCLParameter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/OpenCLParameter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OpenCLParameter.cpp$(DependSuffix): OpenCLParameter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/OpenCLParameter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/OpenCLParameter.cpp$(DependSuffix) -MM OpenCLParameter.cpp

$(IntermediateDirectory)/OpenCLParameter.cpp$(PreprocessSuffix): OpenCLParameter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OpenCLParameter.cpp$(PreprocessSuffix) OpenCLParameter.cpp

$(IntermediateDirectory)/OpenCLProgram.cpp$(ObjectSuffix): OpenCLProgram.cpp $(IntermediateDirectory)/OpenCLProgram.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libOpenCL/OpenCLProgram.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/OpenCLProgram.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OpenCLProgram.cpp$(DependSuffix): OpenCLProgram.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/OpenCLProgram.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/OpenCLProgram.cpp$(DependSuffix) -MM OpenCLProgram.cpp

$(IntermediateDirectory)/OpenCLProgram.cpp$(PreprocessSuffix): OpenCLProgram.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OpenCLProgram.cpp$(PreprocessSuffix) OpenCLProgram.cpp

$(IntermediateDirectory)/OpenCLEffectVideoYUV.cpp$(ObjectSuffix): OpenCLEffectVideoYUV.cpp $(IntermediateDirectory)/OpenCLEffectVideoYUV.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libOpenCL/OpenCLEffectVideoYUV.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/OpenCLEffectVideoYUV.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OpenCLEffectVideoYUV.cpp$(DependSuffix): OpenCLEffectVideoYUV.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/OpenCLEffectVideoYUV.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/OpenCLEffectVideoYUV.cpp$(DependSuffix) -MM OpenCLEffectVideoYUV.cpp

$(IntermediateDirectory)/OpenCLEffectVideoYUV.cpp$(PreprocessSuffix): OpenCLEffectVideoYUV.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OpenCLEffectVideoYUV.cpp$(PreprocessSuffix) OpenCLEffectVideoYUV.cpp

$(IntermediateDirectory)/OpenCLRawDev.cpp$(ObjectSuffix): OpenCLRawDev.cpp $(IntermediateDirectory)/OpenCLRawDev.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libOpenCL/OpenCLRawDev.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/OpenCLRawDev.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OpenCLRawDev.cpp$(DependSuffix): OpenCLRawDev.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/OpenCLRawDev.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/OpenCLRawDev.cpp$(DependSuffix) -MM OpenCLRawDev.cpp

$(IntermediateDirectory)/OpenCLRawDev.cpp$(PreprocessSuffix): OpenCLRawDev.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OpenCLRawDev.cpp$(PreprocessSuffix) OpenCLRawDev.cpp

$(IntermediateDirectory)/utility.cpp$(ObjectSuffix): utility.cpp $(IntermediateDirectory)/utility.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libOpenCL/utility.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/utility.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/utility.cpp$(DependSuffix): utility.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/utility.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/utility.cpp$(DependSuffix) -MM utility.cpp

$(IntermediateDirectory)/utility.cpp$(PreprocessSuffix): utility.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/utility.cpp$(PreprocessSuffix) utility.cpp

$(IntermediateDirectory)/OpenCLEffectVideo.cpp$(ObjectSuffix): OpenCLEffectVideo.cpp $(IntermediateDirectory)/OpenCLEffectVideo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libOpenCL/OpenCLEffectVideo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/OpenCLEffectVideo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OpenCLEffectVideo.cpp$(DependSuffix): OpenCLEffectVideo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/OpenCLEffectVideo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/OpenCLEffectVideo.cpp$(DependSuffix) -MM OpenCLEffectVideo.cpp

$(IntermediateDirectory)/OpenCLEffectVideo.cpp$(PreprocessSuffix): OpenCLEffectVideo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OpenCLEffectVideo.cpp$(PreprocessSuffix) OpenCLEffectVideo.cpp

$(IntermediateDirectory)/OpenCLFilter.cpp$(ObjectSuffix): OpenCLFilter.cpp $(IntermediateDirectory)/OpenCLFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libOpenCL/OpenCLFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/OpenCLFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OpenCLFilter.cpp$(DependSuffix): OpenCLFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/OpenCLFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/OpenCLFilter.cpp$(DependSuffix) -MM OpenCLFilter.cpp

$(IntermediateDirectory)/OpenCLFilter.cpp$(PreprocessSuffix): OpenCLFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OpenCLFilter.cpp$(PreprocessSuffix) OpenCLFilter.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/
	$(RM) ../include/Release/header.h.gch


