##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libDataStructure
ConfigurationName      :=Debug
WorkspacePath          :=C:/developpement/git/Regards
ProjectPath            :=C:/developpement/git/Regards/libDataStructure
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=jfigu
Date                   :=11/03/2019
CodeLitePath           :="C:/Program Files/CodeLite"
LinkerName             :=C:/msys64/usr/bin/ccache.exe C:/msys64/mingw64/bin/g++.exe
SharedObjectLinkerName :=C:/msys64/usr/bin/ccache.exe C:/msys64/mingw64/bin/g++.exe -shared -fPIC
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
Preprocessors          :=$(PreprocessorSwitch)FFMPEG $(PreprocessorSwitch)TURBOJPEG $(PreprocessorSwitch)EXIV2 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libDataStructure.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/msys64/mingw64/bin/windres.exe
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)"C:\Program Files (x86)\AMD APP SDK\3.0_mingw\include" $(IncludeSwitch)/mingw64/include/libxml2 $(IncludeSwitch)/mingw64/include/OpenEXR  $(IncludeSwitch). $(IncludeSwitch)../libCompress $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libUtility $(IncludeSwitch)../libResource $(IncludeSwitch)../libPicture $(IncludeSwitch)../CxImage $(IncludeSwitch)../libFiltre $(IncludeSwitch)../libextern/libjpeg-turbo-1.5.1 $(IncludeSwitch)../libExif $(IncludeSwitch)../include $(IncludeSwitch)../libSqlData $(IncludeSwitch)../libSqlEngine 
IncludePCH             :=  -include ../include/Debug/header.h 
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
CC       := C:/msys64/usr/bin/ccache.exe C:/msys64/mingw64/bin/gcc.exe
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread  -fopenmp -std=gnu++11 -ggdb -O0 $(Preprocessors)
CFLAGS   :=  -Wall  -ggdb -O0 $(Preprocessors)
ASFLAGS  := 
AS       := C:/msys64/usr/bin/ccache.exe C:/msys64/mingw64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
PATH:=C:\msys64\usr\bin;$PATH
WXWIN:=C:\Regards\libextern\wxwidgets-3.1.2
WXCFG:=lib/mswud
Objects0=$(IntermediateDirectory)/ThumbnailDataStorage.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailData.cpp$(ObjectSuffix) $(IntermediateDirectory)/RGBAQuad.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeDataEffect.cpp$(ObjectSuffix) $(IntermediateDirectory)/RegardsFloatBitmap.cpp$(ObjectSuffix) $(IntermediateDirectory)/PhotoCategorie.cpp$(ObjectSuffix) $(IntermediateDirectory)/FolderCatalog.cpp$(ObjectSuffix) $(IntermediateDirectory)/GeoPluginValue.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeData.cpp$(ObjectSuffix) $(IntermediateDirectory)/RegardsBitmap.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/RegardsJpegPicture.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailDataSQL.cpp$(ObjectSuffix) $(IntermediateDirectory)/Catalog.cpp$(ObjectSuffix) $(IntermediateDirectory)/Country.cpp$(ObjectSuffix) $(IntermediateDirectory)/PhotoCriteria.cpp$(ObjectSuffix) $(IntermediateDirectory)/PhotoGps.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeDataLink.cpp$(ObjectSuffix) $(IntermediateDirectory)/Photos.cpp$(ObjectSuffix) $(IntermediateDirectory)/ImageLoadingFormat.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeDataCategory.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Criteria.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "C:\developpement\git\Regards/.build-debug"
	@echo rebuilt > "C:\developpement\git\Regards/.build-debug/libDataStructure"

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Debug"


./Debug:
	@$(MakeDirCommand) "./Debug"

PreBuild:

# PreCompiled Header
../include/Debug/header.h.gch: ../include/Debug/header.h
	$(CXX) $(SourceSwitch) ../include/Debug/header.h $(PCHCompileFlags) $(CXXFLAGS) $(IncludePath)



##
## Objects
##
$(IntermediateDirectory)/ThumbnailDataStorage.cpp$(ObjectSuffix): ThumbnailDataStorage.cpp $(IntermediateDirectory)/ThumbnailDataStorage.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libDataStructure/ThumbnailDataStorage.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailDataStorage.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailDataStorage.cpp$(DependSuffix): ThumbnailDataStorage.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailDataStorage.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailDataStorage.cpp$(DependSuffix) -MM ThumbnailDataStorage.cpp

