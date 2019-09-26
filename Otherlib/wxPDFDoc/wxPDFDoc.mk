##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=wxPDFDoc
ConfigurationName      :=Debug
WorkspacePath          :=C:/developpement/git/Regards
ProjectPath            :=C:/developpement/git/Regards/Otherlib/wxPDFDoc
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=jfigu_000
Date                   :=26/09/2019
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
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="wxPDFDoc.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/msys64/mingw64/bin/windres.exe
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)"C:\Program Files (x86)\AMD APP SDK\3.0_mingw\include" $(IncludeSwitch)/mingw64/include/OpenEXR $(IncludeSwitch)/mingw64/x86_64-w64-mingw32/include/gdiplus  $(IncludeSwitch). $(IncludeSwitch)../../include $(IncludeSwitch)include $(IncludeSwitch)../../libextern/poppler_windows-master/cpp $(IncludeSwitch). 
IncludePCH             :=  -include ../../include/Debug/header.h 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                :=$(LibraryPathSwitch)"C:\Program Files (x86)\AMD APP SDK\3.0_mingw\lib\x86_64"  $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/msys64/mingw64/bin/ar.exe rcu
CXX      := C:/msys64/mingw64/bin/g++.exe
CC       := C:/msys64/mingw64/bin/gcc.exe
CXXFLAGS := -fopenmp -Wall -std=gnu++11 $(shell ../../libextern/wxWidgets-master/wx-config --cppflags) -pthread -Wno-narrowing -fpermissive -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := C:/msys64/mingw64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
PATH:=C:\msys64\usr\bin;$PATH
WXWIN:=C:\Regards\libextern\wxwidgets-3.1.2
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
	@$(MakeDirCommand) "C:\developpement\git\Regards/.build-debug"
	@echo rebuilt > "C:\developpement\git\Regards/.build-debug/wxPDFDoc"

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Debug"


./Debug:
	@$(MakeDirCommand) "./Debug"

PreBuild:

# PreCompiled Header
../../include/Debug/header.h.gch: ../../include/Debug/header.h
	$(CXX) $(SourceSwitch) ../../include/Debug/header.h $(PCHCompileFlags) $(CXXFLAGS) $(IncludePath)



##
## Objects
##
$(IntermediateDirectory)/src_pdfannotation.cpp$(ObjectSuffix): src/pdfannotation.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfannotation.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfannotation.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfannotation.cpp$(PreprocessSuffix): src/pdfannotation.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfannotation.cpp$(PreprocessSuffix) src/pdfannotation.cpp

$(IntermediateDirectory)/src_pdfbarcode.cpp$(ObjectSuffix): src/pdfbarcode.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfbarcode.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfbarcode.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfbarcode.cpp$(PreprocessSuffix): src/pdfbarcode.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfbarcode.cpp$(PreprocessSuffix) src/pdfbarcode.cpp

$(IntermediateDirectory)/src_pdfcffindex.cpp$(ObjectSuffix): src/pdfcffindex.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfcffindex.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfcffindex.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfcffindex.cpp$(PreprocessSuffix): src/pdfcffindex.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfcffindex.cpp$(PreprocessSuffix) src/pdfcffindex.cpp

$(IntermediateDirectory)/src_pdfdc.cpp$(ObjectSuffix): src/pdfdc.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfdc.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfdc.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfdc.cpp$(PreprocessSuffix): src/pdfdc.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfdc.cpp$(PreprocessSuffix) src/pdfdc.cpp

$(IntermediateDirectory)/src_pdfdocument.cpp$(ObjectSuffix): src/pdfdocument.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfdocument.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfdocument.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfdocument.cpp$(PreprocessSuffix): src/pdfdocument.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfdocument.cpp$(PreprocessSuffix) src/pdfdocument.cpp

$(IntermediateDirectory)/src_pdfencoding.cpp$(ObjectSuffix): src/pdfencoding.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfencoding.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfencoding.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfencoding.cpp$(PreprocessSuffix): src/pdfencoding.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfencoding.cpp$(PreprocessSuffix) src/pdfencoding.cpp

