##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libSqlData
ConfigurationName      :=Debug
WorkspacePath          :=/home/figuinha/developpement/Regards
ProjectPath            :=/home/figuinha/developpement/Regards/libSqlData
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=10/02/19
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
Preprocessors          :=$(PreprocessorSwitch)TURBOJPEG $(PreprocessorSwitch)FFMPEG $(PreprocessorSwitch)WX_PRECOMP 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libSqlData.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2 $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include  $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libSqlEngine $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libResource $(IncludeSwitch)../libPicture $(IncludeSwitch)../libCompress $(IncludeSwitch)../libUtility $(IncludeSwitch)../libFiltre $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../include $(IncludeSwitch)../libextern/libjpeg-turbo-1.5.1 $(IncludeSwitch)../libextern/libRapidXML 
IncludePCH             :=  -include ../include/Debug/header.h 
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
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread -fopenmp -std=gnu++11 -Wall -ggdb -O0 $(Preprocessors)
CFLAGS   :=  -Wall  -ggdb -O0 $(Preprocessors)
ASFLAGS  := 
AS       := ccache /usr/bin/x86_64-linux-gnu-as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/SqlLibExplorer.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlFolderCatalog.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlFindYear.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailDataSQL.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlFaceLabel.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlFindFolderCatalog.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlPhotoCategorie.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlFindCriteria.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlThumbnailVideo.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlOpenCLKernel.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/SqlCatalog.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlFindMonth.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlCountry.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlInsertFile.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlCriteria.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlPhotos.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlPhotoCriteria.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlVersion.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlFindDay.cpp$(ObjectSuffix) $(IntermediateDirectory)/SQLRemoveData.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/SqlThumbnail.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlGps.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlPhotosWithoutThumbnail.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlFindFacePhoto.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlFaceDescriptor.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlFacePhoto.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlFindPhotos.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlFindCatalog.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlFaceRecognition.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlFaceThumbnail.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/SqlFindLocalisation.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlPhotoGPS.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "/home/figuinha/developpement/Regards/.build-debug"
	@echo rebuilt > "/home/figuinha/developpement/Regards/.build-debug/libSqlData"

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
$(IntermediateDirectory)/SqlLibExplorer.cpp$(ObjectSuffix): SqlLibExplorer.cpp $(IntermediateDirectory)/SqlLibExplorer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlLibExplorer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlLibExplorer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlLibExplorer.cpp$(DependSuffix): SqlLibExplorer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlLibExplorer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlLibExplorer.cpp$(DependSuffix) -MM SqlLibExplorer.cpp

$(IntermediateDirectory)/SqlLibExplorer.cpp$(PreprocessSuffix): SqlLibExplorer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlLibExplorer.cpp$(PreprocessSuffix) SqlLibExplorer.cpp

$(IntermediateDirectory)/SqlFolderCatalog.cpp$(ObjectSuffix): SqlFolderCatalog.cpp $(IntermediateDirectory)/SqlFolderCatalog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlFolderCatalog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlFolderCatalog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlFolderCatalog.cpp$(DependSuffix): SqlFolderCatalog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlFolderCatalog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlFolderCatalog.cpp$(DependSuffix) -MM SqlFolderCatalog.cpp

$(IntermediateDirectory)/SqlFolderCatalog.cpp$(PreprocessSuffix): SqlFolderCatalog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlFolderCatalog.cpp$(PreprocessSuffix) SqlFolderCatalog.cpp

$(IntermediateDirectory)/SqlFindYear.cpp$(ObjectSuffix): SqlFindYear.cpp $(IntermediateDirectory)/SqlFindYear.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlFindYear.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlFindYear.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlFindYear.cpp$(DependSuffix): SqlFindYear.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlFindYear.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlFindYear.cpp$(DependSuffix) -MM SqlFindYear.cpp

$(IntermediateDirectory)/SqlFindYear.cpp$(PreprocessSuffix): SqlFindYear.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlFindYear.cpp$(PreprocessSuffix) SqlFindYear.cpp

$(IntermediateDirectory)/ThumbnailDataSQL.cpp$(ObjectSuffix): ThumbnailDataSQL.cpp $(IntermediateDirectory)/ThumbnailDataSQL.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/ThumbnailDataSQL.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailDataSQL.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailDataSQL.cpp$(DependSuffix): ThumbnailDataSQL.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailDataSQL.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailDataSQL.cpp$(DependSuffix) -MM ThumbnailDataSQL.cpp

