##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=FaceRecognition
ConfigurationName      :=Debug
WorkspacePath          :=/home/figuinha/developpement/Regards
ProjectPath            :=/home/figuinha/developpement/Regards/FaceRecognition
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=10/02/19
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
OutputFile             :=$(IntermediateDirectory)/lib$(ProjectName).a
Preprocessors          :=$(PreprocessorSwitch)DLIB_JPEG_SUPPORT $(PreprocessorSwitch)DLIB_NO_GUI_SUPPORT $(PreprocessorSwitch)DLIB_JPEG_STATIC 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="FaceRecognition.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2 $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include  $(IncludeSwitch). $(IncludeSwitch)../libextern/dlib-19.7 $(IncludeSwitch). 
IncludePCH             := 
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
CXXFLAGS := -fopenmp -std=gnu++11 -Wall -fpermissive -fPIC -g $(Preprocessors)
CFLAGS   :=  -g $(Preprocessors)
ASFLAGS  := 
AS       := ccache /usr/bin/x86_64-linux-gnu-as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jquant2.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jquant1.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jmemnobs.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jmemmgr.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctint.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctfst.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctflt.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jfdctflt.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctred.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jerror.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmaster.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcdctmgr.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jccolor.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_all_source.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcsample.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmainct.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcinit.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdatasrc.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcparam.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcapistd.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcomapi.cpp$(ObjectSuffix) $(IntermediateDirectory)/FaceData.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jddctmgr.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcapimin.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jfdctint.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jfdctfst.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcmainct.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdpostct.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdcolor.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jccoefct.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcphuff.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jutils.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcmaster.cpp$(ObjectSuffix) $(IntermediateDirectory)/FaceDetector.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdapimin.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdsample.cpp$(ObjectSuffix) $(IntermediateDirectory)/base64.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcprepct.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmerge.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdapistd.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jchuff.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdinput.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdatadst.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdphuff.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcmarker.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdcoefct.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmarker.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdhuff.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "/home/figuinha/developpement/Regards/.build-debug"
	@echo rebuilt > "/home/figuinha/developpement/Regards/.build-debug/FaceRecognition"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jquant2.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jquant2.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jquant2.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jquant2.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jquant2.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jquant2.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jquant2.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jquant2.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jquant2.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jquant2.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jquant2.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jquant2.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jquant2.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jquant2.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jquant1.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jquant1.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jquant1.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jquant1.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jquant1.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jquant1.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jquant1.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jquant1.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jquant1.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jquant1.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jquant1.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jquant1.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jquant1.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jquant1.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jmemnobs.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jmemnobs.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jmemnobs.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jmemnobs.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jmemnobs.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jmemnobs.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jmemnobs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jmemnobs.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jmemnobs.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jmemnobs.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jmemnobs.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jmemnobs.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jmemnobs.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jmemnobs.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jmemmgr.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jmemmgr.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jmemmgr.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jmemmgr.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jmemmgr.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jmemmgr.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jmemmgr.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jmemmgr.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jmemmgr.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jmemmgr.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jmemmgr.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jmemmgr.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jmemmgr.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jmemmgr.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctint.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jidctint.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctint.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jidctint.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctint.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctint.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jidctint.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctint.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctint.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jidctint.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctint.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jidctint.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctint.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jidctint.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctfst.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jidctfst.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctfst.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jidctfst.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctfst.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctfst.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jidctfst.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctfst.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctfst.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jidctfst.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctfst.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jidctfst.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctfst.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jidctfst.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctflt.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jidctflt.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctflt.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jidctflt.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctflt.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctflt.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jidctflt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctflt.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctflt.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jidctflt.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctflt.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jidctflt.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctflt.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jidctflt.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jfdctflt.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jfdctflt.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jfdctflt.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jfdctflt.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jfdctflt.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jfdctflt.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jfdctflt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jfdctflt.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jfdctflt.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jfdctflt.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jfdctflt.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jfdctflt.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jfdctflt.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jfdctflt.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctred.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jidctred.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctred.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jidctred.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctred.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctred.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jidctred.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctred.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctred.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jidctred.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctred.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jidctred.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jidctred.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jidctred.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jerror.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jerror.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jerror.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jerror.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jerror.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jerror.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jerror.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jerror.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jerror.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jerror.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jerror.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jerror.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jerror.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jerror.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmaster.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdmaster.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmaster.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jdmaster.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmaster.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmaster.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdmaster.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmaster.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmaster.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jdmaster.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmaster.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdmaster.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmaster.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jdmaster.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcdctmgr.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcdctmgr.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcdctmgr.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jcdctmgr.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcdctmgr.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcdctmgr.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcdctmgr.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcdctmgr.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcdctmgr.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jcdctmgr.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcdctmgr.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcdctmgr.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcdctmgr.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jcdctmgr.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jccolor.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jccolor.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jccolor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jccolor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jccolor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jccolor.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jccolor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jccolor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jccolor.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jccolor.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jccolor.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jccolor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jccolor.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jccolor.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_all_source.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/all/source.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_all_source.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/all/source.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_all_source.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_all_source.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/all/source.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_all_source.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_all_source.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/all/source.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_all_source.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/all/source.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_all_source.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/all/source.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcsample.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcsample.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcsample.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jcsample.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcsample.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcsample.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcsample.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcsample.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcsample.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jcsample.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcsample.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcsample.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcsample.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jcsample.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmainct.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdmainct.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmainct.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jdmainct.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmainct.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmainct.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdmainct.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmainct.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmainct.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jdmainct.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmainct.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdmainct.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmainct.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jdmainct.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcinit.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcinit.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcinit.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jcinit.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcinit.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcinit.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcinit.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcinit.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcinit.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jcinit.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcinit.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcinit.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcinit.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jcinit.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdatasrc.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdatasrc.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdatasrc.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jdatasrc.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdatasrc.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdatasrc.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdatasrc.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdatasrc.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdatasrc.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jdatasrc.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdatasrc.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdatasrc.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdatasrc.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jdatasrc.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcparam.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcparam.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcparam.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jcparam.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcparam.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcparam.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcparam.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcparam.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcparam.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jcparam.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcparam.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcparam.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcparam.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jcparam.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcapistd.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcapistd.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcapistd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jcapistd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcapistd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcapistd.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcapistd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcapistd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcapistd.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jcapistd.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcapistd.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcapistd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcapistd.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jcapistd.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcomapi.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcomapi.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcomapi.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jcomapi.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcomapi.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcomapi.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcomapi.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcomapi.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcomapi.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jcomapi.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcomapi.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcomapi.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcomapi.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jcomapi.cpp

