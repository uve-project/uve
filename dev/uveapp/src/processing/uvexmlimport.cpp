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
#include <QXmlSchemaValidator>
#include <QXmlSchema>

#include "uvexmlimport.h"
#include "uvmproject.h"
#include "current_version.h"

#include "uvmtop.h"
#include "uvmsequence.h"
#include "uvmpackage.h"
#include "namesetter.h"

UveXmlImport::UveXmlImport()
{
    errorString="";
}

QString UveXmlImport::getErrorString() const
{
    return errorString;
}

QStringList UveXmlImport::getFaultyFiles() const
{
    return faultyFiles;
}

UvmProject* UveXmlImport::loadProject(const QString fileName)
{
    QString xsdFileName = UVESettings::getInstance()->value("UVEXSD").toString();

    QUrl schemaUrl=QUrl::fromLocalFile(xsdFileName);


    QXmlSchema schema;
    schema.load(schemaUrl);

    if (schema.isValid()) {

        QXmlSchemaValidator validator(schema);
        if (!validator.validate(QUrl::fromLocalFile(fileName)))
        {
            qDebug() << "instance document is invalid";
            errorString += "The file " + fileName + " does not contain a valid .uve structure.";
            faultyFiles.append(fileName);
            return 0;
        }

    }
    else
    {
        qDebug() << "The file uve.xsd is not a valid xsd file";
    }



    UvmProject *project= new UvmProject();

    // Fetch the file name
    project->setProjectFileName(QFileInfo(fileName).fileName());
    //Fetch the folder from the file path
    project->setFolder(QFileInfo(fileName).path() + "/");

    //Load the file
    doc=QDomDocument("VC");
    QFile f(fileName);
    if (!f.open(QIODevice::ReadOnly)) {
        errorString += QObject::tr("Could not open the project file %1\n").arg(fileName);
        delete project;
        return 0;
    } if (!doc.setContent(&f)) {
        f.close();
        errorString += QObject::tr("Could not read the project XML %1\n").arg(fileName);
        delete project;
        return 0;
    }
    f.close();

    QList<QString> vcs;
    QList<BaseConnection> connList;

    //Verify the XML version
    QString version = doc.documentElement().attribute("version","0.0");
    if ( version != XML_PROJECT_VERSION ) {
        errorString += QObject::tr("The given XML has an invalid version (expected %1, got %2)\n").arg(XML_PROJECT_VERSION, version);
        delete project;
        return 0;
    }

    //Go through the sub-elements
    QDomNode n = doc.documentElement().firstChild();
    while(!n.isNull()) {
        //Should be an element
        QDomElement crt = n.toElement();

        //Name
        if (crt.tagName() == "name")
            project->setName( crt.firstChild().toText().data() );
        //Simulator
        else if (crt.tagName() == "simulator")
            project->setSimulator( crt.firstChild().toText().data() );
        //DUT
        else if (crt.tagName() == "dut")
            project->setDut( crt.firstChild().toText().data() );
        //Templates
        else if (crt.tagName() == "templates")
            project->setTemplatePath(crt.firstChild().toText().data());

        //VHDL files
        else if (crt.tagName() == "vhdl") {
            QDomNode nn = crt.firstChild();
            while(!nn.isNull()) {
                project->addVHDLFile(nn.toElement().firstChild().toText().data());
                nn = nn.nextSibling();
            }
        }

        //Top
        else if (crt.tagName() == "top")
            project->setTop(getTop(crt,project));

        //What the hell?
        else
            qDebug()<< QObject::tr("Unknown tag : ") << crt.tagName() << ". In file " << __FILE__ << " at line " << __LINE__;

        n = n.nextSibling();
    }




    // Ugly, but necessary to display the interface-interface connection
    foreach (UvmVerificationComponent *uvc,project->getTop()->getVips())
    {
        //Visual signal between the VC interface and the interface
        new UvmConnection ( UvmPort::newFakePort(uvc->getInterface()->getInstName()+"_interinterport", UvmPort::VCTODUT_INTERFACE, UvmPort::INOUT, uvc->getInterface()),
                            UvmPort::newFakePort(project->getTop()->getInterface()->getInstName()+"_interinterport", UvmPort::VC_INTERFACE, UvmPort::INOUT, project->getTop()->getInterface()),
                            (uvc->getInterface()->getInstName() + "To" + project->getTop()->getInterface()->getInstName()),
                            project->getTop());
    }



//    project->printTree();

    if (!errorString.isEmpty())
    {
        delete project;
        return 0;
    }

    return project;
}


