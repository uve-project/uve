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
#ifndef UVM_DESIGN_VIEW_H
#define UVM_DESIGN_VIEW_H

#include <QtGui>
#include "../uvmmodel/uvmdesign.h"
#include "uvmportview.h"
#include "uvmcomponentview.h"

class MainWindow;

class UvmDesignView : public UvmComponentView
{
    Q_OBJECT

    public:
        UvmDesignView(UvmComponentView* parent, UvmComponent* model, MainWindow* mainwindow);
        virtual void calculRect();
        void untangleConnections();
        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget *widget);
protected:
        void placePorts();
    private:
        virtual QString getTypeName();
        QRectF intRect;
};

#endif // UVM_DESIGN_VIEW_H
