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
#include "vhdlutils.h"

VHDLUtils *VHDLUtils::singleton=0;


AnalyzerMessage::AnalyzerMessage()
{
    m_line=0;
    m_type=NONE;
    m_variable="";
}


AnalyzerMessage::AnalyzerMessage(int line,MessageType type,QString variable)
{
    m_line=line;
    m_type=type;
    m_variable=variable;
}


QString AnalyzerMessage::message()
{
    switch (m_type)
    {
        case NONE:
            return "No message"; break;
        case PARSING:
            return "Parsing error"; break;
        case SIGNAME:
            return "Signal name \""+m_variable+"\" not corresponding to the methodology";break;
        case PORTNAME:
            return "Port name \""+m_variable+"\" not corresponding to the methodology";break;
        default:
            return "Internal error";break;
    }
}


VHDLUtils::VHDLUtils()
{
    vhdlKeywords=listFromFile(":/resources/vhdlkeywords.txt");
    vhdlTypes=listFromFile(":/resources/vhdltypes.txt");

    m_showSpaces=false;
    m_showEndOfLines=false;
}


QStringList VHDLUtils::listFromFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return QStringList();

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QStringList words=QStringList();

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if (!line.isEmpty())
            words << line.trimmed();
    }

    QApplication::restoreOverrideCursor();
    return words;

}

VHDLUtils *VHDLUtils::getReference()
{
    if (!singleton)
        singleton=new VHDLUtils();
    return singleton;
}


QStringList VHDLUtils::getVhdlKeywords()
{
    return vhdlKeywords;
}


QStringList VHDLUtils::getVhdlTypes()
{
    return vhdlTypes;
}


bool VHDLUtils::showSpaces()
{
    return m_showSpaces;
}


bool VHDLUtils::showEndOfLines()
{
    return m_showEndOfLines;
}


void VHDLUtils::setShowSpaces(bool set)
{
    m_showSpaces=set;
}


void VHDLUtils::setShowEndOfLines(bool set)
{
    m_showEndOfLines=set;
}
