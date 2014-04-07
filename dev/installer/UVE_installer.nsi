###############################################################################
#
# Script to generate an installer for UVE
# based on the NullSoft Scriptable Install System
# http://nsis.sourceforge.net/Main_Page
#
# Author: Oliver A. Gubler (oliver.gubler@hevs.ch).
# Version: 1.0
#
#    Copyright (C) 2013, 2014 HES-SO Valais Wallis
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
###############################################################################
!include "MUI2.nsh"
!include "LogicLib.nsh"
!include "FileFunc.nsh"
!include "nsDialogs.nsh"

# Get UVE version from current_version.h
!searchparse /file "../uveapp/src/current_version.h" `UVE_VERSION "` versionNumber `"`

# Execution Rights
RequestExecutionLevel user

# Helper defines
!define PRODUCT_NAME "UVE"
!define PRODUCT_VERSION "${versionNumber}"
!define PRODUCT_FULLNAME "${PRODUCT_NAME}_${PRODUCT_VERSION}"
!define PRODUCT_OS "win" ; win32|win64|lin32|lin64
!define PRODUCT_PUBLISHER "HES-SO (www.hes-so.ch)"
!define PRODUCT_WEB_SITE "www.systemverilog.ch"
!define BIN_PATH "\bin"
!define REGISTRY_KEY_USER "HKCU"
!define REGISTRY_ROOT ${REGISTRY_KEY_USER}
!define REGISTRY_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_FULLNAME}"
!define PRODUCT_ICON "res/logo.ico"
!define PRODUCT_LICENSE "bin/COPYING"
!define INSTALLER_MULTILANGUAGE
#!define INSTALLER_DEBUG

###############################################################################
# Variables
## Start Menu
##   used to write to registry and read back from registry
Var StartMenuFolder
Section
  StrCpy $StartMenuFolder "${PRODUCT_FULLNAME}"
SectionEnd
## Portable/Registered
Var PortableMode

###############################################################################
# Compression rules optimizations
#  We will use LZMA compression as 7Zip, with a dictionary size of 96Mb
# (to reproduce 7Zip Ultra compression mode)
SetCompress force
SetCompressor /SOLID lzma
SetDatablockOptimize on
SetCompressorDictSize 96

###############################################################################
# UI
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

## Installer Pages
### Welcome page
Section
    !ifdef INSTALLER_DEBUG
        !define MUI_WELCOMEPAGE_TITLE "DEBUG MODE!!"
    !endif
SectionEnd
!insertmacro MUI_PAGE_WELCOME
### License page
!define MUI_LICENSEPAGE_RADIOBUTTONS
!insertmacro MUI_PAGE_LICENSE ${PRODUCT_LICENSE}
### Portable/Registered page
Page Custom portableModePageCreate portableModePageLeave
### Directory page
!define MUI_PAGE_CUSTOMFUNCTION_LEAVE directoryPageLeave
!insertmacro MUI_PAGE_DIRECTORY
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

###############################################################################
# Language files (first is default)
!insertmacro MUI_LANGUAGE "English"
!ifdef INSTALLER_MULTILANGUAGE
    !insertmacro MUI_LANGUAGE "French"
    !insertmacro MUI_LANGUAGE "German"
    !insertmacro MUI_LANGUAGE "Italian"
    !insertmacro MUI_LANGUAGE "Spanish"
!endif

###############################################################################
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
LangString InstMode_Label ${LANG_FRENCH}  "Choisissez le mode d'installation:"
LangString InstMode_Label ${LANG_GERMAN}  "Wählen Sie die Installationsart:"
LangString InstMode_Label ${LANG_ITALIAN} "Select the installation mode:"
LangString InstMode_Label ${LANG_SPANISH} "Select the installation mode:"

LangString InstMode_Registered ${LANG_ENGLISH} "Registered"
LangString InstMode_Registered ${LANG_FRENCH}  "Régistré"
LangString InstMode_Registered ${LANG_GERMAN}  "Registriert"
LangString InstMode_Registered ${LANG_ITALIAN} "Registered"
LangString InstMode_Registered ${LANG_SPANISH} "Registered"

