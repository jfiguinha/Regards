##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libbpg
ConfigurationName      :=Release
WorkspacePath          :=/home/figuinha/developpement/Regards
ProjectPath            :=/home/figuinha/developpement/Regards/Otherlib/libbpg/libbpg
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=14/02/19
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
OutputFile             :=$(IntermediateDirectory)/$(ProjectName).a
Preprocessors          :=$(PreprocessorSwitch)USE_VAR_BIT_DEPTH $(PreprocessorSwitch)_ISOC99_SOURCE $(PreprocessorSwitch)HAVE_AV_CONFIG_H $(PreprocessorSwitch)_FILE_OFFSET_BITS=64 $(PreprocessorSwitch)_LARGEFILE_SOURCE $(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libbpg.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2 $(IncludeSwitch)/usr/include/libexif  $(IncludeSwitch). $(IncludeSwitch)../ $(IncludeSwitch). 
IncludePCH             := 
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
CXXFLAGS :=  -O2 -msse4.1 $(Preprocessors)
CFLAGS   := -std=c99 -Wall -fPIC -pthread -O2 -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/yasm


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwtran.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwio.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngset.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrutil.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwrite.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngmem.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_png.c$(ObjectSuffix) $(IntermediateDirectory)/libbpg.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngerror.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngget.c$(ObjectSuffix) \
	$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngtrans.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngpread.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngread.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwutil.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrio.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrtran.c$(ObjectSuffix) 



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
	@echo rebuilt > "/home/figuinha/developpement/Regards/.build-release/libbpg"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


./Release:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwtran.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngwtran.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwtran.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libpng/pngwtran.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwtran.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwtran.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngwtran.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwtran.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwtran.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/pngwtran.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwtran.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngwtran.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwtran.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/pngwtran.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwio.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngwio.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwio.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libpng/pngwio.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwio.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwio.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngwio.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwio.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwio.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/pngwio.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwio.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngwio.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwio.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/pngwio.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngset.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngset.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngset.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libpng/pngset.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngset.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngset.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngset.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngset.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngset.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/pngset.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngset.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngset.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngset.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/pngset.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrutil.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngrutil.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrutil.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libpng/pngrutil.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrutil.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrutil.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngrutil.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrutil.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrutil.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/pngrutil.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrutil.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngrutil.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrutil.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/pngrutil.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwrite.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngwrite.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwrite.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libpng/pngwrite.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwrite.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwrite.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngwrite.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwrite.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwrite.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/pngwrite.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwrite.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngwrite.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwrite.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/pngwrite.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngmem.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngmem.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngmem.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libpng/pngmem.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngmem.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngmem.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngmem.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngmem.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngmem.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/pngmem.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngmem.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngmem.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngmem.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/pngmem.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_png.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/png.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_png.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libpng/png.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_png.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_png.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/png.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_png.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_png.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/png.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_png.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/png.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_png.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/png.c

$(IntermediateDirectory)/libbpg.c$(ObjectSuffix): libbpg.c $(IntermediateDirectory)/libbpg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libbpg/libbpg/libbpg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/libbpg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/libbpg.c$(DependSuffix): libbpg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/libbpg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/libbpg.c$(DependSuffix) -MM libbpg.c

$(IntermediateDirectory)/libbpg.c$(PreprocessSuffix): libbpg.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/libbpg.c$(PreprocessSuffix) libbpg.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngerror.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngerror.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngerror.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libpng/pngerror.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngerror.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngerror.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngerror.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngerror.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngerror.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/pngerror.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngerror.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngerror.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngerror.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/pngerror.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngget.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngget.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngget.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libpng/pngget.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngget.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngget.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngget.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngget.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngget.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/pngget.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngget.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngget.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngget.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/pngget.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngtrans.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngtrans.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngtrans.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libpng/pngtrans.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngtrans.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngtrans.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngtrans.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngtrans.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngtrans.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/pngtrans.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngtrans.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngtrans.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngtrans.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/pngtrans.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngpread.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngpread.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngpread.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libpng/pngpread.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngpread.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngpread.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngpread.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngpread.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngpread.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/pngpread.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngpread.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngpread.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngpread.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/pngpread.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngread.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngread.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngread.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libpng/pngread.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngread.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngread.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngread.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngread.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngread.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/pngread.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngread.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngread.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngread.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/pngread.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwutil.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngwutil.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwutil.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libpng/pngwutil.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwutil.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwutil.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngwutil.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwutil.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwutil.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/pngwutil.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwutil.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngwutil.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwutil.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/pngwutil.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrio.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngrio.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrio.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libpng/pngrio.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrio.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrio.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngrio.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrio.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrio.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/pngrio.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrio.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngrio.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrio.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/pngrio.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrtran.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngrtran.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrtran.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/libextern/dlib-19.7/dlib/external/libpng/pngrtran.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrtran.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrtran.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngrtran.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrtran.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrtran.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/pngrtran.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrtran.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngrtran.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrtran.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/pngrtran.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


