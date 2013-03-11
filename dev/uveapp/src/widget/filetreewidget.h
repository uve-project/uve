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
#ifndef FILETREEWIDGET_H
#define FILETREEWIDGET_H

#include <QTreeWidget>

class MainWindow;

class FileTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit FileTreeWidget(MainWindow* mainwindow, QWidget *parent = 0);
    void populateTree();
signals:
    
public slots:
    
private:
    void createFileItem(QString name, QTreeWidgetItem* parent, QString fullPath);
    QTreeWidgetItem *testBenchTWI;
    QTreeWidgetItem *dutTWI;
    QTreeWidgetItem *VIPTWI;
    MainWindow* mainwindow;
};

#endif // FILETREEWIDGET_H
