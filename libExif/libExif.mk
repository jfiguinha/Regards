##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libExif
ConfigurationName      :=Debug
WorkspacePath          :=C:/developpement/git/Regards
ProjectPath            :=C:/developpement/git/Regards/libExif
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
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)_regards.a
Preprocessors          :=$(PreprocessorSwitch)LIBHEIC 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libExif.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/msys64/mingw64/bin/windres.exe
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)"C:\Program Files (x86)\AMD APP SDK\3.0_mingw\include" $(IncludeSwitch)/mingw64/include/libxml2 $(IncludeSwitch)/mingw64/include/OpenEXR  $(IncludeSwitch). $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libResource $(IncludeSwitch)../libextern/exiv2-0.26/include/exiv2 $(IncludeSwitch)../libextern/exiv2-0.26/xmpsdk/include $(IncludeSwitch)../CxImage $(IncludeSwitch)../libUtility $(IncludeSwitch)../libPicture $(IncludeSwitch)../libMediaInfo $(IncludeSwitch)../include 
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
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread -fopenmp -std=gnu++11 -ggdb -O0 $(Preprocessors)
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
Objects0=$(IntermediateDirectory)/MetadataExiv2.cpp$(ObjectSuffix) $(IntermediateDirectory)/PictureMetadataExiv.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "C:\developpement\git\Regards/.build-debug/libExif"

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
$(IntermediateDirectory)/MetadataExiv2.cpp$(ObjectSuffix): MetadataExiv2.cpp $(IntermediateDirectory)/MetadataExiv2.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libExif/MetadataExiv2.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MetadataExiv2.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MetadataExiv2.cpp$(DependSuffix): MetadataExiv2.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MetadataExiv2.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MetadataExiv2.cpp$(DependSuffix) -MM MetadataExiv2.cpp

$(IntermediateDirectory)/MetadataExiv2.cpp$(PreprocessSuffix): MetadataExiv2.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MetadataExiv2.cpp$(PreprocessSuffix) MetadataExiv2.cpp

$(IntermediateDirectory)/PictureMetadataExiv.cpp$(ObjectSuffix): PictureMetadataExiv.cpp $(IntermediateDirectory)/PictureMetadataExiv.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libExif/PictureMetadataExiv.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PictureMetadataExiv.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PictureMetadataExiv.cpp$(DependSuffix): PictureMetadataExiv.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PictureMetadataExiv.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PictureMetadataExiv.cpp$(DependSuffix) -MM PictureMetadataExiv.cpp

$(IntermediateDirectory)/PictureMetadataExiv.cpp$(PreprocessSuffix): PictureMetadataExiv.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PictureMetadataExiv.cpp$(PreprocessSuffix) PictureMetadataExiv.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/
	$(RM) ../include/Debug/header.h.gch


