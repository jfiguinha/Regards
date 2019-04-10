##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libControl
ConfigurationName      :=Debug
WorkspacePath          :=/home/figuinha/developpement/git/Regards
ProjectPath            :=/home/figuinha/developpement/git/Regards/libControl
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=10/04/19
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
Preprocessors          :=$(PreprocessorSwitch)EXIV2 $(PreprocessorSwitch)FFMPEG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libControl.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2 $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include  $(IncludeSwitch)../Otherlib/RegardsRaw $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libTheme $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libParameter $(IncludeSwitch)../libExif $(IncludeSwitch)../libUtility $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libResource $(IncludeSwitch)../libSqlData $(IncludeSwitch)../libRenderBitmap $(IncludeSwitch)../libSqlEngine $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libFiltre $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../Otherlib/wxSVG/include $(IncludeSwitch)../libWindow $(IncludeSwitch)../libPrint $(IncludeSwitch)../libPicture $(IncludeSwitch)../libTheme $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libWindowMain $(IncludeSwitch)../libEmail $(IncludeSwitch)../libOpenCL $(IncludeSwitch)../libOpenGL $(IncludeSwitch)../CxImage $(IncludeSwitch)../libextern/glew-2.1.0/include $(IncludeSwitch)../libDialog $(IncludeSwitch)../libAppleFunction $(IncludeSwitch)../libPiccante $(IncludeSwitch)../libextern/OpenCL-Headers 
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
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread -fopenmp -std=gnu++11 -ggdb  -O0 $(Preprocessors)
CFLAGS   :=  -Wall  -ggdb  -O0 $(Preprocessors)
ASFLAGS  := 
AS       := ccache /usr/bin/x86_64-linux-gnu-as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/BitmapWndViewer.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailHorizontal.cpp$(ObjectSuffix) $(IntermediateDirectory)/CriteriaTreeWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/ClickToolbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/BitmapPrintout.cpp$(ObjectSuffix) $(IntermediateDirectory)/InfosSeparationBarEffect.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailVideo.cpp$(ObjectSuffix) $(IntermediateDirectory)/BitmapToolbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/ShowBitmap.cpp$(ObjectSuffix) $(IntermediateDirectory)/InfosFile.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/ThumbnailVertical.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailEffect.cpp$(ObjectSuffix) $(IntermediateDirectory)/CriteriaTree.cpp$(ObjectSuffix) $(IntermediateDirectory)/InfosFileWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/SplitterWithPanel.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "/home/figuinha/developpement/git/Regards/.build-debug"
	@echo rebuilt > "/home/figuinha/developpement/git/Regards/.build-debug/libControl"

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
$(IntermediateDirectory)/BitmapWndViewer.cpp$(ObjectSuffix): BitmapWndViewer.cpp $(IntermediateDirectory)/BitmapWndViewer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libControl/BitmapWndViewer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BitmapWndViewer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BitmapWndViewer.cpp$(DependSuffix): BitmapWndViewer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BitmapWndViewer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BitmapWndViewer.cpp$(DependSuffix) -MM BitmapWndViewer.cpp

$(IntermediateDirectory)/BitmapWndViewer.cpp$(PreprocessSuffix): BitmapWndViewer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BitmapWndViewer.cpp$(PreprocessSuffix) BitmapWndViewer.cpp

$(IntermediateDirectory)/ThumbnailHorizontal.cpp$(ObjectSuffix): ThumbnailHorizontal.cpp $(IntermediateDirectory)/ThumbnailHorizontal.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libControl/ThumbnailHorizontal.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailHorizontal.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailHorizontal.cpp$(DependSuffix): ThumbnailHorizontal.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailHorizontal.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailHorizontal.cpp$(DependSuffix) -MM ThumbnailHorizontal.cpp

$(IntermediateDirectory)/ThumbnailHorizontal.cpp$(PreprocessSuffix): ThumbnailHorizontal.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailHorizontal.cpp$(PreprocessSuffix) ThumbnailHorizontal.cpp