$(IntermediateDirectory)/ThumbnailDataStorage.cpp$(PreprocessSuffix): ThumbnailDataStorage.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailDataStorage.cpp$(PreprocessSuffix) ThumbnailDataStorage.cpp

$(IntermediateDirectory)/ThumbnailData.cpp$(ObjectSuffix): ThumbnailData.cpp $(IntermediateDirectory)/ThumbnailData.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libDataStructure/ThumbnailData.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailData.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailData.cpp$(DependSuffix): ThumbnailData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailData.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailData.cpp$(DependSuffix) -MM ThumbnailData.cpp

$(IntermediateDirectory)/ThumbnailData.cpp$(PreprocessSuffix): ThumbnailData.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailData.cpp$(PreprocessSuffix) ThumbnailData.cpp

$(IntermediateDirectory)/RGBAQuad.cpp$(ObjectSuffix): RGBAQuad.cpp $(IntermediateDirectory)/RGBAQuad.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libDataStructure/RGBAQuad.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RGBAQuad.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RGBAQuad.cpp$(DependSuffix): RGBAQuad.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RGBAQuad.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RGBAQuad.cpp$(DependSuffix) -MM RGBAQuad.cpp

$(IntermediateDirectory)/RGBAQuad.cpp$(PreprocessSuffix): RGBAQuad.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RGBAQuad.cpp$(PreprocessSuffix) RGBAQuad.cpp

$(IntermediateDirectory)/TreeDataEffect.cpp$(ObjectSuffix): TreeDataEffect.cpp $(IntermediateDirectory)/TreeDataEffect.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libDataStructure/TreeDataEffect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeDataEffect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeDataEffect.cpp$(DependSuffix): TreeDataEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeDataEffect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeDataEffect.cpp$(DependSuffix) -MM TreeDataEffect.cpp

$(IntermediateDirectory)/TreeDataEffect.cpp$(PreprocessSuffix): TreeDataEffect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeDataEffect.cpp$(PreprocessSuffix) TreeDataEffect.cpp

$(IntermediateDirectory)/RegardsFloatBitmap.cpp$(ObjectSuffix): RegardsFloatBitmap.cpp $(IntermediateDirectory)/RegardsFloatBitmap.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libDataStructure/RegardsFloatBitmap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RegardsFloatBitmap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RegardsFloatBitmap.cpp$(DependSuffix): RegardsFloatBitmap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RegardsFloatBitmap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RegardsFloatBitmap.cpp$(DependSuffix) -MM RegardsFloatBitmap.cpp

$(IntermediateDirectory)/RegardsFloatBitmap.cpp$(PreprocessSuffix): RegardsFloatBitmap.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RegardsFloatBitmap.cpp$(PreprocessSuffix) RegardsFloatBitmap.cpp

$(IntermediateDirectory)/PhotoCategorie.cpp$(ObjectSuffix): PhotoCategorie.cpp $(IntermediateDirectory)/PhotoCategorie.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libDataStructure/PhotoCategorie.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PhotoCategorie.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PhotoCategorie.cpp$(DependSuffix): PhotoCategorie.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PhotoCategorie.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PhotoCategorie.cpp$(DependSuffix) -MM PhotoCategorie.cpp

$(IntermediateDirectory)/PhotoCategorie.cpp$(PreprocessSuffix): PhotoCategorie.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PhotoCategorie.cpp$(PreprocessSuffix) PhotoCategorie.cpp

$(IntermediateDirectory)/FolderCatalog.cpp$(ObjectSuffix): FolderCatalog.cpp $(IntermediateDirectory)/FolderCatalog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libDataStructure/FolderCatalog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FolderCatalog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FolderCatalog.cpp$(DependSuffix): FolderCatalog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FolderCatalog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FolderCatalog.cpp$(DependSuffix) -MM FolderCatalog.cpp

$(IntermediateDirectory)/FolderCatalog.cpp$(PreprocessSuffix): FolderCatalog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FolderCatalog.cpp$(PreprocessSuffix) FolderCatalog.cpp

