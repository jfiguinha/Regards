##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=libAppleFunction
ConfigurationName      :=Release
WorkspacePath          :=/Users/jacques/developpement/codelite/Regards
ProjectPath            :=/Users/jacques/developpement/codelite/Regards/libAppleFunction
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
OutputFile             :=$(IntermediateDirectory)/$(ProjectName).a
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="libAppleFunction.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)/usr/local/opt/cairo/include $(IncludeSwitch)/usr/local/opt/openexr/include/OpenEXR $(IncludeSwitch)/usr/local/opt/ilmbase/include/openexr $(IncludeSwitch)/usr/local/opt/libxml2/include/libxml2 $(IncludeSwitch)/usr/local/opt/libexif/include  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                :=$(LibraryPathSwitch)/usr/local/opt/llvm/lib $(LibraryPathSwitch)/usr/local/opt/openexr/lib $(LibraryPathSwitch)/usr/local/opt/ilmbase/lib $(LibraryPathSwitch)/usr/local/opt/glib/lib $(LibraryPathSwitch)/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/lib $(LibraryPathSwitch)/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library $(LibraryPathSwitch)/usr/lib/system  $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/local/opt/ccache/bin/ccache /usr/local/opt/llvm/bin/clang++ -stdlib=libc++ -mmacosx-version-min=10.10 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/
CC       := /usr/local/opt/ccache/bin/ccache /usr/local/opt/llvm/bin/clang -mmacosx-version-min=10.10 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/
CXXFLAGS := $(shell ../libextern/wxWidgets-master/wx-config --cppflags) -Wall -pthread -fopenmp -std=gnu++11 -fpermissive -Os -fopenmp -Wall $(Preprocessors)
CFLAGS   := -fopenmp $(shell ../libextern/wxWidgets-master/wx-config --cflags) -fpermissive -Os -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr//bin/as


##
## User defined environment variables
##
CodeLiteDir:=/Applications/codelite.app/Contents/SharedSupport/
Objects0=$(IntermediateDirectory)/SendEmail.cpp$(ObjectSuffix) $(IntermediateDirectory)/SaveFileFormat.cpp$(ObjectSuffix) $(IntermediateDirectory)/SaveFromCFunction.mm$(ObjectSuffix) $(IntermediateDirectory)/MyEmail.mm$(ObjectSuffix) 



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
	$(AR) $(ArchiveOutputSwitch)$(OutputFile) $(Objects) $(ArLibs)
	@$(MakeDirCommand) "/Users/jacques/developpement/codelite/Regards/.build-release"
	@echo rebuilt > "/Users/jacques/developpement/codelite/Regards/.build-release/libAppleFunction"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


./Release:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/SendEmail.cpp$(ObjectSuffix): SendEmail.cpp $(IntermediateDirectory)/SendEmail.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/codelite/Regards/libAppleFunction/SendEmail.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SendEmail.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SendEmail.cpp$(DependSuffix): SendEmail.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SendEmail.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SendEmail.cpp$(DependSuffix) -MM SendEmail.cpp

$(IntermediateDirectory)/SendEmail.cpp$(PreprocessSuffix): SendEmail.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SendEmail.cpp$(PreprocessSuffix) SendEmail.cpp

$(IntermediateDirectory)/SaveFileFormat.cpp$(ObjectSuffix): SaveFileFormat.cpp $(IntermediateDirectory)/SaveFileFormat.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/jacques/developpement/codelite/Regards/libAppleFunction/SaveFileFormat.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SaveFileFormat.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SaveFileFormat.cpp$(DependSuffix): SaveFileFormat.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SaveFileFormat.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SaveFileFormat.cpp$(DependSuffix) -MM SaveFileFormat.cpp

$(IntermediateDirectory)/SaveFileFormat.cpp$(PreprocessSuffix): SaveFileFormat.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SaveFileFormat.cpp$(PreprocessSuffix) SaveFileFormat.cpp

$(IntermediateDirectory)/SaveFromCFunction.mm$(ObjectSuffix): SaveFromCFunction.mm $(IntermediateDirectory)/SaveFromCFunction.mm$(DependSuffix)
	$(CXX) $(IncludePCH) -x objective-c++ $(SourceSwitch) "/Users/jacques/developpement/codelite/Regards/libAppleFunction/SaveFromCFunction.mm" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SaveFromCFunction.mm$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SaveFromCFunction.mm$(DependSuffix): SaveFromCFunction.mm
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SaveFromCFunction.mm$(ObjectSuffix) -MF$(IntermediateDirectory)/SaveFromCFunction.mm$(DependSuffix) -MM SaveFromCFunction.mm

$(IntermediateDirectory)/SaveFromCFunction.mm$(PreprocessSuffix): SaveFromCFunction.mm
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SaveFromCFunction.mm$(PreprocessSuffix) SaveFromCFunction.mm

$(IntermediateDirectory)/MyEmail.mm$(ObjectSuffix): MyEmail.mm $(IntermediateDirectory)/MyEmail.mm$(DependSuffix)
	$(CXX) $(IncludePCH) -x objective-c++ $(SourceSwitch) "/Users/jacques/developpement/codelite/Regards/libAppleFunction/MyEmail.mm" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MyEmail.mm$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MyEmail.mm$(DependSuffix): MyEmail.mm
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MyEmail.mm$(ObjectSuffix) -MF$(IntermediateDirectory)/MyEmail.mm$(DependSuffix) -MM MyEmail.mm

$(IntermediateDirectory)/MyEmail.mm$(PreprocessSuffix): MyEmail.mm
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MyEmail.mm$(PreprocessSuffix) MyEmail.mm


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