UvmTop* UveXmlImport::getTop(QDomElement source, UvmProject *project)
{
    UvmTop *top=new UvmTop(project);
    project->setTop(top);
    //Go through the sub-elements
    QDomNode n = source.firstChild();
    while(!n.isNull()) {
        //Should be an element
        QDomElement crt = n.toElement();

        //Name
        if (crt.tagName() == "shortName") {
            top->setShortName(crt.firstChild().toText().data());
            top->setClassName(top->getShortName());
            top->setRelativePath(top->getShortName().toLower()+"/");
        }
        //The file is not loaded right now, as there is no way to set it


        //Class name
        else if (crt.tagName() == "className")
            top->setClassName(crt.firstChild().toText().data());
        //Instname
        else if (crt.tagName() == "instName")
            top->setInstName(crt.firstChild().toText().data());
        //body File
        else if (crt.tagName() == "bodyFile")
            top->setBodyFileName(crt.firstChild().toText().data());
        //header File
        else if (crt.tagName() == "headerFile")
            top->setHeaderFileName(crt.firstChild().toText().data());
        //other files
        else if (crt.tagName() == "otherFile")
            top->addFile(crt.firstChild().toText().data());

        //Sub-Components
        else if (crt.tagName() == "components")
            getSubComponents(top,&crt);

        //Ports
        else if (crt.tagName() == "tlmports")
            getPorts(top,&crt);
        //Ports
        else if (crt.tagName() == "fakeports")
            getPorts(top,&crt);

        //Connections
        else if (crt.tagName() == "connections") {
            QDomNode nn = crt.firstChild();
            while (!nn.isNull()) {
                getConnection(nn.toElement(),top);
                nn = nn.nextSibling();
            }
        }

        //What the hell?
        else
            qDebug()<< tr("Unknown tag : ") << crt.tagName() << ". In file " << __FILE__ << " at line " << __LINE__;

        n = n.nextSibling();
    }
    return top;
}


//Load all the ports inside ourselves
void UveXmlImport::getPorts(UvmComponent *comp,QDomElement *root)
{
    //Basic check
    if ((root->tagName() != "tlmports")&&(root->tagName() != "fakeports")) {
        qDebug() << tr("Invalid root given for ports loading (%1)").arg(root->tagName());
        return;
    }

    //For each element
    QDomNode n = root->firstChild();
     while (!n.isNull()) {
         if (n.toElement().tagName() == "port")
             getPort(n.toElement(),comp);
         else if (n.toElement().tagName() == "tlmport")
             getTlmPort(n.toElement(),comp);
         else if (n.toElement().tagName() == "fakeport")
             getFakePort(n.toElement(),comp);
         n = n.nextSibling();
     }
}


//Load all the ports inside ourselves
void UveXmlImport::getPhysicalPorts(UvmComponent *comp,QDomElement *root)
{
    //Basic check
    if (root->tagName() != "physicalports") {
        qDebug() << tr("Invalid root given for ports loading (%1)").arg(root->tagName());
        return;
    }

    //For each element
    QDomNode n = root->firstChild();
     while (!n.isNull()) {
         if (n.toElement().tagName() == "port")
             getPort(n.toElement(),comp);
         n = n.nextSibling();
     }
}