$(IntermediateDirectory)/GeoPluginValue.cpp$(ObjectSuffix): GeoPluginValue.cpp $(IntermediateDirectory)/GeoPluginValue.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libDataStructure/GeoPluginValue.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GeoPluginValue.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GeoPluginValue.cpp$(DependSuffix): GeoPluginValue.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GeoPluginValue.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GeoPluginValue.cpp$(DependSuffix) -MM GeoPluginValue.cpp

$(IntermediateDirectory)/GeoPluginValue.cpp$(PreprocessSuffix): GeoPluginValue.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GeoPluginValue.cpp$(PreprocessSuffix) GeoPluginValue.cpp

$(IntermediateDirectory)/TreeData.cpp$(ObjectSuffix): TreeData.cpp $(IntermediateDirectory)/TreeData.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libDataStructure/TreeData.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeData.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeData.cpp$(DependSuffix): TreeData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeData.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeData.cpp$(DependSuffix) -MM TreeData.cpp

$(IntermediateDirectory)/TreeData.cpp$(PreprocessSuffix): TreeData.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeData.cpp$(PreprocessSuffix) TreeData.cpp

$(IntermediateDirectory)/RegardsBitmap.cpp$(ObjectSuffix): RegardsBitmap.cpp $(IntermediateDirectory)/RegardsBitmap.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libDataStructure/RegardsBitmap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RegardsBitmap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RegardsBitmap.cpp$(DependSuffix): RegardsBitmap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RegardsBitmap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RegardsBitmap.cpp$(DependSuffix) -MM RegardsBitmap.cpp

$(IntermediateDirectory)/RegardsBitmap.cpp$(PreprocessSuffix): RegardsBitmap.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RegardsBitmap.cpp$(PreprocessSuffix) RegardsBitmap.cpp

$(IntermediateDirectory)/RegardsJpegPicture.cpp$(ObjectSuffix): RegardsJpegPicture.cpp $(IntermediateDirectory)/RegardsJpegPicture.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libDataStructure/RegardsJpegPicture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RegardsJpegPicture.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RegardsJpegPicture.cpp$(DependSuffix): RegardsJpegPicture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RegardsJpegPicture.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RegardsJpegPicture.cpp$(DependSuffix) -MM RegardsJpegPicture.cpp

$(IntermediateDirectory)/RegardsJpegPicture.cpp$(PreprocessSuffix): RegardsJpegPicture.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RegardsJpegPicture.cpp$(PreprocessSuffix) RegardsJpegPicture.cpp

$(IntermediateDirectory)/ThumbnailDataSQL.cpp$(ObjectSuffix): ThumbnailDataSQL.cpp $(IntermediateDirectory)/ThumbnailDataSQL.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libDataStructure/ThumbnailDataSQL.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailDataSQL.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailDataSQL.cpp$(DependSuffix): ThumbnailDataSQL.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailDataSQL.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailDataSQL.cpp$(DependSuffix) -MM ThumbnailDataSQL.cpp

$(IntermediateDirectory)/ThumbnailDataSQL.cpp$(PreprocessSuffix): ThumbnailDataSQL.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailDataSQL.cpp$(PreprocessSuffix) ThumbnailDataSQL.cpp

$(IntermediateDirectory)/Catalog.cpp$(ObjectSuffix): Catalog.cpp $(IntermediateDirectory)/Catalog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libDataStructure/Catalog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Catalog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Catalog.cpp$(DependSuffix): Catalog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Catalog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Catalog.cpp$(DependSuffix) -MM Catalog.cpp

$(IntermediateDirectory)/Catalog.cpp$(PreprocessSuffix): Catalog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Catalog.cpp$(PreprocessSuffix) Catalog.cpp

$(IntermediateDirectory)/Country.cpp$(ObjectSuffix): Country.cpp $(IntermediateDirectory)/Country.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libDataStructure/Country.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Country.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Country.cpp$(DependSuffix): Country.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Country.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Country.cpp$(DependSuffix) -MM Country.cpp

$(IntermediateDirectory)/Country.cpp$(PreprocessSuffix): Country.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Country.cpp$(PreprocessSuffix) Country.cpp

