##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=wxPDFDoc
ConfigurationName      :=Release
WorkspacePath          :=C:/developpement/git/Regards
ProjectPath            :=C:/developpement/git/Regards/Otherlib/wxPDFDoc
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=jfigu
Date                   :=27/09/2019
CodeLitePath           :="C:/Program Files/CodeLite"
LinkerName             :=C:/msys64/mingw64/bin/g++.exe
SharedObjectLinkerName :=C:/msys64/mingw64/bin/g++.exe -shared -fPIC
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
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="wxPDFDoc.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/msys64/mingw64/bin/windres.exe
LinkOptions            :=   $(shell wx-config --libs --debug=no --unicode=yes)
IncludePath            := $(IncludeSwitch)/mingw64/include/OpenEXR $(IncludeSwitch)/mingw64/include/poppler $(IncludeSwitch)/mingw64/include/poppler/cpp $(IncludeSwitch)/mingw64/include/qpdf $(IncludeSwitch)/mingw64/x86_64-w64-mingw32/include/gdiplus  $(IncludeSwitch). $(IncludeSwitch)../../include $(IncludeSwitch)include $(IncludeSwitch)../../libextern/poppler_windows-master/cpp $(IncludeSwitch). 
IncludePCH             :=  -include ../../include/Release/header.h 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/msys64/mingw64/bin/ar.exe rcu
CXX      := C:/msys64/mingw64/bin/g++.exe
CC       := C:/msys64/mingw64/bin/gcc.exe
CXXFLAGS := -fopenmp -Wall -std=gnu++11 $(shell ../../libextern/wxWidgets-master/wx-config --cppflags) -pthread -Wno-narrowing -fpermissive -O2 -msse4.1 $(Preprocessors)
CFLAGS   :=  -O2 -msse4.1 $(Preprocessors)
ASFLAGS  := 
AS       := C:/msys64/mingw64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
PATH:=C:\msys64\usr\bin;$PATH
WXWIN:=C:\developpement\git\Regards\libextern\wxWidgets-master
WXCFG:=lib/mswud
Objects0=$(IntermediateDirectory)/src_pdfannotation.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdfbarcode.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdfcffindex.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdfdc.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdfdocument.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdfencoding.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdfencrypt.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdffont.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdffontdata.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdffontdatacore.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_pdffontdataopentype.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdffontdatatruetype.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdffontdatatype1.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdffontdescription.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdffontdetails.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdffontextended.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdffontmanager.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdffontparser.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdffontdatatype0.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdffontparsertruetype.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_pdfdecode.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdfocg.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdfgradient.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdfparser.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdfrijndael.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdffontsubsettruetype.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_wxpoppler.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdfform.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdftemplate.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdffontsubsetcff.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_pdfcolour.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdfobjects.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdfcffdecoder.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdffontvolt.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdfxml.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdfutility.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdfprint.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdffontparsertype1.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdfpattern.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdflayer.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_pdfkernel.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_pdfimage.cpp$(ObjectSuffix) 

Objects1=$(IntermediateDirectory)/src_pdfgraphics.cpp$(ObjectSuffix) 



Objects=$(Objects0) $(Objects1) 

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
	$(AR) $(ArchiveOutputSwitch)$(OutputFile) @$(ObjectsFileList) $(ArLibs)
	@$(MakeDirCommand) "C:\developpement\git\Regards/.build-release"
	@echo rebuilt > "C:\developpement\git\Regards/.build-release/wxPDFDoc"

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Release"


./Release:
	@$(MakeDirCommand) "./Release"

PreBuild:

# PreCompiled Header
../../include/Release/header.h.gch: ../../include/Release/header.h
	$(CXX) $(SourceSwitch) ../../include/Release/header.h $(PCHCompileFlags) $(CXXFLAGS) $(IncludePath)



##
## Objects
##
$(IntermediateDirectory)/src_pdfannotation.cpp$(ObjectSuffix): src/pdfannotation.cpp $(IntermediateDirectory)/src_pdfannotation.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfannotation.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfannotation.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfannotation.cpp$(DependSuffix): src/pdfannotation.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdfannotation.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdfannotation.cpp$(DependSuffix) -MM src/pdfannotation.cpp

