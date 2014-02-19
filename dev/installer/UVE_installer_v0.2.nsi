########################################################################################################################
# 
# Script to generate an installer for UVE
# based on the NullSoft Scriptable Install System (http://nsis.sourceforge.net/Main_Page)
#
# Author: Oliver A. Gubler (oliver.gubler@hevs.ch).
# Version: 0.2.0116
# 
#    Copyright (C) 2013  HES-SO Valais Wallis
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program. If not, see <http://www.gnu.org/licenses/>.
#
########################################################################################################################

# Get version from current_version.h
!searchparse /file "../uveapp/src/current_version.h" `UVE_VERSION "` versionNumber `"`

# Helper defines
!define PRODUCT_NAME "UVE"
!define PRODUCT_VERSION "${versionNumber}"
!define PRODUCT_FULLNAME "${PRODUCT_NAME}_${PRODUCT_VERSION}"
!define PRODUCT_OS "win32" ; win32|win64|lin32|lin64
!define PRODUCT_PUBLISHER "HES-SO (www.hes-so.ch)"
!define PRODUCT_WEB_SITE "www.systemverilog.ch"
!define BIN_PATH "\bin"
!define REGISTRY_ROOT "HKLM"
!define REGISTRY_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_FULLNAME}" ;"Software\${PRODUCT_NAME}"
!define PRODUCT_ICON "res/logo.ico"
!define PRODUCT_LICENSE "bin/COPYING"
!define INSTALLER_MULTILANGUAGE
!define INSTALLER_DEBUG

# Variables
## Start Menu
Var StartMenuFolder
Section
  StrCpy $StartMenuFolder "${PRODUCT_FULLNAME}"
SectionEnd
## Portable
Var PortableMode

#Portable
#!include "LogicLib.nsh"
!include "FileFunc.nsh"
!include "Sections.nsh"

# UI
!include "MUI2.nsh"
!define MUI_CUSTOMFUNCTION_GUIINIT myGUIInit

## MUI Settings
!define MUI_ABORTWARNING
!define MUI_UNABORTWARNING
!define MUI_ICON ${PRODUCT_ICON}
!define MUI_UNICON ${PRODUCT_ICON}
### Remember the Installer Language for the Uninstaller
!define MUI_LANGDLL_REGISTRY_ROOT "${REGISTRY_ROOT}"
!define MUI_LANGDLL_REGISTRY_KEY "${REGISTRY_KEY}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"
### Load Language files first (speeds up loading of installer)
!insertmacro MUI_RESERVEFILE_LANGDLL
### Start Menu folder page configuration
;!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${REGISTRY_ROOT}"
;!define MUI_STARTMENUPAGE_REGISTRY_KEY "${REGISTRY_KEY}"
;!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "NSIS:StartMenuFolder"

## Installer Pages
### Welcome page
!insertmacro MUI_PAGE_WELCOME
### License page
!define MUI_LICENSEPAGE_RADIOBUTTONS
!insertmacro MUI_PAGE_LICENSE ${PRODUCT_LICENSE}
### Portable page
Page Custom PortableModePageCreate PortableModePageLeave
### Directory page
!insertmacro MUI_PAGE_DIRECTORY
### Start Menu folder page
; not compatible with portable mode;!insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
### Instfiles page
!insertmacro MUI_PAGE_INSTFILES
### Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR${BIN_PATH}\${PRODUCT_NAME}.exe"
Section
  !ifdef INSTALLER_DEBUG
    !define MUI_FINISHPAGE_NOAUTOCLOSE
  !endif
SectionEnd
!insertmacro MUI_PAGE_FINISH

## Uninstaller pages
### Welcome page
!insertmacro MUI_UNPAGE_WELCOME
### Confirm page
!insertmacro MUI_UNPAGE_CONFIRM
### Instfiles page
!insertmacro MUI_UNPAGE_INSTFILES
### Finish page
Section
  !ifdef INSTALLER_DEBUG
    !define MUI_UNFINISHPAGE_NOAUTOCLOSE
  !endif
SectionEnd
!insertmacro MUI_UNPAGE_FINISH

# Language files (first is default)
!insertmacro MUI_LANGUAGE "English"
!ifdef INSTALLER_MULTILANGUAGE
  !insertmacro MUI_LANGUAGE "French"
  !insertmacro MUI_LANGUAGE "German"
  !insertmacro MUI_LANGUAGE "Italian"
  !insertmacro MUI_LANGUAGE "Spanish"
