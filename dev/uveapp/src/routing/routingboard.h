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
#ifndef ROUTINGBOARD_H
#define ROUTINGBOARD_H

#include <QtGui>
#include <QDebug>

class RoutingBoard
{
    public:
        RoutingBoard();

        QList<QPolygon> route(QRectF parent, QList<QPoint> srcPoints, QList<QPoint> dstPoints, QList<QRectF> obstacles);
        void printOut();

        ~RoutingBoard();

    private:
        int sizeX;
        int sizeY;
        int *board;
        bool *boardBool;
        bool found;

        QPolygon route(QPoint srcPoint, QPoint dstPoint);
        QPolygon traceBack(QPoint srcPoint, QPoint dstPoint);
        bool check(QPoint point, int value, QPoint dstPoint);
        void setObstacle(QRectF obstacle);
        void setValue(QPoint p,int value);
        void setValue(QPoint p,bool value);
        int  getValueInt(QPoint p);
        bool getValueBool(QPoint p);
};

#endif // ROUTINGBOARD_H
