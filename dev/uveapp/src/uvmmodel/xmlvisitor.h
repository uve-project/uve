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
#ifndef XMLVISITOR_H
#define XMLVISITOR_H

#include <QDomElement>


#define ABSTRACT_XMLVISITOR_ACCEPT \
    virtual QDomElement accept(XmlVisitor *visitor)=0;

#define XMLVISITOR_ACCEPT \
    virtual QDomElement accept(XmlVisitor *visitor){ \
        return visitor->visit(this); \
    }

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
class PhysicalPort;
class TlmPort;
class FakePort;

class XmlVisitor
{
public:
    XmlVisitor();

    virtual QDomElement visit(const UvmTop *comp)=0;
    virtual QDomElement visit(const UvmTestCase *comp)=0;
    virtual QDomElement visit(const UvmTestbench *comp)=0;
    virtual QDomElement visit(const UvmScoreboard *comp)=0;

    virtual QDomElement visit(const UvmDesign *comp)=0;

    virtual QDomElement visit(const UvmVerificationComponent *comp)=0;
    virtual QDomElement visit(const UvmVirtualSequencer *comp)=0;
    virtual QDomElement visit(const UvmAgent *comp)=0;
    virtual QDomElement visit(const UvmCollector *comp)=0;
    virtual QDomElement visit(const UvmDriver *comp)=0;
    virtual QDomElement visit(const UvmMonitor *comp)=0;
    virtual QDomElement visit(const UvmSequencer *comp)=0;
    virtual QDomElement visit(const UvmSequence *comp)=0;
    virtual QDomElement visit(const UvmSequenceItem *comp)=0;
    virtual QDomElement visit(const UvmPackage *comp)=0;

    virtual QDomElement visit(const UvmConfig *comp)=0;
    virtual QDomElement visit(const UvmInterface *comp)=0;


    virtual QDomElement visit(const PhysicalPort *port)=0;
    virtual QDomElement visit(const TlmPort *port)=0;
    virtual QDomElement visit(const FakePort *port)=0;

};

#endif // XMLVISITOR_H
