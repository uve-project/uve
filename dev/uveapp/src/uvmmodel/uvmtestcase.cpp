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
#include "uvmtestcase.h"

// Constructor of the test case
UvmTestCase::UvmTestCase(UvmComponent *parent) : UvmComponent(parent)
{
}


// Constructor of the test case
UvmTestCase::UvmTestCase(QString name, UvmComponent *parent) : UvmComponent(name, parent)
{
}

// Set the test of the test case
void UvmTestCase::setTestbench(UvmTestbench *testbench)
{
    UvmComponent* comp = getTestbench();
    int i;
    if(comp == 0) {
        children.append(testbench);
    }
    else {
        i = children.indexOf(comp);
        children.replace(i, testbench);
    }
}


// Get the test of the test case
UvmTestbench* UvmTestCase::getTestbench() const
{
    UvmComponent* comp;
    for (int i=0; i < children.count(); i++) {
        comp = children.at(i);
        if(dynamic_cast<UvmTestbench*>(comp) != 0)
            return (UvmTestbench*) comp;
    }
    return 0;
}


// Set the config of the testbench
void UvmTestCase::setConfig(UvmConfig *config)
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
UvmConfig* UvmTestCase::getConfig()
{
    UvmComponent* comp;
    for (int i=0; i < children.count(); i++) {
        comp = children.at(i);
        if(dynamic_cast<UvmConfig*>(comp) != 0)
            return (UvmConfig*) comp;
    }
    return 0;
}

#include "uvesettings.h"

QString UvmTestCase::getRelativeTemplateFileName() const
{
    return QString(UVESettings::generator_gtb_folder() + "gtb_test_lib.sv");
}

ComponentType UvmTestCase::getComponentType()
{
    return TESTCASE;
}
