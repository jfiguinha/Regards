##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libTheme
ConfigurationName      :=Debug
WorkspacePath          :=C:/developpement/git/Regards
ProjectPath            :=C:/developpement/git/Regards/libTheme
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
ObjectsFileList        :="libTheme.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/msys64/mingw64/bin/windres.exe
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)"C:\Program Files (x86)\AMD APP SDK\3.0_mingw\include" $(IncludeSwitch)/mingw64/include/libxml2 $(IncludeSwitch)/mingw64/include/OpenEXR  $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libParameter $(IncludeSwitch)../include $(IncludeSwitch)../libUtility 
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
CFLAGS   :=  -Wall -ggdb -O0 $(Preprocessors)
ASFLAGS  := 
AS       := C:/msys64/usr/bin/ccache.exe C:/msys64/mingw64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
PATH:=C:\msys64\usr\bin;$PATH
WXWIN:=C:\Regards\libextern\wxwidgets-3.1.2
WXCFG:=lib/mswud
Objects0=$(IntermediateDirectory)/theme.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThemeInit.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThemeParam.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "C:\developpement\git\Regards/.build-debug/libTheme"

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
$(IntermediateDirectory)/theme.cpp$(ObjectSuffix): theme.cpp $(IntermediateDirectory)/theme.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libTheme/theme.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/theme.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/theme.cpp$(DependSuffix): theme.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/theme.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/theme.cpp$(DependSuffix) -MM theme.cpp

$(IntermediateDirectory)/theme.cpp$(PreprocessSuffix): theme.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/theme.cpp$(PreprocessSuffix) theme.cpp

$(IntermediateDirectory)/ThemeInit.cpp$(ObjectSuffix): ThemeInit.cpp $(IntermediateDirectory)/ThemeInit.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libTheme/ThemeInit.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThemeInit.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThemeInit.cpp$(DependSuffix): ThemeInit.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThemeInit.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThemeInit.cpp$(DependSuffix) -MM ThemeInit.cpp

$(IntermediateDirectory)/ThemeInit.cpp$(PreprocessSuffix): ThemeInit.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThemeInit.cpp$(PreprocessSuffix) ThemeInit.cpp

$(IntermediateDirectory)/ThemeParam.cpp$(ObjectSuffix): ThemeParam.cpp $(IntermediateDirectory)/ThemeParam.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libTheme/ThemeParam.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThemeParam.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThemeParam.cpp$(DependSuffix): ThemeParam.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThemeParam.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThemeParam.cpp$(DependSuffix) -MM ThemeParam.cpp

$(IntermediateDirectory)/ThemeParam.cpp$(PreprocessSuffix): ThemeParam.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThemeParam.cpp$(PreprocessSuffix) ThemeParam.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/
	$(RM) ../include/Debug/header.h.gch