UvmPort* UveXmlImport::getTlmPort(QDomElement source, UvmComponent *parent)
{
    NonPhysicalPort *port=UvmPort::newTlmPort(parent);

    //Go through the sub-elements
    QDomNode n = source.firstChild();
    while(!n.isNull()) {
        //Should be an element
        QDomElement crt = n.toElement();
        QString tag=crt.tagName();

        //Name
        if (tag == "name")
            port->setName(crt.firstChild().toText().data());
        //ClassName
        else if (tag == "className")
        {
            UvmPort::PortMode mode = UvmPort::StringToMode(crt.firstChild().toText().data());
            if (mode == UvmPort::MODE_ERROR)
            {
                errorString += QString("Mode of port unknown: %1").arg(crt.firstChild().toText().data());
                return 0;
            }
            port->setMode(mode);
        }

        //What the hell?
        else
        qDebug()<< tr("Unknown tag : ") << crt.tagName() << ". In file " << __FILE__ << " at line " << __LINE__;

        n = n.nextSibling();
    }

    //Add ourselves to our parent
    if (parent)
        parent->addPort(port);

    return port;

}


UvmPort* UveXmlImport::getFakePort(QDomElement source, UvmComponent *parent)
{
    FakePort *port=UvmPort::newFakePort(parent);

    //Go through the sub-elements
    QDomNode n = source.firstChild();
    while(!n.isNull()) {
        //Should be an element
        QDomElement crt = n.toElement();

        //Name
        if (crt.tagName() == "name")
            port->setName(crt.firstChild().toText().data());
        //Mode
        else if (crt.tagName() == "className")
        {
            UvmPort::PortMode mode = UvmPort::StringToMode(crt.firstChild().toText().data());
            if (mode == UvmPort::MODE_ERROR)
            {
                errorString += QString("Mode of port unknown: %1").arg(crt.firstChild().toText().data());
                return 0;
            }
            port->setMode(mode);
        }
        //Direction
        else if (crt.tagName() == "direction")
        {
            UvmPort::PortDirection direction = UvmPort::StringToDirection(crt.firstChild().toText().data());
            if (direction == UvmPort::DIRECTION_ERROR)
            {
                errorString += QString("Direction of port unknown: %1").arg(crt.firstChild().toText().data());
                return 0;
            }
            port->setDirection(direction);
        }

        //Port specific data


        //What the hell?
        else
        qDebug()<< tr("Unknown tag : ") << crt.tagName() << ". In file " << __FILE__ << " at line " << __LINE__;

        n = n.nextSibling();
    }

    //Add ourselves to our parent
    if (parent)
        parent->addPort(port);

    return port;
}

UvmPort* UveXmlImport::getPort(QDomElement source, UvmComponent *parent)
{
    PhysicalPortList *p=dynamic_cast<PhysicalPortList *>(parent);
    if (!p)
        qCritical() << "Aie in file " << __FILE__ << " line " << __LINE__;

    PhysicalPort *port=UvmPort::newPhysicalPort(p);

    //Go through the sub-elements
    QDomNode n = source.firstChild();
    while(!n.isNull()) {
        //Should be an element
        QDomElement crt = n.toElement();

        //Name
        if (crt.tagName() == "name")
            port->setName(crt.firstChild().toText().data());
        //Type
        else if (crt.tagName() == "type")
            port->setType( crt.firstChild().toText().data());
        //Mode
        else if (crt.tagName() == "mode")
        {
            UvmPort::PortMode mode = UvmPort::StringToMode(crt.firstChild().toText().data());
            if (mode == UvmPort::MODE_ERROR)
            {
                errorString += QString("Mode of port unknown: %1").arg(crt.firstChild().toText().data());
                return 0;
            }
            port->setMode(mode);
        }
        //Direction
        else if (crt.tagName() == "direction")
        {
            UvmPort::PortDirection direction = UvmPort::StringToDirection(crt.firstChild().toText().data());
            if (direction == UvmPort::DIRECTION_ERROR)
            {
                errorString += QString("Direction of port unknown: %1").arg(crt.firstChild().toText().data());
                return 0;
            }
            port->setDirection(direction);
        }
        //Category
        else if (crt.tagName() == "category")
        {
            UvmPort::PortCategory category = UvmPort::StringToCategory(crt.firstChild().toText().data());
            if (category == UvmPort::CATEGORY_ERROR)
            {
                errorString += QString("Category of port unknown: %1").arg(crt.firstChild().toText().data());
                return 0;
            }
            port->setCategory(category);
        }
        else if (crt.tagName() == "size")
            port->setSize( crt.firstChild().toText().data().toInt());

        //Port specific data


        //Clock frequency
        else if (crt.tagName() == "frequency")
            port->setClockFrequency( crt.firstChild().toText().data().toDouble());
        //Reset active low
        else if (crt.tagName() == "active")
            port->setResetActiveLow( crt.firstChild().toText().data() == "1");
        //Reset duration
        else if (crt.tagName() == "duration")
            port->setResetDuration( crt.firstChild().toText().data().toInt());


        //What the hell?
        else
        qDebug()<< tr("Unknown tag : ") << crt.tagName() << ". In file " << __FILE__ << " at line " << __LINE__;

        n = n.nextSibling();
    }

    //Add ourselves to our parent
    if (p)
        p->addPhysicalPort(port);

    return port;
}

