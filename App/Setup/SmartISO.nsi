; Script generated by the HM NIS Edit Script Wizard.

!system "GetVersion.exe SmartISO.exe"
!include "VersionNo.nsi"

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "SmartISO"
!define PRODUCT_PUBLISHER "TechSun"
!define PRODUCT_WEB_SITE "http://www.TechSun.co.kr"
!define PRODUCT_DIR_REGKEY "Software\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}"
!define PRODUCT_UNINST_KEY "Software\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}\Uninstall"
!define PRODUCT_UNINST_ROOT_KEY "hklm"

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "TechSun.bmp" ; optional
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; Component page
!insertmacro MUI_PAGE_COMPONENTS
; Directory page
!insertmacro MUI_PAGE_DIRECTORY

; MSTN J폴더를 선택하는 페이지
Var vMSTNJFolder
!define TEMP1 $R0 ;Temp variable
Page custom SmartISO_NSIS SmartISO_NSISLeave

; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\SmartISO.exe"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "Korean"

Var projectSelected

BrandingText "${PRODUCT_NAME}"

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "Setup\SmartISO.${PRODUCT_VERSION}.exe"

InstallDir "$PROGRAMFILES\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Reverse a string
;; P1 :o: Reversed string
;; P2 :i: Original string
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
!define StrRev "!insertmacro _StrRev"
!macro _StrRev _REV_ _STR_
   Push "${_STR_}"
   Call StrRev
   Pop ${_REV_}
!macroend

Function StrRev
   Exch $1  ;; Orig string
   Push $0  ;; Reversed string
   Exch
   Push $2  ;; String Len / Counter
   Push $3  ;; Current character

   StrCpy $0 ""
   StrLen $2 "$1"

   IntCmp $2 0 +5
      IntOp $2 $2 - 1
      StrCpy $3 "$1" 1 $2
      StrCpy $0 "$0$3"
      Goto -4

   Pop $3
   Pop $2
   Pop $1
   Exch $0
FunctionEnd

!define StrLoc "!insertmacro StrLoc"

!macro StrLoc ResultVar String SubString StartPoint
  Push "${String}"
  Push "${SubString}"
  Push "${StartPoint}"
  Call StrLoc
  Pop "${ResultVar}"
!macroend

Function StrLoc
/*After this point:
  ------------------------------------------
   $R0 = StartPoint (input)
   $R1 = SubString (input)
   $R2 = String (input)
   $R3 = SubStringLen (temp)
   $R4 = StrLen (temp)
   $R5 = StartCharPos (temp)
   $R6 = TempStr (temp)*/

  ;Get input from user
  Exch $R0
  Exch
  Exch $R1
  Exch 2
  Exch $R2
  Push $R3
  Push $R4
  Push $R5
  Push $R6

  ;Get "String" and "SubString" length
  StrLen $R3 $R1
  StrLen $R4 $R2
  ;Start "StartCharPos" counter
  StrCpy $R5 0

  ;Loop until "SubString" is found or "String" reaches its end
  ${Do}
    ;Remove everything before and after the searched part ("TempStr")
    StrCpy $R6 $R2 $R3 $R5

    ;Compare "TempStr" with "SubString"
    ${If} $R6 == $R1
      ${If} $R0 == `<`
        IntOp $R6 $R3 + $R5
        IntOp $R0 $R4 - $R6
      ${Else}
        StrCpy $R0 $R5
      ${EndIf}
      ${ExitDo}
    ${EndIf}
    ;If not "SubString", this could be "String"'s end
    ${If} $R5 >= $R4
      StrCpy $R0 ``
      ${ExitDo}
    ${EndIf}
    ;If not, continue the loop
    IntOp $R5 $R5 + 1
  ${Loop}

  ;Return output to user
  Pop $R6
  Pop $R5
  Pop $R4
  Pop $R3
  Pop $R2
  Exch
  Pop $R1
  Exch $R0
FunctionEnd

