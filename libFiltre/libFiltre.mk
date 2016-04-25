##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libFiltre
ConfigurationName      :=Release
WorkspacePath          := "/home/figuinha/dev/Regards"
ProjectPath            := "/home/figuinha/dev/Regards/libFiltre"
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=24/04/16
CodeLitePath           :="/home/figuinha/.codelite"
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
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
Preprocessors          :=$(PreprocessorSwitch)NDEBUG $(PreprocessorSwitch)__WXGTK__ 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libFiltre.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../libOpenCL $(IncludeSwitch)../libResource $(IncludeSwitch)../include $(IncludeSwitch)../libextern/wxWidgets-3.0.2/include $(IncludeSwitch)../libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0 $(IncludeSwitch)../libControl 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -fopenmp -std=c++11 -Wall  $(Preprocessors)
CFLAGS   :=  -O2 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/Color.cpp$(ObjectSuffix) $(IntermediateDirectory)/Crop.cpp$(ObjectSuffix) $(IntermediateDirectory)/Draw.cpp$(ObjectSuffix) $(IntermediateDirectory)/Filtre.cpp$(ObjectSuffix) $(IntermediateDirectory)/FiltreEffet.cpp$(ObjectSuffix) $(IntermediateDirectory)/FiltreEffetCPU.cpp$(ObjectSuffix) $(IntermediateDirectory)/Histogramme.cpp$(ObjectSuffix) $(IntermediateDirectory)/Interpolation.cpp$(ObjectSuffix) $(IntermediateDirectory)/InterpolationBicubic.cpp$(ObjectSuffix) $(IntermediateDirectory)/InterpolationBilinear.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/LensFlare.cpp$(ObjectSuffix) $(IntermediateDirectory)/MotionBlur.cpp$(ObjectSuffix) $(IntermediateDirectory)/PerlinNoise.cpp$(ObjectSuffix) $(IntermediateDirectory)/RedEye.cpp$(ObjectSuffix) $(IntermediateDirectory)/Rotate.cpp$(ObjectSuffix) $(IntermediateDirectory)/Rotation.cpp$(ObjectSuffix) $(IntermediateDirectory)/Selection.cpp$(ObjectSuffix) $(IntermediateDirectory)/GaussianBlur.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "/home/figuinha/dev/Regards/.build-release"
	@echo rebuilt > "/home/figuinha/dev/Regards/.build-release/libFiltre"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


./Release:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/Color.cpp$(ObjectSuffix): Color.cpp $(IntermediateDirectory)/Color.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libFiltre/Color.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Color.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Color.cpp$(DependSuffix): Color.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Color.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Color.cpp$(DependSuffix) -MM "Color.cpp"

$(IntermediateDirectory)/Color.cpp$(PreprocessSuffix): Color.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Color.cpp$(PreprocessSuffix) "Color.cpp"

$(IntermediateDirectory)/Crop.cpp$(ObjectSuffix): Crop.cpp $(IntermediateDirectory)/Crop.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libFiltre/Crop.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Crop.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Crop.cpp$(DependSuffix): Crop.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Crop.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Crop.cpp$(DependSuffix) -MM "Crop.cpp"

$(IntermediateDirectory)/Crop.cpp$(PreprocessSuffix): Crop.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Crop.cpp$(PreprocessSuffix) "Crop.cpp"

$(IntermediateDirectory)/Draw.cpp$(ObjectSuffix): Draw.cpp $(IntermediateDirectory)/Draw.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libFiltre/Draw.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Draw.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Draw.cpp$(DependSuffix): Draw.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Draw.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Draw.cpp$(DependSuffix) -MM "Draw.cpp"

$(IntermediateDirectory)/Draw.cpp$(PreprocessSuffix): Draw.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Draw.cpp$(PreprocessSuffix) "Draw.cpp"

$(IntermediateDirectory)/Filtre.cpp$(ObjectSuffix): Filtre.cpp $(IntermediateDirectory)/Filtre.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libFiltre/Filtre.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Filtre.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Filtre.cpp$(DependSuffix): Filtre.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Filtre.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Filtre.cpp$(DependSuffix) -MM "Filtre.cpp"

