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
#ifndef UVMPORT_H
#define UVMPORT_H

#include "uvmcomponent.h"
#include "uvmconnection.h"
#include "xmlvisitor.h"

class UvmComponent;
class UvmConnection;

class PhysicalPort;
class TlmPort;
class FakePort;
class PhysicalPortList;

/**
  * This class represent an UVM port.
  */
class UvmPort : public QObject
{
    Q_OBJECT


    public:

    UvmPort();

    virtual ~UvmPort();

        enum PortMode {DUT,
                       DUT_INTERFACE,
                       VC_INTERFACE,
                       VCTODUT_INTERFACE,
                       GLOBAL,
                       UVM_ANALYSIS_PORT,
                       UVM_ANALYSIS_IMP,
                       UVM_BLOCKING_PUT_PORT,
                       UVM_BLOCKING_PUT_IMP,
                       UVM_BLOCKING_GET_PORT,
                       UVM_BLOCKING_GET_IMP,
                       UVM_SEQ_ITEM_PULL_IMP,
                       UVM_SEQ_ITEM_PULL_PORT,
                       UVM_BLOCKING_PEEK_PORT,
                       UVM_BLOCKING_PEEK_IMP,
                       VIRT_SEQ_TO_SEQ_OUT,
                       VIRT_SEQ_TO_SEQ_IN,
                       MODE_ERROR
                      };

        /**
          \enum PortMode::UVM_ANALISIS_PORT
          * port d'analyse
          */
        enum PortDirection {IN,
                            OUT,
                            INOUT,
                            DIRECTION_ERROR
                           };

        enum PortCategory {NONE,
                           CLOCK,
                           RESET,
                           CATEGORY_ERROR
                          };


        bool isTlmPort() const;
        bool isFakePort() const;
        bool isPhysicalPort() const;
        bool isInPort() const;
        bool isOutPort() const;
        bool isInOutPort() const;


        static QString modeToString(PortMode mode);
        static PortMode StringToMode(const QString &mode);

        static QString categoryToString(PortCategory category);
        static PortCategory StringToCategory(const QString &category);

        static QString DirectionToString(PortDirection direction);
        static PortDirection StringToDirection(const QString &direction);

        static PhysicalPort *newPhysicalPort(const QString &name,
                                        PortMode mode,
                                        PortDirection direction,
                                        const QString &type,
                                        PhysicalPortList *parent=0);

        static TlmPort *newTlmPort(const QString &name,
                                   PortMode mode,
                                   UvmComponent *parent=0);

        static FakePort *newFakePort(const QString &name,
                                    PortMode mode,
                                    PortDirection direction,
                                    UvmComponent *parent=0);

        static PhysicalPort *newPhysicalPort(PhysicalPortList *parent=0);

        static TlmPort *newTlmPort(UvmComponent *parent=0);

        static FakePort *newFakePort(UvmComponent *parent=0);


        void setName(QString name);
        QString getName() const;



        void setMode(PortMode mode);
        PortMode getMode() const;

        void setDirection(PortDirection direction);
        PortDirection getDirection() const;

        void addConnection(UvmConnection* connection);
        void delConnection(UvmConnection* connection);
        QList<UvmConnection*> getConnections() const;

        //Same as the UvmComponent one
        QString printTree (int deepness) const;

        //Same as the UvmComponent one
        QString printHtmlTree (int deepness) const;

        ComponentType getHostType();
        ComponentType getConnectedComponentType();

        virtual UvmComponent* getParent() const=0;

    protected:
        QList<UvmConnection*> uvmConnections;

        QString  name;
        PortMode mode;
        PortDirection direction;
};

class NonPhysicalPort : public UvmPort
{
public:
    ABSTRACT_XMLVISITOR_ACCEPT

    NonPhysicalPort(){};
    virtual ~NonPhysicalPort();


    virtual UvmComponent* getParent() const;
    void setParent(UvmComponent *parent);



protected:

        UvmComponent *parent;
};

class TlmPort : public NonPhysicalPort
{
public:
    XMLVISITOR_ACCEPT

    TlmPort(const QString &name, PortMode mode, UvmComponent *parent=0);

    TlmPort(UvmComponent *parent=0);

public:

};


class PhysicalPort : public UvmPort
{
public:

    XMLVISITOR_ACCEPT

    PhysicalPort(const QString &name, PortMode mode, PortDirection direction, const QString &type = "", PhysicalPortList *phyParent=0);

    PhysicalPort(PhysicalPortList *phyParent=0);
    PhysicalPort(const PhysicalPort *port,PhysicalPortList *phyParent);

    virtual ~PhysicalPort();

    void setType(QString name);
    QString getType() const;

    void setCategory(PortCategory category);
    PortCategory getCategory() const;

    PhysicalPortList *getPhysicalParent() const;

    virtual UvmComponent* getParent() const;

protected:
    QString type;
    PhysicalPortList *phyParent;
    PortCategory category;
    bool resetActiveLow;
    int resetDuration;
    double clockFrequency;
    int size;

public:

    //For vector signals
    void setSize(int size);
    int getSize() const;

    // In Hz
    void setClockFrequency (double freq);
    double getClockFrequency () const;
    // In ns
    double getClockHalfPeriod () const;

    void setResetActiveLow (bool activeLow);
    bool getResetActiveLow () const;

