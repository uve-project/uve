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
#include "filelineedit.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QDir>

//Call init with default value
FileLineEdit::FileLineEdit(QWidget *parent) :
    QWidget(parent)
{
    init(tr("File selection"),QFileDialog::AnyFile);
}

//Call init with the given values
FileLineEdit::FileLineEdit(QString title, QFileDialog::FileMode mode)
{
    init(title,mode);
}

//Construct the base GUI, and connect the signals
void FileLineEdit::init(QString title, QFileDialog::FileMode mode)
{
    //Layout
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);

    //Line edit
    _line = new QLineEdit();
    layout->addWidget(_line);
    CONNECT(_line, SIGNAL(textChanged(QString)), this, SLOT(updateFileName(QString)));

    //Button
    QPushButton *button = new QPushButton(tr("Select..."));
    //Reduce the button size so that it's not higher than the line edit, and not much larger than its text
    button->setMaximumHeight(_line->size().height()-5);
    button->setMaximumWidth(button->fontMetrics().boundingRect(button->text()).width()+15);
    layout->addWidget(button);
    CONNECT(button, SIGNAL(clicked()), this, SLOT(clicked()));

    //Create the file dialog and set its mode
    _fileDialog = new QFileDialog(this,title,QDir::homePath());
    _fileDialog->setFileMode(mode);
}

//Set in the dialog
void FileLineEdit::setDialogTitle(const QString &title)
{
    _fileDialog->setWindowTitle(title);
}

//Fetch from the dialog
QString FileLineEdit::getDialogTitle() const
{
    return _fileDialog->windowTitle();
}

//Set in the dialog
void FileLineEdit::setMode(QFileDialog::FileMode mode)
{
    _fileDialog->setFileMode(mode);
}

//Fetch from the dialog
QFileDialog::FileMode FileLineEdit::getMode() const
{
    return _fileDialog->fileMode();
}

//To the dialog
void FileLineEdit::setAcceptMode(QFileDialog::AcceptMode mode)
{
    _fileDialog->setAcceptMode(mode);
}

//From the dialog
QFileDialog::AcceptMode FileLineEdit::getAcceptMode() const
{
    return _fileDialog->acceptMode();
}

//Set in the dialog
void FileLineEdit::setFilters(const QStringList &filters)
{
    _fileDialog->setNameFilters(filters);
}

//Fetch from the dialog
QStringList FileLineEdit::getFilters() const
{
    return _fileDialog->nameFilters();
}

//Our internal value
QString FileLineEdit::getFileName() const
{
    return _file;
}

void FileLineEdit::updateFileName(QString file)
{
    _file = file;
    emit fileNameChanged(file);
}

//Set in our internal value and emit a signal
void FileLineEdit::setFileName(QString file)
{
    _line->setText(file);
}

//Set the current file in the dialog, show it, and fetch the resulting file
void FileLineEdit::clicked()
{
    _fileDialog->selectFile(_file);
    if (_fileDialog->exec() == QDialog::Accepted) {
        setFileName(_fileDialog->selectedFiles().at(0));
        _line->setText(_file);
    }
}
