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
#include "uvmverificationcomponentview.h"


// Constructor with model and parent
UvmVerificationComponentView::UvmVerificationComponentView(UvmComponentView* parent, UvmComponent * model, MainWindow * mainwindow) :
    UvmComponentView(parent, model, mainwindow)
{
}


// Calculate the rectangle size depending on children
void UvmVerificationComponentView::calculRect()
{
    qreal x = INTER_NAME_X;
    qreal y = INTER_NAME_Y;
    qreal yfut = INTER_NAME_Y;
    qreal xfut = 0;

    UvmMonitor* busmon = ((UvmVerificationComponent*)model)->getBusMonitor();
    UvmInterface* uInterface = ((UvmVerificationComponent*)model)->getInterface();
    QList<UvmScoreboard*> scoreboards = ((UvmVerificationComponent*)model)->getScoreboards();
    QList<UvmVirtualSequencer*> vseqs = ((UvmVerificationComponent*)model)->getVirtualSequencers();
    QList<UvmAgent*> agents = ((UvmVerificationComponent*)model)->getAgents();

    UvmScoreboardView* scoreboardView;
    foreach(UvmScoreboard* scoreboard, scoreboards) {
        scoreboardView = (UvmScoreboardView*)getComponentView(scoreboard);
        scoreboardView->calculRect();
        scoreboardView->setPos(x, y);
        x += scoreboardView->boundingRect().width()+INTER_X;
        yfut = std::max(y+scoreboardView->boundingRect().height()+INTER_Y, yfut);
    }

    UvmVirtualSequencerView* vseqView;
    foreach(UvmVirtualSequencer* vseq, vseqs) {
        vseqView = (UvmVirtualSequencerView*)getComponentView(vseq);
        vseqView->calculRect();
        vseqView->setPos(x, y);
        x += vseqView->boundingRect().width()+INTER_X;
        yfut = std::max(y+vseqView->boundingRect().height()+INTER_Y, yfut);
    }

    y = yfut;
    xfut = x;
    x = INTER_NAME_X;

    UvmMonitorView* monView = (UvmMonitorView*)getComponentView(busmon);
    if(monView != 0) {
        monView->calculRect();
        monView->setPos(x, y);
        x += monView->boundingRect().width()+INTER_X;
        yfut = std::max(y+monView->boundingRect().height()+INTER_Y, yfut);
    }

    UvmAgentView* agentView;
    foreach(UvmAgent* verifcomp, agents) {
        agentView = (UvmAgentView*)getComponentView(verifcomp);
        agentView->calculRect();
        agentView->setPos(x, y);
        x += agentView->boundingRect().width()+INTER_X;
        yfut = std::max(y+agentView->boundingRect().height()+INTER_Y, yfut);
    }


    if(monView != 0) {
        monView->setPos(INTER_NAME_X, yfut-INTER_Y-monView->boundingRect().height());
    }

    xfut = std::max(x-INTER_X+INTER_X_END, xfut);


    UvmInterfaceView* interfaceView = (UvmInterfaceView*)getComponentView(uInterface);
    if(interfaceView != 0) {
        interfaceView->calculRect();
        interfaceView->setPos((xfut/2)-interfaceView->boundingRect().width()/2,yfut);
        yfut += interfaceView->boundingRect().height()+INTER_Y;
    }

    yfut = yfut-INTER_Y+INTER_Y_END;
    rect = QRectF(0, 0, xfut, yfut);

    rect = placeAccessors(rect);
    placePorts();
}


// Paint the vc
void UvmVerificationComponentView::paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget *widget)
{
    UvmComponentView::paint(painter, option, widget);

    painter->drawText(INTER_X, 17, getTypeName());
}


// Get the type name of the vc
QString UvmVerificationComponentView::getTypeName()
{
    return "VC::" + model->getShortName();
}

void UvmVerificationComponentView::placePorts()
{
    if(!getPortsViews().empty())
    {
        qDebug() << "VerificationComponentView has ports!!!";
    }
}

void UvmVerificationComponentView::untangleConnections()
{
    UvmMonitor* busmon = ((UvmVerificationComponent*)model)->getBusMonitor();
    UvmInterface* uInterface = ((UvmVerificationComponent*)model)->getInterface();
    QList<UvmScoreboard*> scoreboards = ((UvmVerificationComponent*)model)->getScoreboards();
    QList<UvmVirtualSequencer*> vseqs = ((UvmVerificationComponent*)model)->getVirtualSequencers();
    QList<UvmAgent*> agents = ((UvmVerificationComponent*)model)->getAgents();

    UvmScoreboardView* scoreboardView;
    foreach(UvmScoreboard* scoreboard, scoreboards) {
        scoreboardView = (UvmScoreboardView*)getComponentView(scoreboard);
        scoreboardView->untangleConnections();
    }

    UvmVirtualSequencerView* vseqView;
    foreach(UvmVirtualSequencer* vseq, vseqs) {
        vseqView = (UvmVirtualSequencerView*)getComponentView(vseq);
        vseqView->untangleConnections();
    }

    UvmMonitorView* monView = (UvmMonitorView*)getComponentView(busmon);
    if(monView != 0) {
        monView->untangleConnections();
    }

    UvmAgentView* agentView;
    foreach(UvmAgent* verifcomp, agents) {
        agentView = (UvmAgentView*)getComponentView(verifcomp);
        agentView->untangleConnections();
    }

    UvmInterfaceView* interfaceView = (UvmInterfaceView*)getComponentView(uInterface);
    if(interfaceView != 0) {
        interfaceView->untangleConnections();
    }
}
