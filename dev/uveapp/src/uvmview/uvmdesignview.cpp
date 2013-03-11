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
#include "uvmdesignview.h"


// Constructor with model and parent
UvmDesignView::UvmDesignView(UvmComponentView* parent, UvmComponent * model, MainWindow * mainwindow) :
    UvmComponentView(parent, model, mainwindow)
{
}


// Calculate the rectangle size depending on children
void UvmDesignView::calculRect()
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

// Paint the design
void UvmDesignView::paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget *widget)
{
    UvmComponentView::paint(painter, option, widget);
    painter->drawText(intRect, Qt::AlignCenter, getTypeName());
}


// Get the type name of the design
QString UvmDesignView::getTypeName()
{
    return "Design::" + model->getShortName();
}

void UvmDesignView::placePorts()
{
    foreach(UvmPortView* pv, getPortsViews())
    {
        if(pv->getModel()->getConnectedComponentType() == INTERFACE)
        {
            pv->setPos(rect.width() /2, pv->boundingRect().height()/2);
        }
    }
}

void UvmDesignView::untangleConnections()
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
