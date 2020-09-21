##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libPicture
ConfigurationName      :=Release
WorkspacePath          :=C:/developpement/git_gcc/Regards
ProjectPath            :=C:/developpement/git_gcc/Regards/libPicture
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=jfigu
Date                   :=20/09/2020
CodeLitePath           :="C:/Program Files/CodeLite"
LinkerName             :=C:/msys64/mingw64/bin/g++.exe
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
Preprocessors          :=$(PreprocessorSwitch)LIBRAW $(PreprocessorSwitch)LIBHEIC $(PreprocessorSwitch)LIBBPG $(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libPicture.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/msys64/mingw64/bin/windres.exe
LinkOptions            :=  -s 
IncludePath            := $(IncludeSwitch)/mingw64/x86_64-w64-mingw32/include/gdiplus $(IncludeSwitch)$$HOME/ffmpeg_build/include  $(IncludeSwitch). $(IncludeSwitch)../libPiccante $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../libOpenCL $(IncludeSwitch)../libResource $(IncludeSwitch)../include $(IncludeSwitch)../Otherlib/RegardsRaw $(IncludeSwitch)../Otherlib/libbpg/Bpg/libRegardsBpg $(IncludeSwitch)../CxImage $(IncludeSwitch)../libFiltre $(IncludeSwitch)../libUtility $(IncludeSwitch)../libVideoThumbnail $(IncludeSwitch)../libExif $(IncludeSwitch)../libDialog $(IncludeSwitch)../Otherlib/libbpg/libRegardsBpg $(IncludeSwitch)../Otherlib/wxSVG/include $(IncludeSwitch)../libextern/libjpeg-turbo-1.5.1 $(IncludeSwitch)../libextern/libwebp-0.6.0/src $(IncludeSwitch)../libextern/libde265-master/libde265 $(IncludeSwitch)../libextern/heif-master/srcs/api/reader $(IncludeSwitch)../libextern/heif-master/srcs/api/common $(IncludeSwitch)../libextern/libde265-master $(IncludeSwitch)../Otherlib/libavif/include $(IncludeSwitch)../Otherlib/wxPDFDoc/include $(IncludeSwitch)../libextern/poppler_windows-master/poppler $(IncludeSwitch)../libextern/poppler_windows-master/cpp $(IncludeSwitch)../libextern/FreeImage3180/pkg/mingw-w64-x86_64-freeimage/mingw64/include $(IncludeSwitch)../libextern/FreeImage3180/FreeImage/Dist $(IncludeSwitch)../Otherlib 
IncludePCH             :=  -include ../include/Release/header.h 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                :=$(LibraryPathSwitch)$$HOME/ffmpeg_build/lib $(LibraryPathSwitch)$$HOME/ffmpeg_build/x64/mingw/lib  $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/msys64/mingw64/bin/ar.exe rcu
CXX      := C:/msys64/mingw64/bin/g++.exe
CC       := C:/msys64/mingw64/bin/gcc.exe
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread  -fopenmp -std=gnu++14 -Wall -O2 -fopenmp -msse4.1 $(Preprocessors)
CFLAGS   :=  -Wall  -O2 -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := C:/msys64/mingw64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
PATH:=C:\msys64\usr\bin;c:\developpement\git_gcc\Regards\libextern\wxWidgets-master\include;c:\developpement\git_gcc\Regards\libextern\wxWidgets-master\lib\wx\include\msw-unicode-static-3.1;$PATH
Objects0=$(IntermediateDirectory)/ScaleThumbnail.cpp$(ObjectSuffix) $(IntermediateDirectory)/raw.cpp$(ObjectSuffix) $(IntermediateDirectory)/libPicture.cpp$(ObjectSuffix) $(IntermediateDirectory)/pfm.cpp$(ObjectSuffix) $(IntermediateDirectory)/PictureData.cpp$(ObjectSuffix) $(IntermediateDirectory)/Heic.cpp$(ObjectSuffix) $(IntermediateDirectory)/avif_heif.cpp$(ObjectSuffix) $(IntermediateDirectory)/avif.cpp$(ObjectSuffix) 



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
	$(AR) $(ArchiveOutputSwitch)$(OutputFile) @$(ObjectsFileList)
	@$(MakeDirCommand) "C:\developpement\git_gcc\Regards/.build-release"
	@echo rebuilt > "C:\developpement\git_gcc\Regards/.build-release/libPicture"

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Release"


./Release:
	@$(MakeDirCommand) "./Release"

PreBuild:

# PreCompiled Header
../include/Release/header.h.gch: ../include/Release/header.h
	$(CXX) $(SourceSwitch) ../include/Release/header.h $(PCHCompileFlags) $(CXXFLAGS) $(IncludePath)



##
## Objects
##
$(IntermediateDirectory)/ScaleThumbnail.cpp$(ObjectSuffix): ScaleThumbnail.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ScaleThumbnail.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ScaleThumbnail.cpp$(DependSuffix) -MM ScaleThumbnail.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libPicture/ScaleThumbnail.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ScaleThumbnail.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ScaleThumbnail.cpp$(PreprocessSuffix): ScaleThumbnail.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ScaleThumbnail.cpp$(PreprocessSuffix) ScaleThumbnail.cpp

$(IntermediateDirectory)/raw.cpp$(ObjectSuffix): raw.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/raw.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/raw.cpp$(DependSuffix) -MM raw.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libPicture/raw.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/raw.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/raw.cpp$(PreprocessSuffix): raw.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/raw.cpp$(PreprocessSuffix) raw.cpp

$(IntermediateDirectory)/libPicture.cpp$(ObjectSuffix): libPicture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/libPicture.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/libPicture.cpp$(DependSuffix) -MM libPicture.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libPicture/libPicture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/libPicture.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/libPicture.cpp$(PreprocessSuffix): libPicture.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/libPicture.cpp$(PreprocessSuffix) libPicture.cpp

$(IntermediateDirectory)/pfm.cpp$(ObjectSuffix): pfm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/pfm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/pfm.cpp$(DependSuffix) -MM pfm.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libPicture/pfm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/pfm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/pfm.cpp$(PreprocessSuffix): pfm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/pfm.cpp$(PreprocessSuffix) pfm.cpp

$(IntermediateDirectory)/PictureData.cpp$(ObjectSuffix): PictureData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PictureData.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PictureData.cpp$(DependSuffix) -MM PictureData.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libPicture/PictureData.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PictureData.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PictureData.cpp$(PreprocessSuffix): PictureData.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PictureData.cpp$(PreprocessSuffix) PictureData.cpp

$(IntermediateDirectory)/Heic.cpp$(ObjectSuffix): Heic.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Heic.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Heic.cpp$(DependSuffix) -MM Heic.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libPicture/Heic.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Heic.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Heic.cpp$(PreprocessSuffix): Heic.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Heic.cpp$(PreprocessSuffix) Heic.cpp

$(IntermediateDirectory)/avif_heif.cpp$(ObjectSuffix): avif_heif.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/avif_heif.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/avif_heif.cpp$(DependSuffix) -MM avif_heif.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libPicture/avif_heif.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/avif_heif.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/avif_heif.cpp$(PreprocessSuffix): avif_heif.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/avif_heif.cpp$(PreprocessSuffix) avif_heif.cpp

$(IntermediateDirectory)/avif.cpp$(ObjectSuffix): avif.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/avif.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/avif.cpp$(DependSuffix) -MM avif.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git_gcc/Regards/libPicture/avif.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/avif.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/avif.cpp$(PreprocessSuffix): avif.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/avif.cpp$(PreprocessSuffix) avif.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/
	$(RM) ../include/Release/header.h.gch


