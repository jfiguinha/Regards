##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libDialog
ConfigurationName      :=Debug
WorkspacePath          :=/home/figuinha/Developpement/Regards
ProjectPath            :=/home/figuinha/Developpement/Regards/libDialog
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=20/01/19
CodeLitePath           :=/home/figuinha/.codelite
LinkerName             :=/usr/bin/ccache /usr/bin/g++
SharedObjectLinkerName :=/usr/bin/ccache /usr/bin/g++ -shared -fPIC
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
Preprocessors          :=$(PreprocessorSwitch)EXIV2 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libDialog.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2  $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libUtility $(IncludeSwitch)../libResource $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libOpenCL $(IncludeSwitch)../libPicture $(IncludeSwitch)../libParameter $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../include $(IncludeSwitch)../libSqlData $(IncludeSwitch)../libSqlEngine $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libExif $(IncludeSwitch)../libAppleFunction $(IncludeSwitch)../include 
IncludePCH             :=  -include ../include/Debug/header.h 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                :=$(LibraryPathSwitch)/opt/AMDAPPSDK-3.0/lib  $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/ccache /usr/bin/g++
CC       := /usr/bin/ccache /usr/bin/gcc
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread -fopenmp -std=gnu++11 -ggdb -O0 $(Preprocessors)
CFLAGS   :=  -Wall  -ggdb -O0 $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/JpegOption.cpp$(ObjectSuffix) $(IntermediateDirectory)/GifOption.cpp$(ObjectSuffix) $(IntermediateDirectory)/webpOption.cpp$(ObjectSuffix) $(IntermediateDirectory)/ExportFile.cpp$(ObjectSuffix) $(IntermediateDirectory)/IndexGenerator.cpp$(ObjectSuffix) $(IntermediateDirectory)/CompressionOption.cpp$(ObjectSuffix) $(IntermediateDirectory)/CopyFileDlg.cpp$(ObjectSuffix) $(IntermediateDirectory)/ConfigRegards.cpp$(ObjectSuffix) $(IntermediateDirectory)/PngOption.cpp$(ObjectSuffix) $(IntermediateDirectory)/OpenCLDialog.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/TiffOption.cpp$(ObjectSuffix) $(IntermediateDirectory)/ChangeLabel.cpp$(ObjectSuffix) $(IntermediateDirectory)/MoveFaceDialog.cpp$(ObjectSuffix) $(IntermediateDirectory)/bpgoption.cpp$(ObjectSuffix) $(IntermediateDirectory)/PertinenceValue.cpp$(ObjectSuffix) $(IntermediateDirectory)/SavePicture.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "/home/figuinha/Developpement/Regards/.build-debug"
	@echo rebuilt > "/home/figuinha/Developpement/Regards/.build-debug/libDialog"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:

# PreCompiled Header
../include/Debug/header.h.gch: ../include/Debug/header.h
	$(CXX) $(SourceSwitch) ../include/Debug/header.h $(PCHCompileFlags) $(CXXFLAGS) $(IncludePath)



##
## Objects
##
$(IntermediateDirectory)/JpegOption.cpp$(ObjectSuffix): JpegOption.cpp $(IntermediateDirectory)/JpegOption.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libDialog/JpegOption.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/JpegOption.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/JpegOption.cpp$(DependSuffix): JpegOption.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/JpegOption.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/JpegOption.cpp$(DependSuffix) -MM JpegOption.cpp

$(IntermediateDirectory)/JpegOption.cpp$(PreprocessSuffix): JpegOption.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/JpegOption.cpp$(PreprocessSuffix) JpegOption.cpp

$(IntermediateDirectory)/GifOption.cpp$(ObjectSuffix): GifOption.cpp $(IntermediateDirectory)/GifOption.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libDialog/GifOption.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GifOption.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GifOption.cpp$(DependSuffix): GifOption.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GifOption.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GifOption.cpp$(DependSuffix) -MM GifOption.cpp

$(IntermediateDirectory)/GifOption.cpp$(PreprocessSuffix): GifOption.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GifOption.cpp$(PreprocessSuffix) GifOption.cpp

