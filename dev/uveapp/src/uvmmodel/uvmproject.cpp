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
#include "uvmproject.h"

#include <QDir>
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QDebug>

#include "uvmcomponent.h"
#include "uvmtop.h"
#include "current_version.h"
#include "svexport.h"

UvmProject::UvmProject() :
    _top(0)
{
}

UvmProject::~UvmProject()
{
    delete _top;
}

void UvmProject::setSimulator(const QString &file)
{
    _simulator = file;
}

QString UvmProject::getSimulator() const
{
    return _simulator;
}

void UvmProject::setDut(const QString &file)
{
    _dut = file;
}

QString UvmProject::getDut() const
{
    return _dut;
}

void UvmProject::setFolder(const QString &folder)
{
    if (QFileInfo(folder).canonicalFilePath().isEmpty())
        this->_folder = QFileInfo(folder).absolutePath() + DIRSEPARATOR;
    else
        this->_folder = QFileInfo(folder).canonicalFilePath() + DIRSEPARATOR;
}

QString UvmProject::getFolder() const
{
    return _folder;
}

void UvmProject::setName(const QString &name)
{
    this->_name = name;
    if (_filename.isEmpty())
        _filename = name+".uve";
}

QString UvmProject::getName() const
{
    return _name;
}

void UvmProject::setProjectFileName(const QString &fileName)
{
    _filename = fileName;
}

QString UvmProject::getProjectFileName() const
{
    return _filename;
}

void UvmProject::setTemplatePath(const QString &templatePath)
{
    _templatePath = templatePath;
}

QString UvmProject::getTemplatePath() const
{
    return _templatePath;
}

void UvmProject::setTop(UvmTop *top)
{
    this->_top = top;
}

UvmTop *UvmProject::getTop() const
{
    return _top;
}

void UvmProject::setVHDLFiles(const QStringList &files)
{
    _vhdl_files = files;
}

void UvmProject::addVHDLFile(const QString &file)
{
    _vhdl_files.append(file);
}

QStringList UvmProject::getVHDLFiles() const
{
    return _vhdl_files;
}

// Print our name and top
void UvmProject::printTree()
{
    QString tree = this->getName() + "\n";
    tree.append(_top->printTree());

    qDebug() << tree;
}

void UvmProject::printTree(QString filename)
{
    QString tree = this->getName() + "\n";
    tree.append(_top->printTree());

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug()<< "Unable to open file " << filename;
        return;
    }

    QTextStream out(&file);
    out << tree;

    file.close();
}


UvmPort* UvmProject::findPortForName(const QString &name) const
{
    UvmPort *port=dynamic_cast<UvmPort *>(this->objectForName(name));
    if (port)
        return port;
    foreach (UvmVerificationComponent *uvc,this->getTop()->getVips())
    {
        port = uvc->findPortForName(name);
        if (port)
            return port;
    }
    return 0;
}