UvmConnection* UveXmlImport::getConnection(QDomElement source, UvmComponent *parent)
{


    BaseConnection bc;
    QDomNode n = source.firstChild();
    while(!n.isNull()) {
        QDomElement crt = n.toElement();
        QString text = crt.firstChild().toText().data();
        //Name
        if (crt.tagName() == "name")
            bc.name = text;
        //Source
        else if (crt.tagName() == "srcPort")
            bc.src = text;
        //Destination
        else if (crt.tagName() == "dstPort")
            bc.dst = text;

        n = n.nextSibling();
    }


    //Fetch the two ports
    //If a prefix is given, search in the corresponding VC for the port

    //Source
    UvmPort *src = dynamic_cast<UvmPort*>(parent->findPortForName(bc.src));
    //Destination
    UvmPort *dst = dynamic_cast<UvmPort*>(parent->findPortForName(bc.dst));

    //If one of them miss, error
    if (!dst || !src) {
        if (!dst)
            qDebug() << QObject::tr("Missing destination port '%1' for connection '%2'").arg(bc.dst,bc.name);
        if (!src)
            qDebug() << QObject::tr("Missing source port '%1' for connection '%2'").arg(bc.src,bc.name);
        return 0;
    }

    //Create the connection
    return new UvmConnection(src,dst,bc.name,parent);

}

BaseConnection UveXmlImport::getConnection(QDomElement source)
{
    BaseConnection bc;
    QDomNode n = source.firstChild();
    while(!n.isNull()) {
        QDomElement crt = n.toElement();
        QString text = crt.firstChild().toText().data();
        //Name
        if (crt.tagName() == "shortName")
            bc.name = text;
        //Source
        else if (crt.tagName() == "srcPort")
            bc.src = text;
        //Destination
        else if (crt.tagName() == "dstPort")
            bc.dst = text;

        n = n.nextSibling();
    }
    return bc;
}

//Parse and load each one, differenciating their type
void UveXmlImport::getSubComponents(UvmComponent *comp,QDomElement *root)
{
    //Basic check
    if (root->tagName() != "components") {
        qDebug() << tr("Invalid root given for component loading (%1)").arg(root->tagName());
        return;
    }

    //For each element
    QDomNode n = root->firstChild();
     while (!n.isNull()) {
         QDomElement e = n.toElement();
         QString tag = e.tagName();
         if (tag == "monitor")
             comp->addChild(getMonitor(e,comp));
         else if (tag == "sequencer")
             comp->addChild(getSequencer(e,comp));
         else if (tag == "collector")
             comp->addChild(getCollector(e,comp));
         else if (tag == "driver")
             comp->addChild(getDriver(e,comp));
         else if (tag == "agent")
             comp->addChild(getAgent(e,comp));
         else if (tag == "interface")
             comp->addChild(getInterface(e,comp));
         else if (tag == "vcdata")
             comp->addChild(getVerificationComponent(e,comp));
         else if (tag == "virtualSequencer")
             comp->addChild(getVirtualSequencer(e,comp));
         else if (tag == "testbench")
             comp->addChild(getTestbench(e,comp));
         else if (tag == "testcase")
             comp->addChild(getTestCase(e,comp));
         else if (tag == "configuration")
             comp->addChild(getConfig(e,comp));
         else if (tag == "scoreboard")
             comp->addChild(getScoreboard(e,comp));
         else if (tag == "sequenceitem")
             comp->addChild(getSequenceItem(e,comp));
         else if (tag == "sequence")
             comp->addChild(getSequence(e,comp));
         else if (tag == "design")
             comp->addChild(getDesign(e,comp));
         else if (tag == "package")
             comp->addChild(getPackage(e,comp));
         else if (tag == "vc")
         {
             UvmVerificationComponent *vc=getVerificationComponentFromFile(e,comp);
             if (vc)
                 comp->addChild(vc);
         }
         else
             qDebug() << tr("Unknown type : %1").arg(tag);

         n = n.nextSibling();
     }
}


