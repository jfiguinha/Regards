##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libFiltre
ConfigurationName      :=Release
WorkspaceConfiguration :=Release
WorkspacePath          :=/home/figuinha/developpement/git/Regards
ProjectPath            :=/home/figuinha/developpement/git/Regards/libFiltre
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=18/03/24
CodeLitePath           :=/home/figuinha/.codelite
LinkerName             :=/bin/clang++
SharedObjectLinkerName :=/bin/clang++ -shared -fPIC
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
OutputDirectory        :=$(IntermediateDirectory)
OutputFile             :=$(IntermediateDirectory)/$(ProjectName).a
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libFiltre.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s
IncludePath            := $(IncludeSwitch)$${HOME}/developpement/git/Regards/libextern/oneapi-tbb-2021.11.0/include $(IncludeSwitch)$${HOME}/developpement/git/Regards/libextern/ffmpeg-master/include $(IncludeSwitch)$${HOME}/developpement/git/Regards/libextern/vcpkg-2022.08.15/installed/x64-linux/include $(IncludeSwitch)$${HOME}/developpement/git/Regards/libextern/vcpkg/installed/x64-linux/include/Imath $(IncludeSwitch)$${HOME}/developpement/git/Regards/libextern/vcpkg/installed/x64-linux/include/libde265 $(IncludeSwitch)$${HOME}/developpement/git/Regards/libextern/vcpkg/installed/x64-linux/include/poppler $(IncludeSwitch)$${HOME}/developpement/git/Regards/libextern/vcpkg/installed/x64-linux/include/poppler/cpp $(IncludeSwitch)$${HOME}/developpement/git/Regards/libextern/vcpkg/installed/x64-linux/include $(IncludeSwitch)$${HOME}/ffmpeg_build/include $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/glib-2.0/include $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include $(IncludeSwitch)/usr/include/harfbuzz  $(IncludeSwitch). $(IncludeSwitch)../include $(IncludeSwitch)../libOpenGL $(IncludeSwitch)../libParameter $(IncludeSwitch)../libAppleFunction $(IncludeSwitch)../libextern/glew-2.1.0/include $(IncludeSwitch)../CxImage $(IncludeSwitch)../libUtility $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../libOpenCL $(IncludeSwitch)../libResource $(IncludeSwitch)../libControl $(IncludeSwitch)../Otherlib/wxSVG/include $(IncludeSwitch)../libPicture $(IncludeSwitch)../libextern/fftw-3.3.8/api $(IncludeSwitch)../libextern/libOpenCL/include $(IncludeSwitch)../libOpenCLEngine $(IncludeSwitch)../libDeepLearning $(IncludeSwitch)../libextern/LibRaw-0.20.0 $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libextern/ffmpeg-master $(IncludeSwitch)../libextern/ffmpeg-master/include 
IncludePCH             :=  -include ../include/Release/header.h 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                :=$(LibraryPathSwitch)$${HOME}/developpement/git/Regards/libextern/ffmpeg-master/lib $(LibraryPathSwitch)$${HOME}/developpement/git/Regards/libextern/oneapi-tbb-2021.11.0/lib/intel64/gcc4.8 $(LibraryPathSwitch)$${HOME}/developpement/git/Regards/libextern/vcpkg-2022.08.15/installed/x64-linux/lib $(LibraryPathSwitch)$${HOME}/developpement/git/Regards/libextern/vcpkg/installed/x64-linux/lib $(LibraryPathSwitch)$${HOME}/developpement/git/Regards/libextern/vcpkg/installed/x64-linux/lib/manual-link/opencv4_thirdparty $(LibraryPathSwitch)$${HOME}/ffmpeg_build/lib $(LibraryPathSwitch)/usr/lib/x86_64-linux-gnu  $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overridden using an environment variable
##
AR       := /bin/llvm-ar rcu
CXX      := /bin/clang++ -fopenmp
CC       := /bin/clang
CXXFLAGS := $(shell ../libextern/wxconfig_cpp.sh) -Wall -pthread -std=gnu++14 -Wno-narrowing -O3 $(Preprocessors)
CFLAGS   :=  -Wall  -O3 $(Preprocessors)
ASFLAGS  := 
AS       := /bin/llvm-as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/FilterData.cpp$(ObjectSuffix) $(IntermediateDirectory)/Filtre.cpp$(ObjectSuffix) $(IntermediateDirectory)/FiltreEffet.cpp$(ObjectSuffix) $(IntermediateDirectory)/FiltreEffetCPU.cpp$(ObjectSuffix) $(IntermediateDirectory)/OpenCLEffect.cpp$(ObjectSuffix) $(IntermediateDirectory)/bilateralFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/Color.cpp$(ObjectSuffix) $(IntermediateDirectory)/VignetteFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/MosaicFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/LensFlareFilter.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Stylization.cpp$(ObjectSuffix) $(IntermediateDirectory)/FilterWindowParam.cpp$(ObjectSuffix) $(IntermediateDirectory)/circle.cpp$(ObjectSuffix) $(IntermediateDirectory)/CloudsFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/moviestreaminfo.cpp$(ObjectSuffix) $(IntermediateDirectory)/GaussianBlurFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/SolarisationFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/PhotoFiltreFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/LensFlare.cpp$(ObjectSuffix) $(IntermediateDirectory)/Line.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/MotionBlur.cpp$(ObjectSuffix) $(IntermediateDirectory)/CropFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/Crop.cpp$(ObjectSuffix) $(IntermediateDirectory)/ColorBalanceFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/Wave.cpp$(ObjectSuffix) $(IntermediateDirectory)/PencilSketch.cpp$(ObjectSuffix) $(IntermediateDirectory)/MeanShift.cpp$(ObjectSuffix) $(IntermediateDirectory)/Draw.cpp$(ObjectSuffix) $(IntermediateDirectory)/NlmeansFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/Selection.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/DecodeRaw.cpp$(ObjectSuffix) $(IntermediateDirectory)/VideoFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/AudioVideoFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/PosterisationFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/PerlinNoise.cpp$(ObjectSuffix) $(IntermediateDirectory)/LensDistortion.cpp$(ObjectSuffix) $(IntermediateDirectory)/SharpenMaskingFilter.cpp$(ObjectSuffix) 

