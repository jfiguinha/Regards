##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=wxSVG
ConfigurationName      :=Release
WorkspacePath          :=/home/figuinha/developpement/Regards
ProjectPath            :=/home/figuinha/developpement/Regards/Otherlib/wxSVG
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha
Date                   :=04/02/19
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
OutputFile             :=$(IntermediateDirectory)/lib$(ProjectName).a
Preprocessors          :=$(PreprocessorSwitch)USE_CAIRO $(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="wxSVG.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)/usr/include/OpenEXR $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/SDL2 $(IncludeSwitch)/usr/include/libxml2 $(IncludeSwitch)/opt/AMDAPPSDK-3.0/include  $(IncludeSwitch)../../libextern/ffmpeg-4.1 $(IncludeSwitch)../../libextern/libexif-master $(IncludeSwitch)../wxSVG/include $(IncludeSwitch)../wxSVG/include/wxSVG $(IncludeSwitch)../wxSVG/include/wxSVGXML 
IncludePCH             :=  -include ../../include/Release/header.h 
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
CXXFLAGS := -std=gnu++11 $(shell ../../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread  -Wno-narrowing -fopenmp -O2 -msse4.1 $(Preprocessors)
CFLAGS   := -Wall  -O2 -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := ccache /usr/bin/x86_64-linux-gnu-as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/src_cairo_SVGCanvasPathCairo.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGImageElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGElementInstanceList.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGTransformList.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGTransformable.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGStylable.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGTransform.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGTextContentElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGTextElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGTests.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_SVGSVGElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_imagsvg.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_GetSVGDocument.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGRectElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGRadialGradientElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Elements_GetAttribute.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGPointList.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGPolygonElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGStringList.cpp$(ObjectSuffix) 

Objects1=$(IntermediateDirectory)/src_SVGPaint.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_SVGNumberList.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGLocatable.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGMatrix.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_CSSStyleDeclaration.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_mediadec_ffmpeg.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Elements_HasAttribute.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGCircleElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Elements_GetAttributes.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGRect.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGAngle.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_SVGMarkerElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGAnimateMotionElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGLineElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_ExifHandler.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_css_values.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGFilterElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_NodeList.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGUseElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGPolylineElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGAnimationElement.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_SVGColor.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGAnimatedType.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGCanvasItem.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGAnimateTransformElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_svgctrl.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGDocument.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGEllipseElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_cairo_SVGCanvasImageCairo.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGFEGaussianBlurElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_cairo_SVGCanvasTextCairo.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_SVGPathSegList.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGPathElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGVideoElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGLength.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGCanvas.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGLengthList.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGPreserveAspectRatio.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SVGFitToViewBox.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_cairo_SVGCanvasCairo.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_svgxml_svgxmlhelpr.cpp$(ObjectSuffix) \
	

Objects2=$(IntermediateDirectory)/src_SVGPoint.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_svgxml_svgxml.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_css_properties.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_CSSValue.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Elements_CopyConstructors.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Elements_SetAttribute.cpp$(ObjectSuffix) 



Objects=$(Objects0) $(Objects1) $(Objects2) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(IntermediateDirectory) $(OutputFile)

$(OutputFile): $(Objects)
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	@echo $(Objects1) >> $(ObjectsFileList)
	@echo $(Objects2) >> $(ObjectsFileList)
	$(AR) $(ArchiveOutputSwitch)$(OutputFile) @$(ObjectsFileList) $(ArLibs)
	@$(MakeDirCommand) "/home/figuinha/developpement/Regards/.build-release"
	@echo rebuilt > "/home/figuinha/developpement/Regards/.build-release/wxSVG"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


./Release:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:

# PreCompiled Header
../../include/Release/header.h.gch: ../../include/Release/header.h
	$(CXX) $(SourceSwitch) ../../include/Release/header.h $(PCHCompileFlags) $(CXXFLAGS) $(IncludePath)



##
## Objects
##
$(IntermediateDirectory)/src_cairo_SVGCanvasPathCairo.cpp$(ObjectSuffix): src/cairo/SVGCanvasPathCairo.cpp $(IntermediateDirectory)/src_cairo_SVGCanvasPathCairo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/cairo/SVGCanvasPathCairo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_cairo_SVGCanvasPathCairo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_cairo_SVGCanvasPathCairo.cpp$(DependSuffix): src/cairo/SVGCanvasPathCairo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_cairo_SVGCanvasPathCairo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_cairo_SVGCanvasPathCairo.cpp$(DependSuffix) -MM src/cairo/SVGCanvasPathCairo.cpp

$(IntermediateDirectory)/src_cairo_SVGCanvasPathCairo.cpp$(PreprocessSuffix): src/cairo/SVGCanvasPathCairo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_cairo_SVGCanvasPathCairo.cpp$(PreprocessSuffix) src/cairo/SVGCanvasPathCairo.cpp

$(IntermediateDirectory)/src_SVGImageElement.cpp$(ObjectSuffix): src/SVGImageElement.cpp $(IntermediateDirectory)/src_SVGImageElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGImageElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGImageElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGImageElement.cpp$(DependSuffix): src/SVGImageElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGImageElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGImageElement.cpp$(DependSuffix) -MM src/SVGImageElement.cpp

$(IntermediateDirectory)/src_SVGImageElement.cpp$(PreprocessSuffix): src/SVGImageElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGImageElement.cpp$(PreprocessSuffix) src/SVGImageElement.cpp

$(IntermediateDirectory)/src_SVGElementInstanceList.cpp$(ObjectSuffix): src/SVGElementInstanceList.cpp $(IntermediateDirectory)/src_SVGElementInstanceList.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGElementInstanceList.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGElementInstanceList.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGElementInstanceList.cpp$(DependSuffix): src/SVGElementInstanceList.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGElementInstanceList.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGElementInstanceList.cpp$(DependSuffix) -MM src/SVGElementInstanceList.cpp

$(IntermediateDirectory)/src_SVGElementInstanceList.cpp$(PreprocessSuffix): src/SVGElementInstanceList.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGElementInstanceList.cpp$(PreprocessSuffix) src/SVGElementInstanceList.cpp

$(IntermediateDirectory)/src_SVGTransformList.cpp$(ObjectSuffix): src/SVGTransformList.cpp $(IntermediateDirectory)/src_SVGTransformList.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGTransformList.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGTransformList.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGTransformList.cpp$(DependSuffix): src/SVGTransformList.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGTransformList.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGTransformList.cpp$(DependSuffix) -MM src/SVGTransformList.cpp

$(IntermediateDirectory)/src_SVGTransformList.cpp$(PreprocessSuffix): src/SVGTransformList.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGTransformList.cpp$(PreprocessSuffix) src/SVGTransformList.cpp

$(IntermediateDirectory)/src_SVGTransformable.cpp$(ObjectSuffix): src/SVGTransformable.cpp $(IntermediateDirectory)/src_SVGTransformable.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGTransformable.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGTransformable.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGTransformable.cpp$(DependSuffix): src/SVGTransformable.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGTransformable.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGTransformable.cpp$(DependSuffix) -MM src/SVGTransformable.cpp

$(IntermediateDirectory)/src_SVGTransformable.cpp$(PreprocessSuffix): src/SVGTransformable.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGTransformable.cpp$(PreprocessSuffix) src/SVGTransformable.cpp

$(IntermediateDirectory)/src_SVGStylable.cpp$(ObjectSuffix): src/SVGStylable.cpp $(IntermediateDirectory)/src_SVGStylable.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGStylable.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGStylable.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGStylable.cpp$(DependSuffix): src/SVGStylable.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGStylable.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGStylable.cpp$(DependSuffix) -MM src/SVGStylable.cpp

$(IntermediateDirectory)/src_SVGStylable.cpp$(PreprocessSuffix): src/SVGStylable.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGStylable.cpp$(PreprocessSuffix) src/SVGStylable.cpp

$(IntermediateDirectory)/src_SVGTransform.cpp$(ObjectSuffix): src/SVGTransform.cpp $(IntermediateDirectory)/src_SVGTransform.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGTransform.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGTransform.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGTransform.cpp$(DependSuffix): src/SVGTransform.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGTransform.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGTransform.cpp$(DependSuffix) -MM src/SVGTransform.cpp

$(IntermediateDirectory)/src_SVGTransform.cpp$(PreprocessSuffix): src/SVGTransform.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGTransform.cpp$(PreprocessSuffix) src/SVGTransform.cpp

$(IntermediateDirectory)/src_SVGTextContentElement.cpp$(ObjectSuffix): src/SVGTextContentElement.cpp $(IntermediateDirectory)/src_SVGTextContentElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGTextContentElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGTextContentElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGTextContentElement.cpp$(DependSuffix): src/SVGTextContentElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGTextContentElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGTextContentElement.cpp$(DependSuffix) -MM src/SVGTextContentElement.cpp

$(IntermediateDirectory)/src_SVGTextContentElement.cpp$(PreprocessSuffix): src/SVGTextContentElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGTextContentElement.cpp$(PreprocessSuffix) src/SVGTextContentElement.cpp

$(IntermediateDirectory)/src_SVGTextElement.cpp$(ObjectSuffix): src/SVGTextElement.cpp $(IntermediateDirectory)/src_SVGTextElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGTextElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGTextElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGTextElement.cpp$(DependSuffix): src/SVGTextElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGTextElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGTextElement.cpp$(DependSuffix) -MM src/SVGTextElement.cpp

$(IntermediateDirectory)/src_SVGTextElement.cpp$(PreprocessSuffix): src/SVGTextElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGTextElement.cpp$(PreprocessSuffix) src/SVGTextElement.cpp

$(IntermediateDirectory)/src_SVGTests.cpp$(ObjectSuffix): src/SVGTests.cpp $(IntermediateDirectory)/src_SVGTests.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGTests.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGTests.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGTests.cpp$(DependSuffix): src/SVGTests.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGTests.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGTests.cpp$(DependSuffix) -MM src/SVGTests.cpp

$(IntermediateDirectory)/src_SVGTests.cpp$(PreprocessSuffix): src/SVGTests.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGTests.cpp$(PreprocessSuffix) src/SVGTests.cpp

$(IntermediateDirectory)/src_SVGSVGElement.cpp$(ObjectSuffix): src/SVGSVGElement.cpp $(IntermediateDirectory)/src_SVGSVGElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGSVGElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGSVGElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGSVGElement.cpp$(DependSuffix): src/SVGSVGElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGSVGElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGSVGElement.cpp$(DependSuffix) -MM src/SVGSVGElement.cpp

$(IntermediateDirectory)/src_SVGSVGElement.cpp$(PreprocessSuffix): src/SVGSVGElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGSVGElement.cpp$(PreprocessSuffix) src/SVGSVGElement.cpp

$(IntermediateDirectory)/src_imagsvg.cpp$(ObjectSuffix): src/imagsvg.cpp $(IntermediateDirectory)/src_imagsvg.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/imagsvg.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_imagsvg.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_imagsvg.cpp$(DependSuffix): src/imagsvg.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_imagsvg.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_imagsvg.cpp$(DependSuffix) -MM src/imagsvg.cpp

$(IntermediateDirectory)/src_imagsvg.cpp$(PreprocessSuffix): src/imagsvg.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_imagsvg.cpp$(PreprocessSuffix) src/imagsvg.cpp

$(IntermediateDirectory)/src_GetSVGDocument.cpp$(ObjectSuffix): src/GetSVGDocument.cpp $(IntermediateDirectory)/src_GetSVGDocument.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/GetSVGDocument.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_GetSVGDocument.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_GetSVGDocument.cpp$(DependSuffix): src/GetSVGDocument.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_GetSVGDocument.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_GetSVGDocument.cpp$(DependSuffix) -MM src/GetSVGDocument.cpp

$(IntermediateDirectory)/src_GetSVGDocument.cpp$(PreprocessSuffix): src/GetSVGDocument.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_GetSVGDocument.cpp$(PreprocessSuffix) src/GetSVGDocument.cpp

$(IntermediateDirectory)/src_SVGRectElement.cpp$(ObjectSuffix): src/SVGRectElement.cpp $(IntermediateDirectory)/src_SVGRectElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGRectElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGRectElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGRectElement.cpp$(DependSuffix): src/SVGRectElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGRectElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGRectElement.cpp$(DependSuffix) -MM src/SVGRectElement.cpp

$(IntermediateDirectory)/src_SVGRectElement.cpp$(PreprocessSuffix): src/SVGRectElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGRectElement.cpp$(PreprocessSuffix) src/SVGRectElement.cpp

$(IntermediateDirectory)/src_SVGRadialGradientElement.cpp$(ObjectSuffix): src/SVGRadialGradientElement.cpp $(IntermediateDirectory)/src_SVGRadialGradientElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGRadialGradientElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGRadialGradientElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGRadialGradientElement.cpp$(DependSuffix): src/SVGRadialGradientElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGRadialGradientElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGRadialGradientElement.cpp$(DependSuffix) -MM src/SVGRadialGradientElement.cpp

$(IntermediateDirectory)/src_SVGRadialGradientElement.cpp$(PreprocessSuffix): src/SVGRadialGradientElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGRadialGradientElement.cpp$(PreprocessSuffix) src/SVGRadialGradientElement.cpp

$(IntermediateDirectory)/src_Elements_GetAttribute.cpp$(ObjectSuffix): src/Elements_GetAttribute.cpp $(IntermediateDirectory)/src_Elements_GetAttribute.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/Elements_GetAttribute.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Elements_GetAttribute.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Elements_GetAttribute.cpp$(DependSuffix): src/Elements_GetAttribute.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Elements_GetAttribute.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Elements_GetAttribute.cpp$(DependSuffix) -MM src/Elements_GetAttribute.cpp

$(IntermediateDirectory)/src_Elements_GetAttribute.cpp$(PreprocessSuffix): src/Elements_GetAttribute.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Elements_GetAttribute.cpp$(PreprocessSuffix) src/Elements_GetAttribute.cpp

$(IntermediateDirectory)/src_SVGPointList.cpp$(ObjectSuffix): src/SVGPointList.cpp $(IntermediateDirectory)/src_SVGPointList.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGPointList.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGPointList.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGPointList.cpp$(DependSuffix): src/SVGPointList.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGPointList.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGPointList.cpp$(DependSuffix) -MM src/SVGPointList.cpp

$(IntermediateDirectory)/src_SVGPointList.cpp$(PreprocessSuffix): src/SVGPointList.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGPointList.cpp$(PreprocessSuffix) src/SVGPointList.cpp

$(IntermediateDirectory)/src_SVGPolygonElement.cpp$(ObjectSuffix): src/SVGPolygonElement.cpp $(IntermediateDirectory)/src_SVGPolygonElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGPolygonElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGPolygonElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGPolygonElement.cpp$(DependSuffix): src/SVGPolygonElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGPolygonElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGPolygonElement.cpp$(DependSuffix) -MM src/SVGPolygonElement.cpp

$(IntermediateDirectory)/src_SVGPolygonElement.cpp$(PreprocessSuffix): src/SVGPolygonElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGPolygonElement.cpp$(PreprocessSuffix) src/SVGPolygonElement.cpp

$(IntermediateDirectory)/src_SVGStringList.cpp$(ObjectSuffix): src/SVGStringList.cpp $(IntermediateDirectory)/src_SVGStringList.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGStringList.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGStringList.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGStringList.cpp$(DependSuffix): src/SVGStringList.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGStringList.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGStringList.cpp$(DependSuffix) -MM src/SVGStringList.cpp

$(IntermediateDirectory)/src_SVGStringList.cpp$(PreprocessSuffix): src/SVGStringList.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGStringList.cpp$(PreprocessSuffix) src/SVGStringList.cpp

$(IntermediateDirectory)/src_SVGPaint.cpp$(ObjectSuffix): src/SVGPaint.cpp $(IntermediateDirectory)/src_SVGPaint.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGPaint.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGPaint.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGPaint.cpp$(DependSuffix): src/SVGPaint.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGPaint.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGPaint.cpp$(DependSuffix) -MM src/SVGPaint.cpp

$(IntermediateDirectory)/src_SVGPaint.cpp$(PreprocessSuffix): src/SVGPaint.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGPaint.cpp$(PreprocessSuffix) src/SVGPaint.cpp

$(IntermediateDirectory)/src_SVGNumberList.cpp$(ObjectSuffix): src/SVGNumberList.cpp $(IntermediateDirectory)/src_SVGNumberList.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGNumberList.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGNumberList.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGNumberList.cpp$(DependSuffix): src/SVGNumberList.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGNumberList.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGNumberList.cpp$(DependSuffix) -MM src/SVGNumberList.cpp

$(IntermediateDirectory)/src_SVGNumberList.cpp$(PreprocessSuffix): src/SVGNumberList.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGNumberList.cpp$(PreprocessSuffix) src/SVGNumberList.cpp

$(IntermediateDirectory)/src_SVGLocatable.cpp$(ObjectSuffix): src/SVGLocatable.cpp $(IntermediateDirectory)/src_SVGLocatable.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGLocatable.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGLocatable.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGLocatable.cpp$(DependSuffix): src/SVGLocatable.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGLocatable.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGLocatable.cpp$(DependSuffix) -MM src/SVGLocatable.cpp

$(IntermediateDirectory)/src_SVGLocatable.cpp$(PreprocessSuffix): src/SVGLocatable.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGLocatable.cpp$(PreprocessSuffix) src/SVGLocatable.cpp

$(IntermediateDirectory)/src_SVGMatrix.cpp$(ObjectSuffix): src/SVGMatrix.cpp $(IntermediateDirectory)/src_SVGMatrix.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGMatrix.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGMatrix.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGMatrix.cpp$(DependSuffix): src/SVGMatrix.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGMatrix.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGMatrix.cpp$(DependSuffix) -MM src/SVGMatrix.cpp

$(IntermediateDirectory)/src_SVGMatrix.cpp$(PreprocessSuffix): src/SVGMatrix.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGMatrix.cpp$(PreprocessSuffix) src/SVGMatrix.cpp

$(IntermediateDirectory)/src_CSSStyleDeclaration.cpp$(ObjectSuffix): src/CSSStyleDeclaration.cpp $(IntermediateDirectory)/src_CSSStyleDeclaration.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/CSSStyleDeclaration.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_CSSStyleDeclaration.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_CSSStyleDeclaration.cpp$(DependSuffix): src/CSSStyleDeclaration.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_CSSStyleDeclaration.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_CSSStyleDeclaration.cpp$(DependSuffix) -MM src/CSSStyleDeclaration.cpp

$(IntermediateDirectory)/src_CSSStyleDeclaration.cpp$(PreprocessSuffix): src/CSSStyleDeclaration.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_CSSStyleDeclaration.cpp$(PreprocessSuffix) src/CSSStyleDeclaration.cpp

$(IntermediateDirectory)/src_mediadec_ffmpeg.cpp$(ObjectSuffix): src/mediadec_ffmpeg.cpp $(IntermediateDirectory)/src_mediadec_ffmpeg.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/mediadec_ffmpeg.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_mediadec_ffmpeg.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_mediadec_ffmpeg.cpp$(DependSuffix): src/mediadec_ffmpeg.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_mediadec_ffmpeg.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_mediadec_ffmpeg.cpp$(DependSuffix) -MM src/mediadec_ffmpeg.cpp

$(IntermediateDirectory)/src_mediadec_ffmpeg.cpp$(PreprocessSuffix): src/mediadec_ffmpeg.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_mediadec_ffmpeg.cpp$(PreprocessSuffix) src/mediadec_ffmpeg.cpp

$(IntermediateDirectory)/src_Elements_HasAttribute.cpp$(ObjectSuffix): src/Elements_HasAttribute.cpp $(IntermediateDirectory)/src_Elements_HasAttribute.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/Elements_HasAttribute.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Elements_HasAttribute.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Elements_HasAttribute.cpp$(DependSuffix): src/Elements_HasAttribute.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Elements_HasAttribute.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Elements_HasAttribute.cpp$(DependSuffix) -MM src/Elements_HasAttribute.cpp

$(IntermediateDirectory)/src_Elements_HasAttribute.cpp$(PreprocessSuffix): src/Elements_HasAttribute.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Elements_HasAttribute.cpp$(PreprocessSuffix) src/Elements_HasAttribute.cpp

$(IntermediateDirectory)/src_SVGCircleElement.cpp$(ObjectSuffix): src/SVGCircleElement.cpp $(IntermediateDirectory)/src_SVGCircleElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGCircleElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGCircleElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGCircleElement.cpp$(DependSuffix): src/SVGCircleElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGCircleElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGCircleElement.cpp$(DependSuffix) -MM src/SVGCircleElement.cpp

$(IntermediateDirectory)/src_SVGCircleElement.cpp$(PreprocessSuffix): src/SVGCircleElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGCircleElement.cpp$(PreprocessSuffix) src/SVGCircleElement.cpp

$(IntermediateDirectory)/src_Elements_GetAttributes.cpp$(ObjectSuffix): src/Elements_GetAttributes.cpp $(IntermediateDirectory)/src_Elements_GetAttributes.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/Elements_GetAttributes.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Elements_GetAttributes.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Elements_GetAttributes.cpp$(DependSuffix): src/Elements_GetAttributes.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Elements_GetAttributes.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Elements_GetAttributes.cpp$(DependSuffix) -MM src/Elements_GetAttributes.cpp

$(IntermediateDirectory)/src_Elements_GetAttributes.cpp$(PreprocessSuffix): src/Elements_GetAttributes.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Elements_GetAttributes.cpp$(PreprocessSuffix) src/Elements_GetAttributes.cpp

$(IntermediateDirectory)/src_SVGRect.cpp$(ObjectSuffix): src/SVGRect.cpp $(IntermediateDirectory)/src_SVGRect.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGRect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGRect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGRect.cpp$(DependSuffix): src/SVGRect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGRect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGRect.cpp$(DependSuffix) -MM src/SVGRect.cpp

$(IntermediateDirectory)/src_SVGRect.cpp$(PreprocessSuffix): src/SVGRect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGRect.cpp$(PreprocessSuffix) src/SVGRect.cpp

$(IntermediateDirectory)/src_SVGAngle.cpp$(ObjectSuffix): src/SVGAngle.cpp $(IntermediateDirectory)/src_SVGAngle.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGAngle.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGAngle.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGAngle.cpp$(DependSuffix): src/SVGAngle.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGAngle.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGAngle.cpp$(DependSuffix) -MM src/SVGAngle.cpp

$(IntermediateDirectory)/src_SVGAngle.cpp$(PreprocessSuffix): src/SVGAngle.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGAngle.cpp$(PreprocessSuffix) src/SVGAngle.cpp

$(IntermediateDirectory)/src_SVGMarkerElement.cpp$(ObjectSuffix): src/SVGMarkerElement.cpp $(IntermediateDirectory)/src_SVGMarkerElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGMarkerElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGMarkerElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGMarkerElement.cpp$(DependSuffix): src/SVGMarkerElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGMarkerElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGMarkerElement.cpp$(DependSuffix) -MM src/SVGMarkerElement.cpp

$(IntermediateDirectory)/src_SVGMarkerElement.cpp$(PreprocessSuffix): src/SVGMarkerElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGMarkerElement.cpp$(PreprocessSuffix) src/SVGMarkerElement.cpp

$(IntermediateDirectory)/src_SVGAnimateMotionElement.cpp$(ObjectSuffix): src/SVGAnimateMotionElement.cpp $(IntermediateDirectory)/src_SVGAnimateMotionElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGAnimateMotionElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGAnimateMotionElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGAnimateMotionElement.cpp$(DependSuffix): src/SVGAnimateMotionElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGAnimateMotionElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGAnimateMotionElement.cpp$(DependSuffix) -MM src/SVGAnimateMotionElement.cpp

$(IntermediateDirectory)/src_SVGAnimateMotionElement.cpp$(PreprocessSuffix): src/SVGAnimateMotionElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGAnimateMotionElement.cpp$(PreprocessSuffix) src/SVGAnimateMotionElement.cpp

$(IntermediateDirectory)/src_SVGLineElement.cpp$(ObjectSuffix): src/SVGLineElement.cpp $(IntermediateDirectory)/src_SVGLineElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGLineElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGLineElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGLineElement.cpp$(DependSuffix): src/SVGLineElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGLineElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGLineElement.cpp$(DependSuffix) -MM src/SVGLineElement.cpp

$(IntermediateDirectory)/src_SVGLineElement.cpp$(PreprocessSuffix): src/SVGLineElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGLineElement.cpp$(PreprocessSuffix) src/SVGLineElement.cpp

$(IntermediateDirectory)/src_ExifHandler.cpp$(ObjectSuffix): src/ExifHandler.cpp $(IntermediateDirectory)/src_ExifHandler.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/ExifHandler.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_ExifHandler.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_ExifHandler.cpp$(DependSuffix): src/ExifHandler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_ExifHandler.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_ExifHandler.cpp$(DependSuffix) -MM src/ExifHandler.cpp

$(IntermediateDirectory)/src_ExifHandler.cpp$(PreprocessSuffix): src/ExifHandler.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_ExifHandler.cpp$(PreprocessSuffix) src/ExifHandler.cpp

$(IntermediateDirectory)/src_css_values.cpp$(ObjectSuffix): src/css_values.cpp $(IntermediateDirectory)/src_css_values.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/css_values.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_css_values.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_css_values.cpp$(DependSuffix): src/css_values.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_css_values.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_css_values.cpp$(DependSuffix) -MM src/css_values.cpp

$(IntermediateDirectory)/src_css_values.cpp$(PreprocessSuffix): src/css_values.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_css_values.cpp$(PreprocessSuffix) src/css_values.cpp

$(IntermediateDirectory)/src_SVGFilterElement.cpp$(ObjectSuffix): src/SVGFilterElement.cpp $(IntermediateDirectory)/src_SVGFilterElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGFilterElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGFilterElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGFilterElement.cpp$(DependSuffix): src/SVGFilterElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGFilterElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGFilterElement.cpp$(DependSuffix) -MM src/SVGFilterElement.cpp

$(IntermediateDirectory)/src_SVGFilterElement.cpp$(PreprocessSuffix): src/SVGFilterElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGFilterElement.cpp$(PreprocessSuffix) src/SVGFilterElement.cpp

$(IntermediateDirectory)/src_NodeList.cpp$(ObjectSuffix): src/NodeList.cpp $(IntermediateDirectory)/src_NodeList.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/NodeList.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_NodeList.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_NodeList.cpp$(DependSuffix): src/NodeList.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_NodeList.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_NodeList.cpp$(DependSuffix) -MM src/NodeList.cpp

$(IntermediateDirectory)/src_NodeList.cpp$(PreprocessSuffix): src/NodeList.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_NodeList.cpp$(PreprocessSuffix) src/NodeList.cpp

$(IntermediateDirectory)/src_SVGUseElement.cpp$(ObjectSuffix): src/SVGUseElement.cpp $(IntermediateDirectory)/src_SVGUseElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGUseElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGUseElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGUseElement.cpp$(DependSuffix): src/SVGUseElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGUseElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGUseElement.cpp$(DependSuffix) -MM src/SVGUseElement.cpp

$(IntermediateDirectory)/src_SVGUseElement.cpp$(PreprocessSuffix): src/SVGUseElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGUseElement.cpp$(PreprocessSuffix) src/SVGUseElement.cpp

$(IntermediateDirectory)/src_SVGPolylineElement.cpp$(ObjectSuffix): src/SVGPolylineElement.cpp $(IntermediateDirectory)/src_SVGPolylineElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGPolylineElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGPolylineElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGPolylineElement.cpp$(DependSuffix): src/SVGPolylineElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGPolylineElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGPolylineElement.cpp$(DependSuffix) -MM src/SVGPolylineElement.cpp

$(IntermediateDirectory)/src_SVGPolylineElement.cpp$(PreprocessSuffix): src/SVGPolylineElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGPolylineElement.cpp$(PreprocessSuffix) src/SVGPolylineElement.cpp

$(IntermediateDirectory)/src_SVGAnimationElement.cpp$(ObjectSuffix): src/SVGAnimationElement.cpp $(IntermediateDirectory)/src_SVGAnimationElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGAnimationElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGAnimationElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGAnimationElement.cpp$(DependSuffix): src/SVGAnimationElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGAnimationElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGAnimationElement.cpp$(DependSuffix) -MM src/SVGAnimationElement.cpp

$(IntermediateDirectory)/src_SVGAnimationElement.cpp$(PreprocessSuffix): src/SVGAnimationElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGAnimationElement.cpp$(PreprocessSuffix) src/SVGAnimationElement.cpp

$(IntermediateDirectory)/src_SVGColor.cpp$(ObjectSuffix): src/SVGColor.cpp $(IntermediateDirectory)/src_SVGColor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGColor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGColor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGColor.cpp$(DependSuffix): src/SVGColor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGColor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGColor.cpp$(DependSuffix) -MM src/SVGColor.cpp

$(IntermediateDirectory)/src_SVGColor.cpp$(PreprocessSuffix): src/SVGColor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGColor.cpp$(PreprocessSuffix) src/SVGColor.cpp

$(IntermediateDirectory)/src_SVGAnimatedType.cpp$(ObjectSuffix): src/SVGAnimatedType.cpp $(IntermediateDirectory)/src_SVGAnimatedType.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGAnimatedType.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGAnimatedType.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGAnimatedType.cpp$(DependSuffix): src/SVGAnimatedType.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGAnimatedType.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGAnimatedType.cpp$(DependSuffix) -MM src/SVGAnimatedType.cpp

$(IntermediateDirectory)/src_SVGAnimatedType.cpp$(PreprocessSuffix): src/SVGAnimatedType.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGAnimatedType.cpp$(PreprocessSuffix) src/SVGAnimatedType.cpp

$(IntermediateDirectory)/src_SVGCanvasItem.cpp$(ObjectSuffix): src/SVGCanvasItem.cpp $(IntermediateDirectory)/src_SVGCanvasItem.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGCanvasItem.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGCanvasItem.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGCanvasItem.cpp$(DependSuffix): src/SVGCanvasItem.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGCanvasItem.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGCanvasItem.cpp$(DependSuffix) -MM src/SVGCanvasItem.cpp

$(IntermediateDirectory)/src_SVGCanvasItem.cpp$(PreprocessSuffix): src/SVGCanvasItem.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGCanvasItem.cpp$(PreprocessSuffix) src/SVGCanvasItem.cpp

$(IntermediateDirectory)/src_SVGAnimateTransformElement.cpp$(ObjectSuffix): src/SVGAnimateTransformElement.cpp $(IntermediateDirectory)/src_SVGAnimateTransformElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGAnimateTransformElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGAnimateTransformElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGAnimateTransformElement.cpp$(DependSuffix): src/SVGAnimateTransformElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGAnimateTransformElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGAnimateTransformElement.cpp$(DependSuffix) -MM src/SVGAnimateTransformElement.cpp

$(IntermediateDirectory)/src_SVGAnimateTransformElement.cpp$(PreprocessSuffix): src/SVGAnimateTransformElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGAnimateTransformElement.cpp$(PreprocessSuffix) src/SVGAnimateTransformElement.cpp

$(IntermediateDirectory)/src_svgctrl.cpp$(ObjectSuffix): src/svgctrl.cpp $(IntermediateDirectory)/src_svgctrl.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/svgctrl.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_svgctrl.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_svgctrl.cpp$(DependSuffix): src/svgctrl.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_svgctrl.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_svgctrl.cpp$(DependSuffix) -MM src/svgctrl.cpp

$(IntermediateDirectory)/src_svgctrl.cpp$(PreprocessSuffix): src/svgctrl.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_svgctrl.cpp$(PreprocessSuffix) src/svgctrl.cpp

$(IntermediateDirectory)/src_SVGDocument.cpp$(ObjectSuffix): src/SVGDocument.cpp $(IntermediateDirectory)/src_SVGDocument.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGDocument.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGDocument.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGDocument.cpp$(DependSuffix): src/SVGDocument.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGDocument.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGDocument.cpp$(DependSuffix) -MM src/SVGDocument.cpp

$(IntermediateDirectory)/src_SVGDocument.cpp$(PreprocessSuffix): src/SVGDocument.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGDocument.cpp$(PreprocessSuffix) src/SVGDocument.cpp

$(IntermediateDirectory)/src_SVGEllipseElement.cpp$(ObjectSuffix): src/SVGEllipseElement.cpp $(IntermediateDirectory)/src_SVGEllipseElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGEllipseElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGEllipseElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGEllipseElement.cpp$(DependSuffix): src/SVGEllipseElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGEllipseElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGEllipseElement.cpp$(DependSuffix) -MM src/SVGEllipseElement.cpp

$(IntermediateDirectory)/src_SVGEllipseElement.cpp$(PreprocessSuffix): src/SVGEllipseElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGEllipseElement.cpp$(PreprocessSuffix) src/SVGEllipseElement.cpp

$(IntermediateDirectory)/src_cairo_SVGCanvasImageCairo.cpp$(ObjectSuffix): src/cairo/SVGCanvasImageCairo.cpp $(IntermediateDirectory)/src_cairo_SVGCanvasImageCairo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/cairo/SVGCanvasImageCairo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_cairo_SVGCanvasImageCairo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_cairo_SVGCanvasImageCairo.cpp$(DependSuffix): src/cairo/SVGCanvasImageCairo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_cairo_SVGCanvasImageCairo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_cairo_SVGCanvasImageCairo.cpp$(DependSuffix) -MM src/cairo/SVGCanvasImageCairo.cpp

$(IntermediateDirectory)/src_cairo_SVGCanvasImageCairo.cpp$(PreprocessSuffix): src/cairo/SVGCanvasImageCairo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_cairo_SVGCanvasImageCairo.cpp$(PreprocessSuffix) src/cairo/SVGCanvasImageCairo.cpp

$(IntermediateDirectory)/src_SVGFEGaussianBlurElement.cpp$(ObjectSuffix): src/SVGFEGaussianBlurElement.cpp $(IntermediateDirectory)/src_SVGFEGaussianBlurElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGFEGaussianBlurElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGFEGaussianBlurElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGFEGaussianBlurElement.cpp$(DependSuffix): src/SVGFEGaussianBlurElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGFEGaussianBlurElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGFEGaussianBlurElement.cpp$(DependSuffix) -MM src/SVGFEGaussianBlurElement.cpp

$(IntermediateDirectory)/src_SVGFEGaussianBlurElement.cpp$(PreprocessSuffix): src/SVGFEGaussianBlurElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGFEGaussianBlurElement.cpp$(PreprocessSuffix) src/SVGFEGaussianBlurElement.cpp

$(IntermediateDirectory)/src_cairo_SVGCanvasTextCairo.cpp$(ObjectSuffix): src/cairo/SVGCanvasTextCairo.cpp $(IntermediateDirectory)/src_cairo_SVGCanvasTextCairo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/cairo/SVGCanvasTextCairo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_cairo_SVGCanvasTextCairo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_cairo_SVGCanvasTextCairo.cpp$(DependSuffix): src/cairo/SVGCanvasTextCairo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_cairo_SVGCanvasTextCairo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_cairo_SVGCanvasTextCairo.cpp$(DependSuffix) -MM src/cairo/SVGCanvasTextCairo.cpp

$(IntermediateDirectory)/src_cairo_SVGCanvasTextCairo.cpp$(PreprocessSuffix): src/cairo/SVGCanvasTextCairo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_cairo_SVGCanvasTextCairo.cpp$(PreprocessSuffix) src/cairo/SVGCanvasTextCairo.cpp

$(IntermediateDirectory)/src_SVGPathSegList.cpp$(ObjectSuffix): src/SVGPathSegList.cpp $(IntermediateDirectory)/src_SVGPathSegList.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGPathSegList.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGPathSegList.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGPathSegList.cpp$(DependSuffix): src/SVGPathSegList.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGPathSegList.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGPathSegList.cpp$(DependSuffix) -MM src/SVGPathSegList.cpp

$(IntermediateDirectory)/src_SVGPathSegList.cpp$(PreprocessSuffix): src/SVGPathSegList.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGPathSegList.cpp$(PreprocessSuffix) src/SVGPathSegList.cpp

$(IntermediateDirectory)/src_SVGPathElement.cpp$(ObjectSuffix): src/SVGPathElement.cpp $(IntermediateDirectory)/src_SVGPathElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGPathElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGPathElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGPathElement.cpp$(DependSuffix): src/SVGPathElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGPathElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGPathElement.cpp$(DependSuffix) -MM src/SVGPathElement.cpp

$(IntermediateDirectory)/src_SVGPathElement.cpp$(PreprocessSuffix): src/SVGPathElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGPathElement.cpp$(PreprocessSuffix) src/SVGPathElement.cpp

$(IntermediateDirectory)/src_SVGVideoElement.cpp$(ObjectSuffix): src/SVGVideoElement.cpp $(IntermediateDirectory)/src_SVGVideoElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGVideoElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGVideoElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGVideoElement.cpp$(DependSuffix): src/SVGVideoElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGVideoElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGVideoElement.cpp$(DependSuffix) -MM src/SVGVideoElement.cpp

$(IntermediateDirectory)/src_SVGVideoElement.cpp$(PreprocessSuffix): src/SVGVideoElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGVideoElement.cpp$(PreprocessSuffix) src/SVGVideoElement.cpp

$(IntermediateDirectory)/src_SVGLength.cpp$(ObjectSuffix): src/SVGLength.cpp $(IntermediateDirectory)/src_SVGLength.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGLength.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGLength.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGLength.cpp$(DependSuffix): src/SVGLength.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGLength.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGLength.cpp$(DependSuffix) -MM src/SVGLength.cpp

$(IntermediateDirectory)/src_SVGLength.cpp$(PreprocessSuffix): src/SVGLength.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGLength.cpp$(PreprocessSuffix) src/SVGLength.cpp

$(IntermediateDirectory)/src_SVGCanvas.cpp$(ObjectSuffix): src/SVGCanvas.cpp $(IntermediateDirectory)/src_SVGCanvas.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGCanvas.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGCanvas.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGCanvas.cpp$(DependSuffix): src/SVGCanvas.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGCanvas.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGCanvas.cpp$(DependSuffix) -MM src/SVGCanvas.cpp

$(IntermediateDirectory)/src_SVGCanvas.cpp$(PreprocessSuffix): src/SVGCanvas.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGCanvas.cpp$(PreprocessSuffix) src/SVGCanvas.cpp

$(IntermediateDirectory)/src_SVGLengthList.cpp$(ObjectSuffix): src/SVGLengthList.cpp $(IntermediateDirectory)/src_SVGLengthList.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGLengthList.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGLengthList.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGLengthList.cpp$(DependSuffix): src/SVGLengthList.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGLengthList.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGLengthList.cpp$(DependSuffix) -MM src/SVGLengthList.cpp

$(IntermediateDirectory)/src_SVGLengthList.cpp$(PreprocessSuffix): src/SVGLengthList.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGLengthList.cpp$(PreprocessSuffix) src/SVGLengthList.cpp

$(IntermediateDirectory)/src_SVGPreserveAspectRatio.cpp$(ObjectSuffix): src/SVGPreserveAspectRatio.cpp $(IntermediateDirectory)/src_SVGPreserveAspectRatio.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGPreserveAspectRatio.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGPreserveAspectRatio.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGPreserveAspectRatio.cpp$(DependSuffix): src/SVGPreserveAspectRatio.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGPreserveAspectRatio.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGPreserveAspectRatio.cpp$(DependSuffix) -MM src/SVGPreserveAspectRatio.cpp

$(IntermediateDirectory)/src_SVGPreserveAspectRatio.cpp$(PreprocessSuffix): src/SVGPreserveAspectRatio.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGPreserveAspectRatio.cpp$(PreprocessSuffix) src/SVGPreserveAspectRatio.cpp

$(IntermediateDirectory)/src_SVGFitToViewBox.cpp$(ObjectSuffix): src/SVGFitToViewBox.cpp $(IntermediateDirectory)/src_SVGFitToViewBox.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGFitToViewBox.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGFitToViewBox.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGFitToViewBox.cpp$(DependSuffix): src/SVGFitToViewBox.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGFitToViewBox.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGFitToViewBox.cpp$(DependSuffix) -MM src/SVGFitToViewBox.cpp

$(IntermediateDirectory)/src_SVGFitToViewBox.cpp$(PreprocessSuffix): src/SVGFitToViewBox.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGFitToViewBox.cpp$(PreprocessSuffix) src/SVGFitToViewBox.cpp

$(IntermediateDirectory)/src_cairo_SVGCanvasCairo.cpp$(ObjectSuffix): src/cairo/SVGCanvasCairo.cpp $(IntermediateDirectory)/src_cairo_SVGCanvasCairo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/cairo/SVGCanvasCairo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_cairo_SVGCanvasCairo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_cairo_SVGCanvasCairo.cpp$(DependSuffix): src/cairo/SVGCanvasCairo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_cairo_SVGCanvasCairo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_cairo_SVGCanvasCairo.cpp$(DependSuffix) -MM src/cairo/SVGCanvasCairo.cpp

$(IntermediateDirectory)/src_cairo_SVGCanvasCairo.cpp$(PreprocessSuffix): src/cairo/SVGCanvasCairo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_cairo_SVGCanvasCairo.cpp$(PreprocessSuffix) src/cairo/SVGCanvasCairo.cpp

$(IntermediateDirectory)/src_svgxml_svgxmlhelpr.cpp$(ObjectSuffix): src/svgxml/svgxmlhelpr.cpp $(IntermediateDirectory)/src_svgxml_svgxmlhelpr.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/svgxml/svgxmlhelpr.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_svgxml_svgxmlhelpr.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_svgxml_svgxmlhelpr.cpp$(DependSuffix): src/svgxml/svgxmlhelpr.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_svgxml_svgxmlhelpr.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_svgxml_svgxmlhelpr.cpp$(DependSuffix) -MM src/svgxml/svgxmlhelpr.cpp

$(IntermediateDirectory)/src_svgxml_svgxmlhelpr.cpp$(PreprocessSuffix): src/svgxml/svgxmlhelpr.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_svgxml_svgxmlhelpr.cpp$(PreprocessSuffix) src/svgxml/svgxmlhelpr.cpp

$(IntermediateDirectory)/src_SVGPoint.cpp$(ObjectSuffix): src/SVGPoint.cpp $(IntermediateDirectory)/src_SVGPoint.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/SVGPoint.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SVGPoint.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SVGPoint.cpp$(DependSuffix): src/SVGPoint.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SVGPoint.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SVGPoint.cpp$(DependSuffix) -MM src/SVGPoint.cpp

$(IntermediateDirectory)/src_SVGPoint.cpp$(PreprocessSuffix): src/SVGPoint.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SVGPoint.cpp$(PreprocessSuffix) src/SVGPoint.cpp

$(IntermediateDirectory)/src_svgxml_svgxml.cpp$(ObjectSuffix): src/svgxml/svgxml.cpp $(IntermediateDirectory)/src_svgxml_svgxml.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/svgxml/svgxml.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_svgxml_svgxml.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_svgxml_svgxml.cpp$(DependSuffix): src/svgxml/svgxml.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_svgxml_svgxml.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_svgxml_svgxml.cpp$(DependSuffix) -MM src/svgxml/svgxml.cpp

$(IntermediateDirectory)/src_svgxml_svgxml.cpp$(PreprocessSuffix): src/svgxml/svgxml.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_svgxml_svgxml.cpp$(PreprocessSuffix) src/svgxml/svgxml.cpp

$(IntermediateDirectory)/src_css_properties.cpp$(ObjectSuffix): src/css_properties.cpp $(IntermediateDirectory)/src_css_properties.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/css_properties.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_css_properties.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_css_properties.cpp$(DependSuffix): src/css_properties.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_css_properties.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_css_properties.cpp$(DependSuffix) -MM src/css_properties.cpp

$(IntermediateDirectory)/src_css_properties.cpp$(PreprocessSuffix): src/css_properties.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_css_properties.cpp$(PreprocessSuffix) src/css_properties.cpp

$(IntermediateDirectory)/src_CSSValue.cpp$(ObjectSuffix): src/CSSValue.cpp $(IntermediateDirectory)/src_CSSValue.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/CSSValue.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_CSSValue.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_CSSValue.cpp$(DependSuffix): src/CSSValue.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_CSSValue.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_CSSValue.cpp$(DependSuffix) -MM src/CSSValue.cpp

$(IntermediateDirectory)/src_CSSValue.cpp$(PreprocessSuffix): src/CSSValue.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_CSSValue.cpp$(PreprocessSuffix) src/CSSValue.cpp

$(IntermediateDirectory)/src_Elements_CopyConstructors.cpp$(ObjectSuffix): src/Elements_CopyConstructors.cpp $(IntermediateDirectory)/src_Elements_CopyConstructors.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/Elements_CopyConstructors.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Elements_CopyConstructors.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Elements_CopyConstructors.cpp$(DependSuffix): src/Elements_CopyConstructors.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Elements_CopyConstructors.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Elements_CopyConstructors.cpp$(DependSuffix) -MM src/Elements_CopyConstructors.cpp

$(IntermediateDirectory)/src_Elements_CopyConstructors.cpp$(PreprocessSuffix): src/Elements_CopyConstructors.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Elements_CopyConstructors.cpp$(PreprocessSuffix) src/Elements_CopyConstructors.cpp

$(IntermediateDirectory)/src_Elements_SetAttribute.cpp$(ObjectSuffix): src/Elements_SetAttribute.cpp $(IntermediateDirectory)/src_Elements_SetAttribute.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/figuinha/developpement/Regards/Otherlib/wxSVG/src/Elements_SetAttribute.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Elements_SetAttribute.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Elements_SetAttribute.cpp$(DependSuffix): src/Elements_SetAttribute.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Elements_SetAttribute.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Elements_SetAttribute.cpp$(DependSuffix) -MM src/Elements_SetAttribute.cpp

$(IntermediateDirectory)/src_Elements_SetAttribute.cpp$(PreprocessSuffix): src/Elements_SetAttribute.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Elements_SetAttribute.cpp$(PreprocessSuffix) src/Elements_SetAttribute.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/
	$(RM) ../../include/Release/header.h.gch


