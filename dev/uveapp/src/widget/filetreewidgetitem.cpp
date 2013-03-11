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
#include "filetreewidgetitem.h"

FileTreeWidgetItem::FileTreeWidgetItem(QTreeWidgetItem *parent) :
    QTreeWidgetItem(parent)
{
    filePath = "";
    entityName = "";
    mode = 0;
}

void FileTreeWidgetItem::setFilePath(QString filePath)
{
    this->filePath = filePath;
    this->fileName = QString(this->filePath.right(this->filePath.length() - this->filePath.lastIndexOf("/") -1));
    if (mode==0)
        this->setText(0, fileName);
    this->setIcon(0, *(new QIcon(":/images/fileIcon.png")));
}

QString FileTreeWidgetItem::getFilePath()
{
    return this->filePath;
}

QString FileTreeWidgetItem::getBodyFileName()
{
    QString tmp;
    tmp = QString(this->filePath.right(this->filePath.length() - this->filePath.lastIndexOf("/") -1));
    return tmp;
}

void FileTreeWidgetItem::activeComponentHasChanged(QString path)
{
    if(filePath.compare(path) == 0)
    {
        setSelected(true);
    }
    else
        setSelected(false);
}

void FileTreeWidgetItem::checkIfSelected()
{
    if(isSelected())
    {
        openOrActivateFile(filePath);
    }
}


void FileTreeWidgetItem::setEntityName(const QString name)
{
    entityName = name;
    if (mode==1)
        this->setText(0, name);

}

void FileTreeWidgetItem::setDisplayMode(int mode)
{
    this->mode = mode;
    if (mode==0)
        this->setText(0, fileName);
    else
        this->setText(0, entityName);
}