Objects1=$(IntermediateDirectory)/panorama.cpp$(ObjectSuffix) $(IntermediateDirectory)/EdgePreservingFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/SwirlFilter.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/DetailEnhance.cpp$(ObjectSuffix) $(IntermediateDirectory)/CartoonFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/BokehFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/RotateFreeFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/OilPaintingFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/hqdn3dFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/MotionBlurFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/videostream.cpp$(ObjectSuffix) $(IntermediateDirectory)/WaveFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/BrightAndContrastFilter.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/BlurFilter.cpp$(ObjectSuffix) 



Objects=$(Objects0) $(Objects1) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(IntermediateDirectory) $(OutputFile)

$(OutputFile): $(Objects)
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	@echo $(Objects1) >> $(ObjectsFileList)
	$(AR) $(ArchiveOutputSwitch)$(OutputFile) @$(ObjectsFileList)
	@$(MakeDirCommand) "/home/figuinha/developpement/git/Regards/.build-release"
	@echo rebuilt > "/home/figuinha/developpement/git/Regards/.build-release/libFiltre"

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
$(IntermediateDirectory)/FilterData.cpp$(ObjectSuffix): FilterData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FilterData.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FilterData.cpp$(DependSuffix) -MM FilterData.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/FilterData.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FilterData.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FilterData.cpp$(PreprocessSuffix): FilterData.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FilterData.cpp$(PreprocessSuffix) FilterData.cpp

$(IntermediateDirectory)/Filtre.cpp$(ObjectSuffix): Filtre.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Filtre.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Filtre.cpp$(DependSuffix) -MM Filtre.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/Filtre.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Filtre.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Filtre.cpp$(PreprocessSuffix): Filtre.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Filtre.cpp$(PreprocessSuffix) Filtre.cpp

