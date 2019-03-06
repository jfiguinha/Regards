##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libVideoThumbnail
ConfigurationName      :=Debug
WorkspacePath          :=C:/developpement/git/Regards
ProjectPath            :=C:/developpement/git/Regards/libVideoThumbnail
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
Preprocessors          :=$(PreprocessorSwitch)LATEST_GREATEST_FFMPEG $(PreprocessorSwitch)FFMPEG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libVideoThumbnail.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/msys64/mingw64/bin/windres.exe
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)"C:\Program Files (x86)\AMD APP SDK\3.0_mingw\include" $(IncludeSwitch)/mingw64/include/libxml2 $(IncludeSwitch)/mingw64/include/OpenEXR  $(IncludeSwitch). $(IncludeSwitch)../CxImage $(IncludeSwitch)../libextern/ffmpeg-4.1 $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libUtility $(IncludeSwitch)../include 
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
Objects0=$(IntermediateDirectory)/moviestreaminfo.cpp$(ObjectSuffix) $(IntermediateDirectory)/videostream.cpp$(ObjectSuffix) $(IntermediateDirectory)/videothumbnailer.cpp$(ObjectSuffix) $(IntermediateDirectory)/filmstripfilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/stringoperations.cpp$(ObjectSuffix) $(IntermediateDirectory)/moviedecoder.cpp$(ObjectSuffix) $(IntermediateDirectory)/videothumb.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "C:\developpement\git\Regards/.build-debug/libVideoThumbnail"

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
$(IntermediateDirectory)/moviestreaminfo.cpp$(ObjectSuffix): moviestreaminfo.cpp $(IntermediateDirectory)/moviestreaminfo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libVideoThumbnail/moviestreaminfo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/moviestreaminfo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/moviestreaminfo.cpp$(DependSuffix): moviestreaminfo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/moviestreaminfo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/moviestreaminfo.cpp$(DependSuffix) -MM moviestreaminfo.cpp

$(IntermediateDirectory)/moviestreaminfo.cpp$(PreprocessSuffix): moviestreaminfo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/moviestreaminfo.cpp$(PreprocessSuffix) moviestreaminfo.cpp

$(IntermediateDirectory)/videostream.cpp$(ObjectSuffix): videostream.cpp $(IntermediateDirectory)/videostream.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libVideoThumbnail/videostream.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/videostream.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/videostream.cpp$(DependSuffix): videostream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/videostream.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/videostream.cpp$(DependSuffix) -MM videostream.cpp

$(IntermediateDirectory)/videostream.cpp$(PreprocessSuffix): videostream.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/videostream.cpp$(PreprocessSuffix) videostream.cpp

$(IntermediateDirectory)/videothumbnailer.cpp$(ObjectSuffix): videothumbnailer.cpp $(IntermediateDirectory)/videothumbnailer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libVideoThumbnail/videothumbnailer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/videothumbnailer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/videothumbnailer.cpp$(DependSuffix): videothumbnailer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/videothumbnailer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/videothumbnailer.cpp$(DependSuffix) -MM videothumbnailer.cpp

$(IntermediateDirectory)/videothumbnailer.cpp$(PreprocessSuffix): videothumbnailer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/videothumbnailer.cpp$(PreprocessSuffix) videothumbnailer.cpp

$(IntermediateDirectory)/filmstripfilter.cpp$(ObjectSuffix): filmstripfilter.cpp $(IntermediateDirectory)/filmstripfilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libVideoThumbnail/filmstripfilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/filmstripfilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/filmstripfilter.cpp$(DependSuffix): filmstripfilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/filmstripfilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/filmstripfilter.cpp$(DependSuffix) -MM filmstripfilter.cpp

$(IntermediateDirectory)/filmstripfilter.cpp$(PreprocessSuffix): filmstripfilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/filmstripfilter.cpp$(PreprocessSuffix) filmstripfilter.cpp

$(IntermediateDirectory)/stringoperations.cpp$(ObjectSuffix): stringoperations.cpp $(IntermediateDirectory)/stringoperations.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libVideoThumbnail/stringoperations.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/stringoperations.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/stringoperations.cpp$(DependSuffix): stringoperations.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/stringoperations.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/stringoperations.cpp$(DependSuffix) -MM stringoperations.cpp

$(IntermediateDirectory)/stringoperations.cpp$(PreprocessSuffix): stringoperations.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/stringoperations.cpp$(PreprocessSuffix) stringoperations.cpp

$(IntermediateDirectory)/moviedecoder.cpp$(ObjectSuffix): moviedecoder.cpp $(IntermediateDirectory)/moviedecoder.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libVideoThumbnail/moviedecoder.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/moviedecoder.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/moviedecoder.cpp$(DependSuffix): moviedecoder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/moviedecoder.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/moviedecoder.cpp$(DependSuffix) -MM moviedecoder.cpp

$(IntermediateDirectory)/moviedecoder.cpp$(PreprocessSuffix): moviedecoder.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/moviedecoder.cpp$(PreprocessSuffix) moviedecoder.cpp

$(IntermediateDirectory)/videothumb.cpp$(ObjectSuffix): videothumb.cpp $(IntermediateDirectory)/videothumb.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libVideoThumbnail/videothumb.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/videothumb.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/videothumb.cpp$(DependSuffix): videothumb.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/videothumb.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/videothumb.cpp$(DependSuffix) -MM videothumb.cpp

$(IntermediateDirectory)/videothumb.cpp$(PreprocessSuffix): videothumb.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/videothumb.cpp$(PreprocessSuffix) videothumb.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/
	$(RM) ../include/Debug/header.h.gch


