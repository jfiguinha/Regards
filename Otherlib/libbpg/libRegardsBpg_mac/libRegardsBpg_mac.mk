##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libRegardsBpg_mac
ConfigurationName      :=Release
WorkspacePath          :=/Users/jacques/developpement/codelite/Regards
ProjectPath            :=/Users/jacques/developpement/codelite/Regards/Otherlib/libbpg/libRegardsBpg_mac
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha jacques
Date                   :=11/01/2019
CodeLitePath           :="/Users/jacques/Library/Application Support/codelite"
LinkerName             :=/usr/local/opt/ccache/bin/ccache /usr/local/opt/llvm/bin/clang++ -stdlib=libc++ -mmacosx-version-min=10.10
SharedObjectLinkerName :=/usr/local/opt/ccache/bin/ccache /usr/local/opt/llvm/bin/clang++ -dynamiclib -fPIC
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
OutputFile             :=$(IntermediateDirectory)/libRegardsBpg.dylib
Preprocessors          :=$(PreprocessorSwitch)USE_VAR_BIT_DEPTH $(PreprocessorSwitch)_ISOC99_SOURCE $(PreprocessorSwitch)HAVE_AV_CONFIG_H $(PreprocessorSwitch)_FILE_OFFSET_BITS=64 $(PreprocessorSwitch)_LARGEFILE_SOURCE $(PreprocessorSwitch)CONFIG_BPG_VERSION=0.9.7 $(PreprocessorSwitch)USE_X265 $(PreprocessorSwitch)_DEBUG $(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libRegardsBpg_mac.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -Os
IncludePath            := $(IncludeSwitch)/usr/local/opt/cairo/include $(IncludeSwitch)/usr/local/opt/openexr/include/OpenEXR $(IncludeSwitch)/usr/local/opt/ilmbase/include/openexr $(IncludeSwitch)/usr/local/opt/libxml2/include/libxml2 $(IncludeSwitch)/usr/local/opt/libexif/include  $(IncludeSwitch). $(IncludeSwitch)../ $(IncludeSwitch)../libbpg $(IncludeSwitch)../../../libextern/x265_2.5/source $(IncludeSwitch)../../../libextern/x265_2.5/build/msys/8bit $(IncludeSwitch)../../../libextern/dlib-19.7/dlib/external/libpng $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)x265 $(LibrarySwitch)bpg $(LibrarySwitch)avcodec $(LibrarySwitch)avutil $(LibrarySwitch)gomp $(LibrarySwitch)z 
ArLibs                 :=  "x265" "bpg" "avcodec" "avutil" "gomp" "z" 
LibPath                :=$(LibraryPathSwitch)/usr/local/opt/llvm/lib $(LibraryPathSwitch)/usr/local/opt/openexr/lib $(LibraryPathSwitch)/usr/local/opt/ilmbase/lib $(LibraryPathSwitch)/usr/local/opt/glib/lib $(LibraryPathSwitch)/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/lib $(LibraryPathSwitch)/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library $(LibraryPathSwitch)/usr/lib/system  $(LibraryPathSwitch). $(LibraryPathSwitch)../../../libextern/x265_2.5/source $(LibraryPathSwitch)../libbpg/Release $(LibraryPathSwitch)../libavutil/Release $(LibraryPathSwitch)../libavcodec/Release 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/local/opt/ccache/bin/ccache /usr/local/opt/llvm/bin/clang++ -stdlib=libc++ -mmacosx-version-min=10.10 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/
CC       := /usr/local/opt/ccache/bin/ccache /usr/local/opt/llvm/bin/clang -mmacosx-version-min=10.10 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/
CXXFLAGS := -fopenmp -std=gnu++11 -Wall -fPIC -O2 -msse4.1 $(Preprocessors)
CFLAGS   := -std=c99 -Wall -fPIC -O2 -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := /usr//bin/as


