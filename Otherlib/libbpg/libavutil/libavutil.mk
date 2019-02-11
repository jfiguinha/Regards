##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libavutil
ConfigurationName      :=Release
WorkspacePath          :=C:/Regards
ProjectPath            :=C:/Regards/Otherlib/libbpg/libavutil
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
Preprocessors          :=$(PreprocessorSwitch)USE_VAR_BIT_DEPTH $(PreprocessorSwitch)_ISOC99_SOURCE $(PreprocessorSwitch)HAVE_AV_CONFIG_H $(PreprocessorSwitch)_FILE_OFFSET_BITS=64 $(PreprocessorSwitch)_LARGEFILE_SOURCE $(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libavutil.txt"
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
CC       := C:/msys64/usr/bin/ccache.exe  C:/msys64/mingw64/bin/gcc.exe
CXXFLAGS :=  -O2 -msse4.1 $(Preprocessors)
CFLAGS   := -std=c99 -Wall -fPIC -pthread -O2 -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := C:/msys64/mingw64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
PATH:=C:\msys64\usr\bin;$PATH
WXWIN:=C:\Regards\libextern\wxwidgets-3.1.2
WXCFG:=lib/mswud
Objects0=$(IntermediateDirectory)/pixdesc.c$(ObjectSuffix) $(IntermediateDirectory)/frame.c$(ObjectSuffix) $(IntermediateDirectory)/buffer.c$(ObjectSuffix) $(IntermediateDirectory)/log2_tab.c$(ObjectSuffix) $(IntermediateDirectory)/mem.c$(ObjectSuffix) $(IntermediateDirectory)/md5.c$(ObjectSuffix) 



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
	@echo rebuilt > "C:\Regards/.build-release/libavutil"

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Release"


./Release:
	@$(MakeDirCommand) "./Release"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/pixdesc.c$(ObjectSuffix): pixdesc.c $(IntermediateDirectory)/pixdesc.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Regards/Otherlib/libbpg/libavutil/pixdesc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/pixdesc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/pixdesc.c$(DependSuffix): pixdesc.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/pixdesc.c$(ObjectSuffix) -MF$(IntermediateDirectory)/pixdesc.c$(DependSuffix) -MM pixdesc.c

$(IntermediateDirectory)/pixdesc.c$(PreprocessSuffix): pixdesc.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/pixdesc.c$(PreprocessSuffix) pixdesc.c

$(IntermediateDirectory)/frame.c$(ObjectSuffix): frame.c $(IntermediateDirectory)/frame.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Regards/Otherlib/libbpg/libavutil/frame.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/frame.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/frame.c$(DependSuffix): frame.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/frame.c$(ObjectSuffix) -MF$(IntermediateDirectory)/frame.c$(DependSuffix) -MM frame.c

$(IntermediateDirectory)/frame.c$(PreprocessSuffix): frame.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/frame.c$(PreprocessSuffix) frame.c

$(IntermediateDirectory)/buffer.c$(ObjectSuffix): buffer.c $(IntermediateDirectory)/buffer.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Regards/Otherlib/libbpg/libavutil/buffer.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/buffer.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/buffer.c$(DependSuffix): buffer.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/buffer.c$(ObjectSuffix) -MF$(IntermediateDirectory)/buffer.c$(DependSuffix) -MM buffer.c

$(IntermediateDirectory)/buffer.c$(PreprocessSuffix): buffer.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/buffer.c$(PreprocessSuffix) buffer.c

$(IntermediateDirectory)/log2_tab.c$(ObjectSuffix): log2_tab.c $(IntermediateDirectory)/log2_tab.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Regards/Otherlib/libbpg/libavutil/log2_tab.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/log2_tab.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/log2_tab.c$(DependSuffix): log2_tab.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/log2_tab.c$(ObjectSuffix) -MF$(IntermediateDirectory)/log2_tab.c$(DependSuffix) -MM log2_tab.c

$(IntermediateDirectory)/log2_tab.c$(PreprocessSuffix): log2_tab.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/log2_tab.c$(PreprocessSuffix) log2_tab.c

$(IntermediateDirectory)/mem.c$(ObjectSuffix): mem.c $(IntermediateDirectory)/mem.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Regards/Otherlib/libbpg/libavutil/mem.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mem.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mem.c$(DependSuffix): mem.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/mem.c$(ObjectSuffix) -MF$(IntermediateDirectory)/mem.c$(DependSuffix) -MM mem.c

$(IntermediateDirectory)/mem.c$(PreprocessSuffix): mem.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mem.c$(PreprocessSuffix) mem.c

$(IntermediateDirectory)/md5.c$(ObjectSuffix): md5.c $(IntermediateDirectory)/md5.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Regards/Otherlib/libbpg/libavutil/md5.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/md5.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/md5.c$(DependSuffix): md5.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/md5.c$(ObjectSuffix) -MF$(IntermediateDirectory)/md5.c$(DependSuffix) -MM md5.c

$(IntermediateDirectory)/md5.c$(PreprocessSuffix): md5.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/md5.c$(PreprocessSuffix) md5.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


