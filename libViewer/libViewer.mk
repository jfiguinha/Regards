##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libViewer
ConfigurationName      :=Release
WorkspacePath          :=C:/developpement/git/Regards
ProjectPath            :=C:/developpement/git/Regards/libViewer
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=jfigu
Date                   :=19/05/2019
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
Preprocessors          :=$(PreprocessorSwitch)VIEWER $(PreprocessorSwitch)FFMPEG $(PreprocessorSwitch)OPENGL $(PreprocessorSwitch)WEBVIEWER $(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libViewer.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/msys64/mingw64/bin/windres.exe
LinkOptions            :=  -s 
IncludePath            := $(IncludeSwitch)"C:\Program Files (x86)\AMD APP SDK\3.0_mingw\include" $(IncludeSwitch)/mingw64/include/libxml2 $(IncludeSwitch)/mingw64/include/OpenEXR  $(IncludeSwitch)../wxCheckTree $(IncludeSwitch)../Otherlib/RegardsRaw $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libTheme $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libParameter $(IncludeSwitch)../libUtility $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libResource $(IncludeSwitch)../libSqlData $(IncludeSwitch)../libRenderBitmap $(IncludeSwitch)../libSqlEngine $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libFiltre $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../Otherlib/wxSVG/include $(IncludeSwitch)../libWindow $(IncludeSwitch)../libPrint $(IncludeSwitch)../libPicture $(IncludeSwitch)../libTheme $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libControl $(IncludeSwitch)../libIntro $(IncludeSwitch)../libDialog $(IncludeSwitch)../libWindowMain $(IncludeSwitch)../libVideoFFMpeg $(IncludeSwitch)../libOpenCL $(IncludeSwitch)../libOpenGL $(IncludeSwitch)../libVideoThumbnail $(IncludeSwitch)../FaceRecognition $(IncludeSwitch)../libCompress $(IncludeSwitch)../libAppleFunction $(IncludeSwitch)../libextern/glew-2.1.0/include $(IncludeSwitch)../libextern/libjpeg-turbo-1.5.1 
IncludePCH             :=  -include ../include/Release/header.h 
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
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread  -fopenmp -std=gnu++11 -Wall -O2 -msse4.1 $(Preprocessors)
CFLAGS   :=  -Wall -O2 -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := C:/msys64/usr/bin/ccache.exe C:/msys64/mingw64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
PATH:=C:\msys64\usr\bin;$PATH
WXWIN:=C:\Regards\libextern\wxwidgets-3.1.2
WXCFG:=lib/mswud
Objects0=$(IntermediateDirectory)/ToolbarInfos.cpp$(ObjectSuffix) $(IntermediateDirectory)/PreviewToolbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(ObjectSuffix) $(IntermediateDirectory)/ListFace.cpp$(ObjectSuffix) $(IntermediateDirectory)/InfosSeparationBarFace.cpp$(ObjectSuffix) $(IntermediateDirectory)/CentralWindow.cpp$(ObjectSuffix) $(IntermediateDirectory)/ViewerWindow.cpp$(ObjectSuffix) $(IntermediateDirectory)/ModificationManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/ViewerParam.cpp$(ObjectSuffix) $(IntermediateDirectory)/InfoEffect.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/MainWindow.cpp$(ObjectSuffix) $(IntermediateDirectory)/ViewerFrame.cpp$(ObjectSuffix) $(IntermediateDirectory)/ViewerThemeInit.cpp$(ObjectSuffix) $(IntermediateDirectory)/ImageList.cpp$(ObjectSuffix) $(IntermediateDirectory)/AnimationToolbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/InfoEffectWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/PreviewInfosWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/PreviewWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/ToolbarPhoto.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/toolbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/CategoryWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/ListPicture.cpp$(ObjectSuffix) $(IntermediateDirectory)/CategoryFolderWindow.cpp$(ObjectSuffix) $(IntermediateDirectory)/ViewerTheme.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(ObjectSuffix) $(IntermediateDirectory)/PanelPhotoWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailViewerPicture.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailFacePertinenceToolBar.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/ThumbnailFace.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailFaceToolBar.cpp$(ObjectSuffix) $(IntermediateDirectory)/FiltreEffect.cpp$(ObjectSuffix) $(IntermediateDirectory)/PanelInfosWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/FiltreEffectWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailFolder.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailToolBar.cpp$(ObjectSuffix) $(IntermediateDirectory)/FiltreToolbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/ViewerParamInit.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(ObjectSuffix) \
	



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
	@$(MakeDirCommand) "C:\developpement\git\Regards/.build-release"
	@echo rebuilt > "C:\developpement\git\Regards/.build-release/libViewer"

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Release"


./Release:
	@$(MakeDirCommand) "./Release"

PreBuild:

# PreCompiled Header
../include/Release/header.h.gch: ../include/Release/header.h
	$(CXX) $(SourceSwitch) ../include/Release/header.h $(PCHCompileFlags) $(CXXFLAGS) $(IncludePath)



##
## Objects
##
$(IntermediateDirectory)/ToolbarInfos.cpp$(ObjectSuffix): ToolbarInfos.cpp $(IntermediateDirectory)/ToolbarInfos.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/ToolbarInfos.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ToolbarInfos.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ToolbarInfos.cpp$(DependSuffix): ToolbarInfos.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ToolbarInfos.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ToolbarInfos.cpp$(DependSuffix) -MM ToolbarInfos.cpp

$(IntermediateDirectory)/ToolbarInfos.cpp$(PreprocessSuffix): ToolbarInfos.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ToolbarInfos.cpp$(PreprocessSuffix) ToolbarInfos.cpp

$(IntermediateDirectory)/PreviewToolbar.cpp$(ObjectSuffix): PreviewToolbar.cpp $(IntermediateDirectory)/PreviewToolbar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/PreviewToolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PreviewToolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PreviewToolbar.cpp$(DependSuffix): PreviewToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PreviewToolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PreviewToolbar.cpp$(DependSuffix) -MM PreviewToolbar.cpp

$(IntermediateDirectory)/PreviewToolbar.cpp$(PreprocessSuffix): PreviewToolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PreviewToolbar.cpp$(PreprocessSuffix) PreviewToolbar.cpp

$(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(ObjectSuffix): InfosSeparationBarExplorer.cpp $(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/InfosSeparationBarExplorer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(DependSuffix): InfosSeparationBarExplorer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(DependSuffix) -MM InfosSeparationBarExplorer.cpp

$(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(PreprocessSuffix): InfosSeparationBarExplorer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InfosSeparationBarExplorer.cpp$(PreprocessSuffix) InfosSeparationBarExplorer.cpp

$(IntermediateDirectory)/ListFace.cpp$(ObjectSuffix): ListFace.cpp $(IntermediateDirectory)/ListFace.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/ListFace.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ListFace.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ListFace.cpp$(DependSuffix): ListFace.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ListFace.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ListFace.cpp$(DependSuffix) -MM ListFace.cpp

$(IntermediateDirectory)/ListFace.cpp$(PreprocessSuffix): ListFace.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ListFace.cpp$(PreprocessSuffix) ListFace.cpp

$(IntermediateDirectory)/InfosSeparationBarFace.cpp$(ObjectSuffix): InfosSeparationBarFace.cpp $(IntermediateDirectory)/InfosSeparationBarFace.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/InfosSeparationBarFace.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InfosSeparationBarFace.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InfosSeparationBarFace.cpp$(DependSuffix): InfosSeparationBarFace.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InfosSeparationBarFace.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InfosSeparationBarFace.cpp$(DependSuffix) -MM InfosSeparationBarFace.cpp

$(IntermediateDirectory)/InfosSeparationBarFace.cpp$(PreprocessSuffix): InfosSeparationBarFace.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InfosSeparationBarFace.cpp$(PreprocessSuffix) InfosSeparationBarFace.cpp

$(IntermediateDirectory)/CentralWindow.cpp$(ObjectSuffix): CentralWindow.cpp $(IntermediateDirectory)/CentralWindow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/CentralWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CentralWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CentralWindow.cpp$(DependSuffix): CentralWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CentralWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CentralWindow.cpp$(DependSuffix) -MM CentralWindow.cpp

$(IntermediateDirectory)/CentralWindow.cpp$(PreprocessSuffix): CentralWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CentralWindow.cpp$(PreprocessSuffix) CentralWindow.cpp

$(IntermediateDirectory)/ViewerWindow.cpp$(ObjectSuffix): ViewerWindow.cpp $(IntermediateDirectory)/ViewerWindow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/ViewerWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ViewerWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ViewerWindow.cpp$(DependSuffix): ViewerWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ViewerWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ViewerWindow.cpp$(DependSuffix) -MM ViewerWindow.cpp

$(IntermediateDirectory)/ViewerWindow.cpp$(PreprocessSuffix): ViewerWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ViewerWindow.cpp$(PreprocessSuffix) ViewerWindow.cpp

$(IntermediateDirectory)/ModificationManager.cpp$(ObjectSuffix): ModificationManager.cpp $(IntermediateDirectory)/ModificationManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/ModificationManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ModificationManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ModificationManager.cpp$(DependSuffix): ModificationManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ModificationManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ModificationManager.cpp$(DependSuffix) -MM ModificationManager.cpp

$(IntermediateDirectory)/ModificationManager.cpp$(PreprocessSuffix): ModificationManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ModificationManager.cpp$(PreprocessSuffix) ModificationManager.cpp

$(IntermediateDirectory)/ViewerParam.cpp$(ObjectSuffix): ViewerParam.cpp $(IntermediateDirectory)/ViewerParam.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/ViewerParam.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ViewerParam.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ViewerParam.cpp$(DependSuffix): ViewerParam.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ViewerParam.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ViewerParam.cpp$(DependSuffix) -MM ViewerParam.cpp

$(IntermediateDirectory)/ViewerParam.cpp$(PreprocessSuffix): ViewerParam.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ViewerParam.cpp$(PreprocessSuffix) ViewerParam.cpp

$(IntermediateDirectory)/InfoEffect.cpp$(ObjectSuffix): InfoEffect.cpp $(IntermediateDirectory)/InfoEffect.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/InfoEffect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InfoEffect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InfoEffect.cpp$(DependSuffix): InfoEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InfoEffect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InfoEffect.cpp$(DependSuffix) -MM InfoEffect.cpp

$(IntermediateDirectory)/InfoEffect.cpp$(PreprocessSuffix): InfoEffect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InfoEffect.cpp$(PreprocessSuffix) InfoEffect.cpp

$(IntermediateDirectory)/MainWindow.cpp$(ObjectSuffix): MainWindow.cpp $(IntermediateDirectory)/MainWindow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/MainWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MainWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MainWindow.cpp$(DependSuffix): MainWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MainWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MainWindow.cpp$(DependSuffix) -MM MainWindow.cpp

$(IntermediateDirectory)/MainWindow.cpp$(PreprocessSuffix): MainWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MainWindow.cpp$(PreprocessSuffix) MainWindow.cpp

$(IntermediateDirectory)/ViewerFrame.cpp$(ObjectSuffix): ViewerFrame.cpp $(IntermediateDirectory)/ViewerFrame.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/ViewerFrame.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ViewerFrame.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ViewerFrame.cpp$(DependSuffix): ViewerFrame.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ViewerFrame.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ViewerFrame.cpp$(DependSuffix) -MM ViewerFrame.cpp

$(IntermediateDirectory)/ViewerFrame.cpp$(PreprocessSuffix): ViewerFrame.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ViewerFrame.cpp$(PreprocessSuffix) ViewerFrame.cpp

$(IntermediateDirectory)/ViewerThemeInit.cpp$(ObjectSuffix): ViewerThemeInit.cpp $(IntermediateDirectory)/ViewerThemeInit.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/ViewerThemeInit.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ViewerThemeInit.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ViewerThemeInit.cpp$(DependSuffix): ViewerThemeInit.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ViewerThemeInit.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ViewerThemeInit.cpp$(DependSuffix) -MM ViewerThemeInit.cpp

$(IntermediateDirectory)/ViewerThemeInit.cpp$(PreprocessSuffix): ViewerThemeInit.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ViewerThemeInit.cpp$(PreprocessSuffix) ViewerThemeInit.cpp

$(IntermediateDirectory)/ImageList.cpp$(ObjectSuffix): ImageList.cpp $(IntermediateDirectory)/ImageList.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/ImageList.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ImageList.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ImageList.cpp$(DependSuffix): ImageList.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ImageList.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ImageList.cpp$(DependSuffix) -MM ImageList.cpp

$(IntermediateDirectory)/ImageList.cpp$(PreprocessSuffix): ImageList.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ImageList.cpp$(PreprocessSuffix) ImageList.cpp

$(IntermediateDirectory)/AnimationToolbar.cpp$(ObjectSuffix): AnimationToolbar.cpp $(IntermediateDirectory)/AnimationToolbar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/AnimationToolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/AnimationToolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/AnimationToolbar.cpp$(DependSuffix): AnimationToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/AnimationToolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/AnimationToolbar.cpp$(DependSuffix) -MM AnimationToolbar.cpp

$(IntermediateDirectory)/AnimationToolbar.cpp$(PreprocessSuffix): AnimationToolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/AnimationToolbar.cpp$(PreprocessSuffix) AnimationToolbar.cpp

$(IntermediateDirectory)/InfoEffectWnd.cpp$(ObjectSuffix): InfoEffectWnd.cpp $(IntermediateDirectory)/InfoEffectWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/InfoEffectWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InfoEffectWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InfoEffectWnd.cpp$(DependSuffix): InfoEffectWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InfoEffectWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InfoEffectWnd.cpp$(DependSuffix) -MM InfoEffectWnd.cpp

$(IntermediateDirectory)/InfoEffectWnd.cpp$(PreprocessSuffix): InfoEffectWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InfoEffectWnd.cpp$(PreprocessSuffix) InfoEffectWnd.cpp

$(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(ObjectSuffix): ThumbnailViewerEffectWnd.cpp $(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/ThumbnailViewerEffectWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(DependSuffix): ThumbnailViewerEffectWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(DependSuffix) -MM ThumbnailViewerEffectWnd.cpp

$(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(PreprocessSuffix): ThumbnailViewerEffectWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailViewerEffectWnd.cpp$(PreprocessSuffix) ThumbnailViewerEffectWnd.cpp

$(IntermediateDirectory)/PreviewInfosWnd.cpp$(ObjectSuffix): PreviewInfosWnd.cpp $(IntermediateDirectory)/PreviewInfosWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/PreviewInfosWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PreviewInfosWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PreviewInfosWnd.cpp$(DependSuffix): PreviewInfosWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PreviewInfosWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PreviewInfosWnd.cpp$(DependSuffix) -MM PreviewInfosWnd.cpp

$(IntermediateDirectory)/PreviewInfosWnd.cpp$(PreprocessSuffix): PreviewInfosWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PreviewInfosWnd.cpp$(PreprocessSuffix) PreviewInfosWnd.cpp

$(IntermediateDirectory)/PreviewWnd.cpp$(ObjectSuffix): PreviewWnd.cpp $(IntermediateDirectory)/PreviewWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/PreviewWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PreviewWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PreviewWnd.cpp$(DependSuffix): PreviewWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PreviewWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PreviewWnd.cpp$(DependSuffix) -MM PreviewWnd.cpp

$(IntermediateDirectory)/PreviewWnd.cpp$(PreprocessSuffix): PreviewWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PreviewWnd.cpp$(PreprocessSuffix) PreviewWnd.cpp

$(IntermediateDirectory)/ToolbarPhoto.cpp$(ObjectSuffix): ToolbarPhoto.cpp $(IntermediateDirectory)/ToolbarPhoto.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/ToolbarPhoto.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ToolbarPhoto.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ToolbarPhoto.cpp$(DependSuffix): ToolbarPhoto.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ToolbarPhoto.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ToolbarPhoto.cpp$(DependSuffix) -MM ToolbarPhoto.cpp

$(IntermediateDirectory)/ToolbarPhoto.cpp$(PreprocessSuffix): ToolbarPhoto.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ToolbarPhoto.cpp$(PreprocessSuffix) ToolbarPhoto.cpp

$(IntermediateDirectory)/toolbar.cpp$(ObjectSuffix): toolbar.cpp $(IntermediateDirectory)/toolbar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/toolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/toolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/toolbar.cpp$(DependSuffix): toolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/toolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/toolbar.cpp$(DependSuffix) -MM toolbar.cpp

$(IntermediateDirectory)/toolbar.cpp$(PreprocessSuffix): toolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/toolbar.cpp$(PreprocessSuffix) toolbar.cpp

$(IntermediateDirectory)/CategoryWnd.cpp$(ObjectSuffix): CategoryWnd.cpp $(IntermediateDirectory)/CategoryWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/CategoryWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CategoryWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CategoryWnd.cpp$(DependSuffix): CategoryWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CategoryWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CategoryWnd.cpp$(DependSuffix) -MM CategoryWnd.cpp

$(IntermediateDirectory)/CategoryWnd.cpp$(PreprocessSuffix): CategoryWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CategoryWnd.cpp$(PreprocessSuffix) CategoryWnd.cpp

$(IntermediateDirectory)/ListPicture.cpp$(ObjectSuffix): ListPicture.cpp $(IntermediateDirectory)/ListPicture.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/ListPicture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ListPicture.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ListPicture.cpp$(DependSuffix): ListPicture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ListPicture.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ListPicture.cpp$(DependSuffix) -MM ListPicture.cpp

$(IntermediateDirectory)/ListPicture.cpp$(PreprocessSuffix): ListPicture.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ListPicture.cpp$(PreprocessSuffix) ListPicture.cpp

$(IntermediateDirectory)/CategoryFolderWindow.cpp$(ObjectSuffix): CategoryFolderWindow.cpp $(IntermediateDirectory)/CategoryFolderWindow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/CategoryFolderWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CategoryFolderWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CategoryFolderWindow.cpp$(DependSuffix): CategoryFolderWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CategoryFolderWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CategoryFolderWindow.cpp$(DependSuffix) -MM CategoryFolderWindow.cpp

$(IntermediateDirectory)/CategoryFolderWindow.cpp$(PreprocessSuffix): CategoryFolderWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CategoryFolderWindow.cpp$(PreprocessSuffix) CategoryFolderWindow.cpp

$(IntermediateDirectory)/ViewerTheme.cpp$(ObjectSuffix): ViewerTheme.cpp $(IntermediateDirectory)/ViewerTheme.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/ViewerTheme.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ViewerTheme.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ViewerTheme.cpp$(DependSuffix): ViewerTheme.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ViewerTheme.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ViewerTheme.cpp$(DependSuffix) -MM ViewerTheme.cpp

$(IntermediateDirectory)/ViewerTheme.cpp$(PreprocessSuffix): ViewerTheme.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ViewerTheme.cpp$(PreprocessSuffix) ViewerTheme.cpp

$(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(ObjectSuffix): ThumbnailToolBarZoom.cpp $(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/ThumbnailToolBarZoom.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(DependSuffix): ThumbnailToolBarZoom.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(DependSuffix) -MM ThumbnailToolBarZoom.cpp

$(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(PreprocessSuffix): ThumbnailToolBarZoom.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailToolBarZoom.cpp$(PreprocessSuffix) ThumbnailToolBarZoom.cpp

$(IntermediateDirectory)/PanelPhotoWnd.cpp$(ObjectSuffix): PanelPhotoWnd.cpp $(IntermediateDirectory)/PanelPhotoWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/PanelPhotoWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PanelPhotoWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PanelPhotoWnd.cpp$(DependSuffix): PanelPhotoWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PanelPhotoWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PanelPhotoWnd.cpp$(DependSuffix) -MM PanelPhotoWnd.cpp

$(IntermediateDirectory)/PanelPhotoWnd.cpp$(PreprocessSuffix): PanelPhotoWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PanelPhotoWnd.cpp$(PreprocessSuffix) PanelPhotoWnd.cpp

$(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(ObjectSuffix): ThumbnailViewerVideo.cpp $(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/ThumbnailViewerVideo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(DependSuffix): ThumbnailViewerVideo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(DependSuffix) -MM ThumbnailViewerVideo.cpp

$(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(PreprocessSuffix): ThumbnailViewerVideo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailViewerVideo.cpp$(PreprocessSuffix) ThumbnailViewerVideo.cpp

$(IntermediateDirectory)/ThumbnailViewerPicture.cpp$(ObjectSuffix): ThumbnailViewerPicture.cpp $(IntermediateDirectory)/ThumbnailViewerPicture.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/ThumbnailViewerPicture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailViewerPicture.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailViewerPicture.cpp$(DependSuffix): ThumbnailViewerPicture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailViewerPicture.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailViewerPicture.cpp$(DependSuffix) -MM ThumbnailViewerPicture.cpp

$(IntermediateDirectory)/ThumbnailViewerPicture.cpp$(PreprocessSuffix): ThumbnailViewerPicture.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailViewerPicture.cpp$(PreprocessSuffix) ThumbnailViewerPicture.cpp

$(IntermediateDirectory)/ThumbnailFacePertinenceToolBar.cpp$(ObjectSuffix): ThumbnailFacePertinenceToolBar.cpp $(IntermediateDirectory)/ThumbnailFacePertinenceToolBar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/ThumbnailFacePertinenceToolBar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailFacePertinenceToolBar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailFacePertinenceToolBar.cpp$(DependSuffix): ThumbnailFacePertinenceToolBar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailFacePertinenceToolBar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailFacePertinenceToolBar.cpp$(DependSuffix) -MM ThumbnailFacePertinenceToolBar.cpp

$(IntermediateDirectory)/ThumbnailFacePertinenceToolBar.cpp$(PreprocessSuffix): ThumbnailFacePertinenceToolBar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailFacePertinenceToolBar.cpp$(PreprocessSuffix) ThumbnailFacePertinenceToolBar.cpp

$(IntermediateDirectory)/ThumbnailFace.cpp$(ObjectSuffix): ThumbnailFace.cpp $(IntermediateDirectory)/ThumbnailFace.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/ThumbnailFace.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailFace.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailFace.cpp$(DependSuffix): ThumbnailFace.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailFace.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailFace.cpp$(DependSuffix) -MM ThumbnailFace.cpp

$(IntermediateDirectory)/ThumbnailFace.cpp$(PreprocessSuffix): ThumbnailFace.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailFace.cpp$(PreprocessSuffix) ThumbnailFace.cpp

$(IntermediateDirectory)/ThumbnailFaceToolBar.cpp$(ObjectSuffix): ThumbnailFaceToolBar.cpp $(IntermediateDirectory)/ThumbnailFaceToolBar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/ThumbnailFaceToolBar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailFaceToolBar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailFaceToolBar.cpp$(DependSuffix): ThumbnailFaceToolBar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailFaceToolBar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailFaceToolBar.cpp$(DependSuffix) -MM ThumbnailFaceToolBar.cpp

$(IntermediateDirectory)/ThumbnailFaceToolBar.cpp$(PreprocessSuffix): ThumbnailFaceToolBar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailFaceToolBar.cpp$(PreprocessSuffix) ThumbnailFaceToolBar.cpp

$(IntermediateDirectory)/FiltreEffect.cpp$(ObjectSuffix): FiltreEffect.cpp $(IntermediateDirectory)/FiltreEffect.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/FiltreEffect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FiltreEffect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FiltreEffect.cpp$(DependSuffix): FiltreEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FiltreEffect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FiltreEffect.cpp$(DependSuffix) -MM FiltreEffect.cpp

$(IntermediateDirectory)/FiltreEffect.cpp$(PreprocessSuffix): FiltreEffect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FiltreEffect.cpp$(PreprocessSuffix) FiltreEffect.cpp

$(IntermediateDirectory)/PanelInfosWnd.cpp$(ObjectSuffix): PanelInfosWnd.cpp $(IntermediateDirectory)/PanelInfosWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/PanelInfosWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PanelInfosWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PanelInfosWnd.cpp$(DependSuffix): PanelInfosWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PanelInfosWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PanelInfosWnd.cpp$(DependSuffix) -MM PanelInfosWnd.cpp

$(IntermediateDirectory)/PanelInfosWnd.cpp$(PreprocessSuffix): PanelInfosWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PanelInfosWnd.cpp$(PreprocessSuffix) PanelInfosWnd.cpp

$(IntermediateDirectory)/FiltreEffectWnd.cpp$(ObjectSuffix): FiltreEffectWnd.cpp $(IntermediateDirectory)/FiltreEffectWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/FiltreEffectWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FiltreEffectWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FiltreEffectWnd.cpp$(DependSuffix): FiltreEffectWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FiltreEffectWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FiltreEffectWnd.cpp$(DependSuffix) -MM FiltreEffectWnd.cpp

$(IntermediateDirectory)/FiltreEffectWnd.cpp$(PreprocessSuffix): FiltreEffectWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FiltreEffectWnd.cpp$(PreprocessSuffix) FiltreEffectWnd.cpp

$(IntermediateDirectory)/ThumbnailFolder.cpp$(ObjectSuffix): ThumbnailFolder.cpp $(IntermediateDirectory)/ThumbnailFolder.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/ThumbnailFolder.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailFolder.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailFolder.cpp$(DependSuffix): ThumbnailFolder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailFolder.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailFolder.cpp$(DependSuffix) -MM ThumbnailFolder.cpp

$(IntermediateDirectory)/ThumbnailFolder.cpp$(PreprocessSuffix): ThumbnailFolder.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailFolder.cpp$(PreprocessSuffix) ThumbnailFolder.cpp

$(IntermediateDirectory)/ThumbnailToolBar.cpp$(ObjectSuffix): ThumbnailToolBar.cpp $(IntermediateDirectory)/ThumbnailToolBar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/ThumbnailToolBar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailToolBar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailToolBar.cpp$(DependSuffix): ThumbnailToolBar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailToolBar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailToolBar.cpp$(DependSuffix) -MM ThumbnailToolBar.cpp

$(IntermediateDirectory)/ThumbnailToolBar.cpp$(PreprocessSuffix): ThumbnailToolBar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailToolBar.cpp$(PreprocessSuffix) ThumbnailToolBar.cpp

$(IntermediateDirectory)/FiltreToolbar.cpp$(ObjectSuffix): FiltreToolbar.cpp $(IntermediateDirectory)/FiltreToolbar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/FiltreToolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FiltreToolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FiltreToolbar.cpp$(DependSuffix): FiltreToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FiltreToolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FiltreToolbar.cpp$(DependSuffix) -MM FiltreToolbar.cpp

$(IntermediateDirectory)/FiltreToolbar.cpp$(PreprocessSuffix): FiltreToolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FiltreToolbar.cpp$(PreprocessSuffix) FiltreToolbar.cpp

$(IntermediateDirectory)/ViewerParamInit.cpp$(ObjectSuffix): ViewerParamInit.cpp $(IntermediateDirectory)/ViewerParamInit.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/ViewerParamInit.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ViewerParamInit.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ViewerParamInit.cpp$(DependSuffix): ViewerParamInit.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ViewerParamInit.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ViewerParamInit.cpp$(DependSuffix) -MM ViewerParamInit.cpp

$(IntermediateDirectory)/ViewerParamInit.cpp$(PreprocessSuffix): ViewerParamInit.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ViewerParamInit.cpp$(PreprocessSuffix) ViewerParamInit.cpp

$(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(ObjectSuffix): ThumbnailViewerEffect.cpp $(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/libViewer/ThumbnailViewerEffect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(DependSuffix): ThumbnailViewerEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(DependSuffix) -MM ThumbnailViewerEffect.cpp

$(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(PreprocessSuffix): ThumbnailViewerEffect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailViewerEffect.cpp$(PreprocessSuffix) ThumbnailViewerEffect.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/
	$(RM) ../include/Release/header.h.gch