UvmVerificationComponent* UveXmlImport::getVerificationComponentFromFile(QDomElement source, UvmComponent *parent)
{
    UvmVerificationComponent *vc=0;

    QString fileName;
    bool toGenerate=false;
    QString instName;

    //Go through the sub-elements
    QDomNode n = source.firstChild();
    while(!n.isNull()) {
        //Should be an element
        QDomElement crt = n.toElement();

        //File (for retrocompatibility purposes)
        if (crt.tagName() == "file")
            fileName = crt.firstChild().toText().data();

        //InstName
        else if (crt.tagName() == "instName")
            instName = crt.firstChild().toText().data();

        //body File
        else if (crt.tagName() == "toGenerate")
        {
            QString s = crt.firstChild().toText().data();
            if (s.compare("true")==0)
                toGenerate = true;
            else if (s.compare("false")==0)
                toGenerate = false;
            else
            {
                errorString += QString("The field toGenerate of the VIP instanciation is wrong: %1").arg(s);
                return 0;
            }
        }

        //What the hell?
        else
            qDebug()<< tr("Unknown tag : ") << crt.tagName() << ". In file " << __FILE__ << " at line " << __LINE__;

        n = n.nextSibling();
    }

    vc=loadVerificationComponent(parent->getProject()->getFolder()+fileName,parent,toGenerate);
    if (vc)
    {
        vc->setVipRelativePath(QFileInfo(fileName).path()+"/");
        vc->setRelativePath(QFileInfo(fileName).path()+"/"+vc->getRelativePath());
        vc->setInstName(instName);
    }
    return vc;
}

UvmMonitor* UveXmlImport::getMonitor(QDomElement source, UvmComponent *parent)
{
    UvmMonitor* comp= new UvmMonitor(parent);
    QStringList ignore;
    ignore.append("coverage");
    getComponent(comp,source,parent,ignore);
/*
    //Go through the sub-elements
    QDomElement n = source.firstChildElement("coverage");
    if (!n.isNull())
        comp->setCoverage( n.text().toInt());
*/
    return comp;

}

UvmSequencer* UveXmlImport::getSequencer(QDomElement source, UvmComponent *parent)
{
    UvmSequencer *comp= new UvmSequencer(parent);
    getComponent(comp,source,parent);
    return comp;
}

UvmCollector* UveXmlImport::getCollector(QDomElement source, UvmComponent *parent)
{
    UvmCollector *comp= new UvmCollector(parent);
    getComponent(comp,source,parent);
    return comp;
}

UvmDriver* UveXmlImport::getDriver(QDomElement source, UvmComponent *parent)
{
    UvmDriver *comp= new UvmDriver(parent);
    getComponent(comp,source,parent);
    return comp;
}

UvmAgent* UveXmlImport::getAgent(QDomElement source, UvmComponent *parent)
{
    UvmAgent *comp= new UvmAgent();
    comp->setParent(parent);
    comp->setType(UvmAgent::ACTIV);
    comp->setNbAgents(1);
    QStringList ignore;
    ignore << "nbAgents";
    getComponent(comp,source,parent,ignore);

    QDomElement n = source.firstChildElement("nbAgents");
    if (!n.isNull())
        comp->setNbAgents(n.text().toInt());

    return comp;
}