$(IntermediateDirectory)/Filtre.cpp$(PreprocessSuffix): Filtre.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Filtre.cpp$(PreprocessSuffix) "Filtre.cpp"

$(IntermediateDirectory)/FiltreEffet.cpp$(ObjectSuffix): FiltreEffet.cpp $(IntermediateDirectory)/FiltreEffet.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libFiltre/FiltreEffet.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FiltreEffet.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FiltreEffet.cpp$(DependSuffix): FiltreEffet.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FiltreEffet.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FiltreEffet.cpp$(DependSuffix) -MM "FiltreEffet.cpp"

$(IntermediateDirectory)/FiltreEffet.cpp$(PreprocessSuffix): FiltreEffet.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FiltreEffet.cpp$(PreprocessSuffix) "FiltreEffet.cpp"

$(IntermediateDirectory)/FiltreEffetCPU.cpp$(ObjectSuffix): FiltreEffetCPU.cpp $(IntermediateDirectory)/FiltreEffetCPU.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libFiltre/FiltreEffetCPU.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FiltreEffetCPU.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FiltreEffetCPU.cpp$(DependSuffix): FiltreEffetCPU.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FiltreEffetCPU.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FiltreEffetCPU.cpp$(DependSuffix) -MM "FiltreEffetCPU.cpp"

$(IntermediateDirectory)/FiltreEffetCPU.cpp$(PreprocessSuffix): FiltreEffetCPU.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FiltreEffetCPU.cpp$(PreprocessSuffix) "FiltreEffetCPU.cpp"

$(IntermediateDirectory)/Histogramme.cpp$(ObjectSuffix): Histogramme.cpp $(IntermediateDirectory)/Histogramme.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libFiltre/Histogramme.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Histogramme.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Histogramme.cpp$(DependSuffix): Histogramme.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Histogramme.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Histogramme.cpp$(DependSuffix) -MM "Histogramme.cpp"

$(IntermediateDirectory)/Histogramme.cpp$(PreprocessSuffix): Histogramme.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Histogramme.cpp$(PreprocessSuffix) "Histogramme.cpp"

$(IntermediateDirectory)/Interpolation.cpp$(ObjectSuffix): Interpolation.cpp $(IntermediateDirectory)/Interpolation.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libFiltre/Interpolation.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Interpolation.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Interpolation.cpp$(DependSuffix): Interpolation.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Interpolation.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Interpolation.cpp$(DependSuffix) -MM "Interpolation.cpp"

$(IntermediateDirectory)/Interpolation.cpp$(PreprocessSuffix): Interpolation.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Interpolation.cpp$(PreprocessSuffix) "Interpolation.cpp"

$(IntermediateDirectory)/InterpolationBicubic.cpp$(ObjectSuffix): InterpolationBicubic.cpp $(IntermediateDirectory)/InterpolationBicubic.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libFiltre/InterpolationBicubic.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InterpolationBicubic.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InterpolationBicubic.cpp$(DependSuffix): InterpolationBicubic.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InterpolationBicubic.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InterpolationBicubic.cpp$(DependSuffix) -MM "InterpolationBicubic.cpp"

$(IntermediateDirectory)/InterpolationBicubic.cpp$(PreprocessSuffix): InterpolationBicubic.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InterpolationBicubic.cpp$(PreprocessSuffix) "InterpolationBicubic.cpp"

$(IntermediateDirectory)/InterpolationBilinear.cpp$(ObjectSuffix): InterpolationBilinear.cpp $(IntermediateDirectory)/InterpolationBilinear.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libFiltre/InterpolationBilinear.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InterpolationBilinear.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InterpolationBilinear.cpp$(DependSuffix): InterpolationBilinear.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InterpolationBilinear.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InterpolationBilinear.cpp$(DependSuffix) -MM "InterpolationBilinear.cpp"

$(IntermediateDirectory)/InterpolationBilinear.cpp$(PreprocessSuffix): InterpolationBilinear.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InterpolationBilinear.cpp$(PreprocessSuffix) "InterpolationBilinear.cpp"

$(IntermediateDirectory)/LensFlare.cpp$(ObjectSuffix): LensFlare.cpp $(IntermediateDirectory)/LensFlare.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libFiltre/LensFlare.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LensFlare.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LensFlare.cpp$(DependSuffix): LensFlare.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LensFlare.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LensFlare.cpp$(DependSuffix) -MM "LensFlare.cpp"

