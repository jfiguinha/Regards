##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libOpenGL
ConfigurationName      :=Debug
WorkspacePath          := "/home/figuinha/dev/Regards"
ProjectPath            := "/home/figuinha/dev/Regards/libOpenGL"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=23/04/16
CodeLitePath           :="/home/figuinha/.codelite"
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
OutputFile             :=$(IntermediateDirectory)/$(ProjectName).a
Preprocessors          :=$(PreprocessorSwitch)__WXGTK__ 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libOpenGL.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../libUtility $(IncludeSwitch)../libextern/exiv2-0.24/src $(IncludeSwitch)../libextern/exiv2-0.24/xmpsdk/include $(IncludeSwitch)../libextern/glew-1.13.0/include $(IncludeSwitch)../libResource $(IncludeSwitch)../libextern/wxWidgets-3.0.2/include $(IncludeSwitch)../libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -O0 -fopenmp -std=c++11 -Wall  $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall  $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/GLSLParameter.cpp$(ObjectSuffix) $(IntermediateDirectory)/GLSLShader.cpp$(ObjectSuffix) $(IntermediateDirectory)/GLTexture.cpp$(ObjectSuffix) $(IntermediateDirectory)/GLTextureRgbWithAlpha.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "/home/figuinha/dev/Regards/.build-debug"
	@echo rebuilt > "/home/figuinha/dev/Regards/.build-debug/libOpenGL"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/GLSLParameter.cpp$(ObjectSuffix): GLSLParameter.cpp $(IntermediateDirectory)/GLSLParameter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libOpenGL/GLSLParameter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GLSLParameter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GLSLParameter.cpp$(DependSuffix): GLSLParameter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GLSLParameter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GLSLParameter.cpp$(DependSuffix) -MM "GLSLParameter.cpp"

$(IntermediateDirectory)/GLSLParameter.cpp$(PreprocessSuffix): GLSLParameter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GLSLParameter.cpp$(PreprocessSuffix) "GLSLParameter.cpp"

$(IntermediateDirectory)/GLSLShader.cpp$(ObjectSuffix): GLSLShader.cpp $(IntermediateDirectory)/GLSLShader.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libOpenGL/GLSLShader.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GLSLShader.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GLSLShader.cpp$(DependSuffix): GLSLShader.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GLSLShader.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GLSLShader.cpp$(DependSuffix) -MM "GLSLShader.cpp"

$(IntermediateDirectory)/GLSLShader.cpp$(PreprocessSuffix): GLSLShader.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GLSLShader.cpp$(PreprocessSuffix) "GLSLShader.cpp"

$(IntermediateDirectory)/GLTexture.cpp$(ObjectSuffix): GLTexture.cpp $(IntermediateDirectory)/GLTexture.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libOpenGL/GLTexture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GLTexture.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GLTexture.cpp$(DependSuffix): GLTexture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GLTexture.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GLTexture.cpp$(DependSuffix) -MM "GLTexture.cpp"

$(IntermediateDirectory)/GLTexture.cpp$(PreprocessSuffix): GLTexture.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GLTexture.cpp$(PreprocessSuffix) "GLTexture.cpp"

$(IntermediateDirectory)/GLTextureRgbWithAlpha.cpp$(ObjectSuffix): GLTextureRgbWithAlpha.cpp $(IntermediateDirectory)/GLTextureRgbWithAlpha.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libOpenGL/GLTextureRgbWithAlpha.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GLTextureRgbWithAlpha.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GLTextureRgbWithAlpha.cpp$(DependSuffix): GLTextureRgbWithAlpha.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GLTextureRgbWithAlpha.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GLTextureRgbWithAlpha.cpp$(DependSuffix) -MM "GLTextureRgbWithAlpha.cpp"

$(IntermediateDirectory)/GLTextureRgbWithAlpha.cpp$(PreprocessSuffix): GLTextureRgbWithAlpha.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GLTextureRgbWithAlpha.cpp$(PreprocessSuffix) "GLTextureRgbWithAlpha.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