$(IntermediateDirectory)/webpOption.cpp$(ObjectSuffix): webpOption.cpp $(IntermediateDirectory)/webpOption.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libDialog/webpOption.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/webpOption.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/webpOption.cpp$(DependSuffix): webpOption.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/webpOption.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/webpOption.cpp$(DependSuffix) -MM webpOption.cpp

$(IntermediateDirectory)/webpOption.cpp$(PreprocessSuffix): webpOption.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/webpOption.cpp$(PreprocessSuffix) webpOption.cpp

$(IntermediateDirectory)/ExportFile.cpp$(ObjectSuffix): ExportFile.cpp $(IntermediateDirectory)/ExportFile.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libDialog/ExportFile.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ExportFile.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ExportFile.cpp$(DependSuffix): ExportFile.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ExportFile.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ExportFile.cpp$(DependSuffix) -MM ExportFile.cpp

$(IntermediateDirectory)/ExportFile.cpp$(PreprocessSuffix): ExportFile.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ExportFile.cpp$(PreprocessSuffix) ExportFile.cpp

$(IntermediateDirectory)/IndexGenerator.cpp$(ObjectSuffix): IndexGenerator.cpp $(IntermediateDirectory)/IndexGenerator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libDialog/IndexGenerator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IndexGenerator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IndexGenerator.cpp$(DependSuffix): IndexGenerator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IndexGenerator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IndexGenerator.cpp$(DependSuffix) -MM IndexGenerator.cpp

$(IntermediateDirectory)/IndexGenerator.cpp$(PreprocessSuffix): IndexGenerator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IndexGenerator.cpp$(PreprocessSuffix) IndexGenerator.cpp

$(IntermediateDirectory)/CompressionOption.cpp$(ObjectSuffix): CompressionOption.cpp $(IntermediateDirectory)/CompressionOption.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libDialog/CompressionOption.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CompressionOption.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CompressionOption.cpp$(DependSuffix): CompressionOption.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CompressionOption.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CompressionOption.cpp$(DependSuffix) -MM CompressionOption.cpp

$(IntermediateDirectory)/CompressionOption.cpp$(PreprocessSuffix): CompressionOption.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CompressionOption.cpp$(PreprocessSuffix) CompressionOption.cpp

$(IntermediateDirectory)/CopyFileDlg.cpp$(ObjectSuffix): CopyFileDlg.cpp $(IntermediateDirectory)/CopyFileDlg.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libDialog/CopyFileDlg.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CopyFileDlg.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CopyFileDlg.cpp$(DependSuffix): CopyFileDlg.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CopyFileDlg.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CopyFileDlg.cpp$(DependSuffix) -MM CopyFileDlg.cpp

$(IntermediateDirectory)/CopyFileDlg.cpp$(PreprocessSuffix): CopyFileDlg.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CopyFileDlg.cpp$(PreprocessSuffix) CopyFileDlg.cpp

$(IntermediateDirectory)/ConfigRegards.cpp$(ObjectSuffix): ConfigRegards.cpp $(IntermediateDirectory)/ConfigRegards.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libDialog/ConfigRegards.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ConfigRegards.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ConfigRegards.cpp$(DependSuffix): ConfigRegards.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ConfigRegards.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ConfigRegards.cpp$(DependSuffix) -MM ConfigRegards.cpp

$(IntermediateDirectory)/ConfigRegards.cpp$(PreprocessSuffix): ConfigRegards.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ConfigRegards.cpp$(PreprocessSuffix) ConfigRegards.cpp

$(IntermediateDirectory)/PngOption.cpp$(ObjectSuffix): PngOption.cpp $(IntermediateDirectory)/PngOption.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libDialog/PngOption.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PngOption.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PngOption.cpp$(DependSuffix): PngOption.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PngOption.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PngOption.cpp$(DependSuffix) -MM PngOption.cpp

$(IntermediateDirectory)/PngOption.cpp$(PreprocessSuffix): PngOption.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PngOption.cpp$(PreprocessSuffix) PngOption.cpp