$(IntermediateDirectory)/src_pdfannotation.cpp$(PreprocessSuffix): src/pdfannotation.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfannotation.cpp$(PreprocessSuffix) src/pdfannotation.cpp

$(IntermediateDirectory)/src_pdfbarcode.cpp$(ObjectSuffix): src/pdfbarcode.cpp $(IntermediateDirectory)/src_pdfbarcode.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfbarcode.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfbarcode.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfbarcode.cpp$(DependSuffix): src/pdfbarcode.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdfbarcode.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdfbarcode.cpp$(DependSuffix) -MM src/pdfbarcode.cpp

$(IntermediateDirectory)/src_pdfbarcode.cpp$(PreprocessSuffix): src/pdfbarcode.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfbarcode.cpp$(PreprocessSuffix) src/pdfbarcode.cpp

$(IntermediateDirectory)/src_pdfcffindex.cpp$(ObjectSuffix): src/pdfcffindex.cpp $(IntermediateDirectory)/src_pdfcffindex.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfcffindex.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfcffindex.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfcffindex.cpp$(DependSuffix): src/pdfcffindex.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdfcffindex.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdfcffindex.cpp$(DependSuffix) -MM src/pdfcffindex.cpp

$(IntermediateDirectory)/src_pdfcffindex.cpp$(PreprocessSuffix): src/pdfcffindex.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfcffindex.cpp$(PreprocessSuffix) src/pdfcffindex.cpp

$(IntermediateDirectory)/src_pdfdc.cpp$(ObjectSuffix): src/pdfdc.cpp $(IntermediateDirectory)/src_pdfdc.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfdc.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfdc.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfdc.cpp$(DependSuffix): src/pdfdc.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdfdc.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdfdc.cpp$(DependSuffix) -MM src/pdfdc.cpp

$(IntermediateDirectory)/src_pdfdc.cpp$(PreprocessSuffix): src/pdfdc.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfdc.cpp$(PreprocessSuffix) src/pdfdc.cpp

$(IntermediateDirectory)/src_pdfdocument.cpp$(ObjectSuffix): src/pdfdocument.cpp $(IntermediateDirectory)/src_pdfdocument.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfdocument.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfdocument.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfdocument.cpp$(DependSuffix): src/pdfdocument.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdfdocument.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdfdocument.cpp$(DependSuffix) -MM src/pdfdocument.cpp

$(IntermediateDirectory)/src_pdfdocument.cpp$(PreprocessSuffix): src/pdfdocument.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfdocument.cpp$(PreprocessSuffix) src/pdfdocument.cpp

$(IntermediateDirectory)/src_pdfencoding.cpp$(ObjectSuffix): src/pdfencoding.cpp $(IntermediateDirectory)/src_pdfencoding.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfencoding.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfencoding.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfencoding.cpp$(DependSuffix): src/pdfencoding.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdfencoding.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdfencoding.cpp$(DependSuffix) -MM src/pdfencoding.cpp

$(IntermediateDirectory)/src_pdfencoding.cpp$(PreprocessSuffix): src/pdfencoding.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfencoding.cpp$(PreprocessSuffix) src/pdfencoding.cpp

$(IntermediateDirectory)/src_pdfencrypt.cpp$(ObjectSuffix): src/pdfencrypt.cpp $(IntermediateDirectory)/src_pdfencrypt.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfencrypt.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfencrypt.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfencrypt.cpp$(DependSuffix): src/pdfencrypt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdfencrypt.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdfencrypt.cpp$(DependSuffix) -MM src/pdfencrypt.cpp

$(IntermediateDirectory)/src_pdfencrypt.cpp$(PreprocessSuffix): src/pdfencrypt.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfencrypt.cpp$(PreprocessSuffix) src/pdfencrypt.cpp

$(IntermediateDirectory)/src_pdffont.cpp$(ObjectSuffix): src/pdffont.cpp $(IntermediateDirectory)/src_pdffont.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffont.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffont.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffont.cpp$(DependSuffix): src/pdffont.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdffont.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdffont.cpp$(DependSuffix) -MM src/pdffont.cpp

$(IntermediateDirectory)/src_pdffont.cpp$(PreprocessSuffix): src/pdffont.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffont.cpp$(PreprocessSuffix) src/pdffont.cpp

