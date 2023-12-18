##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libViewer
ConfigurationName      :=Release
WorkspaceConfiguration :=Release
WorkspacePath          :=/home/figuinha/developpement/git/Regards
ProjectPath            :=/home/figuinha/developpement/git/Regards/libViewer
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=18/12/23
CodeLitePath           :=/home/figuinha/.codelite
LinkerName             :=/home/figuinha/intel/oneapi/compiler/2023.2.0/linux/bin/icpx
SharedObjectLinkerName :=/home/figuinha/intel/oneapi/compiler/2023.2.0/linux/bin/icpx -shared -fPIC
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
OutputDirectory        :=$(IntermediateDirectory)
OutputFile             :=$(IntermediateDirectory)/$(ProjectName).a
Preprocessors          :=$(PreprocessorSwitch)VIEWER $(PreprocessorSwitch)OPENGL $(PreprocessorSwitch)WEBVIEWER $(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libViewer.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s 
IncludePath            := $(IncludeSwitch)$${HOME}/developpement/git/Regards/libextern/oneapi-tbb-2021.7.0/include $(IncludeSwitch)$${HOME}/developpement/git/Regards/libextern/vcpkg-2022.08.15/installed/x64-linux/include $(IncludeSwitch)$${HOME}/developpement/git/Regards/libextern/vcpkg/installed/x64-linux/include/Imath $(IncludeSwitch)$${HOME}/developpement/git/Regards/libextern/vcpkg/installed/x64-linux/include/libde265 $(IncludeSwitch)$${HOME}/developpement/git/Regards/libextern/vcpkg/installed/x64-linux/include/poppler $(IncludeSwitch)$${HOME}/developpement/git/Regards/libextern/vcpkg/installed/x64-linux/include/poppler/cpp $(IncludeSwitch)$${HOME}/developpement/git/Regards/libextern/vcpkg/installed/x64-linux/include $(IncludeSwitch)$${HOME}/developpement/git/Regards/libextern/vcpkg/buildtrees/ffmpeg/src/n6.1-f7bac3f35b.clean $(IncludeSwitch)$${HOME}/ffmpeg_build/include $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/glib-2.0/include $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include $(IncludeSwitch)/usr/include/harfbuzz  $(IncludeSwitch). $(IncludeSwitch)../include $(IncludeSwitch)../libVideoThumbnail $(IncludeSwitch)../libControl $(IncludeSwitch)../libextern/CvPlot/inc $(IncludeSwitch)../libDeepLearning $(IncludeSwitch)../libDialog $(IncludeSwitch)../libPicture $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libFiltre $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libIntro $(IncludeSwitch)../libOpenCL $(IncludeSwitch)../libOpenCLEngine $(IncludeSwitch)../libOpenGL $(IncludeSwitch)../libParameter $(IncludeSwitch)../libResource $(IncludeSwitch)../libScanner $(IncludeSwitch)../libSqlData $(IncludeSwitch)../libTheme $(IncludeSwitch)../libUtility $(IncludeSwitch)../libWindow $(IncludeSwitch)../libWindowMain $(IncludeSwitch)../Otherlib/wxIA/include $(IncludeSwitch)../Otherlib/wxIA/WIA $(IncludeSwitch)../Otherlib/wxSVG/include $(IncludeSwitch)../wxCheckTree $(IncludeSwitch)../libextern/libOpenCL/include $(IncludeSwitch)../libextern/glew-2.1.0/include $(IncludeSwitch)../libextern/ffmpeg-master $(IncludeSwitch)../libextern/ffmpeg-master/include $(IncludeSwitch)../libAppleFunction $(IncludeSwitch)../Otherlib/libffmpeg 
IncludePCH             :=  -include ../include/Release/header.h 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                :=$(LibraryPathSwitch)$${HOME}/developpement/git/Regards/libextern/oneapi-tbb-2021.7.0/lib/intel64/gcc4.8 $(LibraryPathSwitch)$${HOME}/developpement/git/Regards/libextern/vcpkg-2022.08.15/installed/x64-linux/lib $(LibraryPathSwitch)$${HOME}/developpement/git/Regards/libextern/vcpkg/installed/x64-linux/lib $(LibraryPathSwitch)$${HOME}/developpement/git/Regards/libextern/vcpkg/installed/x64-linux/lib/manual-link/opencv4_thirdparty $(LibraryPathSwitch)$${HOME}/ffmpeg_build/lib $(LibraryPathSwitch)/usr/lib/x86_64-linux-gnu  $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overridden using an environment variable
##
AR       := /usr/bin/ar rcu
CXX      := /home/figuinha/intel/oneapi/compiler/2023.2.0/linux/bin/icpx
CC       := /home/figuinha/intel/oneapi/compiler/2023.2.0/linux/bin/icx
CXXFLAGS := $(shell ../libextern/wxconfig_cpp.sh) -Wall -pthread -std=gnu++14 -Wall -O3 $(Preprocessors)
CFLAGS   :=  -Wall -O3 $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/FolderProcess.cpp$(ObjectSuffix) $(IntermediateDirectory)/ToolbarInfos.cpp$(ObjectSuffix) $(IntermediateDirectory)/ToolbarPhoto.cpp$(ObjectSuffix) $(IntermediateDirectory)/ExportDiaporama.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailToolBar.cpp$(ObjectSuffix) $(IntermediateDirectory)/ListPicture.cpp$(ObjectSuffix) $(IntermediateDirectory)/CategoryWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/md5check.cpp$(ObjectSuffix) $(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/toolbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/CentralWindow.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailFacePertinenceToolBar.cpp$(ObjectSuffix) $(IntermediateDirectory)/WaitingWindow.cpp$(ObjectSuffix) $(IntermediateDirectory)/PicturePanel.cpp$(ObjectSuffix) $(IntermediateDirectory)/ListFace.cpp$(ObjectSuffix) $(IntermediateDirectory)/KeywordManagementWindow.cpp$(ObjectSuffix) $(IntermediateDirectory)/ImageList.cpp$(ObjectSuffix) $(IntermediateDirectory)/ToolbarKeyword.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailViewerPicture.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/MainWindow.cpp$(ObjectSuffix) $(IntermediateDirectory)/KeywordWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/InfosSeparationBarFace.cpp$(ObjectSuffix) $(IntermediateDirectory)/PreviewWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/PreviewToolbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(ObjectSuffix) $(IntermediateDirectory)/KeywordWndToolbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/ViewerFrame.cpp$(ObjectSuffix) $(IntermediateDirectory)/ToolbarViewerMode.cpp$(ObjectSuffix) $(IntermediateDirectory)/CategoryFolderWindow.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/ThumbnailFaceToolBar.cpp$(ObjectSuffix) $(IntermediateDirectory)/AnimationToolbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/PanelInfosWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/PanelPhotoWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/CriteriaWindow.cpp$(ObjectSuffix) $(IntermediateDirectory)/CheckVersion.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailFace.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailFolder.cpp$(ObjectSuffix) 



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
	$(AR) $(ArchiveOutputSwitch)$(OutputFile) $(Objects)
	@$(MakeDirCommand) "/home/figuinha/developpement/git/Regards/.build-release"
	@echo rebuilt > "/home/figuinha/developpement/git/Regards/.build-release/libViewer"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


./Release:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:

# PreCompiled Header
../include/Release/header.h.gch: ../include/Release/header.h
	$(CXX) $(SourceSwitch) ../include/Release/header.h $(PCHCompileFlags) $(CXXFLAGS) $(IncludePath)



##
## Objects
##
$(IntermediateDirectory)/FolderProcess.cpp$(ObjectSuffix): FolderProcess.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FolderProcess.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FolderProcess.cpp$(DependSuffix) -MM FolderProcess.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/FolderProcess.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FolderProcess.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FolderProcess.cpp$(PreprocessSuffix): FolderProcess.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FolderProcess.cpp$(PreprocessSuffix) FolderProcess.cpp

$(IntermediateDirectory)/ToolbarInfos.cpp$(ObjectSuffix): ToolbarInfos.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ToolbarInfos.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ToolbarInfos.cpp$(DependSuffix) -MM ToolbarInfos.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/ToolbarInfos.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ToolbarInfos.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ToolbarInfos.cpp$(PreprocessSuffix): ToolbarInfos.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ToolbarInfos.cpp$(PreprocessSuffix) ToolbarInfos.cpp

$(IntermediateDirectory)/ToolbarPhoto.cpp$(ObjectSuffix): ToolbarPhoto.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ToolbarPhoto.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ToolbarPhoto.cpp$(DependSuffix) -MM ToolbarPhoto.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/ToolbarPhoto.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ToolbarPhoto.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ToolbarPhoto.cpp$(PreprocessSuffix): ToolbarPhoto.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ToolbarPhoto.cpp$(PreprocessSuffix) ToolbarPhoto.cpp

$(IntermediateDirectory)/ExportDiaporama.cpp$(ObjectSuffix): ExportDiaporama.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ExportDiaporama.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ExportDiaporama.cpp$(DependSuffix) -MM ExportDiaporama.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/ExportDiaporama.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ExportDiaporama.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ExportDiaporama.cpp$(PreprocessSuffix): ExportDiaporama.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ExportDiaporama.cpp$(PreprocessSuffix) ExportDiaporama.cpp

$(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(ObjectSuffix): ThumbnailViewerVideo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(DependSuffix) -MM ThumbnailViewerVideo.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/ThumbnailViewerVideo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(PreprocessSuffix): ThumbnailViewerVideo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(PreprocessSuffix) ThumbnailViewerVideo.cpp

$(IntermediateDirectory)/ThumbnailToolBar.cpp$(ObjectSuffix): ThumbnailToolBar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailToolBar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailToolBar.cpp$(DependSuffix) -MM ThumbnailToolBar.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/ThumbnailToolBar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailToolBar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailToolBar.cpp$(PreprocessSuffix): ThumbnailToolBar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailToolBar.cpp$(PreprocessSuffix) ThumbnailToolBar.cpp

$(IntermediateDirectory)/ListPicture.cpp$(ObjectSuffix): ListPicture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ListPicture.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ListPicture.cpp$(DependSuffix) -MM ListPicture.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/ListPicture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ListPicture.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ListPicture.cpp$(PreprocessSuffix): ListPicture.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ListPicture.cpp$(PreprocessSuffix) ListPicture.cpp

$(IntermediateDirectory)/CategoryWnd.cpp$(ObjectSuffix): CategoryWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CategoryWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CategoryWnd.cpp$(DependSuffix) -MM CategoryWnd.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/CategoryWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CategoryWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CategoryWnd.cpp$(PreprocessSuffix): CategoryWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CategoryWnd.cpp$(PreprocessSuffix) CategoryWnd.cpp

$(IntermediateDirectory)/md5check.cpp$(ObjectSuffix): md5check.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/md5check.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/md5check.cpp$(DependSuffix) -MM md5check.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/md5check.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/md5check.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/md5check.cpp$(PreprocessSuffix): md5check.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/md5check.cpp$(PreprocessSuffix) md5check.cpp

$(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(ObjectSuffix): InfosSeparationBarExplorer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(DependSuffix) -MM InfosSeparationBarExplorer.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/InfosSeparationBarExplorer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(PreprocessSuffix): InfosSeparationBarExplorer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(PreprocessSuffix) InfosSeparationBarExplorer.cpp

$(IntermediateDirectory)/toolbar.cpp$(ObjectSuffix): toolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/toolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/toolbar.cpp$(DependSuffix) -MM toolbar.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/toolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/toolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/toolbar.cpp$(PreprocessSuffix): toolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/toolbar.cpp$(PreprocessSuffix) toolbar.cpp

$(IntermediateDirectory)/CentralWindow.cpp$(ObjectSuffix): CentralWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CentralWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CentralWindow.cpp$(DependSuffix) -MM CentralWindow.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/CentralWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CentralWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CentralWindow.cpp$(PreprocessSuffix): CentralWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CentralWindow.cpp$(PreprocessSuffix) CentralWindow.cpp

$(IntermediateDirectory)/ThumbnailFacePertinenceToolBar.cpp$(ObjectSuffix): ThumbnailFacePertinenceToolBar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailFacePertinenceToolBar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailFacePertinenceToolBar.cpp$(DependSuffix) -MM ThumbnailFacePertinenceToolBar.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/ThumbnailFacePertinenceToolBar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailFacePertinenceToolBar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailFacePertinenceToolBar.cpp$(PreprocessSuffix): ThumbnailFacePertinenceToolBar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailFacePertinenceToolBar.cpp$(PreprocessSuffix) ThumbnailFacePertinenceToolBar.cpp

$(IntermediateDirectory)/WaitingWindow.cpp$(ObjectSuffix): WaitingWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/WaitingWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/WaitingWindow.cpp$(DependSuffix) -MM WaitingWindow.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/WaitingWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/WaitingWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/WaitingWindow.cpp$(PreprocessSuffix): WaitingWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/WaitingWindow.cpp$(PreprocessSuffix) WaitingWindow.cpp

$(IntermediateDirectory)/PicturePanel.cpp$(ObjectSuffix): PicturePanel.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PicturePanel.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PicturePanel.cpp$(DependSuffix) -MM PicturePanel.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/PicturePanel.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PicturePanel.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PicturePanel.cpp$(PreprocessSuffix): PicturePanel.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PicturePanel.cpp$(PreprocessSuffix) PicturePanel.cpp

$(IntermediateDirectory)/ListFace.cpp$(ObjectSuffix): ListFace.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ListFace.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ListFace.cpp$(DependSuffix) -MM ListFace.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/ListFace.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ListFace.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ListFace.cpp$(PreprocessSuffix): ListFace.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ListFace.cpp$(PreprocessSuffix) ListFace.cpp

$(IntermediateDirectory)/KeywordManagementWindow.cpp$(ObjectSuffix): KeywordManagementWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/KeywordManagementWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/KeywordManagementWindow.cpp$(DependSuffix) -MM KeywordManagementWindow.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/KeywordManagementWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/KeywordManagementWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/KeywordManagementWindow.cpp$(PreprocessSuffix): KeywordManagementWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/KeywordManagementWindow.cpp$(PreprocessSuffix) KeywordManagementWindow.cpp

$(IntermediateDirectory)/ImageList.cpp$(ObjectSuffix): ImageList.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ImageList.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ImageList.cpp$(DependSuffix) -MM ImageList.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/ImageList.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ImageList.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ImageList.cpp$(PreprocessSuffix): ImageList.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ImageList.cpp$(PreprocessSuffix) ImageList.cpp

$(IntermediateDirectory)/ToolbarKeyword.cpp$(ObjectSuffix): ToolbarKeyword.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ToolbarKeyword.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ToolbarKeyword.cpp$(DependSuffix) -MM ToolbarKeyword.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/ToolbarKeyword.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ToolbarKeyword.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ToolbarKeyword.cpp$(PreprocessSuffix): ToolbarKeyword.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ToolbarKeyword.cpp$(PreprocessSuffix) ToolbarKeyword.cpp

$(IntermediateDirectory)/ThumbnailViewerPicture.cpp$(ObjectSuffix): ThumbnailViewerPicture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailViewerPicture.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailViewerPicture.cpp$(DependSuffix) -MM ThumbnailViewerPicture.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/ThumbnailViewerPicture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailViewerPicture.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailViewerPicture.cpp$(PreprocessSuffix): ThumbnailViewerPicture.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailViewerPicture.cpp$(PreprocessSuffix) ThumbnailViewerPicture.cpp

$(IntermediateDirectory)/MainWindow.cpp$(ObjectSuffix): MainWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MainWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MainWindow.cpp$(DependSuffix) -MM MainWindow.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/MainWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MainWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MainWindow.cpp$(PreprocessSuffix): MainWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MainWindow.cpp$(PreprocessSuffix) MainWindow.cpp

$(IntermediateDirectory)/KeywordWnd.cpp$(ObjectSuffix): KeywordWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/KeywordWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/KeywordWnd.cpp$(DependSuffix) -MM KeywordWnd.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/KeywordWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/KeywordWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/KeywordWnd.cpp$(PreprocessSuffix): KeywordWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/KeywordWnd.cpp$(PreprocessSuffix) KeywordWnd.cpp

$(IntermediateDirectory)/InfosSeparationBarFace.cpp$(ObjectSuffix): InfosSeparationBarFace.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InfosSeparationBarFace.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InfosSeparationBarFace.cpp$(DependSuffix) -MM InfosSeparationBarFace.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/InfosSeparationBarFace.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InfosSeparationBarFace.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InfosSeparationBarFace.cpp$(PreprocessSuffix): InfosSeparationBarFace.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InfosSeparationBarFace.cpp$(PreprocessSuffix) InfosSeparationBarFace.cpp

$(IntermediateDirectory)/PreviewWnd.cpp$(ObjectSuffix): PreviewWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PreviewWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PreviewWnd.cpp$(DependSuffix) -MM PreviewWnd.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/PreviewWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PreviewWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PreviewWnd.cpp$(PreprocessSuffix): PreviewWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PreviewWnd.cpp$(PreprocessSuffix) PreviewWnd.cpp

$(IntermediateDirectory)/PreviewToolbar.cpp$(ObjectSuffix): PreviewToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PreviewToolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PreviewToolbar.cpp$(DependSuffix) -MM PreviewToolbar.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/PreviewToolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PreviewToolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PreviewToolbar.cpp$(PreprocessSuffix): PreviewToolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PreviewToolbar.cpp$(PreprocessSuffix) PreviewToolbar.cpp

$(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(ObjectSuffix): ThumbnailToolBarZoom.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(DependSuffix) -MM ThumbnailToolBarZoom.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/ThumbnailToolBarZoom.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(PreprocessSuffix): ThumbnailToolBarZoom.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(PreprocessSuffix) ThumbnailToolBarZoom.cpp

$(IntermediateDirectory)/KeywordWndToolbar.cpp$(ObjectSuffix): KeywordWndToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/KeywordWndToolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/KeywordWndToolbar.cpp$(DependSuffix) -MM KeywordWndToolbar.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/KeywordWndToolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/KeywordWndToolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/KeywordWndToolbar.cpp$(PreprocessSuffix): KeywordWndToolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/KeywordWndToolbar.cpp$(PreprocessSuffix) KeywordWndToolbar.cpp

$(IntermediateDirectory)/ViewerFrame.cpp$(ObjectSuffix): ViewerFrame.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ViewerFrame.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ViewerFrame.cpp$(DependSuffix) -MM ViewerFrame.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/ViewerFrame.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ViewerFrame.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ViewerFrame.cpp$(PreprocessSuffix): ViewerFrame.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ViewerFrame.cpp$(PreprocessSuffix) ViewerFrame.cpp

$(IntermediateDirectory)/ToolbarViewerMode.cpp$(ObjectSuffix): ToolbarViewerMode.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ToolbarViewerMode.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ToolbarViewerMode.cpp$(DependSuffix) -MM ToolbarViewerMode.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/ToolbarViewerMode.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ToolbarViewerMode.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ToolbarViewerMode.cpp$(PreprocessSuffix): ToolbarViewerMode.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ToolbarViewerMode.cpp$(PreprocessSuffix) ToolbarViewerMode.cpp

$(IntermediateDirectory)/CategoryFolderWindow.cpp$(ObjectSuffix): CategoryFolderWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CategoryFolderWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CategoryFolderWindow.cpp$(DependSuffix) -MM CategoryFolderWindow.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/CategoryFolderWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CategoryFolderWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CategoryFolderWindow.cpp$(PreprocessSuffix): CategoryFolderWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CategoryFolderWindow.cpp$(PreprocessSuffix) CategoryFolderWindow.cpp

$(IntermediateDirectory)/ThumbnailFaceToolBar.cpp$(ObjectSuffix): ThumbnailFaceToolBar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailFaceToolBar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailFaceToolBar.cpp$(DependSuffix) -MM ThumbnailFaceToolBar.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/ThumbnailFaceToolBar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailFaceToolBar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailFaceToolBar.cpp$(PreprocessSuffix): ThumbnailFaceToolBar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailFaceToolBar.cpp$(PreprocessSuffix) ThumbnailFaceToolBar.cpp

$(IntermediateDirectory)/AnimationToolbar.cpp$(ObjectSuffix): AnimationToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/AnimationToolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/AnimationToolbar.cpp$(DependSuffix) -MM AnimationToolbar.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/AnimationToolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/AnimationToolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/AnimationToolbar.cpp$(PreprocessSuffix): AnimationToolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/AnimationToolbar.cpp$(PreprocessSuffix) AnimationToolbar.cpp

$(IntermediateDirectory)/PanelInfosWnd.cpp$(ObjectSuffix): PanelInfosWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PanelInfosWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PanelInfosWnd.cpp$(DependSuffix) -MM PanelInfosWnd.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/PanelInfosWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PanelInfosWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PanelInfosWnd.cpp$(PreprocessSuffix): PanelInfosWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PanelInfosWnd.cpp$(PreprocessSuffix) PanelInfosWnd.cpp

$(IntermediateDirectory)/PanelPhotoWnd.cpp$(ObjectSuffix): PanelPhotoWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PanelPhotoWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PanelPhotoWnd.cpp$(DependSuffix) -MM PanelPhotoWnd.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/PanelPhotoWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PanelPhotoWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PanelPhotoWnd.cpp$(PreprocessSuffix): PanelPhotoWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PanelPhotoWnd.cpp$(PreprocessSuffix) PanelPhotoWnd.cpp

$(IntermediateDirectory)/CriteriaWindow.cpp$(ObjectSuffix): CriteriaWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CriteriaWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CriteriaWindow.cpp$(DependSuffix) -MM CriteriaWindow.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/CriteriaWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CriteriaWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CriteriaWindow.cpp$(PreprocessSuffix): CriteriaWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CriteriaWindow.cpp$(PreprocessSuffix) CriteriaWindow.cpp

$(IntermediateDirectory)/CheckVersion.cpp$(ObjectSuffix): CheckVersion.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CheckVersion.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CheckVersion.cpp$(DependSuffix) -MM CheckVersion.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/CheckVersion.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CheckVersion.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CheckVersion.cpp$(PreprocessSuffix): CheckVersion.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CheckVersion.cpp$(PreprocessSuffix) CheckVersion.cpp

$(IntermediateDirectory)/ThumbnailFace.cpp$(ObjectSuffix): ThumbnailFace.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailFace.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailFace.cpp$(DependSuffix) -MM ThumbnailFace.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/ThumbnailFace.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailFace.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailFace.cpp$(PreprocessSuffix): ThumbnailFace.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailFace.cpp$(PreprocessSuffix) ThumbnailFace.cpp

$(IntermediateDirectory)/ThumbnailFolder.cpp$(ObjectSuffix): ThumbnailFolder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailFolder.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailFolder.cpp$(DependSuffix) -MM ThumbnailFolder.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/git/Regards/libViewer/ThumbnailFolder.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailFolder.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailFolder.cpp$(PreprocessSuffix): ThumbnailFolder.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailFolder.cpp$(PreprocessSuffix) ThumbnailFolder.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/
	$(RM) ../include/Release/header.h.gch