$(IntermediateDirectory)/FiltreEffet.cpp$(ObjectSuffix): FiltreEffet.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FiltreEffet.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FiltreEffet.cpp$(DependSuffix) -MM FiltreEffet.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/FiltreEffet.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FiltreEffet.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FiltreEffet.cpp$(PreprocessSuffix): FiltreEffet.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FiltreEffet.cpp$(PreprocessSuffix) FiltreEffet.cpp

$(IntermediateDirectory)/FiltreEffetCPU.cpp$(ObjectSuffix): FiltreEffetCPU.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FiltreEffetCPU.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FiltreEffetCPU.cpp$(DependSuffix) -MM FiltreEffetCPU.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/FiltreEffetCPU.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FiltreEffetCPU.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FiltreEffetCPU.cpp$(PreprocessSuffix): FiltreEffetCPU.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FiltreEffetCPU.cpp$(PreprocessSuffix) FiltreEffetCPU.cpp

$(IntermediateDirectory)/OpenCLEffect.cpp$(ObjectSuffix): OpenCLEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/OpenCLEffect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/OpenCLEffect.cpp$(DependSuffix) -MM OpenCLEffect.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/OpenCLEffect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/OpenCLEffect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OpenCLEffect.cpp$(PreprocessSuffix): OpenCLEffect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OpenCLEffect.cpp$(PreprocessSuffix) OpenCLEffect.cpp

$(IntermediateDirectory)/bilateralFilter.cpp$(ObjectSuffix): bilateralFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bilateralFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bilateralFilter.cpp$(DependSuffix) -MM bilateralFilter.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/bilateralFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bilateralFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bilateralFilter.cpp$(PreprocessSuffix): bilateralFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bilateralFilter.cpp$(PreprocessSuffix) bilateralFilter.cpp

$(IntermediateDirectory)/Color.cpp$(ObjectSuffix): Color.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Color.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Color.cpp$(DependSuffix) -MM Color.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/Color.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Color.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Color.cpp$(PreprocessSuffix): Color.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Color.cpp$(PreprocessSuffix) Color.cpp

$(IntermediateDirectory)/VignetteFilter.cpp$(ObjectSuffix): VignetteFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/VignetteFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/VignetteFilter.cpp$(DependSuffix) -MM VignetteFilter.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/VignetteFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/VignetteFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/VignetteFilter.cpp$(PreprocessSuffix): VignetteFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/VignetteFilter.cpp$(PreprocessSuffix) VignetteFilter.cpp

$(IntermediateDirectory)/MosaicFilter.cpp$(ObjectSuffix): MosaicFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MosaicFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MosaicFilter.cpp$(DependSuffix) -MM MosaicFilter.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/MosaicFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MosaicFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MosaicFilter.cpp$(PreprocessSuffix): MosaicFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MosaicFilter.cpp$(PreprocessSuffix) MosaicFilter.cpp

$(IntermediateDirectory)/LensFlareFilter.cpp$(ObjectSuffix): LensFlareFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LensFlareFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LensFlareFilter.cpp$(DependSuffix) -MM LensFlareFilter.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/LensFlareFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LensFlareFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LensFlareFilter.cpp$(PreprocessSuffix): LensFlareFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LensFlareFilter.cpp$(PreprocessSuffix) LensFlareFilter.cpp

$(IntermediateDirectory)/Stylization.cpp$(ObjectSuffix): Stylization.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Stylization.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Stylization.cpp$(DependSuffix) -MM Stylization.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/Stylization.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Stylization.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Stylization.cpp$(PreprocessSuffix): Stylization.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Stylization.cpp$(PreprocessSuffix) Stylization.cpp

$(IntermediateDirectory)/FilterWindowParam.cpp$(ObjectSuffix): FilterWindowParam.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FilterWindowParam.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FilterWindowParam.cpp$(DependSuffix) -MM FilterWindowParam.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/FilterWindowParam.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FilterWindowParam.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FilterWindowParam.cpp$(PreprocessSuffix): FilterWindowParam.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FilterWindowParam.cpp$(PreprocessSuffix) FilterWindowParam.cpp