$(IntermediateDirectory)/src_pdffontdata.cpp$(ObjectSuffix): src/pdffontdata.cpp $(IntermediateDirectory)/src_pdffontdata.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontdata.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontdata.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontdata.cpp$(DependSuffix): src/pdffontdata.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdffontdata.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdffontdata.cpp$(DependSuffix) -MM src/pdffontdata.cpp

$(IntermediateDirectory)/src_pdffontdata.cpp$(PreprocessSuffix): src/pdffontdata.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontdata.cpp$(PreprocessSuffix) src/pdffontdata.cpp

$(IntermediateDirectory)/src_pdffontdatacore.cpp$(ObjectSuffix): src/pdffontdatacore.cpp $(IntermediateDirectory)/src_pdffontdatacore.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontdatacore.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontdatacore.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontdatacore.cpp$(DependSuffix): src/pdffontdatacore.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdffontdatacore.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdffontdatacore.cpp$(DependSuffix) -MM src/pdffontdatacore.cpp

$(IntermediateDirectory)/src_pdffontdatacore.cpp$(PreprocessSuffix): src/pdffontdatacore.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontdatacore.cpp$(PreprocessSuffix) src/pdffontdatacore.cpp

$(IntermediateDirectory)/src_pdffontdataopentype.cpp$(ObjectSuffix): src/pdffontdataopentype.cpp $(IntermediateDirectory)/src_pdffontdataopentype.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontdataopentype.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontdataopentype.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontdataopentype.cpp$(DependSuffix): src/pdffontdataopentype.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdffontdataopentype.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdffontdataopentype.cpp$(DependSuffix) -MM src/pdffontdataopentype.cpp

$(IntermediateDirectory)/src_pdffontdataopentype.cpp$(PreprocessSuffix): src/pdffontdataopentype.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontdataopentype.cpp$(PreprocessSuffix) src/pdffontdataopentype.cpp

$(IntermediateDirectory)/src_pdffontdatatruetype.cpp$(ObjectSuffix): src/pdffontdatatruetype.cpp $(IntermediateDirectory)/src_pdffontdatatruetype.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontdatatruetype.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontdatatruetype.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontdatatruetype.cpp$(DependSuffix): src/pdffontdatatruetype.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdffontdatatruetype.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdffontdatatruetype.cpp$(DependSuffix) -MM src/pdffontdatatruetype.cpp

$(IntermediateDirectory)/src_pdffontdatatruetype.cpp$(PreprocessSuffix): src/pdffontdatatruetype.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontdatatruetype.cpp$(PreprocessSuffix) src/pdffontdatatruetype.cpp

$(IntermediateDirectory)/src_pdffontdatatype1.cpp$(ObjectSuffix): src/pdffontdatatype1.cpp $(IntermediateDirectory)/src_pdffontdatatype1.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontdatatype1.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontdatatype1.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontdatatype1.cpp$(DependSuffix): src/pdffontdatatype1.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdffontdatatype1.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdffontdatatype1.cpp$(DependSuffix) -MM src/pdffontdatatype1.cpp

$(IntermediateDirectory)/src_pdffontdatatype1.cpp$(PreprocessSuffix): src/pdffontdatatype1.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontdatatype1.cpp$(PreprocessSuffix) src/pdffontdatatype1.cpp

$(IntermediateDirectory)/src_pdffontdescription.cpp$(ObjectSuffix): src/pdffontdescription.cpp $(IntermediateDirectory)/src_pdffontdescription.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontdescription.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontdescription.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontdescription.cpp$(DependSuffix): src/pdffontdescription.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdffontdescription.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdffontdescription.cpp$(DependSuffix) -MM src/pdffontdescription.cpp

$(IntermediateDirectory)/src_pdffontdescription.cpp$(PreprocessSuffix): src/pdffontdescription.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontdescription.cpp$(PreprocessSuffix) src/pdffontdescription.cpp

$(IntermediateDirectory)/src_pdffontdetails.cpp$(ObjectSuffix): src/pdffontdetails.cpp $(IntermediateDirectory)/src_pdffontdetails.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontdetails.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontdetails.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontdetails.cpp$(DependSuffix): src/pdffontdetails.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdffontdetails.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdffontdetails.cpp$(DependSuffix) -MM src/pdffontdetails.cpp

