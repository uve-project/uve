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
#include "routingboard.h"
#include <iostream>

using namespace std;


// Contructor of the routing board
RoutingBoard::RoutingBoard()
{
}


// Route a liste of connection depending on a parent and some obstacle
QList<QPolygon> RoutingBoard::route(QRectF parent, QList<QPoint> srcPoints, QList<QPoint> dstPoints, QList<QRectF> obstacles)
{
    QList<QPolygon> polyList;
    this->sizeX = parent.width();
    this->sizeY = parent.height();
    this->board = new int[sizeX*sizeY];
    this->boardBool= new bool[sizeX*sizeY];

    for(int y=0; y<sizeY; y++)
        for(int x=0; x<sizeX; x++)
            setValue(QPoint(x,y),false);

    for(int i=0; i<srcPoints.count(); i++)
    {
        found = false;

        for(int y=0; y<sizeY; y++)
            for(int x=0; x<sizeX; x++)
                setValue(QPoint(x,y),10000);

        foreach(QRectF rect , obstacles)
            setObstacle(rect);

        polyList.append(route(srcPoints.at(i), dstPoints.at(i)));
        printOut();
    }

    return polyList;
}


// Set an obstacle to a particular point represented by -1
void RoutingBoard::setObstacle(QRectF rect)
{
    int x1 = rect.x();
    int y1 = rect.y();
    for(int y2=0; y2<rect.height(); y2++)
        for(int x2=0; x2<rect.width(); x2++)
            setValue(QPoint(x1+x2, y1+y2), -1);
}


// Route a connection and return a polygon
QPolygon RoutingBoard::route(QPoint srcPoint, QPoint dstPoint)
{
    QPoint curPoint;
    QList<QPoint> pointList;
    QPolygon result;

    // Intialization of values
    setValue(srcPoint, 1);
    pointList.append(srcPoint);

    // Search the routing path
    while (!pointList.isEmpty())
    {
        curPoint = pointList.first();
        pointList.removeFirst();

        if (check(curPoint+QPoint(-1,0), 1+getValueInt(curPoint), dstPoint))
            pointList.append(curPoint+QPoint(-1,0));
        if (check(curPoint+QPoint(1,0),  1+getValueInt(curPoint), dstPoint))
            pointList.append(curPoint+QPoint(1,0));
        if (check(curPoint+QPoint(0,-1), 1+getValueInt(curPoint), dstPoint))
            pointList.append(curPoint+QPoint(0,-1));
        if (check(curPoint+QPoint(0,1),  1+getValueInt(curPoint), dstPoint))
            pointList.append(curPoint+QPoint(0,1));
    }

    // Get the path
    if (found)
        result = traceBack(srcPoint, dstPoint);

    return result;
}



// Check if the point is the target. If not, add it to the list of point.
bool RoutingBoard::check(QPoint point, int value, QPoint dstPoint)
{
    if (getValueBool(point)) {
        if(getValueInt(point) > value+5) {
            setValue(point, value+5);
            if(!found)
                found = point == dstPoint;
            return true;
        }
    }
    else if (getValueInt(point)> value) {
        setValue(point, value);
        if(!found)
            found = point == dstPoint;
        return true;
    }
    return false;
}


// Trace back the connection and add point when there is a change of direction
QPolygon RoutingBoard::traceBack(QPoint srcPoint, QPoint dstPoint)
{   
    QPoint curPoint = dstPoint;
    QPoint tmpPoint = dstPoint;
    QList<QPoint> resultList;
    QPolygon m_path;
    int direction = -1;
    int tmpDirection = -1;
    int curentVal;
    int tempVal;

    while(curPoint != srcPoint)
    {
        tempVal = getValueInt(tmpPoint);
        curentVal = getValueInt(curPoint+QPoint(1,0));
        if (curentVal+1 <= tempVal && curentVal+1 > 0) {
            tmpDirection = 0;
            tmpPoint = curPoint+QPoint(1,0);
        }

        tempVal = getValueInt(tmpPoint);
        curentVal = getValueInt(curPoint+QPoint(-1,0));
        if (curentVal+1 <= tempVal && curentVal+1 > 0) {
            tmpDirection = 1;
            tmpPoint = curPoint+QPoint(-1,0);
        }

        tempVal = getValueInt(tmpPoint);
        curentVal = getValueInt(curPoint+QPoint(0,1));
        if (curentVal+1 <= tempVal && curentVal+1 > 0) {
            tmpDirection = 2;
            tmpPoint = curPoint+QPoint(0,1);
        }

        tempVal = getValueInt(tmpPoint);
        curentVal = getValueInt(curPoint+QPoint(0,-1));
        if (curentVal+1 <= tempVal && curentVal+1 > 0) {
            tmpDirection = 3;
            tmpPoint = curPoint+QPoint(0,-1);
        }

        if (direction != tmpDirection) {
            m_path.append(curPoint);
            direction = tmpDirection;
        }

        resultList.append(tmpPoint);
        curPoint = tmpPoint;
    }

    foreach(QPoint point , resultList)
        setValue(point, true);

    m_path.append(srcPoint);
    return m_path;
}


// Get a value of a point of the int rooting board if it's valid
int RoutingBoard::getValueInt(QPoint p)
{
    if (p.x()<0 || p.y()<0 || p.x()>=sizeX ||p.y()>=sizeY)
        return -10;
    return board[p.x()*sizeY+p.y()];
}


// Get a value of a point of the bool rooting board if it's valid
bool RoutingBoard::getValueBool(QPoint p)
{
    if (p.x()<0 || p.y()<0 || p.x()>=sizeX ||p.y()>=sizeY)
        return false;
    return boardBool[p.x()*sizeY+p.y()];
}


// Set a value of a point of the int rooting board if it's valid
void RoutingBoard::setValue(QPoint p, int value)
{
    if (p.x()<0 || p.y()<0 || p.x()>=sizeX ||p.y()>=sizeY)
        return;
    board[p.x()*sizeY+p.y()]=value;
}


// Set a value of a point of the bool rooting board if it's valid
void RoutingBoard::setValue(QPoint p, bool value)
{
    if (p.x()<0 || p.y()<0 || p.x()>=sizeX ||p.y()>=sizeY)
        return;
    boardBool[p.x()*sizeY+p.y()]=value;
}


// Print out all point of the routing board
void RoutingBoard::printOut()
{
    QPoint curPoint;
    for(int y=0;y<sizeY;y++) {
        for(int x=0;x<sizeX;x++) {
            curPoint.setX(x); curPoint.setY(y);
            cout.width(2);
            cout.fill(' ');
            cout << getValueInt(curPoint);
        }
        cout << endl;
    }
}


RoutingBoard::~RoutingBoard()
{
    if(board!=NULL)
        delete board;
    if(boardBool!=NULL)
        delete boardBool;
}
