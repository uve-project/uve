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
#include "uvmvirtualsequencer.h"


// Contructor of the virtual sequencer
UvmVirtualSequencer::UvmVirtualSequencer(UvmComponent *parent) : UvmComponent(parent)
{
}


// Contructor of the virtual sequencer
UvmVirtualSequencer::UvmVirtualSequencer(QString name, UvmComponent *parent) : UvmComponent(name, parent)
{
}

#include "uvesettings.h"

QString UvmVirtualSequencer::getRelativeTemplateFileName() const
{
    // a virtual sequencer can be in a VIP or in a testbench. The templates are not the same
    if (this->getParentVip())
        return QString(UVESettings::generator_gvip_folder() + "gvip_virtual_sequencer.sv");
    else
        return QString(UVESettings::generator_gtb_folder() + "gtb_virtual_sequencer.sv");
}

// Get the tlm round ports of the virtual sequencer
QList<NonPhysicalPort *> UvmVirtualSequencer::getSubSequencersPorts() const
{
    QList<NonPhysicalPort *> outPorts;
    for (int i=0; i < ports.count(); i++) {
        UvmPort *port = ports.at(i);
        if(port->getMode() == UvmPort::VIRT_SEQ_TO_SEQ_OUT)
            outPorts.append(dynamic_cast<NonPhysicalPort*>(port));
    }
    return outPorts;
}

ComponentType UvmVirtualSequencer::getComponentType()
{
    return VIRTUAL_SEQUENCER;
}
