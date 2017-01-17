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
/*
 Auteur        : Yann Thoma
                 Professeur HEIG-VD
                 Departement TIC

 Modifications : Romain Cornaz
                 Etudiant HEIG-VD
                 Departement TIC
 Date   : 28.07.11

 Modifications : Yann Thoma
 Date   : 09.03.2016
 Description: Removed dependency on QtGui

 */

#ifndef VHDLUTILS_H
#define VHDLUTILS_H

#include <QString>
#include <QStringList>
#include <QFile>

class AnalyzerMessage
{
    public:
	enum MessageType {NONE,PARSING,SIGNAME,PORTNAME};

    public:
	AnalyzerMessage();
	AnalyzerMessage(int line,MessageType type,QString variable);
	QString message();

	int m_line;
	MessageType m_type;
	QString m_variable;
};


class VHDLUtils
{
    public:
	static VHDLUtils *getReference();

	QStringList getVhdlKeywords();
	QStringList getVhdlTypes();

	bool showSpaces();
	bool showEndOfLines();

	void setShowSpaces(bool set);
	void setShowEndOfLines(bool set);

    protected:
        VHDLUtils();
	static VHDLUtils *singleton;

	QStringList listFromFile(const QString& fileName);
	QStringList vhdlKeywords;
	QStringList vhdlTypes;

	bool m_showSpaces;
	bool m_showEndOfLines;
};

#endif // VHDLUTILS_H