$(IntermediateDirectory)/circle.cpp$(ObjectSuffix): circle.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/circle.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/circle.cpp$(DependSuffix) -MM circle.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/circle.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/circle.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/circle.cpp$(PreprocessSuffix): circle.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/circle.cpp$(PreprocessSuffix) circle.cpp

$(IntermediateDirectory)/CloudsFilter.cpp$(ObjectSuffix): CloudsFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CloudsFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CloudsFilter.cpp$(DependSuffix) -MM CloudsFilter.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/CloudsFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CloudsFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CloudsFilter.cpp$(PreprocessSuffix): CloudsFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CloudsFilter.cpp$(PreprocessSuffix) CloudsFilter.cpp

$(IntermediateDirectory)/moviestreaminfo.cpp$(ObjectSuffix): moviestreaminfo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/moviestreaminfo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/moviestreaminfo.cpp$(DependSuffix) -MM moviestreaminfo.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/moviestreaminfo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/moviestreaminfo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/moviestreaminfo.cpp$(PreprocessSuffix): moviestreaminfo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/moviestreaminfo.cpp$(PreprocessSuffix) moviestreaminfo.cpp

$(IntermediateDirectory)/GaussianBlurFilter.cpp$(ObjectSuffix): GaussianBlurFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GaussianBlurFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GaussianBlurFilter.cpp$(DependSuffix) -MM GaussianBlurFilter.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/GaussianBlurFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GaussianBlurFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GaussianBlurFilter.cpp$(PreprocessSuffix): GaussianBlurFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GaussianBlurFilter.cpp$(PreprocessSuffix) GaussianBlurFilter.cpp

$(IntermediateDirectory)/SolarisationFilter.cpp$(ObjectSuffix): SolarisationFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SolarisationFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SolarisationFilter.cpp$(DependSuffix) -MM SolarisationFilter.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/SolarisationFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SolarisationFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SolarisationFilter.cpp$(PreprocessSuffix): SolarisationFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SolarisationFilter.cpp$(PreprocessSuffix) SolarisationFilter.cpp

$(IntermediateDirectory)/PhotoFiltreFilter.cpp$(ObjectSuffix): PhotoFiltreFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PhotoFiltreFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PhotoFiltreFilter.cpp$(DependSuffix) -MM PhotoFiltreFilter.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/PhotoFiltreFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PhotoFiltreFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PhotoFiltreFilter.cpp$(PreprocessSuffix): PhotoFiltreFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PhotoFiltreFilter.cpp$(PreprocessSuffix) PhotoFiltreFilter.cpp

$(IntermediateDirectory)/LensFlare.cpp$(ObjectSuffix): LensFlare.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LensFlare.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LensFlare.cpp$(DependSuffix) -MM LensFlare.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/LensFlare.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LensFlare.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LensFlare.cpp$(PreprocessSuffix): LensFlare.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LensFlare.cpp$(PreprocessSuffix) LensFlare.cpp

$(IntermediateDirectory)/Line.cpp$(ObjectSuffix): Line.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Line.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Line.cpp$(DependSuffix) -MM Line.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/Line.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Line.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Line.cpp$(PreprocessSuffix): Line.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Line.cpp$(PreprocessSuffix) Line.cpp

$(IntermediateDirectory)/MotionBlur.cpp$(ObjectSuffix): MotionBlur.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MotionBlur.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MotionBlur.cpp$(DependSuffix) -MM MotionBlur.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/MotionBlur.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MotionBlur.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MotionBlur.cpp$(PreprocessSuffix): MotionBlur.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MotionBlur.cpp$(PreprocessSuffix) MotionBlur.cpp

$(IntermediateDirectory)/CropFilter.cpp$(ObjectSuffix): CropFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CropFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CropFilter.cpp$(DependSuffix) -MM CropFilter.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/CropFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CropFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CropFilter.cpp$(PreprocessSuffix): CropFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CropFilter.cpp$(PreprocessSuffix) CropFilter.cpp

