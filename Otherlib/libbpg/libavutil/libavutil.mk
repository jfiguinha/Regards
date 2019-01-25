##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libavutil
ConfigurationName      :=Release
WorkspacePath          :=/home/figuinha/developpement/Regards
ProjectPath            :=/home/figuinha/developpement/Regards/Otherlib/libbpg/libavutil
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=24/01/19
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
ObjectsFileList        :="libavutil.txt"
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
Objects0=$(IntermediateDirectory)/mem.c$(ObjectSuffix) $(IntermediateDirectory)/md5.c$(ObjectSuffix) $(IntermediateDirectory)/pixdesc.c$(ObjectSuffix) $(IntermediateDirectory)/log2_tab.c$(ObjectSuffix) $(IntermediateDirectory)/buffer.c$(ObjectSuffix) $(IntermediateDirectory)/frame.c$(ObjectSuffix) 



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
	@echo rebuilt > "/home/figuinha/developpement/Regards/.build-release/libavutil"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


./Release:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/mem.c$(ObjectSuffix): mem.c $(IntermediateDirectory)/mem.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libbpg/libavutil/mem.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mem.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mem.c$(DependSuffix): mem.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/mem.c$(ObjectSuffix) -MF$(IntermediateDirectory)/mem.c$(DependSuffix) -MM mem.c

$(IntermediateDirectory)/mem.c$(PreprocessSuffix): mem.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mem.c$(PreprocessSuffix) mem.c

$(IntermediateDirectory)/md5.c$(ObjectSuffix): md5.c $(IntermediateDirectory)/md5.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libbpg/libavutil/md5.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/md5.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/md5.c$(DependSuffix): md5.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/md5.c$(ObjectSuffix) -MF$(IntermediateDirectory)/md5.c$(DependSuffix) -MM md5.c

$(IntermediateDirectory)/md5.c$(PreprocessSuffix): md5.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/md5.c$(PreprocessSuffix) md5.c

$(IntermediateDirectory)/pixdesc.c$(ObjectSuffix): pixdesc.c $(IntermediateDirectory)/pixdesc.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libbpg/libavutil/pixdesc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/pixdesc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/pixdesc.c$(DependSuffix): pixdesc.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/pixdesc.c$(ObjectSuffix) -MF$(IntermediateDirectory)/pixdesc.c$(DependSuffix) -MM pixdesc.c

$(IntermediateDirectory)/pixdesc.c$(PreprocessSuffix): pixdesc.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/pixdesc.c$(PreprocessSuffix) pixdesc.c

$(IntermediateDirectory)/log2_tab.c$(ObjectSuffix): log2_tab.c $(IntermediateDirectory)/log2_tab.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libbpg/libavutil/log2_tab.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/log2_tab.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/log2_tab.c$(DependSuffix): log2_tab.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/log2_tab.c$(ObjectSuffix) -MF$(IntermediateDirectory)/log2_tab.c$(DependSuffix) -MM log2_tab.c

$(IntermediateDirectory)/log2_tab.c$(PreprocessSuffix): log2_tab.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/log2_tab.c$(PreprocessSuffix) log2_tab.c

$(IntermediateDirectory)/buffer.c$(ObjectSuffix): buffer.c $(IntermediateDirectory)/buffer.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libbpg/libavutil/buffer.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/buffer.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/buffer.c$(DependSuffix): buffer.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/buffer.c$(ObjectSuffix) -MF$(IntermediateDirectory)/buffer.c$(DependSuffix) -MM buffer.c

$(IntermediateDirectory)/buffer.c$(PreprocessSuffix): buffer.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/buffer.c$(PreprocessSuffix) buffer.c

$(IntermediateDirectory)/frame.c$(ObjectSuffix): frame.c $(IntermediateDirectory)/frame.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libbpg/libavutil/frame.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/frame.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/frame.c$(DependSuffix): frame.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/frame.c$(ObjectSuffix) -MF$(IntermediateDirectory)/frame.c$(DependSuffix) -MM frame.c

$(IntermediateDirectory)/frame.c$(PreprocessSuffix): frame.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/frame.c$(PreprocessSuffix) frame.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


