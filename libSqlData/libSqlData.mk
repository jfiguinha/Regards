##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libSqlData
ConfigurationName      :=Debug
WorkspacePath          := "/home/figuinha/dev/Regards"
ProjectPath            := "/home/figuinha/dev/Regards/libSqlData"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=25/04/16
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
ObjectsFileList        :="libSqlData.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libSqlEngine $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libResource $(IncludeSwitch)../libPicture $(IncludeSwitch)../libCompress $(IncludeSwitch)../libUtility $(IncludeSwitch)../libFiltre $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../include $(IncludeSwitch)../libextern/wxWidgets-3.0.2/include $(IncludeSwitch)../libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0 
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
Objects0=$(IntermediateDirectory)/SqlCatalog.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlCountry.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlCriteria.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlFindCatalog.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlFindCriteria.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlFindDay.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlFindFolderCatalog.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlFindLocalisation.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlFindMonth.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlFindPhotos.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/SqlFindYear.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlFolderCatalog.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlInsertFile.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlLibExplorer.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlPhotoCategorie.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlPhotoCriteria.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlPhotos.cpp$(ObjectSuffix) $(IntermediateDirectory)/SQLRemoveData.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlThumbnail.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailDataSQL.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/SqlGps.cpp$(ObjectSuffix) $(IntermediateDirectory)/SqlVersion.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/home/figuinha/dev/Regards/.build-debug/libSqlData"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/SqlCatalog.cpp$(ObjectSuffix): SqlCatalog.cpp $(IntermediateDirectory)/SqlCatalog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libSqlData/SqlCatalog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlCatalog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlCatalog.cpp$(DependSuffix): SqlCatalog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlCatalog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlCatalog.cpp$(DependSuffix) -MM "SqlCatalog.cpp"

$(IntermediateDirectory)/SqlCatalog.cpp$(PreprocessSuffix): SqlCatalog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlCatalog.cpp$(PreprocessSuffix) "SqlCatalog.cpp"

$(IntermediateDirectory)/SqlCountry.cpp$(ObjectSuffix): SqlCountry.cpp $(IntermediateDirectory)/SqlCountry.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libSqlData/SqlCountry.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlCountry.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlCountry.cpp$(DependSuffix): SqlCountry.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlCountry.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlCountry.cpp$(DependSuffix) -MM "SqlCountry.cpp"

$(IntermediateDirectory)/SqlCountry.cpp$(PreprocessSuffix): SqlCountry.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlCountry.cpp$(PreprocessSuffix) "SqlCountry.cpp"

$(IntermediateDirectory)/SqlCriteria.cpp$(ObjectSuffix): SqlCriteria.cpp $(IntermediateDirectory)/SqlCriteria.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libSqlData/SqlCriteria.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlCriteria.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlCriteria.cpp$(DependSuffix): SqlCriteria.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlCriteria.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlCriteria.cpp$(DependSuffix) -MM "SqlCriteria.cpp"

$(IntermediateDirectory)/SqlCriteria.cpp$(PreprocessSuffix): SqlCriteria.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlCriteria.cpp$(PreprocessSuffix) "SqlCriteria.cpp"

$(IntermediateDirectory)/SqlFindCatalog.cpp$(ObjectSuffix): SqlFindCatalog.cpp $(IntermediateDirectory)/SqlFindCatalog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libSqlData/SqlFindCatalog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlFindCatalog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlFindCatalog.cpp$(DependSuffix): SqlFindCatalog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlFindCatalog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlFindCatalog.cpp$(DependSuffix) -MM "SqlFindCatalog.cpp"

$(IntermediateDirectory)/SqlFindCatalog.cpp$(PreprocessSuffix): SqlFindCatalog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlFindCatalog.cpp$(PreprocessSuffix) "SqlFindCatalog.cpp"

