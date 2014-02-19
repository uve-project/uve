########################################################################################################################
# 
# Script to generate an installer for UVE
# based on the NullSoft Scriptable Install System (http://nsis.sourceforge.net/Main_Page)
#
# A desktop icon will be generated. To uninstall execute the uninstaller in the installation directory.
#
# Author: Oliver A. Gubler (oliver.gubler@hevs.ch).
# Version: 0.0.20120511
# 
#    Copyright (C) 2012  HES-SO Valais Wallis
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
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
########################################################################################################################

# Helper defines
!define PRODUCT_NAME "UVE"
!define PRODUCT_VERSION "0.0"
!define PRODUCT_FULLNAME "${PRODUCT_NAME}_${PRODUCT_VERSION}"
!define PRODUCT_OS "win32" ; win32|win64|lin32|lin64
!define PRODUCT_PUBLISHER "HES-SO Valais Wallis, HEIG-VD"
;!define PRODUCT_WEB_SITE "uve.hevs.ch"
!define REGISTRY_ROOT "HKLM"
!define REGISTRY_KEY "Software\${PRODUCT_NAME}"
!define PRODUCT_ICON "res/logo.ico"
!define PRODUCT_LICENSE "res/COPYING"
;!define INSTALLER_DEBUG

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

##  Installer Pages
###  Welcome page
!insertmacro MUI_PAGE_WELCOME
###  License page
!define MUI_LICENSEPAGE_RADIOBUTTONS
!insertmacro MUI_PAGE_LICENSE ${PRODUCT_LICENSE}
###  Directory page
!insertmacro MUI_PAGE_DIRECTORY
###  Instfiles page
!insertmacro MUI_PAGE_INSTFILES
###  Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\${PRODUCT_NAME}.exe"
!insertmacro MUI_PAGE_FINISH

##  Uninstaller pages
###  Welcome page
!insertmacro MUI_UNPAGE_WELCOME
###  Confirm page
!insertmacro MUI_UNPAGE_CONFIRM
###  Instfiles page
!insertmacro MUI_UNPAGE_INSTFILES
###  Finish page
Section
  !ifdef INSTALLER_DEBUG
    !define MUI_UNFINISHPAGE_NOAUTOCLOSE
  !endif
SectionEnd
!insertmacro MUI_UNPAGE_FINISH

# Language files (first is default)
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "French"
!insertmacro MUI_LANGUAGE "German"
!insertmacro MUI_LANGUAGE "Italian"
!insertmacro MUI_LANGUAGE "Spanish"

# User functions
Function myGUIInit
  Aero::Apply
FunctionEnd

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "${PRODUCT_NAME}_${PRODUCT_VERSION}_installer_${PRODUCT_OS}.exe"
InstallDir "$PROGRAMFILES\${PRODUCT_FULLNAME}"
InstallDirRegKey "${REGISTRY_ROOT}" "${REGISTRY_KEY}" ""
BrandingText "${PRODUCT_PUBLISHER}"
InstProgressFlags smooth

Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

Section "Application" Section1
  SetOutPath "$INSTDIR"
  SetOverwrite try
  # Files
  !ifdef INSTALLER_DEBUG
    ; only one file is included, reduces compilation,un-/installation time compared to full database
    File "res\COPYING"
  !else
    File /r "bin\*.*"
  !endif
  # Shortcuts
  ## Desktop
  CreateShortCut "$DESKTOP\${PRODUCT_FULLNAME}.lnk" "$INSTDIR\${PRODUCT_NAME}.exe"
SectionEnd

Section -FinishSection
  WriteUninstaller "$INSTDIR\uninst.exe"
  # Register @ Windows
  WriteRegStr "${REGISTRY_ROOT}" "${REGISTRY_KEY}" ""                "$INSTDIR\${PRODUCT_NAME}.exe"
  WriteRegStr "${REGISTRY_ROOT}" "${REGISTRY_KEY}" "DisplayName"     "$(^Name)"
  WriteRegStr "${REGISTRY_ROOT}" "${REGISTRY_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr "${REGISTRY_ROOT}" "${REGISTRY_KEY}" "DisplayIcon"     "$INSTDIR\${PRODUCT_NAME}.exe"
  WriteRegStr "${REGISTRY_ROOT}" "${REGISTRY_KEY}" "DisplayVersion"  "${PRODUCT_VERSION}"
  WriteRegStr "${REGISTRY_ROOT}" "${REGISTRY_KEY}" "URLInfoAbout"    "${PRODUCT_WEB_SITE}"
  WriteRegStr "${REGISTRY_ROOT}" "${REGISTRY_KEY}" "Publisher"       "${PRODUCT_PUBLISHER}"
SectionEnd

Function un.onInit
  !insertmacro MUI_UNGETLANGUAGE
FunctionEnd

Section Uninstall
  # Files
  RMDir /r /REBOOTOK $INSTDIR
  # Shortcuts
  ## Desktop
  Delete "$DESKTOP\${PRODUCT_FULLNAME}.lnk"
  # Registry
  DeleteRegKey "${REGISTRY_ROOT}" "${REGISTRY_KEY}"
SectionEnd