##
## User defined environment variables
##
CodeLiteDir:=/Applications/codelite.app/Contents/SharedSupport/
Objects0=$(IntermediateDirectory)/up_libRegardsBpg_bpgenc.c$(ObjectSuffix) $(IntermediateDirectory)/up_libRegardsBpg_DllBpg.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_libRegardsBpg_x265_glue.c$(ObjectSuffix) $(IntermediateDirectory)/up_libRegardsBpg_bpgdec.c$(ObjectSuffix) 



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
	$(SharedObjectLinkerName) $(OutputSwitch)$(OutputFile) $(Objects) $(LibPath) $(Libs) $(LinkOptions)
	@$(MakeDirCommand) "/Users/jacques/developpement/codelite/Regards/.build-release"
	@echo rebuilt > "/Users/jacques/developpement/codelite/Regards/.build-release/libRegardsBpg_mac"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


$(IntermediateDirectory)/.d:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_libRegardsBpg_bpgenc.c$(ObjectSuffix): ../libRegardsBpg/bpgenc.c $(IntermediateDirectory)/up_libRegardsBpg_bpgenc.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/Users/jacques/developpement/codelite/Regards/Otherlib/libbpg/libRegardsBpg/bpgenc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libRegardsBpg_bpgenc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libRegardsBpg_bpgenc.c$(DependSuffix): ../libRegardsBpg/bpgenc.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libRegardsBpg_bpgenc.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libRegardsBpg_bpgenc.c$(DependSuffix) -MM ../libRegardsBpg/bpgenc.c

$(IntermediateDirectory)/up_libRegardsBpg_bpgenc.c$(PreprocessSuffix): ../libRegardsBpg/bpgenc.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libRegardsBpg_bpgenc.c$(PreprocessSuffix) ../libRegardsBpg/bpgenc.c

$(IntermediateDirectory)/up_libRegardsBpg_DllBpg.cpp$(ObjectSuffix): ../libRegardsBpg/DllBpg.cpp $(IntermediateDirectory)/up_libRegardsBpg_DllBpg.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/codelite/Regards/Otherlib/libbpg/libRegardsBpg/DllBpg.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libRegardsBpg_DllBpg.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libRegardsBpg_DllBpg.cpp$(DependSuffix): ../libRegardsBpg/DllBpg.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libRegardsBpg_DllBpg.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libRegardsBpg_DllBpg.cpp$(DependSuffix) -MM ../libRegardsBpg/DllBpg.cpp

$(IntermediateDirectory)/up_libRegardsBpg_DllBpg.cpp$(PreprocessSuffix): ../libRegardsBpg/DllBpg.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libRegardsBpg_DllBpg.cpp$(PreprocessSuffix) ../libRegardsBpg/DllBpg.cpp

$(IntermediateDirectory)/up_libRegardsBpg_x265_glue.c$(ObjectSuffix): ../libRegardsBpg/x265_glue.c $(IntermediateDirectory)/up_libRegardsBpg_x265_glue.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/Users/jacques/developpement/codelite/Regards/Otherlib/libbpg/libRegardsBpg/x265_glue.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libRegardsBpg_x265_glue.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libRegardsBpg_x265_glue.c$(DependSuffix): ../libRegardsBpg/x265_glue.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libRegardsBpg_x265_glue.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libRegardsBpg_x265_glue.c$(DependSuffix) -MM ../libRegardsBpg/x265_glue.c

$(IntermediateDirectory)/up_libRegardsBpg_x265_glue.c$(PreprocessSuffix): ../libRegardsBpg/x265_glue.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libRegardsBpg_x265_glue.c$(PreprocessSuffix) ../libRegardsBpg/x265_glue.c

$(IntermediateDirectory)/up_libRegardsBpg_bpgdec.c$(ObjectSuffix): ../libRegardsBpg/bpgdec.c $(IntermediateDirectory)/up_libRegardsBpg_bpgdec.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/Users/jacques/developpement/codelite/Regards/Otherlib/libbpg/libRegardsBpg/bpgdec.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_libRegardsBpg_bpgdec.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_libRegardsBpg_bpgdec.c$(DependSuffix): ../libRegardsBpg/bpgdec.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_libRegardsBpg_bpgdec.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_libRegardsBpg_bpgdec.c$(DependSuffix) -MM ../libRegardsBpg/bpgdec.c

$(IntermediateDirectory)/up_libRegardsBpg_bpgdec.c$(PreprocessSuffix): ../libRegardsBpg/bpgdec.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_libRegardsBpg_bpgdec.c$(PreprocessSuffix) ../libRegardsBpg/bpgdec.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


