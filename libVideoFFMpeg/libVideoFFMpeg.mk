##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libVideoFFMpeg
ConfigurationName      :=Release
WorkspacePath          :=C:/Regards
ProjectPath            :=C:/Regards/libVideoFFMpeg
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
Preprocessors          :=$(PreprocessorSwitch)FFMPEG $(PreprocessorSwitch)SDL2 $(PreprocessorSwitch)YUV $(PreprocessorSwitch)OPENCL $(PreprocessorSwitch)OPENGL $(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libVideoFFMpeg.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/msys64/mingw64/bin/windres.exe
LinkOptions            :=  -s 
IncludePath            := $(IncludeSwitch)"C:\Program Files (x86)\AMD APP SDK\3.0_mingw\include" $(IncludeSwitch)/mingw64/include/libxml2 $(IncludeSwitch)/mingw64/include/OpenEXR  $(IncludeSwitch). $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libTheme $(IncludeSwitch)../libextern/glew-2.1.0/include $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libextern/ffmpeg-4.1 $(IncludeSwitch)../libextern/SDL2-2.0.5/include $(IncludeSwitch)../libWindowMain $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libResource $(IncludeSwitch)../libParameter $(IncludeSwitch)../libFiltre $(IncludeSwitch)../libUtility $(IncludeSwitch)../include $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../Otherlib/wxSVG/include $(IncludeSwitch)../libOpenCL $(IncludeSwitch)../libOpenGL $(IncludeSwitch)../libextern/OpenCL-Headers 
IncludePCH             :=  -include ../include/Release/header.h 
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
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread  -fopenmp -std=gnu++11 -Wall -fpermissive -O2 -msse4.1 $(Preprocessors)
CFLAGS   := -Wall  -O2 -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := C:/msys64/mingw64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
PATH:=C:\msys64\usr\bin;$PATH
WXWIN:=C:\Regards\libextern\wxwidgets-3.1.2
WXCFG:=lib/mswud
Objects0=$(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(ObjectSuffix) $(IntermediateDirectory)/ffplaycore.cpp$(ObjectSuffix) $(IntermediateDirectory)/VideoControl_soft.cpp$(ObjectSuffix) $(IntermediateDirectory)/VideoControlInterface.cpp$(ObjectSuffix) $(IntermediateDirectory)/SliderVideo.cpp$(ObjectSuffix) $(IntermediateDirectory)/ffmpegToBitmap.cpp$(ObjectSuffix) $(IntermediateDirectory)/VideoControl.cpp$(ObjectSuffix) $(IntermediateDirectory)/ffmpeg_dxva2.cpp$(ObjectSuffix) $(IntermediateDirectory)/ShowVideo.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "C:\Regards/.build-release/libVideoFFMpeg"

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
$(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(ObjectSuffix): RenderBitmapInterfaceOpenGL.cpp $(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/libVideoFFMpeg/RenderBitmapInterfaceOpenGL.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(DependSuffix): RenderBitmapInterfaceOpenGL.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(DependSuffix) -MM RenderBitmapInterfaceOpenGL.cpp

$(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(PreprocessSuffix): RenderBitmapInterfaceOpenGL.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(PreprocessSuffix) RenderBitmapInterfaceOpenGL.cpp

$(IntermediateDirectory)/ffplaycore.cpp$(ObjectSuffix): ffplaycore.cpp $(IntermediateDirectory)/ffplaycore.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/libVideoFFMpeg/ffplaycore.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ffplaycore.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ffplaycore.cpp$(DependSuffix): ffplaycore.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ffplaycore.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ffplaycore.cpp$(DependSuffix) -MM ffplaycore.cpp

$(IntermediateDirectory)/ffplaycore.cpp$(PreprocessSuffix): ffplaycore.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ffplaycore.cpp$(PreprocessSuffix) ffplaycore.cpp

$(IntermediateDirectory)/VideoControl_soft.cpp$(ObjectSuffix): VideoControl_soft.cpp $(IntermediateDirectory)/VideoControl_soft.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/libVideoFFMpeg/VideoControl_soft.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/VideoControl_soft.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/VideoControl_soft.cpp$(DependSuffix): VideoControl_soft.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/VideoControl_soft.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/VideoControl_soft.cpp$(DependSuffix) -MM VideoControl_soft.cpp

$(IntermediateDirectory)/VideoControl_soft.cpp$(PreprocessSuffix): VideoControl_soft.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/VideoControl_soft.cpp$(PreprocessSuffix) VideoControl_soft.cpp

$(IntermediateDirectory)/VideoControlInterface.cpp$(ObjectSuffix): VideoControlInterface.cpp $(IntermediateDirectory)/VideoControlInterface.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/libVideoFFMpeg/VideoControlInterface.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/VideoControlInterface.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/VideoControlInterface.cpp$(DependSuffix): VideoControlInterface.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/VideoControlInterface.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/VideoControlInterface.cpp$(DependSuffix) -MM VideoControlInterface.cpp

$(IntermediateDirectory)/VideoControlInterface.cpp$(PreprocessSuffix): VideoControlInterface.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/VideoControlInterface.cpp$(PreprocessSuffix) VideoControlInterface.cpp

$(IntermediateDirectory)/SliderVideo.cpp$(ObjectSuffix): SliderVideo.cpp $(IntermediateDirectory)/SliderVideo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/libVideoFFMpeg/SliderVideo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SliderVideo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SliderVideo.cpp$(DependSuffix): SliderVideo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SliderVideo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SliderVideo.cpp$(DependSuffix) -MM SliderVideo.cpp

$(IntermediateDirectory)/SliderVideo.cpp$(PreprocessSuffix): SliderVideo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SliderVideo.cpp$(PreprocessSuffix) SliderVideo.cpp

$(IntermediateDirectory)/ffmpegToBitmap.cpp$(ObjectSuffix): ffmpegToBitmap.cpp $(IntermediateDirectory)/ffmpegToBitmap.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/libVideoFFMpeg/ffmpegToBitmap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ffmpegToBitmap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ffmpegToBitmap.cpp$(DependSuffix): ffmpegToBitmap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ffmpegToBitmap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ffmpegToBitmap.cpp$(DependSuffix) -MM ffmpegToBitmap.cpp

$(IntermediateDirectory)/ffmpegToBitmap.cpp$(PreprocessSuffix): ffmpegToBitmap.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ffmpegToBitmap.cpp$(PreprocessSuffix) ffmpegToBitmap.cpp

$(IntermediateDirectory)/VideoControl.cpp$(ObjectSuffix): VideoControl.cpp $(IntermediateDirectory)/VideoControl.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/libVideoFFMpeg/VideoControl.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/VideoControl.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/VideoControl.cpp$(DependSuffix): VideoControl.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/VideoControl.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/VideoControl.cpp$(DependSuffix) -MM VideoControl.cpp

$(IntermediateDirectory)/VideoControl.cpp$(PreprocessSuffix): VideoControl.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/VideoControl.cpp$(PreprocessSuffix) VideoControl.cpp

$(IntermediateDirectory)/ffmpeg_dxva2.cpp$(ObjectSuffix): ffmpeg_dxva2.cpp $(IntermediateDirectory)/ffmpeg_dxva2.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/libVideoFFMpeg/ffmpeg_dxva2.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ffmpeg_dxva2.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ffmpeg_dxva2.cpp$(DependSuffix): ffmpeg_dxva2.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ffmpeg_dxva2.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ffmpeg_dxva2.cpp$(DependSuffix) -MM ffmpeg_dxva2.cpp

$(IntermediateDirectory)/ffmpeg_dxva2.cpp$(PreprocessSuffix): ffmpeg_dxva2.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ffmpeg_dxva2.cpp$(PreprocessSuffix) ffmpeg_dxva2.cpp

$(IntermediateDirectory)/ShowVideo.cpp$(ObjectSuffix): ShowVideo.cpp $(IntermediateDirectory)/ShowVideo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/libVideoFFMpeg/ShowVideo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ShowVideo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ShowVideo.cpp$(DependSuffix): ShowVideo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ShowVideo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ShowVideo.cpp$(DependSuffix) -MM ShowVideo.cpp

$(IntermediateDirectory)/ShowVideo.cpp$(PreprocessSuffix): ShowVideo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ShowVideo.cpp$(PreprocessSuffix) ShowVideo.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/
	$(RM) ../include/Release/header.h.gch