$(IntermediateDirectory)/ThumbnailDataSQL.cpp$(PreprocessSuffix): ThumbnailDataSQL.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailDataSQL.cpp$(PreprocessSuffix) ThumbnailDataSQL.cpp

$(IntermediateDirectory)/SqlFaceLabel.cpp$(ObjectSuffix): SqlFaceLabel.cpp $(IntermediateDirectory)/SqlFaceLabel.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlFaceLabel.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlFaceLabel.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlFaceLabel.cpp$(DependSuffix): SqlFaceLabel.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlFaceLabel.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlFaceLabel.cpp$(DependSuffix) -MM SqlFaceLabel.cpp

$(IntermediateDirectory)/SqlFaceLabel.cpp$(PreprocessSuffix): SqlFaceLabel.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlFaceLabel.cpp$(PreprocessSuffix) SqlFaceLabel.cpp

$(IntermediateDirectory)/SqlFindFolderCatalog.cpp$(ObjectSuffix): SqlFindFolderCatalog.cpp $(IntermediateDirectory)/SqlFindFolderCatalog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlFindFolderCatalog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlFindFolderCatalog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlFindFolderCatalog.cpp$(DependSuffix): SqlFindFolderCatalog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlFindFolderCatalog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlFindFolderCatalog.cpp$(DependSuffix) -MM SqlFindFolderCatalog.cpp

$(IntermediateDirectory)/SqlFindFolderCatalog.cpp$(PreprocessSuffix): SqlFindFolderCatalog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlFindFolderCatalog.cpp$(PreprocessSuffix) SqlFindFolderCatalog.cpp

$(IntermediateDirectory)/SqlPhotoCategorie.cpp$(ObjectSuffix): SqlPhotoCategorie.cpp $(IntermediateDirectory)/SqlPhotoCategorie.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlPhotoCategorie.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlPhotoCategorie.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlPhotoCategorie.cpp$(DependSuffix): SqlPhotoCategorie.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlPhotoCategorie.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlPhotoCategorie.cpp$(DependSuffix) -MM SqlPhotoCategorie.cpp

$(IntermediateDirectory)/SqlPhotoCategorie.cpp$(PreprocessSuffix): SqlPhotoCategorie.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlPhotoCategorie.cpp$(PreprocessSuffix) SqlPhotoCategorie.cpp

$(IntermediateDirectory)/SqlFindCriteria.cpp$(ObjectSuffix): SqlFindCriteria.cpp $(IntermediateDirectory)/SqlFindCriteria.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlFindCriteria.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlFindCriteria.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlFindCriteria.cpp$(DependSuffix): SqlFindCriteria.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlFindCriteria.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlFindCriteria.cpp$(DependSuffix) -MM SqlFindCriteria.cpp

$(IntermediateDirectory)/SqlFindCriteria.cpp$(PreprocessSuffix): SqlFindCriteria.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlFindCriteria.cpp$(PreprocessSuffix) SqlFindCriteria.cpp

$(IntermediateDirectory)/SqlThumbnailVideo.cpp$(ObjectSuffix): SqlThumbnailVideo.cpp $(IntermediateDirectory)/SqlThumbnailVideo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlThumbnailVideo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlThumbnailVideo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlThumbnailVideo.cpp$(DependSuffix): SqlThumbnailVideo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlThumbnailVideo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlThumbnailVideo.cpp$(DependSuffix) -MM SqlThumbnailVideo.cpp

$(IntermediateDirectory)/SqlThumbnailVideo.cpp$(PreprocessSuffix): SqlThumbnailVideo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlThumbnailVideo.cpp$(PreprocessSuffix) SqlThumbnailVideo.cpp

$(IntermediateDirectory)/SqlOpenCLKernel.cpp$(ObjectSuffix): SqlOpenCLKernel.cpp $(IntermediateDirectory)/SqlOpenCLKernel.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlOpenCLKernel.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlOpenCLKernel.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlOpenCLKernel.cpp$(DependSuffix): SqlOpenCLKernel.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlOpenCLKernel.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlOpenCLKernel.cpp$(DependSuffix) -MM SqlOpenCLKernel.cpp

$(IntermediateDirectory)/SqlOpenCLKernel.cpp$(PreprocessSuffix): SqlOpenCLKernel.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlOpenCLKernel.cpp$(PreprocessSuffix) SqlOpenCLKernel.cpp

$(IntermediateDirectory)/SqlCatalog.cpp$(ObjectSuffix): SqlCatalog.cpp $(IntermediateDirectory)/SqlCatalog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlCatalog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlCatalog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlCatalog.cpp$(DependSuffix): SqlCatalog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlCatalog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlCatalog.cpp$(DependSuffix) -MM SqlCatalog.cpp

