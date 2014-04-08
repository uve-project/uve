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
#include "uvmtestbenchview.h"
#include <QApplication>


// Constructor with model and parent
UvmTestbenchView::UvmTestbenchView(UvmComponentView* parent, UvmComponent * model, MainWindow * mainwindow) :
    UvmComponentView(parent, model, mainwindow)
{
}


// Calculate the rectangle size depending on children
void UvmTestbenchView::calculRect()
{
    qreal x = INTER_NAME_X;
    qreal y = INTER_NAME_Y;
    qreal yfut = INTER_NAME_Y;
    qreal xfut = INTER_NAME_X;
    rect = QRectF(0,0,0,0);
    rect.setWidth(QApplication::fontMetrics().boundingRect(getTypeName()).width()+15);

    QList<UvmScoreboard*> scoreboards = ((UvmTestbench*)model)->getScoreboards();
    QList<UvmVirtualSequencer*> vseqs = ((UvmTestbench*)model)->getVirtualSequencers();
    QList<UvmVerificationComponent*> verifcomps = ((UvmTestbench*)model)->getVerificationComponents();

    // Calcul rect of scoreboards
    UvmScoreboardView* scoreboardView;
    foreach(UvmScoreboard* scoreboard, scoreboards) {
        scoreboardView = (UvmScoreboardView*)getComponentView(scoreboard);
        scoreboardView->calculRect();
        scoreboardView->setPos(x, y);
        x += scoreboardView->boundingRect().width()+INTER_X;
        yfut = std::max(y+scoreboardView->boundingRect().height()+INTER_Y, yfut);
    }

    // Calcul rect of the vitural sequencer
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

    // Calcul rect of verification components
    UvmVerificationComponentView* verifcompView;
    foreach(UvmVerificationComponent* verifcomp, verifcomps) {
        verifcompView = (UvmVerificationComponentView*)getComponentView(verifcomp);
        verifcompView->calculRect();
        verifcompView->setPos(x, y);
        x += verifcompView->boundingRect().width()+INTER_X;
        yfut = std::max(y+verifcompView->boundingRect().height()+INTER_Y, yfut);
    }

    xfut = std::max(x-INTER_X+INTER_X_END, xfut);

    yfut = yfut-INTER_Y+INTER_Y_END;
    rect = QRectF(0, 0, xfut, yfut);

    rect = placeAccessors(rect);
    placePorts();
}


// Paint the testbench
void UvmTestbenchView::paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget *widget)
{
    UvmComponentView::paint(painter, option, widget);

    painter->drawText(INTER_X, 17, getTypeName());
}


// Get the type name of the testbench
QString UvmTestbenchView::getTypeName()
{
    return "Testbench::" + model->getShortName();
}

void UvmTestbenchView::placePorts()
{
    if(!getPortsViews().empty())
    {
        qDebug() << "TestBenchView has ports!!!";
    }
}

void UvmTestbenchView::untangleConnections()
{
    QList<UvmScoreboard*> scoreboards = ((UvmTestbench*)model)->getScoreboards();
    QList<UvmVirtualSequencer*> vseqs = ((UvmTestbench*)model)->getVirtualSequencers();
    QList<UvmVerificationComponent*> verifcomps = ((UvmTestbench*)model)->getVerificationComponents();

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

    UvmVerificationComponentView* verifcompView;
    foreach(UvmVerificationComponent* verifcomp, verifcomps) {
        verifcompView = (UvmVerificationComponentView*)getComponentView(verifcomp);
        verifcompView->untangleConnections();
    }
}
