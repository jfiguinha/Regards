##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libheif
ConfigurationName      :=Release
WorkspaceConfiguration :=Release
WorkspacePath          :=/home/figuinha/developpement/git/Regards
ProjectPath            :=/home/figuinha/developpement/git/Regards/Otherlib/libheif
IntermediateDirectory  :=$(ConfigurationName)
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=10/04/23
CodeLitePath           :=/home/figuinha/.codelite
LinkerName             :=g++
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputDirectory        :=$(IntermediateDirectory)
OutputFile             :=$(IntermediateDirectory)/$(ProjectName).a
Preprocessors          :=$(PreprocessorSwitch)HAVE_AOM $(PreprocessorSwitch)HAVE_X265 $(PreprocessorSwitch)HAVE_LIBDE265 $(PreprocessorSwitch)HAVE_UNISTD_H 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="libheif.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s
IncludePath            := $(IncludeSwitch)$${HOME}/developpement/git/Regards/libextern/oneapi-tbb-2021.7.0/include $(IncludeSwitch)$${HOME}/developpement/git/Regards/libextern/vcpkg-2022.08.15/installed/x64-linux/include/ $(IncludeSwitch)$${HOME}/developpement/git/Regards/libextern/vcpkg/installed/x64-linux/include/Imath $(IncludeSwitch)$${HOME}/developpement/git/Regards/libextern/vcpkg/installed/x64-linux/include/libde265 $(IncludeSwitch)$${HOME}/developpement/git/Regards/libextern/vcpkg/installed/x64-linux/include/poppler $(IncludeSwitch)$${HOME}/developpement/git/Regards/libextern/vcpkg/installed/x64-linux/include/poppler/cpp $(IncludeSwitch)$${HOME}/developpement/git/Regards/libextern/vcpkg/installed/x64-linux/include $(IncludeSwitch)$${HOME}/ffmpeg_build/include $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/glib-2.0/include $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include $(IncludeSwitch)/usr/include/harfbuzz  $(IncludeSwitch). $(IncludeSwitch)include $(IncludeSwitch)../ $(IncludeSwitch)../../libextern/libde265-master $(IncludeSwitch)../../libextern/libde265-master/build $(IncludeSwitch)../../libextern/x265_3.2/source $(IncludeSwitch)../../libextern/x265_3.2/source/build $(IncludeSwitch)../../libextern/x265_3.2/build/msys/8bit $(IncludeSwitch)../../libextern/x265_3.2/build/linux/8bit $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                :=$(LibraryPathSwitch)$${HOME}/developpement/git/Regards/libextern/oneapi-tbb-2021.7.0/lib/intel64/gcc4.8 $(LibraryPathSwitch)$${HOME}/developpement/git/Regards/libextern/vcpkg-2022.08.15/installed/x64-linux/lib $(LibraryPathSwitch)$${HOME}/developpement/git/Regards/libextern/vcpkg/installed/x64-linux/lib $(LibraryPathSwitch)$${HOME}/developpement/git/Regards/libextern/vcpkg/installed/x64-linux/lib/manual-link/opencv4_thirdparty $(LibraryPathSwitch)$${HOME}/ffmpeg_build/lib $(LibraryPathSwitch)/usr/lib/x86_64-linux-gnu  $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overridden using an environment variable
##
AR       := ar rcus
CXX      := g++ -fopenmp
CC       := gcc
CXXFLAGS :=  -std=gnu++14 -Wall -pthread -Wno-narrowing -O3 $(Preprocessors)
CFLAGS   := -Wall -pthread -O3 $(Preprocessors)
ASFLAGS  := 
AS       := as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/heif_decoder_libde265.cc$(ObjectSuffix) $(IntermediateDirectory)/encoder_fuzzer.cc$(ObjectSuffix) $(IntermediateDirectory)/heif_encoder_x265.cc$(ObjectSuffix) $(IntermediateDirectory)/heif_context.cc$(ObjectSuffix) $(IntermediateDirectory)/heif_avif.cc$(ObjectSuffix) $(IntermediateDirectory)/heif.cc$(ObjectSuffix) $(IntermediateDirectory)/box.cc$(ObjectSuffix) $(IntermediateDirectory)/heif_file.cc$(ObjectSuffix) $(IntermediateDirectory)/heif_colorconversion.cc$(ObjectSuffix) $(IntermediateDirectory)/heif_image.cc$(ObjectSuffix) \
	$(IntermediateDirectory)/heif_decoder_aom.cc$(ObjectSuffix) $(IntermediateDirectory)/heif_plugin.cc$(ObjectSuffix) $(IntermediateDirectory)/error.cc$(ObjectSuffix) $(IntermediateDirectory)/heif_encoder_aom.cc$(ObjectSuffix) $(IntermediateDirectory)/nclx.cc$(ObjectSuffix) $(IntermediateDirectory)/heif_plugin_registry.cc$(ObjectSuffix) $(IntermediateDirectory)/box_fuzzer.cc$(ObjectSuffix) $(IntermediateDirectory)/heif_hevc.cc$(ObjectSuffix) $(IntermediateDirectory)/file_fuzzer.cc$(ObjectSuffix) $(IntermediateDirectory)/color_conversion_fuzzer.cc$(ObjectSuffix) \
	$(IntermediateDirectory)/bitstream.cc$(ObjectSuffix) 



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
	@$(MakeDirCommand) "/home/figuinha/developpement/git/Regards/.build-release"
	@echo rebuilt > "/home/figuinha/developpement/git/Regards/.build-release/libheif"

