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
#include "uvmagentview.h"
#include <QApplication>


// Constructor with model and parent
UvmAgentView::UvmAgentView(UvmComponentView* parent, UvmComponent * model, MainWindow * mainwindow) :
    UvmComponentView(parent, model, mainwindow)
{
}


// Calculate the rectangle size depending on children
void UvmAgentView::calculRect()
{
    qreal x = INTER_NAME_X;
    qreal y = INTER_NAME_Y;
    qreal yfut = 0;
    qreal xfut = 0;
    rect = QRectF(0,0,0,0);

    UvmCollector* collector = ((UvmAgent*)model)->getCollector();
    UvmSequencer* sequencer = ((UvmAgent*)model)->getSequencer();
    UvmMonitor* monitor = ((UvmAgent*)model)->getMonitor();
    UvmDriver* driver = ((UvmAgent*)model)->getDriver();


    UvmMonitorView* monitorView = (UvmMonitorView*)getComponentView(monitor);
    if(monitorView != 0) {
        monitorView->calculRect();
        monitorView->setPos(x, y);
        y += monitorView->boundingRect().height()+INTER_Y;
        xfut = std::max(x+monitorView->boundingRect().width()+INTER_X, xfut);
    }

    UvmCollectorView* collView = (UvmCollectorView*)getComponentView(collector);
    if(collView != 0) {
        collView->calculRect();
        collView->setPos(x, y);
        y += collView->boundingRect().height()+INTER_Y;
        xfut = std::max(x+collView->boundingRect().width()+INTER_X, xfut);
    }

    if(monitorView != 0) // Problem? without seq?
        xfut += 2*INTER_X;

    if(collView != 0 && monitorView != 0) {
        if(collView->boundingRect().width() > monitorView->boundingRect().width())
            monitorView->setX(x+collView->boundingRect().width()/2-monitorView->boundingRect().width()/2 );
        else
            collView->setX(x+monitorView->boundingRect().width()/2-collView->boundingRect().width()/2);
    }


    x = xfut;
    yfut = std::max(y, yfut);
    y = INTER_NAME_Y;

    UvmSequencerView* seqView = (UvmSequencerView*)getComponentView(sequencer);
    if(seqView != 0) {
        seqView->calculRect();
        seqView->setPos(x, y);
        y += seqView->boundingRect().height()+INTER_Y;
        xfut = std::max(x+seqView->boundingRect().width()+INTER_X, xfut);
    }

    UvmDriverView* driverView = (UvmDriverView*)getComponentView(driver);
    if(driverView != 0) {
        driverView->calculRect();
        driverView->setPos(x, y);
        y += driverView->boundingRect().height()+INTER_Y;
        xfut = std::max(x+driverView->boundingRect().width()+INTER_X, xfut);
    }

    if(driverView != 0 && seqView != 0) {
        if(driverView->boundingRect().width() > seqView->boundingRect().width())
            seqView->setX(x+driverView->boundingRect().width()/2-seqView->boundingRect().width()/2);
        else
            driverView->setX(x+seqView->boundingRect().width()/2-driverView->boundingRect().width()/2);
    }

    xfut = xfut-INTER_X+INTER_X_END;
    if(model->hasHeaderFile() && xfut < QApplication::fontMetrics().boundingRect(getTypeName()).width()+35)
    {
        xfut= QApplication::fontMetrics().boundingRect(getTypeName()).width()+35;
    }
    yfut = std::max(y, yfut);
    yfut = yfut-INTER_Y+INTER_Y_END;

    intRect = QRectF(0,0,xfut,yfut);
    intRect = placeAccessors(intRect);
    placePorts();

    qreal xExt = intRect.width() + (((UvmAgent*)model)->getNbAgents() -1)*DELTA_PIX;
    qreal yExt = intRect.height() + (((UvmAgent*)model)->getNbAgents() -1)*DELTA_PIX;

    rect = QRectF(0, 0, xExt, yExt);
}

// Paint the agent
void UvmAgentView::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{

    painter->setRenderHint(QPainter::Antialiasing);
    for(int i=(((UvmAgent*)model)->getNbAgents())-1; i>=0; i--)
    {
        QBrush brush(Qt::darkGray);
        brush.setStyle(Qt::SolidPattern);
        painter->setBrush(brush);
        QPen oldPen = painter->pen();
        QPen pen(Qt::NoPen);
        painter->setPen(pen);
        painter->drawRoundedRect(i*DELTA_PIX+2, i*DELTA_PIX+2, intRect.width(), intRect.height(), 10, 10);

        //painter->drawRoundedRect(2, 2, rect.width(), rect.height(), 10, 10);
        painter->setPen(oldPen);

        // Set main rectangle
        QLinearGradient linearGrad(QPointF(0, 0), QPointF(rect.width()/2, rect.height()/2));
        linearGrad.setColorAt(0, Qt::white);
        if(isSelected()) {
            linearGrad.setColorAt(1, QColor(100,255,100));
        } else {
            //If the model's SV file doesn't exist, paint darker
            if (QFileInfo(model->getAbsoluteBodyFileName()).exists())
                linearGrad.setColorAt(1, baseColor);
            else
                linearGrad.setColorAt(1, noFileColor);
        }
        QBrush newBrush(linearGrad);
        painter->setBrush(newBrush);

        painter->drawRoundedRect(i*DELTA_PIX, i*DELTA_PIX, intRect.width(), intRect.height(), 10, 10);
    }
    QString tableWithIndex ="";
    if((((UvmAgent*)model)->getNbAgents()) > 1)
    {
        tableWithIndex = "["+ QString().setNum(((UvmAgent*)model)->getNbAgents()) +"]";
    }
    painter->drawText(INTER_X, 17, getTypeName() + tableWithIndex);
}


// Get the type name of the agent
QString UvmAgentView::getTypeName()
{
    return QString("Agent::") + model->getClassName().replace(QRegExp(".*_"), "");
}

void UvmAgentView::placePorts()
{
    if(!getPortsViews().empty())
    {
        qDebug() << "AgentsView has ports!!!";
    }
}

void UvmAgentView::untangleConnections()
{
    UvmCollector* collector = ((UvmAgent*)model)->getCollector();
    UvmSequencer* sequencer = ((UvmAgent*)model)->getSequencer();
    UvmMonitor* monitor = ((UvmAgent*)model)->getMonitor();
    UvmDriver* driver = ((UvmAgent*)model)->getDriver();


    UvmMonitorView* monitorView = (UvmMonitorView*)getComponentView(monitor);
    if(monitorView != 0) {
        monitorView->untangleConnections();
    }

    UvmCollectorView* collView = (UvmCollectorView*)getComponentView(collector);
    if(collView != 0) {
        collView->untangleConnections();
    }

    UvmSequencerView* seqView = (UvmSequencerView*)getComponentView(sequencer);
    if(seqView != 0) {
        seqView->untangleConnections();
    }

    UvmDriverView* driverView = (UvmDriverView*)getComponentView(driver);
    if(driverView != 0) {
        driverView->untangleConnections();
    }
}
