##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libPsd
ConfigurationName      :=Release
WorkspacePath          := "/home/figuinha/dev/Regards/Otherlib"
ProjectPath            := "/home/figuinha/dev/Regards/Otherlib/libPsd"
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=root
Date                   :=21/09/15
CodeLitePath           :="/home/figuinha/.codelite"
LinkerName             :=/usr/bin/g++-4.8
SharedObjectLinkerName :=/usr/bin/g++-4.8 -shared -fPIC
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
OutputFile             :=$(IntermediateDirectory)/lib$(ProjectName).a
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libPsd.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
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
CXX      := /usr/bin/g++-4.8
CC       := /usr/bin/gcc-4.8
CXXFLAGS :=   $(Preprocessors)
CFLAGS   :=   $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/adjustment.c$(ObjectSuffix) $(IntermediateDirectory)/bevel_emboss.c$(ObjectSuffix) $(IntermediateDirectory)/bitmap.c$(ObjectSuffix) $(IntermediateDirectory)/blend.c$(ObjectSuffix) $(IntermediateDirectory)/boundary.c$(ObjectSuffix) $(IntermediateDirectory)/brightness_contrast.c$(ObjectSuffix) $(IntermediateDirectory)/channel_image.c$(ObjectSuffix) $(IntermediateDirectory)/channel_mixer.c$(ObjectSuffix) $(IntermediateDirectory)/color.c$(ObjectSuffix) $(IntermediateDirectory)/color_balance.c$(ObjectSuffix) \
	$(IntermediateDirectory)/color_mode.c$(ObjectSuffix) $(IntermediateDirectory)/color_overlay.c$(ObjectSuffix) $(IntermediateDirectory)/curves.c$(ObjectSuffix) $(IntermediateDirectory)/descriptor.c$(ObjectSuffix) $(IntermediateDirectory)/drop_shadow.c$(ObjectSuffix) $(IntermediateDirectory)/effects.c$(ObjectSuffix) $(IntermediateDirectory)/file_header.c$(ObjectSuffix) $(IntermediateDirectory)/fixed.c$(ObjectSuffix) $(IntermediateDirectory)/gaussian_blur.c$(ObjectSuffix) $(IntermediateDirectory)/gradient_blend.c$(ObjectSuffix) \
	$(IntermediateDirectory)/gradient_fill.c$(ObjectSuffix) $(IntermediateDirectory)/gradient_map.c$(ObjectSuffix) $(IntermediateDirectory)/gradient_overlay.c$(ObjectSuffix) $(IntermediateDirectory)/hue_saturation.c$(ObjectSuffix) $(IntermediateDirectory)/image_data.c$(ObjectSuffix) $(IntermediateDirectory)/image_resource.c$(ObjectSuffix) $(IntermediateDirectory)/inner_glow.c$(ObjectSuffix) $(IntermediateDirectory)/inner_shadow.c$(ObjectSuffix) $(IntermediateDirectory)/invert.c$(ObjectSuffix) $(IntermediateDirectory)/layer_mask.c$(ObjectSuffix) \
	$(IntermediateDirectory)/levels.c$(ObjectSuffix) $(IntermediateDirectory)/outer_glow.c$(ObjectSuffix) $(IntermediateDirectory)/path.c$(ObjectSuffix) $(IntermediateDirectory)/pattern.c$(ObjectSuffix) $(IntermediateDirectory)/pattern_fill.c$(ObjectSuffix) $(IntermediateDirectory)/pattern_overlay.c$(ObjectSuffix) $(IntermediateDirectory)/photo_filter.c$(ObjectSuffix) $(IntermediateDirectory)/posterize.c$(ObjectSuffix) $(IntermediateDirectory)/psd.c$(ObjectSuffix) $(IntermediateDirectory)/psd_system.c$(ObjectSuffix) \
	$(IntermediateDirectory)/psd_zip.c$(ObjectSuffix) $(IntermediateDirectory)/rect.c$(ObjectSuffix) $(IntermediateDirectory)/satin.c$(ObjectSuffix) $(IntermediateDirectory)/selective_color.c$(ObjectSuffix) $(IntermediateDirectory)/solid_color.c$(ObjectSuffix) $(IntermediateDirectory)/stream.c$(ObjectSuffix) $(IntermediateDirectory)/stroke.c$(ObjectSuffix) $(IntermediateDirectory)/test.c$(ObjectSuffix) $(IntermediateDirectory)/threshold.c$(ObjectSuffix) $(IntermediateDirectory)/thumbnail.c$(ObjectSuffix) \
	$(IntermediateDirectory)/type_tool.c$(ObjectSuffix) 



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
	@$(MakeDirCommand) "/home/figuinha/dev/Regards/Otherlib/.build-release"
	@echo rebuilt > "/home/figuinha/dev/Regards/Otherlib/.build-release/libPsd"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


