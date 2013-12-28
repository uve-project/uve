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
#include "uvmverificationcomponent.h"
#include "uvesettings.h"


#include <QDebug>

#include "uvmtestcase.h"
#include "uvmport.h"
#include "uvmpackage.h"
#include "current_version.h"


// Contructor of the verifcomp
UvmVerificationComponent::UvmVerificationComponent(UvmComponent *parent) : UvmComponent(parent)
{
    generated = true;
}


// Contructor of the verifcomp
UvmVerificationComponent::UvmVerificationComponent(QString name, UvmComponent *parent) : UvmComponent(name, parent)
{
    generated = true;
}

QString UvmVerificationComponent::getRelativeTemplateFileName() const
{
    return QString(UVESettings::generator_gvip_folder() + "gvip_env.sv");
}

QString UvmVerificationComponent::getConfigTemplateFileName() const
{
    return QString(UVESettings::generator_gvip_folder() + "gvip_config.sv");
}

QString UvmVerificationComponent::getPackageFileName() const
{
    UvmPackage *pkg=getPackage();
    if (pkg)
        return pkg->getBodyFileName();
    return "";

//    return getBodyFileName().replace(QRegExp("\\.sv"), "_pkg.sv");
}

// Get the relative path depending on the top of the project
QString UvmVerificationComponent::getRelativePath() const
{
    return relativePath;
}

QString UvmVerificationComponent::getVipRelativePath() const
{
    return vipRelativePath;
}

void UvmVerificationComponent::setVipRelativePath(const QString &path)
{
    vipRelativePath = path;
}

void UvmVerificationComponent::setDescription(const QString &desc)
{
    description = desc;
}

QString UvmVerificationComponent::getDescription() const
{
    return description;
}

// Add an agent to the verifcomp
void UvmVerificationComponent::addAgent(UvmAgent *agent)
{
    if(!children.contains(agent)) {
        children.append(agent);
    }
}


// Add an scoreboard to the verifcomp
void UvmVerificationComponent::addScoreboard(UvmScoreboard *scoreboard)
{
    if(!children.contains(scoreboard)) {
        children.append(scoreboard);
    }
}


// Add an virtual sequencer to the verifcomp
void UvmVerificationComponent::addVirtualSequencer(UvmVirtualSequencer *virtualSequencer)
{
    if(!children.contains(virtualSequencer)) {
        children.append(virtualSequencer);
    }
}


// Remove an virtual sequencer to the verifcomp
void UvmVerificationComponent::removeAgent(UvmAgent *agent)
{
    int i;
    if(children.contains(agent)) {
        i = children.indexOf(agent);
        children.removeAt(i);
    }
}


// Remove an virtual sequencer to the verifcomp
void UvmVerificationComponent::removeScoreboard(UvmScoreboard *scoreboard)
{
    int i;
    if(children.contains(scoreboard)) {
        i = children.indexOf(scoreboard);
        children.removeAt(i);
    }
}


// Remove an virtual sequencer to the verifcomp
void UvmVerificationComponent::removeVirtualSequencer(UvmVirtualSequencer *virtualSequencer)
{
    int i;
    if(children.contains(virtualSequencer)) {
        i = children.indexOf(virtualSequencer);
        children.removeAt(i);
    }
}


// Get the list of *UvmAgent contained in the verifcomp
QList<UvmAgent*> UvmVerificationComponent::getAgents() const
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


// Get the list of *UvmVirtualSequencer contained in the verifcomp
QList<UvmVirtualSequencer*> UvmVerificationComponent::getVirtualSequencers() const
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


// Get the list of *UvmScoreboard contained in the verifcomp
QList<UvmScoreboard*> UvmVerificationComponent::getScoreboards() const
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


// Set the monitor of the verifcomp
void UvmVerificationComponent::setBusMonitor(UvmMonitor *monitor)
{
    UvmMonitor* comp = getBusMonitor();
    int i;
    if(comp == 0) {
        children.append(monitor);
    }
    else {
        i = children.indexOf(comp);
        children.replace(i, monitor);
    }
}


// Get the monitor of the verifcomp
UvmMonitor* UvmVerificationComponent::getBusMonitor() const
{
    UvmComponent* comp;
    for (int i=0; i < children.count(); i++) {
        comp = children.at(i);
        if(dynamic_cast<UvmMonitor*>(comp) != 0)
            return (UvmMonitor*)comp;
    }
    return 0;
}


