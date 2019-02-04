##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libFiltre
ConfigurationName      :=Release
WorkspacePath          :=/home/figuinha/developpement/Regards
ProjectPath            :=/home/figuinha/developpement/Regards/libFiltre
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=04/02/19
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
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libFiltre.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s
IncludePath            := $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2 $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include  $(IncludeSwitch)../libVideoFFMpeg $(IncludeSwitch)../libVideoThumbnail $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../Otherlib/RegardsRaw $(IncludeSwitch)../libextern/piccante-master/include $(IncludeSwitch). $(IncludeSwitch)../libUtility $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../libOpenCL $(IncludeSwitch)../libResource $(IncludeSwitch)../include $(IncludeSwitch)../libControl $(IncludeSwitch)../Otherlib/wxSVG/include $(IncludeSwitch)../libPiccante $(IncludeSwitch)../libPicture $(IncludeSwitch)../libextern/fftw-3.3.8/api 
IncludePCH             :=  -include ../include/Release/header.h 
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
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread  -fopenmp -std=gnu++11 -Wno-narrowing -O2 -msse4.1 $(Preprocessors)
CFLAGS   :=  -Wall  -O2 -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := ccache /usr/bin/x86_64-linux-gnu-as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/Bm3dDlg.cpp$(ObjectSuffix) $(IntermediateDirectory)/InterpolationFloatBicubic.cpp$(ObjectSuffix) $(IntermediateDirectory)/Interpolation.cpp$(ObjectSuffix) $(IntermediateDirectory)/FiltreEffet.cpp$(ObjectSuffix) $(IntermediateDirectory)/BlurFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/bilateralFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/ClaheFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/CloudsFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/MotionBlurFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/InterpolationBicubic.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/NlmeansFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/PhotoFiltreFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/SharpenMaskingFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/WaveFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/ColorBalanceFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/BestExposureFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/KuwaharaFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/bm3dWindowFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/AudioVideoFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/VideoFilter.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/GaussianBlurFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/GaussianBlur.cpp$(ObjectSuffix) $(IntermediateDirectory)/bm3dfilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/Wave.cpp$(ObjectSuffix) $(IntermediateDirectory)/MotionBlur.cpp$(ObjectSuffix) $(IntermediateDirectory)/utilities.cpp$(ObjectSuffix) $(IntermediateDirectory)/BrightAndContrastFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/Histogramme.cpp$(ObjectSuffix) $(IntermediateDirectory)/Selection.cpp$(ObjectSuffix) $(IntermediateDirectory)/DecodeRaw.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/PosterisationFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/LensFlare.cpp$(ObjectSuffix) $(IntermediateDirectory)/Draw.cpp$(ObjectSuffix) $(IntermediateDirectory)/FilterData.cpp$(ObjectSuffix) $(IntermediateDirectory)/circle.cpp$(ObjectSuffix) $(IntermediateDirectory)/InterpolationBilinear.cpp$(ObjectSuffix) $(IntermediateDirectory)/SolarisationFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/PiccanteFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/LensFlareFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/SwirlFilter.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Line.cpp$(ObjectSuffix) $(IntermediateDirectory)/Painting.cpp$(ObjectSuffix) $(IntermediateDirectory)/bm3d.cpp$(ObjectSuffix) $(IntermediateDirectory)/lib_transforms.cpp$(ObjectSuffix) $(IntermediateDirectory)/Crop.cpp$(ObjectSuffix) $(IntermediateDirectory)/Rotate.cpp$(ObjectSuffix) $(IntermediateDirectory)/PerlinNoise.cpp$(ObjectSuffix) 

Objects1=$(IntermediateDirectory)/Rotation.cpp$(ObjectSuffix) $(IntermediateDirectory)/FilterWindowParam.cpp$(ObjectSuffix) $(IntermediateDirectory)/Filtre.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/RotateFreeFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/RedEye.cpp$(ObjectSuffix) $(IntermediateDirectory)/clahe.cpp$(ObjectSuffix) $(IntermediateDirectory)/Color.cpp$(ObjectSuffix) $(IntermediateDirectory)/FiltreEffetCPU.cpp$(ObjectSuffix) 



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
	$(AR) $(ArchiveOutputSwitch)$(OutputFile) @$(ObjectsFileList) $(ArLibs)
	@$(MakeDirCommand) "/home/figuinha/developpement/Regards/.build-release"
	@echo rebuilt > "/home/figuinha/developpement/Regards/.build-release/libFiltre"

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
$(IntermediateDirectory)/Bm3dDlg.cpp$(ObjectSuffix): Bm3dDlg.cpp $(IntermediateDirectory)/Bm3dDlg.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/Bm3dDlg.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Bm3dDlg.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Bm3dDlg.cpp$(DependSuffix): Bm3dDlg.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Bm3dDlg.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Bm3dDlg.cpp$(DependSuffix) -MM Bm3dDlg.cpp

