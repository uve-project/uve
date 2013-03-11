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
#include "uvmenv.h"

// Contructor of the environment
UvmEnv::UvmEnv(QString name, UvmComponent *parent) : UvmComponent(name, parent)
{
}


// Add an agent to the environment
void UvmEnv::addAgent(UvmAgent *agent)
{
    if(!children.contains(agent)) {
        children.append(agent);
    }
}


// Add an scoreboard to the environment
void UvmEnv::addScoreboard(UvmScoreboard *scoreboard)
{
    if(!children.contains(scoreboard)) {
        children.append(scoreboard);
    }
}


// Add an virtual sequencer to the environment
void UvmEnv::addSequencer(UvmVirtualSequencer *virtualSequencer)
{
    if(!children.contains(virtualSequencer)) {
        children.append(virtualSequencer);
    }
}


// Remove an virtual sequencer to the environment
void UvmEnv::removeAgent(UvmAgent *agent)
{
   int i;
   if(children.contains(agent)) {
       i = children.indexOf(agent);
       children.removeAt(i);
    }
}


// Remove an virtual sequencer to the environment
void UvmEnv::removeScoreboard(UvmScoreboard *scoreboard)
{
    int i;
    if(children.contains(scoreboard)) {
        i = children.indexOf(scoreboard);
        children.removeAt(i);
     }
}


// Remove an virtual sequencer to the environment
void UvmEnv::removeSequencer(UvmVirtualSequencer *virtualSequencer)
{
    int i;
    if(children.contains(virtualSequencer)) {
        i = children.indexOf(virtualSequencer);
        children.removeAt(i);
     }
}


// Get the list of *UvmScoreboard contained in the driver
QList<UvmScoreboard*> UvmEnv::getScoreboards()
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


// Get the list of *UvmAgent contained in the driver
QList<UvmAgent*> UvmEnv::getAgents()
{
    QList<UvmAgent*> agents;
    UvmComponent* comp;
    for (int i=0; i < children.count(); i++) {
        comp = children.at(i);
        if(dynamic_cast<UvmAgent*>(comp) != 0)
            agents.append((UvmAgent*)comp);
    }
    return agents;
}


// Get the list of *UvmVirtualSequencer contained in the driver
QList<UvmVirtualSequencer*> UvmEnv::getVirtualSequencers()
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
