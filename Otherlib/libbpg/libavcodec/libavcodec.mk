##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libavcodec
ConfigurationName      :=Debug
WorkspacePath          :=C:/developpement/git/Regards
ProjectPath            :=C:/developpement/git/Regards/Otherlib/libbpg/libavcodec
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
Preprocessors          :=$(PreprocessorSwitch)USE_VAR_BIT_DEPTH $(PreprocessorSwitch)_ISOC99_SOURCE $(PreprocessorSwitch)HAVE_AV_CONFIG_H $(PreprocessorSwitch)_FILE_OFFSET_BITS=64 $(PreprocessorSwitch)_LARGEFILE_SOURCE 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libavcodec.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/msys64/mingw64/bin/windres.exe
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)"C:\Program Files (x86)\AMD APP SDK\3.0_mingw\include" $(IncludeSwitch)/mingw64/include/libxml2 $(IncludeSwitch)/mingw64/include/OpenEXR  $(IncludeSwitch). $(IncludeSwitch)../ $(IncludeSwitch). 
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
CXXFLAGS :=  -g $(Preprocessors)
CFLAGS   := -std=c99 -Wall -fPIC -pthread -g  $(Preprocessors)
ASFLAGS  := 
AS       := C:/msys64/usr/bin/ccache.exe C:/msys64/mingw64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
PATH:=C:\msys64\usr\bin;$PATH
WXWIN:=C:\Regards\libextern\wxwidgets-3.1.2
WXCFG:=lib/mswud
Objects0=$(IntermediateDirectory)/golomb.c$(ObjectSuffix) $(IntermediateDirectory)/hevc_cabac.c$(ObjectSuffix) $(IntermediateDirectory)/hevc_filter.c$(ObjectSuffix) $(IntermediateDirectory)/utils.c$(ObjectSuffix) $(IntermediateDirectory)/cabac.c$(ObjectSuffix) $(IntermediateDirectory)/hevcpred.c$(ObjectSuffix) $(IntermediateDirectory)/allcodecs.c$(ObjectSuffix) $(IntermediateDirectory)/bit_depth_template.c$(ObjectSuffix) $(IntermediateDirectory)/hevcdsp.c$(ObjectSuffix) $(IntermediateDirectory)/videodsp.c$(ObjectSuffix) \
	$(IntermediateDirectory)/hevc_mvs.c$(ObjectSuffix) $(IntermediateDirectory)/hevc_ps.c$(ObjectSuffix) $(IntermediateDirectory)/hevc.c$(ObjectSuffix) $(IntermediateDirectory)/hevc_refs.c$(ObjectSuffix) $(IntermediateDirectory)/hevc_sei.c$(ObjectSuffix) 



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
	@echo rebuilt > "C:\developpement\git\Regards/.build-debug/libavcodec"

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Debug"


./Debug:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/golomb.c$(ObjectSuffix): golomb.c $(IntermediateDirectory)/golomb.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/libbpg/libavcodec/golomb.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/golomb.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/golomb.c$(DependSuffix): golomb.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/golomb.c$(ObjectSuffix) -MF$(IntermediateDirectory)/golomb.c$(DependSuffix) -MM golomb.c

$(IntermediateDirectory)/golomb.c$(PreprocessSuffix): golomb.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/golomb.c$(PreprocessSuffix) golomb.c

$(IntermediateDirectory)/hevc_cabac.c$(ObjectSuffix): hevc_cabac.c $(IntermediateDirectory)/hevc_cabac.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/libbpg/libavcodec/hevc_cabac.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hevc_cabac.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hevc_cabac.c$(DependSuffix): hevc_cabac.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hevc_cabac.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hevc_cabac.c$(DependSuffix) -MM hevc_cabac.c

$(IntermediateDirectory)/hevc_cabac.c$(PreprocessSuffix): hevc_cabac.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hevc_cabac.c$(PreprocessSuffix) hevc_cabac.c

$(IntermediateDirectory)/hevc_filter.c$(ObjectSuffix): hevc_filter.c $(IntermediateDirectory)/hevc_filter.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/libbpg/libavcodec/hevc_filter.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hevc_filter.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hevc_filter.c$(DependSuffix): hevc_filter.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hevc_filter.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hevc_filter.c$(DependSuffix) -MM hevc_filter.c

$(IntermediateDirectory)/hevc_filter.c$(PreprocessSuffix): hevc_filter.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hevc_filter.c$(PreprocessSuffix) hevc_filter.c

$(IntermediateDirectory)/utils.c$(ObjectSuffix): utils.c $(IntermediateDirectory)/utils.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/libbpg/libavcodec/utils.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/utils.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/utils.c$(DependSuffix): utils.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/utils.c$(ObjectSuffix) -MF$(IntermediateDirectory)/utils.c$(DependSuffix) -MM utils.c

$(IntermediateDirectory)/utils.c$(PreprocessSuffix): utils.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/utils.c$(PreprocessSuffix) utils.c

$(IntermediateDirectory)/cabac.c$(ObjectSuffix): cabac.c $(IntermediateDirectory)/cabac.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/libbpg/libavcodec/cabac.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/cabac.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/cabac.c$(DependSuffix): cabac.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/cabac.c$(ObjectSuffix) -MF$(IntermediateDirectory)/cabac.c$(DependSuffix) -MM cabac.c

$(IntermediateDirectory)/cabac.c$(PreprocessSuffix): cabac.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/cabac.c$(PreprocessSuffix) cabac.c

