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
#include "otherfilesaccessor.h"
#include "mainwindow.h"
#include "uvmcomponentview.h"
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QAction>

OtherFilesAccessor::OtherFilesAccessor(UvmComponentView *parent, UvmComponent *model, MainWindow *mainwindow)
    :Accessor(parent, model, mainwindow)
{
    label = "o";
}

void OtherFilesAccessor::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QStringList files = model->getAbsoluteOtherFiles();
        QMenu* menu = new QMenu();
        foreach(QString file, files)
        {
            QString fileName = file.right(file.length() - file.lastIndexOf(QDir::separator())-1);
            if(QFileInfo(file).exists())
            {
                QAction *a = new QAction(this);
                a->setText(fileName);
                a->setData(file);
                CONNECT(a, SIGNAL(triggered()), this, SLOT(openFile()));
                menu->addAction(a);
            }
            else
            {
                QAction *a = new QAction(this);
                a->setText(fileName + " was not found. ");
                a->setEnabled(false);
                menu->addAction(a);
            }
        }

        menu->exec(QCursor::pos());
    }

    // pass on to base class
    QGraphicsObject::mousePressEvent(event);
}

void OtherFilesAccessor::openFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action) {
        QString file = action->data().toString();
        mainwindow->openFile(file);
    }
}
