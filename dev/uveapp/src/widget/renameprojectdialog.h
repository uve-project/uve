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
#ifndef RENAMEPROJECTDIALOG_H
#define RENAMEPROJECTDIALOG_H

#include <QDialog>
#include "connect.h"

namespace Ui {
class RenameProjectDialog;
}

/** Propose to rename the old project in a new one
  Nothing is done inside this object, the choice is only presented to the user.
  */
class RenameProjectDialog : public QDialog
{
    Q_OBJECT

public:
    /** Constructor
      @param projectDirectory The absolute path of the current project
      */
    explicit RenameProjectDialog(const QString &projectDirectory, QWidget *parent = 0);
    ~RenameProjectDialog();

    /** The new project directory
      This data is typed in in the dialog
      */
    QString newProjectName () const;

protected slots:

    // Check the filesystem for the new name
    bool checkFileName (QString filename);

private:
    Ui::RenameProjectDialog *ui;
};

#endif // RENAMEPROJECTDIALOG_H
