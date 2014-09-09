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
#include <QtGui>
#include "uvmconnectionview.h"


// Constructor of the Uvm connection
UvmConnectionView::UvmConnectionView(UvmPortView *srcPort, UvmPortView *dstPort, UvmConnection* model, UvmComponentView* parent)
{
    path = new QPainterPath();
    color = Qt::black;

    this->model = model;
    this->srcPort = srcPort;
    this->dstPort = dstPort;
    this->srcPort->addConnection(this);
    this->dstPort->addConnection(this);

    this->name = new QGraphicsTextItem(model->getName());
    this->name->setParentItem(this);
    this->name->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->name->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->name->setVisible(false);

    setZValue(10.0);
    setParentItem(parent);
    setPen(QPen(color, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}


// Return the type of an item as an int
int UvmConnectionView::type() const
{
    return Type;
}


// Define the outer bounds of the item as a rectangle
QRectF UvmConnectionView::boundingRect() const
{
    return path->boundingRect();
}


// Return the shape of this item as a QPainterPath in local coordinates
QPainterPath UvmConnectionView::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    return path;
}


// Set the color of the connection
void UvmConnectionView::setColor(const QColor &color)
{
    this->color = color;
}


// Update the line of the connection
void UvmConnectionView::updateLine()
{
    QLineF line(mapFromItem(this->srcPort, 0, 0), mapFromItem(this->dstPort, 0, 0));
    //QLineF line(getSrcPoint(), getDstPoint());
    setLine(line);
}


// Get the direction of a port
int UvmConnectionView::getDirection(UvmPortView* port, QPointF linePoint)
{
    QRectF parentLayout = port->parentItem()->boundingRect();
    parentLayout.adjust(-10, -10, 10, 10);
    QPointF tabPoint [4];
    QPointF point = mapToItem(port->parentItem(), linePoint);

    tabPoint[0] = point + QPointF( 0,  1);
    tabPoint[1] = point + QPointF( 1,  0);
    tabPoint[2] = point + QPointF( 0, -1);
    tabPoint[3] = point + QPointF(-1,  0);

    while(true)
    {
        /* Go through the four neighbors */
        for(int i = 0; i < 4; i++)
        {
            /* Check if the anchor point is found */
            if(!parentLayout.contains(tabPoint[i]))
                return i;
            else
                /* Get the four next neighbors of the start point */
                switch(i)
                {
                    case 0: tabPoint[i] = tabPoint[i] + QPointF( 0,  1); break;
                    case 1: tabPoint[i] = tabPoint[i] + QPointF( 1,  0); break;
                    case 2: tabPoint[i] = tabPoint[i] + QPointF( 0, -1); break;
                    case 3: tabPoint[i] = tabPoint[i] + QPointF(-1,  0); break;
                }
        }
    }
}


// Get the source point of the connection view
QPointF UvmConnectionView::getSrcPoint()
{
    if(srcPort->getModel()->getMode() == UvmPort::VIRT_SEQ_TO_SEQ_IN || srcPort->getModel()->getMode() == UvmPort::VIRT_SEQ_TO_SEQ_OUT)
        return QPointF((line().p1().x()), (line().p1().y()));

    switch(getDirection(srcPort, line().p1()))
    {
        case 0:  return QPointF((line().p1().x()), (line().p1().y()+5));
        case 1:  return QPointF((line().p1().x()+5), line().p1().y());
        case 2:  return QPointF((line().p1().x()), (line().p1().y()-5));
        case 3:  return QPointF((line().p1().x()-5), line().p1().y());
        default: return QPointF(0,0);
    }
}

QPointF UvmConnectionView::getAbsoluteSrcPoint()
{
    return mapToScene(mapFromItem(this->srcPort, 0, 0));
}

QPointF UvmConnectionView::getAbsoluteDstPoint()
{
    return mapToScene(mapFromItem(this->dstPort, 0, 0));
}


// Get the destination point of the connection view
QPointF UvmConnectionView::getDstPoint()
{
    if(dstPort->getModel()->getMode() == UvmPort::VIRT_SEQ_TO_SEQ_IN || dstPort->getModel()->getMode() == UvmPort::VIRT_SEQ_TO_SEQ_OUT)
        return QPointF((line().p2().x()), (line().p2().y()));
    switch(getDirection(dstPort, line().p2()))
    {
        case 0:  return QPointF((line().p2().x()), (line().p2().y()+5));
        case 1:  return QPointF((line().p2().x()+5), line().p2().y());
        case 2:  return QPointF((line().p2().x()), (line().p2().y()-5));
        case 3:  return QPointF((line().p2().x()-5), line().p2().y());
        default: return QPointF(0,0);
    }
}


// Get the source point of the connection
UvmPortView* UvmConnectionView::getSrcPortView()
{
    return srcPort;
}


// Set the source point of the connection
void UvmConnectionView::setSrcPortView(UvmPortView *srcPort)
{
    this->srcPort = srcPort;
}


// Get the destination point of the connection
UvmPortView* UvmConnectionView::getDstPortView()
{
    return dstPort;
}


// Set the destination point of the connection
void UvmConnectionView::setDstPortView(UvmPortView *dstPort)
{
    this->dstPort = dstPort;
}


// Paint the contents of an item in local coordinates
void UvmConnectionView::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //Do not paint if the type of one of the ports is GLOBAL
    if (dstPort->getModel()->getMode() == UvmPort::GLOBAL || srcPort->getModel()->getMode() == UvmPort::GLOBAL)
        return;

    painter->setRenderHint(QPainter::Antialiasing);
    QBrush brush(Qt::black);
    brush.setStyle(Qt::SolidPattern);
    painter->setBrush(brush);
    painter->drawLine(getSrcPoint(), getDstPoint());

    if(srcPort->getModel()->getDirection() == UvmPort::PORT_OUT)
        drawArrow(painter, getSrcPoint(), getDstPoint());
    else if(srcPort->getModel()->getDirection() == UvmPort::PORT_IN)
        drawArrow(painter, getDstPoint(), getSrcPoint());
    else if(srcPort->getModel()->getDirection() == UvmPort::PORT_INOUT) {
        drawArrow(painter, getDstPoint(), getSrcPoint());
        drawArrow(painter, getSrcPoint(), getDstPoint());
    }
}