$(IntermediateDirectory)/Crop.cpp$(ObjectSuffix): Crop.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Crop.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Crop.cpp$(DependSuffix) -MM Crop.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/Crop.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Crop.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Crop.cpp$(PreprocessSuffix): Crop.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Crop.cpp$(PreprocessSuffix) Crop.cpp

$(IntermediateDirectory)/ColorBalanceFilter.cpp$(ObjectSuffix): ColorBalanceFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ColorBalanceFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ColorBalanceFilter.cpp$(DependSuffix) -MM ColorBalanceFilter.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/ColorBalanceFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ColorBalanceFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ColorBalanceFilter.cpp$(PreprocessSuffix): ColorBalanceFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ColorBalanceFilter.cpp$(PreprocessSuffix) ColorBalanceFilter.cpp

$(IntermediateDirectory)/Wave.cpp$(ObjectSuffix): Wave.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Wave.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Wave.cpp$(DependSuffix) -MM Wave.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/Wave.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Wave.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Wave.cpp$(PreprocessSuffix): Wave.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Wave.cpp$(PreprocessSuffix) Wave.cpp

$(IntermediateDirectory)/PencilSketch.cpp$(ObjectSuffix): PencilSketch.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PencilSketch.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PencilSketch.cpp$(DependSuffix) -MM PencilSketch.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/PencilSketch.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PencilSketch.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PencilSketch.cpp$(PreprocessSuffix): PencilSketch.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PencilSketch.cpp$(PreprocessSuffix) PencilSketch.cpp

$(IntermediateDirectory)/MeanShift.cpp$(ObjectSuffix): MeanShift.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MeanShift.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MeanShift.cpp$(DependSuffix) -MM MeanShift.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/MeanShift.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MeanShift.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MeanShift.cpp$(PreprocessSuffix): MeanShift.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MeanShift.cpp$(PreprocessSuffix) MeanShift.cpp

$(IntermediateDirectory)/Draw.cpp$(ObjectSuffix): Draw.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Draw.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Draw.cpp$(DependSuffix) -MM Draw.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/Draw.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Draw.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Draw.cpp$(PreprocessSuffix): Draw.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Draw.cpp$(PreprocessSuffix) Draw.cpp

$(IntermediateDirectory)/NlmeansFilter.cpp$(ObjectSuffix): NlmeansFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/NlmeansFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/NlmeansFilter.cpp$(DependSuffix) -MM NlmeansFilter.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/NlmeansFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/NlmeansFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/NlmeansFilter.cpp$(PreprocessSuffix): NlmeansFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/NlmeansFilter.cpp$(PreprocessSuffix) NlmeansFilter.cpp

$(IntermediateDirectory)/Selection.cpp$(ObjectSuffix): Selection.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Selection.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Selection.cpp$(DependSuffix) -MM Selection.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/Selection.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Selection.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Selection.cpp$(PreprocessSuffix): Selection.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Selection.cpp$(PreprocessSuffix) Selection.cpp

$(IntermediateDirectory)/DecodeRaw.cpp$(ObjectSuffix): DecodeRaw.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/DecodeRaw.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/DecodeRaw.cpp$(DependSuffix) -MM DecodeRaw.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/DecodeRaw.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/DecodeRaw.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/DecodeRaw.cpp$(PreprocessSuffix): DecodeRaw.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/DecodeRaw.cpp$(PreprocessSuffix) DecodeRaw.cpp

$(IntermediateDirectory)/VideoFilter.cpp$(ObjectSuffix): VideoFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/VideoFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/VideoFilter.cpp$(DependSuffix) -MM VideoFilter.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/VideoFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/VideoFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/VideoFilter.cpp$(PreprocessSuffix): VideoFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/VideoFilter.cpp$(PreprocessSuffix) VideoFilter.cpp

