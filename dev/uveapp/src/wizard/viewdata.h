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
#ifndef VIEWDATA_H
#define VIEWDATA_H

#include <QString>
#include <QList>
#include <QStringList> //Elementary, my dear Watson

#include "uvmport.h"
#include "vhdlclasses.h"

// This file contains the view's models

class NamespaceViewData {

    QStringList _names;
public:

    //Check in the list
    bool isNameUsed(const QString &name) const
    {
        return _names.contains(name);
    }

    //Save it in the list
    void registerName(const QString &name)
    {
        _names.append(name);
    }

    //Remove from the list
    void unRegisterName(const QString &name)
    {
        _names.removeAll(name);
    }


};

class ProjectViewData: public NamespaceViewData {
public:
    QString dutEntityName;
    VHDLFileContent* dut;
    QString name;
    QString folder;
    QString simulator;
    QString templatePath;
    QStringList files;
    QList<PhysicalPort*> duvPorts;
};

struct TestCaseViewData {
    QString name;
};

struct AgentViewData {
    QString name;
    int nbAgents;
    bool monitor;
    bool monitorCoverage;
    bool monitorLink; //Sequencer to monitor link
    bool sequencer;
    bool sequencerCoverage;
    bool sequencerLink; //VC Virtual sequencer to sequencer link
    bool collector;
    bool driver;
    bool driver2monitorLink; // Driver to monitor link

    //Default values
    AgentViewData () :
        monitor(false),
        nbAgents(1),
        monitorCoverage(false),
        monitorLink(false),
        sequencer(false),
        sequencerCoverage(false),
        sequencerLink(false),
        collector(false),
        driver(false),
        driver2monitorLink(false){ }
};

class VCViewData: public NamespaceViewData {
public:
    QString name;
    QString file;
    QString description;
    bool virtualSequencer;
    bool virtualSequencerLink;
    bool scoreboard;
    int scoreboardComparatorType;
    bool busMonitor;
    QList<AgentViewData*> agents;
    QList<BaseConnection> connections;
    QStringList interConnects;

    //Default values
    VCViewData () :
        virtualSequencer(false),
        virtualSequencerLink(false),
        scoreboard(false),
        busMonitor(false),
        scoreboardComparatorType(0)
    { }
};

struct TestBenchViewData : public NamespaceViewData {
    QString name;
    bool virtualSequencer;
    bool scoreboard;
    int scoreboardComparatorType;
    QList<VCViewData*> vcs;

    //Default data
    TestBenchViewData() :
        virtualSequencer(false),
        scoreboard(true),
        scoreboardComparatorType(0)
    { }
};

struct SignalsViewData {
    QList<PhysicalPort*> ports;
};

#endif // VIEWDATA_H