!endif

# User functions
## Aero GUI for Windows Vista/7
Function myGUIInit
  Aero::Apply
FunctionEnd

LangString InstMode_Header ${LANG_ENGLISH} "Choose Installation Mode"
LangString InstMode_Header ${LANG_FRENCH}  "Choisissez le mode d'installation"
LangString InstMode_Header ${LANG_GERMAN}  "Installationsart auswählen"
LangString InstMode_Header ${LANG_ITALIAN} "Selezionare la modalità di installazione"
LangString InstMode_Header ${LANG_SPANISH} "Seleccione el modo de instalación"

LangString InstMode_SubHeader ${LANG_ENGLISH} "Choose how you want to install ${PRODUCT_NAME}."
LangString InstMode_SubHeader ${LANG_FRENCH}  "Choisissez comment installer ${PRODUCT_NAME}."
LangString InstMode_SubHeader ${LANG_GERMAN}  "Wählen Sie die Art, wie ${PRODUCT_NAME} installiert werden soll."
LangString InstMode_SubHeader ${LANG_ITALIAN} "Scegliere come installare ${PRODUCT_NAME}."
LangString InstMode_SubHeader ${LANG_SPANISH} "Elija la forma de instalar ${PRODUCT_NAME}."

LangString InstMode_Label ${LANG_ENGLISH} "Select the installation mode:"
LangString InstMode_Label ${LANG_FRENCH}  "Select the installation mode:"
LangString InstMode_Label ${LANG_GERMAN}  "Select the installation mode:"
LangString InstMode_Label ${LANG_ITALIAN} "Select the installation mode:"
LangString InstMode_Label ${LANG_SPANISH} "Select the installation mode:"

LangString InstMode_Normal ${LANG_ENGLISH} "Normal"
LangString InstMode_Normal ${LANG_FRENCH}  "Normal"
LangString InstMode_Normal ${LANG_GERMAN}  "Normal"
LangString InstMode_Normal ${LANG_ITALIAN} "Normale"
LangString InstMode_Normal ${LANG_SPANISH} "Normal"

LangString InstMode_Normal_Label ${LANG_ENGLISH} "Install ${PRODUCT_NAME} for all users on this computer."
LangString InstMode_Normal_Label ${LANG_FRENCH}  "Installez ${PRODUCT_NAME} pour tous les utilisateurs de cet ordinateur."
LangString InstMode_Normal_Label ${LANG_GERMAN}  "Installiert ${PRODUCT_NAME} für alle Benutzer auf diesem Computer."
LangString InstMode_Normal_Label ${LANG_ITALIAN} "Installare ${PRODUCT_NAME} per tutti gli utenti del computer."
LangString InstMode_Normal_Label ${LANG_SPANISH} "Instale ${PRODUCT_NAME} para todos los usuarios de este equipo"

LangString InstMode_Portable ${LANG_ENGLISH} "Portable"
LangString InstMode_Portable ${LANG_FRENCH}  "Portable"
LangString InstMode_Portable ${LANG_GERMAN}  "Tragbar"
LangString InstMode_Portable ${LANG_ITALIAN} "Portatile"
LangString InstMode_Portable ${LANG_SPANISH} "Portátil"

LangString InstMode_Portable_Label ${LANG_ENGLISH} "Copies the required files into the destination folder. Does not write to the registry and no shortcuts are created."
LangString InstMode_Portable_Label ${LANG_FRENCH}  "Copie les fichiers nécessaires dans le dossier de destination. N'écrit pas dans le registre et ne crée pas de raccourcis."
LangString InstMode_Portable_Label ${LANG_GERMAN}  "Kopiert die benötigten Dateien in den Zielordner. Es wird nicht in die Registry geschreiben und keine Dateiverknüpfungen werden erstellt."
LangString InstMode_Portable_Label ${LANG_ITALIAN} "Copia i file necessari nella cartella di destinazione. Non scrivere nel Registro di sistema e non crea scorciatoie."
LangString InstMode_Portable_Label ${LANG_SPANISH} "Copia los archivos necesarios en la carpeta de destino. No escriba en el registro y no crea accesos directos."