./Release:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/adjustment.c$(ObjectSuffix): adjustment.c $(IntermediateDirectory)/adjustment.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/adjustment.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/adjustment.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/adjustment.c$(DependSuffix): adjustment.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/adjustment.c$(ObjectSuffix) -MF$(IntermediateDirectory)/adjustment.c$(DependSuffix) -MM "adjustment.c"

$(IntermediateDirectory)/adjustment.c$(PreprocessSuffix): adjustment.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/adjustment.c$(PreprocessSuffix) "adjustment.c"

$(IntermediateDirectory)/bevel_emboss.c$(ObjectSuffix): bevel_emboss.c $(IntermediateDirectory)/bevel_emboss.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/bevel_emboss.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bevel_emboss.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bevel_emboss.c$(DependSuffix): bevel_emboss.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bevel_emboss.c$(ObjectSuffix) -MF$(IntermediateDirectory)/bevel_emboss.c$(DependSuffix) -MM "bevel_emboss.c"

$(IntermediateDirectory)/bevel_emboss.c$(PreprocessSuffix): bevel_emboss.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bevel_emboss.c$(PreprocessSuffix) "bevel_emboss.c"

$(IntermediateDirectory)/bitmap.c$(ObjectSuffix): bitmap.c $(IntermediateDirectory)/bitmap.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/bitmap.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bitmap.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bitmap.c$(DependSuffix): bitmap.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bitmap.c$(ObjectSuffix) -MF$(IntermediateDirectory)/bitmap.c$(DependSuffix) -MM "bitmap.c"

$(IntermediateDirectory)/bitmap.c$(PreprocessSuffix): bitmap.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bitmap.c$(PreprocessSuffix) "bitmap.c"

$(IntermediateDirectory)/blend.c$(ObjectSuffix): blend.c $(IntermediateDirectory)/blend.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/blend.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/blend.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/blend.c$(DependSuffix): blend.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/blend.c$(ObjectSuffix) -MF$(IntermediateDirectory)/blend.c$(DependSuffix) -MM "blend.c"

$(IntermediateDirectory)/blend.c$(PreprocessSuffix): blend.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/blend.c$(PreprocessSuffix) "blend.c"

$(IntermediateDirectory)/boundary.c$(ObjectSuffix): boundary.c $(IntermediateDirectory)/boundary.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/boundary.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/boundary.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/boundary.c$(DependSuffix): boundary.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/boundary.c$(ObjectSuffix) -MF$(IntermediateDirectory)/boundary.c$(DependSuffix) -MM "boundary.c"

$(IntermediateDirectory)/boundary.c$(PreprocessSuffix): boundary.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/boundary.c$(PreprocessSuffix) "boundary.c"

$(IntermediateDirectory)/brightness_contrast.c$(ObjectSuffix): brightness_contrast.c $(IntermediateDirectory)/brightness_contrast.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/brightness_contrast.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/brightness_contrast.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/brightness_contrast.c$(DependSuffix): brightness_contrast.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/brightness_contrast.c$(ObjectSuffix) -MF$(IntermediateDirectory)/brightness_contrast.c$(DependSuffix) -MM "brightness_contrast.c"

$(IntermediateDirectory)/brightness_contrast.c$(PreprocessSuffix): brightness_contrast.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/brightness_contrast.c$(PreprocessSuffix) "brightness_contrast.c"