$(IntermediateDirectory)/SqlFindCriteria.cpp$(ObjectSuffix): SqlFindCriteria.cpp $(IntermediateDirectory)/SqlFindCriteria.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libSqlData/SqlFindCriteria.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlFindCriteria.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlFindCriteria.cpp$(DependSuffix): SqlFindCriteria.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlFindCriteria.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlFindCriteria.cpp$(DependSuffix) -MM "SqlFindCriteria.cpp"

$(IntermediateDirectory)/SqlFindCriteria.cpp$(PreprocessSuffix): SqlFindCriteria.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlFindCriteria.cpp$(PreprocessSuffix) "SqlFindCriteria.cpp"

$(IntermediateDirectory)/SqlFindDay.cpp$(ObjectSuffix): SqlFindDay.cpp $(IntermediateDirectory)/SqlFindDay.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libSqlData/SqlFindDay.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlFindDay.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlFindDay.cpp$(DependSuffix): SqlFindDay.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlFindDay.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlFindDay.cpp$(DependSuffix) -MM "SqlFindDay.cpp"

$(IntermediateDirectory)/SqlFindDay.cpp$(PreprocessSuffix): SqlFindDay.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlFindDay.cpp$(PreprocessSuffix) "SqlFindDay.cpp"

$(IntermediateDirectory)/SqlFindFolderCatalog.cpp$(ObjectSuffix): SqlFindFolderCatalog.cpp $(IntermediateDirectory)/SqlFindFolderCatalog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libSqlData/SqlFindFolderCatalog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlFindFolderCatalog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlFindFolderCatalog.cpp$(DependSuffix): SqlFindFolderCatalog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlFindFolderCatalog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlFindFolderCatalog.cpp$(DependSuffix) -MM "SqlFindFolderCatalog.cpp"

$(IntermediateDirectory)/SqlFindFolderCatalog.cpp$(PreprocessSuffix): SqlFindFolderCatalog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlFindFolderCatalog.cpp$(PreprocessSuffix) "SqlFindFolderCatalog.cpp"

$(IntermediateDirectory)/SqlFindLocalisation.cpp$(ObjectSuffix): SqlFindLocalisation.cpp $(IntermediateDirectory)/SqlFindLocalisation.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libSqlData/SqlFindLocalisation.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlFindLocalisation.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlFindLocalisation.cpp$(DependSuffix): SqlFindLocalisation.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlFindLocalisation.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlFindLocalisation.cpp$(DependSuffix) -MM "SqlFindLocalisation.cpp"

$(IntermediateDirectory)/SqlFindLocalisation.cpp$(PreprocessSuffix): SqlFindLocalisation.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlFindLocalisation.cpp$(PreprocessSuffix) "SqlFindLocalisation.cpp"

$(IntermediateDirectory)/SqlFindMonth.cpp$(ObjectSuffix): SqlFindMonth.cpp $(IntermediateDirectory)/SqlFindMonth.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libSqlData/SqlFindMonth.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlFindMonth.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlFindMonth.cpp$(DependSuffix): SqlFindMonth.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlFindMonth.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlFindMonth.cpp$(DependSuffix) -MM "SqlFindMonth.cpp"

$(IntermediateDirectory)/SqlFindMonth.cpp$(PreprocessSuffix): SqlFindMonth.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlFindMonth.cpp$(PreprocessSuffix) "SqlFindMonth.cpp"

$(IntermediateDirectory)/SqlFindPhotos.cpp$(ObjectSuffix): SqlFindPhotos.cpp $(IntermediateDirectory)/SqlFindPhotos.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libSqlData/SqlFindPhotos.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlFindPhotos.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlFindPhotos.cpp$(DependSuffix): SqlFindPhotos.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlFindPhotos.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlFindPhotos.cpp$(DependSuffix) -MM "SqlFindPhotos.cpp"

$(IntermediateDirectory)/SqlFindPhotos.cpp$(PreprocessSuffix): SqlFindPhotos.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlFindPhotos.cpp$(PreprocessSuffix) "SqlFindPhotos.cpp"

