;NSIS Modern User Interface
;Basic Example Script
;Written by Joost Verburg

;--------------------------------
;Include Modern UI

!include "MUI2.nsh"
!include "FileAssociation.nsh"

# Reserve plugin files (good practice)
ReserveFile `ListView_CheckBoxes.ini`
ReserveFile `${NSISDIR}\Plugins\InstallOptions.dll`
ReserveFile `${NSISDIR}\Plugins\EmbeddedLists.dll`


;--------------------------------
;General
!define MUI_PRODUCT "Regards Viewer 2.53"
!define MUI_FILE "RegardsViewer"
!define MUI_ICON "viewer.ico"
!define UninstId "RegardsViewer2" ; You might want to use a GUID here

  ;Name and file
  Name "Associate"
  OutFile "Associate.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES64\RegardsViewer2"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\RegardsViewer2" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel admin

;--------------------------------
;Interface Settings
!define buttonText "!insertMacro buttonText"
  !define MUI_ABORTWARNING
  
MiscButtonText "Previous" "Next" "Cancel" "Ok"

;--------------------------------
;Pages

 ; !insertmacro MUI_PAGE_WELCOME
 ; !insertmacro MUI_PAGE_LICENSE "License.txt"
 ; !insertmacro MUI_PAGE_COMPONENTS
 ; !insertmacro MUI_PAGE_DIRECTORY
 ; !insertmacro MUI_PAGE_INSTFILES
	Page Custom ListViewShow ListViewLeave  
 ; !insertmacro MUI_PAGE_FINISH
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"
 !define Explode "!insertmacro Explode"
 
!macro buttonText button value
	Push ${button}
	Push t
	Push "${value}"
	Call Buttons
!macroend

; Function (inserted via macros for installer vs uninstaller)
; Buttons FUnction
;   Used to interact with the three default buttons; Back, Next, Cancel
; Usage :
;   Push Back|Next|Cancel
;   Push v|e|t
;   Push 1|0|<string>
;   Call Buttons | Call Un.Buttons
; Result :
;   Interacts with the appropriate button
;   If 'v', then 1 will set visible and 0 will set invisible
;   If 'e', then 1 will set enabled and 0 will set disabled
;   If 't', then <string> will be set as the button's new label text
!macro ButtonsFunc Un
	Function ${Un}Buttons
									; Stack: <value> <mode> <button>
		Exch $2 ; value				; Stack: $2 <mode> <button>
		Exch						; Stack: <mode> $2 <button>
		Exch $1 ; mode				; Stack: $1 $2 <button>
		Exch 2						; Stack: <button> $2 $1
		Exch $0 ; button			; Stack: $0 $2 $1
		Push $3						; Stack: $3 $0 $2 $1
 
		StrCmp $0 "Back" 0 +3
			StrCpy $3 3
			goto _setbutton
		StrCmp $0 "Next" 0 +3
			StrCpy $3 1
			goto _setbutton
		StrCmp $0 "Cancel" 0 _end
			StrCpy $3 2
 
		_setbutton:
		GetDlgItem $3 $HWNDPARENT $3
 
		StrCmp $1 "v" 0 +3
			ShowWindow $3 $2
			goto _end
		StrCmp $1 "e" 0 +3
			EnableWindow $3 $2
			goto _end
		StrCmp $1 "t" 0 _end
			SendMessage $3 ${WM_SETTEXT} 0 "STR:$2"
 
		_end:
 
									; Stack: $3 $0 $2 $1
		Pop $3						; Stack: $0 $2 $1
		Pop $0						; Stack: $2 $1
		Pop $2						; Stack: $1
		Pop $1						; Stack: [clean]
	FunctionEnd
!macroend
!insertmacro ButtonsFunc ""

Function .onInit

 InitPluginsDir
 File `/oname=$PLUGINSDIR\ListView_CheckBoxes.ini` `ListView_CheckBoxes.ini`

 WriteINIStr `$PLUGINSDIR\ListView_CheckBoxes.ini` `Icons` `Icon1` `$EXEDIR\icon1.ico`
 WriteINIStr `$PLUGINSDIR\ListView_CheckBoxes.ini` `Icons` `Icon2` `$EXEDIR\icon2.ico`

	${buttonText} "Close" "Ok"
FunctionEnd

 

;--------------------------------
;Descriptions

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecRegardsViewer} $(DESC_SecRegardsViewer)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

Var CHECKBOX

# Custom page functions
# [[

Function ListViewShow

 EmbeddedLists::Dialog `$PLUGINSDIR\ListView_CheckBoxes.ini`
  Pop $R0
  
WriteINIStr "$PLUGINSDIR\ioSpecial.ini" "Field 4" "Type" "Checkbox"

FunctionEnd

Function OnCheckbox
	Pop $0 # HWND
	MessageBox MB_OK "checkbox clicked"
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


FunctionEnd

# ]]

# Empty section
Section
SectionEnd
 
 


