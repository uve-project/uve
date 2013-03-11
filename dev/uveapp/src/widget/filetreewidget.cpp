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
#include "filetreewidget.h"

#include "mainwindow.h"

FileTreeWidget::FileTreeWidget(MainWindow* mainwindow, QWidget *parent) :
    QTreeWidget(parent)
{
    setColumnCount(1);
    QTreeWidgetItem* headerItem = new QTreeWidgetItem();
    headerItem->setText(0,QString("Project"));
    setHeaderItem(headerItem);
    this->mainwindow = mainwindow;
}

void FileTreeWidget::populateTree()
{
    this->clear();
    testBenchTWI = new QTreeWidgetItem();
    dutTWI = new QTreeWidgetItem();
    VIPTWI = new QTreeWidgetItem();
    addTopLevelItem(testBenchTWI);
    addTopLevelItem(dutTWI);
    testBenchTWI->setText(0, "Test Bench");
    dutTWI->setText(0, "DUT");
    VIPTWI->setText(0, "VIP");
    testBenchTWI->setIcon(0, *(new QIcon(":/images/testbenchIcon.png")));
    dutTWI->setIcon(0, *(new QIcon(":/images/DUTIcon.png")));
    VIPTWI->setIcon(0, *(new QIcon(":/images/vipIcon.png")));
    testBenchTWI->addChild(VIPTWI);
    //repopulate with datas from project
    // DUT
    QStringList vhdlFiles = mainwindow->getProject()->getVHDLFiles();
    foreach(QString str, vhdlFiles)
    {
        if(QFile::exists(str))
        {
            QString filename = str.right(str.length() - str.lastIndexOf(QDir::separator()) -1);
            createFileItem(filename, dutTWI, str);
        }
    }

    // VIP

    QList<UvmVerificationComponent*>* VIPs = new QList<UvmVerificationComponent*>();
    QList<UvmComponent*>* tmp = new QList<UvmComponent*>();
    tmp->append(mainwindow->getProject()->getTop());

    while(tmp->length()>0)
    {
        UvmComponent* current = tmp->first();
        tmp->removeFirst();
        tmp->append(current->getChildren());
        UvmVerificationComponent* v = dynamic_cast<UvmVerificationComponent*>(current);
        if(v !=0)
        {
            VIPs->append(v);
        }
    }

    for(int i=0; i<VIPs->length(); i++)
    {
        UvmVerificationComponent* v= VIPs->at(i);
        QTreeWidgetItem* currentVIPFolder = new QTreeWidgetItem();
        currentVIPFolder->setText(0, v->getShortName());
        currentVIPFolder->setIcon(0, *(new QIcon(":/images/folderIcon.png")));
        VIPTWI->addChild(currentVIPFolder);
        if(QFile::exists(v->getAbsoluteBodyFileName()) &&
                !((v->getAbsoluteBodyFileName().endsWith(".vhd")) ||
                 (v->getAbsoluteBodyFileName().endsWith(".vhdl")) ))
        {
            QString filename = v->getAbsoluteBodyFileName().right(v->getAbsoluteBodyFileName().length() - v->getAbsoluteBodyFileName().lastIndexOf(QDir::separator()) -1);
            createFileItem(filename, currentVIPFolder, v->getAbsoluteBodyFileName());
        }
        if(v->hasHeaderFile() &&
                QFile::exists(v->getAbsoluteHeaderFileName()) &&
                !((v->getAbsoluteHeaderFileName().endsWith(".vhd")) ||
                 (v->getAbsoluteHeaderFileName().endsWith(".vhdl"))))
        {
            QString filename = v->getAbsoluteHeaderFileName().right(v->getAbsoluteHeaderFileName().length() - v->getAbsoluteHeaderFileName().lastIndexOf(QDir::separator()) -1);
            createFileItem(filename, currentVIPFolder, v->getAbsoluteHeaderFileName());
        }

        if(!v->getAbsoluteOtherFiles().empty())
        {
            foreach(QString otherFile, v->getAbsoluteOtherFiles())
            {
                if(QFile::exists(otherFile) &&
                        !((otherFile.endsWith(".vhd")) ||
                         (otherFile.endsWith(".vhdl")) ))
                {
                    QString filename = otherFile.right(otherFile.length() - otherFile.lastIndexOf(QDir::separator())-1);
                    createFileItem(filename, currentVIPFolder, otherFile);
                }
            }
        }

        QList<UvmComponent*> children = v->getChildren();
        for(int j=0; j<children.length(); j++)
        {
            UvmComponent* currentChild = children.at(j);
            UvmAgent* a = dynamic_cast<UvmAgent*>(currentChild);
            if(a != 0)
            {
                QTreeWidgetItem* currentAgentFolder = new QTreeWidgetItem();
                currentAgentFolder->setText(0, a->getShortName());
                currentAgentFolder->setIcon(0, *(new QIcon(":/images/folderIcon.png")));
                currentVIPFolder->addChild(currentAgentFolder);
                if(QFile::exists(a->getAbsoluteBodyFileName()) &&
                        !((a->getAbsoluteBodyFileName().endsWith(".vhd")) ||
                         (a->getAbsoluteBodyFileName().endsWith(".vhdl")) ))
                {
                    QString filename = a->getAbsoluteBodyFileName().right(a->getAbsoluteBodyFileName().length() - a->getAbsoluteBodyFileName().lastIndexOf(QDir::separator()) -1);
                    createFileItem(filename, currentAgentFolder, a->getAbsoluteBodyFileName());
                }

                if(a->hasHeaderFile() &&
                        QFile::exists(a->getAbsoluteHeaderFileName()) &&
                        !((a->getAbsoluteHeaderFileName().endsWith(".vhd")) ||
                         (a->getAbsoluteHeaderFileName().endsWith(".vhdl"))))
                {
                    QString filename = a->getAbsoluteHeaderFileName().right(a->getAbsoluteHeaderFileName().length() - a->getAbsoluteHeaderFileName().lastIndexOf(QDir::separator()) -1);
                    createFileItem(filename, currentAgentFolder, a->getAbsoluteHeaderFileName());
                }

                if(!a->getAbsoluteOtherFiles().empty())
                {
                    foreach(QString otherFile, a->getAbsoluteOtherFiles())
                    {
                        if(QFile::exists(otherFile) &&
                                !((otherFile.endsWith(".vhd")) ||
                                 (otherFile.endsWith(".vhdl")) ))
                        {
                            QString filename = otherFile.right(otherFile.length() - otherFile.lastIndexOf(QDir::separator())-1);
                            createFileItem(filename, currentVIPFolder, otherFile);
                        }
                    }
                }

                QList<UvmComponent*> grandChildren = a->getChildren();
                for(int k=0; k<grandChildren.length(); k++)
                {
                    UvmComponent* currentGrandChild = grandChildren.at(k);
                    if(QFile::exists(currentGrandChild->getAbsoluteBodyFileName()) &&
                            !((currentGrandChild->getAbsoluteBodyFileName().endsWith(".vhd")) ||
                              (currentGrandChild->getAbsoluteBodyFileName().endsWith(".vhdl")) ))
                    {
                        QString filename = currentGrandChild->getAbsoluteBodyFileName().right(currentGrandChild->getAbsoluteBodyFileName().length() - currentGrandChild->getAbsoluteBodyFileName().lastIndexOf(QDir::separator())-1);
                        createFileItem(filename, currentAgentFolder, currentGrandChild->getAbsoluteBodyFileName());
                    }
                    if(currentGrandChild->hasHeaderFile() &&
                            QFile::exists(currentGrandChild->getAbsoluteHeaderFileName()) &&
                            !((currentGrandChild->getAbsoluteHeaderFileName().endsWith(".vhd")) ||
                              (currentGrandChild->getAbsoluteHeaderFileName().endsWith(".vhdl")) ))
                    {
                        QString filename = currentGrandChild->getAbsoluteHeaderFileName().right(currentGrandChild->getAbsoluteHeaderFileName().length() - currentGrandChild->getAbsoluteHeaderFileName().lastIndexOf(QDir::separator())-1);
                        createFileItem(filename, currentAgentFolder, currentGrandChild->getAbsoluteHeaderFileName());
                    }

                    if(!currentGrandChild->getAbsoluteOtherFiles().empty())
                    {
                        foreach(QString otherFile, currentGrandChild->getAbsoluteOtherFiles())
                        {
                            if(QFile::exists(otherFile) &&
                                    !((otherFile.endsWith(".vhd")) ||
                                      (otherFile.endsWith(".vhdl")) ))
                            {
                                QString filename = otherFile.right(otherFile.length() - otherFile.lastIndexOf(QDir::separator())-1);
                                createFileItem(filename, currentVIPFolder, otherFile);
                            }
                        }
                    }
                }
            }
            else
            {
                if(QFile::exists(currentChild->getAbsoluteBodyFileName()) &&
                        !((currentChild->getAbsoluteBodyFileName().endsWith(".vhd")) ||
                          (currentChild->getAbsoluteBodyFileName().endsWith(".vhdl")) ))
                {
                    QString filename = currentChild->getAbsoluteBodyFileName().right(currentChild->getAbsoluteBodyFileName().length() - currentChild->getAbsoluteBodyFileName().lastIndexOf(QDir::separator()) -1);
                    createFileItem(filename, currentVIPFolder, currentChild->getAbsoluteBodyFileName());
                }
                if(currentChild->hasHeaderFile() &&
                        QFile::exists(currentChild->getAbsoluteHeaderFileName()) &&
                        !((currentChild->getAbsoluteHeaderFileName().endsWith(".vhd")) ||
                          (currentChild->getAbsoluteHeaderFileName().endsWith(".vhdl")) ))
                {
                    QString filename = currentChild->getAbsoluteHeaderFileName().right(currentChild->getAbsoluteHeaderFileName().length() - currentChild->getAbsoluteHeaderFileName().lastIndexOf(QDir::separator()) -1);
                    createFileItem(filename, currentVIPFolder, currentChild->getAbsoluteHeaderFileName());
                }

                if(!currentChild->getAbsoluteOtherFiles().empty())
                {
                    foreach(QString otherFile, currentChild->getAbsoluteOtherFiles())
                    {
                        if(QFile::exists(otherFile) &&
                                !((otherFile.endsWith(".vhd")) ||
                                 (otherFile.endsWith(".vhdl"))))
                        {
                            QString filename = otherFile.right(otherFile.length() - otherFile.lastIndexOf(QDir::separator())-1);
                            createFileItem(filename, currentVIPFolder, otherFile);
                        }
                    }
                }
            }
        }
    }

    QList<UvmComponent*> childrenOfTopWithUniqueFiles;
    foreach(UvmComponent* comp, mainwindow->getProject()->getTop()->getAllChildren())
    {
        bool alreadyInTheList = false;
        foreach(UvmComponent* comp2, childrenOfTopWithUniqueFiles)
        {
            alreadyInTheList = alreadyInTheList || comp->getAbsoluteBodyFileName().compare(comp2->getAbsoluteBodyFileName()) ==0;
        }
        if(!alreadyInTheList)
            childrenOfTopWithUniqueFiles.append(comp);
    }

    foreach(UvmComponent *comp, childrenOfTopWithUniqueFiles)
    {
        if ( (comp->getParentVip()==0) &&
             (QFile::exists(comp->getAbsoluteBodyFileName())) &&
             !((comp->getAbsoluteBodyFileName().endsWith(".vhd")) ||
              (comp->getAbsoluteBodyFileName().endsWith(".vhdl"))))
        {
            QString filename = comp->getAbsoluteBodyFileName().right(comp->getAbsoluteBodyFileName().length() - comp->getAbsoluteBodyFileName().lastIndexOf(QDir::separator()) -1);
            createFileItem(filename, testBenchTWI, comp->getAbsoluteBodyFileName());
        }
    }

    expandAll();
}

void FileTreeWidget::createFileItem(QString name, QTreeWidgetItem *parent, QString fullPath)
{
    FileTreeWidgetItem* tmp = new FileTreeWidgetItem();
    CONNECT(mainwindow, SIGNAL(newActiveComponent(QString)), tmp, SLOT(activeComponentHasChanged(QString)));
    CONNECT(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), tmp, SLOT(checkIfSelected()));
    CONNECT(tmp, SIGNAL(openOrActivateFile(QString)), mainwindow, SLOT(openFile(QString)));
    tmp->setFilePath(fullPath);
    tmp->setEntityName(name);
    parent->addChild(tmp);
}
