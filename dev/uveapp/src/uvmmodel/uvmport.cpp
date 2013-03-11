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
#include "uvmport.h"

#include "uvmproject.h"

UvmPort::UvmPort()
{

}

UvmPort::~UvmPort()
{

}



PhysicalPort *UvmPort::newPhysicalPort(PhysicalPortList *parent)
{
    return new PhysicalPort(parent);
}

TlmPort *UvmPort::newTlmPort(UvmComponent *parent)
{
    return new TlmPort(parent);
}

FakePort *UvmPort::newFakePort(UvmComponent *parent)
{
    return new FakePort(parent);
}


PhysicalPort *UvmPort::newPhysicalPort(const QString &name,
                                  PortMode mode,
                                  PortDirection direction,
                                  const QString &type,
                                  PhysicalPortList *parent)
{
    return new PhysicalPort(name,mode,direction,type,parent);
}

TlmPort *UvmPort::newTlmPort(const QString &name,
                           PortMode mode,
                           UvmComponent *parent)
{
    return new TlmPort(name,mode,parent);
}

FakePort *UvmPort::newFakePort(const QString &name,
                            PortMode mode,
                            PortDirection direction,
                            UvmComponent *parent)
{
    return new FakePort(name,mode,direction,parent);
}


// Set the name of the port
void UvmPort::setName(QString name)
{
    QString crtName = name;
/*
    int i=0;

    //Check the name
    if (getParent())
        while (!getParent()->nameRegister(this)->registerName(crtName,this))
            crtName = QString(name +"_%1").arg(i++);
*/
    //Set it
    this->name = crtName;
}


// Get the names of the port
QString UvmPort::getName() const
{
    return name;
}


// Set the mode of the port
void UvmPort::setMode(PortMode mode)
{
    this->mode = mode;
}


// Get the mode of the port
UvmPort::PortMode UvmPort::getMode() const
{
    return mode;
}


// Set the direction of the port
void UvmPort::setDirection(PortDirection direction)
{
    this->direction = direction;
}


// Get the direction of the port
UvmPort::PortDirection UvmPort::getDirection() const
{
    switch (mode)
    {
    case UVM_ANALYSIS_PORT:
    case UVM_BLOCKING_PUT_PORT:
    case UVM_BLOCKING_GET_IMP:
    case UVM_SEQ_ITEM_PULL_IMP:
    case UVM_BLOCKING_PEEK_IMP:
    case VIRT_SEQ_TO_SEQ_OUT: return OUT;


    case UVM_ANALYSIS_IMP:
    case UVM_BLOCKING_PUT_IMP:
    case UVM_BLOCKING_GET_PORT:
    case UVM_SEQ_ITEM_PULL_PORT:
    case UVM_BLOCKING_PEEK_PORT:
    case VIRT_SEQ_TO_SEQ_IN: return IN;
    default:
        return direction;
    }

}


// Add a connections
void UvmPort::addConnection(UvmConnection* connection)
{
    if(!uvmConnections.contains(connection))
        uvmConnections.append(connection);
}


// Remove a connections
void UvmPort::delConnection(UvmConnection* connection)
{
    uvmConnections.removeOne(connection);
}


// Get all connections
QList<UvmConnection *> UvmPort::getConnections() const
{
    return uvmConnections;
}


//Print our name and our connections
QString UvmPort::printTree(int deepness) const
{
    QString tree;

    //Space stuff
    for (int i=0; i<deepness; i++)
        tree.append(" ");

    deepness += 3;

    //Our class and name
    tree.append("Port::" + this->getName() + "\n");

    //Append our connections
    foreach (UvmConnection *c,uvmConnections)
        tree.append(c->printTree(deepness));

    return tree;
}


//Print our name and our connections
QString UvmPort::printHtmlTree(int deepness) const
{
    QString tree;

    //Space stuff
    for (int i=0; i<deepness; i++)
        tree.append("&nbsp;");

    deepness += 3;

    //Our class and name
    tree.append("Port::" + this->getName() + "<br>\n");

    //Append our connections
    foreach (UvmConnection *c,uvmConnections)
        tree.append(c->printHtmlTree(deepness));

    return tree;
}