$(IntermediateDirectory)/SqlCatalog.cpp$(PreprocessSuffix): SqlCatalog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlCatalog.cpp$(PreprocessSuffix) SqlCatalog.cpp

$(IntermediateDirectory)/SqlFindMonth.cpp$(ObjectSuffix): SqlFindMonth.cpp $(IntermediateDirectory)/SqlFindMonth.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlFindMonth.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlFindMonth.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlFindMonth.cpp$(DependSuffix): SqlFindMonth.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlFindMonth.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlFindMonth.cpp$(DependSuffix) -MM SqlFindMonth.cpp

$(IntermediateDirectory)/SqlFindMonth.cpp$(PreprocessSuffix): SqlFindMonth.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlFindMonth.cpp$(PreprocessSuffix) SqlFindMonth.cpp

$(IntermediateDirectory)/SqlCountry.cpp$(ObjectSuffix): SqlCountry.cpp $(IntermediateDirectory)/SqlCountry.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlCountry.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlCountry.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlCountry.cpp$(DependSuffix): SqlCountry.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlCountry.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlCountry.cpp$(DependSuffix) -MM SqlCountry.cpp

$(IntermediateDirectory)/SqlCountry.cpp$(PreprocessSuffix): SqlCountry.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlCountry.cpp$(PreprocessSuffix) SqlCountry.cpp

$(IntermediateDirectory)/SqlInsertFile.cpp$(ObjectSuffix): SqlInsertFile.cpp $(IntermediateDirectory)/SqlInsertFile.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlInsertFile.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlInsertFile.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlInsertFile.cpp$(DependSuffix): SqlInsertFile.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlInsertFile.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlInsertFile.cpp$(DependSuffix) -MM SqlInsertFile.cpp

$(IntermediateDirectory)/SqlInsertFile.cpp$(PreprocessSuffix): SqlInsertFile.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlInsertFile.cpp$(PreprocessSuffix) SqlInsertFile.cpp

$(IntermediateDirectory)/SqlCriteria.cpp$(ObjectSuffix): SqlCriteria.cpp $(IntermediateDirectory)/SqlCriteria.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlCriteria.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlCriteria.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlCriteria.cpp$(DependSuffix): SqlCriteria.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlCriteria.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlCriteria.cpp$(DependSuffix) -MM SqlCriteria.cpp

$(IntermediateDirectory)/SqlCriteria.cpp$(PreprocessSuffix): SqlCriteria.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlCriteria.cpp$(PreprocessSuffix) SqlCriteria.cpp

$(IntermediateDirectory)/SqlPhotos.cpp$(ObjectSuffix): SqlPhotos.cpp $(IntermediateDirectory)/SqlPhotos.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlPhotos.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlPhotos.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlPhotos.cpp$(DependSuffix): SqlPhotos.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlPhotos.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlPhotos.cpp$(DependSuffix) -MM SqlPhotos.cpp

$(IntermediateDirectory)/SqlPhotos.cpp$(PreprocessSuffix): SqlPhotos.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlPhotos.cpp$(PreprocessSuffix) SqlPhotos.cpp

$(IntermediateDirectory)/SqlPhotoCriteria.cpp$(ObjectSuffix): SqlPhotoCriteria.cpp $(IntermediateDirectory)/SqlPhotoCriteria.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlPhotoCriteria.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlPhotoCriteria.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlPhotoCriteria.cpp$(DependSuffix): SqlPhotoCriteria.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlPhotoCriteria.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlPhotoCriteria.cpp$(DependSuffix) -MM SqlPhotoCriteria.cpp

$(IntermediateDirectory)/SqlPhotoCriteria.cpp$(PreprocessSuffix): SqlPhotoCriteria.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlPhotoCriteria.cpp$(PreprocessSuffix) SqlPhotoCriteria.cpp

$(IntermediateDirectory)/SqlVersion.cpp$(ObjectSuffix): SqlVersion.cpp $(IntermediateDirectory)/SqlVersion.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlVersion.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlVersion.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlVersion.cpp$(DependSuffix): SqlVersion.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlVersion.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlVersion.cpp$(DependSuffix) -MM SqlVersion.cpp

$(IntermediateDirectory)/SqlVersion.cpp$(PreprocessSuffix): SqlVersion.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlVersion.cpp$(PreprocessSuffix) SqlVersion.cpp

