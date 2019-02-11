##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libOpenGL
ConfigurationName      :=Release
WorkspacePath          :=C:/Regards
ProjectPath            :=C:/Regards/libOpenGL
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=jfigu_000
Date                   :=11/02/2019
CodeLitePath           :="C:/Program Files/CodeLite"
LinkerName             :=C:/msys64/usr/bin/ccache.exe C:/msys64/mingw64/bin/g++.exe
SharedObjectLinkerName :=C:/msys64/mingw64/bin/g++.exe -shared -fPIC
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
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/msys64/mingw64/bin/windres.exe
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)"C:\Program Files (x86)\AMD APP SDK\3.0_mingw\include" $(IncludeSwitch)/mingw64/include/libxml2 $(IncludeSwitch)/mingw64/include/OpenEXR  $(IncludeSwitch). $(IncludeSwitch)../libFiltre $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libResource $(IncludeSwitch)../libOpenCL $(IncludeSwitch)../libextern/glew-2.1.0/include $(IncludeSwitch)../libUtility $(IncludeSwitch)../libextern/OpenCL-Headers 
IncludePCH             :=  -include ../include/Release/header.h 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                :=$(LibraryPathSwitch)"C:\Program Files (x86)\AMD APP SDK\3.0_mingw\lib\x86_64"  $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/msys64/mingw64/bin/ar.exe rcu
CXX      := C:/msys64/usr/bin/ccache.exe C:/msys64/mingw64/bin/g++.exe
CC       := C:/msys64/usr/bin/ccache.exe  C:/msys64/mingw64/bin/gcc.exe
CXXFLAGS := -fopenmp -std=gnu++11 $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread  -O2 -msse4.1 $(Preprocessors)
CFLAGS   :=  -Wall  -O2 -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := C:/msys64/mingw64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
PATH:=C:\msys64\usr\bin;$PATH
WXWIN:=C:\Regards\libextern\wxwidgets-3.1.2
WXCFG:=lib/mswud
Objects0=$(IntermediateDirectory)/GLSLShader.cpp$(ObjectSuffix) $(IntermediateDirectory)/GLSLParameter.cpp$(ObjectSuffix) $(IntermediateDirectory)/RenderOpenGL.cpp$(ObjectSuffix) $(IntermediateDirectory)/GLTexture.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "C:\Regards/.build-release"
	@echo rebuilt > "C:\Regards/.build-release/libOpenGL"

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Release"


./Release:
	@$(MakeDirCommand) "./Release"

PreBuild:

# PreCompiled Header
../include/Release/header.h.gch: ../include/Release/header.h
	$(CXX) $(SourceSwitch) ../include/Release/header.h $(PCHCompileFlags) $(CXXFLAGS) $(IncludePath)



##
## Objects
##
$(IntermediateDirectory)/GLSLShader.cpp$(ObjectSuffix): GLSLShader.cpp $(IntermediateDirectory)/GLSLShader.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/libOpenGL/GLSLShader.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GLSLShader.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GLSLShader.cpp$(DependSuffix): GLSLShader.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GLSLShader.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GLSLShader.cpp$(DependSuffix) -MM GLSLShader.cpp

$(IntermediateDirectory)/GLSLShader.cpp$(PreprocessSuffix): GLSLShader.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GLSLShader.cpp$(PreprocessSuffix) GLSLShader.cpp

$(IntermediateDirectory)/GLSLParameter.cpp$(ObjectSuffix): GLSLParameter.cpp $(IntermediateDirectory)/GLSLParameter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/libOpenGL/GLSLParameter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GLSLParameter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GLSLParameter.cpp$(DependSuffix): GLSLParameter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GLSLParameter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GLSLParameter.cpp$(DependSuffix) -MM GLSLParameter.cpp

$(IntermediateDirectory)/GLSLParameter.cpp$(PreprocessSuffix): GLSLParameter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GLSLParameter.cpp$(PreprocessSuffix) GLSLParameter.cpp

$(IntermediateDirectory)/RenderOpenGL.cpp$(ObjectSuffix): RenderOpenGL.cpp $(IntermediateDirectory)/RenderOpenGL.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/libOpenGL/RenderOpenGL.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RenderOpenGL.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RenderOpenGL.cpp$(DependSuffix): RenderOpenGL.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RenderOpenGL.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RenderOpenGL.cpp$(DependSuffix) -MM RenderOpenGL.cpp

$(IntermediateDirectory)/RenderOpenGL.cpp$(PreprocessSuffix): RenderOpenGL.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RenderOpenGL.cpp$(PreprocessSuffix) RenderOpenGL.cpp

$(IntermediateDirectory)/GLTexture.cpp$(ObjectSuffix): GLTexture.cpp $(IntermediateDirectory)/GLTexture.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/libOpenGL/GLTexture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GLTexture.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GLTexture.cpp$(DependSuffix): GLTexture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GLTexture.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GLTexture.cpp$(DependSuffix) -MM GLTexture.cpp

$(IntermediateDirectory)/GLTexture.cpp$(PreprocessSuffix): GLTexture.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GLTexture.cpp$(PreprocessSuffix) GLTexture.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/
	$(RM) ../include/Release/header.h.gch