$(IntermediateDirectory)/Bm3dDlg.cpp$(PreprocessSuffix): Bm3dDlg.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Bm3dDlg.cpp$(PreprocessSuffix) Bm3dDlg.cpp

$(IntermediateDirectory)/InterpolationFloatBicubic.cpp$(ObjectSuffix): InterpolationFloatBicubic.cpp $(IntermediateDirectory)/InterpolationFloatBicubic.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/InterpolationFloatBicubic.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InterpolationFloatBicubic.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InterpolationFloatBicubic.cpp$(DependSuffix): InterpolationFloatBicubic.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InterpolationFloatBicubic.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InterpolationFloatBicubic.cpp$(DependSuffix) -MM InterpolationFloatBicubic.cpp

$(IntermediateDirectory)/InterpolationFloatBicubic.cpp$(PreprocessSuffix): InterpolationFloatBicubic.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InterpolationFloatBicubic.cpp$(PreprocessSuffix) InterpolationFloatBicubic.cpp

$(IntermediateDirectory)/Interpolation.cpp$(ObjectSuffix): Interpolation.cpp $(IntermediateDirectory)/Interpolation.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/Interpolation.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Interpolation.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Interpolation.cpp$(DependSuffix): Interpolation.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Interpolation.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Interpolation.cpp$(DependSuffix) -MM Interpolation.cpp

$(IntermediateDirectory)/Interpolation.cpp$(PreprocessSuffix): Interpolation.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Interpolation.cpp$(PreprocessSuffix) Interpolation.cpp

$(IntermediateDirectory)/FiltreEffet.cpp$(ObjectSuffix): FiltreEffet.cpp $(IntermediateDirectory)/FiltreEffet.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/FiltreEffet.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FiltreEffet.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FiltreEffet.cpp$(DependSuffix): FiltreEffet.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FiltreEffet.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FiltreEffet.cpp$(DependSuffix) -MM FiltreEffet.cpp

$(IntermediateDirectory)/FiltreEffet.cpp$(PreprocessSuffix): FiltreEffet.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FiltreEffet.cpp$(PreprocessSuffix) FiltreEffet.cpp

$(IntermediateDirectory)/BlurFilter.cpp$(ObjectSuffix): BlurFilter.cpp $(IntermediateDirectory)/BlurFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/BlurFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BlurFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BlurFilter.cpp$(DependSuffix): BlurFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BlurFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BlurFilter.cpp$(DependSuffix) -MM BlurFilter.cpp

$(IntermediateDirectory)/BlurFilter.cpp$(PreprocessSuffix): BlurFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BlurFilter.cpp$(PreprocessSuffix) BlurFilter.cpp

$(IntermediateDirectory)/bilateralFilter.cpp$(ObjectSuffix): bilateralFilter.cpp $(IntermediateDirectory)/bilateralFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/bilateralFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bilateralFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bilateralFilter.cpp$(DependSuffix): bilateralFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bilateralFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bilateralFilter.cpp$(DependSuffix) -MM bilateralFilter.cpp

$(IntermediateDirectory)/bilateralFilter.cpp$(PreprocessSuffix): bilateralFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bilateralFilter.cpp$(PreprocessSuffix) bilateralFilter.cpp

$(IntermediateDirectory)/ClaheFilter.cpp$(ObjectSuffix): ClaheFilter.cpp $(IntermediateDirectory)/ClaheFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/ClaheFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ClaheFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ClaheFilter.cpp$(DependSuffix): ClaheFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ClaheFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ClaheFilter.cpp$(DependSuffix) -MM ClaheFilter.cpp

$(IntermediateDirectory)/ClaheFilter.cpp$(PreprocessSuffix): ClaheFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ClaheFilter.cpp$(PreprocessSuffix) ClaheFilter.cpp

$(IntermediateDirectory)/CloudsFilter.cpp$(ObjectSuffix): CloudsFilter.cpp $(IntermediateDirectory)/CloudsFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/CloudsFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CloudsFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CloudsFilter.cpp$(DependSuffix): CloudsFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CloudsFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CloudsFilter.cpp$(DependSuffix) -MM CloudsFilter.cpp

