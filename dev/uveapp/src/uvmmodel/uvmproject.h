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
#ifndef UVMPROJECT_H
#define UVMPROJECT_H

#include <QString>
#include <QMap>
#include <QStringList>

#include "namespaceregister.h"

class UvmTop;
class UvmComponent;

/** Object representing the whole project
  Some project specific data are stored here, along with the function to save and generate the project files and configurations.
  For now, it's empty. Put your crap around here if you feel like it.
  */
class UvmProject : public NameSpaceRegister
{
public:
    UvmProject();
    virtual ~UvmProject();

    void setSimulator (const QString &file);
    QString getSimulator () const;

    void setDut (const QString &file);
    QString getDut () const;

    void setFolder (const QString &folder);
    QString getFolder () const;

    void setName (const QString &name);
    QString getName () const;

    void setProjectFileName(const QString &fileName);
    QString getProjectFileName () const;

    void setTemplatePath(const QString &templatePath);
    QString getTemplatePath() const;

    void setTop (UvmTop *top);
    UvmTop* getTop () const;

    void setVHDLFiles (const QStringList &files);
    void addVHDLFile(const QString &file);
    QStringList getVHDLFiles() const;

    /** Print the full project tree
      The project will print its data, and start the top component
      */
    void printTree();

    /** Save the ASCII project tree
      The project will print its data, and start the top component.
      The date will be written in this file.
      */
    void printTree(QString filename);


    virtual UvmPort* findPortForName(const QString &name) const;


private:
    QString _simulator;
    QString _dut;
    QString _folder;
    QString _name;
    QString _templatePath;
    //! The filename is derived from the name of the project. It only adds .uve
    QString _filename;
    QStringList _vhdl_files;
    UvmTop *_top;
};

#endif // UVMPROJECT_H