Function SmartISO_NSIS
    Push ${TEMP1}
    SetOutPath "$INSTDIR"
    File /oname=$INSTDIR\SmartISO_NSIS.ini "SmartISO_NSIS.ini"

    !insertmacro MUI_HEADER_TEXT 'Write Module' 'MSTN J 설치 폴더를 선택하세요'

    ; get microstation j folder
    ReadRegStr $1 HKLM SOFTWARE\Bentley\MicroStation\07.01 "PathName"
    ${StrRev} $0 "$1"   ;; $StrVar="gnirtS"
    ${StrLoc} $R0 $0 '\' '<'
    StrCpy $0 "$1" $R0 # get mstn j folder
    WriteINIStr "$INSTDIR\SmartISO_NSIS.ini" "Field 2" "State" "$0"

    InstallOptions::dialog "$INSTDIR\SmartISO_NSIS.ini"
    Pop ${TEMP1}

  Pop ${TEMP1}
FunctionEnd

Function SmartISO_NSISLeave
    ReadINIStr $vMSTNJFolder "$INSTDIR\SmartISO_NSIS.ini" "Field 2" "State"

    ${StrRev} $0 $vMSTNJFolder  ;; $StrVar="gnirtS"
    ${StrLoc} $R0 $0 '\' '<'
    StrCpy $0 $vMSTNJFolder $R0
    ${StrRev} $1 $0  ;; $StrVar="gnirtS"
    ${StrLoc} $R0 $1 '\' '<'
    StrCpy $0 "$0" $R0 # get mstn j folder

    ;copy macro and pen table files to workspace\system\macros and workspace\system\tables\pen folder
    ${If} $0 != ""
        File /oname=$0\workspace\system\macros\SmartISO.ba  "Backup\Plotting\SmartISO.ba"
        File /oname=$0\workspace\system\tables\pen\SmartISO.tbl  "Backup\Plotting\SmartISO.tbl"
    ${EndIf}
FunctionEnd

;-------------------------------
; Install Visual Studio Redistributables 2008+ SP1
; 2013.06.10
Function InstallVC2008Redist
   Push $R0
   ClearErrors
   ; check Redistributables is installed on x64
   ReadRegDword $R0 HKLM "SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\{887868A2-D6DE-3255-AA92-AA0B5A59B874}" "Version"
   IfErrors 0 VSRedistInstalled
   ; check Redistributables is installed on x26
   ReadRegDword $R0 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{887868A2-D6DE-3255-AA92-AA0B5A59B874}" "Version"
   ; if VS 2008+ redist SP1 not installed, install it
   IfErrors 0 VSRedistInstalled
   IfFileExists "$INSTDIR\Temp\vcredist_x86_sp1.exe"  0 VSRedistInstalled
       ExecWait "$INSTDIR\Temp\vcredist_x86_sp1.exe"
   Delete "$INSTDIR\Temp\vcredist_x86_sp1.exe"

VSRedistInstalled:
   Pop $R0
FunctionEnd
   
Section "SmartISO" SEC01
  SectionIn RO   ; 필수 설치 요소
  
  SetOutPath "$INSTDIR"
  SetOverwrite on
  File "SmartISO.exe"

  SetShellVarContext all             ;;; create folders for all users - 2012.06.04 added by humkyung
  CreateDirectory "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk" "$INSTDIR\SmartISO.exe"
  CreateShortCut "$DESKTOP\SmartISO.lnk" "$INSTDIR\SmartISO.exe"

  File "SmartAdmin.exe"
  File "module_vc100.dll"
;  File "SupportCell.exe"
;  File "IssueManager.exe"               ; 2011.02.19 added by HumKyung
;  File "IsoCreator.exe"                 ; 2012.05.18 added by humkyung
;  File "PrintDesign.exe"                ; 2012.09.20 added by humkyung
  File "UserDLL_vc100.dll"
  File "is_vc100.dll"
  File "isutil_vc100.dll"
  File "isgui_vc100.dll"                  ;2011.08.06 added by humkyung
  File "DgnV7Lib_vc100.dll"
;  File "SciLexer.dll"                    ;use scilexer instead of cryseditctrl - 2011.11.01 added by humkyung
  File "ogl2d_vc100.dll"