$(IntermediateDirectory)/AudioVideoFilter.cpp$(ObjectSuffix): AudioVideoFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/AudioVideoFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/AudioVideoFilter.cpp$(DependSuffix) -MM AudioVideoFilter.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/AudioVideoFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/AudioVideoFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/AudioVideoFilter.cpp$(PreprocessSuffix): AudioVideoFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/AudioVideoFilter.cpp$(PreprocessSuffix) AudioVideoFilter.cpp

$(IntermediateDirectory)/PosterisationFilter.cpp$(ObjectSuffix): PosterisationFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PosterisationFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PosterisationFilter.cpp$(DependSuffix) -MM PosterisationFilter.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/PosterisationFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PosterisationFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PosterisationFilter.cpp$(PreprocessSuffix): PosterisationFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PosterisationFilter.cpp$(PreprocessSuffix) PosterisationFilter.cpp

$(IntermediateDirectory)/PerlinNoise.cpp$(ObjectSuffix): PerlinNoise.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PerlinNoise.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PerlinNoise.cpp$(DependSuffix) -MM PerlinNoise.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/PerlinNoise.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PerlinNoise.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PerlinNoise.cpp$(PreprocessSuffix): PerlinNoise.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PerlinNoise.cpp$(PreprocessSuffix) PerlinNoise.cpp

$(IntermediateDirectory)/LensDistortion.cpp$(ObjectSuffix): LensDistortion.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LensDistortion.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LensDistortion.cpp$(DependSuffix) -MM LensDistortion.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/LensDistortion.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LensDistortion.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LensDistortion.cpp$(PreprocessSuffix): LensDistortion.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LensDistortion.cpp$(PreprocessSuffix) LensDistortion.cpp

$(IntermediateDirectory)/SharpenMaskingFilter.cpp$(ObjectSuffix): SharpenMaskingFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SharpenMaskingFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SharpenMaskingFilter.cpp$(DependSuffix) -MM SharpenMaskingFilter.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/SharpenMaskingFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SharpenMaskingFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SharpenMaskingFilter.cpp$(PreprocessSuffix): SharpenMaskingFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SharpenMaskingFilter.cpp$(PreprocessSuffix) SharpenMaskingFilter.cpp

$(IntermediateDirectory)/panorama.cpp$(ObjectSuffix): panorama.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/panorama.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/panorama.cpp$(DependSuffix) -MM panorama.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/panorama.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/panorama.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/panorama.cpp$(PreprocessSuffix): panorama.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/panorama.cpp$(PreprocessSuffix) panorama.cpp

$(IntermediateDirectory)/EdgePreservingFilter.cpp$(ObjectSuffix): EdgePreservingFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/EdgePreservingFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/EdgePreservingFilter.cpp$(DependSuffix) -MM EdgePreservingFilter.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/EdgePreservingFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/EdgePreservingFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/EdgePreservingFilter.cpp$(PreprocessSuffix): EdgePreservingFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/EdgePreservingFilter.cpp$(PreprocessSuffix) EdgePreservingFilter.cpp

$(IntermediateDirectory)/SwirlFilter.cpp$(ObjectSuffix): SwirlFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SwirlFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SwirlFilter.cpp$(DependSuffix) -MM SwirlFilter.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/SwirlFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SwirlFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SwirlFilter.cpp$(PreprocessSuffix): SwirlFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SwirlFilter.cpp$(PreprocessSuffix) SwirlFilter.cpp

$(IntermediateDirectory)/DetailEnhance.cpp$(ObjectSuffix): DetailEnhance.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/DetailEnhance.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/DetailEnhance.cpp$(DependSuffix) -MM DetailEnhance.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/DetailEnhance.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/DetailEnhance.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/DetailEnhance.cpp$(PreprocessSuffix): DetailEnhance.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/DetailEnhance.cpp$(PreprocessSuffix) DetailEnhance.cpp

$(IntermediateDirectory)/CartoonFilter.cpp$(ObjectSuffix): CartoonFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CartoonFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CartoonFilter.cpp$(DependSuffix) -MM CartoonFilter.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/CartoonFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CartoonFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CartoonFilter.cpp$(PreprocessSuffix): CartoonFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CartoonFilter.cpp$(PreprocessSuffix) CartoonFilter.cpp

