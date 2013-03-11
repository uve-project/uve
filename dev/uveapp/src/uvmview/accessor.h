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
#ifndef ACCESSOR_H
#define ACCESSOR_H

#include <QGraphicsObject>
#include "../uvmmodel/uvmcomponent.h"

class UvmComponentView;
class MainWindow;
/**
class Accessor.
Abstract class ensuring all the accessors look and behave the same. Accessors are small buttons placed in a projectview element, allowing access to various files, other than the body file.
*/
class Accessor : public QGraphicsObject
{
    Q_OBJECT
public:
    Accessor(UvmComponentView* parent, UvmComponent* model, MainWindow* mainwindow);
    virtual void calculRect();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget *);
    QRectF boundingRect() const;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)=0;
signals:
    
public slots:

protected:
    QString label;
    QRectF rect;
    UvmComponent* model;
    MainWindow* mainwindow;
    QColor bgColor, borderColor, selectedColor;
};

#endif // ACCESSOR_H