$(IntermediateDirectory)/channel_image.c$(ObjectSuffix): channel_image.c $(IntermediateDirectory)/channel_image.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/channel_image.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/channel_image.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/channel_image.c$(DependSuffix): channel_image.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/channel_image.c$(ObjectSuffix) -MF$(IntermediateDirectory)/channel_image.c$(DependSuffix) -MM "channel_image.c"

$(IntermediateDirectory)/channel_image.c$(PreprocessSuffix): channel_image.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/channel_image.c$(PreprocessSuffix) "channel_image.c"

$(IntermediateDirectory)/channel_mixer.c$(ObjectSuffix): channel_mixer.c $(IntermediateDirectory)/channel_mixer.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/channel_mixer.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/channel_mixer.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/channel_mixer.c$(DependSuffix): channel_mixer.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/channel_mixer.c$(ObjectSuffix) -MF$(IntermediateDirectory)/channel_mixer.c$(DependSuffix) -MM "channel_mixer.c"

$(IntermediateDirectory)/channel_mixer.c$(PreprocessSuffix): channel_mixer.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/channel_mixer.c$(PreprocessSuffix) "channel_mixer.c"

$(IntermediateDirectory)/color.c$(ObjectSuffix): color.c $(IntermediateDirectory)/color.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/color.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/color.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/color.c$(DependSuffix): color.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/color.c$(ObjectSuffix) -MF$(IntermediateDirectory)/color.c$(DependSuffix) -MM "color.c"

$(IntermediateDirectory)/color.c$(PreprocessSuffix): color.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/color.c$(PreprocessSuffix) "color.c"

$(IntermediateDirectory)/color_balance.c$(ObjectSuffix): color_balance.c $(IntermediateDirectory)/color_balance.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/color_balance.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/color_balance.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/color_balance.c$(DependSuffix): color_balance.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/color_balance.c$(ObjectSuffix) -MF$(IntermediateDirectory)/color_balance.c$(DependSuffix) -MM "color_balance.c"

$(IntermediateDirectory)/color_balance.c$(PreprocessSuffix): color_balance.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/color_balance.c$(PreprocessSuffix) "color_balance.c"

$(IntermediateDirectory)/color_mode.c$(ObjectSuffix): color_mode.c $(IntermediateDirectory)/color_mode.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/color_mode.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/color_mode.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/color_mode.c$(DependSuffix): color_mode.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/color_mode.c$(ObjectSuffix) -MF$(IntermediateDirectory)/color_mode.c$(DependSuffix) -MM "color_mode.c"

$(IntermediateDirectory)/color_mode.c$(PreprocessSuffix): color_mode.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/color_mode.c$(PreprocessSuffix) "color_mode.c"

$(IntermediateDirectory)/color_overlay.c$(ObjectSuffix): color_overlay.c $(IntermediateDirectory)/color_overlay.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/color_overlay.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/color_overlay.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/color_overlay.c$(DependSuffix): color_overlay.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/color_overlay.c$(ObjectSuffix) -MF$(IntermediateDirectory)/color_overlay.c$(DependSuffix) -MM "color_overlay.c"

$(IntermediateDirectory)/color_overlay.c$(PreprocessSuffix): color_overlay.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/color_overlay.c$(PreprocessSuffix) "color_overlay.c"

$(IntermediateDirectory)/curves.c$(ObjectSuffix): curves.c $(IntermediateDirectory)/curves.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/curves.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/curves.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/curves.c$(DependSuffix): curves.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/curves.c$(ObjectSuffix) -MF$(IntermediateDirectory)/curves.c$(DependSuffix) -MM "curves.c"

$(IntermediateDirectory)/curves.c$(PreprocessSuffix): curves.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/curves.c$(PreprocessSuffix) "curves.c"

$(IntermediateDirectory)/descriptor.c$(ObjectSuffix): descriptor.c $(IntermediateDirectory)/descriptor.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/descriptor.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/descriptor.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/descriptor.c$(DependSuffix): descriptor.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/descriptor.c$(ObjectSuffix) -MF$(IntermediateDirectory)/descriptor.c$(DependSuffix) -MM "descriptor.c"

