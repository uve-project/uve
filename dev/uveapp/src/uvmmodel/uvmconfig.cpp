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
#include "uvmconfig.h"


// Constructor of the config
UvmConfig::UvmConfig(UvmComponent *parent) : UvmComponent(parent)
{
}


// Constructor of the config
UvmConfig::UvmConfig(QString name, UvmComponent *parent) : UvmComponent(name, parent)
{
}

ComponentType UvmConfig::getComponentType() const
{
    return CONFIG;
}

/*
// Set the interface port of the config
void UvmConfig::setInterfacePort(UvmPort* newPort)
{
    UvmPort* port = getInterfacePort();
    int i;
    if(port == 0) {
        ports.append(newPort);
    }
    else {
        i = ports.indexOf(port);
        ports.replace(i, newPort);
    }
}


// Get the interface port of the config
UvmPort* UvmConfig::getInterfacePort()
{
    UvmPort* port;
    for (int i=0; i < ports.count(); i++) {
        port = ports.at(i);
        if(port->getMode() == UvmPort::CONFIG)
            return port;
    }
    return 0;
}
*/
