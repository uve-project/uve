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
#ifndef UVM_COMPONENT_VIEW_H
#define UVM_COMPONENT_VIEW_H

#include <QtGui>
#include <QDebug>
#include "../uvmmodel/uvmcomponent.h"
#include "../uvmmodel/uvmport.h"
#include "uvmportview.h"
#include "headerfileaccessor.h"
#include "configfileaccessor.h"
#include "packagefileaccessor.h"
#include "sequenceaccessor.h"
#include "sequenceitemaccessor.h"
#include "otherfilesaccessor.h"

#define GRID_X 5
#define GRID_Y 5

#define DEFAULT_HEIGHT 40
#define DEFAULT_WIDTH  80

#define INTER_Y  30
#define INTER_X  10

#define INTER_X_END 10
#define INTER_Y_END 10

#define INTER_NAME_Y 35
#define INTER_NAME_X 10

class UvmPortView;
class MainWindow;

class UvmComponentView: public QGraphicsObject
{
    Q_OBJECT

    public:
        UvmComponentView(UvmComponentView* parent, UvmComponent* model, MainWindow* mainwindow);

        virtual void calculRect() = 0;
        virtual void untangleConnections() =0;

        QRectF boundingRect() const;
        QRectF getObstacle(UvmComponentView* topView);
        QRectF getRoutingParent();
        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget *widget);

        void setModel(UvmComponent* model);
        UvmComponent* getModel();

        UvmComponentView* getComponentView(UvmComponent* comp);
        UvmPortView* getPortView(UvmPort* port);

        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

        bool hasAccessors();

    public slots:
        void applyModelChanges();
        void addOtherFile();

    private:
        virtual QString getTypeName();

    protected:
        QRectF placeAccessors(QRectF rect);
        QList<UvmPortView*> getPortsViews();
        virtual void placePorts()=0;
        QRectF rect;
        QRectF tmpRect;
        UvmComponent* model;
        MainWindow* mainwindow;
        HeaderFileAccessor* hfa;
        ConfigFileAccessor* cfa;
        PackageFileAccessor* pfa;
        SequenceAccessor* sfa;
        SequenceItemAccessor* sifa;
        OtherFilesAccessor* ofa;
        QColor baseColor, noFileColor;
};

#endif // UVM_COMPONENT_VIEW_H
