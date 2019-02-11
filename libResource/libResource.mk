##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libResource
ConfigurationName      :=Release
WorkspacePath          :=C:/Regards
ProjectPath            :=C:/Regards/libResource
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=jfigu_000
Date                   :=11/02/2019
CodeLitePath           :="C:/Program Files/CodeLite"
LinkerName             :=C:/msys64/usr/bin/ccache.exe C:/msys64/mingw64/bin/g++.exe
SharedObjectLinkerName :=C:/msys64/mingw64/bin/g++.exe -shared -fPIC
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
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libResource.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/msys64/mingw64/bin/windres.exe
LinkOptions            :=  -s 
IncludePath            := $(IncludeSwitch)"C:\Program Files (x86)\AMD APP SDK\3.0_mingw\include" $(IncludeSwitch)/mingw64/include/libxml2 $(IncludeSwitch)/mingw64/include/OpenEXR  $(IncludeSwitch)../libPicture $(IncludeSwitch). $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libParameter $(IncludeSwitch)../libFiltre $(IncludeSwitch)../libOpenGL $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../libUtility $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libResource $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libSqlEngine $(IncludeSwitch)../libCompress 
IncludePCH             :=  -include ../include/Release/header.h 
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
CC       := C:/msys64/usr/bin/ccache.exe  C:/msys64/mingw64/bin/gcc.exe
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread  -fopenmp -std=gnu++11 -Wall -Wno-narrowing -O2 -msse4.1 $(Preprocessors)
CFLAGS   :=  -Wall  -O2 -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := C:/msys64/mingw64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
PATH:=C:\msys64\usr\bin;$PATH
WXWIN:=C:\Regards\libextern\wxwidgets-3.1.2
WXCFG:=lib/mswud
Objects0=$(IntermediateDirectory)/LibResource.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlResource.cpp$(ObjectSuffix) $(IntermediateDirectory)/ClosedHandCursor.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlLibResource.cpp$(ObjectSuffix) $(IntermediateDirectory)/PictureData.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "C:\Regards/.build-release"
	@echo rebuilt > "C:\Regards/.build-release/libResource"

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Release"


./Release:
	@$(MakeDirCommand) "./Release"

PreBuild:
	@echo Executing Pre Build commands ...
	
	@echo Done

# PreCompiled Header
../include/Release/header.h.gch: ../include/Release/header.h
	$(CXX) $(SourceSwitch) ../include/Release/header.h $(PCHCompileFlags) $(CXXFLAGS) $(IncludePath)



##
## Objects
##
$(IntermediateDirectory)/LibResource.cpp$(ObjectSuffix): LibResource.cpp $(IntermediateDirectory)/LibResource.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/libResource/LibResource.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LibResource.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LibResource.cpp$(DependSuffix): LibResource.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LibResource.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LibResource.cpp$(DependSuffix) -MM LibResource.cpp

$(IntermediateDirectory)/LibResource.cpp$(PreprocessSuffix): LibResource.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LibResource.cpp$(PreprocessSuffix) LibResource.cpp

$(IntermediateDirectory)/SqlResource.cpp$(ObjectSuffix): SqlResource.cpp $(IntermediateDirectory)/SqlResource.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/libResource/SqlResource.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlResource.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlResource.cpp$(DependSuffix): SqlResource.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlResource.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlResource.cpp$(DependSuffix) -MM SqlResource.cpp

$(IntermediateDirectory)/SqlResource.cpp$(PreprocessSuffix): SqlResource.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlResource.cpp$(PreprocessSuffix) SqlResource.cpp

$(IntermediateDirectory)/ClosedHandCursor.cpp$(ObjectSuffix): ClosedHandCursor.cpp $(IntermediateDirectory)/ClosedHandCursor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/libResource/ClosedHandCursor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ClosedHandCursor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ClosedHandCursor.cpp$(DependSuffix): ClosedHandCursor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ClosedHandCursor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ClosedHandCursor.cpp$(DependSuffix) -MM ClosedHandCursor.cpp

$(IntermediateDirectory)/ClosedHandCursor.cpp$(PreprocessSuffix): ClosedHandCursor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ClosedHandCursor.cpp$(PreprocessSuffix) ClosedHandCursor.cpp

$(IntermediateDirectory)/SqlLibResource.cpp$(ObjectSuffix): SqlLibResource.cpp $(IntermediateDirectory)/SqlLibResource.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/libResource/SqlLibResource.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlLibResource.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlLibResource.cpp$(DependSuffix): SqlLibResource.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlLibResource.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlLibResource.cpp$(DependSuffix) -MM SqlLibResource.cpp

$(IntermediateDirectory)/SqlLibResource.cpp$(PreprocessSuffix): SqlLibResource.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlLibResource.cpp$(PreprocessSuffix) SqlLibResource.cpp

$(IntermediateDirectory)/PictureData.cpp$(ObjectSuffix): PictureData.cpp $(IntermediateDirectory)/PictureData.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/libResource/PictureData.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PictureData.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PictureData.cpp$(DependSuffix): PictureData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PictureData.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PictureData.cpp$(DependSuffix) -MM PictureData.cpp

$(IntermediateDirectory)/PictureData.cpp$(PreprocessSuffix): PictureData.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PictureData.cpp$(PreprocessSuffix) PictureData.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/
	$(RM) ../include/Release/header.h.gch


