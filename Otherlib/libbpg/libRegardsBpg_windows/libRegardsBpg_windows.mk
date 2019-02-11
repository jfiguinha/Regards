##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libRegardsBpg_windows
ConfigurationName      :=Release
WorkspacePath          :=C:/Regards
ProjectPath            :=C:/Regards/Otherlib/libbpg/libRegardsBpg_windows
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
OutputFile             :=$(IntermediateDirectory)/libRegardsBpg.dll
Preprocessors          :=$(PreprocessorSwitch)USE_VAR_BIT_DEPTH $(PreprocessorSwitch)_ISOC99_SOURCE $(PreprocessorSwitch)HAVE_AV_CONFIG_H $(PreprocessorSwitch)_FILE_OFFSET_BITS=64 $(PreprocessorSwitch)_LARGEFILE_SOURCE $(PreprocessorSwitch)DLLFORMATBPG_EXPORTS $(PreprocessorSwitch)CONFIG_BPG_VERSION=0.9.7 $(PreprocessorSwitch)USE_X265 $(PreprocessorSwitch)WIN32 $(PreprocessorSwitch)NDEBUG $(PreprocessorSwitch)_WINDOWS $(PreprocessorSwitch)_USRDLL $(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libRegardsBpg_windows.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/msys64/mingw64/bin/windres.exe
LinkOptions            :=  -Os
IncludePath            := $(IncludeSwitch)"C:\Program Files (x86)\AMD APP SDK\3.0_mingw\include" $(IncludeSwitch)/mingw64/include/libxml2 $(IncludeSwitch)/mingw64/include/OpenEXR  $(IncludeSwitch). $(IncludeSwitch)../ $(IncludeSwitch)../libbpg $(IncludeSwitch)../../../libextern/x265_2.5/source $(IncludeSwitch)../../../libextern/x265_2.5/build/msys/8bit $(IncludeSwitch)../../../libextern/dlib-19.7/dlib/external/libpng $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)x265 $(LibrarySwitch)bpg $(LibrarySwitch)avcodec $(LibrarySwitch)avutil $(LibrarySwitch)gomp $(LibrarySwitch)z 
ArLibs                 :=  "x265" "bpg" "avcodec" "avutil" "gomp" "z" 
LibPath                :=$(LibraryPathSwitch)"C:\Program Files (x86)\AMD APP SDK\3.0_mingw\lib\x86_64"  $(LibraryPathSwitch). $(LibraryPathSwitch)../../../libextern/x265_2.5/build/msys/8bit $(LibraryPathSwitch)../libbpg/Release $(LibraryPathSwitch)../libavutil/Release $(LibraryPathSwitch)../libavcodec/Release 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/msys64/mingw64/bin/ar.exe rcu
CXX      := C:/msys64/usr/bin/ccache.exe C:/msys64/mingw64/bin/g++.exe
CC       := C:/msys64/usr/bin/ccache.exe  C:/msys64/mingw64/bin/gcc.exe
CXXFLAGS := -fopenmp -std=gnu++11 -Wall -fPIC -pthread -O2 -msse4.1 $(Preprocessors)
CFLAGS   := -std=c99 -Wall -fPIC -pthread -fopenmp  -O2 -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := C:/msys64/mingw64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
PATH:=C:\msys64\usr\bin;$PATH
WXWIN:=C:\Regards\libextern\wxwidgets-3.1.2
WXCFG:=lib/mswud
Objects0=$(IntermediateDirectory)/up_libRegardsBpg_DllBpg.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libRegardsBpg_bpgenc.c$(ObjectSuffix) $(IntermediateDirectory)/up_libRegardsBpg_bpgdec.c$(ObjectSuffix) $(IntermediateDirectory)/up_libRegardsBpg_x265_glue.c$(ObjectSuffix) $(IntermediateDirectory)/up_libRegardsBpg_dllmain.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(SharedObjectLinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)
	@$(MakeDirCommand) "C:\Regards/.build-release"
	@echo rebuilt > "C:\Regards/.build-release/libRegardsBpg_windows"

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Release"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Release"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_libRegardsBpg_DllBpg.cpp$(ObjectSuffix): ../libRegardsBpg/DllBpg.cpp $(IntermediateDirectory)/up_libRegardsBpg_DllBpg.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/libbpg/libRegardsBpg/DllBpg.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libRegardsBpg_DllBpg.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libRegardsBpg_DllBpg.cpp$(DependSuffix): ../libRegardsBpg/DllBpg.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libRegardsBpg_DllBpg.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libRegardsBpg_DllBpg.cpp$(DependSuffix) -MM ../libRegardsBpg/DllBpg.cpp