Var boldFont
Function PortableModePageCreate
  # custom font
  CreateFont $boldFont "Microsoft Sans Serif" "8.25" "700"
  # create page
  nsDialogs::Create 1018
  Pop $0
  !insertmacro MUI_HEADER_TEXT "$(InstMode_Header)" "$(InstMode_SubHeader)"
  # Title
  ${NSD_CreateLabel}         0  10u 100%  8u "$(InstMode_Label)"
  Pop $0
  # Group Normal
  ${NSD_CreateGroupBox}    10u  30u 280u 46u ""
  Pop $0
  ## Radio Normal
  ${NSD_CreateRadioButton} 20u  40u -30u  8u "$(InstMode_Normal)"
  Pop $1
  ${NSD_AddStyle} $1 ${WS_GROUP}
  SendMessage $1 ${WM_SETFONT} $boldFont 0
  ## Label Normal
  ${NSD_CreateLabel}       31u  56u 240u 16u "$(InstMode_Normal_Label)"
  Pop $0
  # Group Portable
  ${NSD_CreateGroupBox}    10u  80u 280u 46u ""
  Pop $0
  ## Radio Portable
  ${NSD_CreateRadioButton} 20u  90u -30u  8u "$(InstMode_Portable)"
  Pop $2
  ${NSD_AddStyle} $2 ${WS_GROUP}
  SendMessage $2 ${WM_SETFONT} $boldFont 0
  ## Label Portable
  ${NSD_CreateLabel}       31u 106u 240u 16u "$(InstMode_Portable_Label)"
  Pop $0
  # Store selection
  ${If} $PortableMode = 0
    SendMessage $1 ${BM_SETCHECK} ${BST_CHECKED} 0
  ${Else}
    SendMessage $2 ${BM_SETCHECK} ${BST_CHECKED} 0
  ${EndIf}
  nsDialogs::Show
FunctionEnd

Function PortableModePageLeave
  ${NSD_GetState} $1 $0
  ${If} $0 <> ${BST_UNCHECKED}
    StrCpy $PortableMode 0
    Call RequireAdmin
  ${Else}
    StrCpy $PortableMode 1
  ${EndIf}
FunctionEnd

Function RequireAdmin
UserInfo::GetAccountType
Pop $8
${If} $8 != "admin"
    MessageBox MB_ICONSTOP "You need administrator rights to install ${PRODUCT_NAME}"
    SetErrorLevel 740 ;ERROR_ELEVATION_REQUIRED
    Abort
${EndIf}
FunctionEnd

# Configuration
Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "${PRODUCT_NAME}_${PRODUCT_VERSION}_installer_${PRODUCT_OS}.exe"
InstallDir "$PROGRAMFILES\${PRODUCT_FULLNAME}"
InstallDirRegKey "${REGISTRY_ROOT}" "${REGISTRY_KEY}" ""
BrandingText "${PRODUCT_PUBLISHER}"
InstProgressFlags smooth

Function .onInit
  # Language
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

Section "Application" Section1
  ${If} $PortableMode = 0
    # Install for every user
    SetShellVarContext all
  ${EndIf}
  SetOutPath "$INSTDIR"
  SetOverwrite try
  # Files
;  !ifdef INSTALLER_DEBUG
;    ; only one file is included, reduces compilation,un-/installation time compared to full database
;    File "res\COPYING"
;  !else
    File /r "bin\*.*"
;  !endif
  ${If} $PortableMode = 0
    # Shortcuts
    ## Desktop
    CreateShortCut  "$DESKTOP\${PRODUCT_FULLNAME}.lnk"                               "$INSTDIR${BIN_PATH}\${PRODUCT_NAME}.exe"
    ## Start Menu
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortCut  "$SMPROGRAMS\$StartMenuFolder\${PRODUCT_FULLNAME}.lnk"           "$INSTDIR${BIN_PATH}\${PRODUCT_NAME}.exe" ""    "" "" "" "" "the UVE program"
    WriteINIStr     "$SMPROGRAMS\$StartMenuFolder\${PRODUCT_NAME} Webpage.url"       "InternetShortcut"                         "URL" "${PRODUCT_WEB_SITE}"
    CreateShortCut  "$SMPROGRAMS\$StartMenuFolder\Uninstall ${PRODUCT_FULLNAME}.lnk" "$INSTDIR${BIN_PATH}\uninst.exe"          ""    "" "" "" "" "the UVE uninstaller"
    !ifdef INSTALLER_DEBUG
      DetailPrint "Start Menu Folder: $SMPROGRAMS\$StartMenuFolder\"
    !endif
  ${EndIf}

