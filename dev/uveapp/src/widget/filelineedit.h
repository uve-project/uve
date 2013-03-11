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
#ifndef FILELINEEDIT_H
#define FILELINEEDIT_H

#include <QWidget>
#include <QFileDialog>
#include <QStringList>
#include <QLineEdit>
#include "connect.h"

/** Display an edition line with a button
  A QLineEdit is shown with a button on his right.
  Clicking on the button opens a file or folder selection dialog.
  This object can be configured either in file selection mode or on directory selection mode.
  A filter can be given for the file type.
  */
class FileLineEdit : public QWidget
{
    Q_OBJECT
public:

    /** Creates the object with the default settings
      */
    explicit FileLineEdit(QWidget *parent = 0);

    /** Set the window title, and possibly the file mode for the file dialog
      */
    FileLineEdit (QString title, QFileDialog::FileMode mode = QFileDialog::AnyFile);

    void setDialogTitle (const QString &title);
    QString getDialogTitle () const;

    void setMode (QFileDialog::FileMode mode);
    QFileDialog::FileMode getMode () const;

    void setAcceptMode (QFileDialog::AcceptMode mode);
    QFileDialog::AcceptMode getAcceptMode () const;

    void setFilters (const QStringList &filters);
    QStringList getFilters () const;

    QString getFileName () const;

    void setFileName (QString file);

signals:

    void fileNameChanged (QString file);

public slots:

    void updateFileName (QString file);

private slots:

    void clicked ();

private:
    //Initialise the object (kind of the constructor)
    void init (QString title, QFileDialog::FileMode mode);

    //The file name
    QString _file;

    //GUI objects
    QFileDialog *_fileDialog;
    QLineEdit *_line;
};

#endif // FILELINEEDIT_H

