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
#include "uvmmonitor.h"
#include "uvmagent.h"

// Constructor of the monitor
UvmMonitor::UvmMonitor(UvmComponent *parent) :
    UvmComponent(parent),
    _coverage(0)
{
}


// Constructor of the monitor
UvmMonitor::UvmMonitor(QString name, UvmComponent *parent) :
    UvmComponent(name, parent),
    _coverage(0)
{
}

#include "uvesettings.h"

QString UvmMonitor::getRelativeTemplateFileName() const
{
    QString tmp;
    if (dynamic_cast<UvmAgent*>(getParent())==0)
        tmp="bus";
    else
        tmp="agent";
    return QString(UVESettings::generator_gvip_folder() + "gvip_" + tmp + "_monitor.sv");
}

//Simple set
void UvmMonitor::setCoverage(int value)
{
    _coverage = value;
}

//Simple get
int UvmMonitor::getCoverage() const
{
    return _coverage;
}

ComponentType UvmMonitor::getComponentType() const
{
    return MONITOR;
}
