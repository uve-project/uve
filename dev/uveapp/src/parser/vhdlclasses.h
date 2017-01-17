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
#ifndef VHDLCLASSES_H
#define VHDLCLASSES_H

#include <QString>
#include <QStringList>

/**
  Types
  */

struct VHDLPort
{
    QString name;
    QString mode;         // in, out, inout
    QString type;         // std_logic, std_logic_vector
    int lowBit;
    QString direction;    // to, downto
    int upBit;
    int lineNum;
};

class VHDLEntity
{
public:
    VHDLEntity();
    int nbPorts;
    QString entityName;
    QVector<VHDLPort> portRacine;
};

class VHDLFileContent
{
public:
    //! Return the entity ports and add them to the given parent
    QList<PhysicalPort*> getPorts (const QString &entityName,PhysicalPortList *parent) const;

    //! Return the port names
    QStringList getPortNames (const QString &entityName) const;

    //! Return the list of the entity names
    QStringList getEntitiesName() const;


    VHDLEntity *getEntity(const QString &entityName) const;

    QString getFileName() const;

    QString fileName;
    QVector<VHDLEntity *> entities;
};

#endif // VHDLCLASSES_H