;  File "idflib_vc100.dll"
;  File "msvcrt.dll"
  File "vftlib_vc100.dll"
;  File "Export.dll"                      ;2012.05.18 added by humkyung
; for oracle 2012.08.22
  File "oci.dll"
  File "ocijdbc11.dll"
  File "ociw32.dll"
  File "orannzsbb11.dll"
  File "oraocci11.dll"
  File "oraociei11.dll"
  File "oraociicus11.dll"
  File "orasql11.dll"
;  File "msvcr80.dll"
  File "soci*.dll"
  File "sqlite3.dll"
  File "libpq.dll"
  File "ssleay32.dll"
  File "libeay32.dll"
  File "libintl-8.dll"
  File "libiconv-2.dll"
; up to here
;  File "adodb.dll"
; for framework.cor
;  File "framework.core.dll"
  File "log4net.dll"

  File "GemBox.Spreadsheet.dll"                    ; 2013.01.26 added by humkyung
  File "GenericParsing.dll"
  File "SmartISOLayer.dll"
;  File "DgvFilterPopup.dll"
  File "DrawingReport.jpg"
  File "readme.txt"

; for Account Tool
  SetOutPath "$INSTDIR\Tools"
  File "Tools\Account.exe"
  File "Tools\EntityFramework.dll"
  File "Tools\EntityFramework.SqlServer.dll"
  File "Tools\System.Data.SQLite.dll"
  File "Tools\System.Data.SQLite.EF6.dll"
  File "Tools\System.Data.SQLite.Linq.dll"
; for postgresql
  File "Tools\Npgsql.dll"
  File "Tools\System.Buffers.dll"
  File "Tools\System.Memory.dll"
  File "Tools\System.Runtime.CompilerServices.Unsafe.dll"
  File "Tools\System.Threading.Tasks.Extensions.dll"
  File "Tools\System.ValueTuple.dll"

  SetOutPath "$INSTDIR\Tools\x64"
  File "Tools\x64\*SQLite.Interop.dll"

  SetOutPath "$INSTDIR\Tools\x86"
  File "Tools\x86\SQLite.Interop.dll"
; up to here

;;; Setting Folder
  SetOutPath "$INSTDIR\Setting"
  File "Setting\CELL_MAPPING.TBL"
  File "Setting\SmartISO_a3.i"          
  File "Setting\SmartISO_a4.i"
  File "Setting\Default.ini"
  File "Setting\interface1.xls"             ;2011.01.20 - modified by HumKyung
;;  File "Setting\interface2.xls"             ;2011.01.20 - added by HumKyung
  File "Setting\ISOPrintTemp.xls"
  File "Setting\IsoProductTemplate.xls"     ;2012.11.06 added by humkyung
  File "Setting\*.SQL"
  File "Setting\*.xml"                      ; 2012.09.03 added by humkyung

; 2012.10.28 added by humkyung
  SetOutPath "$APPDATA\TechSun\IssueManager\Setting"
  File "Setting\*.html"
; up to here

  SetOutPath $SYSDIR
;  File "VB6KO.DLL"        ; for PrintDesign.exe 2012.09.21 added by humkyung

exit_setting:
  ;up to here

  SetOutPath "$INSTDIR\fonts"
  File "fonts\*.vft"

  SetOutPath "$INSTDIR\Sample MDB"
  File "Sample MDB\USER.mdb"
  File "Sample MDB\TABLE.mdb"
  File "Sample MDB\project.mdb"
  
  SetOutPath "$INSTDIR\Backup"
  File "Backup\list.bat"
  File "Backup\121.ris"
  File "Backup\122.ris"
;  File "Backup\SmartISO_ntr.exe"
;  File "Backup\SmartISO_ntr.ini"
  File "Backup\WeldType.xml"              ; 2012.08.22 added by humkyung
  File "Backup\ris.mdb"                   ; 2012.08.31 added by humkyung
  File "Backup\BMPrintTemp.xls"           ; 2013.01.27 added by humkyung
  File "Backup\Account.sql"               ; 2020.04.29 added by humkyung
  
  ; 2012.05.24 added by humkyung
  SetOutPath "$INSTDIR\Help"
