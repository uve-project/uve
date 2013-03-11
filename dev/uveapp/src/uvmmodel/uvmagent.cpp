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
#include "uvmagent.h"
#include "uvesettings.h"

UvmAgent::UvmAgent(UvmComponent *parent) :
    UvmComponent(parent)
{

}

// Constructor of the agent
UvmAgent::UvmAgent(QString name,  UvmComponent *parent) :
    UvmComponent(name, parent),
    type(ACTIV),
    nbAgents(1)
{
}

void UvmAgent::setNbAgents(int number)
{
    this->nbAgents = number;
}

int UvmAgent::getNbAgents() const
{
    return nbAgents;
}


QString UvmAgent::getRelativeTemplateFileName() const
{
    return QString(UVESettings::generator_gvip_folder() + "gvip_agent.sv");
}


// Set the driver of the agent
void UvmAgent::setDriver(UvmDriver *driver)
{
    UvmComponent* comp = getDriver();
    int i;
    if(comp == 0) {
        children.append(driver);
    }
    else {
        i = children.indexOf(comp);
        children.replace(i, driver);
    }
}


// Set the monitor of the agent
void UvmAgent::setMonitor(UvmMonitor *monitor)
{
    UvmComponent* comp = getMonitor();
    int i;
    if(comp == 0) {
        children.append(monitor);
    }
    else {
        i = children.indexOf(comp);
        children.replace(i, monitor);
    }
}


// Set the collector of the agent
void UvmAgent::setCollector(UvmCollector *collector)
{
    UvmComponent* comp = getCollector();
    int i;
    if(comp == 0) {
        children.append(collector);
    }
    else {
        i = children.indexOf(comp);
        children.replace(i, collector);
    }
}


// Set the sequencer of the agent
void UvmAgent::setSequencer(UvmSequencer *sequencer)
{
    UvmComponent* comp = getSequencer();
    int i;
    if(comp == 0) {
        children.append(sequencer);
    }
    else {
        i = children.indexOf(comp);
        children.replace(i, sequencer);
    }
}


// Set the config of the agent
void UvmAgent::setConfig(UvmConfig *config)
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


// Get the driver of the agent
UvmDriver* UvmAgent::getDriver() const
{
    UvmComponent* comp;
    for (int i=0; i < children.count(); i++) {
        comp = children.at(i);
        if(dynamic_cast<UvmDriver*>(comp) != 0)
            return (UvmDriver*) comp;
    }
    return 0;
}


// Get the monitor of the agent
UvmMonitor* UvmAgent::getMonitor() const
{
    UvmComponent* comp;
    for (int i=0; i < children.count(); i++) {
        comp = children.at(i);
        if(dynamic_cast<UvmMonitor*>(comp) != 0)
            return (UvmMonitor*)comp;
    }
    return 0;
}


// Get the collector of the agent
UvmCollector* UvmAgent::getCollector() const
{
    UvmComponent* comp;
    for (int i=0; i < children.count(); i++) {
        comp = children.at(i);
        if(dynamic_cast<UvmCollector*>(comp) != 0)
            return (UvmCollector*) comp;
    }
    return 0;
}


// Get the sequencer of the agent
UvmSequencer* UvmAgent::getSequencer() const
{
    UvmComponent* comp;
    for (int i=0; i < children.count(); i++) {
        comp = children.at(i);
        if(dynamic_cast<UvmSequencer*>(comp) != 0)
            return (UvmSequencer*) comp;
    }
    return 0;
}


// Get the config of the agent
UvmConfig* UvmAgent::getConfig() const
{
    UvmComponent* comp;
    for (int i=0; i < children.count(); i++) {
        comp = children.at(i);
        if(dynamic_cast<UvmConfig*>(comp) != 0)
            return (UvmConfig*) comp;
    }
    return 0;
}


// Set the type of the agent
void UvmAgent::setType(UvmAgent::AgentType type)
{
    this->type = type;
}


// Get the type of the agent
UvmAgent::AgentType UvmAgent::getType() const
{
    return type;
}

ComponentType UvmAgent::getComponentType()
{
    return AGENT;
}