$(IntermediateDirectory)/CloudsFilter.cpp$(PreprocessSuffix): CloudsFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CloudsFilter.cpp$(PreprocessSuffix) CloudsFilter.cpp

$(IntermediateDirectory)/MotionBlurFilter.cpp$(ObjectSuffix): MotionBlurFilter.cpp $(IntermediateDirectory)/MotionBlurFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/MotionBlurFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MotionBlurFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MotionBlurFilter.cpp$(DependSuffix): MotionBlurFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MotionBlurFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MotionBlurFilter.cpp$(DependSuffix) -MM MotionBlurFilter.cpp

$(IntermediateDirectory)/MotionBlurFilter.cpp$(PreprocessSuffix): MotionBlurFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MotionBlurFilter.cpp$(PreprocessSuffix) MotionBlurFilter.cpp

$(IntermediateDirectory)/InterpolationBicubic.cpp$(ObjectSuffix): InterpolationBicubic.cpp $(IntermediateDirectory)/InterpolationBicubic.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/InterpolationBicubic.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InterpolationBicubic.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InterpolationBicubic.cpp$(DependSuffix): InterpolationBicubic.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InterpolationBicubic.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InterpolationBicubic.cpp$(DependSuffix) -MM InterpolationBicubic.cpp

$(IntermediateDirectory)/InterpolationBicubic.cpp$(PreprocessSuffix): InterpolationBicubic.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InterpolationBicubic.cpp$(PreprocessSuffix) InterpolationBicubic.cpp

$(IntermediateDirectory)/NlmeansFilter.cpp$(ObjectSuffix): NlmeansFilter.cpp $(IntermediateDirectory)/NlmeansFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/NlmeansFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/NlmeansFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/NlmeansFilter.cpp$(DependSuffix): NlmeansFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/NlmeansFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/NlmeansFilter.cpp$(DependSuffix) -MM NlmeansFilter.cpp

$(IntermediateDirectory)/NlmeansFilter.cpp$(PreprocessSuffix): NlmeansFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/NlmeansFilter.cpp$(PreprocessSuffix) NlmeansFilter.cpp

$(IntermediateDirectory)/PhotoFiltreFilter.cpp$(ObjectSuffix): PhotoFiltreFilter.cpp $(IntermediateDirectory)/PhotoFiltreFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/PhotoFiltreFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PhotoFiltreFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PhotoFiltreFilter.cpp$(DependSuffix): PhotoFiltreFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PhotoFiltreFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PhotoFiltreFilter.cpp$(DependSuffix) -MM PhotoFiltreFilter.cpp

$(IntermediateDirectory)/PhotoFiltreFilter.cpp$(PreprocessSuffix): PhotoFiltreFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PhotoFiltreFilter.cpp$(PreprocessSuffix) PhotoFiltreFilter.cpp

$(IntermediateDirectory)/SharpenMaskingFilter.cpp$(ObjectSuffix): SharpenMaskingFilter.cpp $(IntermediateDirectory)/SharpenMaskingFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/SharpenMaskingFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SharpenMaskingFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SharpenMaskingFilter.cpp$(DependSuffix): SharpenMaskingFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SharpenMaskingFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SharpenMaskingFilter.cpp$(DependSuffix) -MM SharpenMaskingFilter.cpp

$(IntermediateDirectory)/SharpenMaskingFilter.cpp$(PreprocessSuffix): SharpenMaskingFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SharpenMaskingFilter.cpp$(PreprocessSuffix) SharpenMaskingFilter.cpp

$(IntermediateDirectory)/WaveFilter.cpp$(ObjectSuffix): WaveFilter.cpp $(IntermediateDirectory)/WaveFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/WaveFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/WaveFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/WaveFilter.cpp$(DependSuffix): WaveFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/WaveFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/WaveFilter.cpp$(DependSuffix) -MM WaveFilter.cpp

$(IntermediateDirectory)/WaveFilter.cpp$(PreprocessSuffix): WaveFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/WaveFilter.cpp$(PreprocessSuffix) WaveFilter.cpp

$(IntermediateDirectory)/ColorBalanceFilter.cpp$(ObjectSuffix): ColorBalanceFilter.cpp $(IntermediateDirectory)/ColorBalanceFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/ColorBalanceFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ColorBalanceFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ColorBalanceFilter.cpp$(DependSuffix): ColorBalanceFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ColorBalanceFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ColorBalanceFilter.cpp$(DependSuffix) -MM ColorBalanceFilter.cpp