$(IntermediateDirectory)/hevcpred.c$(ObjectSuffix): hevcpred.c $(IntermediateDirectory)/hevcpred.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/libbpg/libavcodec/hevcpred.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hevcpred.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hevcpred.c$(DependSuffix): hevcpred.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hevcpred.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hevcpred.c$(DependSuffix) -MM hevcpred.c

$(IntermediateDirectory)/hevcpred.c$(PreprocessSuffix): hevcpred.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hevcpred.c$(PreprocessSuffix) hevcpred.c

$(IntermediateDirectory)/allcodecs.c$(ObjectSuffix): allcodecs.c $(IntermediateDirectory)/allcodecs.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/libbpg/libavcodec/allcodecs.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/allcodecs.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/allcodecs.c$(DependSuffix): allcodecs.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/allcodecs.c$(ObjectSuffix) -MF$(IntermediateDirectory)/allcodecs.c$(DependSuffix) -MM allcodecs.c

$(IntermediateDirectory)/allcodecs.c$(PreprocessSuffix): allcodecs.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/allcodecs.c$(PreprocessSuffix) allcodecs.c

$(IntermediateDirectory)/bit_depth_template.c$(ObjectSuffix): bit_depth_template.c $(IntermediateDirectory)/bit_depth_template.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/libbpg/libavcodec/bit_depth_template.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bit_depth_template.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bit_depth_template.c$(DependSuffix): bit_depth_template.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bit_depth_template.c$(ObjectSuffix) -MF$(IntermediateDirectory)/bit_depth_template.c$(DependSuffix) -MM bit_depth_template.c

$(IntermediateDirectory)/bit_depth_template.c$(PreprocessSuffix): bit_depth_template.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bit_depth_template.c$(PreprocessSuffix) bit_depth_template.c

$(IntermediateDirectory)/hevcdsp.c$(ObjectSuffix): hevcdsp.c $(IntermediateDirectory)/hevcdsp.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/libbpg/libavcodec/hevcdsp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hevcdsp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hevcdsp.c$(DependSuffix): hevcdsp.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hevcdsp.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hevcdsp.c$(DependSuffix) -MM hevcdsp.c

$(IntermediateDirectory)/hevcdsp.c$(PreprocessSuffix): hevcdsp.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hevcdsp.c$(PreprocessSuffix) hevcdsp.c

$(IntermediateDirectory)/videodsp.c$(ObjectSuffix): videodsp.c $(IntermediateDirectory)/videodsp.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/libbpg/libavcodec/videodsp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/videodsp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/videodsp.c$(DependSuffix): videodsp.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/videodsp.c$(ObjectSuffix) -MF$(IntermediateDirectory)/videodsp.c$(DependSuffix) -MM videodsp.c

$(IntermediateDirectory)/videodsp.c$(PreprocessSuffix): videodsp.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/videodsp.c$(PreprocessSuffix) videodsp.c

$(IntermediateDirectory)/hevc_mvs.c$(ObjectSuffix): hevc_mvs.c $(IntermediateDirectory)/hevc_mvs.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/libbpg/libavcodec/hevc_mvs.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hevc_mvs.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hevc_mvs.c$(DependSuffix): hevc_mvs.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hevc_mvs.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hevc_mvs.c$(DependSuffix) -MM hevc_mvs.c

$(IntermediateDirectory)/hevc_mvs.c$(PreprocessSuffix): hevc_mvs.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hevc_mvs.c$(PreprocessSuffix) hevc_mvs.c

$(IntermediateDirectory)/hevc_ps.c$(ObjectSuffix): hevc_ps.c $(IntermediateDirectory)/hevc_ps.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/libbpg/libavcodec/hevc_ps.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hevc_ps.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hevc_ps.c$(DependSuffix): hevc_ps.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hevc_ps.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hevc_ps.c$(DependSuffix) -MM hevc_ps.c

$(IntermediateDirectory)/hevc_ps.c$(PreprocessSuffix): hevc_ps.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hevc_ps.c$(PreprocessSuffix) hevc_ps.c

$(IntermediateDirectory)/hevc.c$(ObjectSuffix): hevc.c $(IntermediateDirectory)/hevc.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/libbpg/libavcodec/hevc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hevc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hevc.c$(DependSuffix): hevc.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hevc.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hevc.c$(DependSuffix) -MM hevc.c

$(IntermediateDirectory)/hevc.c$(PreprocessSuffix): hevc.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hevc.c$(PreprocessSuffix) hevc.c

$(IntermediateDirectory)/hevc_refs.c$(ObjectSuffix): hevc_refs.c $(IntermediateDirectory)/hevc_refs.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/libbpg/libavcodec/hevc_refs.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hevc_refs.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hevc_refs.c$(DependSuffix): hevc_refs.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hevc_refs.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hevc_refs.c$(DependSuffix) -MM hevc_refs.c

$(IntermediateDirectory)/hevc_refs.c$(PreprocessSuffix): hevc_refs.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hevc_refs.c$(PreprocessSuffix) hevc_refs.c

$(IntermediateDirectory)/hevc_sei.c$(ObjectSuffix): hevc_sei.c $(IntermediateDirectory)/hevc_sei.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/libbpg/libavcodec/hevc_sei.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hevc_sei.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hevc_sei.c$(DependSuffix): hevc_sei.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hevc_sei.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hevc_sei.c$(DependSuffix) -MM hevc_sei.c

$(IntermediateDirectory)/hevc_sei.c$(PreprocessSuffix): hevc_sei.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hevc_sei.c$(PreprocessSuffix) hevc_sei.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


