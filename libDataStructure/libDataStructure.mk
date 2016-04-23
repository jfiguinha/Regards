##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libDataStructure
ConfigurationName      :=Debug
WorkspacePath          := "/home/figuinha/dev/Regards"
ProjectPath            := "/home/figuinha/dev/Regards/libDataStructure"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=root
Date                   :=22/09/15
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
ObjectsFileList        :="libDataStructure.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libUtility $(IncludeSwitch)../libResource $(IncludeSwitch)../../libextern/wxWidgets-3.0.2/include $(IncludeSwitch)../../libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0 
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
Objects0=$(IntermediateDirectory)/Catalog.cpp$(ObjectSuffix) $(IntermediateDirectory)/Country.cpp$(ObjectSuffix) $(IntermediateDirectory)/Criteria.cpp$(ObjectSuffix) $(IntermediateDirectory)/FolderCatalog.cpp$(ObjectSuffix) $(IntermediateDirectory)/GeoPluginValue.cpp$(ObjectSuffix) $(IntermediateDirectory)/PhotoCategorie.cpp$(ObjectSuffix) $(IntermediateDirectory)/PhotoCriteria.cpp$(ObjectSuffix) $(IntermediateDirectory)/Photos.cpp$(ObjectSuffix) $(IntermediateDirectory)/RegardsBitmap.cpp$(ObjectSuffix) $(IntermediateDirectory)/RGBAQuad.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/ThumbnailData.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailDataStorage.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeData.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeDataEffect.cpp$(ObjectSuffix) $(IntermediateDirectory)/videothumbnail.cpp$(ObjectSuffix) $(IntermediateDirectory)/TreeDataLink.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/home/figuinha/dev/Regards/.build-debug/libDataStructure"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/Catalog.cpp$(ObjectSuffix): Catalog.cpp $(IntermediateDirectory)/Catalog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libDataStructure/Catalog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Catalog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Catalog.cpp$(DependSuffix): Catalog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Catalog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Catalog.cpp$(DependSuffix) -MM "Catalog.cpp"

$(IntermediateDirectory)/Catalog.cpp$(PreprocessSuffix): Catalog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Catalog.cpp$(PreprocessSuffix) "Catalog.cpp"

$(IntermediateDirectory)/Country.cpp$(ObjectSuffix): Country.cpp $(IntermediateDirectory)/Country.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libDataStructure/Country.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Country.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Country.cpp$(DependSuffix): Country.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Country.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Country.cpp$(DependSuffix) -MM "Country.cpp"

$(IntermediateDirectory)/Country.cpp$(PreprocessSuffix): Country.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Country.cpp$(PreprocessSuffix) "Country.cpp"

$(IntermediateDirectory)/Criteria.cpp$(ObjectSuffix): Criteria.cpp $(IntermediateDirectory)/Criteria.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libDataStructure/Criteria.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Criteria.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Criteria.cpp$(DependSuffix): Criteria.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Criteria.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Criteria.cpp$(DependSuffix) -MM "Criteria.cpp"

$(IntermediateDirectory)/Criteria.cpp$(PreprocessSuffix): Criteria.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Criteria.cpp$(PreprocessSuffix) "Criteria.cpp"

$(IntermediateDirectory)/FolderCatalog.cpp$(ObjectSuffix): FolderCatalog.cpp $(IntermediateDirectory)/FolderCatalog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libDataStructure/FolderCatalog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FolderCatalog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FolderCatalog.cpp$(DependSuffix): FolderCatalog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FolderCatalog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FolderCatalog.cpp$(DependSuffix) -MM "FolderCatalog.cpp"

$(IntermediateDirectory)/FolderCatalog.cpp$(PreprocessSuffix): FolderCatalog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FolderCatalog.cpp$(PreprocessSuffix) "FolderCatalog.cpp"

$(IntermediateDirectory)/GeoPluginValue.cpp$(ObjectSuffix): GeoPluginValue.cpp $(IntermediateDirectory)/GeoPluginValue.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libDataStructure/GeoPluginValue.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GeoPluginValue.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GeoPluginValue.cpp$(DependSuffix): GeoPluginValue.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GeoPluginValue.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GeoPluginValue.cpp$(DependSuffix) -MM "GeoPluginValue.cpp"

$(IntermediateDirectory)/GeoPluginValue.cpp$(PreprocessSuffix): GeoPluginValue.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GeoPluginValue.cpp$(PreprocessSuffix) "GeoPluginValue.cpp"

$(IntermediateDirectory)/PhotoCategorie.cpp$(ObjectSuffix): PhotoCategorie.cpp $(IntermediateDirectory)/PhotoCategorie.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libDataStructure/PhotoCategorie.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PhotoCategorie.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PhotoCategorie.cpp$(DependSuffix): PhotoCategorie.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PhotoCategorie.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PhotoCategorie.cpp$(DependSuffix) -MM "PhotoCategorie.cpp"

$(IntermediateDirectory)/PhotoCategorie.cpp$(PreprocessSuffix): PhotoCategorie.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PhotoCategorie.cpp$(PreprocessSuffix) "PhotoCategorie.cpp"

$(IntermediateDirectory)/PhotoCriteria.cpp$(ObjectSuffix): PhotoCriteria.cpp $(IntermediateDirectory)/PhotoCriteria.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libDataStructure/PhotoCriteria.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PhotoCriteria.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PhotoCriteria.cpp$(DependSuffix): PhotoCriteria.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PhotoCriteria.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PhotoCriteria.cpp$(DependSuffix) -MM "PhotoCriteria.cpp"