$(IntermediateDirectory)/LensFlare.cpp$(PreprocessSuffix): LensFlare.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LensFlare.cpp$(PreprocessSuffix) "LensFlare.cpp"

$(IntermediateDirectory)/MotionBlur.cpp$(ObjectSuffix): MotionBlur.cpp $(IntermediateDirectory)/MotionBlur.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libFiltre/MotionBlur.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MotionBlur.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MotionBlur.cpp$(DependSuffix): MotionBlur.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MotionBlur.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MotionBlur.cpp$(DependSuffix) -MM "MotionBlur.cpp"

$(IntermediateDirectory)/MotionBlur.cpp$(PreprocessSuffix): MotionBlur.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MotionBlur.cpp$(PreprocessSuffix) "MotionBlur.cpp"

$(IntermediateDirectory)/PerlinNoise.cpp$(ObjectSuffix): PerlinNoise.cpp $(IntermediateDirectory)/PerlinNoise.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libFiltre/PerlinNoise.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PerlinNoise.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PerlinNoise.cpp$(DependSuffix): PerlinNoise.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PerlinNoise.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PerlinNoise.cpp$(DependSuffix) -MM "PerlinNoise.cpp"

$(IntermediateDirectory)/PerlinNoise.cpp$(PreprocessSuffix): PerlinNoise.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PerlinNoise.cpp$(PreprocessSuffix) "PerlinNoise.cpp"

$(IntermediateDirectory)/RedEye.cpp$(ObjectSuffix): RedEye.cpp $(IntermediateDirectory)/RedEye.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libFiltre/RedEye.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RedEye.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RedEye.cpp$(DependSuffix): RedEye.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RedEye.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RedEye.cpp$(DependSuffix) -MM "RedEye.cpp"

$(IntermediateDirectory)/RedEye.cpp$(PreprocessSuffix): RedEye.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RedEye.cpp$(PreprocessSuffix) "RedEye.cpp"

$(IntermediateDirectory)/Rotate.cpp$(ObjectSuffix): Rotate.cpp $(IntermediateDirectory)/Rotate.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libFiltre/Rotate.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Rotate.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Rotate.cpp$(DependSuffix): Rotate.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Rotate.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Rotate.cpp$(DependSuffix) -MM "Rotate.cpp"

$(IntermediateDirectory)/Rotate.cpp$(PreprocessSuffix): Rotate.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Rotate.cpp$(PreprocessSuffix) "Rotate.cpp"

$(IntermediateDirectory)/Rotation.cpp$(ObjectSuffix): Rotation.cpp $(IntermediateDirectory)/Rotation.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libFiltre/Rotation.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Rotation.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Rotation.cpp$(DependSuffix): Rotation.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Rotation.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Rotation.cpp$(DependSuffix) -MM "Rotation.cpp"

$(IntermediateDirectory)/Rotation.cpp$(PreprocessSuffix): Rotation.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Rotation.cpp$(PreprocessSuffix) "Rotation.cpp"

$(IntermediateDirectory)/Selection.cpp$(ObjectSuffix): Selection.cpp $(IntermediateDirectory)/Selection.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libFiltre/Selection.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Selection.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Selection.cpp$(DependSuffix): Selection.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Selection.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Selection.cpp$(DependSuffix) -MM "Selection.cpp"

$(IntermediateDirectory)/Selection.cpp$(PreprocessSuffix): Selection.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Selection.cpp$(PreprocessSuffix) "Selection.cpp"

$(IntermediateDirectory)/GaussianBlur.cpp$(ObjectSuffix): GaussianBlur.cpp $(IntermediateDirectory)/GaussianBlur.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libFiltre/GaussianBlur.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GaussianBlur.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GaussianBlur.cpp$(DependSuffix): GaussianBlur.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GaussianBlur.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GaussianBlur.cpp$(DependSuffix) -MM "GaussianBlur.cpp"

$(IntermediateDirectory)/GaussianBlur.cpp$(PreprocessSuffix): GaussianBlur.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GaussianBlur.cpp$(PreprocessSuffix) "GaussianBlur.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


