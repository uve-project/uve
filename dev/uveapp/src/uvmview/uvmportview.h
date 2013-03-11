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
#ifndef UVM_PORT_VIEW_H
#define UVM_PORT_VIEW_H


#include "../uvmmodel/uvmport.h"
#include "uvmcomponentview.h"
#include "uvmconnectionview.h"
#include <QtGui>


class UvmComponentView;
class UvmConnectionView;

class UvmPortView : public QGraphicsObject
{
    Q_OBJECT

    public:
        UvmPortView(UvmComponentView* parent, UvmPort* model);

        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget *widget);

        void setModel(UvmPort* model);
        UvmPort* getModel();

        void addConnection(UvmConnectionView* connection);
        void delConnection(UvmConnectionView* connection);
        QList<UvmConnectionView *> getConnections();

        ~UvmPortView();

    public slots:
        void applyModelChanges();

    private :
        QPolygonF poly;
        UvmPort *model;
        QList<UvmConnectionView *> uvmConnectionsView;
};


#endif // UVM_PORT_VIEW_H
