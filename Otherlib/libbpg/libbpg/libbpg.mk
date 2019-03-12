##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libbpg
ConfigurationName      :=Debug
WorkspacePath          :=C:/developpement/git/Regards
ProjectPath            :=C:/developpement/git/Regards/Otherlib/libbpg/libbpg
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=jfigu
Date                   :=11/03/2019
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
ObjectsFileList        :="libbpg.txt"
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
CFLAGS   := -std=c99 -Wall -fPIC -pthread -g $(Preprocessors)
ASFLAGS  := 
AS       := C:/msys64/usr/bin/ccache.exe C:/msys64/mingw64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
PATH:=C:\msys64\usr\bin;$PATH
WXWIN:=C:\Regards\libextern\wxwidgets-3.1.2
WXCFG:=lib/mswud
Objects0=$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwtran.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwrite.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwio.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngtrans.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwutil.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngset.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrutil.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrio.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngerror.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngget.c$(ObjectSuffix) \
	$(IntermediateDirectory)/libbpg.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngmem.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_png.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngpread.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngread.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrtran.c$(ObjectSuffix) 



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
	@echo rebuilt > "C:\developpement\git\Regards/.build-debug/libbpg"

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Debug"


./Debug:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwtran.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngwtran.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwtran.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/libextern/dlib-19.7/dlib/external/libpng/pngwtran.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwtran.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwtran.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngwtran.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwtran.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwtran.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/pngwtran.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwtran.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngwtran.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwtran.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/pngwtran.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwrite.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngwrite.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwrite.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/libextern/dlib-19.7/dlib/external/libpng/pngwrite.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwrite.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwrite.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngwrite.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwrite.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwrite.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/pngwrite.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwrite.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngwrite.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwrite.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/pngwrite.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwio.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngwio.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwio.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/libextern/dlib-19.7/dlib/external/libpng/pngwio.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwio.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwio.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngwio.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwio.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwio.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/pngwio.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwio.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngwio.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwio.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/pngwio.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngtrans.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngtrans.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngtrans.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/libextern/dlib-19.7/dlib/external/libpng/pngtrans.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngtrans.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngtrans.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngtrans.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngtrans.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngtrans.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/pngtrans.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngtrans.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngtrans.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngtrans.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/pngtrans.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwutil.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngwutil.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwutil.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/libextern/dlib-19.7/dlib/external/libpng/pngwutil.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwutil.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwutil.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngwutil.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwutil.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwutil.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/pngwutil.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwutil.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngwutil.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngwutil.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/pngwutil.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngset.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngset.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngset.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/libextern/dlib-19.7/dlib/external/libpng/pngset.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngset.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngset.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngset.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngset.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngset.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/pngset.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngset.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngset.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngset.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/pngset.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrutil.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngrutil.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrutil.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/libextern/dlib-19.7/dlib/external/libpng/pngrutil.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrutil.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrutil.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngrutil.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrutil.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrutil.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/pngrutil.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrutil.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngrutil.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrutil.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/pngrutil.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrio.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngrio.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrio.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/libextern/dlib-19.7/dlib/external/libpng/pngrio.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrio.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrio.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngrio.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrio.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrio.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/pngrio.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrio.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngrio.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrio.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/pngrio.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngerror.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngerror.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngerror.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/libextern/dlib-19.7/dlib/external/libpng/pngerror.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngerror.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngerror.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngerror.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngerror.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngerror.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/pngerror.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngerror.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngerror.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngerror.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/pngerror.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngget.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngget.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngget.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/libextern/dlib-19.7/dlib/external/libpng/pngget.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngget.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngget.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngget.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngget.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngget.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/pngget.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngget.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngget.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngget.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/pngget.c

$(IntermediateDirectory)/libbpg.c$(ObjectSuffix): libbpg.c $(IntermediateDirectory)/libbpg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/libbpg/libbpg/libbpg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/libbpg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/libbpg.c$(DependSuffix): libbpg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/libbpg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/libbpg.c$(DependSuffix) -MM libbpg.c

$(IntermediateDirectory)/libbpg.c$(PreprocessSuffix): libbpg.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/libbpg.c$(PreprocessSuffix) libbpg.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngmem.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngmem.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngmem.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/libextern/dlib-19.7/dlib/external/libpng/pngmem.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngmem.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngmem.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngmem.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngmem.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngmem.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/pngmem.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngmem.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngmem.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngmem.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/pngmem.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_png.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/png.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_png.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/libextern/dlib-19.7/dlib/external/libpng/png.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_png.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_png.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/png.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_png.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_png.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/png.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_png.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/png.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_png.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/png.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngpread.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngpread.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngpread.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/libextern/dlib-19.7/dlib/external/libpng/pngpread.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngpread.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngpread.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngpread.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngpread.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngpread.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/pngpread.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngpread.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngpread.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngpread.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/pngpread.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngread.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngread.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngread.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/libextern/dlib-19.7/dlib/external/libpng/pngread.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngread.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngread.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngread.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngread.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngread.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/pngread.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngread.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngread.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngread.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/pngread.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrtran.c$(ObjectSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngrtran.c $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrtran.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/developpement/git/Regards/libextern/dlib-19.7/dlib/external/libpng/pngrtran.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrtran.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrtran.c$(DependSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngrtran.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrtran.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrtran.c$(DependSuffix) -MM ../../../libextern/dlib-19.7/dlib/external/libpng/pngrtran.c

$(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrtran.c$(PreprocessSuffix): ../../../libextern/dlib-19.7/dlib/external/libpng/pngrtran.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_libextern_dlib-19.7_dlib_external_libpng_pngrtran.c$(PreprocessSuffix) ../../../libextern/dlib-19.7/dlib/external/libpng/pngrtran.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