$(IntermediateDirectory)/src_pdfencrypt.cpp$(ObjectSuffix): src/pdfencrypt.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfencrypt.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfencrypt.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfencrypt.cpp$(PreprocessSuffix): src/pdfencrypt.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfencrypt.cpp$(PreprocessSuffix) src/pdfencrypt.cpp

$(IntermediateDirectory)/src_pdffont.cpp$(ObjectSuffix): src/pdffont.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffont.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffont.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffont.cpp$(PreprocessSuffix): src/pdffont.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffont.cpp$(PreprocessSuffix) src/pdffont.cpp

$(IntermediateDirectory)/src_pdffontdata.cpp$(ObjectSuffix): src/pdffontdata.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontdata.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontdata.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontdata.cpp$(PreprocessSuffix): src/pdffontdata.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontdata.cpp$(PreprocessSuffix) src/pdffontdata.cpp

$(IntermediateDirectory)/src_pdffontdatacore.cpp$(ObjectSuffix): src/pdffontdatacore.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontdatacore.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontdatacore.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontdatacore.cpp$(PreprocessSuffix): src/pdffontdatacore.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontdatacore.cpp$(PreprocessSuffix) src/pdffontdatacore.cpp

$(IntermediateDirectory)/src_pdffontdataopentype.cpp$(ObjectSuffix): src/pdffontdataopentype.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontdataopentype.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontdataopentype.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontdataopentype.cpp$(PreprocessSuffix): src/pdffontdataopentype.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontdataopentype.cpp$(PreprocessSuffix) src/pdffontdataopentype.cpp

$(IntermediateDirectory)/src_pdffontdatatruetype.cpp$(ObjectSuffix): src/pdffontdatatruetype.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontdatatruetype.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontdatatruetype.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontdatatruetype.cpp$(PreprocessSuffix): src/pdffontdatatruetype.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontdatatruetype.cpp$(PreprocessSuffix) src/pdffontdatatruetype.cpp

$(IntermediateDirectory)/src_pdffontdatatype1.cpp$(ObjectSuffix): src/pdffontdatatype1.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontdatatype1.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontdatatype1.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontdatatype1.cpp$(PreprocessSuffix): src/pdffontdatatype1.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontdatatype1.cpp$(PreprocessSuffix) src/pdffontdatatype1.cpp

$(IntermediateDirectory)/src_pdffontdescription.cpp$(ObjectSuffix): src/pdffontdescription.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontdescription.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontdescription.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontdescription.cpp$(PreprocessSuffix): src/pdffontdescription.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontdescription.cpp$(PreprocessSuffix) src/pdffontdescription.cpp

$(IntermediateDirectory)/src_pdffontdetails.cpp$(ObjectSuffix): src/pdffontdetails.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontdetails.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontdetails.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontdetails.cpp$(PreprocessSuffix): src/pdffontdetails.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontdetails.cpp$(PreprocessSuffix) src/pdffontdetails.cpp

$(IntermediateDirectory)/src_pdffontextended.cpp$(ObjectSuffix): src/pdffontextended.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontextended.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontextended.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontextended.cpp$(PreprocessSuffix): src/pdffontextended.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontextended.cpp$(PreprocessSuffix) src/pdffontextended.cpp

$(IntermediateDirectory)/src_pdffontmanager.cpp$(ObjectSuffix): src/pdffontmanager.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontmanager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontmanager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontmanager.cpp$(PreprocessSuffix): src/pdffontmanager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontmanager.cpp$(PreprocessSuffix) src/pdffontmanager.cpp

$(IntermediateDirectory)/src_pdffontparser.cpp$(ObjectSuffix): src/pdffontparser.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontparser.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontparser.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontparser.cpp$(PreprocessSuffix): src/pdffontparser.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontparser.cpp$(PreprocessSuffix) src/pdffontparser.cpp