$(IntermediateDirectory)/ColorBalanceFilter.cpp$(PreprocessSuffix): ColorBalanceFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ColorBalanceFilter.cpp$(PreprocessSuffix) ColorBalanceFilter.cpp

$(IntermediateDirectory)/BestExposureFilter.cpp$(ObjectSuffix): BestExposureFilter.cpp $(IntermediateDirectory)/BestExposureFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/BestExposureFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BestExposureFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BestExposureFilter.cpp$(DependSuffix): BestExposureFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BestExposureFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BestExposureFilter.cpp$(DependSuffix) -MM BestExposureFilter.cpp

$(IntermediateDirectory)/BestExposureFilter.cpp$(PreprocessSuffix): BestExposureFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BestExposureFilter.cpp$(PreprocessSuffix) BestExposureFilter.cpp

$(IntermediateDirectory)/KuwaharaFilter.cpp$(ObjectSuffix): KuwaharaFilter.cpp $(IntermediateDirectory)/KuwaharaFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/KuwaharaFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/KuwaharaFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/KuwaharaFilter.cpp$(DependSuffix): KuwaharaFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/KuwaharaFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/KuwaharaFilter.cpp$(DependSuffix) -MM KuwaharaFilter.cpp

$(IntermediateDirectory)/KuwaharaFilter.cpp$(PreprocessSuffix): KuwaharaFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/KuwaharaFilter.cpp$(PreprocessSuffix) KuwaharaFilter.cpp

$(IntermediateDirectory)/bm3dWindowFilter.cpp$(ObjectSuffix): bm3dWindowFilter.cpp $(IntermediateDirectory)/bm3dWindowFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/bm3dWindowFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bm3dWindowFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bm3dWindowFilter.cpp$(DependSuffix): bm3dWindowFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bm3dWindowFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bm3dWindowFilter.cpp$(DependSuffix) -MM bm3dWindowFilter.cpp

$(IntermediateDirectory)/bm3dWindowFilter.cpp$(PreprocessSuffix): bm3dWindowFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bm3dWindowFilter.cpp$(PreprocessSuffix) bm3dWindowFilter.cpp

$(IntermediateDirectory)/AudioVideoFilter.cpp$(ObjectSuffix): AudioVideoFilter.cpp $(IntermediateDirectory)/AudioVideoFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/AudioVideoFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/AudioVideoFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/AudioVideoFilter.cpp$(DependSuffix): AudioVideoFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/AudioVideoFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/AudioVideoFilter.cpp$(DependSuffix) -MM AudioVideoFilter.cpp

$(IntermediateDirectory)/AudioVideoFilter.cpp$(PreprocessSuffix): AudioVideoFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/AudioVideoFilter.cpp$(PreprocessSuffix) AudioVideoFilter.cpp

$(IntermediateDirectory)/VideoFilter.cpp$(ObjectSuffix): VideoFilter.cpp $(IntermediateDirectory)/VideoFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/VideoFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/VideoFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/VideoFilter.cpp$(DependSuffix): VideoFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/VideoFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/VideoFilter.cpp$(DependSuffix) -MM VideoFilter.cpp

$(IntermediateDirectory)/VideoFilter.cpp$(PreprocessSuffix): VideoFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/VideoFilter.cpp$(PreprocessSuffix) VideoFilter.cpp

$(IntermediateDirectory)/GaussianBlurFilter.cpp$(ObjectSuffix): GaussianBlurFilter.cpp $(IntermediateDirectory)/GaussianBlurFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/GaussianBlurFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GaussianBlurFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GaussianBlurFilter.cpp$(DependSuffix): GaussianBlurFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GaussianBlurFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GaussianBlurFilter.cpp$(DependSuffix) -MM GaussianBlurFilter.cpp

$(IntermediateDirectory)/GaussianBlurFilter.cpp$(PreprocessSuffix): GaussianBlurFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GaussianBlurFilter.cpp$(PreprocessSuffix) GaussianBlurFilter.cpp

$(IntermediateDirectory)/GaussianBlur.cpp$(ObjectSuffix): GaussianBlur.cpp $(IntermediateDirectory)/GaussianBlur.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/GaussianBlur.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GaussianBlur.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GaussianBlur.cpp$(DependSuffix): GaussianBlur.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GaussianBlur.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GaussianBlur.cpp$(DependSuffix) -MM GaussianBlur.cpp

$(IntermediateDirectory)/GaussianBlur.cpp$(PreprocessSuffix): GaussianBlur.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GaussianBlur.cpp$(PreprocessSuffix) GaussianBlur.cpp

