##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libResource
ConfigurationName      :=Debug
WorkspacePath          := "/home/figuinha/dev/Regards"
ProjectPath            := "/home/figuinha/dev/Regards/libResource"
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
ObjectsFileList        :="libResource.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libParameter $(IncludeSwitch)../libFiltre $(IncludeSwitch)../libOpenGL $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../libUtility $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libResource $(IncludeSwitch)../libextern/glew-1.13.0/include $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libSqlEngine $(IncludeSwitch)../libCompress $(IncludeSwitch)../libextern/wxWidgets-3.0.2/include $(IncludeSwitch)../libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0 
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
Objects0=$(IntermediateDirectory)/ClosedHandCursor.cpp$(ObjectSuffix) $(IntermediateDirectory)/LibResource.cpp$(ObjectSuffix) $(IntermediateDirectory)/PictureData.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlLibResource.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlResource.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/home/figuinha/dev/Regards/.build-debug/libResource"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/ClosedHandCursor.cpp$(ObjectSuffix): ClosedHandCursor.cpp $(IntermediateDirectory)/ClosedHandCursor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libResource/ClosedHandCursor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ClosedHandCursor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ClosedHandCursor.cpp$(DependSuffix): ClosedHandCursor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ClosedHandCursor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ClosedHandCursor.cpp$(DependSuffix) -MM "ClosedHandCursor.cpp"

$(IntermediateDirectory)/ClosedHandCursor.cpp$(PreprocessSuffix): ClosedHandCursor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ClosedHandCursor.cpp$(PreprocessSuffix) "ClosedHandCursor.cpp"

$(IntermediateDirectory)/LibResource.cpp$(ObjectSuffix): LibResource.cpp $(IntermediateDirectory)/LibResource.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libResource/LibResource.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LibResource.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LibResource.cpp$(DependSuffix): LibResource.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LibResource.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LibResource.cpp$(DependSuffix) -MM "LibResource.cpp"

$(IntermediateDirectory)/LibResource.cpp$(PreprocessSuffix): LibResource.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LibResource.cpp$(PreprocessSuffix) "LibResource.cpp"

$(IntermediateDirectory)/PictureData.cpp$(ObjectSuffix): PictureData.cpp $(IntermediateDirectory)/PictureData.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libResource/PictureData.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PictureData.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PictureData.cpp$(DependSuffix): PictureData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PictureData.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PictureData.cpp$(DependSuffix) -MM "PictureData.cpp"

$(IntermediateDirectory)/PictureData.cpp$(PreprocessSuffix): PictureData.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PictureData.cpp$(PreprocessSuffix) "PictureData.cpp"

$(IntermediateDirectory)/SqlLibResource.cpp$(ObjectSuffix): SqlLibResource.cpp $(IntermediateDirectory)/SqlLibResource.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libResource/SqlLibResource.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlLibResource.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlLibResource.cpp$(DependSuffix): SqlLibResource.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlLibResource.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlLibResource.cpp$(DependSuffix) -MM "SqlLibResource.cpp"

$(IntermediateDirectory)/SqlLibResource.cpp$(PreprocessSuffix): SqlLibResource.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlLibResource.cpp$(PreprocessSuffix) "SqlLibResource.cpp"

$(IntermediateDirectory)/SqlResource.cpp$(ObjectSuffix): SqlResource.cpp $(IntermediateDirectory)/SqlResource.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libResource/SqlResource.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlResource.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlResource.cpp$(DependSuffix): SqlResource.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlResource.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlResource.cpp$(DependSuffix) -MM "SqlResource.cpp"

$(IntermediateDirectory)/SqlResource.cpp$(PreprocessSuffix): SqlResource.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlResource.cpp$(PreprocessSuffix) "SqlResource.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