;  File "Help\*.chm"
  ; up to here
  
  ; 2012.01.04 added by humkyung
  SetOutPath "$INSTDIR\Plugin"
  File "Plugin\SmartISODiff.dll"
  ; up to here
  
  SetOutPath "$INSTDIR\Temp"
;  File "Backup\vcredist_x86_sp1.exe"

  SetShellVarContext current ;create folders for current user
  CreateDirectory "$APPDATA\${PRODUCT_NAME}"
SectionEnd

Section "AutoUp" SEC02
  SectionIn RO   ; 필수 설치 요소

  SetOutPath "$INSTDIR"
  File ".\AutoUp_vc100.dll"
SectionEnd

;Section ".NET Framework 3.1" SEC04
;    SectionIn 1 2
;SectionEnd

Section -AdditionalIcons
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"

  SetShellVarContext all             ;;; create folders for all users - 2012.06.04 added by humkyung
  
  CreateShortCut "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}\SupportCell.lnk" "$INSTDIR\SupportCell.exe"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

; 설치하고 난 후...
Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\SmartISO.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "Version" "${PRODUCT_VERSION}"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}\License" "IP" "175.126.145.37"
  WriteRegDWORD HKLM "${PRODUCT_DIR_REGKEY}\License" "Port" "2002"

  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\SmartISO.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
  
  Sleep 1000

  call InstallVC2008Redist
no_project:
  ExecShell "open" "$INSTDIR\readme.txt"
SectionEnd

Function .onInit
         ReadRegStr $R0 HKLM "${PRODUCT_UNINST_KEY}" "UninstallString"
         StrCmp $R0 "" done

         MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION \
                    "${PRODUCT_NAME} is already installed. $\n$\nClick `OK` to remove the \
                    previous version or `Cancel` to cancel this upgrade." \
                    IDOK uninst
                    Abort done

;Run the uninstaller
uninst:
       ClearErrors
       ExecWait '"$R0" _?=$INSTDIR' ;Do not copy the uninstaller to a temp file
       ;abort          ; 프로그램 종료

       IfErrors no_remove_uninstaller
    ;You can either use Delete /REBOOTOK in the uninstaller or add some code
    ;here to remove the uninstaller. Use a registry key to check
    ;whether the user has chosen to uninstall. If you are using an uninstaller
    ;components page, make sure all sections are uninstalled.
  no_remove_uninstaller:

done:
FunctionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name)는(은) 완전히 제거되었습니다."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "$(^Name)을(를) 제거하시겠습니까?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"

  Delete "$INSTDIR\*.exe"
  Delete "$INSTDIR\*.dll"
  Delete "$INSTDIR\*.i"
  Delete "$INSTDIR\DrawingReport.jpg"

  SetShellVarContext all             ;;; create folders for all users - 2012.06.04 added by humkyung

  Delete "$DESKTOP\SmartISO.lnk"
  Delete "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}\Uninstall.lnk"
  Delete "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}\Website.lnk"
  Delete "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}\SmartISO.lnk"
  Delete "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}\SupportCell.lnk"
  RMDir  "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}"
  
  Delete "$INSTDIR\fonts\*.vft"
  RMDir "$INSTDIR\fonts"
  
;;; setting folder
  Delete "$INSTDIR\Setting\Default.ini"
  Delete "$INSTDIR\Setting\*.xls"
  Delete "$INSTDIR\Setting\*.SQL"
  Delete "$INSTDIR\Setting\CELL_MAPPING.TBL"
  Delete "$INSTDIR\Setting\*.i"

  RMDir "$INSTDIR\Setting"
  
  Delete "$INSTDIR\Backup\*.*"
  RMDir "$INSTDIR\Backup"
  
  ; 2012.01.04 added by humkyung
  Delete "$INSTDIR\Plugin\*.*"
  RMDir "$INSTDIR\Plugin"
  ; up to here
  
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd
