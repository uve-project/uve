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
#include "uvmcomponentwithinterface.h"

// Contructeur of the component
UvmComponentWithInterface::UvmComponentWithInterface(QString name, UvmComponent *parent) : UvmComponent(name, parent)
{
}


// Set the interface of the component
void UvmComponentWithInterface::setInterface(UvmInterface *interface)
{
    UvmComponent* comp = getInterface();
    int i;
    if(comp == 0) {
        children.append(interface);
    }
    else {
        i = children.indexOf(comp);
        children.replace(i, interface);
    }
}


// Get the interface of the component
UvmInterface* UvmComponentWithInterface::getInterface()
{
    UvmComponent* comp;
    for (int i=0; i < children.count(); i++) {
        comp = children.at(i);
        if(dynamic_cast<UvmInterface*>(comp) != 0)
            return (UvmInterface*) comp;
    }
    return 0;
}
