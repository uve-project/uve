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
#ifndef NAMESPACEREGISTER_H
#define NAMESPACEREGISTER_H

#include <QMap>
#include <QString>

#include "uvmcomponent.h"

/** Provide some simple functions to handle a namespace
  */
class NameSpaceRegister
{
public:

    NameSpaceRegister();

    //! Clear the namespace
    void clearNameSpace ();

    /** Check if the given name is unique along the project
         If the name is already registered, but for the specified object, this method will return true.
         */
    bool isNameUsed (const QString &name, void *object = 0) const;

    //! Simple helper for the above function
    bool isNameUsed (const QString &name, UvmComponent *component) const;

    /** Register a name
         If the name is already used (but not by this component), this function return false.
         If the component already has a name, the old one will be removed.
         */
    bool registerName (const QString &name, QObject *object);

    /** Associated object
      @return The object associated with this name
      */
    QObject* objectForName (const QString &name) const;

    /** Un-register a name
      The name and its associated object will be removed.
      */
    void unRegisterName (const QString &name);

private:
    //Name and objects association
    QMap<QString,QObject*> _names;
};

#endif // NAMESPACEREGISTER_H