$(IntermediateDirectory)/SqlFindDay.cpp$(ObjectSuffix): SqlFindDay.cpp $(IntermediateDirectory)/SqlFindDay.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlFindDay.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlFindDay.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlFindDay.cpp$(DependSuffix): SqlFindDay.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlFindDay.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlFindDay.cpp$(DependSuffix) -MM SqlFindDay.cpp

$(IntermediateDirectory)/SqlFindDay.cpp$(PreprocessSuffix): SqlFindDay.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlFindDay.cpp$(PreprocessSuffix) SqlFindDay.cpp

$(IntermediateDirectory)/SQLRemoveData.cpp$(ObjectSuffix): SQLRemoveData.cpp $(IntermediateDirectory)/SQLRemoveData.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SQLRemoveData.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SQLRemoveData.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SQLRemoveData.cpp$(DependSuffix): SQLRemoveData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SQLRemoveData.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SQLRemoveData.cpp$(DependSuffix) -MM SQLRemoveData.cpp

$(IntermediateDirectory)/SQLRemoveData.cpp$(PreprocessSuffix): SQLRemoveData.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SQLRemoveData.cpp$(PreprocessSuffix) SQLRemoveData.cpp

$(IntermediateDirectory)/SqlThumbnail.cpp$(ObjectSuffix): SqlThumbnail.cpp $(IntermediateDirectory)/SqlThumbnail.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlThumbnail.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlThumbnail.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlThumbnail.cpp$(DependSuffix): SqlThumbnail.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlThumbnail.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlThumbnail.cpp$(DependSuffix) -MM SqlThumbnail.cpp

$(IntermediateDirectory)/SqlThumbnail.cpp$(PreprocessSuffix): SqlThumbnail.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlThumbnail.cpp$(PreprocessSuffix) SqlThumbnail.cpp

$(IntermediateDirectory)/SqlGps.cpp$(ObjectSuffix): SqlGps.cpp $(IntermediateDirectory)/SqlGps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlGps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlGps.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlGps.cpp$(DependSuffix): SqlGps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlGps.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlGps.cpp$(DependSuffix) -MM SqlGps.cpp

$(IntermediateDirectory)/SqlGps.cpp$(PreprocessSuffix): SqlGps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlGps.cpp$(PreprocessSuffix) SqlGps.cpp

$(IntermediateDirectory)/SqlPhotosWithoutThumbnail.cpp$(ObjectSuffix): SqlPhotosWithoutThumbnail.cpp $(IntermediateDirectory)/SqlPhotosWithoutThumbnail.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlPhotosWithoutThumbnail.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlPhotosWithoutThumbnail.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlPhotosWithoutThumbnail.cpp$(DependSuffix): SqlPhotosWithoutThumbnail.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlPhotosWithoutThumbnail.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlPhotosWithoutThumbnail.cpp$(DependSuffix) -MM SqlPhotosWithoutThumbnail.cpp

$(IntermediateDirectory)/SqlPhotosWithoutThumbnail.cpp$(PreprocessSuffix): SqlPhotosWithoutThumbnail.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlPhotosWithoutThumbnail.cpp$(PreprocessSuffix) SqlPhotosWithoutThumbnail.cpp

$(IntermediateDirectory)/SqlFindFacePhoto.cpp$(ObjectSuffix): SqlFindFacePhoto.cpp $(IntermediateDirectory)/SqlFindFacePhoto.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlFindFacePhoto.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlFindFacePhoto.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlFindFacePhoto.cpp$(DependSuffix): SqlFindFacePhoto.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlFindFacePhoto.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlFindFacePhoto.cpp$(DependSuffix) -MM SqlFindFacePhoto.cpp

$(IntermediateDirectory)/SqlFindFacePhoto.cpp$(PreprocessSuffix): SqlFindFacePhoto.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlFindFacePhoto.cpp$(PreprocessSuffix) SqlFindFacePhoto.cpp

$(IntermediateDirectory)/SqlFaceDescriptor.cpp$(ObjectSuffix): SqlFaceDescriptor.cpp $(IntermediateDirectory)/SqlFaceDescriptor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlFaceDescriptor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlFaceDescriptor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlFaceDescriptor.cpp$(DependSuffix): SqlFaceDescriptor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlFaceDescriptor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlFaceDescriptor.cpp$(DependSuffix) -MM SqlFaceDescriptor.cpp

$(IntermediateDirectory)/SqlFaceDescriptor.cpp$(PreprocessSuffix): SqlFaceDescriptor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlFaceDescriptor.cpp$(PreprocessSuffix) SqlFaceDescriptor.cpp

