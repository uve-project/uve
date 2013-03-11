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
#ifndef FILETREEWIDGETITEM_H
#define FILETREEWIDGETITEM_H

#include <QTreeWidgetItem>
#include <QDebug>

class FileTreeWidgetItem : public QObject, public QTreeWidgetItem
{
    Q_OBJECT
public:
    explicit FileTreeWidgetItem(QTreeWidgetItem *parent = 0);
    void setFilePath(QString filePath);
    QString getFilePath();
    QString getBodyFileName();
    void setEntityName(const QString name);
    void setDisplayMode(int mode);
signals:
    void openOrActivateFile(QString file);

public slots:
    void activeComponentHasChanged(QString path);
    void checkIfSelected();

private:
    QString filePath;
    QString fileName;
    QString entityName;
    int mode;
};

#endif // FILETREEWIDGETITEM_H