$(IntermediateDirectory)/src_pdffontdatatype0.cpp$(ObjectSuffix): src/pdffontdatatype0.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontdatatype0.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontdatatype0.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontdatatype0.cpp$(PreprocessSuffix): src/pdffontdatatype0.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontdatatype0.cpp$(PreprocessSuffix) src/pdffontdatatype0.cpp

$(IntermediateDirectory)/src_pdffontparsertruetype.cpp$(ObjectSuffix): src/pdffontparsertruetype.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontparsertruetype.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontparsertruetype.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontparsertruetype.cpp$(PreprocessSuffix): src/pdffontparsertruetype.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontparsertruetype.cpp$(PreprocessSuffix) src/pdffontparsertruetype.cpp

$(IntermediateDirectory)/src_pdfdecode.cpp$(ObjectSuffix): src/pdfdecode.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfdecode.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfdecode.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfdecode.cpp$(PreprocessSuffix): src/pdfdecode.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfdecode.cpp$(PreprocessSuffix) src/pdfdecode.cpp

$(IntermediateDirectory)/src_pdfocg.cpp$(ObjectSuffix): src/pdfocg.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfocg.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfocg.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfocg.cpp$(PreprocessSuffix): src/pdfocg.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfocg.cpp$(PreprocessSuffix) src/pdfocg.cpp

$(IntermediateDirectory)/src_pdfgradient.cpp$(ObjectSuffix): src/pdfgradient.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfgradient.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfgradient.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfgradient.cpp$(PreprocessSuffix): src/pdfgradient.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfgradient.cpp$(PreprocessSuffix) src/pdfgradient.cpp

$(IntermediateDirectory)/src_pdfparser.cpp$(ObjectSuffix): src/pdfparser.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfparser.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfparser.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfparser.cpp$(PreprocessSuffix): src/pdfparser.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfparser.cpp$(PreprocessSuffix) src/pdfparser.cpp

$(IntermediateDirectory)/src_pdfrijndael.cpp$(ObjectSuffix): src/pdfrijndael.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfrijndael.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfrijndael.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfrijndael.cpp$(PreprocessSuffix): src/pdfrijndael.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfrijndael.cpp$(PreprocessSuffix) src/pdfrijndael.cpp

$(IntermediateDirectory)/src_pdffontsubsettruetype.cpp$(ObjectSuffix): src/pdffontsubsettruetype.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontsubsettruetype.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontsubsettruetype.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontsubsettruetype.cpp$(PreprocessSuffix): src/pdffontsubsettruetype.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontsubsettruetype.cpp$(PreprocessSuffix) src/pdffontsubsettruetype.cpp

$(IntermediateDirectory)/src_wxpoppler.cpp$(ObjectSuffix): src/wxpoppler.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/wxpoppler.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_wxpoppler.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_wxpoppler.cpp$(PreprocessSuffix): src/wxpoppler.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_wxpoppler.cpp$(PreprocessSuffix) src/wxpoppler.cpp

$(IntermediateDirectory)/src_pdfform.cpp$(ObjectSuffix): src/pdfform.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfform.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfform.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfform.cpp$(PreprocessSuffix): src/pdfform.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfform.cpp$(PreprocessSuffix) src/pdfform.cpp

$(IntermediateDirectory)/src_pdftemplate.cpp$(ObjectSuffix): src/pdftemplate.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdftemplate.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdftemplate.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdftemplate.cpp$(PreprocessSuffix): src/pdftemplate.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdftemplate.cpp$(PreprocessSuffix) src/pdftemplate.cpp

$(IntermediateDirectory)/src_pdffontsubsetcff.cpp$(ObjectSuffix): src/pdffontsubsetcff.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontsubsetcff.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontsubsetcff.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontsubsetcff.cpp$(PreprocessSuffix): src/pdffontsubsetcff.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontsubsetcff.cpp$(PreprocessSuffix) src/pdffontsubsetcff.cpp

$(IntermediateDirectory)/src_pdfcolour.cpp$(ObjectSuffix): src/pdfcolour.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfcolour.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfcolour.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfcolour.cpp$(PreprocessSuffix): src/pdfcolour.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfcolour.cpp$(PreprocessSuffix) src/pdfcolour.cpp

