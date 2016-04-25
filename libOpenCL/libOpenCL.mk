##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libOpenCL
ConfigurationName      :=Release
WorkspacePath          := "/home/figuinha/dev/Regards"
ProjectPath            := "/home/figuinha/dev/Regards/libOpenCL"
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
OutputFile             :=$(IntermediateDirectory)/libRegardsCL.a
Preprocessors          :=$(PreprocessorSwitch)NDEBUG $(PreprocessorSwitch)__WXGTK__ 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libOpenCL.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../libUtility $(IncludeSwitch)../libextern/exiv2-0.24/src $(IncludeSwitch)../libextern/exiv2-0.24/xmpsdk/include $(IncludeSwitch)../libResource $(IncludeSwitch)../libParameter $(IncludeSwitch)/opt/intel/intel-opencl-1.2-5.0.0.43/opencl-sdk/include $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../include $(IncludeSwitch)../libextern/wxWidgets-3.0.2/include $(IncludeSwitch)../libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0 
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
Objects0=$(IntermediateDirectory)/OpenCLContext.cpp$(ObjectSuffix) $(IntermediateDirectory)/OpenCLEffect.cpp$(ObjectSuffix) $(IntermediateDirectory)/OpenCLEngine.cpp$(ObjectSuffix) $(IntermediateDirectory)/OpenCLExecuteProgram.cpp$(ObjectSuffix) $(IntermediateDirectory)/OpenCLInfos.cpp$(ObjectSuffix) $(IntermediateDirectory)/OpenCLParameter.cpp$(ObjectSuffix) $(IntermediateDirectory)/OpenCLProgram.cpp$(ObjectSuffix) $(IntermediateDirectory)/utility.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/home/figuinha/dev/Regards/.build-release/libOpenCL"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


./Release:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/OpenCLContext.cpp$(ObjectSuffix): OpenCLContext.cpp $(IntermediateDirectory)/OpenCLContext.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libOpenCL/OpenCLContext.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/OpenCLContext.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OpenCLContext.cpp$(DependSuffix): OpenCLContext.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/OpenCLContext.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/OpenCLContext.cpp$(DependSuffix) -MM "OpenCLContext.cpp"

$(IntermediateDirectory)/OpenCLContext.cpp$(PreprocessSuffix): OpenCLContext.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OpenCLContext.cpp$(PreprocessSuffix) "OpenCLContext.cpp"

$(IntermediateDirectory)/OpenCLEffect.cpp$(ObjectSuffix): OpenCLEffect.cpp $(IntermediateDirectory)/OpenCLEffect.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libOpenCL/OpenCLEffect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/OpenCLEffect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OpenCLEffect.cpp$(DependSuffix): OpenCLEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/OpenCLEffect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/OpenCLEffect.cpp$(DependSuffix) -MM "OpenCLEffect.cpp"

$(IntermediateDirectory)/OpenCLEffect.cpp$(PreprocessSuffix): OpenCLEffect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OpenCLEffect.cpp$(PreprocessSuffix) "OpenCLEffect.cpp"

$(IntermediateDirectory)/OpenCLEngine.cpp$(ObjectSuffix): OpenCLEngine.cpp $(IntermediateDirectory)/OpenCLEngine.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libOpenCL/OpenCLEngine.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/OpenCLEngine.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OpenCLEngine.cpp$(DependSuffix): OpenCLEngine.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/OpenCLEngine.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/OpenCLEngine.cpp$(DependSuffix) -MM "OpenCLEngine.cpp"

$(IntermediateDirectory)/OpenCLEngine.cpp$(PreprocessSuffix): OpenCLEngine.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OpenCLEngine.cpp$(PreprocessSuffix) "OpenCLEngine.cpp"

$(IntermediateDirectory)/OpenCLExecuteProgram.cpp$(ObjectSuffix): OpenCLExecuteProgram.cpp $(IntermediateDirectory)/OpenCLExecuteProgram.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libOpenCL/OpenCLExecuteProgram.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/OpenCLExecuteProgram.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OpenCLExecuteProgram.cpp$(DependSuffix): OpenCLExecuteProgram.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/OpenCLExecuteProgram.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/OpenCLExecuteProgram.cpp$(DependSuffix) -MM "OpenCLExecuteProgram.cpp"

$(IntermediateDirectory)/OpenCLExecuteProgram.cpp$(PreprocessSuffix): OpenCLExecuteProgram.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OpenCLExecuteProgram.cpp$(PreprocessSuffix) "OpenCLExecuteProgram.cpp"

$(IntermediateDirectory)/OpenCLInfos.cpp$(ObjectSuffix): OpenCLInfos.cpp $(IntermediateDirectory)/OpenCLInfos.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libOpenCL/OpenCLInfos.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/OpenCLInfos.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OpenCLInfos.cpp$(DependSuffix): OpenCLInfos.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/OpenCLInfos.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/OpenCLInfos.cpp$(DependSuffix) -MM "OpenCLInfos.cpp"

$(IntermediateDirectory)/OpenCLInfos.cpp$(PreprocessSuffix): OpenCLInfos.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OpenCLInfos.cpp$(PreprocessSuffix) "OpenCLInfos.cpp"

$(IntermediateDirectory)/OpenCLParameter.cpp$(ObjectSuffix): OpenCLParameter.cpp $(IntermediateDirectory)/OpenCLParameter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libOpenCL/OpenCLParameter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/OpenCLParameter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OpenCLParameter.cpp$(DependSuffix): OpenCLParameter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/OpenCLParameter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/OpenCLParameter.cpp$(DependSuffix) -MM "OpenCLParameter.cpp"

$(IntermediateDirectory)/OpenCLParameter.cpp$(PreprocessSuffix): OpenCLParameter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OpenCLParameter.cpp$(PreprocessSuffix) "OpenCLParameter.cpp"

$(IntermediateDirectory)/OpenCLProgram.cpp$(ObjectSuffix): OpenCLProgram.cpp $(IntermediateDirectory)/OpenCLProgram.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libOpenCL/OpenCLProgram.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/OpenCLProgram.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OpenCLProgram.cpp$(DependSuffix): OpenCLProgram.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/OpenCLProgram.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/OpenCLProgram.cpp$(DependSuffix) -MM "OpenCLProgram.cpp"

$(IntermediateDirectory)/OpenCLProgram.cpp$(PreprocessSuffix): OpenCLProgram.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OpenCLProgram.cpp$(PreprocessSuffix) "OpenCLProgram.cpp"

$(IntermediateDirectory)/utility.cpp$(ObjectSuffix): utility.cpp $(IntermediateDirectory)/utility.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libOpenCL/utility.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/utility.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/utility.cpp$(DependSuffix): utility.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/utility.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/utility.cpp$(DependSuffix) -MM "utility.cpp"

$(IntermediateDirectory)/utility.cpp$(PreprocessSuffix): utility.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/utility.cpp$(PreprocessSuffix) "utility.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


