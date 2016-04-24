##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libPicture
ConfigurationName      :=Debug
WorkspacePath          := "/home/figuinha/dev/Regards"
ProjectPath            := "/home/figuinha/dev/Regards/libPicture"
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
Preprocessors          :=$(PreprocessorSwitch)__WXGTK__ $(PreprocessorSwitch)_LINUX $(PreprocessorSwitch)LIBRAW $(PreprocessorSwitch)EXIV2 $(PreprocessorSwitch)FFMPEG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libPicture.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../libOpenCL $(IncludeSwitch)../libResource $(IncludeSwitch)../include $(IncludeSwitch)../Otherlib/RegardsRaw $(IncludeSwitch)../CxImage $(IncludeSwitch)../libFiltre $(IncludeSwitch)../Otherlib/libRegardsBpg $(IncludeSwitch)../libUtility $(IncludeSwitch)../libVideoThumbnail $(IncludeSwitch)../libExif $(IncludeSwitch)../libDialog $(IncludeSwitch)../Otherlib/wxSVG/include $(IncludeSwitch)../Otherlib/RegardsBpg $(IncludeSwitch)../libextern/wxWidgets-3.0.2/include $(IncludeSwitch)../libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0 
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
Objects0=$(IntermediateDirectory)/CPCD.cpp$(ObjectSuffix) $(IntermediateDirectory)/libPicture.cpp$(ObjectSuffix) $(IntermediateDirectory)/ScaleThumbnail.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/home/figuinha/dev/Regards/.build-debug/libPicture"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/CPCD.cpp$(ObjectSuffix): CPCD.cpp $(IntermediateDirectory)/CPCD.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libPicture/CPCD.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CPCD.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CPCD.cpp$(DependSuffix): CPCD.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CPCD.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CPCD.cpp$(DependSuffix) -MM "CPCD.cpp"

$(IntermediateDirectory)/CPCD.cpp$(PreprocessSuffix): CPCD.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CPCD.cpp$(PreprocessSuffix) "CPCD.cpp"

$(IntermediateDirectory)/libPicture.cpp$(ObjectSuffix): libPicture.cpp $(IntermediateDirectory)/libPicture.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libPicture/libPicture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/libPicture.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/libPicture.cpp$(DependSuffix): libPicture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/libPicture.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/libPicture.cpp$(DependSuffix) -MM "libPicture.cpp"

$(IntermediateDirectory)/libPicture.cpp$(PreprocessSuffix): libPicture.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/libPicture.cpp$(PreprocessSuffix) "libPicture.cpp"

$(IntermediateDirectory)/ScaleThumbnail.cpp$(ObjectSuffix): ScaleThumbnail.cpp $(IntermediateDirectory)/ScaleThumbnail.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libPicture/ScaleThumbnail.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ScaleThumbnail.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ScaleThumbnail.cpp$(DependSuffix): ScaleThumbnail.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ScaleThumbnail.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ScaleThumbnail.cpp$(DependSuffix) -MM "ScaleThumbnail.cpp"

$(IntermediateDirectory)/ScaleThumbnail.cpp$(PreprocessSuffix): ScaleThumbnail.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ScaleThumbnail.cpp$(PreprocessSuffix) "ScaleThumbnail.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


