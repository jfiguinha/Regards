;NSIS Modern User Interface
;Basic Example Script
;Written by Joost Verburg

;--------------------------------
;Include Modern UI

!include "MUI2.nsh"
!include "ZipDLL.nsh"
!include "FileAssociation.nsh"

# Reserve plugin files (good practice)
ReserveFile `ListView_CheckBoxes.ini`
ReserveFile `${NSISDIR}\Plugins\InstallOptions.dll`
ReserveFile `${NSISDIR}\Plugins\EmbeddedLists.dll`

;--------------------------------
;General
!define MUI_PRODUCT "Regards Viewer 2.70.5"
!define MUI_FILE "RegardsViewer"
!define MUI_ICON "viewer.ico"
!define UninstId "RegardsViewer2" ; You might want to use a GUID here

  ;Name and file
  Name "Regards Viewer 2.70.5"
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
	Page Custom ListViewShow ListViewLeave
  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"
 !define Explode "!insertmacro Explode"


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

 InitPluginsDir
 File `/oname=$PLUGINSDIR\ListView_CheckBoxes.ini` `ListView_CheckBoxes.ini`

 WriteINIStr `$PLUGINSDIR\ListView_CheckBoxes.ini` `Icons` `Icon1` `$EXEDIR\icon1.ico`
 WriteINIStr `$PLUGINSDIR\ListView_CheckBoxes.ini` `Icons` `Icon2` `$EXEDIR\icon2.ico`

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
Section "Regards Viewer 2.70.5" SecRegardsViewer

  SetOutPath "$INSTDIR"


  ;ADD YOUR OWN FILES HERE...
  DetailPrint "*** Installing Regards Viewer 2.70.5..."
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
;Installer Sections
Section "Visual Studio 2017 Redistribuable x64" SecVisualStudio
  SetOutPath "$INSTDIR"
  ;ADD YOUR OWN FILES HERE...
  DetailPrint "*** Installing Visual Studio 2017 Redistribuable x64..."
  File "Prerequisites\VC_redist.x64.exe"
  ExecWait "$INSTDIR\VC_redist.x64.exe"
  Delete "$INSTDIR\VC_redist.x64.exe"
SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecRegardsViewer ${LANG_ENGLISH} "Regards Viewer 2.70.5"
  LangString DESC_SecVisualStudio ${LANG_ENGLISH} "Visual Studio 2017 Redistribuable x64"
  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecRegardsViewer} $(DESC_SecRegardsViewer)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecVisualStudio} $(DESC_SecVisualStudio)
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


# Custom page functions
# [[

Function ListViewShow

 EmbeddedLists::Dialog `$PLUGINSDIR\ListView_CheckBoxes.ini`
  Pop $R0

FunctionEnd

!macro  Explode Length  Separator   String
    Push    `${Separator}`
    Push    `${String}`
    Call    Explode
    Pop     `${Length}`
!macroend

Function Explode
  ; Initialize variables
  Var /GLOBAL explString
  Var /GLOBAL explSeparator
  Var /GLOBAL explStrLen
  Var /GLOBAL explSepLen
  Var /GLOBAL explOffset
  Var /GLOBAL explTmp
  Var /GLOBAL explTmp2
  Var /GLOBAL explTmp3
  Var /GLOBAL explArrCount

  ; Get input from user
  Pop $explString
  Pop $explSeparator

  ; Calculates initial values
  StrLen $explStrLen $explString
  StrLen $explSepLen $explSeparator
  StrCpy $explArrCount 1

  ${If}   $explStrLen <= 1          ;   If we got a single character
  ${OrIf} $explSepLen > $explStrLen ;   or separator is larger than the string,
    Push    $explString             ;   then we return initial string with no change
    Push    1                       ;   and set array's length to 1
    Return
  ${EndIf}

  ; Set offset to the last symbol of the string
  StrCpy $explOffset $explStrLen
  IntOp  $explOffset $explOffset - 1

  ; Clear temp string to exclude the possibility of appearance of occasional data
  StrCpy $explTmp   ""
  StrCpy $explTmp2  ""
  StrCpy $explTmp3  ""

  ; Loop until the offset becomes negative
  ${Do}
    ;   If offset becomes negative, it is time to leave the function
    ${IfThen} $explOffset == -1 ${|} ${ExitDo} ${|}

    ;   Remove everything before and after the searched part ("TempStr")
    StrCpy $explTmp $explString $explSepLen $explOffset

    ${If} $explTmp == $explSeparator
        ;   Calculating offset to start copy from
        IntOp   $explTmp2 $explOffset + $explSepLen ;   Offset equals to the current offset plus length of separator
        StrCpy  $explTmp3 $explString "" $explTmp2

        Push    $explTmp3                           ;   Throwing array item to the stack
        IntOp   $explArrCount $explArrCount + 1     ;   Increasing array's counter

        StrCpy  $explString $explString $explOffset 0   ;   Cutting all characters beginning with the separator entry
        StrLen  $explStrLen $explString
    ${EndIf}

    ${If} $explOffset = 0                       ;   If the beginning of the line met and there is no separator,
                                                ;   copying the rest of the string
        ${If} $explSeparator == ""              ;   Fix for the empty separator
            IntOp   $explArrCount   $explArrCount - 1
        ${Else}
            Push    $explString
        ${EndIf}
    ${EndIf}

    IntOp   $explOffset $explOffset - 1
  ${Loop}

  Push $explArrCount
FunctionEnd

Function ListViewLeave

 StrCpy $R1 ``        ; Clear checked items list.

 Pop $R0              ; Checked item number.
 StrCmp $R0 /END +4   ; No item checked?
  ReadINIStr $R0 `$PLUGINSDIR\ListView_CheckBoxes.ini` `Item $R0` `Text`
  StrCpy $R1 $R1|$R0
 Goto -4              ; Loop.

 StrCpy $R1 $R1 `` 1  ; Trim first | from front.
 StrCmp $R1 `` +2     ; Skip MessageBox.

  ${Explode}  $0  "|" $R1

 #MessageBox  MB_OK "$0 elements in array"

	${For} $1 1 $0
		Pop $2
		${registerExtension} "$INSTDIR\RegardsViewer.exe" $2 "RegardsViewer.Files"
		#MessageBox MB_OK "Element #$1: $2"
	${Next}



  Push "https://fr.tipeee.com/regardsviewer"
  Call openLinkNewWindow

FunctionEnd

# ]]

# Empty section
Section
SectionEnd
