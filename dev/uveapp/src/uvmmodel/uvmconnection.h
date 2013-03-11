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
#ifndef UVMCONNECTION_H
#define UVMCONNECTION_H

#include <QtGui>
#include <QDomElement>
#include <QDomDocument>

#include "uvmport.h"
#include "componenttype.h"


class UvmComponent;
class UvmPort;

/** This structure is used when loading from XML
  See the loadXML function in UvmProject for more info
  */
struct BaseConnection {
    QString src;
    QString dst;
    QString name;
};

class UvmConnection : public QObject
{
    public:
        UvmConnection(UvmPort *srcPort, UvmPort *dstPort, QString name,UvmComponent *parent);

        UvmPort* getSrcPort() const;
        void setSrcPort();

        UvmPort* getDstPort() const;
        void setDstPort();

        QString getName() const;
        void setName(QString name);

        //Same as the UvmComponent one
        QString printTree (int deepness) const;

        //Same as the UvmComponent one
        QString printHtmlTree (int deepness) const;

        UvmComponent *getParent() const;
        void setParent(UvmComponent *p);

        ComponentType getOtherConnectedHostType(UvmPort* whosAsking);

        UvmPort *getOtherPort(UvmPort* whosAsking) const;

    private:
        UvmPort* srcPort;
        UvmPort* dstPort;
        QString name;
        UvmComponent *parent;
};

#endif // UVMCONNECTION_H