$(IntermediateDirectory)/src_pdffontdetails.cpp$(PreprocessSuffix): src/pdffontdetails.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontdetails.cpp$(PreprocessSuffix) src/pdffontdetails.cpp

$(IntermediateDirectory)/src_pdffontextended.cpp$(ObjectSuffix): src/pdffontextended.cpp $(IntermediateDirectory)/src_pdffontextended.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontextended.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontextended.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontextended.cpp$(DependSuffix): src/pdffontextended.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdffontextended.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdffontextended.cpp$(DependSuffix) -MM src/pdffontextended.cpp

$(IntermediateDirectory)/src_pdffontextended.cpp$(PreprocessSuffix): src/pdffontextended.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontextended.cpp$(PreprocessSuffix) src/pdffontextended.cpp

$(IntermediateDirectory)/src_pdffontmanager.cpp$(ObjectSuffix): src/pdffontmanager.cpp $(IntermediateDirectory)/src_pdffontmanager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontmanager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontmanager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontmanager.cpp$(DependSuffix): src/pdffontmanager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdffontmanager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdffontmanager.cpp$(DependSuffix) -MM src/pdffontmanager.cpp

$(IntermediateDirectory)/src_pdffontmanager.cpp$(PreprocessSuffix): src/pdffontmanager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontmanager.cpp$(PreprocessSuffix) src/pdffontmanager.cpp

$(IntermediateDirectory)/src_pdffontparser.cpp$(ObjectSuffix): src/pdffontparser.cpp $(IntermediateDirectory)/src_pdffontparser.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontparser.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontparser.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontparser.cpp$(DependSuffix): src/pdffontparser.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdffontparser.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdffontparser.cpp$(DependSuffix) -MM src/pdffontparser.cpp

$(IntermediateDirectory)/src_pdffontparser.cpp$(PreprocessSuffix): src/pdffontparser.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontparser.cpp$(PreprocessSuffix) src/pdffontparser.cpp

$(IntermediateDirectory)/src_pdffontdatatype0.cpp$(ObjectSuffix): src/pdffontdatatype0.cpp $(IntermediateDirectory)/src_pdffontdatatype0.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontdatatype0.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontdatatype0.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontdatatype0.cpp$(DependSuffix): src/pdffontdatatype0.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdffontdatatype0.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdffontdatatype0.cpp$(DependSuffix) -MM src/pdffontdatatype0.cpp

$(IntermediateDirectory)/src_pdffontdatatype0.cpp$(PreprocessSuffix): src/pdffontdatatype0.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontdatatype0.cpp$(PreprocessSuffix) src/pdffontdatatype0.cpp

$(IntermediateDirectory)/src_pdffontparsertruetype.cpp$(ObjectSuffix): src/pdffontparsertruetype.cpp $(IntermediateDirectory)/src_pdffontparsertruetype.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontparsertruetype.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontparsertruetype.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontparsertruetype.cpp$(DependSuffix): src/pdffontparsertruetype.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdffontparsertruetype.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdffontparsertruetype.cpp$(DependSuffix) -MM src/pdffontparsertruetype.cpp

$(IntermediateDirectory)/src_pdffontparsertruetype.cpp$(PreprocessSuffix): src/pdffontparsertruetype.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontparsertruetype.cpp$(PreprocessSuffix) src/pdffontparsertruetype.cpp

$(IntermediateDirectory)/src_pdfdecode.cpp$(ObjectSuffix): src/pdfdecode.cpp $(IntermediateDirectory)/src_pdfdecode.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfdecode.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfdecode.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfdecode.cpp$(DependSuffix): src/pdfdecode.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdfdecode.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdfdecode.cpp$(DependSuffix) -MM src/pdfdecode.cpp

$(IntermediateDirectory)/src_pdfdecode.cpp$(PreprocessSuffix): src/pdfdecode.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfdecode.cpp$(PreprocessSuffix) src/pdfdecode.cpp

$(IntermediateDirectory)/src_pdfocg.cpp$(ObjectSuffix): src/pdfocg.cpp $(IntermediateDirectory)/src_pdfocg.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfocg.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfocg.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfocg.cpp$(DependSuffix): src/pdfocg.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdfocg.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdfocg.cpp$(DependSuffix) -MM src/pdfocg.cpp

