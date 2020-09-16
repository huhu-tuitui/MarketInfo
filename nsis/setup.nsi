; �ýű�ʹ�� HM VNISEdit �ű��༭���򵼲���

; ��װ�����ʼ���峣��
!define PRODUCT_NAME "��������"
!define SHORTCUT_NAME "MarketInfo"
!define PRODUCT_VERSION "1.0.1"
!define PRODUCT_PUBLISHER "TSingFun, Inc."
!define PRODUCT_WEB_SITE "http://futures.tsingfun.com"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\MarketInfo.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTCUT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_DEFAULT_PATH "C:\${SHORTCUT_NAME}"
!define PRODUCT_STARTMENU_REGVAL "NSIS:StartMenuDir"

SetCompressor /SOLID lzma
SetCompressorDictSize 32

; ------ MUI �ִ����涨�� (1.67 �汾���ϼ���) ------
!include "MUI.nsh"

; MUI Ԥ���峣��
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"
!define MUI_WELCOMEFINISHPAGE_BITMAP "install.bmp"
!define MUI_CUSTOMFUNCTION_GUIINIT onGUIInit

; ����ѡ�񴰿ڳ�������
!define MUI_LANGDLL_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

; ��ӭҳ��
!insertmacro MUI_PAGE_WELCOME
; ���Э��ҳ��
!define MUI_LICENSEPAGE_RADIOBUTTONS
!insertmacro MUI_PAGE_LICENSE "Eula.rtf"
; ��װĿ¼ѡ��ҳ��
!insertmacro MUI_PAGE_DIRECTORY
DirText "��װ���򽫰�װ $(^NameDA) �������ļ��С�Ҫ��װ����ͬ�ļ��У����� [���(B)] ��ѡ���������ļ��С� $_CLICK"

; ��װ����ҳ��
!insertmacro MUI_PAGE_INSTFILES
; ��װ���ҳ��
!define MUI_FINISHPAGE_RUN "$INSTDIR\MarketInfo.exe"
!insertmacro MUI_PAGE_FINISH

; ��װж�ع���ҳ��
!insertmacro MUI_UNPAGE_INSTFILES

; ��װ�����������������
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "SimpChinese"

; ��װԤ�ͷ��ļ�
!insertmacro MUI_RESERVEFILE_LANGDLL
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS
; ------ MUI �ִ����涨����� ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "Setup.exe"
InstallDir "$PROGRAMFILES\MarketInfo"
InstallDirRegKey HKLM "${PRODUCT_UNINST_KEY}" "UninstallString"
ShowInstDetails hide
ShowUnInstDetails show
BrandingText "�������������װ��"
Function onGUIInit
	StrCpy $INSTDIR "${PRODUCT_DEFAULT_PATH}"
FunctionEnd

Section "MainSection" SEC01

;--------------------------------------------------------------
!include logiclib.nsh
FindProcDLL::FindProc "MarketInfo.exe"
StrCmp $R0 1 0 +2

messagebox::show MB_SETFOREGROUND|MB_ICONHAND|MB_DEFBUTTON3|MB_TOPMOST "${PRODUCT_NAME}" "" \
	'��⵽�����������У��Ƿ�������ֹ����$\n$\n\
	����ֹ����ֹ���򣬰�װ���°汾$\n\
	��ȡ����ȡ����װ' \
	 "��ֹ" "ȡ��"
Pop $0

${If} $0 == 1
  KillProcDLL::KillProc "MarketInfo.exe"
${Elseif} $0 == 2
  Abort
${EndIf}
;--------------------------------------------------------------

  SetOutPath "$INSTDIR"
  SetOverwrite on
  
  ; first pic
  EBanner::show /NOUNLOAD "$PLUGINSDIR\Data_1.png"
  Sleep 500
  File "mfc*.dll"
  File "msvcp*.dll"
  File "msvcr*.dll"
  
  File "commlib.dll"
  File "thostmduserapi.dll"
  File "securitymduserapi.dll"
  File "securitytraderapi.dll"
  File "MarketInfo.exe"
  
  ;LiveUpdate
  File "LiveUpdate.exe"
  File "libcurl*.dll"
  ;BugTrap
  File "BugTrap*.dll"
  
  ; second pic
  EBanner::show /NOUNLOAD "$PLUGINSDIR\Data_2.png"
  Sleep 500
  SetOutPath "$INSTDIR\config"
  File "config\futures.dat"
  File "config\ETFs.dat"
  File "config\stocks.dat"
  
  SetOutPath "$INSTDIR\config\bitmap"
  File "config\bitmap\toolbar_close.bmp"
  File "config\bitmap\toolbar_close_hover.bmp"
  File "config\bitmap\toolbar_max.bmp"
  File "config\bitmap\toolbar_max_hover.bmp"
  File "config\bitmap\toolbar_min.bmp"
  File "config\bitmap\toolbar_min_hover.bmp"
  File "config\bitmap\caption_logo.bmp"
  
  SetOutPath "$INSTDIR\config\bitmap\scroll_bar"
  File "config\bitmap\scroll_bar\DownArrow_big.bmp"
  File "config\bitmap\scroll_bar\DownArrow_window_big.bmp"
  File "config\bitmap\scroll_bar\UpArrow_big.bmp"
  File "config\bitmap\scroll_bar\UpArrow_window_big.bmp"
  File "config\bitmap\scroll_bar\VSPAN_window_big.bmp"
  File "config\bitmap\scroll_bar\VThumb1_big.bmp"
  File "config\bitmap\scroll_bar\VThumb1_window_big.bmp"
  File "config\bitmap\scroll_bar\VThumb2_window_big.bmp"
  
  ; third pic
  EBanner::show /NOUNLOAD "$PLUGINSDIR\Data_3.png"
  Sleep 500
  SetOutPath "$INSTDIR"