$(IntermediateDirectory)/CriteriaTreeWnd.cpp$(ObjectSuffix): CriteriaTreeWnd.cpp $(IntermediateDirectory)/CriteriaTreeWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libControl/CriteriaTreeWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CriteriaTreeWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CriteriaTreeWnd.cpp$(DependSuffix): CriteriaTreeWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CriteriaTreeWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CriteriaTreeWnd.cpp$(DependSuffix) -MM CriteriaTreeWnd.cpp

$(IntermediateDirectory)/CriteriaTreeWnd.cpp$(PreprocessSuffix): CriteriaTreeWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CriteriaTreeWnd.cpp$(PreprocessSuffix) CriteriaTreeWnd.cpp

$(IntermediateDirectory)/ClickToolbar.cpp$(ObjectSuffix): ClickToolbar.cpp $(IntermediateDirectory)/ClickToolbar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libControl/ClickToolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ClickToolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ClickToolbar.cpp$(DependSuffix): ClickToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ClickToolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ClickToolbar.cpp$(DependSuffix) -MM ClickToolbar.cpp

$(IntermediateDirectory)/ClickToolbar.cpp$(PreprocessSuffix): ClickToolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ClickToolbar.cpp$(PreprocessSuffix) ClickToolbar.cpp

$(IntermediateDirectory)/BitmapPrintout.cpp$(ObjectSuffix): BitmapPrintout.cpp $(IntermediateDirectory)/BitmapPrintout.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libControl/BitmapPrintout.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BitmapPrintout.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BitmapPrintout.cpp$(DependSuffix): BitmapPrintout.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BitmapPrintout.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BitmapPrintout.cpp$(DependSuffix) -MM BitmapPrintout.cpp

$(IntermediateDirectory)/BitmapPrintout.cpp$(PreprocessSuffix): BitmapPrintout.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BitmapPrintout.cpp$(PreprocessSuffix) BitmapPrintout.cpp

$(IntermediateDirectory)/InfosSeparationBarEffect.cpp$(ObjectSuffix): InfosSeparationBarEffect.cpp $(IntermediateDirectory)/InfosSeparationBarEffect.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libControl/InfosSeparationBarEffect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InfosSeparationBarEffect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InfosSeparationBarEffect.cpp$(DependSuffix): InfosSeparationBarEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InfosSeparationBarEffect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InfosSeparationBarEffect.cpp$(DependSuffix) -MM InfosSeparationBarEffect.cpp

$(IntermediateDirectory)/InfosSeparationBarEffect.cpp$(PreprocessSuffix): InfosSeparationBarEffect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InfosSeparationBarEffect.cpp$(PreprocessSuffix) InfosSeparationBarEffect.cpp

$(IntermediateDirectory)/ThumbnailVideo.cpp$(ObjectSuffix): ThumbnailVideo.cpp $(IntermediateDirectory)/ThumbnailVideo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libControl/ThumbnailVideo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailVideo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailVideo.cpp$(DependSuffix): ThumbnailVideo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailVideo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailVideo.cpp$(DependSuffix) -MM ThumbnailVideo.cpp

$(IntermediateDirectory)/ThumbnailVideo.cpp$(PreprocessSuffix): ThumbnailVideo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailVideo.cpp$(PreprocessSuffix) ThumbnailVideo.cpp

$(IntermediateDirectory)/BitmapToolbar.cpp$(ObjectSuffix): BitmapToolbar.cpp $(IntermediateDirectory)/BitmapToolbar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libControl/BitmapToolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BitmapToolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BitmapToolbar.cpp$(DependSuffix): BitmapToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BitmapToolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BitmapToolbar.cpp$(DependSuffix) -MM BitmapToolbar.cpp

$(IntermediateDirectory)/BitmapToolbar.cpp$(PreprocessSuffix): BitmapToolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BitmapToolbar.cpp$(PreprocessSuffix) BitmapToolbar.cpp

$(IntermediateDirectory)/ShowBitmap.cpp$(ObjectSuffix): ShowBitmap.cpp $(IntermediateDirectory)/ShowBitmap.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libControl/ShowBitmap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ShowBitmap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ShowBitmap.cpp$(DependSuffix): ShowBitmap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ShowBitmap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ShowBitmap.cpp$(DependSuffix) -MM ShowBitmap.cpp

