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
#ifndef XMLEXPORT_H
#define XMLEXPORT_H

#include "xmlvisitor.h"
#include "uvmproject.h"

class UveXmlExport : public XmlVisitor
{
protected:
    QString errorString;
    QDomDocument doc;

public:
    UveXmlExport();

    QString getErrorString() const;

    void saveElement(const QString &name, const QString &value, QDomElement *root);
    void savePorts(const UvmComponent *comp,QDomElement *root);
    void savePhysicalPorts(const PhysicalPortList *comp,QDomElement *root);
    void saveConnections(const UvmComponent *comp,QDomElement *root);

    static QString getTagName(const UvmComponent *comp);

    //For each component, save it
    void saveComponents(const UvmComponent *comp,QDomElement *root);

    /** Save to an XML file
      Will create or re-create the project XML save.
      Save all the contained VC in the same time.
      @return The error message if any, or an empty one if none
      */

    bool saveProject(const UvmProject *project);


    /** Save the VC data in an XML file
      Sub-component are saved as well in the same file
      @param nogen If true, the generated part is not bundled in the VIP.
      @return The error message if any, or an empty one if none
      */

    bool saveVip(const UvmVerificationComponent *comp,const QString fileName);

    void saveBasicComponent(const UvmComponent* comp, QDomElement root);

    /** Save the component in a DOM element
      Everything describing the component should be contained within.
      The element itself should have the component type as name.
      By default, this function will save the name, the file, each ports and each sub-component of this component.
      */

    virtual QDomElement saveVipData(const UvmVerificationComponent *comp);

    virtual QDomElement visit(const PhysicalPort *port);
    virtual QDomElement visit(const TlmPort *port);
    virtual QDomElement visit(const FakePort *port);

    virtual QDomElement visit(const UvmConnection *conn);

    virtual QDomElement visit(const UvmTop *comp);
    virtual QDomElement visit(const UvmTestCase *comp);
    virtual QDomElement visit(const UvmTestbench *comp);
    virtual QDomElement visit(const UvmScoreboard *comp);

    virtual QDomElement visit(const UvmDesign *comp);

    virtual QDomElement visit(const UvmVerificationComponent *comp);
    virtual QDomElement visit(const UvmVirtualSequencer *comp);
    virtual QDomElement visit(const UvmAgent *comp);
    virtual QDomElement visit(const UvmCollector *comp);
    virtual QDomElement visit(const UvmDriver *comp);
    virtual QDomElement visit(const UvmMonitor *comp);
    virtual QDomElement visit(const UvmSequencer *comp);
    virtual QDomElement visit(const UvmSequence *comp);
    virtual QDomElement visit(const UvmSequenceItem *comp);
    virtual QDomElement visit(const UvmPackage *comp);
    virtual QDomElement visit(const UvmConfig *comp);
    virtual QDomElement visit(const UvmInterface *comp);

};

#endif // XMLEXPORT_H