$(IntermediateDirectory)/src_pdfocg.cpp$(PreprocessSuffix): src/pdfocg.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfocg.cpp$(PreprocessSuffix) src/pdfocg.cpp

$(IntermediateDirectory)/src_pdfgradient.cpp$(ObjectSuffix): src/pdfgradient.cpp $(IntermediateDirectory)/src_pdfgradient.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfgradient.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfgradient.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfgradient.cpp$(DependSuffix): src/pdfgradient.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdfgradient.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdfgradient.cpp$(DependSuffix) -MM src/pdfgradient.cpp

$(IntermediateDirectory)/src_pdfgradient.cpp$(PreprocessSuffix): src/pdfgradient.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfgradient.cpp$(PreprocessSuffix) src/pdfgradient.cpp

$(IntermediateDirectory)/src_pdfparser.cpp$(ObjectSuffix): src/pdfparser.cpp $(IntermediateDirectory)/src_pdfparser.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfparser.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfparser.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfparser.cpp$(DependSuffix): src/pdfparser.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdfparser.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdfparser.cpp$(DependSuffix) -MM src/pdfparser.cpp

$(IntermediateDirectory)/src_pdfparser.cpp$(PreprocessSuffix): src/pdfparser.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfparser.cpp$(PreprocessSuffix) src/pdfparser.cpp

$(IntermediateDirectory)/src_pdfrijndael.cpp$(ObjectSuffix): src/pdfrijndael.cpp $(IntermediateDirectory)/src_pdfrijndael.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfrijndael.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfrijndael.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfrijndael.cpp$(DependSuffix): src/pdfrijndael.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdfrijndael.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdfrijndael.cpp$(DependSuffix) -MM src/pdfrijndael.cpp

$(IntermediateDirectory)/src_pdfrijndael.cpp$(PreprocessSuffix): src/pdfrijndael.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfrijndael.cpp$(PreprocessSuffix) src/pdfrijndael.cpp

$(IntermediateDirectory)/src_pdffontsubsettruetype.cpp$(ObjectSuffix): src/pdffontsubsettruetype.cpp $(IntermediateDirectory)/src_pdffontsubsettruetype.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontsubsettruetype.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontsubsettruetype.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontsubsettruetype.cpp$(DependSuffix): src/pdffontsubsettruetype.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdffontsubsettruetype.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdffontsubsettruetype.cpp$(DependSuffix) -MM src/pdffontsubsettruetype.cpp

$(IntermediateDirectory)/src_pdffontsubsettruetype.cpp$(PreprocessSuffix): src/pdffontsubsettruetype.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontsubsettruetype.cpp$(PreprocessSuffix) src/pdffontsubsettruetype.cpp

$(IntermediateDirectory)/src_wxpoppler.cpp$(ObjectSuffix): src/wxpoppler.cpp $(IntermediateDirectory)/src_wxpoppler.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/wxpoppler.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_wxpoppler.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_wxpoppler.cpp$(DependSuffix): src/wxpoppler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_wxpoppler.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_wxpoppler.cpp$(DependSuffix) -MM src/wxpoppler.cpp

$(IntermediateDirectory)/src_wxpoppler.cpp$(PreprocessSuffix): src/wxpoppler.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_wxpoppler.cpp$(PreprocessSuffix) src/wxpoppler.cpp

$(IntermediateDirectory)/src_pdfform.cpp$(ObjectSuffix): src/pdfform.cpp $(IntermediateDirectory)/src_pdfform.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfform.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfform.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfform.cpp$(DependSuffix): src/pdfform.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdfform.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdfform.cpp$(DependSuffix) -MM src/pdfform.cpp

$(IntermediateDirectory)/src_pdfform.cpp$(PreprocessSuffix): src/pdfform.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfform.cpp$(PreprocessSuffix) src/pdfform.cpp

$(IntermediateDirectory)/src_pdftemplate.cpp$(ObjectSuffix): src/pdftemplate.cpp $(IntermediateDirectory)/src_pdftemplate.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdftemplate.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdftemplate.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdftemplate.cpp$(DependSuffix): src/pdftemplate.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdftemplate.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdftemplate.cpp$(DependSuffix) -MM src/pdftemplate.cpp

