##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=libControl
ConfigurationName      :=Debug
WorkspacePath          := "/home/figuinha/dev/Regards"
ProjectPath            := "/home/figuinha/dev/Regards/libControl"
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
Preprocessors          :=$(PreprocessorSwitch)__WXGTK__ $(PreprocessorSwitch)EXIV2 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libControl.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libTheme $(IncludeSwitch)../libextern/libRapidXML $(IncludeSwitch)../libextern/ffmpeg-2.6.2 $(IncludeSwitch)../libextern/SDL2-2.0.3/include $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libParameter $(IncludeSwitch)../libExif $(IncludeSwitch)../libUtility $(IncludeSwitch)../libFiltreInterface $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../libResource $(IncludeSwitch)../libSqlData $(IncludeSwitch)../libOpenGL $(IncludeSwitch)../libRenderBitmap $(IncludeSwitch)../libSqlEngine $(IncludeSwitch)../libSqlite $(IncludeSwitch)../libextern/glew-1.13.0/include $(IncludeSwitch)../libFiltre $(IncludeSwitch)../include $(IncludeSwitch)../libDataStructure $(IncludeSwitch)../Otherlib/wxSVG/include $(IncludeSwitch)../libWindow $(IncludeSwitch)../libPrint $(IncludeSwitch)../libPicture $(IncludeSwitch)../libTheme $(IncludeSwitch)../libGeolocalisation $(IncludeSwitch)../libWindowMain $(IncludeSwitch)../libEmail $(IncludeSwitch)../libextern/wxWidgets-3.0.2/include $(IncludeSwitch)../libextern/wxWidgets-3.0.2/lib/wx/include/gtk2-unicode-static-3.0 
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
Objects0=$(IntermediateDirectory)/BitmapPrintout.cpp$(ObjectSuffix) $(IntermediateDirectory)/BitmapToolbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/BitmapWndViewer.cpp$(ObjectSuffix) $(IntermediateDirectory)/ClickToolbar.cpp$(ObjectSuffix) $(IntermediateDirectory)/EffectVideoWnd.cpp$(ObjectSuffix) $(IntermediateDirectory)/InfosFile.cpp$(ObjectSuffix) $(IntermediateDirectory)/InfosSeparationBarEffect.cpp$(ObjectSuffix) $(IntermediateDirectory)/ShowBitmap.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailEffect.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailHorizontal.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/ThumbnailVertical.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThumbnailVideo.cpp$(ObjectSuffix) $(IntermediateDirectory)/BitmapInfos.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/home/figuinha/dev/Regards/.build-debug/libControl"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/BitmapPrintout.cpp$(ObjectSuffix): BitmapPrintout.cpp $(IntermediateDirectory)/BitmapPrintout.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libControl/BitmapPrintout.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BitmapPrintout.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BitmapPrintout.cpp$(DependSuffix): BitmapPrintout.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BitmapPrintout.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BitmapPrintout.cpp$(DependSuffix) -MM "BitmapPrintout.cpp"

$(IntermediateDirectory)/BitmapPrintout.cpp$(PreprocessSuffix): BitmapPrintout.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BitmapPrintout.cpp$(PreprocessSuffix) "BitmapPrintout.cpp"

$(IntermediateDirectory)/BitmapToolbar.cpp$(ObjectSuffix): BitmapToolbar.cpp $(IntermediateDirectory)/BitmapToolbar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libControl/BitmapToolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BitmapToolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BitmapToolbar.cpp$(DependSuffix): BitmapToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BitmapToolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BitmapToolbar.cpp$(DependSuffix) -MM "BitmapToolbar.cpp"

$(IntermediateDirectory)/BitmapToolbar.cpp$(PreprocessSuffix): BitmapToolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BitmapToolbar.cpp$(PreprocessSuffix) "BitmapToolbar.cpp"

$(IntermediateDirectory)/BitmapWndViewer.cpp$(ObjectSuffix): BitmapWndViewer.cpp $(IntermediateDirectory)/BitmapWndViewer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libControl/BitmapWndViewer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BitmapWndViewer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BitmapWndViewer.cpp$(DependSuffix): BitmapWndViewer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BitmapWndViewer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BitmapWndViewer.cpp$(DependSuffix) -MM "BitmapWndViewer.cpp"

$(IntermediateDirectory)/BitmapWndViewer.cpp$(PreprocessSuffix): BitmapWndViewer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BitmapWndViewer.cpp$(PreprocessSuffix) "BitmapWndViewer.cpp"

$(IntermediateDirectory)/ClickToolbar.cpp$(ObjectSuffix): ClickToolbar.cpp $(IntermediateDirectory)/ClickToolbar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libControl/ClickToolbar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ClickToolbar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ClickToolbar.cpp$(DependSuffix): ClickToolbar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ClickToolbar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ClickToolbar.cpp$(DependSuffix) -MM "ClickToolbar.cpp"

$(IntermediateDirectory)/ClickToolbar.cpp$(PreprocessSuffix): ClickToolbar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ClickToolbar.cpp$(PreprocessSuffix) "ClickToolbar.cpp"

$(IntermediateDirectory)/EffectVideoWnd.cpp$(ObjectSuffix): EffectVideoWnd.cpp $(IntermediateDirectory)/EffectVideoWnd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libControl/EffectVideoWnd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/EffectVideoWnd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/EffectVideoWnd.cpp$(DependSuffix): EffectVideoWnd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/EffectVideoWnd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/EffectVideoWnd.cpp$(DependSuffix) -MM "EffectVideoWnd.cpp"

