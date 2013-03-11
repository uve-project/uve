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
#include "accessor.h"
#include "uvmcomponentview.h"
#include "mainwindow.h"

Accessor::Accessor(UvmComponentView *parent, UvmComponent *model, MainWindow *mainwindow):
    mainwindow(mainwindow),
    model(model)
{
    setParentItem(parent);
    bgColor = QColor(134,151,219);
    borderColor = Qt::black;
    selectedColor = QColor(100,255,100);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setZValue(1.0);

    QString toolTip;
    toolTip += "<table>";
    toolTip += "<tr><td>Class&nbsp;name:</td><td>"+model->getClassName()+"</td></tr>";
    if (model->hasHeaderFile())
        toolTip += "<tr><td>Header&nbsp;file:&nbsp;</td><td>"+model->getAbsoluteHeaderFileName()+"</td></tr>";
    toolTip += "<tr><td>Body&nbsp;file:&nbsp;</td><td>"+model->getAbsoluteBodyFileName()+"</td></tr>";
    foreach (QString fileName,model->getAbsoluteOtherFiles())
        toolTip += "<tr><td>Other&nbsp;file:&nbsp;</td><td>"+fileName+"</td></tr>";
    toolTip += "</table>";


    setToolTip(toolTip);
}

void Accessor::calculRect()
{
    rect = QRectF(0,0,16,16);
}

void Accessor::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    painter->setRenderHint(QPainter::Antialiasing);
    QBrush brush(bgColor);
    if(isSelected())
        brush.setColor(selectedColor);
    brush.setStyle(Qt::SolidPattern);
    painter->setBrush(brush);
    QPen oldPen = painter->pen();
    QPen pen(Qt::black);
    painter->setPen(pen);
    painter->drawEllipse(rect);
    QPen blackPen(Qt::black);
    QFont oldFont = painter->font();
    QFont font = painter->font();
    font.setBold(true);
    font.setPixelSize(11);
    painter->setFont(font);
    painter->setPen(blackPen);
    painter->drawText(rect,Qt::AlignCenter,label);
    painter->setPen(oldPen);
    painter->setFont(oldFont);
}

QRectF Accessor::boundingRect() const
{
    return rect;
}