LangString InstMode_Registered_Label ${LANG_ENGLISH} "Install ${PRODUCT_NAME} on this computer and create shortcuts on the desktop and in the startmenu"
LangString InstMode_Registered_Label ${LANG_FRENCH}  "Installez ${PRODUCT_NAME} sur cet ordinateur et créez des raccourcis sur le bureau et dans le menu démarrer."
LangString InstMode_Registered_Label ${LANG_GERMAN}  "Installiert ${PRODUCT_NAME} auf diesem Computer und erstellt Verknüpfungen auf dem Desktop und im Startmenü."
LangString InstMode_Registered_Label ${LANG_ITALIAN} "Installare ${PRODUCT_NAME} sul computer."
LangString InstMode_Registered_Label ${LANG_SPANISH} "Instale ${PRODUCT_NAME} en este equipo."

LangString InstMode_Portable ${LANG_ENGLISH} "Portable"
LangString InstMode_Portable ${LANG_FRENCH}  "Portable"
LangString InstMode_Portable ${LANG_GERMAN}  "Tragbar"
LangString InstMode_Portable ${LANG_ITALIAN} "Portatile"
LangString InstMode_Portable ${LANG_SPANISH} "Portátil"

LangString InstMode_Portable_Label ${LANG_ENGLISH} "Copy the required files into the destination folder."
LangString InstMode_Portable_Label ${LANG_FRENCH}  "Copie les fichiers nécessaires dans le dossier de destination."
LangString InstMode_Portable_Label ${LANG_GERMAN}  "Kopiere die benötigten Dateien in den Zielordner."
LangString InstMode_Portable_Label ${LANG_ITALIAN} "Copia i file necessari nella cartella di destinazione."
LangString InstMode_Portable_Label ${LANG_SPANISH} "Copia los archivos necesarios en la carpeta de destino."

LangString Directory_AccessRights_Error ${LANG_ENGLISH} "You do not seem to have write access to this directory. Please choose another location or restart the installer with administrator privileges."
LangString Directory_AccessRights_Error ${LANG_FRENCH}  "Il semble que vous n'avez pas d'accès en écriture à ce répertoire. Choisissez un autre répertoire ou redémarrez le programme d'installation avec des privilèges d'administrateur."
LangString Directory_AccessRights_Error ${LANG_GERMAN}  "Sie scheinen keinen Schreibzugriff auf dieses Verzeichnis zu haben. Bitte wählen Sie ein anderes Verzeichnis oder starten Sie das Installationsprogramm mit Administratorrechten neu."
LangString Directory_AccessRights_Error ${LANG_ITALIAN} "Non sembrano avere accesso in scrittura a questa directory. Si prega di scegliere un'altra posizione o riavviare il programma di installazione con privilegi di amministratore."
LangString Directory_AccessRights_Error ${LANG_SPANISH} "No parecen tener acceso de escritura a este directorio. Por favor seleccione un directorio diferente o reiniciar el programa de instalación con privilegios de administrador."

Var boldFont
Function portableModePageCreate
    # custom font (must be inside function)
    CreateFont $boldFont "Microsoft Sans Serif" "8.25" "700"
    # create page
    nsDialogs::Create 1018
    Pop $0
    !insertmacro MUI_HEADER_TEXT "$(InstMode_Header)" "$(InstMode_SubHeader)"
    # Group Normal
    ${NSD_CreateGroupBox}    10u  10u 280u 46u ""
    Pop $0
    ## Radio Normal
    ${NSD_CreateRadioButton} 20u  20u -30u  8u "$(InstMode_Registered)"
    Pop $1
    SendMessage $1 ${WM_SETFONT} $boldFont 0
    ## Label Normal
    ${NSD_CreateLabel}       31u  36u 240u 16u "$(InstMode_Registered_Label)"
    Pop $0
    # Group Portable
    ${NSD_CreateGroupBox}    10u  60u 280u 46u ""
    Pop $0
    ## Radio Portable
    ${NSD_CreateRadioButton} 20u  70u -30u  8u "$(InstMode_Portable)"
    Pop $2
    SendMessage $2 ${WM_SETFONT} $boldFont 0
    ## Label Portable
    ${NSD_CreateLabel}       31u 86u 240u 16u "$(InstMode_Portable_Label)"
    Pop $0
    # Store selection
    ${If} $PortableMode = 0
        SendMessage $1 ${BM_SETCHECK} ${BST_CHECKED} 0
    ${Else}
        SendMessage $2 ${BM_SETCHECK} ${BST_CHECKED} 0
    ${EndIf}
    nsDialogs::Show
