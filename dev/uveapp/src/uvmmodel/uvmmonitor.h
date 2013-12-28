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
#ifndef UVMMONITOR_H
#define UVMMONITOR_H

#include "uvmcomponent.h"
#include "uvmport.h"

/**
  * This class represent an UVM monitor.
  */
class UvmMonitor: public UvmComponent
{
    UVMCOMPONENT_UTILS

    public:
        UvmMonitor(UvmComponent *parent=0);
        UvmMonitor(QString shortName, UvmComponent *parent=0);

        void setCoverage (int value);
        int getCoverage () const;

        QString getRelativeTemplateFileName() const;
        virtual ComponentType getComponentType() const;

    private:
        int _coverage;
};

#endif // UVMMONITOR_H