MakeIntermediateDirs:
	@test -d $(ConfigurationName) || $(MakeDirCommand) $(ConfigurationName)


$(ConfigurationName):
	@test -d $(ConfigurationName) || $(MakeDirCommand) $(ConfigurationName)

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/heif_decoder_libde265.cc$(ObjectSuffix): heif_decoder_libde265.cc
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/Otherlib/libheif/heif_decoder_libde265.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/heif_decoder_libde265.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/heif_decoder_libde265.cc$(PreprocessSuffix): heif_decoder_libde265.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/heif_decoder_libde265.cc$(PreprocessSuffix) heif_decoder_libde265.cc

$(IntermediateDirectory)/encoder_fuzzer.cc$(ObjectSuffix): encoder_fuzzer.cc
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/Otherlib/libheif/encoder_fuzzer.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/encoder_fuzzer.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/encoder_fuzzer.cc$(PreprocessSuffix): encoder_fuzzer.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/encoder_fuzzer.cc$(PreprocessSuffix) encoder_fuzzer.cc

$(IntermediateDirectory)/heif_encoder_x265.cc$(ObjectSuffix): heif_encoder_x265.cc
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/Otherlib/libheif/heif_encoder_x265.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/heif_encoder_x265.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/heif_encoder_x265.cc$(PreprocessSuffix): heif_encoder_x265.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/heif_encoder_x265.cc$(PreprocessSuffix) heif_encoder_x265.cc

$(IntermediateDirectory)/heif_context.cc$(ObjectSuffix): heif_context.cc
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/Otherlib/libheif/heif_context.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/heif_context.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/heif_context.cc$(PreprocessSuffix): heif_context.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/heif_context.cc$(PreprocessSuffix) heif_context.cc

$(IntermediateDirectory)/heif_avif.cc$(ObjectSuffix): heif_avif.cc
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/Otherlib/libheif/heif_avif.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/heif_avif.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/heif_avif.cc$(PreprocessSuffix): heif_avif.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/heif_avif.cc$(PreprocessSuffix) heif_avif.cc

$(IntermediateDirectory)/heif.cc$(ObjectSuffix): heif.cc
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/Otherlib/libheif/heif.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/heif.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/heif.cc$(PreprocessSuffix): heif.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/heif.cc$(PreprocessSuffix) heif.cc

$(IntermediateDirectory)/box.cc$(ObjectSuffix): box.cc
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/Otherlib/libheif/box.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/box.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/box.cc$(PreprocessSuffix): box.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/box.cc$(PreprocessSuffix) box.cc

$(IntermediateDirectory)/heif_file.cc$(ObjectSuffix): heif_file.cc
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/Otherlib/libheif/heif_file.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/heif_file.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/heif_file.cc$(PreprocessSuffix): heif_file.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/heif_file.cc$(PreprocessSuffix) heif_file.cc

$(IntermediateDirectory)/heif_colorconversion.cc$(ObjectSuffix): heif_colorconversion.cc
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/Otherlib/libheif/heif_colorconversion.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/heif_colorconversion.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/heif_colorconversion.cc$(PreprocessSuffix): heif_colorconversion.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/heif_colorconversion.cc$(PreprocessSuffix) heif_colorconversion.cc

