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
#include "namesetter.h"

#include "uvmsequence.h"
#include "uvmpackage.h"

NameSetter::NameSetter()
{
}

bool NameSetter::setNames(UvmProject *project)
{
    project->getTop()->accept(this);
    return true;
}

void NameSetter::visit(UvmTop *comp){
    if (!comp->toGenerate())
        return;
    comp->setInstName("Top");
    comp->setClassName(comp->getShortName());
    comp->setBodyFileName(comp->getClassName().toLower()+".sv");
    comp->setRelativePath(comp->getShortName().toLower() + DIRSEPARATOR);
}

void NameSetter::visit(UvmDesign *comp){
    if (!comp->toGenerate())
        return;
    comp->setInstName(comp->getShortName());
    comp->setClassName(comp->getShortName());
    QFileInfo info(comp->getProject()->getProjectFileName());
    comp->setRelativePath(info.absoluteDir().relativeFilePath(comp->getAbsoluteBodyFileName()) + DIRSEPARATOR);
}

void NameSetter::visit(UvmVerificationComponent *comp){
    if (!comp->toGenerate())
    {
            comp->setRelativePath(comp->getShortName().toLower() + DIRSEPARATOR + comp->getRelativePath());
            comp->setVipRelativePath(comp->getShortName().toLower() + DIRSEPARATOR);
            comp->setInstName(comp->getShortName().toLower());
            comp->setClassName(comp->getShortName());
        return;
    }
    if (comp->getShortName().toLower()==comp->getShortName())
        comp->setInstName(comp->getShortName()+"_inst");
    else
        comp->setInstName(comp->getShortName().toLower());
    comp->setClassName(comp->getShortName());
    comp->setBodyFileName(comp->getClassName().toLower()+".sv");
    comp->setRelativePath(comp->getShortName().toLower() + DIRSEPARATOR);
    comp->setVipRelativePath(comp->getShortName().toLower() + DIRSEPARATOR);
}

void NameSetter::visit(UvmTestCase *comp){
    if (!comp->toGenerate())
        return;
    comp->setInstName(comp->getShortName());
    comp->setClassName(comp->getProject()->getTop()->getClassName()+"_"+comp->getShortName());
    comp->setBodyFileName(comp->getClassName().toLower()+".sv");
}

void NameSetter::visit(UvmTestbench *comp){
    if (!comp->toGenerate())
        return;
    comp->setInstName(comp->getShortName());
    comp->setClassName(comp->getProject()->getTop()->getClassName()+"_"+comp->getShortName());
    comp->setBodyFileName(comp->getClassName().toLower()+".sv");
}

void NameSetter::visit(UvmScoreboard *comp){
    if (!comp->toGenerate())
        return;
    if (comp->getParentVip())
    {
        comp->setInstName("scoreboard");
        comp->setClassName(comp->getParentVip()->getClassName()+"_"+comp->getShortName());
    }
    else
    {
        comp->setInstName("scoreboard");
        comp->setClassName(comp->getProject()->getTop()->getClassName()+"_"+comp->getShortName());
    }
    comp->setBodyFileName(comp->getClassName().toLower()+".sv");
}



void NameSetter::visit(UvmVirtualSequencer *comp){
    if (!comp->toGenerate())
        return;
    comp->setInstName(comp->getShortName());
    if (comp->getParentVip())
        comp->setClassName(comp->getParentVip()->getClassName()+"_"+comp->getShortName());
    else
        comp->setClassName(comp->getProject()->getTop()->getClassName()+"_"+comp->getShortName());
    comp->setBodyFileName(comp->getClassName().toLower()+".sv");
}

void NameSetter::visit(UvmAgent *comp){
    if (!comp->toGenerate())
        return;
    if (comp->getShortName().toLower()==comp->getShortName())
        comp->setInstName(comp->getShortName()+"_inst");
    else
        comp->setInstName(comp->getShortName().toLower());
    comp->setClassName(comp->getParentVip()->getClassName()+"_"+comp->getShortName());
    comp->setBodyFileName(comp->getClassName().toLower()+".sv");
    comp->setRelativePath(comp->getShortName() + DIRSEPARATOR);
}

void NameSetter::visit(UvmCollector *comp){
    if (!comp->toGenerate())
        return;
    comp->setInstName("collector");
    comp->setClassName(comp->getParent()->getClassName()+"_"+comp->getShortName());
    comp->setBodyFileName(comp->getClassName().toLower()+".sv");
}

void NameSetter::visit(UvmDriver *comp){
    if (!comp->toGenerate())
        return;
    comp->setInstName("driver");
    comp->setClassName(comp->getParent()->getClassName()+"_"+comp->getShortName());
    comp->setBodyFileName(comp->getClassName().toLower()+".sv");
}

void NameSetter::visit(UvmMonitor *comp){
    if (!comp->toGenerate())
        return;
    if (comp->getParent()==comp->getParentVip())
        comp->setInstName("bus_monitor");
    else
        comp->setInstName("monitor");
    comp->setClassName(comp->getParent()->getClassName()+"_"+comp->getShortName());
    comp->setBodyFileName(comp->getClassName().toLower()+".sv");
}

void NameSetter::visit(UvmSequencer *comp){
    if (!comp->toGenerate())
        return;
    comp->setInstName("sequencer");
    comp->setClassName(comp->getParent()->getClassName()+"_"+comp->getShortName());
    comp->setBodyFileName(comp->getClassName().toLower()+".sv");
}

void NameSetter::visit(UvmSequence *comp){
    if (!comp->toGenerate())
        return;
    comp->setInstName(comp->getShortName());
    comp->setClassName(comp->getParentVip()->getClassName()+"_"+comp->getShortName());
    comp->setBodyFileName(comp->getClassName().toLower()+".sv");
}

void NameSetter::visit(UvmSequenceItem *comp){
    if (!comp->toGenerate())
        return;
    comp->setInstName(comp->getShortName());
    comp->setClassName(comp->getParentVip()->getClassName()+"_"+comp->getShortName());
    comp->setBodyFileName(comp->getClassName().toLower()+".sv");
}

void NameSetter::visit(UvmPackage *comp){
    if (!comp->toGenerate())
        return;
    comp->setInstName(comp->getShortName());
    comp->setClassName(comp->getParent()->getClassName()+"_"+comp->getShortName());
    comp->setBodyFileName(comp->getClassName().toLower()+".sv");
}

void NameSetter::visit(UvmConfig *comp){
    if (!comp->toGenerate())
        return;
//    comp->setEntityName(comp->getParent()->getEntityName()+"_"+comp->getName());
    comp->setInstName(comp->getShortName().toLower());
    comp->setClassName(comp->getShortName());
//    comp->setBodyFileName(comp->getEntityName().toLower()+".sv");
}

void NameSetter::visit(UvmInterface *comp){
    if (!comp->toGenerate())
        return;
    if (comp->getParentVip())
    {
        if (comp->getParentVip()->toGenerate())
            comp->setClassName(comp->getParentVip()->getClassName()+"_"+comp->getShortName());
        else
            comp->setClassName(comp->getShortName());
        comp->setBodyFileName(comp->getClassName().toLower()+".sv");
        comp->setInstName(comp->getParentVip()->getClassName()+"_vif");
    }
    else
    {
        comp->setClassName(comp->getProject()->getTop()->getClassName()+"_"+comp->getShortName());
        comp->setBodyFileName(comp->getProject()->getTop()->getBodyFileName());
        comp->setInstName(comp->getClassName());
    }
}
