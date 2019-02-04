##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libPsd
ConfigurationName      :=Release
WorkspacePath          :=/home/figuinha/developpement/Regards
ProjectPath            :=/home/figuinha/developpement/Regards/Otherlib/libPsd
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
ObjectsFileList        :="libPsd.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2 $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include  $(IncludeSwitch). $(IncludeSwitch)../libPsd/include $(IncludeSwitch)../../libextern/wxWidgets-master/src/jpeg $(IncludeSwitch). 
IncludePCH             := 
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
CXXFLAGS := $(shell ../../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread  -O2 -msse4.1 $(Preprocessors)
CFLAGS   := $(shell ../../libextern/wxWidgets-master/wx-config --cflags)  -O2 -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := ccache /usr/bin/x86_64-linux-gnu-as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/src_stroke.c$(ObjectSuffix) $(IntermediateDirectory)/src_thumbnail.c$(ObjectSuffix) $(IntermediateDirectory)/src_stream.c$(ObjectSuffix) $(IntermediateDirectory)/src_solid_color.c$(ObjectSuffix) $(IntermediateDirectory)/src_rect.c$(ObjectSuffix) $(IntermediateDirectory)/src_psd_system.c$(ObjectSuffix) $(IntermediateDirectory)/src_threshold.c$(ObjectSuffix) $(IntermediateDirectory)/src_psd.c$(ObjectSuffix) $(IntermediateDirectory)/src_photo_filter.c$(ObjectSuffix) $(IntermediateDirectory)/src_color_overlay.c$(ObjectSuffix) \
	$(IntermediateDirectory)/src_color_balance.c$(ObjectSuffix) $(IntermediateDirectory)/src_psd_zip.c$(ObjectSuffix) $(IntermediateDirectory)/src_channel_mixer.c$(ObjectSuffix) $(IntermediateDirectory)/src_hue_saturation.c$(ObjectSuffix) $(IntermediateDirectory)/src_fixed.c$(ObjectSuffix) $(IntermediateDirectory)/src_channel_image.c$(ObjectSuffix) $(IntermediateDirectory)/src_bevel_emboss.c$(ObjectSuffix) $(IntermediateDirectory)/src_gradient_map.c$(ObjectSuffix) $(IntermediateDirectory)/src_levels.c$(ObjectSuffix) $(IntermediateDirectory)/src_type_tool.c$(ObjectSuffix) \
	$(IntermediateDirectory)/src_bitmap.c$(ObjectSuffix) $(IntermediateDirectory)/src_adjustment.c$(ObjectSuffix) $(IntermediateDirectory)/src_posterize.c$(ObjectSuffix) $(IntermediateDirectory)/src_blend.c$(ObjectSuffix) $(IntermediateDirectory)/src_gradient_blend.c$(ObjectSuffix) $(IntermediateDirectory)/src_brightness_contrast.c$(ObjectSuffix) $(IntermediateDirectory)/src_color_mode.c$(ObjectSuffix) $(IntermediateDirectory)/src_effects.c$(ObjectSuffix) $(IntermediateDirectory)/src_inner_glow.c$(ObjectSuffix) $(IntermediateDirectory)/src_inner_shadow.c$(ObjectSuffix) \
	$(IntermediateDirectory)/src_gaussian_blur.c$(ObjectSuffix) $(IntermediateDirectory)/src_gradient_fill.c$(ObjectSuffix) $(IntermediateDirectory)/src_descriptor.c$(ObjectSuffix) $(IntermediateDirectory)/src_gradient_overlay.c$(ObjectSuffix) $(IntermediateDirectory)/src_pattern_overlay.c$(ObjectSuffix) $(IntermediateDirectory)/src_color.c$(ObjectSuffix) $(IntermediateDirectory)/src_image_data.c$(ObjectSuffix) $(IntermediateDirectory)/src_boundary.c$(ObjectSuffix) $(IntermediateDirectory)/src_image_resource.c$(ObjectSuffix) $(IntermediateDirectory)/src_path.c$(ObjectSuffix) \
	$(IntermediateDirectory)/src_selective_color.c$(ObjectSuffix) $(IntermediateDirectory)/src_invert.c$(ObjectSuffix) $(IntermediateDirectory)/src_curves.c$(ObjectSuffix) $(IntermediateDirectory)/src_layer_mask.c$(ObjectSuffix) $(IntermediateDirectory)/src_satin.c$(ObjectSuffix) $(IntermediateDirectory)/src_drop_shadow.c$(ObjectSuffix) $(IntermediateDirectory)/src_outer_glow.c$(ObjectSuffix) $(IntermediateDirectory)/src_pattern.c$(ObjectSuffix) $(IntermediateDirectory)/src_file_header.c$(ObjectSuffix) $(IntermediateDirectory)/src_pattern_fill.c$(ObjectSuffix) \
	



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
	@echo rebuilt > "/home/figuinha/developpement/Regards/.build-release/libPsd"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


./Release:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_stroke.c$(ObjectSuffix): src/stroke.c $(IntermediateDirectory)/src_stroke.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/stroke.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_stroke.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_stroke.c$(DependSuffix): src/stroke.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_stroke.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_stroke.c$(DependSuffix) -MM src/stroke.c

$(IntermediateDirectory)/src_stroke.c$(PreprocessSuffix): src/stroke.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_stroke.c$(PreprocessSuffix) src/stroke.c

$(IntermediateDirectory)/src_thumbnail.c$(ObjectSuffix): src/thumbnail.c $(IntermediateDirectory)/src_thumbnail.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/thumbnail.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_thumbnail.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_thumbnail.c$(DependSuffix): src/thumbnail.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_thumbnail.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_thumbnail.c$(DependSuffix) -MM src/thumbnail.c

$(IntermediateDirectory)/src_thumbnail.c$(PreprocessSuffix): src/thumbnail.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_thumbnail.c$(PreprocessSuffix) src/thumbnail.c

$(IntermediateDirectory)/src_stream.c$(ObjectSuffix): src/stream.c $(IntermediateDirectory)/src_stream.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/stream.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_stream.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_stream.c$(DependSuffix): src/stream.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_stream.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_stream.c$(DependSuffix) -MM src/stream.c

$(IntermediateDirectory)/src_stream.c$(PreprocessSuffix): src/stream.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_stream.c$(PreprocessSuffix) src/stream.c

$(IntermediateDirectory)/src_solid_color.c$(ObjectSuffix): src/solid_color.c $(IntermediateDirectory)/src_solid_color.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/solid_color.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_solid_color.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_solid_color.c$(DependSuffix): src/solid_color.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_solid_color.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_solid_color.c$(DependSuffix) -MM src/solid_color.c

$(IntermediateDirectory)/src_solid_color.c$(PreprocessSuffix): src/solid_color.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_solid_color.c$(PreprocessSuffix) src/solid_color.c

$(IntermediateDirectory)/src_rect.c$(ObjectSuffix): src/rect.c $(IntermediateDirectory)/src_rect.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/rect.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_rect.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_rect.c$(DependSuffix): src/rect.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_rect.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_rect.c$(DependSuffix) -MM src/rect.c

$(IntermediateDirectory)/src_rect.c$(PreprocessSuffix): src/rect.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_rect.c$(PreprocessSuffix) src/rect.c

$(IntermediateDirectory)/src_psd_system.c$(ObjectSuffix): src/psd_system.c $(IntermediateDirectory)/src_psd_system.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/psd_system.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_psd_system.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_psd_system.c$(DependSuffix): src/psd_system.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_psd_system.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_psd_system.c$(DependSuffix) -MM src/psd_system.c

$(IntermediateDirectory)/src_psd_system.c$(PreprocessSuffix): src/psd_system.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_psd_system.c$(PreprocessSuffix) src/psd_system.c

$(IntermediateDirectory)/src_threshold.c$(ObjectSuffix): src/threshold.c $(IntermediateDirectory)/src_threshold.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/threshold.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_threshold.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_threshold.c$(DependSuffix): src/threshold.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_threshold.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_threshold.c$(DependSuffix) -MM src/threshold.c

$(IntermediateDirectory)/src_threshold.c$(PreprocessSuffix): src/threshold.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_threshold.c$(PreprocessSuffix) src/threshold.c

$(IntermediateDirectory)/src_psd.c$(ObjectSuffix): src/psd.c $(IntermediateDirectory)/src_psd.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/psd.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_psd.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_psd.c$(DependSuffix): src/psd.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_psd.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_psd.c$(DependSuffix) -MM src/psd.c

$(IntermediateDirectory)/src_psd.c$(PreprocessSuffix): src/psd.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_psd.c$(PreprocessSuffix) src/psd.c

$(IntermediateDirectory)/src_photo_filter.c$(ObjectSuffix): src/photo_filter.c $(IntermediateDirectory)/src_photo_filter.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/photo_filter.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_photo_filter.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_photo_filter.c$(DependSuffix): src/photo_filter.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_photo_filter.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_photo_filter.c$(DependSuffix) -MM src/photo_filter.c

$(IntermediateDirectory)/src_photo_filter.c$(PreprocessSuffix): src/photo_filter.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_photo_filter.c$(PreprocessSuffix) src/photo_filter.c

$(IntermediateDirectory)/src_color_overlay.c$(ObjectSuffix): src/color_overlay.c $(IntermediateDirectory)/src_color_overlay.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/color_overlay.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_color_overlay.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_color_overlay.c$(DependSuffix): src/color_overlay.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_color_overlay.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_color_overlay.c$(DependSuffix) -MM src/color_overlay.c

$(IntermediateDirectory)/src_color_overlay.c$(PreprocessSuffix): src/color_overlay.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_color_overlay.c$(PreprocessSuffix) src/color_overlay.c

$(IntermediateDirectory)/src_color_balance.c$(ObjectSuffix): src/color_balance.c $(IntermediateDirectory)/src_color_balance.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/color_balance.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_color_balance.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_color_balance.c$(DependSuffix): src/color_balance.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_color_balance.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_color_balance.c$(DependSuffix) -MM src/color_balance.c

$(IntermediateDirectory)/src_color_balance.c$(PreprocessSuffix): src/color_balance.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_color_balance.c$(PreprocessSuffix) src/color_balance.c

$(IntermediateDirectory)/src_psd_zip.c$(ObjectSuffix): src/psd_zip.c $(IntermediateDirectory)/src_psd_zip.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/psd_zip.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_psd_zip.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_psd_zip.c$(DependSuffix): src/psd_zip.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_psd_zip.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_psd_zip.c$(DependSuffix) -MM src/psd_zip.c

$(IntermediateDirectory)/src_psd_zip.c$(PreprocessSuffix): src/psd_zip.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_psd_zip.c$(PreprocessSuffix) src/psd_zip.c

$(IntermediateDirectory)/src_channel_mixer.c$(ObjectSuffix): src/channel_mixer.c $(IntermediateDirectory)/src_channel_mixer.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/channel_mixer.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_channel_mixer.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_channel_mixer.c$(DependSuffix): src/channel_mixer.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_channel_mixer.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_channel_mixer.c$(DependSuffix) -MM src/channel_mixer.c

$(IntermediateDirectory)/src_channel_mixer.c$(PreprocessSuffix): src/channel_mixer.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_channel_mixer.c$(PreprocessSuffix) src/channel_mixer.c

$(IntermediateDirectory)/src_hue_saturation.c$(ObjectSuffix): src/hue_saturation.c $(IntermediateDirectory)/src_hue_saturation.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/hue_saturation.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_hue_saturation.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_hue_saturation.c$(DependSuffix): src/hue_saturation.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_hue_saturation.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_hue_saturation.c$(DependSuffix) -MM src/hue_saturation.c

$(IntermediateDirectory)/src_hue_saturation.c$(PreprocessSuffix): src/hue_saturation.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_hue_saturation.c$(PreprocessSuffix) src/hue_saturation.c

$(IntermediateDirectory)/src_fixed.c$(ObjectSuffix): src/fixed.c $(IntermediateDirectory)/src_fixed.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/fixed.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_fixed.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_fixed.c$(DependSuffix): src/fixed.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_fixed.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_fixed.c$(DependSuffix) -MM src/fixed.c

$(IntermediateDirectory)/src_fixed.c$(PreprocessSuffix): src/fixed.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_fixed.c$(PreprocessSuffix) src/fixed.c

$(IntermediateDirectory)/src_channel_image.c$(ObjectSuffix): src/channel_image.c $(IntermediateDirectory)/src_channel_image.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/channel_image.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_channel_image.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_channel_image.c$(DependSuffix): src/channel_image.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_channel_image.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_channel_image.c$(DependSuffix) -MM src/channel_image.c

$(IntermediateDirectory)/src_channel_image.c$(PreprocessSuffix): src/channel_image.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_channel_image.c$(PreprocessSuffix) src/channel_image.c

$(IntermediateDirectory)/src_bevel_emboss.c$(ObjectSuffix): src/bevel_emboss.c $(IntermediateDirectory)/src_bevel_emboss.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/bevel_emboss.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_bevel_emboss.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_bevel_emboss.c$(DependSuffix): src/bevel_emboss.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_bevel_emboss.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_bevel_emboss.c$(DependSuffix) -MM src/bevel_emboss.c

$(IntermediateDirectory)/src_bevel_emboss.c$(PreprocessSuffix): src/bevel_emboss.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_bevel_emboss.c$(PreprocessSuffix) src/bevel_emboss.c

$(IntermediateDirectory)/src_gradient_map.c$(ObjectSuffix): src/gradient_map.c $(IntermediateDirectory)/src_gradient_map.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/gradient_map.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_gradient_map.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_gradient_map.c$(DependSuffix): src/gradient_map.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_gradient_map.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_gradient_map.c$(DependSuffix) -MM src/gradient_map.c

$(IntermediateDirectory)/src_gradient_map.c$(PreprocessSuffix): src/gradient_map.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_gradient_map.c$(PreprocessSuffix) src/gradient_map.c

$(IntermediateDirectory)/src_levels.c$(ObjectSuffix): src/levels.c $(IntermediateDirectory)/src_levels.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/levels.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_levels.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_levels.c$(DependSuffix): src/levels.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_levels.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_levels.c$(DependSuffix) -MM src/levels.c

$(IntermediateDirectory)/src_levels.c$(PreprocessSuffix): src/levels.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_levels.c$(PreprocessSuffix) src/levels.c

$(IntermediateDirectory)/src_type_tool.c$(ObjectSuffix): src/type_tool.c $(IntermediateDirectory)/src_type_tool.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/type_tool.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_type_tool.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_type_tool.c$(DependSuffix): src/type_tool.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_type_tool.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_type_tool.c$(DependSuffix) -MM src/type_tool.c

$(IntermediateDirectory)/src_type_tool.c$(PreprocessSuffix): src/type_tool.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_type_tool.c$(PreprocessSuffix) src/type_tool.c

$(IntermediateDirectory)/src_bitmap.c$(ObjectSuffix): src/bitmap.c $(IntermediateDirectory)/src_bitmap.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/bitmap.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_bitmap.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_bitmap.c$(DependSuffix): src/bitmap.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_bitmap.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_bitmap.c$(DependSuffix) -MM src/bitmap.c

$(IntermediateDirectory)/src_bitmap.c$(PreprocessSuffix): src/bitmap.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_bitmap.c$(PreprocessSuffix) src/bitmap.c

$(IntermediateDirectory)/src_adjustment.c$(ObjectSuffix): src/adjustment.c $(IntermediateDirectory)/src_adjustment.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/adjustment.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_adjustment.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_adjustment.c$(DependSuffix): src/adjustment.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_adjustment.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_adjustment.c$(DependSuffix) -MM src/adjustment.c

$(IntermediateDirectory)/src_adjustment.c$(PreprocessSuffix): src/adjustment.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_adjustment.c$(PreprocessSuffix) src/adjustment.c

$(IntermediateDirectory)/src_posterize.c$(ObjectSuffix): src/posterize.c $(IntermediateDirectory)/src_posterize.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/posterize.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_posterize.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_posterize.c$(DependSuffix): src/posterize.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_posterize.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_posterize.c$(DependSuffix) -MM src/posterize.c

$(IntermediateDirectory)/src_posterize.c$(PreprocessSuffix): src/posterize.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_posterize.c$(PreprocessSuffix) src/posterize.c

$(IntermediateDirectory)/src_blend.c$(ObjectSuffix): src/blend.c $(IntermediateDirectory)/src_blend.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/blend.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_blend.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_blend.c$(DependSuffix): src/blend.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_blend.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_blend.c$(DependSuffix) -MM src/blend.c

$(IntermediateDirectory)/src_blend.c$(PreprocessSuffix): src/blend.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_blend.c$(PreprocessSuffix) src/blend.c

$(IntermediateDirectory)/src_gradient_blend.c$(ObjectSuffix): src/gradient_blend.c $(IntermediateDirectory)/src_gradient_blend.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/gradient_blend.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_gradient_blend.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_gradient_blend.c$(DependSuffix): src/gradient_blend.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_gradient_blend.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_gradient_blend.c$(DependSuffix) -MM src/gradient_blend.c

$(IntermediateDirectory)/src_gradient_blend.c$(PreprocessSuffix): src/gradient_blend.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_gradient_blend.c$(PreprocessSuffix) src/gradient_blend.c

$(IntermediateDirectory)/src_brightness_contrast.c$(ObjectSuffix): src/brightness_contrast.c $(IntermediateDirectory)/src_brightness_contrast.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/brightness_contrast.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_brightness_contrast.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_brightness_contrast.c$(DependSuffix): src/brightness_contrast.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_brightness_contrast.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_brightness_contrast.c$(DependSuffix) -MM src/brightness_contrast.c

$(IntermediateDirectory)/src_brightness_contrast.c$(PreprocessSuffix): src/brightness_contrast.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_brightness_contrast.c$(PreprocessSuffix) src/brightness_contrast.c

$(IntermediateDirectory)/src_color_mode.c$(ObjectSuffix): src/color_mode.c $(IntermediateDirectory)/src_color_mode.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/color_mode.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_color_mode.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_color_mode.c$(DependSuffix): src/color_mode.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_color_mode.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_color_mode.c$(DependSuffix) -MM src/color_mode.c

$(IntermediateDirectory)/src_color_mode.c$(PreprocessSuffix): src/color_mode.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_color_mode.c$(PreprocessSuffix) src/color_mode.c

$(IntermediateDirectory)/src_effects.c$(ObjectSuffix): src/effects.c $(IntermediateDirectory)/src_effects.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/effects.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_effects.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_effects.c$(DependSuffix): src/effects.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_effects.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_effects.c$(DependSuffix) -MM src/effects.c

$(IntermediateDirectory)/src_effects.c$(PreprocessSuffix): src/effects.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_effects.c$(PreprocessSuffix) src/effects.c

$(IntermediateDirectory)/src_inner_glow.c$(ObjectSuffix): src/inner_glow.c $(IntermediateDirectory)/src_inner_glow.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/inner_glow.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_inner_glow.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_inner_glow.c$(DependSuffix): src/inner_glow.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_inner_glow.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_inner_glow.c$(DependSuffix) -MM src/inner_glow.c

$(IntermediateDirectory)/src_inner_glow.c$(PreprocessSuffix): src/inner_glow.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_inner_glow.c$(PreprocessSuffix) src/inner_glow.c

$(IntermediateDirectory)/src_inner_shadow.c$(ObjectSuffix): src/inner_shadow.c $(IntermediateDirectory)/src_inner_shadow.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/inner_shadow.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_inner_shadow.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_inner_shadow.c$(DependSuffix): src/inner_shadow.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_inner_shadow.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_inner_shadow.c$(DependSuffix) -MM src/inner_shadow.c

$(IntermediateDirectory)/src_inner_shadow.c$(PreprocessSuffix): src/inner_shadow.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_inner_shadow.c$(PreprocessSuffix) src/inner_shadow.c

$(IntermediateDirectory)/src_gaussian_blur.c$(ObjectSuffix): src/gaussian_blur.c $(IntermediateDirectory)/src_gaussian_blur.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/gaussian_blur.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_gaussian_blur.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_gaussian_blur.c$(DependSuffix): src/gaussian_blur.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_gaussian_blur.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_gaussian_blur.c$(DependSuffix) -MM src/gaussian_blur.c

$(IntermediateDirectory)/src_gaussian_blur.c$(PreprocessSuffix): src/gaussian_blur.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_gaussian_blur.c$(PreprocessSuffix) src/gaussian_blur.c

$(IntermediateDirectory)/src_gradient_fill.c$(ObjectSuffix): src/gradient_fill.c $(IntermediateDirectory)/src_gradient_fill.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/gradient_fill.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_gradient_fill.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_gradient_fill.c$(DependSuffix): src/gradient_fill.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_gradient_fill.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_gradient_fill.c$(DependSuffix) -MM src/gradient_fill.c

$(IntermediateDirectory)/src_gradient_fill.c$(PreprocessSuffix): src/gradient_fill.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_gradient_fill.c$(PreprocessSuffix) src/gradient_fill.c

$(IntermediateDirectory)/src_descriptor.c$(ObjectSuffix): src/descriptor.c $(IntermediateDirectory)/src_descriptor.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/descriptor.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_descriptor.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_descriptor.c$(DependSuffix): src/descriptor.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_descriptor.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_descriptor.c$(DependSuffix) -MM src/descriptor.c

$(IntermediateDirectory)/src_descriptor.c$(PreprocessSuffix): src/descriptor.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_descriptor.c$(PreprocessSuffix) src/descriptor.c

$(IntermediateDirectory)/src_gradient_overlay.c$(ObjectSuffix): src/gradient_overlay.c $(IntermediateDirectory)/src_gradient_overlay.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/gradient_overlay.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_gradient_overlay.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_gradient_overlay.c$(DependSuffix): src/gradient_overlay.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_gradient_overlay.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_gradient_overlay.c$(DependSuffix) -MM src/gradient_overlay.c

$(IntermediateDirectory)/src_gradient_overlay.c$(PreprocessSuffix): src/gradient_overlay.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_gradient_overlay.c$(PreprocessSuffix) src/gradient_overlay.c

$(IntermediateDirectory)/src_pattern_overlay.c$(ObjectSuffix): src/pattern_overlay.c $(IntermediateDirectory)/src_pattern_overlay.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/pattern_overlay.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pattern_overlay.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pattern_overlay.c$(DependSuffix): src/pattern_overlay.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pattern_overlay.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pattern_overlay.c$(DependSuffix) -MM src/pattern_overlay.c

$(IntermediateDirectory)/src_pattern_overlay.c$(PreprocessSuffix): src/pattern_overlay.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pattern_overlay.c$(PreprocessSuffix) src/pattern_overlay.c

$(IntermediateDirectory)/src_color.c$(ObjectSuffix): src/color.c $(IntermediateDirectory)/src_color.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/color.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_color.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_color.c$(DependSuffix): src/color.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_color.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_color.c$(DependSuffix) -MM src/color.c

$(IntermediateDirectory)/src_color.c$(PreprocessSuffix): src/color.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_color.c$(PreprocessSuffix) src/color.c

$(IntermediateDirectory)/src_image_data.c$(ObjectSuffix): src/image_data.c $(IntermediateDirectory)/src_image_data.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/image_data.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_image_data.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_image_data.c$(DependSuffix): src/image_data.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_image_data.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_image_data.c$(DependSuffix) -MM src/image_data.c

$(IntermediateDirectory)/src_image_data.c$(PreprocessSuffix): src/image_data.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_image_data.c$(PreprocessSuffix) src/image_data.c

$(IntermediateDirectory)/src_boundary.c$(ObjectSuffix): src/boundary.c $(IntermediateDirectory)/src_boundary.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/boundary.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_boundary.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_boundary.c$(DependSuffix): src/boundary.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_boundary.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_boundary.c$(DependSuffix) -MM src/boundary.c

$(IntermediateDirectory)/src_boundary.c$(PreprocessSuffix): src/boundary.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_boundary.c$(PreprocessSuffix) src/boundary.c

$(IntermediateDirectory)/src_image_resource.c$(ObjectSuffix): src/image_resource.c $(IntermediateDirectory)/src_image_resource.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/image_resource.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_image_resource.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_image_resource.c$(DependSuffix): src/image_resource.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_image_resource.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_image_resource.c$(DependSuffix) -MM src/image_resource.c

$(IntermediateDirectory)/src_image_resource.c$(PreprocessSuffix): src/image_resource.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_image_resource.c$(PreprocessSuffix) src/image_resource.c

$(IntermediateDirectory)/src_path.c$(ObjectSuffix): src/path.c $(IntermediateDirectory)/src_path.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/path.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_path.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_path.c$(DependSuffix): src/path.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_path.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_path.c$(DependSuffix) -MM src/path.c

$(IntermediateDirectory)/src_path.c$(PreprocessSuffix): src/path.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_path.c$(PreprocessSuffix) src/path.c

$(IntermediateDirectory)/src_selective_color.c$(ObjectSuffix): src/selective_color.c $(IntermediateDirectory)/src_selective_color.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/selective_color.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_selective_color.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_selective_color.c$(DependSuffix): src/selective_color.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_selective_color.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_selective_color.c$(DependSuffix) -MM src/selective_color.c

$(IntermediateDirectory)/src_selective_color.c$(PreprocessSuffix): src/selective_color.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_selective_color.c$(PreprocessSuffix) src/selective_color.c

$(IntermediateDirectory)/src_invert.c$(ObjectSuffix): src/invert.c $(IntermediateDirectory)/src_invert.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/invert.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_invert.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_invert.c$(DependSuffix): src/invert.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_invert.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_invert.c$(DependSuffix) -MM src/invert.c

$(IntermediateDirectory)/src_invert.c$(PreprocessSuffix): src/invert.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_invert.c$(PreprocessSuffix) src/invert.c

$(IntermediateDirectory)/src_curves.c$(ObjectSuffix): src/curves.c $(IntermediateDirectory)/src_curves.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/curves.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_curves.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_curves.c$(DependSuffix): src/curves.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_curves.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_curves.c$(DependSuffix) -MM src/curves.c

$(IntermediateDirectory)/src_curves.c$(PreprocessSuffix): src/curves.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_curves.c$(PreprocessSuffix) src/curves.c

$(IntermediateDirectory)/src_layer_mask.c$(ObjectSuffix): src/layer_mask.c $(IntermediateDirectory)/src_layer_mask.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/layer_mask.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_layer_mask.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_layer_mask.c$(DependSuffix): src/layer_mask.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_layer_mask.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_layer_mask.c$(DependSuffix) -MM src/layer_mask.c

$(IntermediateDirectory)/src_layer_mask.c$(PreprocessSuffix): src/layer_mask.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_layer_mask.c$(PreprocessSuffix) src/layer_mask.c

$(IntermediateDirectory)/src_satin.c$(ObjectSuffix): src/satin.c $(IntermediateDirectory)/src_satin.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/satin.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_satin.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_satin.c$(DependSuffix): src/satin.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_satin.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_satin.c$(DependSuffix) -MM src/satin.c

$(IntermediateDirectory)/src_satin.c$(PreprocessSuffix): src/satin.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_satin.c$(PreprocessSuffix) src/satin.c

$(IntermediateDirectory)/src_drop_shadow.c$(ObjectSuffix): src/drop_shadow.c $(IntermediateDirectory)/src_drop_shadow.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/drop_shadow.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_drop_shadow.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_drop_shadow.c$(DependSuffix): src/drop_shadow.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_drop_shadow.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_drop_shadow.c$(DependSuffix) -MM src/drop_shadow.c

$(IntermediateDirectory)/src_drop_shadow.c$(PreprocessSuffix): src/drop_shadow.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_drop_shadow.c$(PreprocessSuffix) src/drop_shadow.c

$(IntermediateDirectory)/src_outer_glow.c$(ObjectSuffix): src/outer_glow.c $(IntermediateDirectory)/src_outer_glow.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/outer_glow.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_outer_glow.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_outer_glow.c$(DependSuffix): src/outer_glow.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_outer_glow.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_outer_glow.c$(DependSuffix) -MM src/outer_glow.c

$(IntermediateDirectory)/src_outer_glow.c$(PreprocessSuffix): src/outer_glow.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_outer_glow.c$(PreprocessSuffix) src/outer_glow.c

$(IntermediateDirectory)/src_pattern.c$(ObjectSuffix): src/pattern.c $(IntermediateDirectory)/src_pattern.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/pattern.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pattern.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pattern.c$(DependSuffix): src/pattern.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pattern.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pattern.c$(DependSuffix) -MM src/pattern.c

$(IntermediateDirectory)/src_pattern.c$(PreprocessSuffix): src/pattern.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pattern.c$(PreprocessSuffix) src/pattern.c

$(IntermediateDirectory)/src_file_header.c$(ObjectSuffix): src/file_header.c $(IntermediateDirectory)/src_file_header.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/file_header.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_file_header.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_file_header.c$(DependSuffix): src/file_header.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_file_header.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_file_header.c$(DependSuffix) -MM src/file_header.c

$(IntermediateDirectory)/src_file_header.c$(PreprocessSuffix): src/file_header.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_file_header.c$(PreprocessSuffix) src/file_header.c

$(IntermediateDirectory)/src_pattern_fill.c$(ObjectSuffix): src/pattern_fill.c $(IntermediateDirectory)/src_pattern_fill.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/libPsd/src/pattern_fill.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pattern_fill.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pattern_fill.c$(DependSuffix): src/pattern_fill.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pattern_fill.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pattern_fill.c$(DependSuffix) -MM src/pattern_fill.c

$(IntermediateDirectory)/src_pattern_fill.c$(PreprocessSuffix): src/pattern_fill.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pattern_fill.c$(PreprocessSuffix) src/pattern_fill.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


