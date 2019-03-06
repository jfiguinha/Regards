##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libIntro
ConfigurationName      :=Debug
WorkspacePath          :=C:/developpement/git/Regards
ProjectPath            :=C:/developpement/git/Regards/libIntro
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
ObjectsFileList        :="libIntro.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/msys64/mingw64/bin/windres.exe
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)"C:\Program Files (x86)\AMD APP SDK\3.0_mingw\include" $(IncludeSwitch)/mingw64/include/libxml2 $(IncludeSwitch)/mingw64/include/OpenEXR  $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libTheme $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libParameter $(IncludeSwitch)../libUtility $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libResource $(IncludeSwitch)../libSqlData $(IncludeSwitch)../libOpenGL $(IncludeSwitch)../libRenderBitmap $(IncludeSwitch)../libSqlEngine $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libFiltre $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../Otherlib/wxSVG/include $(IncludeSwitch)../libWindow $(IncludeSwitch)../libPrint $(IncludeSwitch)../libPicture $(IncludeSwitch)../libTheme $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libControl $(IncludeSwitch)../libIntro $(IncludeSwitch)../libWindowMain 
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
Objects0=$(IntermediateDirectory)/MyCentralWindowIntro.cpp$(ObjectSuffix) $(IntermediateDirectory)/IntroTheme.cpp$(ObjectSuffix) $(IntermediateDirectory)/MyFrameIntro.cpp$(ObjectSuffix) $(IntermediateDirectory)/infoAbout.cpp$(ObjectSuffix) $(IntermediateDirectory)/TitleIntro.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "C:\developpement\git\Regards/.build-debug/libIntro"

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
$(IntermediateDirectory)/MyCentralWindowIntro.cpp$(ObjectSuffix): MyCentralWindowIntro.cpp $(IntermediateDirectory)/MyCentralWindowIntro.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libIntro/MyCentralWindowIntro.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MyCentralWindowIntro.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MyCentralWindowIntro.cpp$(DependSuffix): MyCentralWindowIntro.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MyCentralWindowIntro.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MyCentralWindowIntro.cpp$(DependSuffix) -MM MyCentralWindowIntro.cpp

$(IntermediateDirectory)/MyCentralWindowIntro.cpp$(PreprocessSuffix): MyCentralWindowIntro.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MyCentralWindowIntro.cpp$(PreprocessSuffix) MyCentralWindowIntro.cpp

$(IntermediateDirectory)/IntroTheme.cpp$(ObjectSuffix): IntroTheme.cpp $(IntermediateDirectory)/IntroTheme.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libIntro/IntroTheme.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IntroTheme.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IntroTheme.cpp$(DependSuffix): IntroTheme.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IntroTheme.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IntroTheme.cpp$(DependSuffix) -MM IntroTheme.cpp

$(IntermediateDirectory)/IntroTheme.cpp$(PreprocessSuffix): IntroTheme.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IntroTheme.cpp$(PreprocessSuffix) IntroTheme.cpp

$(IntermediateDirectory)/MyFrameIntro.cpp$(ObjectSuffix): MyFrameIntro.cpp $(IntermediateDirectory)/MyFrameIntro.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libIntro/MyFrameIntro.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MyFrameIntro.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MyFrameIntro.cpp$(DependSuffix): MyFrameIntro.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MyFrameIntro.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MyFrameIntro.cpp$(DependSuffix) -MM MyFrameIntro.cpp

$(IntermediateDirectory)/MyFrameIntro.cpp$(PreprocessSuffix): MyFrameIntro.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MyFrameIntro.cpp$(PreprocessSuffix) MyFrameIntro.cpp

$(IntermediateDirectory)/infoAbout.cpp$(ObjectSuffix): infoAbout.cpp $(IntermediateDirectory)/infoAbout.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libIntro/infoAbout.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/infoAbout.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/infoAbout.cpp$(DependSuffix): infoAbout.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/infoAbout.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/infoAbout.cpp$(DependSuffix) -MM infoAbout.cpp

$(IntermediateDirectory)/infoAbout.cpp$(PreprocessSuffix): infoAbout.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/infoAbout.cpp$(PreprocessSuffix) infoAbout.cpp

$(IntermediateDirectory)/TitleIntro.cpp$(ObjectSuffix): TitleIntro.cpp $(IntermediateDirectory)/TitleIntro.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libIntro/TitleIntro.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TitleIntro.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TitleIntro.cpp$(DependSuffix): TitleIntro.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TitleIntro.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TitleIntro.cpp$(DependSuffix) -MM TitleIntro.cpp

$(IntermediateDirectory)/TitleIntro.cpp$(PreprocessSuffix): TitleIntro.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TitleIntro.cpp$(PreprocessSuffix) TitleIntro.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/
	$(RM) ../include/Debug/header.h.gch


