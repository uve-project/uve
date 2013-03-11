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
#include "uvmtest.h"


// Contructor of the test
UvmTest::UvmTest(QString name, UvmComponent *parent) : UvmComponent(name, parent)
{
}


// Set the environment of the test
void UvmTest::setEnv(UvmEnv *env)
{
    UvmEnv* comp = getEnv();
    int i;
    if(comp == 0) {
        children.append(env);
    }
    else {
        i = children.indexOf(comp);
        children.replace(i, env);
    }
}


// Get the environment of the test
UvmEnv* UvmTest::getEnv()
{
    UvmComponent* comp;
    for (int i=0; i < children.count(); i++) {
        comp = children.at(i);
        if(dynamic_cast<UvmEnv*>(comp) != 0)
            return (UvmEnv*)comp;
    }
    return 0;
}
