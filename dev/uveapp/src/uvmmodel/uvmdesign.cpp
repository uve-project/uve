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
#include "uvmdesign.h"


// Constructor of the design
UvmDesign::UvmDesign(UvmComponent *parent) : UvmComponent(parent)
{
}


// Constructor of the design
UvmDesign::UvmDesign(QString name, UvmComponent *parent) : UvmComponent(name, parent)
{
}


QString UvmDesign::getAbsoluteBodyFileName () const
{
    return bodyFileName;
}

// Get the relative path depending on the top of the project
QString UvmDesign::getRelativePath() const
{
    return relativePath;
}


// Add an interface port to the design
void UvmDesign::addInterfacePort(NonPhysicalPort *newPort)
{
    if(!ports.contains(newPort)) {
        ports.append(newPort);
    }
}


// Remove an interface port to the design
void UvmDesign::removeInterfacePort(NonPhysicalPort *oldPort)
{
   int i;
   if(ports.contains(oldPort)) {
       i = ports.indexOf(oldPort);
       ports.removeAt(i);
    }
}


// Get the list of *UvmPort contained in the design
QList<NonPhysicalPort*> UvmDesign::getInterfacePorts() const
{
    QList<NonPhysicalPort*> portList;
    NonPhysicalPort* port;
    for (int i=0; i < ports.count(); i++) {
        port = ports.at(i);
        if(port->getMode() == UvmPort::DUT_INTERFACE)
            portList.append(port);
    }
    return portList;
}


ComponentType UvmDesign::getComponentType()
{
    return DESIGN;
}
