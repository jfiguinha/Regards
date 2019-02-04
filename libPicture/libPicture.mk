##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libPicture
ConfigurationName      :=Release
WorkspacePath          :=/home/figuinha/developpement/Regards
ProjectPath            :=/home/figuinha/developpement/Regards/libPicture
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=04/02/19
CodeLitePath           :=/home/figuinha/.codelite
LinkerName             :=ccache /usr/bin/x86_64-linux-gnu-g++
SharedObjectLinkerName :=ccache /usr/bin/x86_64-linux-gnu-g++ -shared -fPIC
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
Preprocessors          :=$(PreprocessorSwitch)LIBRAW $(PreprocessorSwitch)EXIV2 $(PreprocessorSwitch)FFMPEG $(PreprocessorSwitch)TURBOJPEG $(PreprocessorSwitch)LIBHEIC $(PreprocessorSwitch)LIBBPG $(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libPicture.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s 
IncludePath            := $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2 $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include  $(IncludeSwitch). $(IncludeSwitch)../libPiccante $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../libOpenCL $(IncludeSwitch)../libResource $(IncludeSwitch)../include $(IncludeSwitch)../Otherlib/RegardsRaw $(IncludeSwitch)../Otherlib/libbpg/Bpg/libRegardsBpg $(IncludeSwitch)../CxImage $(IncludeSwitch)../libFiltre $(IncludeSwitch)../libUtility $(IncludeSwitch)../libVideoThumbnail $(IncludeSwitch)../libExif $(IncludeSwitch)../libDialog $(IncludeSwitch)../Otherlib/libbpg/libRegardsBpg $(IncludeSwitch)../Otherlib/wxSVG/include $(IncludeSwitch)../libextern/libjpeg-turbo-1.5.1 $(IncludeSwitch)../libextern/libwebp-0.6.0/src $(IncludeSwitch)../libextern/libde265-master/libde265 $(IncludeSwitch)../libextern/heif-master/srcs/api/reader $(IncludeSwitch)../libextern/heif-master/srcs/api/common $(IncludeSwitch)../libextern/libde265-master 
IncludePCH             :=  -include ../include/Release/header.h 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                :=$(LibraryPathSwitch)/opt/AMDAPPSDK-3.0/lib/x86_64/sdk  $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/x86_64-linux-gnu-ar rcu
CXX      := ccache /usr/bin/x86_64-linux-gnu-g++
CC       := ccache /usr/bin/x86_64-linux-gnu-gcc
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread  -fopenmp -std=gnu++11 -Wall -O2 -msse4.1 $(Preprocessors)
CFLAGS   :=  -Wall  -O2 -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := ccache /usr/bin/x86_64-linux-gnu-as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/libPicture.cpp$(ObjectSuffix) $(IntermediateDirectory)/CPCD.cpp$(ObjectSuffix) $(IntermediateDirectory)/ScaleThumbnail.cpp$(ObjectSuffix) $(IntermediateDirectory)/raw.cpp$(ObjectSuffix) $(IntermediateDirectory)/Heic.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "/home/figuinha/developpement/Regards/.build-release"
	@echo rebuilt > "/home/figuinha/developpement/Regards/.build-release/libPicture"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


./Release:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:

# PreCompiled Header
../include/Release/header.h.gch: ../include/Release/header.h
	$(CXX) $(SourceSwitch) ../include/Release/header.h $(PCHCompileFlags) $(CXXFLAGS) $(IncludePath)



##
## Objects
##
$(IntermediateDirectory)/libPicture.cpp$(ObjectSuffix): libPicture.cpp $(IntermediateDirectory)/libPicture.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libPicture/libPicture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/libPicture.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/libPicture.cpp$(DependSuffix): libPicture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/libPicture.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/libPicture.cpp$(DependSuffix) -MM libPicture.cpp

$(IntermediateDirectory)/libPicture.cpp$(PreprocessSuffix): libPicture.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/libPicture.cpp$(PreprocessSuffix) libPicture.cpp

$(IntermediateDirectory)/CPCD.cpp$(ObjectSuffix): CPCD.cpp $(IntermediateDirectory)/CPCD.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libPicture/CPCD.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CPCD.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CPCD.cpp$(DependSuffix): CPCD.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CPCD.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CPCD.cpp$(DependSuffix) -MM CPCD.cpp

$(IntermediateDirectory)/CPCD.cpp$(PreprocessSuffix): CPCD.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CPCD.cpp$(PreprocessSuffix) CPCD.cpp

$(IntermediateDirectory)/ScaleThumbnail.cpp$(ObjectSuffix): ScaleThumbnail.cpp $(IntermediateDirectory)/ScaleThumbnail.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libPicture/ScaleThumbnail.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ScaleThumbnail.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ScaleThumbnail.cpp$(DependSuffix): ScaleThumbnail.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ScaleThumbnail.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ScaleThumbnail.cpp$(DependSuffix) -MM ScaleThumbnail.cpp

$(IntermediateDirectory)/ScaleThumbnail.cpp$(PreprocessSuffix): ScaleThumbnail.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ScaleThumbnail.cpp$(PreprocessSuffix) ScaleThumbnail.cpp

$(IntermediateDirectory)/raw.cpp$(ObjectSuffix): raw.cpp $(IntermediateDirectory)/raw.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libPicture/raw.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/raw.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/raw.cpp$(DependSuffix): raw.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/raw.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/raw.cpp$(DependSuffix) -MM raw.cpp

$(IntermediateDirectory)/raw.cpp$(PreprocessSuffix): raw.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/raw.cpp$(PreprocessSuffix) raw.cpp

$(IntermediateDirectory)/Heic.cpp$(ObjectSuffix): Heic.cpp $(IntermediateDirectory)/Heic.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libPicture/Heic.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Heic.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Heic.cpp$(DependSuffix): Heic.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Heic.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Heic.cpp$(DependSuffix) -MM Heic.cpp

$(IntermediateDirectory)/Heic.cpp$(PreprocessSuffix): Heic.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Heic.cpp$(PreprocessSuffix) Heic.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/
	$(RM) ../include/Release/header.h.gch


