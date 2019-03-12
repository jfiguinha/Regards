##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libUtility
ConfigurationName      :=Debug
WorkspacePath          :=C:/developpement/git/Regards
ProjectPath            :=C:/developpement/git/Regards/libUtility
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
ObjectsFileList        :="libUtility.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/msys64/mingw64/bin/windres.exe
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)"C:\Program Files (x86)\AMD APP SDK\3.0_mingw\include" $(IncludeSwitch)/mingw64/include/libxml2 $(IncludeSwitch)/mingw64/include/OpenEXR  $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libResource $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../include 
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
CFLAGS   := -Wall  -ggdb -O0  $(Preprocessors)
ASFLAGS  := 
AS       := C:/msys64/usr/bin/ccache.exe C:/msys64/mingw64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
PATH:=C:\msys64\usr\bin;$PATH
WXWIN:=C:\Regards\libextern\wxwidgets-3.1.2
WXCFG:=lib/mswud
Objects0=$(IntermediateDirectory)/FileUtility.cpp$(ObjectSuffix) $(IntermediateDirectory)/LoadingResource.cpp$(ObjectSuffix) $(IntermediateDirectory)/ConvertUtility.cpp$(ObjectSuffix) $(IntermediateDirectory)/wxmd5.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "C:\developpement\git\Regards/.build-debug/libUtility"

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
$(IntermediateDirectory)/FileUtility.cpp$(ObjectSuffix): FileUtility.cpp $(IntermediateDirectory)/FileUtility.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libUtility/FileUtility.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FileUtility.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FileUtility.cpp$(DependSuffix): FileUtility.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FileUtility.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FileUtility.cpp$(DependSuffix) -MM FileUtility.cpp

$(IntermediateDirectory)/FileUtility.cpp$(PreprocessSuffix): FileUtility.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FileUtility.cpp$(PreprocessSuffix) FileUtility.cpp

$(IntermediateDirectory)/LoadingResource.cpp$(ObjectSuffix): LoadingResource.cpp $(IntermediateDirectory)/LoadingResource.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libUtility/LoadingResource.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LoadingResource.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LoadingResource.cpp$(DependSuffix): LoadingResource.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LoadingResource.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LoadingResource.cpp$(DependSuffix) -MM LoadingResource.cpp

$(IntermediateDirectory)/LoadingResource.cpp$(PreprocessSuffix): LoadingResource.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LoadingResource.cpp$(PreprocessSuffix) LoadingResource.cpp

$(IntermediateDirectory)/ConvertUtility.cpp$(ObjectSuffix): ConvertUtility.cpp $(IntermediateDirectory)/ConvertUtility.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libUtility/ConvertUtility.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ConvertUtility.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ConvertUtility.cpp$(DependSuffix): ConvertUtility.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ConvertUtility.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ConvertUtility.cpp$(DependSuffix) -MM ConvertUtility.cpp

$(IntermediateDirectory)/ConvertUtility.cpp$(PreprocessSuffix): ConvertUtility.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ConvertUtility.cpp$(PreprocessSuffix) ConvertUtility.cpp

$(IntermediateDirectory)/wxmd5.cpp$(ObjectSuffix): wxmd5.cpp $(IntermediateDirectory)/wxmd5.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libUtility/wxmd5.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wxmd5.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wxmd5.cpp$(DependSuffix): wxmd5.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/wxmd5.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/wxmd5.cpp$(DependSuffix) -MM wxmd5.cpp

$(IntermediateDirectory)/wxmd5.cpp$(PreprocessSuffix): wxmd5.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wxmd5.cpp$(PreprocessSuffix) wxmd5.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/
	$(RM) ../include/Debug/header.h.gch