UvmInterface* UveXmlImport::getInterface(QDomElement source, UvmComponent *parent)
{
    UvmInterface *comp= new UvmInterface(parent);

    QStringList ignore;
    ignore << "physicalports";
    getComponent(comp,source,parent,ignore);


    //Go through the sub-elements
    QDomNode n = source.firstChild();
    while(!n.isNull()) {
        //Should be an element
        QDomElement crt = n.toElement();
        if (crt.tagName() == "physicalports")
            getPhysicalPorts(comp,&crt);

        n = n.nextSibling();
    }

    return comp;
}

UvmVerificationComponent* UveXmlImport::getVerificationComponent(QDomElement source, UvmComponent *parent)
{
    UvmVerificationComponent *comp= new UvmVerificationComponent(parent);
    getComponent(comp,source,parent);
    return comp;
}

UvmVirtualSequencer* UveXmlImport::getVirtualSequencer(QDomElement source, UvmComponent *parent)
{
    UvmVirtualSequencer *comp= new UvmVirtualSequencer(parent);
    getComponent(comp,source,parent);
    return comp;
}

UvmTestbench* UveXmlImport::getTestbench(QDomElement source, UvmComponent *parent)
{
    UvmTestbench *comp= new UvmTestbench(parent);
    getComponent(comp,source,parent);
    return comp;
}

UvmTestCase* UveXmlImport::getTestCase(QDomElement source, UvmComponent *parent)
{
    UvmTestCase *comp= new UvmTestCase(parent);
    getComponent(comp,source,parent);
    return comp;
}

UvmConfig* UveXmlImport::getConfig(QDomElement source, UvmComponent *parent)
{
    UvmConfig *comp= new UvmConfig(parent);
    getComponent(comp,source,parent);
    return comp;
}

UvmScoreboard* UveXmlImport::getScoreboard(QDomElement source, UvmComponent *parent)
{
    UvmScoreboard *comp= new UvmScoreboard(parent);
    getComponent(comp,source,parent);
    return comp;
}

UvmSequenceItem* UveXmlImport::getSequenceItem(QDomElement source, UvmComponent *parent)
{
    UvmSequenceItem *comp= new UvmSequenceItem(parent);
    getComponent(comp,source,parent);
    return comp;
}

UvmSequence* UveXmlImport::getSequence(QDomElement source, UvmComponent *parent)
{
    UvmSequence *comp= new UvmSequence(parent);
    getComponent(comp,source,parent);
    return comp;
}

UvmDesign* UveXmlImport::getDesign(QDomElement source, UvmComponent *parent)
{
    UvmDesign *comp= new UvmDesign(parent);

    //Go through the sub-elements
    QDomNode n = source.firstChild();
    while(!n.isNull()) {
        //Should be an element
        QDomElement crt = n.toElement();

        //Name
        if (crt.tagName() == "shortName") {
            comp->setShortName(crt.firstChild().toText().data());
            comp->setClassName(comp->getShortName());
        }
        //Inst
        else if (crt.tagName() == "instName") {
            comp->setInstName(crt.firstChild().toText().data());
        }
        //File
        else if (crt.tagName() == "file")
            comp->setBodyFileName(crt.firstChild().toText().data());
        //body File
        else if (crt.tagName() == "bodyFile") {
            comp->setBodyFileName(crt.firstChild().toText().data());
            QFileInfo info(comp->getProject()->getProjectFileName());
            comp->setRelativePath(info.absoluteDir().relativeFilePath(comp->getAbsoluteBodyFileName()) + DIRSEPARATOR);

//            comp->setRelativePath(QFileInfo(comp->getBodyFileName()).path()+"/");
        }
        //header File
        else if (crt.tagName() == "headerFile")
            comp->setHeaderFileName(crt.firstChild().toText().data());
        //other files
        else if (crt.tagName() == "otherFile")
            comp->addFile(crt.firstChild().toText().data());

        //Sub-Components
        else if (crt.tagName() == "components")
            getSubComponents(comp,&crt);

            //Ports
            else if (crt.tagName() == "tlmports")
                getPorts(comp,&crt);
            //Ports
            else if (crt.tagName() == "fakeports")
                getPorts(comp,&crt);

        //Ports
        else if (crt.tagName() == "physicalports")
            getPhysicalPorts(comp,&crt);

        //What the hell?
        else
            qDebug()<< tr("Unknown tag : ") << crt.tagName() << ". In file " << __FILE__ << " at line " << __LINE__;

        n = n.nextSibling();
    }

    return comp;
}