$(IntermediateDirectory)/src_pdfobjects.cpp$(ObjectSuffix): src/pdfobjects.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfobjects.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfobjects.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfobjects.cpp$(PreprocessSuffix): src/pdfobjects.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfobjects.cpp$(PreprocessSuffix) src/pdfobjects.cpp

$(IntermediateDirectory)/src_pdfcffdecoder.cpp$(ObjectSuffix): src/pdfcffdecoder.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfcffdecoder.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfcffdecoder.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfcffdecoder.cpp$(PreprocessSuffix): src/pdfcffdecoder.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfcffdecoder.cpp$(PreprocessSuffix) src/pdfcffdecoder.cpp

$(IntermediateDirectory)/src_pdffontvolt.cpp$(ObjectSuffix): src/pdffontvolt.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontvolt.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontvolt.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontvolt.cpp$(PreprocessSuffix): src/pdffontvolt.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontvolt.cpp$(PreprocessSuffix) src/pdffontvolt.cpp

$(IntermediateDirectory)/src_pdfxml.cpp$(ObjectSuffix): src/pdfxml.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfxml.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfxml.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfxml.cpp$(PreprocessSuffix): src/pdfxml.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfxml.cpp$(PreprocessSuffix) src/pdfxml.cpp

$(IntermediateDirectory)/src_pdfutility.cpp$(ObjectSuffix): src/pdfutility.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfutility.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfutility.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfutility.cpp$(PreprocessSuffix): src/pdfutility.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfutility.cpp$(PreprocessSuffix) src/pdfutility.cpp

$(IntermediateDirectory)/src_pdfprint.cpp$(ObjectSuffix): src/pdfprint.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfprint.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfprint.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfprint.cpp$(PreprocessSuffix): src/pdfprint.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfprint.cpp$(PreprocessSuffix) src/pdfprint.cpp

$(IntermediateDirectory)/src_pdffontparsertype1.cpp$(ObjectSuffix): src/pdffontparsertype1.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdffontparsertype1.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdffontparsertype1.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdffontparsertype1.cpp$(PreprocessSuffix): src/pdffontparsertype1.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdffontparsertype1.cpp$(PreprocessSuffix) src/pdffontparsertype1.cpp

$(IntermediateDirectory)/src_pdfpattern.cpp$(ObjectSuffix): src/pdfpattern.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfpattern.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfpattern.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfpattern.cpp$(PreprocessSuffix): src/pdfpattern.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfpattern.cpp$(PreprocessSuffix) src/pdfpattern.cpp

$(IntermediateDirectory)/src_pdflayer.cpp$(ObjectSuffix): src/pdflayer.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdflayer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdflayer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdflayer.cpp$(PreprocessSuffix): src/pdflayer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdflayer.cpp$(PreprocessSuffix) src/pdflayer.cpp

$(IntermediateDirectory)/src_pdfkernel.cpp$(ObjectSuffix): src/pdfkernel.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfkernel.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfkernel.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfkernel.cpp$(PreprocessSuffix): src/pdfkernel.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfkernel.cpp$(PreprocessSuffix) src/pdfkernel.cpp

$(IntermediateDirectory)/src_pdfimage.cpp$(ObjectSuffix): src/pdfimage.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfimage.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfimage.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfimage.cpp$(PreprocessSuffix): src/pdfimage.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfimage.cpp$(PreprocessSuffix) src/pdfimage.cpp

$(IntermediateDirectory)/src_pdfgraphics.cpp$(ObjectSuffix): src/pdfgraphics.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/developpement/git/Regards/Otherlib/wxPDFDoc/src/pdfgraphics.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_pdfgraphics.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_pdfgraphics.cpp$(PreprocessSuffix): src/pdfgraphics.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_pdfgraphics.cpp$(PreprocessSuffix) src/pdfgraphics.cpp

##
## Clean
##
clean:
	$(RM) -r ./Debug/
	$(RM) ../../include/Debug/header.h.gch