$(IntermediateDirectory)/FaceData.cpp$(ObjectSuffix): FaceData.cpp $(IntermediateDirectory)/FaceData.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/FaceRecognition/FaceData.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FaceData.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FaceData.cpp$(DependSuffix): FaceData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FaceData.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FaceData.cpp$(DependSuffix) -MM FaceData.cpp

$(IntermediateDirectory)/FaceData.cpp$(PreprocessSuffix): FaceData.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FaceData.cpp$(PreprocessSuffix) FaceData.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jddctmgr.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jddctmgr.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jddctmgr.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jddctmgr.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jddctmgr.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jddctmgr.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jddctmgr.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jddctmgr.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jddctmgr.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jddctmgr.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jddctmgr.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jddctmgr.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jddctmgr.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jddctmgr.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcapimin.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcapimin.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcapimin.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jcapimin.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcapimin.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcapimin.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcapimin.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcapimin.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcapimin.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jcapimin.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcapimin.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcapimin.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcapimin.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jcapimin.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jfdctint.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jfdctint.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jfdctint.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jfdctint.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jfdctint.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jfdctint.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jfdctint.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jfdctint.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jfdctint.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jfdctint.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jfdctint.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jfdctint.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jfdctint.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jfdctint.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jfdctfst.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jfdctfst.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jfdctfst.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jfdctfst.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jfdctfst.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jfdctfst.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jfdctfst.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jfdctfst.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jfdctfst.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jfdctfst.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jfdctfst.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jfdctfst.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jfdctfst.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jfdctfst.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcmainct.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcmainct.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcmainct.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jcmainct.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcmainct.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcmainct.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcmainct.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcmainct.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcmainct.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jcmainct.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcmainct.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcmainct.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcmainct.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jcmainct.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdpostct.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdpostct.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdpostct.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jdpostct.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdpostct.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdpostct.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdpostct.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdpostct.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdpostct.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jdpostct.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdpostct.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdpostct.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdpostct.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jdpostct.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdcolor.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdcolor.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdcolor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jdcolor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdcolor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdcolor.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdcolor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdcolor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdcolor.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jdcolor.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdcolor.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdcolor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdcolor.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jdcolor.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jccoefct.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jccoefct.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jccoefct.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jccoefct.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jccoefct.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jccoefct.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jccoefct.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jccoefct.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jccoefct.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jccoefct.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jccoefct.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jccoefct.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jccoefct.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jccoefct.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcphuff.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcphuff.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcphuff.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jcphuff.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcphuff.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcphuff.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcphuff.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcphuff.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcphuff.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jcphuff.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcphuff.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcphuff.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcphuff.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jcphuff.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jutils.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jutils.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jutils.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jutils.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jutils.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jutils.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jutils.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jutils.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jutils.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jutils.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jutils.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jutils.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jutils.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jutils.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcmaster.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcmaster.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcmaster.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jcmaster.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcmaster.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcmaster.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcmaster.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcmaster.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcmaster.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jcmaster.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcmaster.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcmaster.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcmaster.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jcmaster.cpp

