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
#include "uvminterface.h"
#include "uvmverificationcomponent.h"

// Constructor of the interface
UvmInterface::UvmInterface(UvmComponent *parent) :
    UvmComponent(parent)
{

}

UvmInterface::UvmInterface(QString name, UvmComponent *parent) : UvmComponent(name, parent)
{
}

// Add the DUT interface port of the interface
void UvmInterface::setDUTInterfacePort(NonPhysicalPort *newPort)
{
    NonPhysicalPort* port = getDUTInterfacePort();
    int i;
    if(port == 0) {
        ports.append(newPort);
    }
    else {
        i = ports.indexOf(port);
        ports.replace(i, newPort);
    }
}


// Get the DUT interface ports of the interface
NonPhysicalPort* UvmInterface::getDUTInterfacePort()
{
    NonPhysicalPort* port;
    for (int i=0; i < ports.count(); i++) {
        port = ports.at(i);
        if(port->getMode() == UvmPort::DUT_INTERFACE)
            return port;
    }
    return 0;
}


// Add an VC interface port to the interface
void UvmInterface::addVCInterfacePort(NonPhysicalPort *newPort)
{
    if(!ports.contains(newPort)) {
        ports.append(newPort);
    }
}


// Remove an VC interface port to the interface
void UvmInterface::removeVCInterfacePort(NonPhysicalPort *oldPort)
{
    int i;
    if(ports.contains(oldPort)) {
        i = ports.indexOf(oldPort);
        ports.removeAt(i);
     }
}


// Get all DUT interface ports of the interface
QList<NonPhysicalPort*> UvmInterface::getVCInterfacePorts()
{
    QList<NonPhysicalPort*> portList;
    NonPhysicalPort* port;
    for (int i=0; i < ports.count(); i++) {
        port = ports.at(i);
        if(port->getMode() == UvmPort::VC_INTERFACE)
            portList.append(port);
    }
    return portList;
}

QList<NonPhysicalPort *> UvmInterface::getVCTODUTInterfacePorts()
{
    QList<NonPhysicalPort*> portList;
    NonPhysicalPort* port;
    for (int i=0; i < ports.count(); i++) {
        port = ports.at(i);
        if(port->getMode() == UvmPort::VCTODUT_INTERFACE)
            portList.append(port);
    }
    return portList;
}

#include "uvesettings.h"

QString UvmInterface::getRelativeTemplateFileName() const
{
    return QString(UVESettings::generator_gvip_folder() + "gvip_if.sv");
}

ComponentType UvmInterface::getComponentType()
{
    return INTERFACE;
}