$(IntermediateDirectory)/descriptor.c$(PreprocessSuffix): descriptor.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/descriptor.c$(PreprocessSuffix) "descriptor.c"

$(IntermediateDirectory)/drop_shadow.c$(ObjectSuffix): drop_shadow.c $(IntermediateDirectory)/drop_shadow.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/drop_shadow.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/drop_shadow.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/drop_shadow.c$(DependSuffix): drop_shadow.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/drop_shadow.c$(ObjectSuffix) -MF$(IntermediateDirectory)/drop_shadow.c$(DependSuffix) -MM "drop_shadow.c"

$(IntermediateDirectory)/drop_shadow.c$(PreprocessSuffix): drop_shadow.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/drop_shadow.c$(PreprocessSuffix) "drop_shadow.c"

$(IntermediateDirectory)/effects.c$(ObjectSuffix): effects.c $(IntermediateDirectory)/effects.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/effects.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/effects.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/effects.c$(DependSuffix): effects.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/effects.c$(ObjectSuffix) -MF$(IntermediateDirectory)/effects.c$(DependSuffix) -MM "effects.c"

$(IntermediateDirectory)/effects.c$(PreprocessSuffix): effects.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/effects.c$(PreprocessSuffix) "effects.c"

$(IntermediateDirectory)/file_header.c$(ObjectSuffix): file_header.c $(IntermediateDirectory)/file_header.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/file_header.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/file_header.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/file_header.c$(DependSuffix): file_header.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/file_header.c$(ObjectSuffix) -MF$(IntermediateDirectory)/file_header.c$(DependSuffix) -MM "file_header.c"

$(IntermediateDirectory)/file_header.c$(PreprocessSuffix): file_header.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/file_header.c$(PreprocessSuffix) "file_header.c"

$(IntermediateDirectory)/fixed.c$(ObjectSuffix): fixed.c $(IntermediateDirectory)/fixed.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/fixed.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/fixed.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/fixed.c$(DependSuffix): fixed.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/fixed.c$(ObjectSuffix) -MF$(IntermediateDirectory)/fixed.c$(DependSuffix) -MM "fixed.c"

$(IntermediateDirectory)/fixed.c$(PreprocessSuffix): fixed.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/fixed.c$(PreprocessSuffix) "fixed.c"

$(IntermediateDirectory)/gaussian_blur.c$(ObjectSuffix): gaussian_blur.c $(IntermediateDirectory)/gaussian_blur.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/gaussian_blur.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/gaussian_blur.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/gaussian_blur.c$(DependSuffix): gaussian_blur.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/gaussian_blur.c$(ObjectSuffix) -MF$(IntermediateDirectory)/gaussian_blur.c$(DependSuffix) -MM "gaussian_blur.c"

$(IntermediateDirectory)/gaussian_blur.c$(PreprocessSuffix): gaussian_blur.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/gaussian_blur.c$(PreprocessSuffix) "gaussian_blur.c"

$(IntermediateDirectory)/gradient_blend.c$(ObjectSuffix): gradient_blend.c $(IntermediateDirectory)/gradient_blend.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/gradient_blend.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/gradient_blend.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/gradient_blend.c$(DependSuffix): gradient_blend.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/gradient_blend.c$(ObjectSuffix) -MF$(IntermediateDirectory)/gradient_blend.c$(DependSuffix) -MM "gradient_blend.c"

$(IntermediateDirectory)/gradient_blend.c$(PreprocessSuffix): gradient_blend.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/gradient_blend.c$(PreprocessSuffix) "gradient_blend.c"

$(IntermediateDirectory)/gradient_fill.c$(ObjectSuffix): gradient_fill.c $(IntermediateDirectory)/gradient_fill.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/gradient_fill.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/gradient_fill.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/gradient_fill.c$(DependSuffix): gradient_fill.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/gradient_fill.c$(ObjectSuffix) -MF$(IntermediateDirectory)/gradient_fill.c$(DependSuffix) -MM "gradient_fill.c"

