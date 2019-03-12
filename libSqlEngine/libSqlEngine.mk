##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libSqlEngine
ConfigurationName      :=Debug
WorkspacePath          :=C:/developpement/git/Regards
ProjectPath            :=C:/developpement/git/Regards/libSqlEngine
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=jfigu
Date                   :=11/03/2019
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
ObjectsFileList        :="libSqlEngine.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/msys64/mingw64/bin/windres.exe
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)"C:\Program Files (x86)\AMD APP SDK\3.0_mingw\include" $(IncludeSwitch)/mingw64/include/libxml2 $(IncludeSwitch)/mingw64/include/OpenEXR  $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libUtility $(IncludeSwitch)../libResource $(IncludeSwitch)../include 
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
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread -fopenmp -std=gnu++11 -Wall -ggdb -O0 $(Preprocessors)
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
Objects0=$(IntermediateDirectory)/SqlExecuteRequest.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlResult.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlEngine.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlLib.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "C:\developpement\git\Regards/.build-debug/libSqlEngine"

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
$(IntermediateDirectory)/SqlExecuteRequest.cpp$(ObjectSuffix): SqlExecuteRequest.cpp $(IntermediateDirectory)/SqlExecuteRequest.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libSqlEngine/SqlExecuteRequest.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlExecuteRequest.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlExecuteRequest.cpp$(DependSuffix): SqlExecuteRequest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlExecuteRequest.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlExecuteRequest.cpp$(DependSuffix) -MM SqlExecuteRequest.cpp

$(IntermediateDirectory)/SqlExecuteRequest.cpp$(PreprocessSuffix): SqlExecuteRequest.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlExecuteRequest.cpp$(PreprocessSuffix) SqlExecuteRequest.cpp

$(IntermediateDirectory)/SqlResult.cpp$(ObjectSuffix): SqlResult.cpp $(IntermediateDirectory)/SqlResult.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libSqlEngine/SqlResult.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlResult.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlResult.cpp$(DependSuffix): SqlResult.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlResult.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlResult.cpp$(DependSuffix) -MM SqlResult.cpp

$(IntermediateDirectory)/SqlResult.cpp$(PreprocessSuffix): SqlResult.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlResult.cpp$(PreprocessSuffix) SqlResult.cpp

$(IntermediateDirectory)/SqlEngine.cpp$(ObjectSuffix): SqlEngine.cpp $(IntermediateDirectory)/SqlEngine.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libSqlEngine/SqlEngine.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlEngine.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlEngine.cpp$(DependSuffix): SqlEngine.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlEngine.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlEngine.cpp$(DependSuffix) -MM SqlEngine.cpp

$(IntermediateDirectory)/SqlEngine.cpp$(PreprocessSuffix): SqlEngine.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlEngine.cpp$(PreprocessSuffix) SqlEngine.cpp

$(IntermediateDirectory)/SqlLib.cpp$(ObjectSuffix): SqlLib.cpp $(IntermediateDirectory)/SqlLib.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libSqlEngine/SqlLib.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlLib.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlLib.cpp$(DependSuffix): SqlLib.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlLib.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlLib.cpp$(DependSuffix) -MM SqlLib.cpp

$(IntermediateDirectory)/SqlLib.cpp$(PreprocessSuffix): SqlLib.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlLib.cpp$(PreprocessSuffix) SqlLib.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/
	$(RM) ../include/Debug/header.h.gch