$(IntermediateDirectory)/up_libRegardsBpg_DllBpg.cpp$(PreprocessSuffix): ../libRegardsBpg/DllBpg.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libRegardsBpg_DllBpg.cpp$(PreprocessSuffix) ../libRegardsBpg/DllBpg.cpp

$(IntermediateDirectory)/up_libRegardsBpg_bpgenc.c$(ObjectSuffix): ../libRegardsBpg/bpgenc.c $(IntermediateDirectory)/up_libRegardsBpg_bpgenc.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Regards/Otherlib/libbpg/libRegardsBpg/bpgenc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libRegardsBpg_bpgenc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libRegardsBpg_bpgenc.c$(DependSuffix): ../libRegardsBpg/bpgenc.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libRegardsBpg_bpgenc.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libRegardsBpg_bpgenc.c$(DependSuffix) -MM ../libRegardsBpg/bpgenc.c

$(IntermediateDirectory)/up_libRegardsBpg_bpgenc.c$(PreprocessSuffix): ../libRegardsBpg/bpgenc.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libRegardsBpg_bpgenc.c$(PreprocessSuffix) ../libRegardsBpg/bpgenc.c

$(IntermediateDirectory)/up_libRegardsBpg_bpgdec.c$(ObjectSuffix): ../libRegardsBpg/bpgdec.c $(IntermediateDirectory)/up_libRegardsBpg_bpgdec.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Regards/Otherlib/libbpg/libRegardsBpg/bpgdec.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libRegardsBpg_bpgdec.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libRegardsBpg_bpgdec.c$(DependSuffix): ../libRegardsBpg/bpgdec.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libRegardsBpg_bpgdec.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libRegardsBpg_bpgdec.c$(DependSuffix) -MM ../libRegardsBpg/bpgdec.c

$(IntermediateDirectory)/up_libRegardsBpg_bpgdec.c$(PreprocessSuffix): ../libRegardsBpg/bpgdec.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libRegardsBpg_bpgdec.c$(PreprocessSuffix) ../libRegardsBpg/bpgdec.c

$(IntermediateDirectory)/up_libRegardsBpg_x265_glue.c$(ObjectSuffix): ../libRegardsBpg/x265_glue.c $(IntermediateDirectory)/up_libRegardsBpg_x265_glue.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Regards/Otherlib/libbpg/libRegardsBpg/x265_glue.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libRegardsBpg_x265_glue.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libRegardsBpg_x265_glue.c$(DependSuffix): ../libRegardsBpg/x265_glue.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libRegardsBpg_x265_glue.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libRegardsBpg_x265_glue.c$(DependSuffix) -MM ../libRegardsBpg/x265_glue.c

$(IntermediateDirectory)/up_libRegardsBpg_x265_glue.c$(PreprocessSuffix): ../libRegardsBpg/x265_glue.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libRegardsBpg_x265_glue.c$(PreprocessSuffix) ../libRegardsBpg/x265_glue.c

$(IntermediateDirectory)/up_libRegardsBpg_dllmain.cpp$(ObjectSuffix): ../libRegardsBpg/dllmain.cpp $(IntermediateDirectory)/up_libRegardsBpg_dllmain.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/libbpg/libRegardsBpg/dllmain.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libRegardsBpg_dllmain.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libRegardsBpg_dllmain.cpp$(DependSuffix): ../libRegardsBpg/dllmain.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libRegardsBpg_dllmain.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libRegardsBpg_dllmain.cpp$(DependSuffix) -MM ../libRegardsBpg/dllmain.cpp

$(IntermediateDirectory)/up_libRegardsBpg_dllmain.cpp$(PreprocessSuffix): ../libRegardsBpg/dllmain.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libRegardsBpg_dllmain.cpp$(PreprocessSuffix) ../libRegardsBpg/dllmain.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


