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
#include "graphicalprojectview.h"

#include <QList>

#include "uvmportview.h"
#include "uvmconnectionview.h"
#include "uvmtestbenchview.h"
#include "uvmtestcaseview.h"
#include "uvmdesignview.h"
#include "uvmcollectorview.h"
#include "uvmvirtualsequencerview.h"
#include "uvmscoreboardview.h"
#include "uvmverificationcomponentview.h"
#include "uvmmonitorview.h"
#include "uvmagentview.h"
#include "uvmsequencerview.h"
#include "uvmdriverview.h"
#include "uvminterfaceview.h"
#include "uvmpackage.h"

//Create the scene and add it to us
GraphicalProjectView::GraphicalProjectView(MainWindow *mainwindow) :
    QGraphicsView((QWidget*)mainwindow),
    mainwindow(mainwindow)
{
    _scene = new QGraphicsScene(this);
    this->setScene(_scene);
}

GraphicalProjectView::~GraphicalProjectView()
{
    if(_scene!= NULL)
        delete _scene;
    _project = NULL;
    //if(board!=NULL)
        //delete board;
}

//Simple getter
UvmProject *GraphicalProjectView::getProject() const
{
    return _project;
}

void GraphicalProjectView::changeActiveComponent(QString pathOfNewlyActiveComponent)
{
    QList<QGraphicsItem*> items = _scene->items();

    foreach(QGraphicsItem* item, items)
    {
        UvmComponentView* currCompView = dynamic_cast<UvmComponentView*>(item);

        if(currCompView != NULL)
        {
            if(currCompView->getModel()->getAbsoluteBodyFileName().compare(pathOfNewlyActiveComponent) == 0)
                currCompView->setSelected(true);
            else
                currCompView->setSelected(false);
        }
    }
}

void GraphicalProjectView::noActiveComponent()
{
    QList<QGraphicsItem*> items = _scene->items();

    foreach(QGraphicsItem* item, items)
    {
        item->setSelected(false);
    }
}

//Save the project and refresh the view
void GraphicalProjectView::setProject(UvmProject *project)
{
    _project = project;
    this->refresh();
}

//Parse the project, and print each element
//Originally written by Daniel Molla, but relocated here by BNZ
void GraphicalProjectView::refresh()
{
    QList<UvmComponent*> compList;
    UvmComponent* curComp;

    UvmComponentView* compView;

    QList<UvmConnection*> connList;

    UvmTop *topModel = _project->getTop();
    topView = new UvmTopView(0,topModel,mainwindow);

    compList.append(topModel);

    while (!compList.isEmpty())
    {
        // Get and remove the first component
        curComp = compList.first();
        compList.removeFirst();

        if (curComp==0)
            continue;

        // Create a component view
        foreach(UvmComponent* childComp , curComp->getChildren())
        {
            compView = createUVMView(topView->getComponentView(curComp), childComp);
            if(compView != NULL)
                CONNECT(childComp, SIGNAL(updateView()), compView, SLOT(applyModelChanges()));
            compList.append(childComp);
        }

        // Create all in port views of the component
        foreach(UvmPort* childPort , curComp->getPorts())
        {
            // We do not create each port, only the transaction ports or interfaces
            if (childPort->getMode()!=UvmPort::DUT)
            {
                new UvmPortView(topView->getComponentView(curComp), childPort);
                foreach(UvmConnection* conn, childPort->getConnections())
                    if(!connList.contains(conn))
                        connList.append(conn);
            }
        }
    }

    // Create all connections
    foreach(UvmConnection* conn, connList)
    {
        UvmPortView *src=topView->getPortView(conn->getSrcPort());
        UvmPortView *dst=topView->getPortView(conn->getDstPort());
        if (src && dst)
            new UvmConnectionView(topView->getPortView(conn->getSrcPort()), topView->getPortView(conn->getDstPort()), conn, topView);
        else
            QMessageBox::warning(0,"Error",QString("Creation of connection %1, %2").arg(conn->getSrcPort()->getName()).arg(conn->getDstPort()->getName()));
    }
    //Do stuff
    topView->calculRect();
    topView->untangleConnections();

    //Route them
    this->routeUVMView(topView);

    //Connect the update signal (model -> view)
    CONNECT(topModel, SIGNAL(updateView()), topView, SLOT(applyModelChanges()));

    //Add the top view to the scene
    _scene->clear();
    _scene->addItem(topView);
}

void GraphicalProjectView::refreshPlacement()
{
    //Do stuff
//    topView->calculRect();
    topView->untangleConnections();

    //Route them
    this->routeUVMView(topView);
}

QPixmap GraphicalProjectView::getPixmap()
{
    // convert QGraphicsScene to QPixmap
    noActiveComponent();
    QRectF sourceRect = _scene->itemsBoundingRect();
    sourceRect.setX(sourceRect.x()-1);
    sourceRect.setY(sourceRect.y()-1);
    QRectF targetRect(0, 0, sourceRect.width()+2, sourceRect.height()+2);
    QPixmap image=QPixmap(sourceRect.width()+2, sourceRect.height()+2);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    _scene->render(&painter, targetRect, sourceRect);
    painter.end();
    return image;
}

//--------------------------------------------- PRIVATE ---------------------------------------------