$(IntermediateDirectory)/BokehFilter.cpp$(ObjectSuffix): BokehFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BokehFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BokehFilter.cpp$(DependSuffix) -MM BokehFilter.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/BokehFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BokehFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BokehFilter.cpp$(PreprocessSuffix): BokehFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BokehFilter.cpp$(PreprocessSuffix) BokehFilter.cpp

$(IntermediateDirectory)/RotateFreeFilter.cpp$(ObjectSuffix): RotateFreeFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RotateFreeFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RotateFreeFilter.cpp$(DependSuffix) -MM RotateFreeFilter.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/RotateFreeFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RotateFreeFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RotateFreeFilter.cpp$(PreprocessSuffix): RotateFreeFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RotateFreeFilter.cpp$(PreprocessSuffix) RotateFreeFilter.cpp

$(IntermediateDirectory)/OilPaintingFilter.cpp$(ObjectSuffix): OilPaintingFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/OilPaintingFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/OilPaintingFilter.cpp$(DependSuffix) -MM OilPaintingFilter.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/OilPaintingFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/OilPaintingFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OilPaintingFilter.cpp$(PreprocessSuffix): OilPaintingFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OilPaintingFilter.cpp$(PreprocessSuffix) OilPaintingFilter.cpp

$(IntermediateDirectory)/hqdn3dFilter.cpp$(ObjectSuffix): hqdn3dFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hqdn3dFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/hqdn3dFilter.cpp$(DependSuffix) -MM hqdn3dFilter.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/hqdn3dFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hqdn3dFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hqdn3dFilter.cpp$(PreprocessSuffix): hqdn3dFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hqdn3dFilter.cpp$(PreprocessSuffix) hqdn3dFilter.cpp

$(IntermediateDirectory)/MotionBlurFilter.cpp$(ObjectSuffix): MotionBlurFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MotionBlurFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MotionBlurFilter.cpp$(DependSuffix) -MM MotionBlurFilter.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/MotionBlurFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MotionBlurFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MotionBlurFilter.cpp$(PreprocessSuffix): MotionBlurFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MotionBlurFilter.cpp$(PreprocessSuffix) MotionBlurFilter.cpp

$(IntermediateDirectory)/videostream.cpp$(ObjectSuffix): videostream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/videostream.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/videostream.cpp$(DependSuffix) -MM videostream.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/videostream.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/videostream.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/videostream.cpp$(PreprocessSuffix): videostream.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/videostream.cpp$(PreprocessSuffix) videostream.cpp

$(IntermediateDirectory)/WaveFilter.cpp$(ObjectSuffix): WaveFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/WaveFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/WaveFilter.cpp$(DependSuffix) -MM WaveFilter.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/WaveFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/WaveFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/WaveFilter.cpp$(PreprocessSuffix): WaveFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/WaveFilter.cpp$(PreprocessSuffix) WaveFilter.cpp

$(IntermediateDirectory)/BrightAndContrastFilter.cpp$(ObjectSuffix): BrightAndContrastFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BrightAndContrastFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BrightAndContrastFilter.cpp$(DependSuffix) -MM BrightAndContrastFilter.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/BrightAndContrastFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BrightAndContrastFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BrightAndContrastFilter.cpp$(PreprocessSuffix): BrightAndContrastFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BrightAndContrastFilter.cpp$(PreprocessSuffix) BrightAndContrastFilter.cpp

$(IntermediateDirectory)/BlurFilter.cpp$(ObjectSuffix): BlurFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BlurFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BlurFilter.cpp$(DependSuffix) -MM BlurFilter.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libFiltre/BlurFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BlurFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BlurFilter.cpp$(PreprocessSuffix): BlurFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BlurFilter.cpp$(PreprocessSuffix) BlurFilter.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/
	$(RM) ../include/Release/header.h.gch


