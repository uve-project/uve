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
#include "uvmmonitorview.h"


// Constructor with model and parent
UvmMonitorView::UvmMonitorView(UvmComponentView* parent, UvmComponent* model, MainWindow * mainwindow) :
    UvmComponentView(parent, model, mainwindow)
{
}


// Calculate the rectangle size depending on children
void UvmMonitorView::calculRect()
{
    // Set the minimal width depending on the text size
    rect.setWidth(QApplication::fontMetrics().boundingRect(getTypeName()).width()+15);

    intRect = QRectF(rect);

    rect = placeAccessors(rect);
    if(hasAccessors())
    {
        intRect.setTop(20);
    }
    placePorts();
}


// Paint the monitor
void UvmMonitorView::paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget *widget)
{
    UvmComponentView::paint(painter, option, widget);
    painter->drawText(intRect, Qt::AlignCenter, getTypeName());
}


// Get the type name of the monitor
QString UvmMonitorView::getTypeName()
{
    return "Monitor";
}

void UvmMonitorView::placePorts()
{
    QList<UvmPortView*> topPV;
    QList<UvmPortView*> bottomPV;
    foreach(UvmPortView* pv, getPortsViews())
    {
        if(pv->getModel()->getConnectedComponentType() == COLLECTOR || pv->getModel()->getConnectedComponentType() == INTERFACE || pv->getModel()->getConnectedComponentType() == DRIVER)
        {
            bottomPV.append(pv);
        }
        else if(pv->getModel()->getConnectedComponentType() == SEQUENCER)
        {
            pv->setPos(rect.width() + pv->boundingRect().width()/2, rect.height()/2);
        }
        else if(pv->getModel()->getConnectedComponentType() == SCOREBOARD)
        {
            topPV.append(pv);
        }
    }
    if(topPV.empty())
        ;
    else if(topPV.length()==1)
        topPV.at(0)->setPos(rect.width()/2, 0);
    else
    {
        qreal spaceNeeded = (topPV.length() - 1) * (topPV.at(0)->boundingRect().width() + INTER_X);
        qreal y = 0 - topPV.at(0)->boundingRect().height()/2;
        qreal x = rect.width()/2 - spaceNeeded /2;
        foreach(UvmPortView* pv, topPV)
        {
            pv->setPos(x, y);
            x += INTER_X + pv->boundingRect().width();
        }
    }
    if(bottomPV.empty())
        return;
    else if(bottomPV.length()==1)
    {
        if(bottomPV.at(0)->getModel()->getConnectedComponentType() == INTERFACE)
            bottomPV.at(0)->setPos(rect.width()/2, rect.height() - bottomPV.at(0)->boundingRect().height()/2);
        else
            bottomPV.at(0)->setPos(rect.width()/2, rect.height() + bottomPV.at(0)->boundingRect().height()/2);
    }
    else
    {
        qreal spaceNeeded = (bottomPV.length() - 1) * (bottomPV.at(0)->boundingRect().width() + INTER_X);
        qreal x = rect.width()/2 - spaceNeeded /2;
        foreach(UvmPortView* pv, bottomPV)
        {
            qreal y;
            if(pv->getModel()->getConnectedComponentType() == INTERFACE)
                y = rect.height() - pv->boundingRect().height()/2;
            else
                y = rect.height() + pv->boundingRect().height()/2;
            pv->setPos(x, y);
            x += INTER_X + pv->boundingRect().width();
        }
    }
}

void UvmMonitorView::untangleConnections()
{
    //untangle
    bool somethingHasChanged = false;
    do{
        somethingHasChanged = false;
        QList<UvmPortView*> PVs = getPortsViews();

        for(int i=0; i< PVs.length()-1; i++)
        {
            for(int j=i+1; j<PVs.length(); j++)
            {
                if ((PVs.at(i)->getConnections().size()!=0)&&
                        (PVs.at(j)->getConnections().size()!=0))
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
            }
            if(somethingHasChanged)
                break;
        }

    }while(somethingHasChanged);
}
