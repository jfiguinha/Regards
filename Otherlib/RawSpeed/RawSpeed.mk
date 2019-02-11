##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=RawSpeed
ConfigurationName      :=Release
WorkspacePath          :=C:/Regards
ProjectPath            :=C:/Regards/Otherlib/RawSpeed
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
OutputFile             :=$(IntermediateDirectory)/lib$(ProjectName).a
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="RawSpeed.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/msys64/mingw64/bin/windres.exe
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)"C:\Program Files (x86)\AMD APP SDK\3.0_mingw\include" $(IncludeSwitch)/mingw64/include/libxml2 $(IncludeSwitch)/mingw64/include/OpenEXR  $(IncludeSwitch)../../libextern/wxWidgets-master/src/jpeg 
IncludePCH             := 
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
CXXFLAGS := $(shell ../../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread  -fopenmp -std=gnu++11 -Wall -funsigned-char -Wno-narrowing -O2 -msse4.1 $(Preprocessors)
CFLAGS   :=  -Wall -O2 -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := C:/msys64/mingw64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
PATH:=C:\msys64\usr\bin;$PATH
WXWIN:=C:\Regards\libextern\wxwidgets-3.1.2
WXCFG:=lib/mswud
Objects0=$(IntermediateDirectory)/RawDecoder.cpp$(ObjectSuffix) $(IntermediateDirectory)/PentaxDecompressor.cpp$(ObjectSuffix) $(IntermediateDirectory)/RawDecoderException.cpp$(ObjectSuffix) $(IntermediateDirectory)/LJpegPlain.cpp$(ObjectSuffix) $(IntermediateDirectory)/LJpegDecompressor.cpp$(ObjectSuffix) $(IntermediateDirectory)/IOException.cpp$(ObjectSuffix) $(IntermediateDirectory)/FileReader.cpp$(ObjectSuffix) $(IntermediateDirectory)/FileMap.cpp$(ObjectSuffix) $(IntermediateDirectory)/DngOpcodes.cpp$(ObjectSuffix) $(IntermediateDirectory)/Rw2Decoder.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/BitPumpPlain.cpp$(ObjectSuffix) $(IntermediateDirectory)/CameraMetaData.cpp$(ObjectSuffix) $(IntermediateDirectory)/BlackArea.cpp$(ObjectSuffix) $(IntermediateDirectory)/RawImageDataU16.cpp$(ObjectSuffix) $(IntermediateDirectory)/BitPumpMSB32.cpp$(ObjectSuffix) $(IntermediateDirectory)/RawParser.cpp$(ObjectSuffix) $(IntermediateDirectory)/NikonDecompressor.cpp$(ObjectSuffix) $(IntermediateDirectory)/BitPumpJPEG.cpp$(ObjectSuffix) $(IntermediateDirectory)/CameraMetadataException.cpp$(ObjectSuffix) $(IntermediateDirectory)/RawImageDataFloat.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/BitPumpMSB.cpp$(ObjectSuffix) $(IntermediateDirectory)/TiffParserHeaderless.cpp$(ObjectSuffix) $(IntermediateDirectory)/NefDecoder.cpp$(ObjectSuffix) $(IntermediateDirectory)/Cr2Decoder.cpp$(ObjectSuffix) $(IntermediateDirectory)/ByteStream.cpp$(ObjectSuffix) $(IntermediateDirectory)/SrwDecoder.cpp$(ObjectSuffix) $(IntermediateDirectory)/TiffEntry.cpp$(ObjectSuffix) $(IntermediateDirectory)/Camera.cpp$(ObjectSuffix) $(IntermediateDirectory)/TiffParser.cpp$(ObjectSuffix) $(IntermediateDirectory)/CameraSensorInfo.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/ColorFilterArray.cpp$(ObjectSuffix) $(IntermediateDirectory)/RawImage.cpp$(ObjectSuffix) $(IntermediateDirectory)/DngDecoderSlices.cpp$(ObjectSuffix) $(IntermediateDirectory)/Common.cpp$(ObjectSuffix) $(IntermediateDirectory)/DngDecoder.cpp$(ObjectSuffix) $(IntermediateDirectory)/TiffIFD.cpp$(ObjectSuffix) $(IntermediateDirectory)/TiffEntryBE.cpp$(ObjectSuffix) $(IntermediateDirectory)/TiffIFDBE.cpp$(ObjectSuffix) $(IntermediateDirectory)/TiffParserException.cpp$(ObjectSuffix) $(IntermediateDirectory)/TiffParserOlympus.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/ArwDecoder.cpp$(ObjectSuffix) $(IntermediateDirectory)/ByteStreamSwap.cpp$(ObjectSuffix) $(IntermediateDirectory)/FileIOException.cpp$(ObjectSuffix) $(IntermediateDirectory)/PefDecoder.cpp$(ObjectSuffix) $(IntermediateDirectory)/OrfDecoder.cpp$(ObjectSuffix) $(IntermediateDirectory)/StdAfx.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "C:\Regards/.build-release/RawSpeed"

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Release"


./Release:
	@$(MakeDirCommand) "./Release"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/RawDecoder.cpp$(ObjectSuffix): RawDecoder.cpp $(IntermediateDirectory)/RawDecoder.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/RawDecoder.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RawDecoder.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RawDecoder.cpp$(DependSuffix): RawDecoder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RawDecoder.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RawDecoder.cpp$(DependSuffix) -MM RawDecoder.cpp

$(IntermediateDirectory)/RawDecoder.cpp$(PreprocessSuffix): RawDecoder.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RawDecoder.cpp$(PreprocessSuffix) RawDecoder.cpp

$(IntermediateDirectory)/PentaxDecompressor.cpp$(ObjectSuffix): PentaxDecompressor.cpp $(IntermediateDirectory)/PentaxDecompressor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/PentaxDecompressor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PentaxDecompressor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PentaxDecompressor.cpp$(DependSuffix): PentaxDecompressor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PentaxDecompressor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PentaxDecompressor.cpp$(DependSuffix) -MM PentaxDecompressor.cpp

$(IntermediateDirectory)/PentaxDecompressor.cpp$(PreprocessSuffix): PentaxDecompressor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PentaxDecompressor.cpp$(PreprocessSuffix) PentaxDecompressor.cpp

$(IntermediateDirectory)/RawDecoderException.cpp$(ObjectSuffix): RawDecoderException.cpp $(IntermediateDirectory)/RawDecoderException.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/RawDecoderException.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RawDecoderException.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RawDecoderException.cpp$(DependSuffix): RawDecoderException.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RawDecoderException.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RawDecoderException.cpp$(DependSuffix) -MM RawDecoderException.cpp

$(IntermediateDirectory)/RawDecoderException.cpp$(PreprocessSuffix): RawDecoderException.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RawDecoderException.cpp$(PreprocessSuffix) RawDecoderException.cpp

$(IntermediateDirectory)/LJpegPlain.cpp$(ObjectSuffix): LJpegPlain.cpp $(IntermediateDirectory)/LJpegPlain.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/LJpegPlain.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LJpegPlain.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LJpegPlain.cpp$(DependSuffix): LJpegPlain.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LJpegPlain.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LJpegPlain.cpp$(DependSuffix) -MM LJpegPlain.cpp

$(IntermediateDirectory)/LJpegPlain.cpp$(PreprocessSuffix): LJpegPlain.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LJpegPlain.cpp$(PreprocessSuffix) LJpegPlain.cpp

$(IntermediateDirectory)/LJpegDecompressor.cpp$(ObjectSuffix): LJpegDecompressor.cpp $(IntermediateDirectory)/LJpegDecompressor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/LJpegDecompressor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LJpegDecompressor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LJpegDecompressor.cpp$(DependSuffix): LJpegDecompressor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LJpegDecompressor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LJpegDecompressor.cpp$(DependSuffix) -MM LJpegDecompressor.cpp

$(IntermediateDirectory)/LJpegDecompressor.cpp$(PreprocessSuffix): LJpegDecompressor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LJpegDecompressor.cpp$(PreprocessSuffix) LJpegDecompressor.cpp

$(IntermediateDirectory)/IOException.cpp$(ObjectSuffix): IOException.cpp $(IntermediateDirectory)/IOException.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/IOException.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IOException.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IOException.cpp$(DependSuffix): IOException.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IOException.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IOException.cpp$(DependSuffix) -MM IOException.cpp

$(IntermediateDirectory)/IOException.cpp$(PreprocessSuffix): IOException.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IOException.cpp$(PreprocessSuffix) IOException.cpp

$(IntermediateDirectory)/FileReader.cpp$(ObjectSuffix): FileReader.cpp $(IntermediateDirectory)/FileReader.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/FileReader.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FileReader.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FileReader.cpp$(DependSuffix): FileReader.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FileReader.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FileReader.cpp$(DependSuffix) -MM FileReader.cpp

$(IntermediateDirectory)/FileReader.cpp$(PreprocessSuffix): FileReader.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FileReader.cpp$(PreprocessSuffix) FileReader.cpp

$(IntermediateDirectory)/FileMap.cpp$(ObjectSuffix): FileMap.cpp $(IntermediateDirectory)/FileMap.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/FileMap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FileMap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FileMap.cpp$(DependSuffix): FileMap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FileMap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FileMap.cpp$(DependSuffix) -MM FileMap.cpp

$(IntermediateDirectory)/FileMap.cpp$(PreprocessSuffix): FileMap.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FileMap.cpp$(PreprocessSuffix) FileMap.cpp

$(IntermediateDirectory)/DngOpcodes.cpp$(ObjectSuffix): DngOpcodes.cpp $(IntermediateDirectory)/DngOpcodes.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/DngOpcodes.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/DngOpcodes.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/DngOpcodes.cpp$(DependSuffix): DngOpcodes.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/DngOpcodes.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/DngOpcodes.cpp$(DependSuffix) -MM DngOpcodes.cpp

$(IntermediateDirectory)/DngOpcodes.cpp$(PreprocessSuffix): DngOpcodes.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/DngOpcodes.cpp$(PreprocessSuffix) DngOpcodes.cpp

$(IntermediateDirectory)/Rw2Decoder.cpp$(ObjectSuffix): Rw2Decoder.cpp $(IntermediateDirectory)/Rw2Decoder.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/Rw2Decoder.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Rw2Decoder.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Rw2Decoder.cpp$(DependSuffix): Rw2Decoder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Rw2Decoder.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Rw2Decoder.cpp$(DependSuffix) -MM Rw2Decoder.cpp

$(IntermediateDirectory)/Rw2Decoder.cpp$(PreprocessSuffix): Rw2Decoder.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Rw2Decoder.cpp$(PreprocessSuffix) Rw2Decoder.cpp

$(IntermediateDirectory)/BitPumpPlain.cpp$(ObjectSuffix): BitPumpPlain.cpp $(IntermediateDirectory)/BitPumpPlain.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/BitPumpPlain.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BitPumpPlain.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BitPumpPlain.cpp$(DependSuffix): BitPumpPlain.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BitPumpPlain.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BitPumpPlain.cpp$(DependSuffix) -MM BitPumpPlain.cpp

$(IntermediateDirectory)/BitPumpPlain.cpp$(PreprocessSuffix): BitPumpPlain.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BitPumpPlain.cpp$(PreprocessSuffix) BitPumpPlain.cpp

$(IntermediateDirectory)/CameraMetaData.cpp$(ObjectSuffix): CameraMetaData.cpp $(IntermediateDirectory)/CameraMetaData.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/CameraMetaData.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CameraMetaData.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CameraMetaData.cpp$(DependSuffix): CameraMetaData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CameraMetaData.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CameraMetaData.cpp$(DependSuffix) -MM CameraMetaData.cpp

$(IntermediateDirectory)/CameraMetaData.cpp$(PreprocessSuffix): CameraMetaData.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CameraMetaData.cpp$(PreprocessSuffix) CameraMetaData.cpp

$(IntermediateDirectory)/BlackArea.cpp$(ObjectSuffix): BlackArea.cpp $(IntermediateDirectory)/BlackArea.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/BlackArea.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BlackArea.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BlackArea.cpp$(DependSuffix): BlackArea.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BlackArea.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BlackArea.cpp$(DependSuffix) -MM BlackArea.cpp

$(IntermediateDirectory)/BlackArea.cpp$(PreprocessSuffix): BlackArea.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BlackArea.cpp$(PreprocessSuffix) BlackArea.cpp

$(IntermediateDirectory)/RawImageDataU16.cpp$(ObjectSuffix): RawImageDataU16.cpp $(IntermediateDirectory)/RawImageDataU16.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/RawImageDataU16.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RawImageDataU16.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RawImageDataU16.cpp$(DependSuffix): RawImageDataU16.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RawImageDataU16.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RawImageDataU16.cpp$(DependSuffix) -MM RawImageDataU16.cpp

$(IntermediateDirectory)/RawImageDataU16.cpp$(PreprocessSuffix): RawImageDataU16.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RawImageDataU16.cpp$(PreprocessSuffix) RawImageDataU16.cpp

$(IntermediateDirectory)/BitPumpMSB32.cpp$(ObjectSuffix): BitPumpMSB32.cpp $(IntermediateDirectory)/BitPumpMSB32.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/BitPumpMSB32.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BitPumpMSB32.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BitPumpMSB32.cpp$(DependSuffix): BitPumpMSB32.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BitPumpMSB32.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BitPumpMSB32.cpp$(DependSuffix) -MM BitPumpMSB32.cpp

$(IntermediateDirectory)/BitPumpMSB32.cpp$(PreprocessSuffix): BitPumpMSB32.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BitPumpMSB32.cpp$(PreprocessSuffix) BitPumpMSB32.cpp

$(IntermediateDirectory)/RawParser.cpp$(ObjectSuffix): RawParser.cpp $(IntermediateDirectory)/RawParser.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/RawParser.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RawParser.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RawParser.cpp$(DependSuffix): RawParser.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RawParser.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RawParser.cpp$(DependSuffix) -MM RawParser.cpp

$(IntermediateDirectory)/RawParser.cpp$(PreprocessSuffix): RawParser.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RawParser.cpp$(PreprocessSuffix) RawParser.cpp

$(IntermediateDirectory)/NikonDecompressor.cpp$(ObjectSuffix): NikonDecompressor.cpp $(IntermediateDirectory)/NikonDecompressor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/NikonDecompressor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/NikonDecompressor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/NikonDecompressor.cpp$(DependSuffix): NikonDecompressor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/NikonDecompressor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/NikonDecompressor.cpp$(DependSuffix) -MM NikonDecompressor.cpp

$(IntermediateDirectory)/NikonDecompressor.cpp$(PreprocessSuffix): NikonDecompressor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/NikonDecompressor.cpp$(PreprocessSuffix) NikonDecompressor.cpp

$(IntermediateDirectory)/BitPumpJPEG.cpp$(ObjectSuffix): BitPumpJPEG.cpp $(IntermediateDirectory)/BitPumpJPEG.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/BitPumpJPEG.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BitPumpJPEG.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BitPumpJPEG.cpp$(DependSuffix): BitPumpJPEG.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BitPumpJPEG.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BitPumpJPEG.cpp$(DependSuffix) -MM BitPumpJPEG.cpp

$(IntermediateDirectory)/BitPumpJPEG.cpp$(PreprocessSuffix): BitPumpJPEG.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BitPumpJPEG.cpp$(PreprocessSuffix) BitPumpJPEG.cpp

$(IntermediateDirectory)/CameraMetadataException.cpp$(ObjectSuffix): CameraMetadataException.cpp $(IntermediateDirectory)/CameraMetadataException.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/CameraMetadataException.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CameraMetadataException.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CameraMetadataException.cpp$(DependSuffix): CameraMetadataException.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CameraMetadataException.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CameraMetadataException.cpp$(DependSuffix) -MM CameraMetadataException.cpp

$(IntermediateDirectory)/CameraMetadataException.cpp$(PreprocessSuffix): CameraMetadataException.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CameraMetadataException.cpp$(PreprocessSuffix) CameraMetadataException.cpp

$(IntermediateDirectory)/RawImageDataFloat.cpp$(ObjectSuffix): RawImageDataFloat.cpp $(IntermediateDirectory)/RawImageDataFloat.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/RawImageDataFloat.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RawImageDataFloat.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RawImageDataFloat.cpp$(DependSuffix): RawImageDataFloat.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RawImageDataFloat.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RawImageDataFloat.cpp$(DependSuffix) -MM RawImageDataFloat.cpp

$(IntermediateDirectory)/RawImageDataFloat.cpp$(PreprocessSuffix): RawImageDataFloat.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RawImageDataFloat.cpp$(PreprocessSuffix) RawImageDataFloat.cpp

$(IntermediateDirectory)/BitPumpMSB.cpp$(ObjectSuffix): BitPumpMSB.cpp $(IntermediateDirectory)/BitPumpMSB.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/BitPumpMSB.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BitPumpMSB.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BitPumpMSB.cpp$(DependSuffix): BitPumpMSB.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BitPumpMSB.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BitPumpMSB.cpp$(DependSuffix) -MM BitPumpMSB.cpp

$(IntermediateDirectory)/BitPumpMSB.cpp$(PreprocessSuffix): BitPumpMSB.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BitPumpMSB.cpp$(PreprocessSuffix) BitPumpMSB.cpp

$(IntermediateDirectory)/TiffParserHeaderless.cpp$(ObjectSuffix): TiffParserHeaderless.cpp $(IntermediateDirectory)/TiffParserHeaderless.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/TiffParserHeaderless.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TiffParserHeaderless.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TiffParserHeaderless.cpp$(DependSuffix): TiffParserHeaderless.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TiffParserHeaderless.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TiffParserHeaderless.cpp$(DependSuffix) -MM TiffParserHeaderless.cpp

$(IntermediateDirectory)/TiffParserHeaderless.cpp$(PreprocessSuffix): TiffParserHeaderless.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TiffParserHeaderless.cpp$(PreprocessSuffix) TiffParserHeaderless.cpp

$(IntermediateDirectory)/NefDecoder.cpp$(ObjectSuffix): NefDecoder.cpp $(IntermediateDirectory)/NefDecoder.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/NefDecoder.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/NefDecoder.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/NefDecoder.cpp$(DependSuffix): NefDecoder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/NefDecoder.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/NefDecoder.cpp$(DependSuffix) -MM NefDecoder.cpp

$(IntermediateDirectory)/NefDecoder.cpp$(PreprocessSuffix): NefDecoder.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/NefDecoder.cpp$(PreprocessSuffix) NefDecoder.cpp

$(IntermediateDirectory)/Cr2Decoder.cpp$(ObjectSuffix): Cr2Decoder.cpp $(IntermediateDirectory)/Cr2Decoder.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/Cr2Decoder.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Cr2Decoder.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Cr2Decoder.cpp$(DependSuffix): Cr2Decoder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Cr2Decoder.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Cr2Decoder.cpp$(DependSuffix) -MM Cr2Decoder.cpp

$(IntermediateDirectory)/Cr2Decoder.cpp$(PreprocessSuffix): Cr2Decoder.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Cr2Decoder.cpp$(PreprocessSuffix) Cr2Decoder.cpp

$(IntermediateDirectory)/ByteStream.cpp$(ObjectSuffix): ByteStream.cpp $(IntermediateDirectory)/ByteStream.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/ByteStream.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ByteStream.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ByteStream.cpp$(DependSuffix): ByteStream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ByteStream.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ByteStream.cpp$(DependSuffix) -MM ByteStream.cpp

$(IntermediateDirectory)/ByteStream.cpp$(PreprocessSuffix): ByteStream.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ByteStream.cpp$(PreprocessSuffix) ByteStream.cpp

$(IntermediateDirectory)/SrwDecoder.cpp$(ObjectSuffix): SrwDecoder.cpp $(IntermediateDirectory)/SrwDecoder.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/SrwDecoder.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SrwDecoder.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SrwDecoder.cpp$(DependSuffix): SrwDecoder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SrwDecoder.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SrwDecoder.cpp$(DependSuffix) -MM SrwDecoder.cpp

$(IntermediateDirectory)/SrwDecoder.cpp$(PreprocessSuffix): SrwDecoder.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SrwDecoder.cpp$(PreprocessSuffix) SrwDecoder.cpp

$(IntermediateDirectory)/TiffEntry.cpp$(ObjectSuffix): TiffEntry.cpp $(IntermediateDirectory)/TiffEntry.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/TiffEntry.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TiffEntry.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TiffEntry.cpp$(DependSuffix): TiffEntry.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TiffEntry.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TiffEntry.cpp$(DependSuffix) -MM TiffEntry.cpp

$(IntermediateDirectory)/TiffEntry.cpp$(PreprocessSuffix): TiffEntry.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TiffEntry.cpp$(PreprocessSuffix) TiffEntry.cpp

$(IntermediateDirectory)/Camera.cpp$(ObjectSuffix): Camera.cpp $(IntermediateDirectory)/Camera.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/Camera.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Camera.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Camera.cpp$(DependSuffix): Camera.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Camera.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Camera.cpp$(DependSuffix) -MM Camera.cpp

$(IntermediateDirectory)/Camera.cpp$(PreprocessSuffix): Camera.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Camera.cpp$(PreprocessSuffix) Camera.cpp

$(IntermediateDirectory)/TiffParser.cpp$(ObjectSuffix): TiffParser.cpp $(IntermediateDirectory)/TiffParser.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/TiffParser.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TiffParser.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TiffParser.cpp$(DependSuffix): TiffParser.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TiffParser.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TiffParser.cpp$(DependSuffix) -MM TiffParser.cpp

$(IntermediateDirectory)/TiffParser.cpp$(PreprocessSuffix): TiffParser.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TiffParser.cpp$(PreprocessSuffix) TiffParser.cpp

$(IntermediateDirectory)/CameraSensorInfo.cpp$(ObjectSuffix): CameraSensorInfo.cpp $(IntermediateDirectory)/CameraSensorInfo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/CameraSensorInfo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CameraSensorInfo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CameraSensorInfo.cpp$(DependSuffix): CameraSensorInfo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CameraSensorInfo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CameraSensorInfo.cpp$(DependSuffix) -MM CameraSensorInfo.cpp

$(IntermediateDirectory)/CameraSensorInfo.cpp$(PreprocessSuffix): CameraSensorInfo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CameraSensorInfo.cpp$(PreprocessSuffix) CameraSensorInfo.cpp

$(IntermediateDirectory)/ColorFilterArray.cpp$(ObjectSuffix): ColorFilterArray.cpp $(IntermediateDirectory)/ColorFilterArray.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/ColorFilterArray.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ColorFilterArray.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ColorFilterArray.cpp$(DependSuffix): ColorFilterArray.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ColorFilterArray.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ColorFilterArray.cpp$(DependSuffix) -MM ColorFilterArray.cpp

$(IntermediateDirectory)/ColorFilterArray.cpp$(PreprocessSuffix): ColorFilterArray.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ColorFilterArray.cpp$(PreprocessSuffix) ColorFilterArray.cpp

$(IntermediateDirectory)/RawImage.cpp$(ObjectSuffix): RawImage.cpp $(IntermediateDirectory)/RawImage.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/RawImage.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RawImage.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RawImage.cpp$(DependSuffix): RawImage.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RawImage.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RawImage.cpp$(DependSuffix) -MM RawImage.cpp

$(IntermediateDirectory)/RawImage.cpp$(PreprocessSuffix): RawImage.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RawImage.cpp$(PreprocessSuffix) RawImage.cpp

$(IntermediateDirectory)/DngDecoderSlices.cpp$(ObjectSuffix): DngDecoderSlices.cpp $(IntermediateDirectory)/DngDecoderSlices.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/DngDecoderSlices.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/DngDecoderSlices.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/DngDecoderSlices.cpp$(DependSuffix): DngDecoderSlices.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/DngDecoderSlices.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/DngDecoderSlices.cpp$(DependSuffix) -MM DngDecoderSlices.cpp

$(IntermediateDirectory)/DngDecoderSlices.cpp$(PreprocessSuffix): DngDecoderSlices.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/DngDecoderSlices.cpp$(PreprocessSuffix) DngDecoderSlices.cpp

$(IntermediateDirectory)/Common.cpp$(ObjectSuffix): Common.cpp $(IntermediateDirectory)/Common.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/Common.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Common.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Common.cpp$(DependSuffix): Common.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Common.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Common.cpp$(DependSuffix) -MM Common.cpp

$(IntermediateDirectory)/Common.cpp$(PreprocessSuffix): Common.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Common.cpp$(PreprocessSuffix) Common.cpp

$(IntermediateDirectory)/DngDecoder.cpp$(ObjectSuffix): DngDecoder.cpp $(IntermediateDirectory)/DngDecoder.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/DngDecoder.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/DngDecoder.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/DngDecoder.cpp$(DependSuffix): DngDecoder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/DngDecoder.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/DngDecoder.cpp$(DependSuffix) -MM DngDecoder.cpp

$(IntermediateDirectory)/DngDecoder.cpp$(PreprocessSuffix): DngDecoder.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/DngDecoder.cpp$(PreprocessSuffix) DngDecoder.cpp

$(IntermediateDirectory)/TiffIFD.cpp$(ObjectSuffix): TiffIFD.cpp $(IntermediateDirectory)/TiffIFD.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/TiffIFD.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TiffIFD.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TiffIFD.cpp$(DependSuffix): TiffIFD.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TiffIFD.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TiffIFD.cpp$(DependSuffix) -MM TiffIFD.cpp

$(IntermediateDirectory)/TiffIFD.cpp$(PreprocessSuffix): TiffIFD.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TiffIFD.cpp$(PreprocessSuffix) TiffIFD.cpp

$(IntermediateDirectory)/TiffEntryBE.cpp$(ObjectSuffix): TiffEntryBE.cpp $(IntermediateDirectory)/TiffEntryBE.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/TiffEntryBE.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TiffEntryBE.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TiffEntryBE.cpp$(DependSuffix): TiffEntryBE.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TiffEntryBE.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TiffEntryBE.cpp$(DependSuffix) -MM TiffEntryBE.cpp

$(IntermediateDirectory)/TiffEntryBE.cpp$(PreprocessSuffix): TiffEntryBE.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TiffEntryBE.cpp$(PreprocessSuffix) TiffEntryBE.cpp

$(IntermediateDirectory)/TiffIFDBE.cpp$(ObjectSuffix): TiffIFDBE.cpp $(IntermediateDirectory)/TiffIFDBE.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/TiffIFDBE.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TiffIFDBE.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TiffIFDBE.cpp$(DependSuffix): TiffIFDBE.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TiffIFDBE.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TiffIFDBE.cpp$(DependSuffix) -MM TiffIFDBE.cpp

$(IntermediateDirectory)/TiffIFDBE.cpp$(PreprocessSuffix): TiffIFDBE.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TiffIFDBE.cpp$(PreprocessSuffix) TiffIFDBE.cpp

$(IntermediateDirectory)/TiffParserException.cpp$(ObjectSuffix): TiffParserException.cpp $(IntermediateDirectory)/TiffParserException.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/TiffParserException.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TiffParserException.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TiffParserException.cpp$(DependSuffix): TiffParserException.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TiffParserException.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TiffParserException.cpp$(DependSuffix) -MM TiffParserException.cpp

$(IntermediateDirectory)/TiffParserException.cpp$(PreprocessSuffix): TiffParserException.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TiffParserException.cpp$(PreprocessSuffix) TiffParserException.cpp

$(IntermediateDirectory)/TiffParserOlympus.cpp$(ObjectSuffix): TiffParserOlympus.cpp $(IntermediateDirectory)/TiffParserOlympus.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/TiffParserOlympus.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TiffParserOlympus.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TiffParserOlympus.cpp$(DependSuffix): TiffParserOlympus.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TiffParserOlympus.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TiffParserOlympus.cpp$(DependSuffix) -MM TiffParserOlympus.cpp

$(IntermediateDirectory)/TiffParserOlympus.cpp$(PreprocessSuffix): TiffParserOlympus.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TiffParserOlympus.cpp$(PreprocessSuffix) TiffParserOlympus.cpp

$(IntermediateDirectory)/ArwDecoder.cpp$(ObjectSuffix): ArwDecoder.cpp $(IntermediateDirectory)/ArwDecoder.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/ArwDecoder.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ArwDecoder.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ArwDecoder.cpp$(DependSuffix): ArwDecoder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ArwDecoder.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ArwDecoder.cpp$(DependSuffix) -MM ArwDecoder.cpp

$(IntermediateDirectory)/ArwDecoder.cpp$(PreprocessSuffix): ArwDecoder.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ArwDecoder.cpp$(PreprocessSuffix) ArwDecoder.cpp

$(IntermediateDirectory)/ByteStreamSwap.cpp$(ObjectSuffix): ByteStreamSwap.cpp $(IntermediateDirectory)/ByteStreamSwap.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/ByteStreamSwap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ByteStreamSwap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ByteStreamSwap.cpp$(DependSuffix): ByteStreamSwap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ByteStreamSwap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ByteStreamSwap.cpp$(DependSuffix) -MM ByteStreamSwap.cpp

$(IntermediateDirectory)/ByteStreamSwap.cpp$(PreprocessSuffix): ByteStreamSwap.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ByteStreamSwap.cpp$(PreprocessSuffix) ByteStreamSwap.cpp

$(IntermediateDirectory)/FileIOException.cpp$(ObjectSuffix): FileIOException.cpp $(IntermediateDirectory)/FileIOException.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/FileIOException.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FileIOException.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FileIOException.cpp$(DependSuffix): FileIOException.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FileIOException.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FileIOException.cpp$(DependSuffix) -MM FileIOException.cpp

$(IntermediateDirectory)/FileIOException.cpp$(PreprocessSuffix): FileIOException.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FileIOException.cpp$(PreprocessSuffix) FileIOException.cpp

$(IntermediateDirectory)/PefDecoder.cpp$(ObjectSuffix): PefDecoder.cpp $(IntermediateDirectory)/PefDecoder.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/PefDecoder.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PefDecoder.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PefDecoder.cpp$(DependSuffix): PefDecoder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PefDecoder.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PefDecoder.cpp$(DependSuffix) -MM PefDecoder.cpp

$(IntermediateDirectory)/PefDecoder.cpp$(PreprocessSuffix): PefDecoder.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PefDecoder.cpp$(PreprocessSuffix) PefDecoder.cpp

$(IntermediateDirectory)/OrfDecoder.cpp$(ObjectSuffix): OrfDecoder.cpp $(IntermediateDirectory)/OrfDecoder.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/OrfDecoder.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/OrfDecoder.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OrfDecoder.cpp$(DependSuffix): OrfDecoder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/OrfDecoder.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/OrfDecoder.cpp$(DependSuffix) -MM OrfDecoder.cpp

$(IntermediateDirectory)/OrfDecoder.cpp$(PreprocessSuffix): OrfDecoder.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OrfDecoder.cpp$(PreprocessSuffix) OrfDecoder.cpp

$(IntermediateDirectory)/StdAfx.cpp$(ObjectSuffix): StdAfx.cpp $(IntermediateDirectory)/StdAfx.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/Otherlib/RawSpeed/StdAfx.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/StdAfx.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/StdAfx.cpp$(DependSuffix): StdAfx.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/StdAfx.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/StdAfx.cpp$(DependSuffix) -MM StdAfx.cpp

$(IntermediateDirectory)/StdAfx.cpp$(PreprocessSuffix): StdAfx.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/StdAfx.cpp$(PreprocessSuffix) StdAfx.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