$(IntermediateDirectory)/EffectVideoWnd.cpp$(PreprocessSuffix): EffectVideoWnd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/EffectVideoWnd.cpp$(PreprocessSuffix) "EffectVideoWnd.cpp"

$(IntermediateDirectory)/InfosFile.cpp$(ObjectSuffix): InfosFile.cpp $(IntermediateDirectory)/InfosFile.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libControl/InfosFile.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InfosFile.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InfosFile.cpp$(DependSuffix): InfosFile.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InfosFile.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InfosFile.cpp$(DependSuffix) -MM "InfosFile.cpp"

$(IntermediateDirectory)/InfosFile.cpp$(PreprocessSuffix): InfosFile.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InfosFile.cpp$(PreprocessSuffix) "InfosFile.cpp"

$(IntermediateDirectory)/InfosSeparationBarEffect.cpp$(ObjectSuffix): InfosSeparationBarEffect.cpp $(IntermediateDirectory)/InfosSeparationBarEffect.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libControl/InfosSeparationBarEffect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InfosSeparationBarEffect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InfosSeparationBarEffect.cpp$(DependSuffix): InfosSeparationBarEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InfosSeparationBarEffect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InfosSeparationBarEffect.cpp$(DependSuffix) -MM "InfosSeparationBarEffect.cpp"

$(IntermediateDirectory)/InfosSeparationBarEffect.cpp$(PreprocessSuffix): InfosSeparationBarEffect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InfosSeparationBarEffect.cpp$(PreprocessSuffix) "InfosSeparationBarEffect.cpp"

$(IntermediateDirectory)/ShowBitmap.cpp$(ObjectSuffix): ShowBitmap.cpp $(IntermediateDirectory)/ShowBitmap.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libControl/ShowBitmap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ShowBitmap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ShowBitmap.cpp$(DependSuffix): ShowBitmap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ShowBitmap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ShowBitmap.cpp$(DependSuffix) -MM "ShowBitmap.cpp"

$(IntermediateDirectory)/ShowBitmap.cpp$(PreprocessSuffix): ShowBitmap.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ShowBitmap.cpp$(PreprocessSuffix) "ShowBitmap.cpp"

$(IntermediateDirectory)/ThumbnailEffect.cpp$(ObjectSuffix): ThumbnailEffect.cpp $(IntermediateDirectory)/ThumbnailEffect.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libControl/ThumbnailEffect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailEffect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailEffect.cpp$(DependSuffix): ThumbnailEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailEffect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailEffect.cpp$(DependSuffix) -MM "ThumbnailEffect.cpp"

$(IntermediateDirectory)/ThumbnailEffect.cpp$(PreprocessSuffix): ThumbnailEffect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailEffect.cpp$(PreprocessSuffix) "ThumbnailEffect.cpp"

$(IntermediateDirectory)/ThumbnailHorizontal.cpp$(ObjectSuffix): ThumbnailHorizontal.cpp $(IntermediateDirectory)/ThumbnailHorizontal.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libControl/ThumbnailHorizontal.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailHorizontal.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailHorizontal.cpp$(DependSuffix): ThumbnailHorizontal.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailHorizontal.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailHorizontal.cpp$(DependSuffix) -MM "ThumbnailHorizontal.cpp"

$(IntermediateDirectory)/ThumbnailHorizontal.cpp$(PreprocessSuffix): ThumbnailHorizontal.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailHorizontal.cpp$(PreprocessSuffix) "ThumbnailHorizontal.cpp"

$(IntermediateDirectory)/ThumbnailVertical.cpp$(ObjectSuffix): ThumbnailVertical.cpp $(IntermediateDirectory)/ThumbnailVertical.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libControl/ThumbnailVertical.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailVertical.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailVertical.cpp$(DependSuffix): ThumbnailVertical.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailVertical.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailVertical.cpp$(DependSuffix) -MM "ThumbnailVertical.cpp"

$(IntermediateDirectory)/ThumbnailVertical.cpp$(PreprocessSuffix): ThumbnailVertical.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailVertical.cpp$(PreprocessSuffix) "ThumbnailVertical.cpp"

$(IntermediateDirectory)/ThumbnailVideo.cpp$(ObjectSuffix): ThumbnailVideo.cpp $(IntermediateDirectory)/ThumbnailVideo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libControl/ThumbnailVideo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThumbnailVideo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThumbnailVideo.cpp$(DependSuffix): ThumbnailVideo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThumbnailVideo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThumbnailVideo.cpp$(DependSuffix) -MM "ThumbnailVideo.cpp"

$(IntermediateDirectory)/ThumbnailVideo.cpp$(PreprocessSuffix): ThumbnailVideo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThumbnailVideo.cpp$(PreprocessSuffix) "ThumbnailVideo.cpp"

$(IntermediateDirectory)/BitmapInfos.cpp$(ObjectSuffix): BitmapInfos.cpp $(IntermediateDirectory)/BitmapInfos.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/dev/Regards/libControl/BitmapInfos.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BitmapInfos.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BitmapInfos.cpp$(DependSuffix): BitmapInfos.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BitmapInfos.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BitmapInfos.cpp$(DependSuffix) -MM "BitmapInfos.cpp"

$(IntermediateDirectory)/BitmapInfos.cpp$(PreprocessSuffix): BitmapInfos.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BitmapInfos.cpp$(PreprocessSuffix) "BitmapInfos.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