$(IntermediateDirectory)/gradient_fill.c$(PreprocessSuffix): gradient_fill.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/gradient_fill.c$(PreprocessSuffix) "gradient_fill.c"

$(IntermediateDirectory)/gradient_map.c$(ObjectSuffix): gradient_map.c $(IntermediateDirectory)/gradient_map.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/gradient_map.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/gradient_map.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/gradient_map.c$(DependSuffix): gradient_map.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/gradient_map.c$(ObjectSuffix) -MF$(IntermediateDirectory)/gradient_map.c$(DependSuffix) -MM "gradient_map.c"

$(IntermediateDirectory)/gradient_map.c$(PreprocessSuffix): gradient_map.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/gradient_map.c$(PreprocessSuffix) "gradient_map.c"

$(IntermediateDirectory)/gradient_overlay.c$(ObjectSuffix): gradient_overlay.c $(IntermediateDirectory)/gradient_overlay.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/gradient_overlay.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/gradient_overlay.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/gradient_overlay.c$(DependSuffix): gradient_overlay.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/gradient_overlay.c$(ObjectSuffix) -MF$(IntermediateDirectory)/gradient_overlay.c$(DependSuffix) -MM "gradient_overlay.c"

$(IntermediateDirectory)/gradient_overlay.c$(PreprocessSuffix): gradient_overlay.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/gradient_overlay.c$(PreprocessSuffix) "gradient_overlay.c"

$(IntermediateDirectory)/hue_saturation.c$(ObjectSuffix): hue_saturation.c $(IntermediateDirectory)/hue_saturation.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/hue_saturation.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/hue_saturation.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/hue_saturation.c$(DependSuffix): hue_saturation.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/hue_saturation.c$(ObjectSuffix) -MF$(IntermediateDirectory)/hue_saturation.c$(DependSuffix) -MM "hue_saturation.c"

$(IntermediateDirectory)/hue_saturation.c$(PreprocessSuffix): hue_saturation.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/hue_saturation.c$(PreprocessSuffix) "hue_saturation.c"

$(IntermediateDirectory)/image_data.c$(ObjectSuffix): image_data.c $(IntermediateDirectory)/image_data.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/image_data.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/image_data.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/image_data.c$(DependSuffix): image_data.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/image_data.c$(ObjectSuffix) -MF$(IntermediateDirectory)/image_data.c$(DependSuffix) -MM "image_data.c"

$(IntermediateDirectory)/image_data.c$(PreprocessSuffix): image_data.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/image_data.c$(PreprocessSuffix) "image_data.c"

$(IntermediateDirectory)/image_resource.c$(ObjectSuffix): image_resource.c $(IntermediateDirectory)/image_resource.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/image_resource.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/image_resource.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/image_resource.c$(DependSuffix): image_resource.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/image_resource.c$(ObjectSuffix) -MF$(IntermediateDirectory)/image_resource.c$(DependSuffix) -MM "image_resource.c"

$(IntermediateDirectory)/image_resource.c$(PreprocessSuffix): image_resource.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/image_resource.c$(PreprocessSuffix) "image_resource.c"

$(IntermediateDirectory)/inner_glow.c$(ObjectSuffix): inner_glow.c $(IntermediateDirectory)/inner_glow.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/inner_glow.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/inner_glow.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/inner_glow.c$(DependSuffix): inner_glow.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/inner_glow.c$(ObjectSuffix) -MF$(IntermediateDirectory)/inner_glow.c$(DependSuffix) -MM "inner_glow.c"

$(IntermediateDirectory)/inner_glow.c$(PreprocessSuffix): inner_glow.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/inner_glow.c$(PreprocessSuffix) "inner_glow.c"

$(IntermediateDirectory)/inner_shadow.c$(ObjectSuffix): inner_shadow.c $(IntermediateDirectory)/inner_shadow.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/inner_shadow.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/inner_shadow.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/inner_shadow.c$(DependSuffix): inner_shadow.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/inner_shadow.c$(ObjectSuffix) -MF$(IntermediateDirectory)/inner_shadow.c$(DependSuffix) -MM "inner_shadow.c"