$(IntermediateDirectory)/bm3dfilter.cpp$(ObjectSuffix): bm3dfilter.cpp $(IntermediateDirectory)/bm3dfilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/bm3dfilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bm3dfilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bm3dfilter.cpp$(DependSuffix): bm3dfilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bm3dfilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bm3dfilter.cpp$(DependSuffix) -MM bm3dfilter.cpp

$(IntermediateDirectory)/bm3dfilter.cpp$(PreprocessSuffix): bm3dfilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bm3dfilter.cpp$(PreprocessSuffix) bm3dfilter.cpp

$(IntermediateDirectory)/Wave.cpp$(ObjectSuffix): Wave.cpp $(IntermediateDirectory)/Wave.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/Wave.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Wave.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Wave.cpp$(DependSuffix): Wave.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Wave.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Wave.cpp$(DependSuffix) -MM Wave.cpp

$(IntermediateDirectory)/Wave.cpp$(PreprocessSuffix): Wave.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Wave.cpp$(PreprocessSuffix) Wave.cpp

$(IntermediateDirectory)/MotionBlur.cpp$(ObjectSuffix): MotionBlur.cpp $(IntermediateDirectory)/MotionBlur.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/MotionBlur.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MotionBlur.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MotionBlur.cpp$(DependSuffix): MotionBlur.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MotionBlur.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MotionBlur.cpp$(DependSuffix) -MM MotionBlur.cpp

$(IntermediateDirectory)/MotionBlur.cpp$(PreprocessSuffix): MotionBlur.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MotionBlur.cpp$(PreprocessSuffix) MotionBlur.cpp

$(IntermediateDirectory)/utilities.cpp$(ObjectSuffix): utilities.cpp $(IntermediateDirectory)/utilities.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/utilities.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/utilities.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/utilities.cpp$(DependSuffix): utilities.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/utilities.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/utilities.cpp$(DependSuffix) -MM utilities.cpp

$(IntermediateDirectory)/utilities.cpp$(PreprocessSuffix): utilities.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/utilities.cpp$(PreprocessSuffix) utilities.cpp

$(IntermediateDirectory)/BrightAndContrastFilter.cpp$(ObjectSuffix): BrightAndContrastFilter.cpp $(IntermediateDirectory)/BrightAndContrastFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/BrightAndContrastFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BrightAndContrastFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BrightAndContrastFilter.cpp$(DependSuffix): BrightAndContrastFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BrightAndContrastFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BrightAndContrastFilter.cpp$(DependSuffix) -MM BrightAndContrastFilter.cpp

$(IntermediateDirectory)/BrightAndContrastFilter.cpp$(PreprocessSuffix): BrightAndContrastFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BrightAndContrastFilter.cpp$(PreprocessSuffix) BrightAndContrastFilter.cpp

$(IntermediateDirectory)/Histogramme.cpp$(ObjectSuffix): Histogramme.cpp $(IntermediateDirectory)/Histogramme.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/Histogramme.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Histogramme.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Histogramme.cpp$(DependSuffix): Histogramme.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Histogramme.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Histogramme.cpp$(DependSuffix) -MM Histogramme.cpp

$(IntermediateDirectory)/Histogramme.cpp$(PreprocessSuffix): Histogramme.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Histogramme.cpp$(PreprocessSuffix) Histogramme.cpp

$(IntermediateDirectory)/Selection.cpp$(ObjectSuffix): Selection.cpp $(IntermediateDirectory)/Selection.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/Selection.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Selection.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Selection.cpp$(DependSuffix): Selection.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Selection.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Selection.cpp$(DependSuffix) -MM Selection.cpp

$(IntermediateDirectory)/Selection.cpp$(PreprocessSuffix): Selection.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Selection.cpp$(PreprocessSuffix) Selection.cpp

$(IntermediateDirectory)/DecodeRaw.cpp$(ObjectSuffix): DecodeRaw.cpp $(IntermediateDirectory)/DecodeRaw.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/DecodeRaw.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/DecodeRaw.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/DecodeRaw.cpp$(DependSuffix): DecodeRaw.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/DecodeRaw.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/DecodeRaw.cpp$(DependSuffix) -MM DecodeRaw.cpp

$(IntermediateDirectory)/DecodeRaw.cpp$(PreprocessSuffix): DecodeRaw.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/DecodeRaw.cpp$(PreprocessSuffix) DecodeRaw.cpp

