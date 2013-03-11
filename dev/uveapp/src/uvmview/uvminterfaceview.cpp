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
#include "uvminterfaceview.h"


// Constructor with model and parent
UvmInterfaceView::UvmInterfaceView(UvmComponentView* parent, UvmComponent * model, MainWindow * mainwindow) :
    UvmComponentView(parent, model, mainwindow)
{
}


// Calculate the rectangle size depending on children
void UvmInterfaceView::calculRect()
{
    // Set the minimal width depending on the text size
    rect.setWidth(QApplication::fontMetrics().boundingRect(getTypeName()).width()+15);

    intRect =QRectF(rect);

    placePorts();

    rect = placeAccessors(rect);
    if(hasAccessors())
    {
        intRect.setTop(20);
    }

    placePorts(); //second call for vertical placement. Needs improvements.
}


// Paint the interface
void UvmInterfaceView::paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget *widget)
{
    UvmComponentView::paint(painter, option, widget);
    painter->drawText(intRect, Qt::AlignCenter, getTypeName());
}


// Get the type name of the interface
QString UvmInterfaceView::getTypeName()
{
    return "Interface";
}

void UvmInterfaceView::placePorts()
{
    QList<UvmPortView*> topPV;
    QList<UvmPortView*> bottomPV;
    foreach(UvmPortView* pv, getPortsViews())
    {
        if(pv->getModel()->getConnectedComponentType() == DESIGN)
            bottomPV.append(pv);
        else if(pv->getModel()->getConnectedComponentType() == MONITOR || pv->getModel()->getConnectedComponentType() == DRIVER || pv->getModel()->getConnectedComponentType() == COLLECTOR)
        {
            topPV.append(pv);
        }
        else if(pv->getModel()->getConnectedComponentType() == INTERFACE)
        {
            if(getModel()->getParent()->getComponentType() == VERIFICATION_COMPONENT)
                bottomPV.append(pv);
            else
                topPV.append(pv);
        }
    }
    qreal spaceNeededTop;
    if (topPV.empty())
        spaceNeededTop = 0.0;
    else
        spaceNeededTop = (topPV.length() - 1) * (topPV.at(0)->boundingRect().width() + INTER_X);
    qreal spaceNeededBottom = (bottomPV.length() - 1) * (bottomPV.at(0)->boundingRect().width() + INTER_X);
    if(spaceNeededTop +20 > rect.width())
    {
        rect.setWidth(spaceNeededTop +40);
        intRect.setWidth(spaceNeededTop +40);
    }
    if(spaceNeededBottom +20 > rect.width())
    {
        rect.setWidth(spaceNeededBottom +40);
        intRect.setWidth(spaceNeededTop +40);
    }
    if(topPV.empty())
        ;
    else if(topPV.length()==1)
        topPV.at(0)->setPos(rect.width()/2, topPV.at(0)->boundingRect().height()/2);
    else
    {
        qreal y = topPV.at(0)->boundingRect().height()/2;
        qreal x = rect.width()/2 - spaceNeededTop /2;
        foreach(UvmPortView* pv, topPV)
        {
            pv->setPos(x, y);
            x += INTER_X + pv->boundingRect().width();
        }
    }
    if(bottomPV.empty())
        return;
    if(bottomPV.length()==1)
        bottomPV.at(0)->setPos(rect.width()/2, rect.height() - bottomPV.at(0)->boundingRect().height()/2);
    else
    {
        qreal y = rect.height() - bottomPV.at(0)->boundingRect().height()/2;
        qreal x = rect.width()/2 - spaceNeededBottom /2;
        foreach(UvmPortView* pv, bottomPV)
        {
            pv->setPos(x, y);
            x += INTER_X + pv->boundingRect().width();
        }
    }
}

void UvmInterfaceView::untangleConnections()
{
    bool somethingHasChanged = false;
    do{
        somethingHasChanged = false;
        QList<UvmPortView*> PVs = getPortsViews();

        for(int i=0; i< PVs.length()-1; i++)
        {
            for(int j=i+1; j<PVs.length(); j++)
            {
                //test si les connections se croisent.
                UvmConnectionView* c1 = PVs.at(i)->getConnections().at(0);
                UvmConnectionView* c2 = PVs.at(j)->getConnections().at(0);
                if(c1->intersectsWith(c2))
                {
                    QPointF pos1 = PVs.at(i)->pos();
                    QPointF pos2 = PVs.at(j)->pos();
                    PVs.at(i)->setPos(pos2);
                    PVs.at(j)->setPos(pos1);
                    somethingHasChanged = true;
                    break;
                }
            }
            if(somethingHasChanged)
                break;
        }

    }while(somethingHasChanged);
}
