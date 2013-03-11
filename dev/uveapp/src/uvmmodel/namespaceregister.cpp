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
#include "namespaceregister.h"

// Check in the internal map
NameSpaceRegister::NameSpaceRegister()
{
}

void NameSpaceRegister::clearNameSpace()
{
    _names.clear();
}

bool NameSpaceRegister::isNameUsed(const QString &name, void *object) const
{
    //Fetch the component for this name (or 0 if none)
    void *c =_names.value(name,0);

    //If no component is given, only check the key presence
    if (!object)
        return c != 0;

    //If not in the map or in the map for this component, return false
    return  ! (c == object || c == 0);
}

// Call the above
bool NameSpaceRegister::isNameUsed(const QString &name, UvmComponent *component) const
{
    return isNameUsed(name,component);
}

// Append, if not already there
// If there but on another name, change it
bool NameSpaceRegister::registerName(const QString &name, QObject *object)
{
    if (this->isNameUsed(name,object))
        return false;

    //If there is a name for this component, replace it
    QString key = _names.key(object);
    if (!key.isEmpty())
        _names.remove(key);

    //Add the new name
    _names.insert(name,object);

    return true;
}


QObject *NameSpaceRegister::objectForName(const QString &name) const
{
    return _names.value(name,0);
}

void NameSpaceRegister::unRegisterName(const QString &name)
{
    _names.remove(name);
}