$(IntermediateDirectory)/PosterisationFilter.cpp$(ObjectSuffix): PosterisationFilter.cpp $(IntermediateDirectory)/PosterisationFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/PosterisationFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PosterisationFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PosterisationFilter.cpp$(DependSuffix): PosterisationFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PosterisationFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PosterisationFilter.cpp$(DependSuffix) -MM PosterisationFilter.cpp

$(IntermediateDirectory)/PosterisationFilter.cpp$(PreprocessSuffix): PosterisationFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PosterisationFilter.cpp$(PreprocessSuffix) PosterisationFilter.cpp

$(IntermediateDirectory)/LensFlare.cpp$(ObjectSuffix): LensFlare.cpp $(IntermediateDirectory)/LensFlare.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/LensFlare.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LensFlare.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LensFlare.cpp$(DependSuffix): LensFlare.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LensFlare.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LensFlare.cpp$(DependSuffix) -MM LensFlare.cpp

$(IntermediateDirectory)/LensFlare.cpp$(PreprocessSuffix): LensFlare.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LensFlare.cpp$(PreprocessSuffix) LensFlare.cpp

$(IntermediateDirectory)/Draw.cpp$(ObjectSuffix): Draw.cpp $(IntermediateDirectory)/Draw.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/Draw.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Draw.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Draw.cpp$(DependSuffix): Draw.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Draw.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Draw.cpp$(DependSuffix) -MM Draw.cpp

$(IntermediateDirectory)/Draw.cpp$(PreprocessSuffix): Draw.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Draw.cpp$(PreprocessSuffix) Draw.cpp

$(IntermediateDirectory)/FilterData.cpp$(ObjectSuffix): FilterData.cpp $(IntermediateDirectory)/FilterData.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/FilterData.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FilterData.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FilterData.cpp$(DependSuffix): FilterData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FilterData.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FilterData.cpp$(DependSuffix) -MM FilterData.cpp

$(IntermediateDirectory)/FilterData.cpp$(PreprocessSuffix): FilterData.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FilterData.cpp$(PreprocessSuffix) FilterData.cpp

$(IntermediateDirectory)/circle.cpp$(ObjectSuffix): circle.cpp $(IntermediateDirectory)/circle.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/circle.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/circle.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/circle.cpp$(DependSuffix): circle.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/circle.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/circle.cpp$(DependSuffix) -MM circle.cpp

$(IntermediateDirectory)/circle.cpp$(PreprocessSuffix): circle.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/circle.cpp$(PreprocessSuffix) circle.cpp

$(IntermediateDirectory)/InterpolationBilinear.cpp$(ObjectSuffix): InterpolationBilinear.cpp $(IntermediateDirectory)/InterpolationBilinear.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/InterpolationBilinear.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InterpolationBilinear.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InterpolationBilinear.cpp$(DependSuffix): InterpolationBilinear.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InterpolationBilinear.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InterpolationBilinear.cpp$(DependSuffix) -MM InterpolationBilinear.cpp

$(IntermediateDirectory)/InterpolationBilinear.cpp$(PreprocessSuffix): InterpolationBilinear.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InterpolationBilinear.cpp$(PreprocessSuffix) InterpolationBilinear.cpp

$(IntermediateDirectory)/SolarisationFilter.cpp$(ObjectSuffix): SolarisationFilter.cpp $(IntermediateDirectory)/SolarisationFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/SolarisationFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SolarisationFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SolarisationFilter.cpp$(DependSuffix): SolarisationFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SolarisationFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SolarisationFilter.cpp$(DependSuffix) -MM SolarisationFilter.cpp

$(IntermediateDirectory)/SolarisationFilter.cpp$(PreprocessSuffix): SolarisationFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SolarisationFilter.cpp$(PreprocessSuffix) SolarisationFilter.cpp

$(IntermediateDirectory)/PiccanteFilter.cpp$(ObjectSuffix): PiccanteFilter.cpp $(IntermediateDirectory)/PiccanteFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/PiccanteFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PiccanteFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PiccanteFilter.cpp$(DependSuffix): PiccanteFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PiccanteFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PiccanteFilter.cpp$(DependSuffix) -MM PiccanteFilter.cpp

$(IntermediateDirectory)/PiccanteFilter.cpp$(PreprocessSuffix): PiccanteFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PiccanteFilter.cpp$(PreprocessSuffix) PiccanteFilter.cpp

