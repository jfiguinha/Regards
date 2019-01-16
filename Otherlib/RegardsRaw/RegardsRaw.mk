##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=RegardsRaw
ConfigurationName      :=Release
WorkspacePath          :=/home/figuinha/Developpement/Regards
ProjectPath            :=/home/figuinha/Developpement/Regards/Otherlib/RegardsRaw
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=15/01/19
CodeLitePath           :=/home/figuinha/.codelite
LinkerName             :=/usr/bin/ccache /usr/bin/g++
SharedObjectLinkerName :=/usr/bin/ccache /usr/bin/g++ -shared -fPIC
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
Preprocessors          :=$(PreprocessorSwitch)LIBRAW $(PreprocessorSwitch)LIBRAW_NODLL $(PreprocessorSwitch)_USE_MATH_DEFINES $(PreprocessorSwitch)NDEBUG $(PreprocessorSwitch)LIBRAW_DEMOSAIC_PACK_GPL2 $(PreprocessorSwitch)LIBRAW_DEMOSAIC_PACK_GPL3 $(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="RegardsRaw.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -Os
IncludePath            := $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2  $(IncludeSwitch)../ $(IncludeSwitch)../RegardsRaw $(IncludeSwitch). $(IncludeSwitch)../../libOpenCL $(IncludeSwitch)../../libDataStructure $(IncludeSwitch)../../libUtility $(IncludeSwitch)../../CxImage $(IncludeSwitch)../../libextern/jasper-1.900.1/src/libjasper/include $(IncludeSwitch)demoisaic_pack/gpl2 $(IncludeSwitch)demoisaic_pack/gpl3 $(IncludeSwitch)../../libextern/wxWidgets-master/src/jpeg $(IncludeSwitch)../../libFiltre 
IncludePCH             :=  -include ../../include/Release/header.h 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                :=$(LibraryPathSwitch)/opt/AMDAPPSDK-3.0/lib  $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/ccache /usr/bin/g++
CC       := /usr/bin/ccache /usr/bin/gcc
CXXFLAGS := -fopenmp -std=gnu++11 -Wall $(shell ../../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread -fopenmp -Wno-narrowing -O2 -msse4.1 $(Preprocessors)
CFLAGS   := $(shell ../../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread  -O2 -Os -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/internal_dcraw_fileio.cpp$(ObjectSuffix) $(IntermediateDirectory)/DecodeRawPicture.cpp$(ObjectSuffix) $(IntermediateDirectory)/internal_dcraw_common.cpp$(ObjectSuffix) $(IntermediateDirectory)/internal_demosaic_packs.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_libraw_c_api.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_libraw_cxx.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_libraw_datastream.cpp$(ObjectSuffix) $(IntermediateDirectory)/RegardsRaw.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "/home/figuinha/Developpement/Regards/.build-release"
	@echo rebuilt > "/home/figuinha/Developpement/Regards/.build-release/RegardsRaw"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


./Release:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:
	@echo Executing Pre Build commands ...
	
	@echo Done

# PreCompiled Header
../../include/Release/header.h.gch: ../../include/Release/header.h
	$(CXX) $(SourceSwitch) ../../include/Release/header.h $(PCHCompileFlags) $(CXXFLAGS) $(IncludePath)



##
## Objects
##
$(IntermediateDirectory)/internal_dcraw_fileio.cpp$(ObjectSuffix): internal/dcraw_fileio.cpp $(IntermediateDirectory)/internal_dcraw_fileio.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/Otherlib/RegardsRaw/internal/dcraw_fileio.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/internal_dcraw_fileio.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/internal_dcraw_fileio.cpp$(DependSuffix): internal/dcraw_fileio.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/internal_dcraw_fileio.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/internal_dcraw_fileio.cpp$(DependSuffix) -MM internal/dcraw_fileio.cpp

$(IntermediateDirectory)/internal_dcraw_fileio.cpp$(PreprocessSuffix): internal/dcraw_fileio.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/internal_dcraw_fileio.cpp$(PreprocessSuffix) internal/dcraw_fileio.cpp

$(IntermediateDirectory)/DecodeRawPicture.cpp$(ObjectSuffix): DecodeRawPicture.cpp $(IntermediateDirectory)/DecodeRawPicture.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/Otherlib/RegardsRaw/DecodeRawPicture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/DecodeRawPicture.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/DecodeRawPicture.cpp$(DependSuffix): DecodeRawPicture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/DecodeRawPicture.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/DecodeRawPicture.cpp$(DependSuffix) -MM DecodeRawPicture.cpp

$(IntermediateDirectory)/DecodeRawPicture.cpp$(PreprocessSuffix): DecodeRawPicture.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/DecodeRawPicture.cpp$(PreprocessSuffix) DecodeRawPicture.cpp

$(IntermediateDirectory)/internal_dcraw_common.cpp$(ObjectSuffix): internal/dcraw_common.cpp $(IntermediateDirectory)/internal_dcraw_common.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/Otherlib/RegardsRaw/internal/dcraw_common.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/internal_dcraw_common.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/internal_dcraw_common.cpp$(DependSuffix): internal/dcraw_common.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/internal_dcraw_common.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/internal_dcraw_common.cpp$(DependSuffix) -MM internal/dcraw_common.cpp

$(IntermediateDirectory)/internal_dcraw_common.cpp$(PreprocessSuffix): internal/dcraw_common.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/internal_dcraw_common.cpp$(PreprocessSuffix) internal/dcraw_common.cpp

$(IntermediateDirectory)/internal_demosaic_packs.cpp$(ObjectSuffix): internal/demosaic_packs.cpp $(IntermediateDirectory)/internal_demosaic_packs.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/Otherlib/RegardsRaw/internal/demosaic_packs.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/internal_demosaic_packs.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/internal_demosaic_packs.cpp$(DependSuffix): internal/demosaic_packs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/internal_demosaic_packs.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/internal_demosaic_packs.cpp$(DependSuffix) -MM internal/demosaic_packs.cpp

$(IntermediateDirectory)/internal_demosaic_packs.cpp$(PreprocessSuffix): internal/demosaic_packs.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/internal_demosaic_packs.cpp$(PreprocessSuffix) internal/demosaic_packs.cpp

$(IntermediateDirectory)/src_libraw_c_api.cpp$(ObjectSuffix): src/libraw_c_api.cpp $(IntermediateDirectory)/src_libraw_c_api.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/Otherlib/RegardsRaw/src/libraw_c_api.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_libraw_c_api.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_libraw_c_api.cpp$(DependSuffix): src/libraw_c_api.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_libraw_c_api.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_libraw_c_api.cpp$(DependSuffix) -MM src/libraw_c_api.cpp

$(IntermediateDirectory)/src_libraw_c_api.cpp$(PreprocessSuffix): src/libraw_c_api.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_libraw_c_api.cpp$(PreprocessSuffix) src/libraw_c_api.cpp

$(IntermediateDirectory)/src_libraw_cxx.cpp$(ObjectSuffix): src/libraw_cxx.cpp $(IntermediateDirectory)/src_libraw_cxx.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/Otherlib/RegardsRaw/src/libraw_cxx.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_libraw_cxx.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_libraw_cxx.cpp$(DependSuffix): src/libraw_cxx.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_libraw_cxx.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_libraw_cxx.cpp$(DependSuffix) -MM src/libraw_cxx.cpp

$(IntermediateDirectory)/src_libraw_cxx.cpp$(PreprocessSuffix): src/libraw_cxx.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_libraw_cxx.cpp$(PreprocessSuffix) src/libraw_cxx.cpp

$(IntermediateDirectory)/src_libraw_datastream.cpp$(ObjectSuffix): src/libraw_datastream.cpp $(IntermediateDirectory)/src_libraw_datastream.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/Otherlib/RegardsRaw/src/libraw_datastream.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_libraw_datastream.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_libraw_datastream.cpp$(DependSuffix): src/libraw_datastream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_libraw_datastream.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_libraw_datastream.cpp$(DependSuffix) -MM src/libraw_datastream.cpp

$(IntermediateDirectory)/src_libraw_datastream.cpp$(PreprocessSuffix): src/libraw_datastream.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_libraw_datastream.cpp$(PreprocessSuffix) src/libraw_datastream.cpp

$(IntermediateDirectory)/RegardsRaw.cpp$(ObjectSuffix): RegardsRaw.cpp $(IntermediateDirectory)/RegardsRaw.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/Otherlib/RegardsRaw/RegardsRaw.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RegardsRaw.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RegardsRaw.cpp$(DependSuffix): RegardsRaw.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RegardsRaw.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RegardsRaw.cpp$(DependSuffix) -MM RegardsRaw.cpp

$(IntermediateDirectory)/RegardsRaw.cpp$(PreprocessSuffix): RegardsRaw.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RegardsRaw.cpp$(PreprocessSuffix) RegardsRaw.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/
	$(RM) ../../include/Release/header.h.gch