$(IntermediateDirectory)/heif_image.cc$(ObjectSuffix): heif_image.cc
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/Otherlib/libheif/heif_image.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/heif_image.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/heif_image.cc$(PreprocessSuffix): heif_image.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/heif_image.cc$(PreprocessSuffix) heif_image.cc

$(IntermediateDirectory)/heif_decoder_aom.cc$(ObjectSuffix): heif_decoder_aom.cc
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/Otherlib/libheif/heif_decoder_aom.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/heif_decoder_aom.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/heif_decoder_aom.cc$(PreprocessSuffix): heif_decoder_aom.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/heif_decoder_aom.cc$(PreprocessSuffix) heif_decoder_aom.cc

$(IntermediateDirectory)/heif_plugin.cc$(ObjectSuffix): heif_plugin.cc
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/Otherlib/libheif/heif_plugin.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/heif_plugin.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/heif_plugin.cc$(PreprocessSuffix): heif_plugin.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/heif_plugin.cc$(PreprocessSuffix) heif_plugin.cc

$(IntermediateDirectory)/error.cc$(ObjectSuffix): error.cc
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/Otherlib/libheif/error.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/error.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/error.cc$(PreprocessSuffix): error.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/error.cc$(PreprocessSuffix) error.cc

$(IntermediateDirectory)/heif_encoder_aom.cc$(ObjectSuffix): heif_encoder_aom.cc
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/Otherlib/libheif/heif_encoder_aom.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/heif_encoder_aom.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/heif_encoder_aom.cc$(PreprocessSuffix): heif_encoder_aom.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/heif_encoder_aom.cc$(PreprocessSuffix) heif_encoder_aom.cc

$(IntermediateDirectory)/nclx.cc$(ObjectSuffix): nclx.cc
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/Otherlib/libheif/nclx.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/nclx.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/nclx.cc$(PreprocessSuffix): nclx.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/nclx.cc$(PreprocessSuffix) nclx.cc

$(IntermediateDirectory)/heif_plugin_registry.cc$(ObjectSuffix): heif_plugin_registry.cc
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/Otherlib/libheif/heif_plugin_registry.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/heif_plugin_registry.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/heif_plugin_registry.cc$(PreprocessSuffix): heif_plugin_registry.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/heif_plugin_registry.cc$(PreprocessSuffix) heif_plugin_registry.cc

$(IntermediateDirectory)/box_fuzzer.cc$(ObjectSuffix): box_fuzzer.cc
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/Otherlib/libheif/box_fuzzer.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/box_fuzzer.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/box_fuzzer.cc$(PreprocessSuffix): box_fuzzer.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/box_fuzzer.cc$(PreprocessSuffix) box_fuzzer.cc

$(IntermediateDirectory)/heif_hevc.cc$(ObjectSuffix): heif_hevc.cc
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/Otherlib/libheif/heif_hevc.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/heif_hevc.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/heif_hevc.cc$(PreprocessSuffix): heif_hevc.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/heif_hevc.cc$(PreprocessSuffix) heif_hevc.cc

$(IntermediateDirectory)/file_fuzzer.cc$(ObjectSuffix): file_fuzzer.cc
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/Otherlib/libheif/file_fuzzer.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/file_fuzzer.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/file_fuzzer.cc$(PreprocessSuffix): file_fuzzer.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/file_fuzzer.cc$(PreprocessSuffix) file_fuzzer.cc

$(IntermediateDirectory)/color_conversion_fuzzer.cc$(ObjectSuffix): color_conversion_fuzzer.cc
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/Otherlib/libheif/color_conversion_fuzzer.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/color_conversion_fuzzer.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/color_conversion_fuzzer.cc$(PreprocessSuffix): color_conversion_fuzzer.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/color_conversion_fuzzer.cc$(PreprocessSuffix) color_conversion_fuzzer.cc

$(IntermediateDirectory)/bitstream.cc$(ObjectSuffix): bitstream.cc
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/Otherlib/libheif/bitstream.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bitstream.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bitstream.cc$(PreprocessSuffix): bitstream.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bitstream.cc$(PreprocessSuffix) bitstream.cc

##
## Clean
##
clean:
	$(RM) -r $(ConfigurationName)/


