 /*
    UVE is a free open source software able to automatically generate
    UVM/SystemVerilog testbenches
    Copyright (C) 2012 HES-SO

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef UVECONSTANTS_H
#define UVECONSTANTS_H

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>

/** This file contains some constant values and strings used troughout UVE
  */

// Webpage
#define UVE_WEBPAGE "www.systemverilog.ch"
#define UVE_WEBPAGE_TICKETS "https://github.com/uve-project/uve/issues/new"
#define UVE_WEBPAGE_DOWNLOAD UVE_WEBPAGE "/downloads"
#define UVE_WEBPAGE_FEED UVE_WEBPAGE "/feed"

// Copyright
#define UVE_COPYRIGHT_SHORT     "Copyright 2013 HES-SO (www.hes-so.ch)"
#define UVE_COPYRIGHT_LONG_RICH UVE_COPYRIGHT_SHORT "<br /><br />This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of DESIGN, MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE."

// Paths
#define UVE_APPLICATION_PATH    QCoreApplication::applicationDirPath()  //the directory that contains the application executable
#define UVE_BASE_PATH           UVE_APPLICATION_PATH + QDir::separator() + ".."
#define UVE_MANUAL              UVE_BASE_PATH + "/UVEUserManualenglish/UVEUserManual_english_.html"
#define UVE_LIB                 UVE_BASE_PATH + QDir::separator() +  "library"

//Configuration settings
// -- store settings in a local file
#define CONF_SETTINGS       UVE_APPLICATION_PATH + "/settings.ini", QSettings::IniFormat
#define CONF_SIMULATOR      "projectdialogview/lastsimulator"
#define CONF_PROJECTFOLDER  "projectdialogview/projectfolder"
#define CONF_VHDLFOLDER     "projectdialogview/vhdlfolder"
#define CONF_VIPFOLDER      "envdialogview/vippath"
#define CONF_TEMPLATEFOLDER "projectdialogview/templatefolder"
#define CONF_LASTPROJECTFOLDER "mainwindow/lastprojectfolder"
#define CONF_FONT           "mdichild/font"

#define DEFAULT_FONT        QFont("Courier",9)

#endif // UVECONSTANTS_H