; ������ʼ�˵���ݷ�ʽ

  CreateDirectory "$SMPROGRAMS\${SHORTCUT_NAME}"
  CreateShortCut "$SMPROGRAMS\${SHORTCUT_NAME}\��������.lnk" "$INSTDIR\MarketInfo.exe"
  CreateShortCut "$DESKTOP\��������.lnk" "$INSTDIR\MarketInfo.exe"

SectionEnd

Section -AdditionalIcons

  WriteIniStr "$INSTDIR\${SHORTCUT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\${SHORTCUT_NAME}\����������ҳ.lnk" "$INSTDIR\${SHORTCUT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\${SHORTCUT_NAME}\ж�ؿ�������.lnk" "$INSTDIR\uninst.exe"

SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\MarketInfo.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\MarketInfo.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd

#-- ���� NSIS �ű��༭�������� Function ���α�������� Section ����֮���д���Ա��ⰲװ�������δ��Ԥ֪�����⡣--#

Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
  
  InitPluginsDir
  SetOutPath $PLUGINSDIR
  File ".\Data_*.png"
FunctionEnd

Function .onInstSuccess
  Delete ".\Data_*.png"
FunctionEnd

/******************************
 *  �����ǰ�װ�����ж�ز���  *
 ******************************/

Section Uninstall
;--------------------------------------------------------------
!include logiclib.nsh
FindProcDLL::FindProc "MarketInfo.exe"
StrCmp $R0 1 0 +2

messagebox::show MB_SETFOREGROUND|MB_ICONHAND|MB_DEFBUTTON3|MB_TOPMOST "${PRODUCT_NAME}" "" \
	'��⵽�����������У��Ƿ�������ֹ����$\n$\n\
	����ֹ����ֹ���򣬼���ж��$\n\
	��ȡ����ȡ��ж��' \
	 "��ֹ" "ȡ��"
Pop $0

${If} $0 == 1
  KillProcDLL::KillProc "MarketInfo.exe"
${Elseif} $0 == 2
  Abort
${EndIf}
;--------------------------------------------------------------
  Delete "$INSTDIR\${SHORTCUT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  ;======================================================
  Delete "$INSTDIR\mfc*.dll"
  Delete "$INSTDIR\msvcp*.dll"
  Delete "$INSTDIR\msvcr*.dll"
  
  Delete "$INSTDIR\MarketInfo.exe"
  Delete "$INSTDIR\commlib.dll"
  Delete "$INSTDIR\thostmduserapi.dll"
  Delete "$INSTDIR\securitymduserapi.dll"
  Delete "$INSTDIR\securitytraderapi.dll"
  Delete "$INSTDIR\*.con"
  
  ;LiveUpdate
  Delete "$INSTDIR\LiveUpdate.exe"
  Delete "$INSTDIR\libcurl*.dll"
  ;BugTrap
  Delete "$INSTDIR\BugTrap*.dll"

  Delete "$SMPROGRAMS\${SHORTCUT_NAME}\ж�ؿ�������.lnk"
  Delete "$SMPROGRAMS\${SHORTCUT_NAME}\����������ҳ.lnk"
  Delete "$DESKTOP\��������.lnk"
  Delete "$SMPROGRAMS\${SHORTCUT_NAME}\��������.lnk"

  RMDir /r "$SMPROGRAMS\${SHORTCUT_NAME}"
  RMDir /r "$INSTDIR\config\"

  RMDir "$INSTDIR\"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd

#-- ���� NSIS �ű��༭�������� Function ���α�������� Section ����֮���д���Ա��ⰲװ�������δ��Ԥ֪�����⡣--#

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "��ȷʵҪ��ȫ�Ƴ� $(^Name) ���������е������" IDYES +2
  Abort
FunctionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) �ѳɹ��ش����ļ�����Ƴ���"
FunctionEnd