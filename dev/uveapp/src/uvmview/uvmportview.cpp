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
#include "uvmportview.h"
#include <QPainter>

// Constructor with model and parent
UvmPortView::UvmPortView(UvmComponentView* parent, UvmPort* model)
{
    this->model = model;
    this->setParentItem(parent);

    //DO NOT CHANGE
    if(model->getMode() == UvmPort::DUT && model->getDirection() == UvmPort::PORT_IN)
        poly << QPointF(0,5) << QPointF(5, 0) << QPointF(0,-5) << QPointF(0,5);

    else if(model->getMode() == UvmPort::DUT && model->getDirection() == UvmPort::PORT_OUT)
        poly << QPointF(0,5) << QPointF(5, 0) << QPointF(0,-5) << QPointF(0,5);

    else if(model->getMode() == UvmPort::DUT && model->getDirection() == UvmPort::PORT_INOUT)
        poly << QPointF(-5,0) << QPointF(0, 5) << QPointF(5,0) << QPointF(0,-5) << QPointF(-5, 0);

    //inside square
    else if(model->getMode() == UvmPort::DUT_INTERFACE || model->getMode() == UvmPort::VC_INTERFACE || model->getMode() == UvmPort::VCTODUT_INTERFACE)
        poly << QPointF(-5, 5) << QPointF(5, 5) << QPointF(5, -5) << QPointF(-5, -5) << QPointF(-5, 5);

    else if((model->getMode() == UvmPort::UVM_ANALYSIS_IMP)||
            (model->getMode() == UvmPort::UVM_BLOCKING_PUT_IMP)||
            (model->getMode() == UvmPort::UVM_BLOCKING_GET_IMP)||
            (model->getMode() == UvmPort::UVM_SEQ_ITEM_PULL_IMP)||
            (model->getMode() == UvmPort::UVM_BLOCKING_PEEK_IMP))
        poly << QPointF(-5,0) << QPointF(0, 5) << QPointF(5,0) << QPointF(0,-5) << QPointF(-5, 0);

    else if((model->getMode() == UvmPort::UVM_ANALYSIS_PORT && model->getParent()->getComponentType() == MONITOR))
        poly << QPointF(-5,0) << QPointF(0, 5) << QPointF(5,0) << QPointF(0,-5) << QPointF(-5, 0);

    else if((model->getMode() == UvmPort::UVM_ANALYSIS_PORT) ||
            (model->getMode() == UvmPort::UVM_BLOCKING_PUT_PORT) ||
            (model->getMode() == UvmPort::UVM_BLOCKING_GET_PORT) ||
            (model->getMode() == UvmPort::UVM_SEQ_ITEM_PULL_PORT) ||
            (model->getMode() == UvmPort::UVM_BLOCKING_PEEK_PORT))
        poly << QPointF(-5, 5) << QPointF(5, 5) << QPointF(5, -5) << QPointF(-5, -5) << QPointF(-5, 5);
    else
        poly << QPointF(0,0);


    setZValue(1.0);
}


// Define the outer bounds of the item as a rectangle
QRectF UvmPortView::boundingRect() const
{
    if(model->getMode() == UvmPort::VIRT_SEQ_TO_SEQ_OUT)
        return QRectF(0,0,0,0);
    return poly.boundingRect();
}