$(IntermediateDirectory)/SqlFindYear.cpp$(ObjectSuffix): SqlFindYear.cpp $(IntermediateDirectory)/SqlFindYear.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libSqlData/SqlFindYear.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlFindYear.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlFindYear.cpp$(DependSuffix): SqlFindYear.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlFindYear.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlFindYear.cpp$(DependSuffix) -MM "SqlFindYear.cpp"

$(IntermediateDirectory)/SqlFindYear.cpp$(PreprocessSuffix): SqlFindYear.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlFindYear.cpp$(PreprocessSuffix) "SqlFindYear.cpp"

$(IntermediateDirectory)/SqlFolderCatalog.cpp$(ObjectSuffix): SqlFolderCatalog.cpp $(IntermediateDirectory)/SqlFolderCatalog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libSqlData/SqlFolderCatalog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlFolderCatalog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlFolderCatalog.cpp$(DependSuffix): SqlFolderCatalog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlFolderCatalog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlFolderCatalog.cpp$(DependSuffix) -MM "SqlFolderCatalog.cpp"

$(IntermediateDirectory)/SqlFolderCatalog.cpp$(PreprocessSuffix): SqlFolderCatalog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlFolderCatalog.cpp$(PreprocessSuffix) "SqlFolderCatalog.cpp"

$(IntermediateDirectory)/SqlInsertFile.cpp$(ObjectSuffix): SqlInsertFile.cpp $(IntermediateDirectory)/SqlInsertFile.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libSqlData/SqlInsertFile.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlInsertFile.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlInsertFile.cpp$(DependSuffix): SqlInsertFile.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlInsertFile.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlInsertFile.cpp$(DependSuffix) -MM "SqlInsertFile.cpp"

$(IntermediateDirectory)/SqlInsertFile.cpp$(PreprocessSuffix): SqlInsertFile.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlInsertFile.cpp$(PreprocessSuffix) "SqlInsertFile.cpp"

$(IntermediateDirectory)/SqlLibExplorer.cpp$(ObjectSuffix): SqlLibExplorer.cpp $(IntermediateDirectory)/SqlLibExplorer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libSqlData/SqlLibExplorer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlLibExplorer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlLibExplorer.cpp$(DependSuffix): SqlLibExplorer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlLibExplorer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlLibExplorer.cpp$(DependSuffix) -MM "SqlLibExplorer.cpp"

$(IntermediateDirectory)/SqlLibExplorer.cpp$(PreprocessSuffix): SqlLibExplorer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlLibExplorer.cpp$(PreprocessSuffix) "SqlLibExplorer.cpp"

$(IntermediateDirectory)/SqlPhotoCategorie.cpp$(ObjectSuffix): SqlPhotoCategorie.cpp $(IntermediateDirectory)/SqlPhotoCategorie.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libSqlData/SqlPhotoCategorie.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlPhotoCategorie.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlPhotoCategorie.cpp$(DependSuffix): SqlPhotoCategorie.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlPhotoCategorie.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlPhotoCategorie.cpp$(DependSuffix) -MM "SqlPhotoCategorie.cpp"

$(IntermediateDirectory)/SqlPhotoCategorie.cpp$(PreprocessSuffix): SqlPhotoCategorie.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlPhotoCategorie.cpp$(PreprocessSuffix) "SqlPhotoCategorie.cpp"

$(IntermediateDirectory)/SqlPhotoCriteria.cpp$(ObjectSuffix): SqlPhotoCriteria.cpp $(IntermediateDirectory)/SqlPhotoCriteria.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libSqlData/SqlPhotoCriteria.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlPhotoCriteria.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlPhotoCriteria.cpp$(DependSuffix): SqlPhotoCriteria.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlPhotoCriteria.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlPhotoCriteria.cpp$(DependSuffix) -MM "SqlPhotoCriteria.cpp"