SectionEnd

Section -FinishSection
  ${If} $PortableMode = 0
    WriteUninstaller "$INSTDIR\uninst.exe"
    # Register @ Windows
    WriteRegStr "${REGISTRY_ROOT}" "${REGISTRY_KEY}" ""                "$INSTDIR${BIN_PATH}\${PRODUCT_NAME}.exe"
    WriteRegStr "${REGISTRY_ROOT}" "${REGISTRY_KEY}" "DisplayName"     "$(^Name)"
    WriteRegStr "${REGISTRY_ROOT}" "${REGISTRY_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
    WriteRegStr "${REGISTRY_ROOT}" "${REGISTRY_KEY}" "DisplayIcon"     "$INSTDIR${BIN_PATH}\${PRODUCT_NAME}.exe"
    WriteRegStr "${REGISTRY_ROOT}" "${REGISTRY_KEY}" "DisplayVersion"  "${PRODUCT_VERSION}"
    WriteRegStr "${REGISTRY_ROOT}" "${REGISTRY_KEY}" "URLInfoAbout"    "${PRODUCT_WEB_SITE}"
    WriteRegStr "${REGISTRY_ROOT}" "${REGISTRY_KEY}" "URLInfoUpdate"   "${PRODUCT_WEB_SITE}/downloads"
    WriteRegStr "${REGISTRY_ROOT}" "${REGISTRY_KEY}" "Publisher"       "${PRODUCT_PUBLISHER}"
    WriteRegStr "${REGISTRY_ROOT}" "${REGISTRY_KEY}" "StartMenuFolder" "$StartMenuFolder"
    WriteRegDWORD "${REGISTRY_ROOT}" "${REGISTRY_KEY}" "NoModify" 1
    WriteRegDWORD "${REGISTRY_ROOT}" "${REGISTRY_KEY}" "NoRepair" 1
  ${EndIf}
SectionEnd

Function un.onInit
  !insertmacro MUI_UNGETLANGUAGE
FunctionEnd

Section Uninstall
  # UnInstall for every user
  SetShellVarContext all
  # Files
  RMDir /r /REBOOTOK $INSTDIR
  # Shortcuts
  ## Desktop
  Delete "$DESKTOP\${PRODUCT_FULLNAME}.lnk"
  ## Start Menu
  ReadRegStr $StartMenuFolder "${REGISTRY_ROOT}" "${REGISTRY_KEY}" "StartMenuFolder"
  !ifdef INSTALLER_DEBUG
    DetailPrint "Start Menu Folder: $SMPROGRAMS\$StartMenuFolder\"
  !endif
  Delete "$SMPROGRAMS\$StartMenuFolder\${PRODUCT_FULLNAME}.lnk"
  !ifdef INSTALLER_DEBUG
    IfErrors 0 +2
      DetailPrint "Failed deleting $SMPROGRAMS\$StartMenuFolder\${PRODUCT_FULLNAME}.lnk"
  !endif
  DeleteINIStr "$SMPROGRAMS\$StartMenuFolder\${PRODUCT_NAME} Webpage.url" "InternetShortcut" "URL"
  !ifdef INSTALLER_DEBUG
    IfErrors 0 +2
      DetailPrint "Failed deleting $SMPROGRAMS\$StartMenuFolder\${PRODUCT_NAME} Webpage.url"
  !endif
  Delete "$SMPROGRAMS\$StartMenuFolder\uninstall.lnk"
  !ifdef INSTALLER_DEBUG
    IfErrors 0 +2
      DetailPrint "Failed deleting $SMPROGRAMS\$StartMenuFolder\uninstall.lnk"
  !endif
  RMDir /r "$SMPROGRAMS\$StartMenuFolder"
  !ifdef INSTALLER_DEBUG
    IfErrors 0 +2
      DetailPrint "Failed removing directory $SMPROGRAMS\$StartMenuFolder"
  !endif
  # Registry
  DeleteRegKey "${REGISTRY_ROOT}" "${REGISTRY_KEY}"
  !ifdef INSTALLER_DEBUG
    IfErrors 0 +2
      DetailPrint "Failed deleting Registery Key ${REGISTRY_ROOT} ${REGISTRY_KEY}"
  !endif
SectionEnd
