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
#ifndef COMPONENTTYPE_H
#define COMPONENTTYPE_H

typedef enum {AGENT=0,
                COLLECTOR=1,
                CONFIG=2,
                DESIGN=3,
                DRIVER=4,
                INTERFACE=5,
                MONITOR=6,
                PACKAGE=7,
                SCOREBOARD=8,
                SEQUENCE=9,
                SEQUENCE_ITEM=10,
                SEQUENCER=11,
                TESTBENCH=12,
                TESTCASE=13,
                TOP=14,
                VERIFICATION_COMPONENT=15,
                VIRTUAL_SEQUENCER=16,
                UNKNOWN_TYPE=17
                } ComponentType;

#endif // COMPONENTTYPE_H