// Set the config of the agent
void UvmVerificationComponent::setConfig(UvmConfig *config)
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


// Get the config of the verifcomp
UvmConfig* UvmVerificationComponent::getConfig()
{
    UvmComponent* comp;
    for (int i=0; i < children.count(); i++) {
        comp = children.at(i);
        if(dynamic_cast<UvmConfig*>(comp) != 0)
            return (UvmConfig*) comp;
    }
    return 0;
}

// Set the interface of the verifcomp
void UvmVerificationComponent::setInterface(UvmInterface * interface)
{
    UvmComponent* comp = getInterface();
    int i;
    if(comp == 0) {
        children.append(interface);
    } else {
        i = children.indexOf(comp);
        children.replace(i, interface);
    }
}


// Get the interface of the verifcomp
UvmInterface* UvmVerificationComponent::getInterface() const
{
    UvmComponent* comp;
    for (int i=0; i < children.count(); i++) {
        comp = children.at(i);
        if(dynamic_cast<UvmInterface*>(comp) != 0)
            return (UvmInterface*) comp;
    }
    return 0;
}

void UvmVerificationComponent::setClockName(QString name) {
    clockName = name;
}

QString UvmVerificationComponent::getClockName() {
    //return clockName;
    return "clock";
}

void UvmVerificationComponent::setResetName(QString name) {
    resetName = name;
}

QString UvmVerificationComponent::getResetName() {
    //return resetName;
    return "reset";
}


// Add an agent to the verifcomp
UvmSequenceItem *UvmVerificationComponent::getSequenceItem() const
{
    UvmComponent *comp;
    for (int i = 0; i < children.count(); i++) {
        comp = children[i];
        if(dynamic_cast<UvmSequenceItem *>(comp) != NULL) {
            return (UvmSequenceItem *) comp;
        }
    }

    return NULL;
}


UvmTestCase *UvmVerificationComponent::getTestCase() const
{
    return (UvmTestCase *) getParent()->getParent();
}


QString UvmVerificationComponent::vcName() const
{
    return this->getShortName();
}

//If we call ourselves, return our parent's namespace
NameSpaceRegister *UvmVerificationComponent::nameRegister(void* caller) const
{
    if (caller == this)
        return parent->nameRegister(caller);
    else
        return (NameSpaceRegister*)this;
}

void UvmVerificationComponent::setVipFileName(const QString &fileName)
{
    vipFileName = fileName;
}

QString UvmVerificationComponent::getVipFileName() const
{
    if (vipFileName.isEmpty())
        return QString("%1.uvc").arg(this->getShortName());
    else
        return vipFileName;
}

QStringList UvmVerificationComponent::getAllVIPFilePaths()
{
    QStringList tmp = QStringList();
    foreach(UvmComponent* child, this->children)
    {
        tmp +=  child->getAllNonVIPFilePaths();
    }
    tmp.append(this->getAbsoluteBodyFileName());
    if (this->hasHeaderFile())
        tmp.append(this->getAbsoluteHeaderFileName());
    foreach (QString fileName,this->getAbsoluteOtherFiles())
        tmp.append(fileName);
    return tmp;
}

QStringList UvmVerificationComponent::getAllNonVIPFilePaths()
{
    QStringList tmp = QStringList();
    return tmp;
}


void UvmVerificationComponent::setOriginalVipPath(QString path)
{
    originalVipPath = path;
}

QString UvmVerificationComponent::getOriginalVipPath() const
{
    return originalVipPath;
}


void UvmVerificationComponent::setConfigFileName(const QString &fileName)
{
    configFileName = fileName;
}

QString UvmVerificationComponent::getConfigFileName() const
{
    return configFileName;
}

UvmPackage* UvmVerificationComponent::getPackage() const
{
    UvmPackage *pkg;
    foreach(UvmComponent *comp,getChildren())
    {
        pkg=dynamic_cast<UvmPackage*>(comp);
        if (pkg)
            return pkg;
    }
    return 0;
}

ComponentType UvmVerificationComponent::getComponentType() const
{
    return VERIFICATION_COMPONENT;
}

void UvmVerificationComponent::setGenerated(bool generated)
{
    this->generated = generated;
}

bool UvmVerificationComponent::getGenerated() const
{
    return generated;
}

