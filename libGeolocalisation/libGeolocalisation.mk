##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libGeolocalisation
ConfigurationName      :=Debug
WorkspacePath          := "/home/figuinha/dev/Regards"
ProjectPath            := "/home/figuinha/dev/Regards/libGeolocalisation"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=root
Date                   :=22/09/15
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
Preprocessors          :=$(PreprocessorSwitch)__WXGTK__ $(PreprocessorSwitch)EXIV2 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libGeolocalisation.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../libUtility $(IncludeSwitch)../libExif $(IncludeSwitch)../libPicture $(IncludeSwitch)../libSqlData $(IncludeSwitch)../libSqlEngine $(IncludeSwitch)../../libextern/wxWidgets-3.0.2/include $(IncludeSwitch)../../libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0 
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
Objects0=$(IntermediateDirectory)/FileGeolocation.cpp$(ObjectSuffix) $(IntermediateDirectory)/Gps.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/home/figuinha/dev/Regards/.build-debug/libGeolocalisation"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/FileGeolocation.cpp$(ObjectSuffix): FileGeolocation.cpp $(IntermediateDirectory)/FileGeolocation.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libGeolocalisation/FileGeolocation.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FileGeolocation.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FileGeolocation.cpp$(DependSuffix): FileGeolocation.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FileGeolocation.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FileGeolocation.cpp$(DependSuffix) -MM "FileGeolocation.cpp"

$(IntermediateDirectory)/FileGeolocation.cpp$(PreprocessSuffix): FileGeolocation.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FileGeolocation.cpp$(PreprocessSuffix) "FileGeolocation.cpp"

$(IntermediateDirectory)/Gps.cpp$(ObjectSuffix): Gps.cpp $(IntermediateDirectory)/Gps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libGeolocalisation/Gps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Gps.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Gps.cpp$(DependSuffix): Gps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Gps.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Gps.cpp$(DependSuffix) -MM "Gps.cpp"

$(IntermediateDirectory)/Gps.cpp$(PreprocessSuffix): Gps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Gps.cpp$(PreprocessSuffix) "Gps.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