UvmPackage* UveXmlImport::getPackage(QDomElement source, UvmComponent *parent)
{
    UvmPackage *comp= new UvmPackage(parent);

    //Go through the sub-elements
    QDomNode n = source.firstChild();
    while(!n.isNull()) {
        //Should be an element
        QDomElement crt = n.toElement();

        //Name
        if (crt.tagName() == "shortName") {
            comp->setShortName(crt.firstChild().toText().data());
            comp->setClassName(comp->getShortName());
        }
        //ClassName
        else if (crt.tagName() == "className")
            comp->setClassName(crt.firstChild().toText().data());
        //File
        else if (crt.tagName() == "file")
            comp->setBodyFileName(crt.firstChild().toText().data());
        //body File
        else if (crt.tagName() == "bodyFile")
            comp->setBodyFileName(crt.firstChild().toText().data());
        //header File
        else if (crt.tagName() == "headerFile")
            comp->setHeaderFileName(crt.firstChild().toText().data());

        //What the hell?
        else
            qDebug()<< tr("Unknown tag : ") << crt.tagName() << ". In file " << __FILE__ << " at line " << __LINE__;

        n = n.nextSibling();
    }

    return comp;
}

void UveXmlImport::getComponent(UvmComponent *comp,QDomElement source, UvmComponent *parent,const QStringList &ignore)
{
    comp->setParent(parent);
    comp->setToGenerate(true);

    //Go through the sub-elements
    QDomNode n = source.firstChild();
    while(!n.isNull()) {
        //Should be an element
        QDomElement crt = n.toElement();

        //Name
        if (crt.tagName() == "shortName") {
            comp->setShortName(crt.firstChild().toText().data());
            comp->setClassName(comp->getShortName());
        }
        //ClassName
        else if (crt.tagName() == "className")
            comp->setClassName( crt.firstChild().toText().data());
        //InstName
        else if (crt.tagName() == "instName")
            comp->setInstName( crt.firstChild().toText().data());

        else if (crt.tagName() == "svdir") {
            QString svdir = crt.firstChild().toText().data();
            if (!svdir.isEmpty()) {
                if (svdir.lastIndexOf("/")==svdir.length()-1)
                    comp->setRelativePath(svdir);
                else
                    comp->setRelativePath(svdir+"/");
            }
        }
        //File
        else if (crt.tagName() == "file")
            comp->setBodyFileName(crt.firstChild().toText().data());
        //body File
        else if (crt.tagName() == "bodyFile")
            comp->setBodyFileName(crt.firstChild().toText().data());
        //header File
        else if (crt.tagName() == "headerFile")
            comp->setHeaderFileName(crt.firstChild().toText().data());
        //other files
        else if (crt.tagName() == "otherFile")
            comp->addFile(crt.firstChild().toText().data());

        //Sub-Components
        else if (crt.tagName() == "components")
            getSubComponents(comp,&crt);

        //Ports
        else if (crt.tagName() == "tlmports")
            getPorts(comp,&crt);
        //Ports
        else if (crt.tagName() == "fakeports")
            getPorts(comp,&crt);

        //Connections
        else if (crt.tagName() == "connections") {
            QDomNode nn = crt.firstChild();
            while (!nn.isNull()) {
                getConnection(nn.toElement(),comp);
                nn = nn.nextSibling();
            }
        }
        else if (!ignore.contains(crt.tagName()))
            //What the hell?
            qDebug()<< tr("Unknown tag : ") << crt.tagName() << ". In file " << __FILE__ << " at line " << __LINE__;

        n = n.nextSibling();
    }
}