QString UvmPort::modeToString(PortMode mode)
{
    switch (mode)
    {
    case DUT : return QString("dut");
    case DUT_INTERFACE: return QString("dut_interface");
    case VC_INTERFACE: return QString("vc_interface");
    case VCTODUT_INTERFACE: return QString("vctodut_interface");
    case GLOBAL: return QString("global");
    case UVM_ANALYSIS_PORT: return QString("uvm_analysis_port");
    case UVM_ANALYSIS_IMP: return QString("uvm_analysis_imp");
    case UVM_BLOCKING_PUT_PORT: return QString("uvm_blocking_put_port");
    case UVM_BLOCKING_PUT_IMP: return QString("uvm_blocking_put_imp");
    case UVM_BLOCKING_GET_PORT: return QString("uvm_blocking_get_port");
    case UVM_BLOCKING_GET_IMP: return QString("uvm_blocking_get_imp");
    case UVM_SEQ_ITEM_PULL_IMP: return QString("uvm_seq_item_pull_imp");
    case UVM_SEQ_ITEM_PULL_PORT: return QString("uvm_seq_item_pull_port");
    case UVM_BLOCKING_PEEK_PORT: return QString("uvm_blocking_peek_port");
    case UVM_BLOCKING_PEEK_IMP: return QString("uvm_blocking_peek_imp");
    case VIRT_SEQ_TO_SEQ_OUT: return QString("virt_seq_to_seq_out");
    case VIRT_SEQ_TO_SEQ_IN: return QString("virt_seq_to_seq_in");
    default: {
        qCritical() << "Error in mode of port";
        return "";
    }
    }
}

UvmPort::PortMode UvmPort::StringToMode(const QString &mode)
{
    if (mode == "dut")
        return DUT;
    if (mode == "dut_interface")
        return DUT_INTERFACE;
    if (mode == "vc_interface")
        return VC_INTERFACE;
    if (mode == "vctodut_interface")
        return VCTODUT_INTERFACE;
    if (mode == "global")
        return GLOBAL;
    if (mode == "uvm_analysis_port")
        return UVM_ANALYSIS_PORT;
    if (mode == "uvm_analysis_imp")
        return UVM_ANALYSIS_IMP;
    if (mode == "uvm_blocking_put_port")
        return UVM_BLOCKING_PUT_PORT;
    if (mode == "uvm_blocking_put_imp")
        return UVM_BLOCKING_PUT_IMP;
    if (mode == "uvm_blocking_get_port")
        return UVM_BLOCKING_GET_PORT;
    if (mode == "uvm_blocking_get_imp")
        return UVM_BLOCKING_GET_IMP;
    if (mode == "uvm_seq_item_pull_imp")
        return UVM_SEQ_ITEM_PULL_IMP;
    if (mode == "uvm_seq_item_pull_port")
        return UVM_SEQ_ITEM_PULL_PORT;
    if (mode == "uvm_blocking_peek_imp")
        return UVM_BLOCKING_PEEK_IMP;
    if (mode == "uvm_blocking_peek_port")
        return UVM_BLOCKING_PEEK_PORT;
    if (mode == "virt_seq_to_seq_out")
        return VIRT_SEQ_TO_SEQ_OUT;
    if (mode == "virt_seq_to_seq_in")
        return VIRT_SEQ_TO_SEQ_IN;
    qCritical() << "Port mode unknown: " << mode;
    return MODE_ERROR;
}

QString UvmPort::categoryToString(PortCategory category)
{
    switch (category)
    {
    case NONE: return QString("none");
    case CLOCK: return QString("clock");
    case RESET: return QString("reset");
    default: {
        qCritical() << "Error in category of port";
        return "";
    }
    }
}

UvmPort::PortCategory UvmPort::StringToCategory(const QString &category)
{
    if (category == "none")
        return NONE;
    if (category == "clock")
        return CLOCK;
    if (category == "reset")
        return RESET;
    qCritical() << "Port category unknown: " << category;
    return CATEGORY_ERROR;
}

