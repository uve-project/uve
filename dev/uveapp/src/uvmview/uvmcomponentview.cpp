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
#include "uvmcomponentview.h"
#include <iostream>
#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QAction>
#include <QFileDialog>

using namespace std;

#include "mainwindow.h"
#include "headerfileaccessor.h"

// Constructor with model and parent
UvmComponentView::UvmComponentView(UvmComponentView* parent, UvmComponent * model, MainWindow* mainwindow) :
    mainwindow(mainwindow)
{
    rect = QRectF(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);
    setParentItem(parent);
    this->model = model;
    baseColor = Qt::gray;
    noFileColor = QColor(143,64,64);

    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setZValue(1.0);

    QString toolTip;
    toolTip += "<table>";
    toolTip += "<tr><td>Class&nbsp;name:</td><td>"+model->getClassName()+"</td></tr>";
    toolTip += "<tr><td>Inst&nbsp;name:</td><td>"+model->getInstName()+"</td></tr>";
    if (model->hasHeaderFile())
        toolTip += "<tr><td>Header&nbsp;file:&nbsp;</td><td>"+model->getAbsoluteHeaderFileName()+"</td></tr>";
    toolTip += "<tr><td>Body&nbsp;file:&nbsp;</td><td>"+model->getAbsoluteBodyFileName()+"</td></tr>";
    foreach (QString fileName,model->getOtherFiles())
        toolTip += "<tr><td>Other&nbsp;file:&nbsp;</td><td>"+fileName+"</td></tr>";
    toolTip += "</table>";


    setToolTip(toolTip);
    this->setFlag(QGraphicsItem::ItemIsMovable,true);
}


// Define the outer bounds of the item as a rectangle
QRectF UvmComponentView::boundingRect() const
{
    return rect;
}


// Get the obstacles representation of the component view
QRectF UvmComponentView::getObstacle(UvmComponentView* topView)
{
    tmpRect = mapRectToItem(topView, rect);
    return QRectF((tmpRect.x()-5)/GRID_X, (tmpRect.y()-5)/GRID_Y, (tmpRect.width()+10)/GRID_X, (tmpRect.height()+10)/GRID_Y);
}


// Get the the routing layout of the component view
QRectF UvmComponentView::getRoutingParent()
{
    return QRectF(rect.x()/GRID_X, rect.y()/GRID_Y, rect.width()/GRID_X, rect.height()/GRID_Y);
}


// Paint the contents of an item in local coordinates
void UvmComponentView::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    painter->setRenderHint(QPainter::Antialiasing);

    QBrush brush(Qt::darkGray);
    brush.setStyle(Qt::SolidPattern);
    painter->setBrush(brush);
    QPen oldPen = painter->pen();
    QPen pen(Qt::NoPen);
    painter->setPen(pen);
    painter->drawRoundedRect(2, 2, rect.width(), rect.height(), 10, 10);
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
    painter->drawRoundedRect(0, 0, rect.width(), rect.height(), 10, 10);
}


// Set the model of the UVM block
void UvmComponentView::setModel(UvmComponent* model)
{
    this->model = model;
}


// Get the model of the UVM block
UvmComponent* UvmComponentView::getModel()
{
    return model;
}


// Apply all model changes
void UvmComponentView::applyModelChanges()
{
}


// Get the type name of the component
QString UvmComponentView::getTypeName()
{
    return "Component";
}

QRectF UvmComponentView::placeAccessors(QRectF rect)
{
    int fileAccessors =0;

    if(model->hasConfig())
        fileAccessors++;
    if(model->hasSequenceItem())
        fileAccessors++;
    if(model->hasSequence())
        fileAccessors++;
    if(model->hasHeaderFile())
        fileAccessors++;
    if(model->hasPackage())
        fileAccessors++;
    if(!model->getOtherFiles().isEmpty())
        fileAccessors++;

    if(!fileAccessors > 0)
        return rect;

    if(model->getChildren().empty())
    {
        qreal minWidth = (35 * fileAccessors > rect.width()? 35* fileAccessors : rect.width());
        rect = QRectF(0,0,minWidth,rect.height() + 10);
    }
    else
    {
        if(rect.width() <= (QApplication::fontMetrics().boundingRect(getTypeName()).width() + 35 * fileAccessors))
        {
            rect = QRectF(0,0,QApplication::fontMetrics().boundingRect(getTypeName()).width() + 35 * fileAccessors,rect.height());
        }
    }

    if(!model->getOtherFiles().isEmpty())
    {
        ofa= new OtherFilesAccessor(this,model,mainwindow);
        ofa->calculRect();
        ofa->setPos(rect.width()-(20 * fileAccessors),4);
        fileAccessors--;
    }
    if(model->hasSequence())
    {
        sfa= new SequenceAccessor(this,model,mainwindow);
        sfa->calculRect();
        sfa->setPos(rect.width()-(20 * fileAccessors),4);
        fileAccessors--;
    }
    if(model->hasSequenceItem())
    {
        sifa= new SequenceItemAccessor(this,model,mainwindow);
        sifa->calculRect();
        sifa->setPos(rect.width()-(20 * fileAccessors),4);
        fileAccessors--;
    }
    if(model->hasPackage())
    {
        pfa= new PackageFileAccessor(this,model,mainwindow);
        pfa->calculRect();
        pfa->setPos(rect.width()-(20 * fileAccessors),4);
        fileAccessors--;
    }
    if(model->hasConfig())
    {
        cfa= new ConfigFileAccessor(this,model,mainwindow);
        cfa->calculRect();
        cfa->setPos(rect.width()-(20 * fileAccessors),4);
        fileAccessors--;
    }
    if(model->hasHeaderFile())
    {
        hfa= new HeaderFileAccessor(this,model,mainwindow);
        hfa->calculRect();
        hfa->setPos(rect.width()-(20 * fileAccessors),4);
        fileAccessors--;
    }

    return rect;
}

