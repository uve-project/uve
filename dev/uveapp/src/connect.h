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
#ifndef __CONNECT_H__
#define __CONNECT_H__

#include <QMessageBox>

#define CONNECT(a,b,c,d) \
    if (!connect(a,b,c, d))\
            {\
                QString mess;\
                mess =QString("Signal connection error in file %1, line %2. %3: %4::%5").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__).arg(#a).arg(#b);\
                QMessageBox::warning(0, "Ubichip Manager",\
                    mess);\
            }



#endif // __CONNECT_H__