QString UvmPort::DirectionToString(PortDirection direction)
{
    switch (direction)
    {
    case IN: return QString("in");
    case OUT: return QString("out");
    case INOUT: return QString("inout");
    default: {
        qCritical() << "Error in direction of port";
        return "";
    }
    }
}

UvmPort::PortDirection UvmPort::StringToDirection(const QString &direction)
{
    if (direction == "in")
        return IN;
    if (direction == "out")
        return OUT;
    if (direction == "inout")
        return INOUT;
    qCritical() << "Port direction unknown: " << direction;
    return IN;
}

bool UvmPort::isTlmPort() const
{
    switch (mode)
    {
    case UVM_ANALYSIS_PORT:
    case UVM_ANALYSIS_IMP:
    case UVM_BLOCKING_PUT_PORT:
    case UVM_BLOCKING_PUT_IMP:
    case UVM_BLOCKING_GET_PORT:
    case UVM_BLOCKING_GET_IMP:
    case UVM_SEQ_ITEM_PULL_IMP:
    case UVM_SEQ_ITEM_PULL_PORT:
    case UVM_BLOCKING_PEEK_IMP:
    case UVM_BLOCKING_PEEK_PORT:
    case VIRT_SEQ_TO_SEQ_IN:
    case VIRT_SEQ_TO_SEQ_OUT:
        return true;
    default:
        return false;
    }
}

bool UvmPort::isPhysicalPort() const
{
    if (dynamic_cast<const PhysicalPort *>(this))
        return true;
    else
        return false;
}

bool UvmPort::isFakePort() const
{
    if (dynamic_cast<const FakePort *>(this))
        return true;
    else
        return false;
}

ComponentType UvmPort::getHostType()
{
    if (getParent())
        return getParent()->getComponentType();
    return UNKNOWN_TYPE;
}

ComponentType UvmPort::getConnectedComponentType()
{
    if (uvmConnections.size()!=0)
        return uvmConnections.at(0)->getOtherConnectedHostType(this);
    return UNKNOWN_TYPE;
}

// Set the parent of the port
void NonPhysicalPort::setParent(UvmComponent *parent)
{
    this->parent = parent;
}
// Get the parent of the port
UvmComponent* NonPhysicalPort::getParent() const
{
    return parent;
}

// Destructor of the port
NonPhysicalPort::~NonPhysicalPort()
{
    UvmConnection* conn;

    //Remove ourselves from our parent
    parent->removePort(this);

    int nbrElement = uvmConnections.count();
    for(int i=0; i < nbrElement; i++ ){
        conn = uvmConnections.takeAt(0);
        if(getDirection() == UvmPort::IN)
            delete conn;
    }
}




TlmPort::TlmPort(const QString &name, PortMode mode, UvmComponent *parent) : NonPhysicalPort()
{
    this->name = name;
    this->mode = mode;
    this->parent = parent;
    if (parent)
        parent->addPort(this);
}

TlmPort::TlmPort(UvmComponent *parent)
{
    this->parent = parent;
    if (parent)
        parent->addPort(this);
}

FakePort::FakePort(const QString &name, PortMode mode, PortDirection direction, UvmComponent *parent) :NonPhysicalPort()
{
    this->name = name;
    this->mode = mode;
    this->direction = direction;
    this->parent = parent;
    if (parent)
        parent->addPort(this);

}


FakePort::FakePort(UvmComponent *parent)
{
    this->parent = parent;
    if (parent)
        parent->addPort(this);
}


PhysicalPort::PhysicalPort(const PhysicalPort *port,PhysicalPortList *parent)
{
    this->phyParent = parent;
    if (parent)
        parent->addPhysicalPort(this);
    this->name=port->name;
    this->type=port->type;
    this->mode=port->mode;
    this->direction=port->direction;
    this->category=port->category;
    this->resetActiveLow=port->resetActiveLow;
    this->resetDuration=port->resetDuration;
    this->clockFrequency=port->clockFrequency;
    this->size=port->size;
}


