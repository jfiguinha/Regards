##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libAppleFunction
ConfigurationName      :=Debug
WorkspacePath          :=C:/developpement/git/Regards
ProjectPath            :=C:/developpement/git/Regards/libAppleFunction
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=jfigu
Date                   :=06/03/2019
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
ObjectsFileList        :="libAppleFunction.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/msys64/mingw64/bin/windres.exe
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)"C:\Program Files (x86)\AMD APP SDK\3.0_mingw\include" $(IncludeSwitch)/mingw64/include/libxml2 $(IncludeSwitch)/mingw64/include/OpenEXR  $(IncludeSwitch). 
IncludePCH             := 
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
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread -fopenmp -std=gnu++11 -fpermissive -g -O0 $(Preprocessors)
CFLAGS   := -fopenmp $(shell ../libextern/wxWidgets-master/wx-config --cflags) -fpermissive -g $(Preprocessors)
ASFLAGS  := 
AS       := C:/msys64/usr/bin/ccache.exe C:/msys64/mingw64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
PATH:=C:\msys64\usr\bin;$PATH
WXWIN:=C:\Regards\libextern\wxwidgets-3.1.2
WXCFG:=lib/mswud
Objects0=$(IntermediateDirectory)/SendEmail.cpp$(ObjectSuffix) $(IntermediateDirectory)/SaveFromCFunction.mm$(ObjectSuffix) $(IntermediateDirectory)/MyEmail.mm$(ObjectSuffix) $(IntermediateDirectory)/SaveFileFormat.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "C:\developpement\git\Regards/.build-debug/libAppleFunction"

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Debug"


./Debug:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/SendEmail.cpp$(ObjectSuffix): SendEmail.cpp $(IntermediateDirectory)/SendEmail.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libAppleFunction/SendEmail.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SendEmail.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SendEmail.cpp$(DependSuffix): SendEmail.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SendEmail.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SendEmail.cpp$(DependSuffix) -MM SendEmail.cpp

$(IntermediateDirectory)/SendEmail.cpp$(PreprocessSuffix): SendEmail.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SendEmail.cpp$(PreprocessSuffix) SendEmail.cpp

$(IntermediateDirectory)/SaveFromCFunction.mm$(ObjectSuffix): SaveFromCFunction.mm $(IntermediateDirectory)/SaveFromCFunction.mm$(DependSuffix)
	$(CXX) $(IncludePCH) -x objective-c++ $(SourceSwitch) "C:/developpement/git/Regards/libAppleFunction/SaveFromCFunction.mm" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SaveFromCFunction.mm$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SaveFromCFunction.mm$(DependSuffix): SaveFromCFunction.mm
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SaveFromCFunction.mm$(ObjectSuffix) -MF$(IntermediateDirectory)/SaveFromCFunction.mm$(DependSuffix) -MM SaveFromCFunction.mm

$(IntermediateDirectory)/SaveFromCFunction.mm$(PreprocessSuffix): SaveFromCFunction.mm
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SaveFromCFunction.mm$(PreprocessSuffix) SaveFromCFunction.mm

$(IntermediateDirectory)/MyEmail.mm$(ObjectSuffix): MyEmail.mm $(IntermediateDirectory)/MyEmail.mm$(DependSuffix)
	$(CXX) $(IncludePCH) -x objective-c++ $(SourceSwitch) "C:/developpement/git/Regards/libAppleFunction/MyEmail.mm" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MyEmail.mm$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MyEmail.mm$(DependSuffix): MyEmail.mm
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MyEmail.mm$(ObjectSuffix) -MF$(IntermediateDirectory)/MyEmail.mm$(DependSuffix) -MM MyEmail.mm

$(IntermediateDirectory)/MyEmail.mm$(PreprocessSuffix): MyEmail.mm
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MyEmail.mm$(PreprocessSuffix) MyEmail.mm

$(IntermediateDirectory)/SaveFileFormat.cpp$(ObjectSuffix): SaveFileFormat.cpp $(IntermediateDirectory)/SaveFileFormat.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libAppleFunction/SaveFileFormat.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SaveFileFormat.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SaveFileFormat.cpp$(DependSuffix): SaveFileFormat.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SaveFileFormat.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SaveFileFormat.cpp$(DependSuffix) -MM SaveFileFormat.cpp

$(IntermediateDirectory)/SaveFileFormat.cpp$(PreprocessSuffix): SaveFileFormat.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SaveFileFormat.cpp$(PreprocessSuffix) SaveFileFormat.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


