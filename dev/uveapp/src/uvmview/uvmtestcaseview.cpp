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
#include "uvmtestcaseview.h"


// Constructor with model and parent
UvmTestCaseView::UvmTestCaseView(UvmComponentView* parent, UvmComponent * model, MainWindow* mainwindow) :
    UvmComponentView(parent, model, mainwindow)
{
    baseColor = Qt::gray;
}


// Calculate the rectangle size depending on children
void UvmTestCaseView::calculRect()
{
    qreal x = INTER_NAME_X;
    qreal y = INTER_NAME_Y;
    rect = QRectF(0,0,0,0);

    UvmTestbench* testbench = ((UvmTestCase*)model)->getTestbench();

    UvmTestbenchView* testbenchView = (UvmTestbenchView*)getComponentView(testbench);
    if(testbenchView != 0) {
        testbenchView->calculRect();
        testbenchView->setPos(x, y);
        y += testbenchView->boundingRect().height()+INTER_Y;
        x += testbenchView->boundingRect().width()+INTER_X_END;
    }

    y = y-INTER_Y+INTER_Y_END;
    rect = QRectF(0, 0, x, y);
    rect = placeAccessors(rect);
    placePorts();
}


// Paint the test case
void UvmTestCaseView::paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget *widget)
{
    UvmComponentView::paint(painter, option, widget);

    painter->drawText(INTER_X, 17, getTypeName());
}


// Get the type name of the test case
QString UvmTestCaseView::getTypeName()
{
    return "TestCase::" + model->getShortName();
}

void UvmTestCaseView::placePorts()
{
    if(!getPortsViews().empty())
    {
        qDebug() << "TestCaseView has ports!!!";
    }
}

void UvmTestCaseView::untangleConnections()
{
    UvmTestbench* testbench = ((UvmTestCase*)model)->getTestbench();

    UvmTestbenchView* testbenchView = (UvmTestbenchView*)getComponentView(testbench);
    if(testbenchView != 0) {
        testbenchView->untangleConnections();
    }
}