$(IntermediateDirectory)/src_pdftemplate.cpp$(PreprocessSuffix): src/pdftemplate.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdftemplate.cpp$(PreprocessSuffix) src/pdftemplate.cpp

$(IntermediateDirectory)/src_pdffontsubsetcff.cpp$(ObjectSuffix): src/pdffontsubsetcff.cpp $(IntermediateDirectory)/src_pdffontsubsetcff.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontsubsetcff.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontsubsetcff.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontsubsetcff.cpp$(DependSuffix): src/pdffontsubsetcff.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdffontsubsetcff.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdffontsubsetcff.cpp$(DependSuffix) -MM src/pdffontsubsetcff.cpp

$(IntermediateDirectory)/src_pdffontsubsetcff.cpp$(PreprocessSuffix): src/pdffontsubsetcff.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontsubsetcff.cpp$(PreprocessSuffix) src/pdffontsubsetcff.cpp

$(IntermediateDirectory)/src_pdfcolour.cpp$(ObjectSuffix): src/pdfcolour.cpp $(IntermediateDirectory)/src_pdfcolour.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfcolour.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfcolour.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfcolour.cpp$(DependSuffix): src/pdfcolour.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdfcolour.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdfcolour.cpp$(DependSuffix) -MM src/pdfcolour.cpp

$(IntermediateDirectory)/src_pdfcolour.cpp$(PreprocessSuffix): src/pdfcolour.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfcolour.cpp$(PreprocessSuffix) src/pdfcolour.cpp

$(IntermediateDirectory)/src_pdfobjects.cpp$(ObjectSuffix): src/pdfobjects.cpp $(IntermediateDirectory)/src_pdfobjects.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfobjects.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfobjects.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfobjects.cpp$(DependSuffix): src/pdfobjects.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdfobjects.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdfobjects.cpp$(DependSuffix) -MM src/pdfobjects.cpp

$(IntermediateDirectory)/src_pdfobjects.cpp$(PreprocessSuffix): src/pdfobjects.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfobjects.cpp$(PreprocessSuffix) src/pdfobjects.cpp

$(IntermediateDirectory)/src_pdfcffdecoder.cpp$(ObjectSuffix): src/pdfcffdecoder.cpp $(IntermediateDirectory)/src_pdfcffdecoder.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfcffdecoder.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfcffdecoder.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfcffdecoder.cpp$(DependSuffix): src/pdfcffdecoder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdfcffdecoder.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdfcffdecoder.cpp$(DependSuffix) -MM src/pdfcffdecoder.cpp

$(IntermediateDirectory)/src_pdfcffdecoder.cpp$(PreprocessSuffix): src/pdfcffdecoder.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfcffdecoder.cpp$(PreprocessSuffix) src/pdfcffdecoder.cpp

$(IntermediateDirectory)/src_pdffontvolt.cpp$(ObjectSuffix): src/pdffontvolt.cpp $(IntermediateDirectory)/src_pdffontvolt.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontvolt.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontvolt.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontvolt.cpp$(DependSuffix): src/pdffontvolt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdffontvolt.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdffontvolt.cpp$(DependSuffix) -MM src/pdffontvolt.cpp

$(IntermediateDirectory)/src_pdffontvolt.cpp$(PreprocessSuffix): src/pdffontvolt.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontvolt.cpp$(PreprocessSuffix) src/pdffontvolt.cpp

$(IntermediateDirectory)/src_pdfxml.cpp$(ObjectSuffix): src/pdfxml.cpp $(IntermediateDirectory)/src_pdfxml.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfxml.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfxml.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfxml.cpp$(DependSuffix): src/pdfxml.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdfxml.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdfxml.cpp$(DependSuffix) -MM src/pdfxml.cpp

$(IntermediateDirectory)/src_pdfxml.cpp$(PreprocessSuffix): src/pdfxml.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfxml.cpp$(PreprocessSuffix) src/pdfxml.cpp

$(IntermediateDirectory)/src_pdfutility.cpp$(ObjectSuffix): src/pdfutility.cpp $(IntermediateDirectory)/src_pdfutility.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfutility.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfutility.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfutility.cpp$(DependSuffix): src/pdfutility.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdfutility.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdfutility.cpp$(DependSuffix) -MM src/pdfutility.cpp