$(IntermediateDirectory)/SqlPhotoCriteria.cpp$(PreprocessSuffix): SqlPhotoCriteria.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlPhotoCriteria.cpp$(PreprocessSuffix) "SqlPhotoCriteria.cpp"

$(IntermediateDirectory)/SqlPhotos.cpp$(ObjectSuffix): SqlPhotos.cpp $(IntermediateDirectory)/SqlPhotos.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libSqlData/SqlPhotos.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlPhotos.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlPhotos.cpp$(DependSuffix): SqlPhotos.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlPhotos.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlPhotos.cpp$(DependSuffix) -MM "SqlPhotos.cpp"

$(IntermediateDirectory)/SqlPhotos.cpp$(PreprocessSuffix): SqlPhotos.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlPhotos.cpp$(PreprocessSuffix) "SqlPhotos.cpp"

$(IntermediateDirectory)/SQLRemoveData.cpp$(ObjectSuffix): SQLRemoveData.cpp $(IntermediateDirectory)/SQLRemoveData.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libSqlData/SQLRemoveData.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SQLRemoveData.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SQLRemoveData.cpp$(DependSuffix): SQLRemoveData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SQLRemoveData.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SQLRemoveData.cpp$(DependSuffix) -MM "SQLRemoveData.cpp"

$(IntermediateDirectory)/SQLRemoveData.cpp$(PreprocessSuffix): SQLRemoveData.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SQLRemoveData.cpp$(PreprocessSuffix) "SQLRemoveData.cpp"

$(IntermediateDirectory)/SqlThumbnail.cpp$(ObjectSuffix): SqlThumbnail.cpp $(IntermediateDirectory)/SqlThumbnail.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libSqlData/SqlThumbnail.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlThumbnail.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlThumbnail.cpp$(DependSuffix): SqlThumbnail.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlThumbnail.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlThumbnail.cpp$(DependSuffix) -MM "SqlThumbnail.cpp"

$(IntermediateDirectory)/SqlThumbnail.cpp$(PreprocessSuffix): SqlThumbnail.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlThumbnail.cpp$(PreprocessSuffix) "SqlThumbnail.cpp"

$(IntermediateDirectory)/ThumbnailDataSQL.cpp$(ObjectSuffix): ThumbnailDataSQL.cpp $(IntermediateDirectory)/ThumbnailDataSQL.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libSqlData/ThumbnailDataSQL.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailDataSQL.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailDataSQL.cpp$(DependSuffix): ThumbnailDataSQL.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailDataSQL.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailDataSQL.cpp$(DependSuffix) -MM "ThumbnailDataSQL.cpp"

$(IntermediateDirectory)/ThumbnailDataSQL.cpp$(PreprocessSuffix): ThumbnailDataSQL.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailDataSQL.cpp$(PreprocessSuffix) "ThumbnailDataSQL.cpp"

$(IntermediateDirectory)/SqlGps.cpp$(ObjectSuffix): SqlGps.cpp $(IntermediateDirectory)/SqlGps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libSqlData/SqlGps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlGps.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlGps.cpp$(DependSuffix): SqlGps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlGps.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlGps.cpp$(DependSuffix) -MM "SqlGps.cpp"

$(IntermediateDirectory)/SqlGps.cpp$(PreprocessSuffix): SqlGps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlGps.cpp$(PreprocessSuffix) "SqlGps.cpp"

$(IntermediateDirectory)/SqlVersion.cpp$(ObjectSuffix): SqlVersion.cpp $(IntermediateDirectory)/SqlVersion.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libSqlData/SqlVersion.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SqlVersion.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SqlVersion.cpp$(DependSuffix): SqlVersion.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SqlVersion.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SqlVersion.cpp$(DependSuffix) -MM "SqlVersion.cpp"

$(IntermediateDirectory)/SqlVersion.cpp$(PreprocessSuffix): SqlVersion.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SqlVersion.cpp$(PreprocessSuffix) "SqlVersion.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


