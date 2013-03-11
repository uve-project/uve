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
#ifndef UVMVISITOR_H
#define UVMVISITOR_H

class UvmDesign;
class UvmAgent;
class UvmDriver;
class UvmMonitor;
class UvmScoreboard;
class UvmSequence;
class UvmSequencer;
class UvmSequenceItem;
class UvmTestbench;
class UvmTestCase;
class UvmTop;
class UvmVerificationComponent;
class UvmVirtualSequencer;
class UvmCollector;
class UvmInterface;
class UvmConfig;
class UvmPackage;

class UvmVisitor
{
public:
    UvmVisitor();

    virtual void visit(UvmTop *comp)=0;
    virtual void visit(UvmTestCase *comp)=0;
    virtual void visit(UvmTestbench *comp)=0;
    virtual void visit(UvmScoreboard *comp)=0;

    virtual void visit(UvmDesign *comp)=0;

    virtual void visit(UvmVerificationComponent *comp)=0;
    virtual void visit(UvmVirtualSequencer *comp)=0;
    virtual void visit(UvmAgent *comp)=0;
    virtual void visit(UvmCollector *comp)=0;
    virtual void visit(UvmDriver *comp)=0;
    virtual void visit(UvmMonitor *comp)=0;
    virtual void visit(UvmSequencer *comp)=0;
    virtual void visit(UvmSequence *comp)=0;
    virtual void visit(UvmSequenceItem *comp)=0;
    virtual void visit(UvmPackage *comp)=0;

    virtual void visit(UvmConfig *comp)=0;
    virtual void visit(UvmInterface *comp)=0;

};


class UvmPassiveVisitor
{
public:
    UvmPassiveVisitor();

    virtual void visit(const UvmTop *comp)=0;
    virtual void visit(const UvmTestCase *comp)=0;
    virtual void visit(const UvmTestbench *comp)=0;
    virtual void visit(const UvmScoreboard *comp)=0;

    virtual void visit(const UvmDesign *comp)=0;

    virtual void visit(const UvmVerificationComponent *comp)=0;
    virtual void visit(const UvmVirtualSequencer *comp)=0;
    virtual void visit(const UvmAgent *comp)=0;
    virtual void visit(const UvmCollector *comp)=0;
    virtual void visit(const UvmDriver *comp)=0;
    virtual void visit(const UvmMonitor *comp)=0;
    virtual void visit(const UvmSequencer *comp)=0;
    virtual void visit(const UvmSequence *comp)=0;
    virtual void visit(const UvmSequenceItem *comp)=0;
    virtual void visit(const UvmPackage *comp)=0;

    virtual void visit(const UvmConfig *comp)=0;
    virtual void visit(const UvmInterface *comp)=0;

};

#endif // UVMVISITOR_H
