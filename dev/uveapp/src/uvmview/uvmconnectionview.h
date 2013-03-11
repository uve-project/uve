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
#ifndef UVM_CONNECTION_VIEW_H
#define UVM_CONNECTION_VIEW_H

#include <QtGui>
#include "../uvmmodel/uvmconnection.h"
#include "../routing/routingboard.h"
#include "uvmportview.h"
#include "uvmcomponentview.h"

class UvmPortView;
class UvmComponentView;

class UvmConnectionView : public QGraphicsLineItem
{
    public:
        enum {Type = UserType + 3};
        UvmConnectionView(UvmPortView *srcPort, UvmPortView *dstPort,  UvmConnection* model, UvmComponentView* parent);

        int type() const;
        QRectF boundingRect() const;
        QPainterPath shape() const;

        void setColor(const QColor &color);
        void updateLine();

        int getDirection(UvmPortView* port, QPointF linePoint);
        QPointF getSrcPoint();
        QPointF getDstPoint();
        QPointF getAbsoluteSrcPoint();
        QPointF getAbsoluteDstPoint();

        UvmPortView* getSrcPortView();
        void setSrcPortView(UvmPortView *srcPort);

        UvmPortView* getDstPortView();
        void setDstPortView(UvmPortView *dstPort);

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
        void drawArrow(QPainter *p, QPointF from, QPointF to);
        void setPolygon(QPolygon polygon);

        ~UvmConnectionView();
        bool intersectsWith(UvmConnectionView* other);

    private:
        UvmConnection* model;
        UvmPortView* srcPort;
        UvmPortView* dstPort;
        QGraphicsTextItem* name;
        QColor color;
        QPainterPath* path;
};

#endif
