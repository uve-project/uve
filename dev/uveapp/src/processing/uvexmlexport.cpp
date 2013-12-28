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
#include "uvexmlexport.h"
#include "uvmtop.h"
#include "uvmsequence.h"
#include "uvmpackage.h"

#include "current_version.h"

#include <QDomElement>

UveXmlExport::UveXmlExport()
{
    errorString="";
}

QString UveXmlExport::getErrorString() const {
    return errorString;
}


//Create the element, add the text node, and add the all in the root
void UveXmlExport::saveElement(const QString &name, const QString &value, QDomElement *root)
{
    QDomDocument doc = root->ownerDocument();
    QDomElement e = doc.createElement(name);
    e.appendChild(doc.createTextNode(value));
    root->appendChild(e);
}

bool UveXmlExport::saveVip(const UvmVerificationComponent *comp,const QString fileName){
    //If the element is not to be generated, then do not generate it
    if ( ! comp->toGenerate() )
        return true;

    //XML document
    doc=QDomDocument("vip");

    doc.appendChild(doc.createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'"));
    doc.appendChild(doc.createProcessingInstruction("xml-stylesheet","href=\"uvc.xsl\" type=\"text/xsl\""));

    doc.appendChild(saveVipData(comp));

    //Save the result
    QFile f(fileName);

    if (!f.open(QIODevice::WriteOnly)) {
        errorString += QObject::tr("Impossible to open %1 for writing\n").arg(fileName);
        return false;
    }
    QTextStream out(&f);
    out << doc.toString();
    f.close();

    return "";


}

bool UveXmlExport::saveProject(const UvmProject *project){

    errorString.clear();

    // Create the folders for the project
    QDir().mkpath(project->getFolder());

    QString fullFilename= project->getFolder() + project->getProjectFileName();

    //XML document
    doc=QDomDocument("uve");
    doc.appendChild(doc.createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'"));
    doc.appendChild(doc.createProcessingInstruction("xml-stylesheet","href=\"uve.xsl\" type=\"text/xsl\""));
    QDomElement root = doc.createElement("uve");


    root.setAttribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance");
    root.setAttribute("xsi:noNamespaceSchemaLocation","uve.xsd");


    root.setAttribute("version", XML_PROJECT_VERSION);
    doc.appendChild(root);

    //  Project stuff
    saveElement("simulator",project->getSimulator(),&root);
    saveElement("dut",project->getDut(),&root);
    saveElement("name",project->getName(),&root);
    saveElement("templates",project->getTemplatePath(),&root);

    // VHDL files
    QDomElement files = doc.createElement("vhdl");
    foreach (QString str, project->getVHDLFiles())
        saveElement("file",str,&files);
    root.appendChild(files);

    //Top
    root.appendChild(visit(project->getTop()));

    //Save the result
    QFile file(fullFilename);
    if (!file.open(QIODevice::WriteOnly)) {
        errorString+= QObject::tr("Impossible to open %1 for writing").arg(file.fileName());
        return false;
    }
    QTextStream out(&file);
    out << doc.toString();
    file.close();




    //Call to each VC, and save the link
    foreach (UvmVerificationComponent *vc, project->getTop()->getTestCase()->getTestbench()->getVerificationComponents()) {
        if (vc->toGenerate())
        {
            QString fn = vc->getRelativePath() + vc->getVipFileName();
            QDir().mkpath(project->getFolder() + vc->getRelativePath());
            errorString += saveVip(vc,project->getFolder() + fn);
        }
    }



    return true;


}



//For each port, save it
void UveXmlExport::savePorts(const UvmComponent *comp,QDomElement *root)
{
    QDomDocument doc = root->ownerDocument();
    //Base element
    QList<TlmPort*> tlmList = comp->getTlmPorts();
    QList<FakePort*> fakeList = comp->getFakePorts();
    if (!tlmList.isEmpty())
    {
        QDomElement pe = doc.createElement("tlmports");
        //For each port
        foreach (TlmPort *port, tlmList)
            pe.appendChild(port->accept(this));
        root->appendChild(pe);
    }
    if (!fakeList.isEmpty())
    {
        QDomElement pe = doc.createElement("fakeports");
        //For each port
        foreach (FakePort *port, fakeList)
            pe.appendChild(port->accept(this));
        root->appendChild(pe);
    }
}

void UveXmlExport::savePhysicalPorts(const PhysicalPortList *comp,QDomElement *root)
{
    QDomDocument doc = root->ownerDocument();
    //Base element
    QDomElement pe = doc.createElement("physicalports");
    //For each port
    foreach (PhysicalPort *port, comp->getPhysicalPorts())
        pe.appendChild(port->accept(this));
    root->appendChild(pe);
}


void UveXmlExport::saveConnections(const UvmComponent *comp,QDomElement *root)
{
    if (comp->getConnections().count()>0)
    {
        //Save each connection
        QDomElement cons = doc.createElement("connections");
        root->appendChild(cons);
        foreach (UvmConnection *conn, comp->getConnections())
            cons.appendChild(visit(conn));
    }
}

static bool compareComponentsTagName(const UvmComponent *c1, const UvmComponent *c2)
 {
     return UveXmlExport::getTagName(c1) < UveXmlExport::getTagName(c2);
 }

QString UveXmlExport::getTagName(const UvmComponent *comp)
{
    if (dynamic_cast<const UvmPackage *>(comp))
        return "package";
    if (dynamic_cast<const UvmConfig *>(comp))
        return "configuration";
    if (dynamic_cast<const UvmInterface *>(comp))
        return "interface";
    if (dynamic_cast<const UvmSequenceItem *>(comp))
        return "sequenceitem";
    if (dynamic_cast<const UvmSequence *>(comp))
        return "sequence";
    if (dynamic_cast<const UvmMonitor *>(comp))
        return "monitor";
    if (dynamic_cast<const UvmSequencer *>(comp))
        return "sequencer";
    if (dynamic_cast<const UvmVirtualSequencer *>(comp))
        return "virtualSequencer";
    if (dynamic_cast<const UvmScoreboard *>(comp))
        return "scoreboard";
    if (dynamic_cast<const UvmAgent *>(comp))
        return "agent";
    if (dynamic_cast<const UvmCollector *>(comp))
        return "collector";
    if (dynamic_cast<const UvmDriver *>(comp))
        return "driver";
    if (dynamic_cast<const UvmTestCase *>(comp))
        return "testcase";
    if (dynamic_cast<const UvmTestbench *>(comp))
        return "testbench";
    if (dynamic_cast<const UvmDesign *>(comp))
        return "design";
    qCritical() << "Component unknown: " << comp->getShortName();
    return "";
}

//For each component, save it
void UveXmlExport::saveComponents(const UvmComponent *comp,QDomElement *root)
{
    if (comp->getChildren().count()==0)
        return;
    QDomDocument doc = root->ownerDocument();
    //Base element
    QDomElement cs = doc.createElement("components");
    //For each port
    QList<UvmComponent *> initialList;

    foreach (UvmComponent *c, comp->getChildren())
        initialList.append(c);

    qSort(initialList.begin(),initialList.end(),compareComponentsTagName);

    foreach (UvmComponent *c, initialList)
        cs.appendChild(c->accept(this));
    root->appendChild(cs);
}

void UveXmlExport::saveBasicComponent(const UvmComponent *comp, QDomElement root)
{
    saveElement("shortName",comp->getShortName(),&root);
    saveElement("instName",comp->getInstName(),&root);
    saveElement("className",comp->getClassName(),&root);
    if(comp->hasHeaderFile())
        saveElement("headerFile",comp->getHeaderFileName(),&root);
    saveElement("bodyFile",comp->getBodyFileName(),&root);
    QList<QString> otherFiles = comp->getOtherFiles();
    if(!otherFiles.empty())
    {
        foreach(QString file, otherFiles)
        {
            saveElement("otherFile",file,&root);
        }
    }
}

QDomElement UveXmlExport::visit(const UvmTop *comp){
    QDomElement root = doc.createElement("top");
    saveBasicComponent(comp, root);
    savePorts(comp,&root);
    saveComponents(comp,&root);
    saveConnections(comp,&root);
    return root;
}


// Just save the data
QDomElement UveXmlExport::visit(const PhysicalPort *port)
{
    if (port->getName().contains("_interinterport"))
        return QDomElement();


    QDomElement root = doc.createElement("port");
    saveElement("name",port->getName(),&root);
    if (!((port->getType().isEmpty()) ||
          (port->getType().compare("std_logic")==0) ||
          (port->getType().compare("std_ulogic")==0) ||
          (port->getType().compare("std_logic_vector")==0) ||
          (port->getType().compare("std_ulogic_vector")==0)))
        saveElement("type",port->getType(),&root);
    saveElement("mode",UvmPort::modeToString(port->getMode()),&root);
    saveElement("direction",UvmPort::DirectionToString(port->getDirection()),&root);
    saveElement("category",UvmPort::categoryToString(port->getCategory()),&root);
    saveElement("size",QString::number(port->getSize()),&root);

    //For the clock/reset, other values have to be saved
    switch (port->getCategory()) {
    case UvmPort::CLOCK:
        saveElement("frequency",QString::number(port->getClockFrequency()),&root);
        break;
    case UvmPort::RESET:
        saveElement("duration",QString::number(port->getResetDuration()),&root);
        saveElement("active",port->getResetActiveLow() ? "low":"high",&root);
        break;
    default: break;
    }
    return root;
}






// Just save the data
QDomElement UveXmlExport::visit(const TlmPort *port)
{
    if (port->getName().contains("_interinterport"))
        return QDomElement();

    QDomElement root = doc.createElement("tlmport");
    saveElement("className",UvmPort::modeToString(port->getMode()),&root);
    saveElement("name",port->getName(),&root);
    return root;
}




// Just save the data
QDomElement UveXmlExport::visit(const FakePort *port)
{
    if (port->getName().contains("_interinterport"))
        return QDomElement();

    const FakePort *p=dynamic_cast<const FakePort*>(port);
    QDomElement root = doc.createElement("fakeport");
    saveElement("className",UvmPort::modeToString(p->getMode()),&root);
    saveElement("name",p->getName(),&root);
    saveElement("direction",UvmPort::DirectionToString(p->getDirection()),&root);
    return root;
}





QDomElement UveXmlExport::visit(const UvmConnection *conn)
{
    // Fetch each port VC name (if any) and use it as prefix
    //Source
    UvmPort *srcPort=conn->getSrcPort();

    //Destination
    UvmPort *dstPort=conn->getDstPort();

    if ((dstPort->getName().contains("_interinterport")) ||
         (srcPort->getName().contains("_interinterport")))
            return QDomElement();

    QDomElement root = doc.createElement("connection");
    saveElement("name",conn->getName(),&root);

    UvmComponent *srcComp = srcPort->getParent();
    UvmComponent *dstComp = dstPort->getParent();

    if (!srcComp)
    {
        qCritical() << "No source component for port " << srcPort->getName() << " in file " <<  __FILE__ << " line " << __LINE__;
        return root;
    }
    if (!dstComp)
    {
        qCritical() << "No destination component for port " << dstPort->getName() << " in file " << __FILE__ << " line " << __LINE__;
        return root;
    }

    saveElement("srcPort",srcPort->getParent()->getRelativeInstName(conn->getParent())+"."+srcPort->getName(),&root);
    saveElement("dstPort",dstPort->getParent()->getRelativeInstName(conn->getParent())+"."+dstPort->getName(),&root);

    return root;
}


QDomElement UveXmlExport::visit(const UvmTestCase *comp){

    QDomElement root = doc.createElement("testcase");
    saveBasicComponent(comp, root);
    savePorts(comp,&root);
    saveComponents(comp,&root);
    saveConnections(comp,&root);
    return root;
}


QDomElement UveXmlExport::visit(const UvmTestbench *comp){

    QDomElement root = doc.createElement("testbench");
    saveBasicComponent(comp, root);
    savePorts(comp,&root);

    //All components except VCs
    QDomElement cs = doc.createElement("components");
    //For each component
    foreach (UvmComponent *c, comp->getChildren())
        cs.appendChild(c->accept(this));
    root.appendChild(cs);

    saveConnections(comp,&root);

    return root;
}

QDomElement UveXmlExport::visit(const UvmScoreboard *comp){

    QDomElement root = doc.createElement("scoreboard");
    saveBasicComponent(comp, root);
    savePorts(comp,&root);
    saveComponents(comp,&root);
    saveConnections(comp,&root);

    QString ctype;
    switch(comp->getComparatorType())
    {
    case UvmScoreboard::NOCOMPARATOR: ctype = "nocomparator";break;
    case UvmScoreboard::ONLYCOMPARATOR: ctype = "onlycomparator";break;
    case UvmScoreboard::PORTANDCOMPARATOR: ctype = "portandcomparator";break;
    }
    saveElement("comparator",ctype,&root);

    return root;
}

QDomElement UveXmlExport::visit(const UvmDesign *comp){

    QDomElement root = doc.createElement("design");
    saveElement("shortName",comp->getShortName(),&root);
    saveElement("instName",comp->getInstName(),&root);
    saveElement("bodyFile",comp->getBodyFileName(),&root);
    savePorts(comp,&root);
    saveComponents(comp,&root);
    saveConnections(comp,&root);
    savePhysicalPorts(comp,&root);
    return root;
}


QDomElement UveXmlExport::visit(const UvmVerificationComponent *comp){

    //Only the components and ports have to be saved, the rest is in the VIP part
    QString fn = comp->getVipRelativePath() + comp->getVipFileName();
    QDir().mkpath(comp->getProject()->getFolder() + comp->getVipRelativePath());

    QDomElement root = doc.createElement("vc");
    saveElement("file",fn,&root);
    saveElement("instName",comp->getInstName(),&root);
    saveElement("toGenerate",comp->toGenerate()?"true":"false",&root);
    return root;
}


QDomElement UveXmlExport::saveVipData(const UvmVerificationComponent *comp){

    //Only the components and ports have to be saved, the rest is in the VIP part


    //VIP data
    QDomElement vip = doc.createElement("vip");
    vip.setAttribute("version", XML_VIP_VERSION);

    vip.setAttribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance");
    vip.setAttribute("xsi:noNamespaceSchemaLocation","uvc.xsd");


    saveElement("generated",comp->getGenerated()?"true":"false",&vip);
    saveElement("shortName",comp->getShortName(),&vip);
    saveElement("className",comp->getClassName(),&vip);
    //saveElement("svdir",".",&vip);
    saveElement("description",comp->getDescription(),&vip);
    saveElement("bodyFile",comp->getBodyFileName(),&vip);
    if(comp->getHeaderFileName().compare("") !=0)
        saveElement("headerFile", comp->getHeaderFileName(), &vip);
    if(!comp->getOtherFiles().isEmpty())
    {
        QList<QString> otherFiles = comp->getOtherFiles();
        for(int i=0; i<otherFiles.count(); i++)
        {
            saveElement("otherFile", otherFiles.at(i), &vip);
        }
    }

    saveComponents(comp,&vip);
    savePorts(comp,&vip);
    saveConnections(comp,&vip);
    return vip;
}

QDomElement UveXmlExport::visit(const UvmVirtualSequencer *comp){

    QDomElement root = doc.createElement("virtualSequencer");
    saveBasicComponent(comp, root);
    savePorts(comp,&root);
    saveComponents(comp,&root);
    saveConnections(comp,&root);
    return root;
}

QDomElement UveXmlExport::visit(const UvmAgent *comp){

    QDomElement root = doc.createElement("agent");
    saveBasicComponent(comp, root);
    savePorts(comp,&root);
    saveComponents(comp,&root);
    saveConnections(comp,&root);
    saveElement("nbAgents", QString().setNum(comp->getNbAgents()), &root);
    if (!comp->getOwnRelativePath().isEmpty())
        saveElement("svdir",comp->getOwnRelativePath(),&root);
    return root;
}

QDomElement UveXmlExport::visit(const UvmCollector *comp){

    QDomElement root = doc.createElement("collector");
    saveBasicComponent(comp, root);
    savePorts(comp,&root);
    saveComponents(comp,&root);
    saveConnections(comp,&root);
    return root;
}

QDomElement UveXmlExport::visit(const UvmDriver *comp){

    QDomElement root = doc.createElement("driver");
    saveBasicComponent(comp, root);
    savePorts(comp,&root);
    saveComponents(comp,&root);
    saveConnections(comp,&root);
    return root;
}

QDomElement UveXmlExport::visit(const UvmMonitor *comp){

    QDomElement root = doc.createElement("monitor");
    saveBasicComponent(comp, root);
//    saveElement("coverage",QString::number(comp->getCoverage()),&root);
    savePorts(comp,&root);
    saveComponents(comp,&root);
    saveConnections(comp,&root);
    return root;
}

QDomElement UveXmlExport::visit(const UvmSequencer *comp){

    QDomElement root = doc.createElement("sequencer");
    saveBasicComponent(comp, root);
    savePorts(comp,&root);
    saveComponents(comp,&root);
    saveConnections(comp,&root);
    return root;
}

QDomElement UveXmlExport::visit(const UvmSequence *comp){

    QDomElement root = doc.createElement("sequence");
    saveBasicComponent(comp, root);
    savePorts(comp,&root);
    saveComponents(comp,&root);
    saveConnections(comp,&root);
    return root;
}

QDomElement UveXmlExport::visit(const UvmSequenceItem *comp){

    QDomElement root = doc.createElement("sequenceitem");
    saveBasicComponent(comp, root);
    savePorts(comp,&root);
    saveComponents(comp,&root);
    saveConnections(comp,&root);
    return root;
}

QDomElement UveXmlExport::visit(const UvmConfig *comp){

    QDomElement root = doc.createElement("configuration");
    saveBasicComponent(comp, root);
    savePorts(comp,&root);
    saveConnections(comp,&root);
    return root;
}

QDomElement UveXmlExport::visit(const UvmInterface *comp){

    QDomElement root = doc.createElement("interface");
    saveBasicComponent(comp, root);
    savePorts(comp,&root);
    savePhysicalPorts(comp,&root);
    saveConnections(comp,&root);
    return root;
}

QDomElement UveXmlExport::visit(const UvmPackage *comp){
    QDomElement root = doc.createElement("package");
    saveBasicComponent(comp, root);
    return root;
}