QList<UvmPortView*> UvmComponentView::getPortsViews()
{
    QList<UvmPortView*> viewPorts;
    foreach(UvmPort* port, model->getFakePorts())
    {
        if(port != NULL && !viewPorts.contains(getPortView(port)))
            viewPorts.append(getPortView(port));
    }
    foreach(UvmPort* port, model->getPorts())
    {
        if(port != NULL && !viewPorts.contains(getPortView(port)))
            viewPorts.append(getPortView(port));
    }
    foreach(UvmPort* port, model->getTlmPorts())
    {
        if(port != NULL && !viewPorts.contains(getPortView(port)))
            viewPorts.append(getPortView(port));
    }
    return viewPorts;
}

// Search the component view corresponding to a component
UvmComponentView* UvmComponentView::getComponentView(UvmComponent* comp)
{
    QList<UvmComponentView*> compViewList;
    UvmComponentView* curCompView;
    UvmComponentView* childView;

    compViewList.append(this);

    while (!compViewList.isEmpty())
    {
        // Get and remove the first component view
        curCompView = compViewList.first();
        compViewList.removeFirst();

        if(curCompView->getModel() == comp)
            return curCompView;

        // Append child if it's a UvmComponentView
        QList<QGraphicsItem*> ll = curCompView->childItems();
        for(int i = 0; i < ll.size(); i++) {
            if(dynamic_cast<UvmComponentView*>(ll.at(i)) != 0) {
                childView = (UvmComponentView*) ll.at(i);
                compViewList.append(childView);
            }
        }
    }

    return 0;
}


// Search the port view corresponding to a port
UvmPortView* UvmComponentView::getPortView(UvmPort* port)
{
    QList<UvmComponentView*> compViewList;
    UvmComponentView* curCompView;
    UvmComponentView* childCompView;
    UvmPortView* childPortView;

    compViewList.append(this);

    while (!compViewList.isEmpty())
    {
        // Get and remove the first component view
        curCompView = compViewList.first();
        compViewList.removeFirst();

        for(int i = 0; i < curCompView->childItems().size(); i++)
        {
            // Append child if it's a UvmComponentView
            if(dynamic_cast<UvmComponentView*>(curCompView->childItems().at(i)) != 0) {
                childCompView = (UvmComponentView*) curCompView->childItems().at(i);
                compViewList.append(childCompView);
            }
            // Check the model of the child if it's a UvmPortView
            if(dynamic_cast<UvmPortView*>(curCompView->childItems().at(i)) != 0) {
                childPortView = (UvmPortView*) curCompView->childItems().at(i);
                if(childPortView->getModel() == port)
                    return childPortView;
            }
        }
    }

    return 0;
}


void UvmComponentView::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    QGraphicsItem::mouseReleaseEvent(event);
    if (this->scene()->views().size()!=0)
    {
        GraphicalProjectView *view=dynamic_cast<GraphicalProjectView *>(this->scene()->views().at(0));
        if (view)
        {
            view->refreshPlacement();
            view->viewport()->update();
        }
    }
}


// When clicked on with the left click, open the corresponding file
void UvmComponentView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Open the file (if it exists)
    if (event->button() == Qt::LeftButton) {
        QString file = model->getAbsoluteBodyFileName();
        if (QFileInfo(file).exists())
            mainwindow->openFile(file);
        QGraphicsItem::mousePressEvent(event);
    }

    // Open the file (if it exists)
    else if (event->button() == Qt::RightButton) {
        //QMessageBox::information(0,QString("Subcomponents of %1").arg(model->getShortName()),model->printHtmlTree());
        QMenu* menu = new QMenu();
        QAction *a = new QAction(this);
        a->setText(tr("Add a file..."));
        CONNECT(a, SIGNAL(triggered()), this, SLOT(addOtherFile()));
        menu->addAction(a);
        menu->exec(QCursor::pos());
    }
    else
    {
        // pass on to base class
        QGraphicsObject::mousePressEvent(event);
    }
}

bool UvmComponentView::hasAccessors()
{
    return model->hasConfig() || model->hasHeaderFile() || model->hasPackage() || model->hasSequence() || model->hasSequenceItem() || !model->getOtherFiles().isEmpty();
}

void UvmComponentView::addOtherFile()
{
    QFileDialog fd;
    fd.setDirectory(model->getAbsoluteBodyFileName().left(model->getAbsoluteBodyFileName().lastIndexOf(QDir::separator())));

    QString fileName;
    fd.exec();
    if(fd.selectedFiles().empty())
        return;
    fileName = fd.selectedFiles().at(0);
    //qDebug() << "IN VIEW : " << fileName;
    model->addNewFile(fileName);
    mainwindow->fileAdded();

    //calculRect();

    GraphicalProjectView *view=dynamic_cast<GraphicalProjectView *>(this->scene()->views().at(0));
    if (view)
    {
        view->refreshPlacement();
        view->viewport()->update();
        view->refresh();
    }
}
