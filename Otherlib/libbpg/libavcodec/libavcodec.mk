##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libavcodec
ConfigurationName      :=Release
WorkspacePath          :=/home/figuinha/developpement/Regards
ProjectPath            :=/home/figuinha/developpement/Regards/Otherlib/libbpg/libavcodec
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
Preprocessors          :=$(PreprocessorSwitch)USE_VAR_BIT_DEPTH $(PreprocessorSwitch)_ISOC99_SOURCE $(PreprocessorSwitch)HAVE_AV_CONFIG_H $(PreprocessorSwitch)_FILE_OFFSET_BITS=64 $(PreprocessorSwitch)_LARGEFILE_SOURCE $(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libavcodec.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2 $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include  $(IncludeSwitch). $(IncludeSwitch)../ $(IncludeSwitch). 
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
CXXFLAGS :=  -O2 -msse4.1 $(Preprocessors)
CFLAGS   := -std=c99 -Wall -fPIC -pthread -O2 -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := ccache /usr/bin/x86_64-linux-gnu-as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/hevc.c$(ObjectSuffix) $(IntermediateDirectory)/cabac.c$(ObjectSuffix) $(IntermediateDirectory)/allcodecs.c$(ObjectSuffix) $(IntermediateDirectory)/hevc_cabac.c$(ObjectSuffix) $(IntermediateDirectory)/bit_depth_template.c$(ObjectSuffix) $(IntermediateDirectory)/hevc_ps.c$(ObjectSuffix) $(IntermediateDirectory)/videodsp.c$(ObjectSuffix) $(IntermediateDirectory)/hevcdsp.c$(ObjectSuffix) $(IntermediateDirectory)/hevc_filter.c$(ObjectSuffix) $(IntermediateDirectory)/utils.c$(ObjectSuffix) \
	$(IntermediateDirectory)/hevc_mvs.c$(ObjectSuffix) $(IntermediateDirectory)/hevcpred.c$(ObjectSuffix) $(IntermediateDirectory)/hevc_refs.c$(ObjectSuffix) $(IntermediateDirectory)/golomb.c$(ObjectSuffix) $(IntermediateDirectory)/hevc_sei.c$(ObjectSuffix) 



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
	@echo rebuilt > "/home/figuinha/developpement/Regards/.build-release/libavcodec"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


./Release:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/hevc.c$(ObjectSuffix): hevc.c $(IntermediateDirectory)/hevc.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libbpg/libavcodec/hevc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hevc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hevc.c$(DependSuffix): hevc.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hevc.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hevc.c$(DependSuffix) -MM hevc.c

$(IntermediateDirectory)/hevc.c$(PreprocessSuffix): hevc.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hevc.c$(PreprocessSuffix) hevc.c

$(IntermediateDirectory)/cabac.c$(ObjectSuffix): cabac.c $(IntermediateDirectory)/cabac.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libbpg/libavcodec/cabac.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/cabac.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/cabac.c$(DependSuffix): cabac.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/cabac.c$(ObjectSuffix) -MF$(IntermediateDirectory)/cabac.c$(DependSuffix) -MM cabac.c

$(IntermediateDirectory)/cabac.c$(PreprocessSuffix): cabac.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/cabac.c$(PreprocessSuffix) cabac.c

$(IntermediateDirectory)/allcodecs.c$(ObjectSuffix): allcodecs.c $(IntermediateDirectory)/allcodecs.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libbpg/libavcodec/allcodecs.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/allcodecs.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/allcodecs.c$(DependSuffix): allcodecs.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/allcodecs.c$(ObjectSuffix) -MF$(IntermediateDirectory)/allcodecs.c$(DependSuffix) -MM allcodecs.c

$(IntermediateDirectory)/allcodecs.c$(PreprocessSuffix): allcodecs.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/allcodecs.c$(PreprocessSuffix) allcodecs.c

$(IntermediateDirectory)/hevc_cabac.c$(ObjectSuffix): hevc_cabac.c $(IntermediateDirectory)/hevc_cabac.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libbpg/libavcodec/hevc_cabac.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hevc_cabac.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hevc_cabac.c$(DependSuffix): hevc_cabac.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hevc_cabac.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hevc_cabac.c$(DependSuffix) -MM hevc_cabac.c

$(IntermediateDirectory)/hevc_cabac.c$(PreprocessSuffix): hevc_cabac.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hevc_cabac.c$(PreprocessSuffix) hevc_cabac.c

$(IntermediateDirectory)/bit_depth_template.c$(ObjectSuffix): bit_depth_template.c $(IntermediateDirectory)/bit_depth_template.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libbpg/libavcodec/bit_depth_template.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bit_depth_template.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bit_depth_template.c$(DependSuffix): bit_depth_template.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bit_depth_template.c$(ObjectSuffix) -MF$(IntermediateDirectory)/bit_depth_template.c$(DependSuffix) -MM bit_depth_template.c

$(IntermediateDirectory)/bit_depth_template.c$(PreprocessSuffix): bit_depth_template.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bit_depth_template.c$(PreprocessSuffix) bit_depth_template.c

$(IntermediateDirectory)/hevc_ps.c$(ObjectSuffix): hevc_ps.c $(IntermediateDirectory)/hevc_ps.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libbpg/libavcodec/hevc_ps.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hevc_ps.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hevc_ps.c$(DependSuffix): hevc_ps.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hevc_ps.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hevc_ps.c$(DependSuffix) -MM hevc_ps.c

$(IntermediateDirectory)/hevc_ps.c$(PreprocessSuffix): hevc_ps.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hevc_ps.c$(PreprocessSuffix) hevc_ps.c

$(IntermediateDirectory)/videodsp.c$(ObjectSuffix): videodsp.c $(IntermediateDirectory)/videodsp.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libbpg/libavcodec/videodsp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/videodsp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/videodsp.c$(DependSuffix): videodsp.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/videodsp.c$(ObjectSuffix) -MF$(IntermediateDirectory)/videodsp.c$(DependSuffix) -MM videodsp.c

$(IntermediateDirectory)/videodsp.c$(PreprocessSuffix): videodsp.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/videodsp.c$(PreprocessSuffix) videodsp.c

$(IntermediateDirectory)/hevcdsp.c$(ObjectSuffix): hevcdsp.c $(IntermediateDirectory)/hevcdsp.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libbpg/libavcodec/hevcdsp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hevcdsp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hevcdsp.c$(DependSuffix): hevcdsp.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hevcdsp.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hevcdsp.c$(DependSuffix) -MM hevcdsp.c

$(IntermediateDirectory)/hevcdsp.c$(PreprocessSuffix): hevcdsp.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hevcdsp.c$(PreprocessSuffix) hevcdsp.c

$(IntermediateDirectory)/hevc_filter.c$(ObjectSuffix): hevc_filter.c $(IntermediateDirectory)/hevc_filter.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libbpg/libavcodec/hevc_filter.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hevc_filter.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hevc_filter.c$(DependSuffix): hevc_filter.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hevc_filter.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hevc_filter.c$(DependSuffix) -MM hevc_filter.c

$(IntermediateDirectory)/hevc_filter.c$(PreprocessSuffix): hevc_filter.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hevc_filter.c$(PreprocessSuffix) hevc_filter.c

$(IntermediateDirectory)/utils.c$(ObjectSuffix): utils.c $(IntermediateDirectory)/utils.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libbpg/libavcodec/utils.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/utils.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/utils.c$(DependSuffix): utils.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/utils.c$(ObjectSuffix) -MF$(IntermediateDirectory)/utils.c$(DependSuffix) -MM utils.c

$(IntermediateDirectory)/utils.c$(PreprocessSuffix): utils.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/utils.c$(PreprocessSuffix) utils.c

$(IntermediateDirectory)/hevc_mvs.c$(ObjectSuffix): hevc_mvs.c $(IntermediateDirectory)/hevc_mvs.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libbpg/libavcodec/hevc_mvs.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hevc_mvs.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hevc_mvs.c$(DependSuffix): hevc_mvs.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hevc_mvs.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hevc_mvs.c$(DependSuffix) -MM hevc_mvs.c

$(IntermediateDirectory)/hevc_mvs.c$(PreprocessSuffix): hevc_mvs.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hevc_mvs.c$(PreprocessSuffix) hevc_mvs.c

$(IntermediateDirectory)/hevcpred.c$(ObjectSuffix): hevcpred.c $(IntermediateDirectory)/hevcpred.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libbpg/libavcodec/hevcpred.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hevcpred.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hevcpred.c$(DependSuffix): hevcpred.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hevcpred.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hevcpred.c$(DependSuffix) -MM hevcpred.c

$(IntermediateDirectory)/hevcpred.c$(PreprocessSuffix): hevcpred.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hevcpred.c$(PreprocessSuffix) hevcpred.c

$(IntermediateDirectory)/hevc_refs.c$(ObjectSuffix): hevc_refs.c $(IntermediateDirectory)/hevc_refs.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libbpg/libavcodec/hevc_refs.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hevc_refs.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hevc_refs.c$(DependSuffix): hevc_refs.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hevc_refs.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hevc_refs.c$(DependSuffix) -MM hevc_refs.c

$(IntermediateDirectory)/hevc_refs.c$(PreprocessSuffix): hevc_refs.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hevc_refs.c$(PreprocessSuffix) hevc_refs.c

$(IntermediateDirectory)/golomb.c$(ObjectSuffix): golomb.c $(IntermediateDirectory)/golomb.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libbpg/libavcodec/golomb.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/golomb.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/golomb.c$(DependSuffix): golomb.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/golomb.c$(ObjectSuffix) -MF$(IntermediateDirectory)/golomb.c$(DependSuffix) -MM golomb.c

$(IntermediateDirectory)/golomb.c$(PreprocessSuffix): golomb.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/golomb.c$(PreprocessSuffix) golomb.c

$(IntermediateDirectory)/hevc_sei.c$(ObjectSuffix): hevc_sei.c $(IntermediateDirectory)/hevc_sei.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libbpg/libavcodec/hevc_sei.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hevc_sei.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hevc_sei.c$(DependSuffix): hevc_sei.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hevc_sei.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hevc_sei.c$(DependSuffix) -MM hevc_sei.c

$(IntermediateDirectory)/hevc_sei.c$(PreprocessSuffix): hevc_sei.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hevc_sei.c$(PreprocessSuffix) hevc_sei.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


