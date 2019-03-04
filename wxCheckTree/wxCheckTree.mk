##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=wxCheckTree
ConfigurationName      :=Debug
WorkspacePath          :=C:/developpement/git/Regards
ProjectPath            :=C:/developpement/git/Regards/wxCheckTree
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
OutputFile             :=$(IntermediateDirectory)/lib$(ProjectName).a
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="wxCheckTree.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/msys64/mingw64/bin/windres.exe
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)"C:\Program Files (x86)\AMD APP SDK\3.0_mingw\include" $(IncludeSwitch)/mingw64/include/libxml2 $(IncludeSwitch)/mingw64/include/OpenEXR  $(IncludeSwitch)../libUtility $(IncludeSwitch)../include 
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
CXXFLAGS := -std=gnu++11 $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread -fopenmp -ggdb -O0 $(Preprocessors)
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
Objects0=$(IntermediateDirectory)/checktree.cpp$(ObjectSuffix) $(IntermediateDirectory)/directoryctrl.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "C:\developpement\git\Regards/.build-debug/wxCheckTree"

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
$(IntermediateDirectory)/checktree.cpp$(ObjectSuffix): checktree.cpp $(IntermediateDirectory)/checktree.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/wxCheckTree/checktree.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/checktree.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/checktree.cpp$(DependSuffix): checktree.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/checktree.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/checktree.cpp$(DependSuffix) -MM checktree.cpp

$(IntermediateDirectory)/checktree.cpp$(PreprocessSuffix): checktree.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/checktree.cpp$(PreprocessSuffix) checktree.cpp

$(IntermediateDirectory)/directoryctrl.cpp$(ObjectSuffix): directoryctrl.cpp $(IntermediateDirectory)/directoryctrl.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/wxCheckTree/directoryctrl.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/directoryctrl.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/directoryctrl.cpp$(DependSuffix): directoryctrl.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/directoryctrl.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/directoryctrl.cpp$(DependSuffix) -MM directoryctrl.cpp

$(IntermediateDirectory)/directoryctrl.cpp$(PreprocessSuffix): directoryctrl.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/directoryctrl.cpp$(PreprocessSuffix) directoryctrl.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/
	$(RM) ../include/Debug/header.h.gch


