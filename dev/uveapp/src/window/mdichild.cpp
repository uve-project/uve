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
#include "mdichild.h"

MdiChild::MdiChild()
{
    setAttribute(Qt::WA_DeleteOnClose);
    isUntitled = true;
    forceClose = false;
    saveAndClose = false;
    lexer = 0;

    setAutoIndent(true);
    setMarginLineNumbers(1,true);
    setFolding(QsciScintilla::BoxedFoldStyle);
    setMarginType(1, QsciScintilla::NumberMargin);
    setMarginWidth(1,"00000");
    setAutoCompletionSource(QsciScintilla::AcsDocument);
    setAutoCompletionThreshold(2);
}


void MdiChild::newFile()
{
    static int sequenceNumber = 1;
    isUntitled = true;
    curFile = tr("document%1.sv").arg(sequenceNumber++);
    lexer=new QsciLexerSystemVerilog();

    lexer->setFont(QFont("Courier", 9),-1);
    setLexer(lexer);

    //setCurrentFile(curFile);
    curFileDate= QDateTime();
    setWindowTitle(curFile + "[*]");
    CONNECT(this, SIGNAL(textChanged()), this, SLOT(documentWasModified()));
}


bool MdiChild::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("UVE"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    setText(in.readAll());
    QApplication::restoreOverrideCursor();
    setCurrentFile(fileName);
    CONNECT(this, SIGNAL(textChanged()), this, SLOT(documentWasModified()));

    return true;
}

void MdiChild::closeWithoutSaving()
{
    forceClose = true;
    close();
}

void MdiChild::closeAndSave()
{
    saveAndClose = true;
    close();
}

bool MdiChild::save()
{
    if (isUntitled) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}


bool MdiChild::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), curFile);
    if (fileName.isEmpty())
        return false;
    return saveFile(fileName);
}


bool MdiChild::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("UVE"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << text();
    QApplication::restoreOverrideCursor();

    file.close();

    setCurrentFile(fileName);
    return true;
}


QString MdiChild::userFriendlyCurrentFile()
{
    return strippedName(curFile);
}


QString MdiChild::currentFile()
{
    return curFile;
}


// Go to a specific line of the text
void MdiChild::goToLine(int line)
{
    this->setFocus();
    this->setCursorPosition(line, 0);
}

void MdiChild::goToSearchResult(int line, int index, QString term)
{
    this->setFocus();
    this->setSelection(line, index, line, index+term.length());
    if(! selectedText().compare(term) == 0)
        goToLine(line);
}

void MdiChild::replace(QString term, QString newTerm, int atLine, int atIndex, bool cs)
{
    Qt::CaseSensitivity caseSensitivity;
    if(cs)
        caseSensitivity = Qt::CaseSensitive;
    else
        caseSensitivity = Qt::CaseInsensitive;

    setSelection(atLine, atIndex, atLine, atIndex + term.length());
    if(selectedText().compare(term, caseSensitivity) == 0)
    {
        replaceSelectedText(newTerm);
    }
    else
        qDebug() << "Replacement failed : the text to be replaced was not found. ";
}

void MdiChild::closeEvent(QCloseEvent *event)
{
    if (forceClose || maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}


#include <QMutex>

void MdiChild::checkModifiedFromOutside()
{
    static QMutex mutex;
    if (!mutex.tryLock())
        return;
    QDateTime t=QFileInfo(curFile).lastModified();
    if ((curFileDate!=QDateTime())&&(QFileInfo(curFile).lastModified()!=curFileDate)) {
        if (QMessageBox::Yes==
                QMessageBox::question(this,
                                      "File modified",
                                      QString("The file %1 has been modified outside the editor. Would you like to reload it?").arg(curFile),
                                      QMessageBox::Yes|QMessageBox::No))
        {
            QFile file(curFile);
            if (!file.open(QFile::ReadOnly | QFile::Text)) {
                QMessageBox::warning(this, tr("UVE"),
                                     tr("Cannot read file %1:\n%2.")
                                     .arg(curFile)
                                     .arg(file.errorString()));
            }
           else {
                QTextStream in(&file);
                QApplication::setOverrideCursor(Qt::WaitCursor);
                setText(in.readAll());
                QApplication::restoreOverrideCursor();
                setCurrentFile(curFile);
            }
        }
        else {
            curFileDate=QFileInfo(curFile).lastModified();
        }
    }
    mutex.unlock();
}

void MdiChild::focusInEvent ( QFocusEvent * event )
{
    checkModifiedFromOutside();
    QsciScintilla::focusInEvent(event);
}


void MdiChild::documentWasModified()
{
    setWindowModified(isModified());
}


bool MdiChild::maybeSave()
{
    if (isModified()) {
        if(saveAndClose)
        {
            return save();
        }
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("UVE"),
                     tr("'%1' has been modified.\n"
                        "Do you want to save your changes?")
                     .arg(userFriendlyCurrentFile()),
                     QMessageBox::Save | QMessageBox::Discard
                     | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}


void MdiChild::setCurrentFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    curFileDate = QFileInfo(fileName).lastModified();
    isUntitled = false;
    setModified(false);
    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");

    if ((curFile.contains(".vhd"))||(curFile.contains(".vhdl")))
    {
        if (!dynamic_cast<QsciLexerVHDL *>(lexer))
        {
            if (lexer)
                delete lexer;
            lexer=new QsciLexerVHDL();
        }
    }
    else //if ((curFile.contains(".sv"))||(curFile.contains(".svh")))
    {
        if (!dynamic_cast<QsciLexerSystemVerilog *>(lexer))
        {
            if (lexer)
                delete lexer;
            lexer = new QsciLexerSystemVerilog();
            lexer->setColor(QColor(200,0,0),QsciLexerSystemVerilog::KeywordUvmMacro);
        }
    }

    lexer->setFont(QFont("Courier", 9),-1);
    setLexer(lexer);
}


QString MdiChild::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
