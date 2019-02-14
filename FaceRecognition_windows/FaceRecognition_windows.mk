##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=FaceRecognition_windows
ConfigurationName      :=Debug
WorkspacePath          :=C:/Regards
ProjectPath            :=C:/Regards/FaceRecognition_windows
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=jfigu
Date                   :=14/01/2019
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
OutputFile             :=$(IntermediateDirectory)/libFaceRecognition.dll
Preprocessors          :=$(PreprocessorSwitch)_WINDOWS $(PreprocessorSwitch)WIN32 $(PreprocessorSwitch)_USRDLL $(PreprocessorSwitch)FACERECOGNITION_EXPORTS $(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="FaceRecognition_windows.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/msys64/mingw64/bin/windres.exe
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)"C:\Program Files (x86)\AMD APP SDK\3.0_mingw\include" $(IncludeSwitch)/mingw64/include/libxml2 $(IncludeSwitch)/mingw64/include/OpenEXR  $(IncludeSwitch)../libextern/dlib-19.7 $(IncludeSwitch)../FaceRecognition 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)FaceRecognition $(LibrarySwitch)rpcrt4 $(LibrarySwitch)oleaut32 $(LibrarySwitch)ole32 $(LibrarySwitch)uuid $(LibrarySwitch)winspool $(LibrarySwitch)winmm $(LibrarySwitch)shell32 $(LibrarySwitch)shlwapi $(LibrarySwitch)comctl32 $(LibrarySwitch)comdlg32 $(LibrarySwitch)advapi32 $(LibrarySwitch)version $(LibrarySwitch)wsock32 $(LibrarySwitch)gdi32 $(LibrarySwitch)Imm32 
ArLibs                 :=  "FaceRecognition" "rpcrt4" "oleaut32" "ole32" "uuid" "winspool" "winmm" "shell32" "shlwapi" "comctl32" "comdlg32" "advapi32" "version" "wsock32" "gdi32" "Imm32" 
LibPath                :=$(LibraryPathSwitch)"C:\Program Files (x86)\AMD APP SDK\3.0_mingw\lib\x86_64"  $(LibraryPathSwitch). $(LibraryPathSwitch)../FaceRecognition/Release 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/msys64/mingw64/bin/ar.exe rcu
CXX      := C:/msys64/usr/bin/ccache.exe C:/msys64/mingw64/bin/g++.exe
CC       := C:/msys64/usr/bin/ccache.exe C:/msys64/mingw64/bin/gcc.exe
CXXFLAGS := -fopenmp -std=gnu++11 -Wall -fpermissive -fPIC -g -O0 -Wall  $(Preprocessors)
CFLAGS   :=  -Wall  -g -O0 -Wall  $(Preprocessors)
ASFLAGS  := 
AS       := C:/msys64/usr/bin/ccache.exe C:/msys64/mingw64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
PATH:=C:\msys64\usr\bin;$PATH
WXWIN:=C:\Regards\libextern\wxwidgets-3.1.2
WXCFG:=lib/mswud
Objects0=$(IntermediateDirectory)/up_FaceRecognition_dllmain.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_FaceRecognition_FaceRecognition.cpp$(ObjectSuffix) 



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
	$(SharedObjectLinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)
	@$(MakeDirCommand) "C:\Regards/.build-debug"
	@echo rebuilt > "C:\Regards/.build-debug/FaceRecognition_windows"

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Debug"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_FaceRecognition_dllmain.cpp$(ObjectSuffix): ../FaceRecognition/dllmain.cpp $(IntermediateDirectory)/up_FaceRecognition_dllmain.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/FaceRecognition/dllmain.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_FaceRecognition_dllmain.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_FaceRecognition_dllmain.cpp$(DependSuffix): ../FaceRecognition/dllmain.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_FaceRecognition_dllmain.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_FaceRecognition_dllmain.cpp$(DependSuffix) -MM ../FaceRecognition/dllmain.cpp

$(IntermediateDirectory)/up_FaceRecognition_dllmain.cpp$(PreprocessSuffix): ../FaceRecognition/dllmain.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_FaceRecognition_dllmain.cpp$(PreprocessSuffix) ../FaceRecognition/dllmain.cpp

$(IntermediateDirectory)/up_FaceRecognition_FaceRecognition.cpp$(ObjectSuffix): ../FaceRecognition/FaceRecognition.cpp $(IntermediateDirectory)/up_FaceRecognition_FaceRecognition.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Regards/FaceRecognition/FaceRecognition.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_FaceRecognition_FaceRecognition.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_FaceRecognition_FaceRecognition.cpp$(DependSuffix): ../FaceRecognition/FaceRecognition.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_FaceRecognition_FaceRecognition.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_FaceRecognition_FaceRecognition.cpp$(DependSuffix) -MM ../FaceRecognition/FaceRecognition.cpp

$(IntermediateDirectory)/up_FaceRecognition_FaceRecognition.cpp$(PreprocessSuffix): ../FaceRecognition/FaceRecognition.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_FaceRecognition_FaceRecognition.cpp$(PreprocessSuffix) ../FaceRecognition/FaceRecognition.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