$(IntermediateDirectory)/LensFlareFilter.cpp$(ObjectSuffix): LensFlareFilter.cpp $(IntermediateDirectory)/LensFlareFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/LensFlareFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LensFlareFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LensFlareFilter.cpp$(DependSuffix): LensFlareFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LensFlareFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LensFlareFilter.cpp$(DependSuffix) -MM LensFlareFilter.cpp

$(IntermediateDirectory)/LensFlareFilter.cpp$(PreprocessSuffix): LensFlareFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LensFlareFilter.cpp$(PreprocessSuffix) LensFlareFilter.cpp

$(IntermediateDirectory)/SwirlFilter.cpp$(ObjectSuffix): SwirlFilter.cpp $(IntermediateDirectory)/SwirlFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/SwirlFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SwirlFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SwirlFilter.cpp$(DependSuffix): SwirlFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SwirlFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SwirlFilter.cpp$(DependSuffix) -MM SwirlFilter.cpp

$(IntermediateDirectory)/SwirlFilter.cpp$(PreprocessSuffix): SwirlFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SwirlFilter.cpp$(PreprocessSuffix) SwirlFilter.cpp

$(IntermediateDirectory)/Line.cpp$(ObjectSuffix): Line.cpp $(IntermediateDirectory)/Line.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/Line.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Line.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Line.cpp$(DependSuffix): Line.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Line.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Line.cpp$(DependSuffix) -MM Line.cpp

$(IntermediateDirectory)/Line.cpp$(PreprocessSuffix): Line.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Line.cpp$(PreprocessSuffix) Line.cpp

$(IntermediateDirectory)/Painting.cpp$(ObjectSuffix): Painting.cpp $(IntermediateDirectory)/Painting.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/Painting.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Painting.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Painting.cpp$(DependSuffix): Painting.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Painting.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Painting.cpp$(DependSuffix) -MM Painting.cpp

$(IntermediateDirectory)/Painting.cpp$(PreprocessSuffix): Painting.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Painting.cpp$(PreprocessSuffix) Painting.cpp

$(IntermediateDirectory)/bm3d.cpp$(ObjectSuffix): bm3d.cpp $(IntermediateDirectory)/bm3d.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/bm3d.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bm3d.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bm3d.cpp$(DependSuffix): bm3d.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bm3d.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bm3d.cpp$(DependSuffix) -MM bm3d.cpp

$(IntermediateDirectory)/bm3d.cpp$(PreprocessSuffix): bm3d.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bm3d.cpp$(PreprocessSuffix) bm3d.cpp

$(IntermediateDirectory)/lib_transforms.cpp$(ObjectSuffix): lib_transforms.cpp $(IntermediateDirectory)/lib_transforms.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/lib_transforms.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lib_transforms.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lib_transforms.cpp$(DependSuffix): lib_transforms.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lib_transforms.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/lib_transforms.cpp$(DependSuffix) -MM lib_transforms.cpp

$(IntermediateDirectory)/lib_transforms.cpp$(PreprocessSuffix): lib_transforms.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lib_transforms.cpp$(PreprocessSuffix) lib_transforms.cpp

$(IntermediateDirectory)/Crop.cpp$(ObjectSuffix): Crop.cpp $(IntermediateDirectory)/Crop.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/Crop.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Crop.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Crop.cpp$(DependSuffix): Crop.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Crop.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Crop.cpp$(DependSuffix) -MM Crop.cpp

$(IntermediateDirectory)/Crop.cpp$(PreprocessSuffix): Crop.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Crop.cpp$(PreprocessSuffix) Crop.cpp

$(IntermediateDirectory)/Rotate.cpp$(ObjectSuffix): Rotate.cpp $(IntermediateDirectory)/Rotate.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/Rotate.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Rotate.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Rotate.cpp$(DependSuffix): Rotate.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Rotate.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Rotate.cpp$(DependSuffix) -MM Rotate.cpp

$(IntermediateDirectory)/Rotate.cpp$(PreprocessSuffix): Rotate.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Rotate.cpp$(PreprocessSuffix) Rotate.cpp

$(IntermediateDirectory)/PerlinNoise.cpp$(ObjectSuffix): PerlinNoise.cpp $(IntermediateDirectory)/PerlinNoise.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/PerlinNoise.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PerlinNoise.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PerlinNoise.cpp$(DependSuffix): PerlinNoise.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PerlinNoise.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PerlinNoise.cpp$(DependSuffix) -MM PerlinNoise.cpp

$(IntermediateDirectory)/PerlinNoise.cpp$(PreprocessSuffix): PerlinNoise.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PerlinNoise.cpp$(PreprocessSuffix) PerlinNoise.cpp