$(IntermediateDirectory)/SqlFacePhoto.cpp$(ObjectSuffix): SqlFacePhoto.cpp $(IntermediateDirectory)/SqlFacePhoto.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlFacePhoto.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlFacePhoto.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlFacePhoto.cpp$(DependSuffix): SqlFacePhoto.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlFacePhoto.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlFacePhoto.cpp$(DependSuffix) -MM SqlFacePhoto.cpp

$(IntermediateDirectory)/SqlFacePhoto.cpp$(PreprocessSuffix): SqlFacePhoto.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlFacePhoto.cpp$(PreprocessSuffix) SqlFacePhoto.cpp

$(IntermediateDirectory)/SqlFindPhotos.cpp$(ObjectSuffix): SqlFindPhotos.cpp $(IntermediateDirectory)/SqlFindPhotos.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlFindPhotos.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlFindPhotos.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlFindPhotos.cpp$(DependSuffix): SqlFindPhotos.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlFindPhotos.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlFindPhotos.cpp$(DependSuffix) -MM SqlFindPhotos.cpp

$(IntermediateDirectory)/SqlFindPhotos.cpp$(PreprocessSuffix): SqlFindPhotos.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlFindPhotos.cpp$(PreprocessSuffix) SqlFindPhotos.cpp

$(IntermediateDirectory)/SqlFindCatalog.cpp$(ObjectSuffix): SqlFindCatalog.cpp $(IntermediateDirectory)/SqlFindCatalog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlFindCatalog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlFindCatalog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlFindCatalog.cpp$(DependSuffix): SqlFindCatalog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlFindCatalog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlFindCatalog.cpp$(DependSuffix) -MM SqlFindCatalog.cpp

$(IntermediateDirectory)/SqlFindCatalog.cpp$(PreprocessSuffix): SqlFindCatalog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlFindCatalog.cpp$(PreprocessSuffix) SqlFindCatalog.cpp

$(IntermediateDirectory)/SqlFaceRecognition.cpp$(ObjectSuffix): SqlFaceRecognition.cpp $(IntermediateDirectory)/SqlFaceRecognition.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlFaceRecognition.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlFaceRecognition.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlFaceRecognition.cpp$(DependSuffix): SqlFaceRecognition.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlFaceRecognition.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlFaceRecognition.cpp$(DependSuffix) -MM SqlFaceRecognition.cpp

$(IntermediateDirectory)/SqlFaceRecognition.cpp$(PreprocessSuffix): SqlFaceRecognition.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlFaceRecognition.cpp$(PreprocessSuffix) SqlFaceRecognition.cpp

$(IntermediateDirectory)/SqlFaceThumbnail.cpp$(ObjectSuffix): SqlFaceThumbnail.cpp $(IntermediateDirectory)/SqlFaceThumbnail.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlFaceThumbnail.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlFaceThumbnail.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlFaceThumbnail.cpp$(DependSuffix): SqlFaceThumbnail.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlFaceThumbnail.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlFaceThumbnail.cpp$(DependSuffix) -MM SqlFaceThumbnail.cpp

$(IntermediateDirectory)/SqlFaceThumbnail.cpp$(PreprocessSuffix): SqlFaceThumbnail.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlFaceThumbnail.cpp$(PreprocessSuffix) SqlFaceThumbnail.cpp

$(IntermediateDirectory)/SqlFindLocalisation.cpp$(ObjectSuffix): SqlFindLocalisation.cpp $(IntermediateDirectory)/SqlFindLocalisation.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlFindLocalisation.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlFindLocalisation.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlFindLocalisation.cpp$(DependSuffix): SqlFindLocalisation.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlFindLocalisation.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlFindLocalisation.cpp$(DependSuffix) -MM SqlFindLocalisation.cpp

$(IntermediateDirectory)/SqlFindLocalisation.cpp$(PreprocessSuffix): SqlFindLocalisation.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlFindLocalisation.cpp$(PreprocessSuffix) SqlFindLocalisation.cpp

$(IntermediateDirectory)/SqlPhotoGPS.cpp$(ObjectSuffix): SqlPhotoGPS.cpp $(IntermediateDirectory)/SqlPhotoGPS.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/libSqlData/SqlPhotoGPS.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlPhotoGPS.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlPhotoGPS.cpp$(DependSuffix): SqlPhotoGPS.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlPhotoGPS.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlPhotoGPS.cpp$(DependSuffix) -MM SqlPhotoGPS.cpp

$(IntermediateDirectory)/SqlPhotoGPS.cpp$(PreprocessSuffix): SqlPhotoGPS.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlPhotoGPS.cpp$(PreprocessSuffix) SqlPhotoGPS.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/
	$(RM) ../include/Debug/header.h.gch


