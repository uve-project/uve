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
#include "uvmtop.h"

#include "uvesettings.h"

// Constructor of the top
UvmTop::UvmTop(QString name, UvmProject *project) :
    UvmComponent(),
    project(project)
{
    setShortName(name);
}

UvmTop::UvmTop(UvmProject *project) :
    UvmComponent(),
    project(project)
{

}

UvmTop::~UvmTop()
{
    project = 0;
}

// Get the relative path depending on the top of the project
QString UvmTop::getRelativePath() const
{
    return relativePath;
}



// Set the design of the top
void UvmTop::setDesign(UvmDesign *design)
{
    UvmComponent* comp = getDesign();
    int i;
    if(comp == 0) {
        children.append(design);
    } else {
        i = children.indexOf(comp);
        children.replace(i, design);
    }
}


// Get the design of the top
UvmDesign* UvmTop::getDesign() const
{
    UvmComponent* comp;
    for (int i=0; i < children.count(); i++) {
        comp = children.at(i);
        if(dynamic_cast<UvmDesign*>(comp) != 0)
            return (UvmDesign*) comp;
    }
    return 0;
}


// Set the test case of the top
void UvmTop::setTestCase(UvmTestCase *design)
{
    UvmComponent* comp = getTestCase();
    int i;
    if(comp == 0) {
        children.append(design);
    }
    else {
        i = children.indexOf(comp);
        children.replace(i, design);
    }
}


// Get the test case of the top
UvmTestCase* UvmTop::getTestCase() const
{
    UvmComponent* comp;
    for (int i=0; i < children.count(); i++) {
        comp = children.at(i);
        if(dynamic_cast<UvmTestCase*>(comp) != 0)
            return (UvmTestCase*) comp;
    }
    return 0;
}


// Set the interface of the testbench
void UvmTop::setInterface(UvmInterface * uInterface)
{
    UvmComponent* comp = getInterface();
    int i;
    if(comp == 0) {
        children.append(uInterface);
    } else {
        i = children.indexOf(comp);
        children.replace(i, uInterface);
    }
}


// Get the interface of the testbench
UvmInterface* UvmTop::getInterface() const
{
    UvmComponent* comp;
    for (int i=0; i < children.count(); i++) {
        comp = children.at(i);
        if(dynamic_cast<UvmInterface*>(comp) != 0)
            return (UvmInterface*) comp;
    }
    return 0;
}

const QList<UvmVerificationComponent*> UvmTop::getVips() const {
    // We may hav to loop over all testCases and testsBenches in the futur...
    return getTestCase()->getTestbench()->getVerificationComponents();
}

// Return our internal project object
UvmProject* UvmTop::getProject() const
{
    return project;
}

QString UvmTop::vcName() const
{
    return "";
}

NameSpaceRegister *UvmTop::nameRegister(void* /*caller*/) const
{
    return (NameSpaceRegister*) project;
}


QString UvmTop::getRelativeTemplateFileName() const
{
    return QString(UVESettings::generator_gtb_folder() + "gtb_tb_top.sv");
}

ComponentType UvmTop::getComponentType() const
{
    return TOP;
}
