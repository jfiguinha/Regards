##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libVideoFFMpeg
ConfigurationName      :=Release
WorkspacePath          :=/home/figuinha/developpement/Regards
ProjectPath            :=/home/figuinha/developpement/Regards/libVideoFFMpeg
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
Preprocessors          :=$(PreprocessorSwitch)FFMPEG $(PreprocessorSwitch)SDL2 $(PreprocessorSwitch)YUV $(PreprocessorSwitch)OPENCL $(PreprocessorSwitch)OPENGL $(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libVideoFFMpeg.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s 
IncludePath            := $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2  $(IncludeSwitch). $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libTheme $(IncludeSwitch)../libextern/glew-2.1.0/include $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libextern/ffmpeg-4.1 $(IncludeSwitch)../libextern/SDL2-2.0.5/include $(IncludeSwitch)../libWindowMain $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libResource $(IncludeSwitch)../libParameter $(IncludeSwitch)../libFiltre $(IncludeSwitch)../libUtility $(IncludeSwitch)../include $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../Otherlib/wxSVG/include $(IncludeSwitch)../libOpenCL $(IncludeSwitch)../libOpenGL 
IncludePCH             :=  -include ../include/Release/header.h 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                :=$(LibraryPathSwitch)/opt/AMDAPPSDK-3.0/lib/x86_64/sdk/  $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/x86_64-linux-gnu-ar rcu
CXX      := ccache /usr/bin/x86_64-linux-gnu-g++
CC       := ccache /usr/bin/x86_64-linux-gnu-gcc
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread  -fopenmp -std=gnu++11 -Wall -fpermissive -O2 -msse4.1 $(Preprocessors)
CFLAGS   := -Wall  -O2 -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := ccache /usr/bin/x86_64-linux-gnu-as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/SliderVideo.cpp$(ObjectSuffix) $(IntermediateDirectory)/ShowVideo.cpp$(ObjectSuffix) $(IntermediateDirectory)/ffplaycore.cpp$(ObjectSuffix) $(IntermediateDirectory)/VideoControlInterface.cpp$(ObjectSuffix) $(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(ObjectSuffix) $(IntermediateDirectory)/ffmpegToBitmap.cpp$(ObjectSuffix) $(IntermediateDirectory)/VideoControl.cpp$(ObjectSuffix) $(IntermediateDirectory)/ffmpeg_dxva2.cpp$(ObjectSuffix) $(IntermediateDirectory)/VideoControl_soft.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/home/figuinha/developpement/Regards/.build-release/libVideoFFMpeg"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


./Release:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:

# PreCompiled Header
../include/Release/header.h.gch: ../include/Release/header.h
	$(CXX) $(SourceSwitch) ../include/Release/header.h $(PCHCompileFlags) $(CXXFLAGS) $(IncludePath)



##
## Objects
##
$(IntermediateDirectory)/SliderVideo.cpp$(ObjectSuffix): SliderVideo.cpp $(IntermediateDirectory)/SliderVideo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libVideoFFMpeg/SliderVideo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SliderVideo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SliderVideo.cpp$(DependSuffix): SliderVideo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SliderVideo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SliderVideo.cpp$(DependSuffix) -MM SliderVideo.cpp

$(IntermediateDirectory)/SliderVideo.cpp$(PreprocessSuffix): SliderVideo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SliderVideo.cpp$(PreprocessSuffix) SliderVideo.cpp

$(IntermediateDirectory)/ShowVideo.cpp$(ObjectSuffix): ShowVideo.cpp $(IntermediateDirectory)/ShowVideo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libVideoFFMpeg/ShowVideo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ShowVideo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ShowVideo.cpp$(DependSuffix): ShowVideo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ShowVideo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ShowVideo.cpp$(DependSuffix) -MM ShowVideo.cpp

$(IntermediateDirectory)/ShowVideo.cpp$(PreprocessSuffix): ShowVideo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ShowVideo.cpp$(PreprocessSuffix) ShowVideo.cpp

$(IntermediateDirectory)/ffplaycore.cpp$(ObjectSuffix): ffplaycore.cpp $(IntermediateDirectory)/ffplaycore.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libVideoFFMpeg/ffplaycore.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ffplaycore.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ffplaycore.cpp$(DependSuffix): ffplaycore.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ffplaycore.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ffplaycore.cpp$(DependSuffix) -MM ffplaycore.cpp

$(IntermediateDirectory)/ffplaycore.cpp$(PreprocessSuffix): ffplaycore.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ffplaycore.cpp$(PreprocessSuffix) ffplaycore.cpp

$(IntermediateDirectory)/VideoControlInterface.cpp$(ObjectSuffix): VideoControlInterface.cpp $(IntermediateDirectory)/VideoControlInterface.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libVideoFFMpeg/VideoControlInterface.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/VideoControlInterface.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/VideoControlInterface.cpp$(DependSuffix): VideoControlInterface.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/VideoControlInterface.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/VideoControlInterface.cpp$(DependSuffix) -MM VideoControlInterface.cpp

$(IntermediateDirectory)/VideoControlInterface.cpp$(PreprocessSuffix): VideoControlInterface.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/VideoControlInterface.cpp$(PreprocessSuffix) VideoControlInterface.cpp

$(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(ObjectSuffix): RenderBitmapInterfaceOpenGL.cpp $(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libVideoFFMpeg/RenderBitmapInterfaceOpenGL.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(DependSuffix): RenderBitmapInterfaceOpenGL.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(DependSuffix) -MM RenderBitmapInterfaceOpenGL.cpp

$(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(PreprocessSuffix): RenderBitmapInterfaceOpenGL.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RenderBitmapInterfaceOpenGL.cpp$(PreprocessSuffix) RenderBitmapInterfaceOpenGL.cpp

$(IntermediateDirectory)/ffmpegToBitmap.cpp$(ObjectSuffix): ffmpegToBitmap.cpp $(IntermediateDirectory)/ffmpegToBitmap.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libVideoFFMpeg/ffmpegToBitmap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ffmpegToBitmap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ffmpegToBitmap.cpp$(DependSuffix): ffmpegToBitmap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ffmpegToBitmap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ffmpegToBitmap.cpp$(DependSuffix) -MM ffmpegToBitmap.cpp

$(IntermediateDirectory)/ffmpegToBitmap.cpp$(PreprocessSuffix): ffmpegToBitmap.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ffmpegToBitmap.cpp$(PreprocessSuffix) ffmpegToBitmap.cpp

$(IntermediateDirectory)/VideoControl.cpp$(ObjectSuffix): VideoControl.cpp $(IntermediateDirectory)/VideoControl.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libVideoFFMpeg/VideoControl.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/VideoControl.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/VideoControl.cpp$(DependSuffix): VideoControl.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/VideoControl.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/VideoControl.cpp$(DependSuffix) -MM VideoControl.cpp

$(IntermediateDirectory)/VideoControl.cpp$(PreprocessSuffix): VideoControl.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/VideoControl.cpp$(PreprocessSuffix) VideoControl.cpp

$(IntermediateDirectory)/ffmpeg_dxva2.cpp$(ObjectSuffix): ffmpeg_dxva2.cpp $(IntermediateDirectory)/ffmpeg_dxva2.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libVideoFFMpeg/ffmpeg_dxva2.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ffmpeg_dxva2.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ffmpeg_dxva2.cpp$(DependSuffix): ffmpeg_dxva2.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ffmpeg_dxva2.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ffmpeg_dxva2.cpp$(DependSuffix) -MM ffmpeg_dxva2.cpp

$(IntermediateDirectory)/ffmpeg_dxva2.cpp$(PreprocessSuffix): ffmpeg_dxva2.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ffmpeg_dxva2.cpp$(PreprocessSuffix) ffmpeg_dxva2.cpp

$(IntermediateDirectory)/VideoControl_soft.cpp$(ObjectSuffix): VideoControl_soft.cpp $(IntermediateDirectory)/VideoControl_soft.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libVideoFFMpeg/VideoControl_soft.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/VideoControl_soft.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/VideoControl_soft.cpp$(DependSuffix): VideoControl_soft.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/VideoControl_soft.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/VideoControl_soft.cpp$(DependSuffix) -MM VideoControl_soft.cpp

$(IntermediateDirectory)/VideoControl_soft.cpp$(PreprocessSuffix): VideoControl_soft.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/VideoControl_soft.cpp$(PreprocessSuffix) VideoControl_soft.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/
	$(RM) ../include/Release/header.h.gch


