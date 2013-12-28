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
#ifndef UVMSCOREBOARD_H
#define UVMSCOREBOARD_H

#include "uvmcomponent.h"

/**
  * This class represent an UVM scoreboard.
  */
class UvmScoreboard: public UvmComponent
{
    UVMCOMPONENT_UTILS

    public :

        enum ComparatorType {
                 NOCOMPARATOR=0,
                 ONLYCOMPARATOR,
                 PORTANDCOMPARATOR
        };

        UvmScoreboard(UvmComponent *parent=0);
        UvmScoreboard(QString shortName, UvmComponent *parent=0);

        virtual QString getRelativeTemplateFileName() const;
        virtual ComponentType getComponentType() const;

        virtual void setComparatorType(ComparatorType type);
        virtual ComparatorType getComparatorType() const;
        virtual bool hasComparator() const;

    protected:

        ComparatorType comparator;
};

#endif // UVMSCOREBOARD_H