$(IntermediateDirectory)/inner_shadow.c$(PreprocessSuffix): inner_shadow.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/inner_shadow.c$(PreprocessSuffix) "inner_shadow.c"

$(IntermediateDirectory)/invert.c$(ObjectSuffix): invert.c $(IntermediateDirectory)/invert.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/invert.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/invert.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/invert.c$(DependSuffix): invert.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/invert.c$(ObjectSuffix) -MF$(IntermediateDirectory)/invert.c$(DependSuffix) -MM "invert.c"

$(IntermediateDirectory)/invert.c$(PreprocessSuffix): invert.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/invert.c$(PreprocessSuffix) "invert.c"

$(IntermediateDirectory)/layer_mask.c$(ObjectSuffix): layer_mask.c $(IntermediateDirectory)/layer_mask.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/layer_mask.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/layer_mask.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/layer_mask.c$(DependSuffix): layer_mask.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/layer_mask.c$(ObjectSuffix) -MF$(IntermediateDirectory)/layer_mask.c$(DependSuffix) -MM "layer_mask.c"

$(IntermediateDirectory)/layer_mask.c$(PreprocessSuffix): layer_mask.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/layer_mask.c$(PreprocessSuffix) "layer_mask.c"

$(IntermediateDirectory)/levels.c$(ObjectSuffix): levels.c $(IntermediateDirectory)/levels.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/levels.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/levels.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/levels.c$(DependSuffix): levels.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/levels.c$(ObjectSuffix) -MF$(IntermediateDirectory)/levels.c$(DependSuffix) -MM "levels.c"

$(IntermediateDirectory)/levels.c$(PreprocessSuffix): levels.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/levels.c$(PreprocessSuffix) "levels.c"

$(IntermediateDirectory)/outer_glow.c$(ObjectSuffix): outer_glow.c $(IntermediateDirectory)/outer_glow.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/outer_glow.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/outer_glow.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/outer_glow.c$(DependSuffix): outer_glow.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/outer_glow.c$(ObjectSuffix) -MF$(IntermediateDirectory)/outer_glow.c$(DependSuffix) -MM "outer_glow.c"

$(IntermediateDirectory)/outer_glow.c$(PreprocessSuffix): outer_glow.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/outer_glow.c$(PreprocessSuffix) "outer_glow.c"

$(IntermediateDirectory)/path.c$(ObjectSuffix): path.c $(IntermediateDirectory)/path.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/path.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/path.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/path.c$(DependSuffix): path.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/path.c$(ObjectSuffix) -MF$(IntermediateDirectory)/path.c$(DependSuffix) -MM "path.c"

$(IntermediateDirectory)/path.c$(PreprocessSuffix): path.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/path.c$(PreprocessSuffix) "path.c"

$(IntermediateDirectory)/pattern.c$(ObjectSuffix): pattern.c $(IntermediateDirectory)/pattern.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/pattern.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/pattern.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/pattern.c$(DependSuffix): pattern.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/pattern.c$(ObjectSuffix) -MF$(IntermediateDirectory)/pattern.c$(DependSuffix) -MM "pattern.c"

$(IntermediateDirectory)/pattern.c$(PreprocessSuffix): pattern.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/pattern.c$(PreprocessSuffix) "pattern.c"

$(IntermediateDirectory)/pattern_fill.c$(ObjectSuffix): pattern_fill.c $(IntermediateDirectory)/pattern_fill.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/pattern_fill.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/pattern_fill.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/pattern_fill.c$(DependSuffix): pattern_fill.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/pattern_fill.c$(ObjectSuffix) -MF$(IntermediateDirectory)/pattern_fill.c$(DependSuffix) -MM "pattern_fill.c"

$(IntermediateDirectory)/pattern_fill.c$(PreprocessSuffix): pattern_fill.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/pattern_fill.c$(PreprocessSuffix) "pattern_fill.c"

$(IntermediateDirectory)/pattern_overlay.c$(ObjectSuffix): pattern_overlay.c $(IntermediateDirectory)/pattern_overlay.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/pattern_overlay.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/pattern_overlay.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/pattern_overlay.c$(DependSuffix): pattern_overlay.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/pattern_overlay.c$(ObjectSuffix) -MF$(IntermediateDirectory)/pattern_overlay.c$(DependSuffix) -MM "pattern_overlay.c"

