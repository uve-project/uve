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
#include "uvmpackage.h"
#include "uvesettings.h"

UvmPackage::UvmPackage(UvmComponent *parent) :
    UvmComponent(parent)
{
}

UvmPackage::UvmPackage(QString name, UvmComponent *parent) :
    UvmComponent(name, parent)
{
}

QString UvmPackage::getRelativeTemplateFileName() const
{
    return QString(UVESettings::generator_gvip_folder() + "gvip_pkg.sv");
}

ComponentType UvmPackage::getComponentType() const
{
    return PACKAGE;
}