FunctionEnd

Function portableModePageLeave
    ${NSD_GetState} $1 $0
    ${If} $0 <> ${BST_UNCHECKED}
        StrCpy $PortableMode 0
    ${Else}
        StrCpy $PortableMode 1
    ${EndIf}
FunctionEnd

Function directoryPageLeave
    ClearErrors
    FileOpen $R0 $INSTDIR\..\tmp.dat w
    FileClose $R0
    Delete $INSTDIR\..\tmp.dat
    ${If} ${Errors}
        MessageBox MB_OK|MB_ICONSTOP  "$(Directory_AccessRights_Error)"
        Abort
    ${EndIf}
FunctionEnd
    
###############################################################################
# Configuration
Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "${PRODUCT_NAME}_${PRODUCT_VERSION}_installer_${PRODUCT_OS}.exe"
InstallDir "$LOCALAPPDATA\${PRODUCT_FULLNAME}" ; sets the path for $INSTDIR
InstallDirRegKey "${REGISTRY_ROOT}" "${REGISTRY_KEY}" ""
BrandingText "${PRODUCT_PUBLISHER}"
InstProgressFlags smooth

###############################################################################
# Init Functions
## Installer
Function .onInit
    # Show language display
    !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

## Uninstaller
Function un.onInit
    # Get language used during installation
    !insertmacro MUI_UNGETLANGUAGE
FunctionEnd

###############################################################################
# Sections
## Application
Section "Application" Section1
    SetOutPath "$INSTDIR"
    SetOverwrite try
    # Files
    !ifdef INSTALLER_DEBUG
        ; only one file is included, reduces compilation,un-/installation time compared to full database
        File "bin\COPYING"
    !else
        File /r "bin\*.*"
    !endif
    ${If} $PortableMode = 0
        # Shortcuts
        ## Desktop
        CreateShortCut  "$DESKTOP\${PRODUCT_FULLNAME}.lnk"                               "$INSTDIR${BIN_PATH}\${PRODUCT_NAME}.exe"
        ## Start Menu
        CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
        CreateShortCut  "$SMPROGRAMS\$StartMenuFolder\${PRODUCT_FULLNAME}.lnk"           "$INSTDIR${BIN_PATH}\${PRODUCT_NAME}.exe" ""    "" "" "" "" "the UVE program"
        WriteINIStr     "$SMPROGRAMS\$StartMenuFolder\${PRODUCT_NAME} Webpage.url"       "InternetShortcut"                        "URL" "${PRODUCT_WEB_SITE}"
        CreateShortCut  "$SMPROGRAMS\$StartMenuFolder\Uninstall ${PRODUCT_FULLNAME}.lnk" "$INSTDIR\uninst.exe"                      ""    "" "" "" "" "the UVE uninstaller"
        !ifdef INSTALLER_DEBUG
            DetailPrint "Start Menu Folder: $SMPROGRAMS\$StartMenuFolder\"
        !endif
    ${EndIf}
SectionEnd

## Registry
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
        WriteRegDWORD "${REGISTRY_ROOT}" "${REGISTRY_KEY}" "NoModify" 0
        WriteRegDWORD "${REGISTRY_ROOT}" "${REGISTRY_KEY}" "NoRepair" 0
    ${EndIf}
SectionEnd

## Uninstaller
Section Uninstall
    # Files
    RMDir /r /REBOOTOK $INSTDIR
    # Shortcuts
    ## Desktop
    Delete "$DESKTOP\${PRODUCT_FULLNAME}.lnk"
    ## Start Menu
    ReadRegStr "$StartMenuFolder" "${REGISTRY_ROOT}" "${REGISTRY_KEY}" "StartMenuFolder"
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
            DetailPrint "Failed deleting Registry Key ${REGISTRY_ROOT} ${REGISTRY_KEY}"
    !endif
SectionEnd
