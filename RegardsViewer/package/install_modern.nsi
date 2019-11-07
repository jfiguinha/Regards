;NSIS Modern User Interface
;Basic Example Script
;Written by Joost Verburg

;--------------------------------
;Include Modern UI

!include "MUI2.nsh"
!include "ZipDLL.nsh"
;--------------------------------
;General
!define MUI_PRODUCT "Regards Viewer 2.30.1"
!define MUI_FILE "RegardsViewer"
!define MUI_ICON "viewer.ico"

  ;Name and file
  Name "Regards Viewer 2.30.1"
  OutFile "RegardsViewer2Setup.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES64\RegardsViewer2"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\RegardsViewer2" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel admin

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "License.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections
Section "Regards Viewer 2.30.1" SecRegardsViewer

  SetOutPath "$INSTDIR"
  
  ;ADD YOUR OWN FILES HERE...
  DetailPrint "*** Installing Regards Viewer 2.30.1..."
  File "Prerequisites\RegardsViewer2.zip"
  ZipDLL::extractall "$INSTDIR\RegardsViewer2.zip" $INSTDIR
  ;Store installation folder
  WriteRegStr HKCU "Software\RegardsViewer2" "" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

  DetailPrint "*** Remove zip installer ..."
  Delete "$INSTDIR\RegardsViewer2.zip"
    
  ;create desktop shortcut
  CreateShortCut "$DESKTOP\${MUI_PRODUCT}.lnk" "$INSTDIR\${MUI_FILE}.exe" ""
  
  ;create start-menu items
  CreateDirectory "$SMPROGRAMS\${MUI_PRODUCT}"
  CreateShortCut "$SMPROGRAMS\${MUI_PRODUCT}\Uninstall.lnk" "$INSTDIR\Uninstall.exe" "" "$INSTDIR\Uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\${MUI_PRODUCT}\${MUI_PRODUCT}.lnk" "$INSTDIR\${MUI_FILE}.exe" "" "$INSTDIR\${MUI_FILE}.exe" 0
  
  ;write uninstall information to the registry
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "DisplayName" "${MUI_PRODUCT} (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "UninstallString" "$INSTDIR\Uninstall.exe"
 
  WriteUninstaller "$INSTDIR\Uninstall.exe"
 
SectionEnd


;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecRegardsViewer ${LANG_ENGLISH} "Regards Viewer 2.30.1"

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecRegardsViewer} $(DESC_SecRegardsViewer)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"


  Delete "$INSTDIR\Uninstall.exe"
  Delete "$DESKTOP\${MUI_PRODUCT}.lnk"
  
  ;create start-menu items
  Delete "$SMPROGRAMS\${MUI_PRODUCT}"

  RMDir "$INSTDIR"

  DeleteRegKey /ifempty HKCU "Software\RegardsViewer2"

SectionEnd