$(IntermediateDirectory)/ShowBitmap.cpp$(PreprocessSuffix): ShowBitmap.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ShowBitmap.cpp$(PreprocessSuffix) ShowBitmap.cpp

$(IntermediateDirectory)/InfosFile.cpp$(ObjectSuffix): InfosFile.cpp $(IntermediateDirectory)/InfosFile.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libControl/InfosFile.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InfosFile.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InfosFile.cpp$(DependSuffix): InfosFile.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InfosFile.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InfosFile.cpp$(DependSuffix) -MM InfosFile.cpp

$(IntermediateDirectory)/InfosFile.cpp$(PreprocessSuffix): InfosFile.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InfosFile.cpp$(PreprocessSuffix) InfosFile.cpp

$(IntermediateDirectory)/ThumbnailVertical.cpp$(ObjectSuffix): ThumbnailVertical.cpp $(IntermediateDirectory)/ThumbnailVertical.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libControl/ThumbnailVertical.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailVertical.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailVertical.cpp$(DependSuffix): ThumbnailVertical.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailVertical.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailVertical.cpp$(DependSuffix) -MM ThumbnailVertical.cpp

$(IntermediateDirectory)/ThumbnailVertical.cpp$(PreprocessSuffix): ThumbnailVertical.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailVertical.cpp$(PreprocessSuffix) ThumbnailVertical.cpp

$(IntermediateDirectory)/ThumbnailEffect.cpp$(ObjectSuffix): ThumbnailEffect.cpp $(IntermediateDirectory)/ThumbnailEffect.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libControl/ThumbnailEffect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailEffect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailEffect.cpp$(DependSuffix): ThumbnailEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailEffect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailEffect.cpp$(DependSuffix) -MM ThumbnailEffect.cpp

$(IntermediateDirectory)/ThumbnailEffect.cpp$(PreprocessSuffix): ThumbnailEffect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailEffect.cpp$(PreprocessSuffix) ThumbnailEffect.cpp

$(IntermediateDirectory)/CriteriaTree.cpp$(ObjectSuffix): CriteriaTree.cpp $(IntermediateDirectory)/CriteriaTree.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libControl/CriteriaTree.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CriteriaTree.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CriteriaTree.cpp$(DependSuffix): CriteriaTree.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CriteriaTree.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CriteriaTree.cpp$(DependSuffix) -MM CriteriaTree.cpp

$(IntermediateDirectory)/CriteriaTree.cpp$(PreprocessSuffix): CriteriaTree.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CriteriaTree.cpp$(PreprocessSuffix) CriteriaTree.cpp

$(IntermediateDirectory)/InfosFileWnd.cpp$(ObjectSuffix): InfosFileWnd.cpp $(IntermediateDirectory)/InfosFileWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libControl/InfosFileWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InfosFileWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InfosFileWnd.cpp$(DependSuffix): InfosFileWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InfosFileWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InfosFileWnd.cpp$(DependSuffix) -MM InfosFileWnd.cpp

$(IntermediateDirectory)/InfosFileWnd.cpp$(PreprocessSuffix): InfosFileWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InfosFileWnd.cpp$(PreprocessSuffix) InfosFileWnd.cpp

$(IntermediateDirectory)/SplitterWithPanel.cpp$(ObjectSuffix): SplitterWithPanel.cpp $(IntermediateDirectory)/SplitterWithPanel.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libControl/SplitterWithPanel.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SplitterWithPanel.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SplitterWithPanel.cpp$(DependSuffix): SplitterWithPanel.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SplitterWithPanel.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SplitterWithPanel.cpp$(DependSuffix) -MM SplitterWithPanel.cpp

$(IntermediateDirectory)/SplitterWithPanel.cpp$(PreprocessSuffix): SplitterWithPanel.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SplitterWithPanel.cpp$(PreprocessSuffix) SplitterWithPanel.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/
	$(RM) ../include/Debug/header.h.gch


