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
#ifndef NAMESETTER_H
#define NAMESETTER_H

#include "uvmvisitor.h"
#include "uvmproject.h"
#include "uvmtop.h"

class NameSetter : public UvmVisitor
{
public:
    NameSetter();

    bool setNames(UvmProject *project);

    virtual void visit(UvmTop *comp);
    virtual void visit(UvmTestCase *comp);
    virtual void visit(UvmTestbench *comp);
    virtual void visit(UvmScoreboard *comp);

    virtual void visit(UvmDesign *comp);

    virtual void visit(UvmVerificationComponent *comp);
    virtual void visit(UvmVirtualSequencer *comp);
    virtual void visit(UvmAgent *comp);
    virtual void visit(UvmCollector *comp);
    virtual void visit(UvmDriver *comp);
    virtual void visit(UvmMonitor *comp);
    virtual void visit(UvmSequencer *comp);
    virtual void visit(UvmSequence *comp);
    virtual void visit(UvmSequenceItem *comp);

    virtual void visit(UvmPackage *comp);
    virtual void visit(UvmConfig *comp);
    virtual void visit(UvmInterface *comp);


};


#endif // NAMESETTER_H
