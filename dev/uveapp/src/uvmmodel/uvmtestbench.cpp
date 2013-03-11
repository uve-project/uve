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
#include "uvmtestbench.h"

#include "uvesettings.h"

#include "uvmtop.h"

// Constructor of the testbench
UvmTestbench::UvmTestbench(UvmComponent *parent) : UvmComponent(parent)
{
}


// Constructor of the testbench
UvmTestbench::UvmTestbench(QString name, UvmComponent *parent) : UvmComponent(name, parent)
{
}

QString UvmTestbench::getRelativeTemplateFileName() const
{
    return QString(UVESettings::generator_gtb_folder() + "gtb_tb.sv");
}


// Add an virtual sequencer to the testbench
void UvmTestbench::addVirtualSequencer(UvmVirtualSequencer *virtualSequencer)
{
    if(!children.contains(virtualSequencer)) {
        children.append(virtualSequencer);
    }
}


// Remove an virtual sequencer to the testbench
void UvmTestbench::removeVirtualSequencer(UvmVirtualSequencer *virtualSequencer)
{
    int i;
    if(children.contains(virtualSequencer)) {
        i = children.indexOf(virtualSequencer);
        children.removeAt(i);
     }
}


// Get the list of *UvmVirtualSequencer contained in the testbench
QList<UvmVirtualSequencer*> UvmTestbench::getVirtualSequencers()
{
    QList<UvmVirtualSequencer*> virtualSequencers;
    UvmComponent* comp;
    for (int i=0; i < children.count(); i++) {
        comp = children.at(i);
        if(dynamic_cast<UvmVirtualSequencer*>(comp) != 0)
            virtualSequencers.append((UvmVirtualSequencer*)comp);
    }
    return virtualSequencers;
}


// Add an scoreboard to the testbench
void UvmTestbench::addScoreboard(UvmScoreboard *scoreboard)
{
    if(!children.contains(scoreboard)) {
        children.append(scoreboard);
    }
}


// Remove a virtual sequencer to the testbench
void UvmTestbench::removeScoreboard(UvmScoreboard *scoreboard)
{
    int i;
    if(children.contains(scoreboard)) {
        i = children.indexOf(scoreboard);
        children.removeAt(i);
     }
}


// Get the list of UvmScoreboard contained in the testbench
QList<UvmScoreboard*> UvmTestbench::getScoreboards() const
{
    QList<UvmScoreboard*> scoreboards;
    UvmComponent* comp;
    for (int i=0; i < children.count(); i++) {
        comp = children.at(i);
        if(dynamic_cast<UvmScoreboard*>(comp) != 0)
            scoreboards.append((UvmScoreboard*)comp);
    }
    return scoreboards;
}


// Add a verification component to the testbench
void UvmTestbench::addVerificationComponent(UvmVerificationComponent *verifcomp)
{
    if(!children.contains(verifcomp)) {
        children.append(verifcomp);
    }
}


// Remove a verification component to the testbench
void UvmTestbench::removeVerificationComponent(UvmVerificationComponent *verifcomp)
{
    int i;
    if(children.contains(verifcomp)) {
        i = children.indexOf(verifcomp);
        children.removeAt(i);
     }
}


// Get the list of UvmVerificationComponent contained in the testbench
QList<UvmVerificationComponent*> UvmTestbench::getVerificationComponents() const
{
    QList<UvmVerificationComponent*> verifcomps;
    UvmComponent* comp;
    for (int i=0; i < children.count(); i++) {
        comp = children.at(i);
        if(dynamic_cast<UvmVerificationComponent*>(comp) != 0)
            verifcomps.append((UvmVerificationComponent*)comp);
    }
    return verifcomps;
}


// Set the config of the testbench
void UvmTestbench::setConfig(UvmConfig *config)
{
    UvmComponent* comp = getConfig();
    int i;
    if(comp == 0) {
        children.append(config);
    }
    else {
        i = children.indexOf(comp);
        children.replace(i, config);
    }
}


// Get the config of the testbench
UvmConfig* UvmTestbench::getConfig()
{
    UvmComponent* comp;
    for (int i=0; i < children.count(); i++) {
        comp = children.at(i);
        if(dynamic_cast<UvmConfig*>(comp) != 0)
            return (UvmConfig*) comp;
    }
    return 0;
}

ComponentType UvmTestbench::getComponentType()
{
    return TESTBENCH;
}