$(IntermediateDirectory)/src_pdfutility.cpp$(PreprocessSuffix): src/pdfutility.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfutility.cpp$(PreprocessSuffix) src/pdfutility.cpp

$(IntermediateDirectory)/src_pdfprint.cpp$(ObjectSuffix): src/pdfprint.cpp $(IntermediateDirectory)/src_pdfprint.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfprint.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfprint.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfprint.cpp$(DependSuffix): src/pdfprint.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdfprint.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdfprint.cpp$(DependSuffix) -MM src/pdfprint.cpp

$(IntermediateDirectory)/src_pdfprint.cpp$(PreprocessSuffix): src/pdfprint.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfprint.cpp$(PreprocessSuffix) src/pdfprint.cpp

$(IntermediateDirectory)/src_pdffontparsertype1.cpp$(ObjectSuffix): src/pdffontparsertype1.cpp $(IntermediateDirectory)/src_pdffontparsertype1.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontparsertype1.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontparsertype1.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontparsertype1.cpp$(DependSuffix): src/pdffontparsertype1.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdffontparsertype1.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdffontparsertype1.cpp$(DependSuffix) -MM src/pdffontparsertype1.cpp

$(IntermediateDirectory)/src_pdffontparsertype1.cpp$(PreprocessSuffix): src/pdffontparsertype1.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontparsertype1.cpp$(PreprocessSuffix) src/pdffontparsertype1.cpp

$(IntermediateDirectory)/src_pdfpattern.cpp$(ObjectSuffix): src/pdfpattern.cpp $(IntermediateDirectory)/src_pdfpattern.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfpattern.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfpattern.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfpattern.cpp$(DependSuffix): src/pdfpattern.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdfpattern.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdfpattern.cpp$(DependSuffix) -MM src/pdfpattern.cpp

$(IntermediateDirectory)/src_pdfpattern.cpp$(PreprocessSuffix): src/pdfpattern.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfpattern.cpp$(PreprocessSuffix) src/pdfpattern.cpp

$(IntermediateDirectory)/src_pdflayer.cpp$(ObjectSuffix): src/pdflayer.cpp $(IntermediateDirectory)/src_pdflayer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdflayer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdflayer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdflayer.cpp$(DependSuffix): src/pdflayer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdflayer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdflayer.cpp$(DependSuffix) -MM src/pdflayer.cpp

$(IntermediateDirectory)/src_pdflayer.cpp$(PreprocessSuffix): src/pdflayer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdflayer.cpp$(PreprocessSuffix) src/pdflayer.cpp

$(IntermediateDirectory)/src_pdfkernel.cpp$(ObjectSuffix): src/pdfkernel.cpp $(IntermediateDirectory)/src_pdfkernel.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfkernel.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfkernel.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfkernel.cpp$(DependSuffix): src/pdfkernel.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdfkernel.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdfkernel.cpp$(DependSuffix) -MM src/pdfkernel.cpp

$(IntermediateDirectory)/src_pdfkernel.cpp$(PreprocessSuffix): src/pdfkernel.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfkernel.cpp$(PreprocessSuffix) src/pdfkernel.cpp

$(IntermediateDirectory)/src_pdfimage.cpp$(ObjectSuffix): src/pdfimage.cpp $(IntermediateDirectory)/src_pdfimage.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfimage.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfimage.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfimage.cpp$(DependSuffix): src/pdfimage.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdfimage.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdfimage.cpp$(DependSuffix) -MM src/pdfimage.cpp

$(IntermediateDirectory)/src_pdfimage.cpp$(PreprocessSuffix): src/pdfimage.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfimage.cpp$(PreprocessSuffix) src/pdfimage.cpp

$(IntermediateDirectory)/src_pdfgraphics.cpp$(ObjectSuffix): src/pdfgraphics.cpp $(IntermediateDirectory)/src_pdfgraphics.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfgraphics.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfgraphics.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfgraphics.cpp$(DependSuffix): src/pdfgraphics.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_pdfgraphics.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_pdfgraphics.cpp$(DependSuffix) -MM src/pdfgraphics.cpp

$(IntermediateDirectory)/src_pdfgraphics.cpp$(PreprocessSuffix): src/pdfgraphics.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfgraphics.cpp$(PreprocessSuffix) src/pdfgraphics.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/
	$(RM) ../../include/Release/header.h.gch