PhysicalPort::PhysicalPort(PhysicalPortList *parent) :
    phyParent(parent)
{
    if (parent)
        parent->addPhysicalPort(this);
}

PhysicalPort::PhysicalPort(const QString &name, PortMode mode, PortDirection direction, const QString &type, PhysicalPortList *parent) :
    category(NONE),
    resetActiveLow(false),
    resetDuration(10),
    clockFrequency(100000000),
    size(1),
    phyParent(parent)
{
    this->name = name;
    this->mode = mode;
    this->direction = direction;
    this->type = type;
    if (parent)
        parent->addPhysicalPort(this);
}



// Destructor of the port
PhysicalPort::~PhysicalPort()
{
    UvmConnection* conn;

    //Remove ourselves from our parent
    phyParent->removePhysicalPort(this);

    int nbrElement = uvmConnections.count();
    for(int i=0; i < nbrElement; i++ ){
        conn = uvmConnections.takeAt(0);
        if(getDirection() == UvmPort::IN)
            delete conn;
    }
}

// Get the parent of the port
UvmComponent* PhysicalPort::getParent() const
{
    return dynamic_cast<UvmComponent*>(phyParent);
}



// Set the type of the port
void PhysicalPort::setType(QString name)
{
    this->type = name;
}

// Get the type of the port
QString PhysicalPort::getType() const
{
    return type;
}


void PhysicalPort::setCategory(UvmPort::PortCategory category)
{
    this->category = category;
}


UvmPort::PortCategory PhysicalPort::getCategory() const
{
    return this->category;
}

void PhysicalPort::setClockFrequency(double freq)
{
    this->clockFrequency = freq;
}

double PhysicalPort::getClockFrequency() const
{
    return this->clockFrequency;
}


double PhysicalPort::getClockHalfPeriod() const
{
    double halfPeriod = 0;

    if (clockFrequency > 0)
        halfPeriod = 1 / clockFrequency;

    return halfPeriod / 2 * 1.E9;
}


void PhysicalPort::setResetActiveLow(bool activeLow)
{
    resetActiveLow = activeLow;
}

bool PhysicalPort::getResetActiveLow() const
{
    return resetActiveLow;
}

void PhysicalPort::setResetDuration(int ns)
{
    resetDuration = ns;
}

int PhysicalPort::getResetDuration() const
{
    return resetDuration;
}

void PhysicalPort::setSize(int size)
{
    this->size = size;
}

int PhysicalPort::getSize() const
{
    return size;
}

PhysicalPortList *PhysicalPort::getPhysicalParent() const
{
    return phyParent;
}








PhysicalPortList::PhysicalPortList()
{

}

void PhysicalPortList::removePhysicalPort(PhysicalPort *port)
{
    physicalPorts.removeAll(port);
}

void PhysicalPortList::addPhysicalPort(PhysicalPort *port)
{
    if(!physicalPorts.contains(port))
    physicalPorts.append(port);
//    if (dynamic_cast<UvmComponent *>(this))
//        port->setParent(dynamic_cast<UvmComponent *>(this);
}


QList<PhysicalPort*> PhysicalPortList::getPhysicalPorts() const
{
    return physicalPorts;
}


QList<PhysicalPort*> PhysicalPortList::getResets() const
{
    QList<PhysicalPort*> list;
    foreach(PhysicalPort *port,physicalPorts)
    {
        if (port->getCategory()==UvmPort::RESET)
            list.append(port);
    }
    return list;
}

QList<PhysicalPort*> PhysicalPortList::getClocks() const
{
    QList<PhysicalPort*> list;
    foreach(PhysicalPort *port,physicalPorts)
    {
        if (port->getCategory()==UvmPort::CLOCK)
            list.append(port);
    }
    return list;
}


PhysicalPort* PhysicalPortList::getPortByName(const QString &name) const
{
    foreach(PhysicalPort *port,physicalPorts)
        if (port->getName() == name)
            return port;
    return 0;
}