$(IntermediateDirectory)/PhotoCriteria.cpp$(PreprocessSuffix): PhotoCriteria.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PhotoCriteria.cpp$(PreprocessSuffix) "PhotoCriteria.cpp"

$(IntermediateDirectory)/Photos.cpp$(ObjectSuffix): Photos.cpp $(IntermediateDirectory)/Photos.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libDataStructure/Photos.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Photos.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Photos.cpp$(DependSuffix): Photos.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Photos.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Photos.cpp$(DependSuffix) -MM "Photos.cpp"

$(IntermediateDirectory)/Photos.cpp$(PreprocessSuffix): Photos.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Photos.cpp$(PreprocessSuffix) "Photos.cpp"

$(IntermediateDirectory)/RegardsBitmap.cpp$(ObjectSuffix): RegardsBitmap.cpp $(IntermediateDirectory)/RegardsBitmap.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libDataStructure/RegardsBitmap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RegardsBitmap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RegardsBitmap.cpp$(DependSuffix): RegardsBitmap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RegardsBitmap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RegardsBitmap.cpp$(DependSuffix) -MM "RegardsBitmap.cpp"

$(IntermediateDirectory)/RegardsBitmap.cpp$(PreprocessSuffix): RegardsBitmap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RegardsBitmap.cpp$(PreprocessSuffix) "RegardsBitmap.cpp"

$(IntermediateDirectory)/RGBAQuad.cpp$(ObjectSuffix): RGBAQuad.cpp $(IntermediateDirectory)/RGBAQuad.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libDataStructure/RGBAQuad.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RGBAQuad.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RGBAQuad.cpp$(DependSuffix): RGBAQuad.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RGBAQuad.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RGBAQuad.cpp$(DependSuffix) -MM "RGBAQuad.cpp"

$(IntermediateDirectory)/RGBAQuad.cpp$(PreprocessSuffix): RGBAQuad.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RGBAQuad.cpp$(PreprocessSuffix) "RGBAQuad.cpp"

$(IntermediateDirectory)/ThumbnailData.cpp$(ObjectSuffix): ThumbnailData.cpp $(IntermediateDirectory)/ThumbnailData.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libDataStructure/ThumbnailData.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailData.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailData.cpp$(DependSuffix): ThumbnailData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailData.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailData.cpp$(DependSuffix) -MM "ThumbnailData.cpp"

$(IntermediateDirectory)/ThumbnailData.cpp$(PreprocessSuffix): ThumbnailData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailData.cpp$(PreprocessSuffix) "ThumbnailData.cpp"

$(IntermediateDirectory)/ThumbnailDataStorage.cpp$(ObjectSuffix): ThumbnailDataStorage.cpp $(IntermediateDirectory)/ThumbnailDataStorage.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libDataStructure/ThumbnailDataStorage.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailDataStorage.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailDataStorage.cpp$(DependSuffix): ThumbnailDataStorage.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailDataStorage.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailDataStorage.cpp$(DependSuffix) -MM "ThumbnailDataStorage.cpp"

$(IntermediateDirectory)/ThumbnailDataStorage.cpp$(PreprocessSuffix): ThumbnailDataStorage.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailDataStorage.cpp$(PreprocessSuffix) "ThumbnailDataStorage.cpp"

$(IntermediateDirectory)/TreeData.cpp$(ObjectSuffix): TreeData.cpp $(IntermediateDirectory)/TreeData.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libDataStructure/TreeData.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeData.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeData.cpp$(DependSuffix): TreeData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeData.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeData.cpp$(DependSuffix) -MM "TreeData.cpp"

$(IntermediateDirectory)/TreeData.cpp$(PreprocessSuffix): TreeData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeData.cpp$(PreprocessSuffix) "TreeData.cpp"

$(IntermediateDirectory)/TreeDataEffect.cpp$(ObjectSuffix): TreeDataEffect.cpp $(IntermediateDirectory)/TreeDataEffect.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libDataStructure/TreeDataEffect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeDataEffect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeDataEffect.cpp$(DependSuffix): TreeDataEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeDataEffect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeDataEffect.cpp$(DependSuffix) -MM "TreeDataEffect.cpp"

$(IntermediateDirectory)/TreeDataEffect.cpp$(PreprocessSuffix): TreeDataEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeDataEffect.cpp$(PreprocessSuffix) "TreeDataEffect.cpp"

$(IntermediateDirectory)/videothumbnail.cpp$(ObjectSuffix): videothumbnail.cpp $(IntermediateDirectory)/videothumbnail.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libDataStructure/videothumbnail.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/videothumbnail.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/videothumbnail.cpp$(DependSuffix): videothumbnail.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/videothumbnail.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/videothumbnail.cpp$(DependSuffix) -MM "videothumbnail.cpp"

$(IntermediateDirectory)/videothumbnail.cpp$(PreprocessSuffix): videothumbnail.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/videothumbnail.cpp$(PreprocessSuffix) "videothumbnail.cpp"

$(IntermediateDirectory)/TreeDataLink.cpp$(ObjectSuffix): TreeDataLink.cpp $(IntermediateDirectory)/TreeDataLink.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libDataStructure/TreeDataLink.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TreeDataLink.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TreeDataLink.cpp$(DependSuffix): TreeDataLink.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TreeDataLink.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TreeDataLink.cpp$(DependSuffix) -MM "TreeDataLink.cpp"

$(IntermediateDirectory)/TreeDataLink.cpp$(PreprocessSuffix): TreeDataLink.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TreeDataLink.cpp$(PreprocessSuffix) "TreeDataLink.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


