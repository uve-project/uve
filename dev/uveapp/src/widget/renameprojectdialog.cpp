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
#include "renameprojectdialog.h"
#include "ui_renameprojectdialog.h"

#include <QDir>
#include <QFileInfo>

//Include the message
RenameProjectDialog::RenameProjectDialog(const QString &projectDirectory, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RenameProjectDialog)
{
    ui->setupUi(this);

    //Setup the message
    ui->message->setText( tr("A directory with the same name as the current project exists") + "\n"
                          +tr("Do you wish to move this driectroy to another place?"));

    //Strip the last "/" from the directory name (if any)
    QString dir = projectDirectory;
    if (dir.endsWith("/") || dir.endsWith("\\"))
        dir.remove(dir.size()-1,1);

    //Propose a new name
    int cnt = 1;
    QString newDir;
    do {
        newDir = dir + QString("_%1").arg(cnt);
        cnt++;
    }
    while ( ! checkFileName(newDir) );

    ui->filename->setText(newDir);
    //ui->filename->setCursorPosition(newDir.size());
    ui->filename->setFocus();

    //Filename check
    CONNECT( ui->filename, SIGNAL(textChanged(QString)), this, SLOT(checkFileName(QString)));
}

RenameProjectDialog::~RenameProjectDialog()
{
    delete ui;
}

//Fetch it from the GUI
QString RenameProjectDialog::newProjectName() const
{
    return ui->filename->text();
}

//If the given name is already in the filesystem, change the font color
bool RenameProjectDialog::checkFileName(QString filename)
{
    if (QFileInfo(filename).isDir()) {
        ui->filename->setStyleSheet("color : red;");
        ui->move->setEnabled(false);
        return false;
    }

    //Good
    ui->move->setEnabled(true);
    ui->filename->setStyleSheet("color : black;");
    return true;
}
