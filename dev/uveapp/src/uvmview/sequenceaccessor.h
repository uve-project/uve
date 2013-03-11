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
#ifndef SEQUENCEACCESSOR_H
#define SEQUENCEACCESSOR_H

#include <accessor.h>

class UvmComponentView;
class MainWindow;

class SequenceAccessor : public Accessor
{
    Q_OBJECT
public:
    SequenceAccessor(UvmComponentView* parent, UvmComponent* model, MainWindow* mainwindow);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // SEQUENCEACCESSOR_H