$(IntermediateDirectory)/OpenCLDialog.cpp$(ObjectSuffix): OpenCLDialog.cpp $(IntermediateDirectory)/OpenCLDialog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libDialog/OpenCLDialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/OpenCLDialog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OpenCLDialog.cpp$(DependSuffix): OpenCLDialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/OpenCLDialog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/OpenCLDialog.cpp$(DependSuffix) -MM OpenCLDialog.cpp

$(IntermediateDirectory)/OpenCLDialog.cpp$(PreprocessSuffix): OpenCLDialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OpenCLDialog.cpp$(PreprocessSuffix) OpenCLDialog.cpp

$(IntermediateDirectory)/TiffOption.cpp$(ObjectSuffix): TiffOption.cpp $(IntermediateDirectory)/TiffOption.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libDialog/TiffOption.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TiffOption.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TiffOption.cpp$(DependSuffix): TiffOption.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TiffOption.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TiffOption.cpp$(DependSuffix) -MM TiffOption.cpp

$(IntermediateDirectory)/TiffOption.cpp$(PreprocessSuffix): TiffOption.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TiffOption.cpp$(PreprocessSuffix) TiffOption.cpp

$(IntermediateDirectory)/ChangeLabel.cpp$(ObjectSuffix): ChangeLabel.cpp $(IntermediateDirectory)/ChangeLabel.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libDialog/ChangeLabel.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ChangeLabel.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ChangeLabel.cpp$(DependSuffix): ChangeLabel.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ChangeLabel.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ChangeLabel.cpp$(DependSuffix) -MM ChangeLabel.cpp

$(IntermediateDirectory)/ChangeLabel.cpp$(PreprocessSuffix): ChangeLabel.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ChangeLabel.cpp$(PreprocessSuffix) ChangeLabel.cpp

$(IntermediateDirectory)/MoveFaceDialog.cpp$(ObjectSuffix): MoveFaceDialog.cpp $(IntermediateDirectory)/MoveFaceDialog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libDialog/MoveFaceDialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MoveFaceDialog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MoveFaceDialog.cpp$(DependSuffix): MoveFaceDialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MoveFaceDialog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MoveFaceDialog.cpp$(DependSuffix) -MM MoveFaceDialog.cpp

$(IntermediateDirectory)/MoveFaceDialog.cpp$(PreprocessSuffix): MoveFaceDialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MoveFaceDialog.cpp$(PreprocessSuffix) MoveFaceDialog.cpp

$(IntermediateDirectory)/bpgoption.cpp$(ObjectSuffix): bpgoption.cpp $(IntermediateDirectory)/bpgoption.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libDialog/bpgoption.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bpgoption.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bpgoption.cpp$(DependSuffix): bpgoption.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bpgoption.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bpgoption.cpp$(DependSuffix) -MM bpgoption.cpp

$(IntermediateDirectory)/bpgoption.cpp$(PreprocessSuffix): bpgoption.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bpgoption.cpp$(PreprocessSuffix) bpgoption.cpp

$(IntermediateDirectory)/PertinenceValue.cpp$(ObjectSuffix): PertinenceValue.cpp $(IntermediateDirectory)/PertinenceValue.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libDialog/PertinenceValue.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PertinenceValue.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PertinenceValue.cpp$(DependSuffix): PertinenceValue.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PertinenceValue.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PertinenceValue.cpp$(DependSuffix) -MM PertinenceValue.cpp

$(IntermediateDirectory)/PertinenceValue.cpp$(PreprocessSuffix): PertinenceValue.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PertinenceValue.cpp$(PreprocessSuffix) PertinenceValue.cpp

$(IntermediateDirectory)/SavePicture.cpp$(ObjectSuffix): SavePicture.cpp $(IntermediateDirectory)/SavePicture.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/Developpement/Regards/libDialog/SavePicture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SavePicture.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SavePicture.cpp$(DependSuffix): SavePicture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SavePicture.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SavePicture.cpp$(DependSuffix) -MM SavePicture.cpp

$(IntermediateDirectory)/SavePicture.cpp$(PreprocessSuffix): SavePicture.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SavePicture.cpp$(PreprocessSuffix) SavePicture.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/
	$(RM) ../include/Debug/header.h.gch