$(IntermediateDirectory)/Rotation.cpp$(ObjectSuffix): Rotation.cpp $(IntermediateDirectory)/Rotation.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/Rotation.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Rotation.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Rotation.cpp$(DependSuffix): Rotation.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Rotation.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Rotation.cpp$(DependSuffix) -MM Rotation.cpp

$(IntermediateDirectory)/Rotation.cpp$(PreprocessSuffix): Rotation.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Rotation.cpp$(PreprocessSuffix) Rotation.cpp

$(IntermediateDirectory)/FilterWindowParam.cpp$(ObjectSuffix): FilterWindowParam.cpp $(IntermediateDirectory)/FilterWindowParam.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/FilterWindowParam.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FilterWindowParam.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FilterWindowParam.cpp$(DependSuffix): FilterWindowParam.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FilterWindowParam.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FilterWindowParam.cpp$(DependSuffix) -MM FilterWindowParam.cpp

$(IntermediateDirectory)/FilterWindowParam.cpp$(PreprocessSuffix): FilterWindowParam.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FilterWindowParam.cpp$(PreprocessSuffix) FilterWindowParam.cpp

$(IntermediateDirectory)/Filtre.cpp$(ObjectSuffix): Filtre.cpp $(IntermediateDirectory)/Filtre.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/Filtre.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Filtre.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Filtre.cpp$(DependSuffix): Filtre.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Filtre.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Filtre.cpp$(DependSuffix) -MM Filtre.cpp

$(IntermediateDirectory)/Filtre.cpp$(PreprocessSuffix): Filtre.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Filtre.cpp$(PreprocessSuffix) Filtre.cpp

$(IntermediateDirectory)/RotateFreeFilter.cpp$(ObjectSuffix): RotateFreeFilter.cpp $(IntermediateDirectory)/RotateFreeFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/RotateFreeFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RotateFreeFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RotateFreeFilter.cpp$(DependSuffix): RotateFreeFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RotateFreeFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RotateFreeFilter.cpp$(DependSuffix) -MM RotateFreeFilter.cpp

$(IntermediateDirectory)/RotateFreeFilter.cpp$(PreprocessSuffix): RotateFreeFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RotateFreeFilter.cpp$(PreprocessSuffix) RotateFreeFilter.cpp

$(IntermediateDirectory)/RedEye.cpp$(ObjectSuffix): RedEye.cpp $(IntermediateDirectory)/RedEye.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/RedEye.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RedEye.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RedEye.cpp$(DependSuffix): RedEye.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RedEye.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RedEye.cpp$(DependSuffix) -MM RedEye.cpp

$(IntermediateDirectory)/RedEye.cpp$(PreprocessSuffix): RedEye.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RedEye.cpp$(PreprocessSuffix) RedEye.cpp

$(IntermediateDirectory)/clahe.cpp$(ObjectSuffix): clahe.cpp $(IntermediateDirectory)/clahe.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/clahe.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/clahe.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/clahe.cpp$(DependSuffix): clahe.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/clahe.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/clahe.cpp$(DependSuffix) -MM clahe.cpp

$(IntermediateDirectory)/clahe.cpp$(PreprocessSuffix): clahe.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/clahe.cpp$(PreprocessSuffix) clahe.cpp

$(IntermediateDirectory)/Color.cpp$(ObjectSuffix): Color.cpp $(IntermediateDirectory)/Color.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/Color.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Color.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Color.cpp$(DependSuffix): Color.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Color.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Color.cpp$(DependSuffix) -MM Color.cpp

$(IntermediateDirectory)/Color.cpp$(PreprocessSuffix): Color.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Color.cpp$(PreprocessSuffix) Color.cpp

$(IntermediateDirectory)/FiltreEffetCPU.cpp$(ObjectSuffix): FiltreEffetCPU.cpp $(IntermediateDirectory)/FiltreEffetCPU.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libFiltre/FiltreEffetCPU.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FiltreEffetCPU.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FiltreEffetCPU.cpp$(DependSuffix): FiltreEffetCPU.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FiltreEffetCPU.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FiltreEffetCPU.cpp$(DependSuffix) -MM FiltreEffetCPU.cpp

$(IntermediateDirectory)/FiltreEffetCPU.cpp$(PreprocessSuffix): FiltreEffetCPU.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FiltreEffetCPU.cpp$(PreprocessSuffix) FiltreEffetCPU.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/
	$(RM) ../include/Release/header.h.gch