$(IntermediateDirectory)/PhotoCriteria.cpp$(ObjectSuffix): PhotoCriteria.cpp $(IntermediateDirectory)/PhotoCriteria.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libDataStructure/PhotoCriteria.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PhotoCriteria.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PhotoCriteria.cpp$(DependSuffix): PhotoCriteria.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PhotoCriteria.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PhotoCriteria.cpp$(DependSuffix) -MM PhotoCriteria.cpp

$(IntermediateDirectory)/PhotoCriteria.cpp$(PreprocessSuffix): PhotoCriteria.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PhotoCriteria.cpp$(PreprocessSuffix) PhotoCriteria.cpp

$(IntermediateDirectory)/PhotoGps.cpp$(ObjectSuffix): PhotoGps.cpp $(IntermediateDirectory)/PhotoGps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libDataStructure/PhotoGps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PhotoGps.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PhotoGps.cpp$(DependSuffix): PhotoGps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PhotoGps.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PhotoGps.cpp$(DependSuffix) -MM PhotoGps.cpp

$(IntermediateDirectory)/PhotoGps.cpp$(PreprocessSuffix): PhotoGps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PhotoGps.cpp$(PreprocessSuffix) PhotoGps.cpp

$(IntermediateDirectory)/TreeDataLink.cpp$(ObjectSuffix): TreeDataLink.cpp $(IntermediateDirectory)/TreeDataLink.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libDataStructure/TreeDataLink.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeDataLink.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeDataLink.cpp$(DependSuffix): TreeDataLink.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeDataLink.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeDataLink.cpp$(DependSuffix) -MM TreeDataLink.cpp

$(IntermediateDirectory)/TreeDataLink.cpp$(PreprocessSuffix): TreeDataLink.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeDataLink.cpp$(PreprocessSuffix) TreeDataLink.cpp

$(IntermediateDirectory)/Photos.cpp$(ObjectSuffix): Photos.cpp $(IntermediateDirectory)/Photos.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libDataStructure/Photos.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Photos.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Photos.cpp$(DependSuffix): Photos.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Photos.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Photos.cpp$(DependSuffix) -MM Photos.cpp

$(IntermediateDirectory)/Photos.cpp$(PreprocessSuffix): Photos.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Photos.cpp$(PreprocessSuffix) Photos.cpp

$(IntermediateDirectory)/ImageLoadingFormat.cpp$(ObjectSuffix): ImageLoadingFormat.cpp $(IntermediateDirectory)/ImageLoadingFormat.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libDataStructure/ImageLoadingFormat.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ImageLoadingFormat.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ImageLoadingFormat.cpp$(DependSuffix): ImageLoadingFormat.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ImageLoadingFormat.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ImageLoadingFormat.cpp$(DependSuffix) -MM ImageLoadingFormat.cpp

$(IntermediateDirectory)/ImageLoadingFormat.cpp$(PreprocessSuffix): ImageLoadingFormat.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ImageLoadingFormat.cpp$(PreprocessSuffix) ImageLoadingFormat.cpp

$(IntermediateDirectory)/TreeDataCategory.cpp$(ObjectSuffix): TreeDataCategory.cpp $(IntermediateDirectory)/TreeDataCategory.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libDataStructure/TreeDataCategory.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeDataCategory.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeDataCategory.cpp$(DependSuffix): TreeDataCategory.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeDataCategory.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeDataCategory.cpp$(DependSuffix) -MM TreeDataCategory.cpp

$(IntermediateDirectory)/TreeDataCategory.cpp$(PreprocessSuffix): TreeDataCategory.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeDataCategory.cpp$(PreprocessSuffix) TreeDataCategory.cpp

$(IntermediateDirectory)/Criteria.cpp$(ObjectSuffix): Criteria.cpp $(IntermediateDirectory)/Criteria.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libDataStructure/Criteria.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Criteria.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Criteria.cpp$(DependSuffix): Criteria.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Criteria.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Criteria.cpp$(DependSuffix) -MM Criteria.cpp

$(IntermediateDirectory)/Criteria.cpp$(PreprocessSuffix): Criteria.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Criteria.cpp$(PreprocessSuffix) Criteria.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/
	$(RM) ../include/Debug/header.h.gch