// Try each possible object, and return the correct one
UvmComponentView *GraphicalProjectView::createUVMView(UvmComponentView *parentView, UvmComponent *comp)
{
    if(dynamic_cast<UvmTestbench*>(comp) != 0)
        return new UvmTestbenchView(parentView, comp, mainwindow);

    else if(dynamic_cast<UvmTestCase*>(comp) != 0)
        return new UvmTestCaseView(parentView, comp, mainwindow);

    else if(dynamic_cast<UvmTop*>(comp) != 0)
        return new UvmTopView(parentView, comp, mainwindow);

    else if(dynamic_cast<UvmDesign*>(comp) != 0)
        return new UvmDesignView(parentView, comp, mainwindow);

    /*else if(dynamic_cast<UvmConfig*>(comp) != 0)
        return new UvmConfigView(parentView, comp, mainwindow);*/

    else if(dynamic_cast<UvmScoreboard*>(comp) != 0)
        return new UvmScoreboardView(parentView, comp, mainwindow);

    else if(dynamic_cast<UvmVerificationComponent*>(comp) != 0)
        return new UvmVerificationComponentView(parentView, comp, mainwindow);

    else if(dynamic_cast<UvmVirtualSequencer*>(comp) != 0)
        return new UvmVirtualSequencerView(parentView, comp, mainwindow);

    else if(dynamic_cast<UvmMonitor*>(comp) != 0)
        return new UvmMonitorView(parentView, comp, mainwindow);

    else if(dynamic_cast<UvmAgent*>(comp) != 0)
        return new UvmAgentView(parentView, comp, mainwindow);

    else if(dynamic_cast<UvmCollector*>(comp) != 0)
        return new UvmCollectorView(parentView, comp, mainwindow);

    else if(dynamic_cast<UvmSequencer*>(comp) != 0)
        return new UvmSequencerView(parentView, comp, mainwindow);

    else if(dynamic_cast<UvmDriver*>(comp) != 0)
        return new UvmDriverView(parentView, comp, mainwindow);

    else if(dynamic_cast<UvmInterface*>(comp) != 0)
        return new UvmInterfaceView(parentView, comp, mainwindow);

    /*else if (dynamic_cast<UvmPackage*>(comp) != 0)
        return new UvmPackageView(parentView, comp, mainwindow);*/

    return 0;
}

//Really, I don't know... Ask Daniel Molla
void GraphicalProjectView::routeUVMView(UvmTopView *topView)
{
    QList<UvmComponentView*> compViewList;
    UvmComponentView* curCompView;
    UvmComponentView* childCompView;
    UvmPortView* childPortView;

    QList<UvmConnectionView*> connList;

    QList<QPolygon> polyList;
    QList<QPointF> srcPoints;
    QList<QPointF> dstPoints;
    QList<QRectF> obstacles;

    compViewList.append(topView);

    while(!compViewList.isEmpty())
    {
        // Get and remove the first component view
        curCompView = compViewList.first();
        compViewList.removeFirst();
        obstacles.append(curCompView->getObstacle(topView));

        // Get all child of the component
        for(int i = 0; i < curCompView->childItems().size(); i++)
        {
            // Create a component view
            if(dynamic_cast<UvmComponentView*>(curCompView->childItems().at(i)) != 0)
            {
                childCompView = (UvmComponentView*) curCompView->childItems().at(i);
                compViewList.append(childCompView);

                // Get all connections view
                for(int j = 0; j < childCompView->childItems().size(); j++) {
                    if(dynamic_cast<UvmPortView*>(childCompView->childItems().at(j)) != 0) {
                        childPortView = (UvmPortView*) childCompView->childItems().at(j);
                        foreach(UvmConnectionView* connView, childPortView->getConnections())
                            if(!connList.contains(connView))
                                connList.append(connView);
                    }
                }
            }
        }
    }

    // Get all sources and destination points
    foreach(UvmConnectionView* connView, connList) {
        connView->updateLine();
        srcPoints.append(connView->getSrcPoint());
        dstPoints.append(connView->getDstPoint());

        compViewList.append((UvmComponentView*)connView->getSrcPortView()->parentItem());
        compViewList.append((UvmComponentView*)connView->getDstPortView()->parentItem());
        while(!compViewList.isEmpty())
        {
            curCompView = compViewList.first();
            compViewList.removeFirst();
            if(curCompView->parentItem() != 0) {
                compViewList.append((UvmComponentView*)curCompView->parentItem());
                obstacles.removeOne(((UvmComponentView*)curCompView->parentItem())->getObstacle(topView));
            }
        }
    }

    /*
    // Route all connections of a component
    RoutingBoard *board = new RoutingBoard();
    QList<QPoint> src;
    QList<QPoint> dst;
    foreach (QPointF p,srcPoints)
        src.append(p.toPoint());
    foreach (QPointF p,dstPoints)
        dst.append(p.toPoint());
    polyList = board->route(topView->getRoutingParent(), src, dst, obstacles);
    delete board;

*/
    QPolygon poly;

    //Set the polygon for each connection
    foreach(UvmConnectionView* connView, connList)
        connView->setPolygon(poly);
        //connView->setPolygon(polyList.at(connList.indexOf(connView)));

}
