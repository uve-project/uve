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
#include <QtGui>
#include "uvmconnection.h"

#include "uvmproject.h"

// Constructor of the connection
UvmConnection::UvmConnection(UvmPort *srcPort, UvmPort *dstPort, QString name,UvmComponent *parent)
{
    this->srcPort = srcPort;
    this->dstPort = dstPort;
    this->srcPort->addConnection(this);
    this->dstPort->addConnection(this);
    parent->addConnection(this);
    this->setName(name);
}

UvmComponent *UvmConnection::getParent() const
{
    return parent;
}

void UvmConnection::setParent(UvmComponent *parent)
{
    this->parent = parent;
}


// Get the source ports of the connection
UvmPort* UvmConnection::getSrcPort() const
{
    return srcPort;
}


// Set the source ports of the connection
void UvmConnection::setSrcPort()
{
    this->srcPort = srcPort;
}


// Get the destination ports of the connection
UvmPort* UvmConnection::getDstPort() const
{
    return dstPort;
}


// Set the destination ports of the connection
void UvmConnection::setDstPort()
{
    this->dstPort = dstPort;
}


// Get the name of the connection
QString UvmConnection::getName() const
{
    return name;
}


// Set the name of the connection
void UvmConnection::setName(QString name)
{
    QString crtName = name;

    /*
    int i=0;

    //Check the name
    if (srcPort)
    {
        if (srcPort->getParent())
        {
            UvmComponent *c=srcPort->getParent();
            NameSpaceRegister *r=c->nameRegister(this);
            while (!r->registerName(crtName,this))
                crtName = QString(name +"_%1").arg(i++);
        }
    }
    */
    //Set it
    this->name = crtName;
}

//Print our name and the port we're connected with
QString UvmConnection::printTree(int deepness) const
{
    QString tree;

    //Space stuff
    for (int i=0; i<deepness; i++)
        tree.append(" ");

    //Our name
    tree.append("Connection::" + this->getName() + " ");

    //Line start
    tree.append(srcPort->getName());
    if (srcPort->getDirection() == UvmPort::PORT_OUT)
        tree.append(" <-");
    else
        tree.append(" --");

    //Line end
    if (dstPort->getDirection() == UvmPort::PORT_OUT)
        tree.append("- ");
    else
        tree.append("> ");
    tree.append(dstPort->getName() + "\n");

    return tree;
}


//Print our name and the port we're connected with
QString UvmConnection::printHtmlTree(int deepness) const
{
    QString tree;

    //Space stuff
    for (int i=0; i<deepness; i++)
        tree.append("&nbsp;");

    //Our name
    tree.append("Connection::" + this->getName() + " ");

    //Line start
    tree.append(srcPort->getName());
    if (srcPort->getDirection() == UvmPort::PORT_OUT)
        tree.append("&nbsp;<-");
    else
        tree.append("&nbsp;--");

    //Line end
    if (dstPort->getDirection() == UvmPort::PORT_OUT)
        tree.append("-&nbsp;");
    else
        tree.append(">&nbsp;");
    tree.append(dstPort->getName() + "<br>\n");

    return tree;
}

ComponentType UvmConnection::getOtherConnectedHostType(UvmPort *whosAsking)
{
    if(whosAsking == srcPort)
    {
        return dstPort->getHostType();
    }
    if(whosAsking == dstPort)
    {
        return srcPort->getHostType();
    }
    return UNKNOWN_TYPE;
}

UvmPort *UvmConnection::getOtherPort(UvmPort* whosAsking) const
{
    if (whosAsking == srcPort)
        return dstPort;
    if (whosAsking == dstPort)
        return srcPort;
    return 0;
}