void UvmConnectionView::drawArrow(QPainter *p, QPointF from, QPointF to)
{
    int size = 6;
    QPointF points[3];

    float a = atan2(from.y()-to.y(), from.x()-to.x());
    QPointF k(to.x()+size*cos(a), to.y()+size*sin(a));
    a += M_PI/2;
    size = size/2;
    QPointF i(k.x()+size*cos(a), k.y()+size*sin(a));
    QPointF j(k.x()-size*cos(a), k.y()-size*sin(a));

    points[0] = to;
    points[1] = i;
    points[2] = j;
    p->drawConvexPolygon(points, 3);
}


// Set the polygon representing the connection view
void UvmConnectionView::setPolygon(QPolygon polygon)
{
    switch(getDirection(dstPort, line().p2()))
    {
        case 0:  polygon.insert(0, 1,  QPoint((line().p2().x())/GRID_X, (line().p2().y()+5)/GRID_Y)); break;
        case 1:  polygon.insert(0, 1,  QPoint((line().p2().x()+5)/GRID_X, line().p2().y()/GRID_Y)); break;
        case 2:  polygon.insert(0, 1,  QPoint((line().p2().x())/GRID_X, (line().p2().y()-5)/GRID_Y)); break;
        case 3:  polygon.insert(0, 1,  QPoint((line().p2().x()-5)/GRID_X, line().p2().y()/GRID_Y)); break;
        default: polygon.insert(0, 1,  QPoint((line().p2().x())/GRID_X, line().p2().y()/GRID_Y));
    }

    switch(getDirection(srcPort, line().p1()))
    {
        case 0:  polygon.insert(polygon.count(), 1,  QPoint((line().p1().x())/GRID_X, (line().p1().y()+5)/GRID_Y)); break;
        case 1:  polygon.insert(polygon.count(), 1,  QPoint((line().p1().x()+5)/GRID_X, line().p1().y()/GRID_Y)); break;
        case 2:  polygon.insert(polygon.count(), 1,  QPoint((line().p1().x())/GRID_X, (line().p1().y()-5)/GRID_Y)); break;
        case 3:  polygon.insert(polygon.count(), 1,  QPoint((line().p1().x()-5)/GRID_X, line().p1().y()/GRID_Y)); break;
        default: polygon.insert(polygon.count(), 1,  QPoint((line().p1().x())/GRID_X, line().p1().y()/GRID_Y));
    }

    path = new QPainterPath();
    if(!polygon.isEmpty()) {
        path->moveTo(polygon.at(0)*GRID_X);
        for(int i=1;i<polygon.count();i++)
            path->lineTo(polygon.at(i)*GRID_X);
    }
}

bool UvmConnectionView::intersectsWith(UvmConnectionView *other)
{
    qreal x1 = this->getAbsoluteSrcPoint().x();
    qreal y1 = this->getAbsoluteSrcPoint().y();
    qreal x2 = this->getAbsoluteDstPoint().x();
    qreal y2 = this->getAbsoluteDstPoint().y();
    qreal x3 = other->getAbsoluteSrcPoint().x();
    qreal y3 = other->getAbsoluteSrcPoint().y();
    qreal x4 = other->getAbsoluteDstPoint().x();
    qreal y4 = other->getAbsoluteDstPoint().y();

    qreal a1 = (y2 - y1) / (x2 - x1);
    qreal a2 = (y4 - y3) / (x4 - x3);

    if(a1==a2)
        return false;

    qreal b1 = y1 - (a1 * x1);
    qreal b2 = y3 - (a2 * x3);

    qreal xcommun = (b2-b1)/(a1-a2);

    if(x1 < x2)
    {
        if(x3 < x4)
        {
            return (xcommun > x1 && xcommun < x2 && xcommun > x3 && xcommun < x4);
        }
        else
        {
            return (xcommun > x1 && xcommun < x2 && xcommun < x3 && xcommun > x4);
        }
    }
    else
    {
        if(x3 < x4)
        {
            return (xcommun < x1 && xcommun > x2 && xcommun > x3 && xcommun < x4);
        }
        else
        {
            return (xcommun < x1 && xcommun > x2 && xcommun < x3 && xcommun > x4);
        }
    }
}


// Destructor of the connection view
UvmConnectionView::~UvmConnectionView()
{
    delete name;
    delete path;
}
