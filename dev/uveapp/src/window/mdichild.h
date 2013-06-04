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
#ifndef MDICHILD_H
#define MDICHILD_H

#include <QtGui>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexersystemverilog.h>
#include <Qsci/qscilexerverilog.h>
#include <Qsci/qscilexervhdl.h>
#include "connect.h"

class MdiChild : public QsciScintilla
{
    Q_OBJECT

    public:
        MdiChild();
        void newFile();
        bool loadFile(const QString &fileName);
        bool save();
        bool saveAs();
        bool saveFile(const QString &fileName);
        QString userFriendlyCurrentFile();
        QString currentFile();
        void goToLine(int line);
        void goToSearchResult(int line, int index, QString term);
        void replace(QString term, QString newTerm, int atLine, int atIndex, bool cs);
        void checkModifiedFromOutside();
        void setLexerFont(const QFont &font);

    public slots:
        void closeWithoutSaving();
        void closeAndSave();
    protected:
        void closeEvent(QCloseEvent *event);
        virtual void focusInEvent ( QFocusEvent * event );

    signals:
        void cursorPositionChanged();

    private slots:
        void documentWasModified();

    private:
        bool maybeSave();
        void setCurrentFile(const QString &fileName);
        QString strippedName(const QString &fullFileName);
        QString curFile;
        QDateTime curFileDate;
        bool isUntitled;
        bool forceClose;
        bool saveAndClose;
        QsciLexer *lexer;
};

#endif
