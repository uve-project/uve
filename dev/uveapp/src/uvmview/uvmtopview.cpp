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
#include "uvmtopview.h"
#include <QApplication>


// Constructor with model and parent
UvmTopView::UvmTopView(UvmComponentView* parent, UvmComponent * model, MainWindow* mainwindow) :
    UvmComponentView(parent, model, mainwindow)
{
}


// Calculate the rectangle size depending on children
void UvmTopView::calculRect()
{
    qreal x = INTER_NAME_X;
    qreal y = INTER_NAME_Y;
    rect = QRectF(0,0,0,0);
    rect.setWidth(QApplication::fontMetrics().boundingRect(getTypeName()).width()+15);

    UvmTestCase* testCase = ((UvmTop*)model)->getTestCase();
    UvmDesign* design = ((UvmTop*)model)->getDesign();
    UvmInterface* uInterface = ((UvmTop*)model)->getInterface();

    UvmTestCaseView* testCaseView = (UvmTestCaseView*)getComponentView(testCase);
    if(testCaseView != 0) {
        testCaseView->calculRect();
        testCaseView->setPos(x, y);
        y += testCaseView->boundingRect().height()+INTER_Y;
        x += testCaseView->boundingRect().width();

        UvmInterfaceView* interfaceView = (UvmInterfaceView*)getComponentView(uInterface);
        if(interfaceView != 0) {
            interfaceView->calculRect();
            interfaceView->setPos(x/2-interfaceView->boundingRect().width()/2, y);
            y += interfaceView->boundingRect().height()+INTER_Y;
        }

        UvmDesignView* designView = (UvmDesignView*)getComponentView(design);
        if(interfaceView != 0) {
            designView->calculRect();
            designView->setPos(x/2-designView->boundingRect().width()/2, y);
            y += designView->boundingRect().height()+INTER_Y;
        }
    }

    y = y-INTER_Y+INTER_Y_END;
    x += INTER_X_END;

    rect = QRectF(0, 0, x, y);
    rect = placeAccessors(rect);
    placePorts();
}


// Paint the top
void UvmTopView::paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget *widget)
{
    UvmComponentView::paint(painter, option, widget);
    painter->drawText(INTER_X, 17, getTypeName());
}


// Get the type name of the top
QString UvmTopView::getTypeName()
{
    return "Top::" + model->getShortName();
}

void UvmTopView::placePorts()
{
    if(!getPortsViews().empty())
    {
        qDebug() << "TopView has ports!!!";
    }
}

void UvmTopView::untangleConnections()
{
    UvmTestCase* testCase = ((UvmTop*)model)->getTestCase();
    UvmDesign* design = ((UvmTop*)model)->getDesign();
    UvmInterface* uInterface = ((UvmTop*)model)->getInterface();

    UvmTestCaseView* testCaseView = (UvmTestCaseView*)getComponentView(testCase);
    if(testCaseView != 0) {
        testCaseView->untangleConnections();

        UvmInterfaceView* interfaceView = (UvmInterfaceView*)getComponentView(uInterface);
        if(interfaceView != 0) {
            interfaceView->untangleConnections();
        }

        UvmDesignView* designView = (UvmDesignView*)getComponentView(design);
        if(interfaceView != 0) {
            designView->untangleConnections();
        }
    }
}