    // In ns
    void setResetDuration (int ns);
    int getResetDuration () const;

};

class FakePort : public NonPhysicalPort
{
public:

    XMLVISITOR_ACCEPT

    FakePort(const QString &name, PortMode mode, PortDirection direction, UvmComponent *parent=0);

    FakePort(UvmComponent *parent=0);

public:

};


class PhysicalPortList
{
public:
    PhysicalPortList();

    void addPhysicalPort(PhysicalPort *port);
    void removePhysicalPort(PhysicalPort *port);
    QList<PhysicalPort *> getPhysicalPorts() const;
    PhysicalPort* getPortByName(const QString &name) const;

    virtual QList<PhysicalPort*> getResets() const;
    virtual QList<PhysicalPort*> getClocks() const;

protected:

    QList<PhysicalPort *> physicalPorts;

};

/*! \enum UvmPort::PortMode
 * The mode describes really what the port is.
 *
 */

/*! \var UvmPort::PortMode UvmPort::UVM_ANALYSIS_PORT
 * Direction: Output<br>
 * Usage: In a monitor (UvmMonitor), to be connected to a scoreboard (UvmScoreboard).<br>
 * Usage: In a collector (UvmCollector), to be connected to a monitor (UvmMonitor).<br>
 * Graphical representation: Square
 */

/*! \var UvmPort::PortMode UvmPort::UVM_ANALYSIS_IMP
 * Direction: Input<br>
 * Usage: In a scoreboard (UvmScoreboard), to be connected to a monitor (UvmMonitor).<br>
 * Usage: In a monitor (UvmMonitor), to be connected to a collector (UvmCollector).<br>
 * Graphical representation: Round
 */

/*! \var UvmPort::PortMode UvmPort::UVM_BLOCKING_PUT_IMP
 * Direction: Input<br>
 * Usage: In a monitor (UvmMonitor), to be connected to a driver (UvmDriver).<br>
 * More generally corresponds to a passive consumer.<br>
 * Graphical representation: Round
 */

/*! \var UvmPort::PortMode UvmPort::UVM_BLOCKING_PUT_PORT
 * Direction: Output<br>
 * Usage: In a driver (UvmDriver), to be connected to a monitor (UvmMonitor).<br>
 * More generraly corresponds to an active producer.<br>
 * Graphical representation: Square
 */

/*! \var UvmPort::PortMode UvmPort::UVM_BLOCKING_GET_IMP
 * Direction: Output<br>
 * Usage: Corresponds to a passive producer.<br>
 * Graphical representation: Round
 */

/*! \var UvmPort::PortMode UvmPort::UVM_BLOCKING_GET_PORT
 * Direction: Input<br>
 * Usage: Corresponds to an active consumer.<br>
 * Graphical representation: Square
 */

/*! \var UvmPort::PortMode UvmPort::UVM_SEQ_ITEM_PULL_IMP
 * Direction: Output<br>
 * Usage: In a sequencer (UvmSequencer), to be connected to a driver (UvmDriver).<br>
 * Graphical representation: Round
 */

/*! \var UvmPort::PortMode UvmPort::UVM_SEQ_ITEM_PULL_PORT
 * Direction: Input<br>
 * Usage: In a driver (UvmDriver), to be connected to a sequencer (UvmSequencer).<br>
 * Graphical representation: Square
 */

/*! \var UvmPort::PortMode UvmPort::VIRT_SEQ_TO_SEQ_OUT
 * Direction: Output<br>
 * Usage: In a virtual sequencer (UvmVirtualSequencer), to be connected to a sequencer (UvmSequencer).<br>
 * Graphical representation: Nothing
 */

/*! \var UvmPort::PortMode UvmPort::VIRT_SEQ_TO_SEQ_IN
 * Direction: Input<br>
 * Usage: In a sequencer (UvmSequencer), to be connected to a virtual sequencer (UvmVirtualSequencer).<br>
 * Graphical representation: A standard arrow (directed to the sequencer)
 */

/*! \var UvmPort::PortMode UvmPort::UVM_BLOCKING_PEEK_PORT
 * Direction: Input<br>
 * Usage: In a sequencer (UvmSequencer), to be connected to a monitor (UvmMonitor).<br>
 * Graphical representation: Square
 */

/*! \var UvmPort::PortMode UvmPort::UVM_BLOCKING_PEEK_IMP
 * Direction: Output<br>
 * Usage: In a monitor (UvmMonitor), to be connected to a sequencer (UvmSequencer).<br>
 * Graphical representation: Round
 */

/*! \var UvmPort::PortMode UvmPort::DUT_INTERFACE
 * Direction: In or out<br>
 * Graphical representation: Inside square
 */

/*! \var UvmPort::PortMode UvmPort::VC_INTERFACE
 * Direction: In or out<br>
 * Graphical representation: Inside square
 */

/*! \var UvmPort::PortMode UvmPort::VCTODUT_INTERFACE
 * Direction: In or out<br>
 * Graphical representation: Inside square
 */

/*! \var UvmPort::PortMode UvmPort::GLOBAL
 * Direction: In or out<br>
 * Graphical representation: At the moment, do not draw anything about the port and the connection to it.
 */

/*! \var UvmPort::PortMode UvmPort::DUT
 * Direction: In or out<br>
 * Graphical representation: Do not change anything for this.
 */

#endif // UVMPORT_H
