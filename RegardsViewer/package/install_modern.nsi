;NSIS Modern User Interface
;Basic Example Script
;Written by Joost Verburg

;--------------------------------
;Include Modern UI

!include "MUI2.nsh"
!include "ZipDLL.nsh"
;--------------------------------
;General
!define MUI_PRODUCT "Regards Viewer 2.47"
!define MUI_FILE "RegardsViewer"
!define MUI_ICON "viewer.ico"
!define UninstId "RegardsViewer2" ; You might want to use a GUID here

  ;Name and file
  Name "Regards Viewer 2.47"
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
 


Function .onInit
StrCpy $0 "$INSTDIR\Uninstall.exe"
IfFileExists "$INSTDIR\Uninstall.exe" file_found file_not_found
file_found:
${If} $0 != ""
${AndIf} ${Cmd} `MessageBox MB_YESNO|MB_ICONQUESTION "Uninstall previous version?" /SD IDYES IDYES`
  Delete "$INSTDIR\Uninstall.exe"
  Delete "$DESKTOP\${MUI_PRODUCT}.lnk"

  ;create start-menu items
  Delete "$SMPROGRAMS\${MUI_PRODUCT}"

  RMDIR /r "$INSTDIR"

  DeleteRegKey /ifempty HKCU "Software\RegardsViewer2"
${EndIf}
file_not_found:
	DetailPrint "Uninstall Regards Viewer"

FunctionEnd
  
Function openLinkNewWindow
  Push $3
  Exch
  Push $2
  Exch
  Push $1
  Exch
  Push $0
  Exch
 
  ReadRegStr $0 HKCR "http\shell\open\command" ""
# Get browser path
    DetailPrint $0
  StrCpy $2 '"'
  StrCpy $1 $0 1
  StrCmp $1 $2 +2 # if path is not enclosed in " look for space as final char
    StrCpy $2 ' '
  StrCpy $3 1
  loop:
    StrCpy $1 $0 1 $3
    DetailPrint $1
    StrCmp $1 $2 found
    StrCmp $1 "" found
    IntOp $3 $3 + 1
    Goto loop
 
  found:
    StrCpy $1 $0 $3
    StrCmp $2 " " +2
      StrCpy $1 '$1"'
 
  Pop $0
  Exec '$1 $0'
  Pop $0
  Pop $1
  Pop $2
  Pop $3
FunctionEnd
 
!macro _OpenURL URL
Push "${URL}"
Call openLinkNewWindow
!macroend  

;--------------------------------
;Installer Sections
Section "Regards Viewer 2.47" SecRegardsViewer

  SetOutPath "$INSTDIR"
  
  
  ;ADD YOUR OWN FILES HERE...
  DetailPrint "*** Installing Regards Viewer 2.47..."
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
  
  Push "https://fr.tipeee.com/regardsviewer"
  Call openLinkNewWindow
SectionEnd


;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecRegardsViewer ${LANG_ENGLISH} "Regards Viewer 2.47"

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

  RMDIR /r "$INSTDIR"

  DeleteRegKey /ifempty HKCU "Software\RegardsViewer2"

SectionEnd


 
 


