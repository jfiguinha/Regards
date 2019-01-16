##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=FaceRecognition_mac
ConfigurationName      :=Release
WorkspacePath          :=/Users/jacques/developpement/codelite/Regards
ProjectPath            :=/Users/jacques/developpement/codelite/Regards/FaceRecognition_mac
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=figuinha jacques
Date                   :=11/01/2019
CodeLitePath           :="/Users/jacques/Library/Application Support/codelite"
LinkerName             :=/usr/local/opt/ccache/bin/ccache /usr/local/opt/llvm/bin/clang++ -stdlib=libc++ -mmacosx-version-min=10.10
SharedObjectLinkerName :=/usr/local/opt/ccache/bin/ccache /usr/local/opt/llvm/bin/clang++ -dynamiclib -fPIC
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
OutputFile             :=$(IntermediateDirectory)/libFaceRecognition.dylib
Preprocessors          :=$(PreprocessorSwitch)DLIB_JPEG_SUPPORT $(PreprocessorSwitch)DLIB_NO_GUI_SUPPORT $(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="FaceRecognition_mac.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -s
IncludePath            := $(IncludeSwitch)/usr/local/opt/cairo/include $(IncludeSwitch)/usr/local/opt/openexr/include/OpenEXR $(IncludeSwitch)/usr/local/opt/ilmbase/include/openexr $(IncludeSwitch)/usr/local/opt/libxml2/include/libxml2 $(IncludeSwitch)/usr/local/opt/libexif/include  $(IncludeSwitch)../libextern/dlib-19.7 $(IncludeSwitch)../FaceRecognition 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)FaceRecognition 
ArLibs                 :=  "FaceRecognition" 
LibPath                :=$(LibraryPathSwitch)/usr/local/opt/llvm/lib $(LibraryPathSwitch)/usr/local/opt/openexr/lib $(LibraryPathSwitch)/usr/local/opt/ilmbase/lib $(LibraryPathSwitch)/usr/local/opt/glib/lib $(LibraryPathSwitch)/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/lib $(LibraryPathSwitch)/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library $(LibraryPathSwitch)/usr/lib/system  $(LibraryPathSwitch). $(LibraryPathSwitch)../FaceRecognition/Release 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/local/opt/ccache/bin/ccache /usr/local/opt/llvm/bin/clang++ -stdlib=libc++ -mmacosx-version-min=10.10 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/
CC       := /usr/local/opt/ccache/bin/ccache /usr/local/opt/llvm/bin/clang -mmacosx-version-min=10.10 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/
CXXFLAGS := -fopenmp -std=gnu++11 -Wall -fpermissive -fPIC -O2 -msse4.1 $(Preprocessors)
CFLAGS   :=  -Wall  -O2 -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := /usr//bin/as


##
## User defined environment variables
##
CodeLiteDir:=/Applications/codelite.app/Contents/SharedSupport/
Objects0=$(IntermediateDirectory)/up_FaceRecognition_FaceRecognition.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(SharedObjectLinkerName) $(OutputSwitch)$(OutputFile) $(Objects) $(LibPath) $(Libs) $(LinkOptions)
	@$(MakeDirCommand) "/Users/jacques/developpement/codelite/Regards/.build-release"
	@echo rebuilt > "/Users/jacques/developpement/codelite/Regards/.build-release/FaceRecognition_mac"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


$(IntermediateDirectory)/.d:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_FaceRecognition_FaceRecognition.cpp$(ObjectSuffix): ../FaceRecognition/FaceRecognition.cpp $(IntermediateDirectory)/up_FaceRecognition_FaceRecognition.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/codelite/Regards/FaceRecognition/FaceRecognition.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_FaceRecognition_FaceRecognition.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_FaceRecognition_FaceRecognition.cpp$(DependSuffix): ../FaceRecognition/FaceRecognition.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_FaceRecognition_FaceRecognition.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_FaceRecognition_FaceRecognition.cpp$(DependSuffix) -MM ../FaceRecognition/FaceRecognition.cpp

$(IntermediateDirectory)/up_FaceRecognition_FaceRecognition.cpp$(PreprocessSuffix): ../FaceRecognition/FaceRecognition.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_FaceRecognition_FaceRecognition.cpp$(PreprocessSuffix) ../FaceRecognition/FaceRecognition.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