$(IntermediateDirectory)/FaceDetector.cpp$(ObjectSuffix): FaceDetector.cpp $(IntermediateDirectory)/FaceDetector.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/FaceRecognition/FaceDetector.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FaceDetector.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FaceDetector.cpp$(DependSuffix): FaceDetector.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FaceDetector.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FaceDetector.cpp$(DependSuffix) -MM FaceDetector.cpp

$(IntermediateDirectory)/FaceDetector.cpp$(PreprocessSuffix): FaceDetector.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FaceDetector.cpp$(PreprocessSuffix) FaceDetector.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdapimin.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdapimin.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdapimin.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jdapimin.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdapimin.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdapimin.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdapimin.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdapimin.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdapimin.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jdapimin.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdapimin.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdapimin.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdapimin.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jdapimin.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdsample.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdsample.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdsample.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jdsample.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdsample.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdsample.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdsample.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdsample.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdsample.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jdsample.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdsample.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdsample.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdsample.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jdsample.cpp

$(IntermediateDirectory)/base64.cpp$(ObjectSuffix): base64.cpp $(IntermediateDirectory)/base64.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/FaceRecognition/base64.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/base64.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/base64.cpp$(DependSuffix): base64.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/base64.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/base64.cpp$(DependSuffix) -MM base64.cpp

$(IntermediateDirectory)/base64.cpp$(PreprocessSuffix): base64.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/base64.cpp$(PreprocessSuffix) base64.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcprepct.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcprepct.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcprepct.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jcprepct.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcprepct.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcprepct.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcprepct.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcprepct.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcprepct.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jcprepct.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcprepct.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcprepct.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcprepct.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jcprepct.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmerge.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdmerge.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmerge.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jdmerge.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmerge.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmerge.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdmerge.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmerge.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmerge.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jdmerge.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmerge.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdmerge.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmerge.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jdmerge.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdapistd.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdapistd.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdapistd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jdapistd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdapistd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdapistd.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdapistd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdapistd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdapistd.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jdapistd.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdapistd.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdapistd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdapistd.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jdapistd.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jchuff.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jchuff.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jchuff.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jchuff.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jchuff.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jchuff.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jchuff.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jchuff.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jchuff.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jchuff.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jchuff.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jchuff.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jchuff.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jchuff.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdinput.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdinput.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdinput.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jdinput.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdinput.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdinput.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdinput.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdinput.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdinput.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jdinput.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdinput.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdinput.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdinput.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jdinput.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdatadst.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdatadst.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdatadst.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jdatadst.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdatadst.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdatadst.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdatadst.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdatadst.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdatadst.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jdatadst.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdatadst.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdatadst.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdatadst.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jdatadst.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdphuff.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdphuff.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdphuff.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jdphuff.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdphuff.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdphuff.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdphuff.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdphuff.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdphuff.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jdphuff.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdphuff.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdphuff.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdphuff.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jdphuff.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcmarker.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcmarker.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcmarker.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jcmarker.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcmarker.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcmarker.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcmarker.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcmarker.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcmarker.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jcmarker.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcmarker.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jcmarker.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jcmarker.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jcmarker.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdcoefct.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdcoefct.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdcoefct.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jdcoefct.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdcoefct.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdcoefct.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdcoefct.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdcoefct.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdcoefct.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jdcoefct.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdcoefct.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdcoefct.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdcoefct.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jdcoefct.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmarker.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdmarker.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmarker.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jdmarker.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmarker.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmarker.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdmarker.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmarker.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmarker.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jdmarker.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmarker.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdmarker.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdmarker.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jdmarker.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdhuff.cpp$(ObjectSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdhuff.cpp $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdhuff.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libjpeg/jdhuff.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdhuff.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdhuff.cpp$(DependSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdhuff.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdhuff.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdhuff.cpp$(DependSuffix) -MM ../libextern/dlib-19.7/dlib/external/libjpeg/jdhuff.cpp

$(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdhuff.cpp$(PreprocessSuffix): ../libextern/dlib-19.7/dlib/external/libjpeg/jdhuff.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libextern_dlib-19.7_dlib_external_libjpeg_jdhuff.cpp$(PreprocessSuffix) ../libextern/dlib-19.7/dlib/external/libjpeg/jdhuff.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


