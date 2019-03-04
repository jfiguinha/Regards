##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libParameter
ConfigurationName      :=Debug
WorkspacePath          :=C:/developpement/git/Regards
ProjectPath            :=C:/developpement/git/Regards/libParameter
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=jfigu
Date                   :=04/03/2019
CodeLitePath           :="C:/Program Files/CodeLite"
LinkerName             :=C:/msys64/usr/bin/ccache.exe C:/msys64/mingw64/bin/g++.exe
SharedObjectLinkerName :=C:/msys64/usr/bin/ccache.exe C:/msys64/mingw64/bin/g++.exe -shared -fPIC
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
ObjectsFileList        :="libParameter.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/msys64/mingw64/bin/windres.exe
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)"C:\Program Files (x86)\AMD APP SDK\3.0_mingw\include" $(IncludeSwitch)/mingw64/include/libxml2 $(IncludeSwitch)/mingw64/include/OpenEXR  $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../include $(IncludeSwitch)../libUtility 
IncludePCH             :=  -include ../include/Debug/header.h 
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
CC       := C:/msys64/usr/bin/ccache.exe C:/msys64/mingw64/bin/gcc.exe
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread  -fopenmp -std=gnu++11 -Wall -ggdb -O0 $(Preprocessors)
CFLAGS   :=  -Wall  -ggdb -O0 $(Preprocessors)
ASFLAGS  := 
AS       := C:/msys64/usr/bin/ccache.exe C:/msys64/mingw64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
PATH:=C:\msys64\usr\bin;$PATH
WXWIN:=C:\Regards\libextern\wxwidgets-3.1.2
WXCFG:=lib/mswud
Objects0=$(IntermediateDirectory)/RegardsConfigParam.cpp$(ObjectSuffix) $(IntermediateDirectory)/ConfigParam.cpp$(ObjectSuffix) $(IntermediateDirectory)/ParamInit.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "C:\developpement\git\Regards/.build-debug"
	@echo rebuilt > "C:\developpement\git\Regards/.build-debug/libParameter"

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Debug"


./Debug:
	@$(MakeDirCommand) "./Debug"

PreBuild:

# PreCompiled Header
../include/Debug/header.h.gch: ../include/Debug/header.h
	$(CXX) $(SourceSwitch) ../include/Debug/header.h $(PCHCompileFlags) $(CXXFLAGS) $(IncludePath)



##
## Objects
##
$(IntermediateDirectory)/RegardsConfigParam.cpp$(ObjectSuffix): RegardsConfigParam.cpp $(IntermediateDirectory)/RegardsConfigParam.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libParameter/RegardsConfigParam.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RegardsConfigParam.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RegardsConfigParam.cpp$(DependSuffix): RegardsConfigParam.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RegardsConfigParam.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RegardsConfigParam.cpp$(DependSuffix) -MM RegardsConfigParam.cpp

$(IntermediateDirectory)/RegardsConfigParam.cpp$(PreprocessSuffix): RegardsConfigParam.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RegardsConfigParam.cpp$(PreprocessSuffix) RegardsConfigParam.cpp

$(IntermediateDirectory)/ConfigParam.cpp$(ObjectSuffix): ConfigParam.cpp $(IntermediateDirectory)/ConfigParam.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libParameter/ConfigParam.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ConfigParam.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ConfigParam.cpp$(DependSuffix): ConfigParam.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ConfigParam.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ConfigParam.cpp$(DependSuffix) -MM ConfigParam.cpp

$(IntermediateDirectory)/ConfigParam.cpp$(PreprocessSuffix): ConfigParam.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ConfigParam.cpp$(PreprocessSuffix) ConfigParam.cpp

$(IntermediateDirectory)/ParamInit.cpp$(ObjectSuffix): ParamInit.cpp $(IntermediateDirectory)/ParamInit.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libParameter/ParamInit.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ParamInit.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ParamInit.cpp$(DependSuffix): ParamInit.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ParamInit.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ParamInit.cpp$(DependSuffix) -MM ParamInit.cpp

$(IntermediateDirectory)/ParamInit.cpp$(PreprocessSuffix): ParamInit.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ParamInit.cpp$(PreprocessSuffix) ParamInit.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/
	$(RM) ../include/Debug/header.h.gch


