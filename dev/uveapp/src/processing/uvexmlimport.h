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
#ifndef UVEXMLIMPORT_H
#define UVEXMLIMPORT_H

#include <QDomDocument>

#include "uvmproject.h"
#include "uvesettings.h"

class UveXmlImport: public QObject
{
protected:
    QString errorString;
    QStringList faultyFiles;
    QDomDocument doc;

public:
    UveXmlImport();
    QString getErrorString() const;
    QStringList getFaultyFiles() const;

    /** Load from an XML file
      All the sub-VCs will be loaded as well.
      @return The error message if any, or an empty one if none
      */

    UvmProject *loadProject(const QString fileName);

    void getSubComponents(UvmComponent *comp,QDomElement *root);
    void getPorts(UvmComponent *comp,QDomElement *root);
    void getPhysicalPorts(UvmComponent *comp,QDomElement *root);

    /** Load a connection from a DOM element
      As the two ends of the connections might be created, only the string representation can be build.
      Each connection found is added to the list.
      */
    BaseConnection getConnection(QDomElement source);

    UvmConnection* getConnection(QDomElement source, UvmComponent *parent);

    /** Construct from a DOM element
      Each connections found is added to the connList list.
      */
    UvmPort* getPort(QDomElement source, UvmComponent *parent);
    UvmPort* getTlmPort(QDomElement source, UvmComponent *parent);
    UvmPort* getFakePort(QDomElement source, UvmComponent *parent);

    UvmTop* getTop(QDomElement source, UvmProject *project);
    UvmMonitor* getMonitor(QDomElement source, UvmComponent *parent);
    UvmSequencer* getSequencer(QDomElement source, UvmComponent *parent);
    UvmCollector* getCollector(QDomElement source, UvmComponent *parent);
    UvmDriver* getDriver(QDomElement source, UvmComponent *parent);
    UvmAgent* getAgent(QDomElement source, UvmComponent *parent);
    UvmInterface* getInterface(QDomElement source, UvmComponent *parent);
    UvmVerificationComponent* getVerificationComponent(QDomElement source, UvmComponent *parent);
    UvmVirtualSequencer* getVirtualSequencer(QDomElement source, UvmComponent *parent);
    UvmTestbench* getTestbench(QDomElement source, UvmComponent *parent);
    UvmTestCase* getTestCase(QDomElement source, UvmComponent *parent);
    UvmConfig* getConfig(QDomElement source, UvmComponent *parent);
    UvmScoreboard* getScoreboard(QDomElement source, UvmComponent *parent);
    UvmSequenceItem* getSequenceItem(QDomElement source, UvmComponent *parent);
    UvmSequence* getSequence(QDomElement source, UvmComponent *parent);
    UvmDesign* getDesign(QDomElement source, UvmComponent *parent);
    UvmPackage* getPackage(QDomElement source, UvmComponent *parent);
    UvmVerificationComponent* getVerificationComponentFromFile(QDomElement source, UvmComponent *parent);

    /** Create the element using this DOM element
      If the element contains elements for sub-component, they have to be created.
      The connList parameter is used to fetch all the connections described in the XML, for them to be created afterwhile.
      Each component should add its own connexions in it.
      By default, this function will load the name, related file, each port and each sub-component of this component.
      */
    void getComponent(UvmComponent *comp,QDomElement source, UvmComponent *parent,const QStringList &ignore=QStringList());

    /** Load the VC data from an XML file
      Sub-component are loaded as well in the same file
      @param fileName Name of the XML file
      @param parent Parent component of the VIP (usually a testbench)
      @param toRegenerate indicates if the Vip should be regenerated if asked by the user
      @return A pointer to the VIP, 0 if something went wrong
      */

    UvmVerificationComponent* loadVerificationComponent(const QString &fileName, UvmComponent *parent, bool toRegenerate);

    QString valueOf(const QString &tag, const QDomElement &root,const QString defaultValue="") const;
};

#endif // UVEXMLIMPORT_H