UvmVerificationComponent* UveXmlImport::loadVerificationComponent(const QString &fileName, UvmComponent *parent,bool toRegenerate)
{

    QString xsdFileName = UVESettings::getInstance()->value("VIPXSD").toString();
    QUrl schemaUrl=QUrl::fromLocalFile(xsdFileName);


    QXmlSchema schema;
    schema.load(schemaUrl);

    if (schema.isValid()) {

        QXmlSchemaValidator validator(schema);
        if (!validator.validate(QUrl::fromLocalFile(fileName)))
        {
            qDebug() << "instance document is invalid";
            errorString += "The file " + fileName + " does not contain a valid .uvc structure.";
            faultyFiles.append(fileName);
            return 0;
        }

    }
    else
    {
        qDebug() << "The file uvc.xsd is not a valid xsd file";
    }


    UvmVerificationComponent *comp=new UvmVerificationComponent(parent);

    comp->setVipFileName(QFileInfo(fileName).fileName());
    comp->setOriginalVipPath(QFileInfo(fileName).absolutePath());

    QDomDocument doc("vip");
    QFile f(fileName);
    if (!f.open(QIODevice::ReadOnly)) {
        errorString += tr("Could not open the VC file %1\n").arg(fileName);
        return 0;
    } if (!doc.setContent(&f)) {
        errorString += tr("Could not read the VC XML %1\n").arg(fileName);
        f.close();
        return 0;
    }
    f.close();

    //Fetch the VIP  and generated roots
    QDomElement vip = doc.documentElement();

    if (vip.isNull()) {
        errorString += tr("Invalid VIP XML (no <vip> tag found)\n");
        return 0;
    }

    //Test the VIP version
    QString version = vip.attribute("version", "0.0");
    if (version != XML_VIP_VERSION) {
        errorString += tr("Invalid VIP XML version (expected %1, got %2)\n").arg(XML_VIP_VERSION,version);
        return 0;
    }

    //Save the VIP name and description
    comp->setShortName(valueOf("shortName",vip));
    comp->setClassName(valueOf("className",vip));

    QString generated =valueOf("generated",vip);
    if (generated == "true")
        comp->setGenerated(true);
    else if (generated == "false")
        comp->setGenerated("false");
    else
        qCritical() << "Aie aie aie in file " << __FILE__ << " line " << __LINE__;

    comp->setDescription(valueOf("description",vip));
    comp->setBodyFileName(valueOf("bodyFile",vip));
    comp->setHeaderFileName(valueOf("headerFile",vip,""));

    QString svdir = valueOf("svdir",vip);
    if (!svdir.isEmpty()) {
        comp->setVipRelativePath(comp->getRelativePath());
        comp->setRelativePath(comp->getRelativePath() + svdir+"/");
    }

        //Go through the sub-elements
    QDomElement e = vip.firstChildElement("components");
    if (!e.isNull())
        getSubComponents(comp,&e);
    e = vip.firstChildElement("connections");
    if (!e.isNull())
    {
        QDomNode nn = e.firstChild();
        while (!nn.isNull()) {
            getConnection(nn.toElement(),comp);
            nn = nn.nextSibling();
        }
    }

    //We need not to be generated again, as we are a library item
    comp->setToGenerate(toRegenerate);
    // We do not need to generate the children of an existing VIP
    foreach ( UvmComponent *child, comp->getAllChildren())
        child->setToGenerate(toRegenerate);

    return comp;
}

//Get the first child of the child named "tag" from root
QString UveXmlImport::valueOf(const QString &tag, const QDomElement &root,const QString defaultValue) const
{
    if (root.firstChildElement(tag).isNull())
        return defaultValue;
    return root.firstChildElement(tag).firstChild().toText().data();
}