$(IntermediateDirectory)/pattern_overlay.c$(PreprocessSuffix): pattern_overlay.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/pattern_overlay.c$(PreprocessSuffix) "pattern_overlay.c"

$(IntermediateDirectory)/photo_filter.c$(ObjectSuffix): photo_filter.c $(IntermediateDirectory)/photo_filter.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/photo_filter.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/photo_filter.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/photo_filter.c$(DependSuffix): photo_filter.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/photo_filter.c$(ObjectSuffix) -MF$(IntermediateDirectory)/photo_filter.c$(DependSuffix) -MM "photo_filter.c"

$(IntermediateDirectory)/photo_filter.c$(PreprocessSuffix): photo_filter.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/photo_filter.c$(PreprocessSuffix) "photo_filter.c"

$(IntermediateDirectory)/posterize.c$(ObjectSuffix): posterize.c $(IntermediateDirectory)/posterize.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/posterize.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/posterize.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/posterize.c$(DependSuffix): posterize.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/posterize.c$(ObjectSuffix) -MF$(IntermediateDirectory)/posterize.c$(DependSuffix) -MM "posterize.c"

$(IntermediateDirectory)/posterize.c$(PreprocessSuffix): posterize.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/posterize.c$(PreprocessSuffix) "posterize.c"

$(IntermediateDirectory)/psd.c$(ObjectSuffix): psd.c $(IntermediateDirectory)/psd.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/psd.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/psd.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/psd.c$(DependSuffix): psd.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/psd.c$(ObjectSuffix) -MF$(IntermediateDirectory)/psd.c$(DependSuffix) -MM "psd.c"

$(IntermediateDirectory)/psd.c$(PreprocessSuffix): psd.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/psd.c$(PreprocessSuffix) "psd.c"

$(IntermediateDirectory)/psd_system.c$(ObjectSuffix): psd_system.c $(IntermediateDirectory)/psd_system.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/psd_system.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/psd_system.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/psd_system.c$(DependSuffix): psd_system.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/psd_system.c$(ObjectSuffix) -MF$(IntermediateDirectory)/psd_system.c$(DependSuffix) -MM "psd_system.c"

$(IntermediateDirectory)/psd_system.c$(PreprocessSuffix): psd_system.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/psd_system.c$(PreprocessSuffix) "psd_system.c"

$(IntermediateDirectory)/psd_zip.c$(ObjectSuffix): psd_zip.c $(IntermediateDirectory)/psd_zip.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/psd_zip.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/psd_zip.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/psd_zip.c$(DependSuffix): psd_zip.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/psd_zip.c$(ObjectSuffix) -MF$(IntermediateDirectory)/psd_zip.c$(DependSuffix) -MM "psd_zip.c"

$(IntermediateDirectory)/psd_zip.c$(PreprocessSuffix): psd_zip.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/psd_zip.c$(PreprocessSuffix) "psd_zip.c"

$(IntermediateDirectory)/rect.c$(ObjectSuffix): rect.c $(IntermediateDirectory)/rect.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/rect.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/rect.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/rect.c$(DependSuffix): rect.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/rect.c$(ObjectSuffix) -MF$(IntermediateDirectory)/rect.c$(DependSuffix) -MM "rect.c"

$(IntermediateDirectory)/rect.c$(PreprocessSuffix): rect.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/rect.c$(PreprocessSuffix) "rect.c"

$(IntermediateDirectory)/satin.c$(ObjectSuffix): satin.c $(IntermediateDirectory)/satin.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/satin.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/satin.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/satin.c$(DependSuffix): satin.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/satin.c$(ObjectSuffix) -MF$(IntermediateDirectory)/satin.c$(DependSuffix) -MM "satin.c"

$(IntermediateDirectory)/satin.c$(PreprocessSuffix): satin.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/satin.c$(PreprocessSuffix) "satin.c"