// Paint the contents of an item in local coordinates
void UvmPortView::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{

    // Paint an in port
    if(model->getMode() == UvmPort::DUT && model->getDirection() == UvmPort::PORT_IN)
    {
        painter->setRenderHint(QPainter::Antialiasing);
        QBrush newBrush(QColor(0,255,0));
        painter->setBrush(newBrush);
        painter->drawPolygon(poly, Qt::WindingFill);
        painter->drawText(12, 3, model->getName());
    }
    // Paint an out port
    if(model->getMode() == UvmPort::DUT && model->getDirection() == UvmPort::PORT_OUT)
    {
        painter->setRenderHint(QPainter::Antialiasing);
        QBrush newBrush(QColor(0,0,255));
        painter->setBrush(newBrush);
        painter->drawPolygon(poly, Qt::WindingFill);
        QRect bounding = painter->boundingRect(QRect(-70,-8,65,20), Qt::AlignRight, model->getName());
        painter->drawText(bounding, Qt::AlignRight, model->getName());
    }

    // Paint an inout port
    if(model->getMode() == UvmPort::DUT && model->getDirection() == UvmPort::PORT_INOUT)
    {
    }
    // Paint an interface port
    if(model->getMode() == UvmPort::DUT_INTERFACE || model->getMode() == UvmPort::VC_INTERFACE || model->getMode() == UvmPort::VCTODUT_INTERFACE)
    {
        painter->setRenderHint(QPainter::Antialiasing);
        QBrush newBrush(QColor(255,0,0));
        painter->setBrush(newBrush);
        painter->drawPolygon(poly, Qt::WindingFill);
        //painter->drawText(poly.boundingRect(), Qt::AlignCenter, model->getName());
    }
    // Paint a tlm round port
    if((model->getMode() == UvmPort::UVM_ANALYSIS_IMP)||
            (model->getMode() == UvmPort::UVM_BLOCKING_PUT_IMP)||
            (model->getMode() == UvmPort::UVM_BLOCKING_GET_IMP)||
            (model->getMode() == UvmPort::UVM_SEQ_ITEM_PULL_IMP)||
            (model->getMode() == UvmPort::UVM_BLOCKING_PEEK_IMP))
    {
        painter->setRenderHint(QPainter::Antialiasing);
        QBrush newBrush(QColor(255,0,0));
        painter->setBrush(newBrush);
        painter->drawEllipse(QPointF(0, 0), 5, 5);
    }
        // Paint a tlm diamond port
        if((model->getMode() == UvmPort::UVM_ANALYSIS_PORT && false /* doit etre sur un moniteur!*/))
        {
            painter->setRenderHint(QPainter::Antialiasing);
            QBrush newBrush(QColor(255,0,0));
            painter->setBrush(newBrush);
            painter->drawPolygon(poly, Qt::WindingFill);
        }
    // Paint a tlm square port
    if((model->getMode() == UvmPort::UVM_ANALYSIS_PORT) ||
            (model->getMode() == UvmPort::UVM_BLOCKING_PUT_PORT) ||
            (model->getMode() == UvmPort::UVM_BLOCKING_GET_PORT) ||
            (model->getMode() == UvmPort::UVM_SEQ_ITEM_PULL_PORT) ||
            (model->getMode() == UvmPort::UVM_BLOCKING_PEEK_PORT))
    {
        painter->setRenderHint(QPainter::Antialiasing);
        QBrush newBrush(QColor(255,0,0));
        painter->setBrush(newBrush);
        painter->drawPolygon(poly, Qt::WindingFill);
    }

    //Do not paint the global one
}


// Set the model of the view
void UvmPortView::setModel(UvmPort* model)
{
    this->model = model;
}


// Get the model of the view
UvmPort* UvmPortView::getModel()
{
    return model;
}


// Add a connection view
void UvmPortView::addConnection(UvmConnectionView* connection)
{
    if(!uvmConnectionsView.contains(connection))
        uvmConnectionsView.append(connection);
}


// Remove a connection view
void UvmPortView::delConnection(UvmConnectionView* connection)
{
    uvmConnectionsView.removeOne(connection);
}


// Get all connections view
QList<UvmConnectionView *> UvmPortView::getConnections()
{
    return uvmConnectionsView;
}


// Apply all model changes
void UvmPortView::applyModelChanges()
{
}


UvmPortView::~UvmPortView()
{
    UvmConnectionView* connView;

    int nbrElement = uvmConnectionsView.count();
    for(int i=0; i < nbrElement; i++ ){
        connView = uvmConnectionsView.takeAt(0);
        if(model->getDirection() == UvmPort::PORT_IN)
            delete connView;
    }
}
