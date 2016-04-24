##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libDialog
ConfigurationName      :=Debug
WorkspacePath          := "/home/figuinha/dev/Regards"
ProjectPath            := "/home/figuinha/dev/Regards/libDialog"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=23/04/16
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
Preprocessors          :=$(PreprocessorSwitch)__WXGTK__ 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libDialog.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libUtility $(IncludeSwitch)../libResource $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libOpenCL $(IncludeSwitch)../libPicture $(IncludeSwitch)../libParameter $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../include $(IncludeSwitch)../libSqlData $(IncludeSwitch)../libSqlEngine $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libextern/wxWidgets-3.0.2/include $(IncludeSwitch)../libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0 
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
CXXFLAGS :=  -g -O0 -fopenmp -std=c++11 -Wall  $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall  $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/CompressionOption.cpp$(ObjectSuffix) $(IntermediateDirectory)/ConfigRegards.cpp$(ObjectSuffix) $(IntermediateDirectory)/CopyFileDlg.cpp$(ObjectSuffix) $(IntermediateDirectory)/ExportFile.cpp$(ObjectSuffix) $(IntermediateDirectory)/GifOption.cpp$(ObjectSuffix) $(IntermediateDirectory)/JpegOption.cpp$(ObjectSuffix) $(IntermediateDirectory)/OpenCLDialog.cpp$(ObjectSuffix) $(IntermediateDirectory)/PngOption.cpp$(ObjectSuffix) $(IntermediateDirectory)/SaveFileFormat.cpp$(ObjectSuffix) $(IntermediateDirectory)/TiffOption.cpp$(ObjectSuffix) \
	



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
	@$(MakeDirCommand) "/home/figuinha/dev/Regards/.build-debug"
	@echo rebuilt > "/home/figuinha/dev/Regards/.build-debug/libDialog"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/CompressionOption.cpp$(ObjectSuffix): CompressionOption.cpp $(IntermediateDirectory)/CompressionOption.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libDialog/CompressionOption.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CompressionOption.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CompressionOption.cpp$(DependSuffix): CompressionOption.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CompressionOption.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CompressionOption.cpp$(DependSuffix) -MM "CompressionOption.cpp"

$(IntermediateDirectory)/CompressionOption.cpp$(PreprocessSuffix): CompressionOption.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CompressionOption.cpp$(PreprocessSuffix) "CompressionOption.cpp"

$(IntermediateDirectory)/ConfigRegards.cpp$(ObjectSuffix): ConfigRegards.cpp $(IntermediateDirectory)/ConfigRegards.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libDialog/ConfigRegards.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ConfigRegards.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ConfigRegards.cpp$(DependSuffix): ConfigRegards.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ConfigRegards.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ConfigRegards.cpp$(DependSuffix) -MM "ConfigRegards.cpp"

$(IntermediateDirectory)/ConfigRegards.cpp$(PreprocessSuffix): ConfigRegards.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ConfigRegards.cpp$(PreprocessSuffix) "ConfigRegards.cpp"

$(IntermediateDirectory)/CopyFileDlg.cpp$(ObjectSuffix): CopyFileDlg.cpp $(IntermediateDirectory)/CopyFileDlg.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libDialog/CopyFileDlg.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CopyFileDlg.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CopyFileDlg.cpp$(DependSuffix): CopyFileDlg.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CopyFileDlg.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CopyFileDlg.cpp$(DependSuffix) -MM "CopyFileDlg.cpp"

$(IntermediateDirectory)/CopyFileDlg.cpp$(PreprocessSuffix): CopyFileDlg.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CopyFileDlg.cpp$(PreprocessSuffix) "CopyFileDlg.cpp"

$(IntermediateDirectory)/ExportFile.cpp$(ObjectSuffix): ExportFile.cpp $(IntermediateDirectory)/ExportFile.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libDialog/ExportFile.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ExportFile.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ExportFile.cpp$(DependSuffix): ExportFile.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ExportFile.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ExportFile.cpp$(DependSuffix) -MM "ExportFile.cpp"

$(IntermediateDirectory)/ExportFile.cpp$(PreprocessSuffix): ExportFile.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ExportFile.cpp$(PreprocessSuffix) "ExportFile.cpp"

$(IntermediateDirectory)/GifOption.cpp$(ObjectSuffix): GifOption.cpp $(IntermediateDirectory)/GifOption.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libDialog/GifOption.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GifOption.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GifOption.cpp$(DependSuffix): GifOption.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GifOption.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GifOption.cpp$(DependSuffix) -MM "GifOption.cpp"

$(IntermediateDirectory)/GifOption.cpp$(PreprocessSuffix): GifOption.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GifOption.cpp$(PreprocessSuffix) "GifOption.cpp"

$(IntermediateDirectory)/JpegOption.cpp$(ObjectSuffix): JpegOption.cpp $(IntermediateDirectory)/JpegOption.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libDialog/JpegOption.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/JpegOption.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/JpegOption.cpp$(DependSuffix): JpegOption.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/JpegOption.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/JpegOption.cpp$(DependSuffix) -MM "JpegOption.cpp"

$(IntermediateDirectory)/JpegOption.cpp$(PreprocessSuffix): JpegOption.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/JpegOption.cpp$(PreprocessSuffix) "JpegOption.cpp"

$(IntermediateDirectory)/OpenCLDialog.cpp$(ObjectSuffix): OpenCLDialog.cpp $(IntermediateDirectory)/OpenCLDialog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libDialog/OpenCLDialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/OpenCLDialog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OpenCLDialog.cpp$(DependSuffix): OpenCLDialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/OpenCLDialog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/OpenCLDialog.cpp$(DependSuffix) -MM "OpenCLDialog.cpp"

$(IntermediateDirectory)/OpenCLDialog.cpp$(PreprocessSuffix): OpenCLDialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OpenCLDialog.cpp$(PreprocessSuffix) "OpenCLDialog.cpp"

$(IntermediateDirectory)/PngOption.cpp$(ObjectSuffix): PngOption.cpp $(IntermediateDirectory)/PngOption.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libDialog/PngOption.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PngOption.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PngOption.cpp$(DependSuffix): PngOption.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PngOption.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PngOption.cpp$(DependSuffix) -MM "PngOption.cpp"

$(IntermediateDirectory)/PngOption.cpp$(PreprocessSuffix): PngOption.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PngOption.cpp$(PreprocessSuffix) "PngOption.cpp"

$(IntermediateDirectory)/SaveFileFormat.cpp$(ObjectSuffix): SaveFileFormat.cpp $(IntermediateDirectory)/SaveFileFormat.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libDialog/SaveFileFormat.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SaveFileFormat.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SaveFileFormat.cpp$(DependSuffix): SaveFileFormat.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SaveFileFormat.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SaveFileFormat.cpp$(DependSuffix) -MM "SaveFileFormat.cpp"

$(IntermediateDirectory)/SaveFileFormat.cpp$(PreprocessSuffix): SaveFileFormat.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SaveFileFormat.cpp$(PreprocessSuffix) "SaveFileFormat.cpp"

$(IntermediateDirectory)/TiffOption.cpp$(ObjectSuffix): TiffOption.cpp $(IntermediateDirectory)/TiffOption.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libDialog/TiffOption.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TiffOption.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TiffOption.cpp$(DependSuffix): TiffOption.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TiffOption.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TiffOption.cpp$(DependSuffix) -MM "TiffOption.cpp"

$(IntermediateDirectory)/TiffOption.cpp$(PreprocessSuffix): TiffOption.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TiffOption.cpp$(PreprocessSuffix) "TiffOption.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


