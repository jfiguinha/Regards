##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libRenderBitmap
ConfigurationName      :=Release
WorkspacePath          := "/home/figuinha/dev/Regards"
ProjectPath            := "/home/figuinha/dev/Regards/libRenderBitmap"
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=24/04/16
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
Preprocessors          :=$(PreprocessorSwitch)NDEBUG $(PreprocessorSwitch)__WXGTK__ 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libRenderBitmap.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s 
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libParameter $(IncludeSwitch)../libFiltre $(IncludeSwitch)../libOpenGL $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../libUtility $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libResource $(IncludeSwitch)../libextern/glew-1.13.0/include $(IncludeSwitch)../libextern/wxWidgets-3.0.2/include $(IncludeSwitch)../libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0 
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
CXXFLAGS :=  -fopenmp -std=c++11 -Wall  $(Preprocessors)
CFLAGS   :=  -O2 -Wall  $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/RenderBitmapInterface.cpp$(ObjectSuffix) $(IntermediateDirectory)/RenderBitmapInterfaceGDI.cpp$(ObjectSuffix) $(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "/home/figuinha/dev/Regards/.build-release"
	@echo rebuilt > "/home/figuinha/dev/Regards/.build-release/libRenderBitmap"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


./Release:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/RenderBitmapInterface.cpp$(ObjectSuffix): RenderBitmapInterface.cpp $(IntermediateDirectory)/RenderBitmapInterface.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libRenderBitmap/RenderBitmapInterface.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RenderBitmapInterface.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RenderBitmapInterface.cpp$(DependSuffix): RenderBitmapInterface.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RenderBitmapInterface.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RenderBitmapInterface.cpp$(DependSuffix) -MM "RenderBitmapInterface.cpp"

$(IntermediateDirectory)/RenderBitmapInterface.cpp$(PreprocessSuffix): RenderBitmapInterface.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RenderBitmapInterface.cpp$(PreprocessSuffix) "RenderBitmapInterface.cpp"

$(IntermediateDirectory)/RenderBitmapInterfaceGDI.cpp$(ObjectSuffix): RenderBitmapInterfaceGDI.cpp $(IntermediateDirectory)/RenderBitmapInterfaceGDI.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libRenderBitmap/RenderBitmapInterfaceGDI.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RenderBitmapInterfaceGDI.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RenderBitmapInterfaceGDI.cpp$(DependSuffix): RenderBitmapInterfaceGDI.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RenderBitmapInterfaceGDI.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RenderBitmapInterfaceGDI.cpp$(DependSuffix) -MM "RenderBitmapInterfaceGDI.cpp"

$(IntermediateDirectory)/RenderBitmapInterfaceGDI.cpp$(PreprocessSuffix): RenderBitmapInterfaceGDI.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RenderBitmapInterfaceGDI.cpp$(PreprocessSuffix) "RenderBitmapInterfaceGDI.cpp"

$(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(ObjectSuffix): RenderBitmapInterfaceOpenGL.cpp $(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libRenderBitmap/RenderBitmapInterfaceOpenGL.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(DependSuffix): RenderBitmapInterfaceOpenGL.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(DependSuffix) -MM "RenderBitmapInterfaceOpenGL.cpp"

$(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(PreprocessSuffix): RenderBitmapInterfaceOpenGL.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(PreprocessSuffix) "RenderBitmapInterfaceOpenGL.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