$(IntermediateDirectory)/selective_color.c$(ObjectSuffix): selective_color.c $(IntermediateDirectory)/selective_color.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/selective_color.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/selective_color.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/selective_color.c$(DependSuffix): selective_color.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/selective_color.c$(ObjectSuffix) -MF$(IntermediateDirectory)/selective_color.c$(DependSuffix) -MM "selective_color.c"

$(IntermediateDirectory)/selective_color.c$(PreprocessSuffix): selective_color.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/selective_color.c$(PreprocessSuffix) "selective_color.c"

$(IntermediateDirectory)/solid_color.c$(ObjectSuffix): solid_color.c $(IntermediateDirectory)/solid_color.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/solid_color.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/solid_color.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/solid_color.c$(DependSuffix): solid_color.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/solid_color.c$(ObjectSuffix) -MF$(IntermediateDirectory)/solid_color.c$(DependSuffix) -MM "solid_color.c"

$(IntermediateDirectory)/solid_color.c$(PreprocessSuffix): solid_color.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/solid_color.c$(PreprocessSuffix) "solid_color.c"

$(IntermediateDirectory)/stream.c$(ObjectSuffix): stream.c $(IntermediateDirectory)/stream.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/stream.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/stream.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/stream.c$(DependSuffix): stream.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/stream.c$(ObjectSuffix) -MF$(IntermediateDirectory)/stream.c$(DependSuffix) -MM "stream.c"

$(IntermediateDirectory)/stream.c$(PreprocessSuffix): stream.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/stream.c$(PreprocessSuffix) "stream.c"

$(IntermediateDirectory)/stroke.c$(ObjectSuffix): stroke.c $(IntermediateDirectory)/stroke.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/stroke.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/stroke.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/stroke.c$(DependSuffix): stroke.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/stroke.c$(ObjectSuffix) -MF$(IntermediateDirectory)/stroke.c$(DependSuffix) -MM "stroke.c"

$(IntermediateDirectory)/stroke.c$(PreprocessSuffix): stroke.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/stroke.c$(PreprocessSuffix) "stroke.c"

$(IntermediateDirectory)/test.c$(ObjectSuffix): test.c $(IntermediateDirectory)/test.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/test.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test.c$(DependSuffix): test.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test.c$(ObjectSuffix) -MF$(IntermediateDirectory)/test.c$(DependSuffix) -MM "test.c"

$(IntermediateDirectory)/test.c$(PreprocessSuffix): test.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test.c$(PreprocessSuffix) "test.c"

$(IntermediateDirectory)/threshold.c$(ObjectSuffix): threshold.c $(IntermediateDirectory)/threshold.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/threshold.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/threshold.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/threshold.c$(DependSuffix): threshold.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/threshold.c$(ObjectSuffix) -MF$(IntermediateDirectory)/threshold.c$(DependSuffix) -MM "threshold.c"

$(IntermediateDirectory)/threshold.c$(PreprocessSuffix): threshold.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/threshold.c$(PreprocessSuffix) "threshold.c"

$(IntermediateDirectory)/thumbnail.c$(ObjectSuffix): thumbnail.c $(IntermediateDirectory)/thumbnail.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/thumbnail.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/thumbnail.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/thumbnail.c$(DependSuffix): thumbnail.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/thumbnail.c$(ObjectSuffix) -MF$(IntermediateDirectory)/thumbnail.c$(DependSuffix) -MM "thumbnail.c"

$(IntermediateDirectory)/thumbnail.c$(PreprocessSuffix): thumbnail.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/thumbnail.c$(PreprocessSuffix) "thumbnail.c"

$(IntermediateDirectory)/type_tool.c$(ObjectSuffix): type_tool.c $(IntermediateDirectory)/type_tool.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/dev/Regards/Otherlib/libPsd/type_tool.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/type_tool.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/type_tool.c$(DependSuffix): type_tool.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/type_tool.c$(ObjectSuffix) -MF$(IntermediateDirectory)/type_tool.c$(DependSuffix) -MM "type_tool.c"

$(IntermediateDirectory)/type_tool.c$(PreprocessSuffix): type_tool.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/type_tool.c$(PreprocessSuffix) "type_tool.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


