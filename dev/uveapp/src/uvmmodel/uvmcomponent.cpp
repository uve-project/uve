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
#include "uvmcomponent.h"

#include <QDomDocument>
#include <QDebug>

#include "uvmproject.h"
#include "uvmtop.h"
#include "uvmconnection.h"
#include "uvmverificationcomponent.h"
#include "uvmdesign.h"
#include "uvmport.h"
#include "uvmtestbench.h"
#include "uvmtestcase.h"
#include "uvmsequenceitem.h"
#include "uvmsequence.h"
#include "uvmpackage.h"

// Constructor of the component
UvmComponent::UvmComponent(UvmComponent *parent) :
    fileNameSet(false),
    _toGenerate(true)
{
    this->shortName = "";
    this->parent = parent;

    if(parent)
        parent->addChild(this);

    setRelativePath("");
}

// Constructor of the component
UvmComponent::UvmComponent(QString name, UvmComponent *parent) :
    fileNameSet(false),
    _toGenerate(true)
{
    this->parent = parent;

    if(parent)
        parent->addChild(this);

    setShortName(name);
    setRelativePath("");
}

UvmComponent::~UvmComponent()
{
    UvmPort *port;
    UvmComponent *comp;

    int nbrElement = ports.count();
    for(int i=0; i < nbrElement; i++ ){
        port = ports.takeAt(0);
        if(port !=NULL)
            delete port;
    }

    nbrElement = children.count();
    for(int i=0; i<nbrElement; i++ ){
        comp = children.takeAt(0);
        if(comp !=NULL)
            delete comp;
    }
}


void UvmComponent::addConnection(UvmConnection *connection)
{
    connections << connection;
    connection->setParent(this);
}

QList<UvmConnection*> UvmComponent::getConnections() const
{
    return connections;
}

// Add a port to the component
void UvmComponent::addPort(NonPhysicalPort *newPort)
{
    if(!ports.contains(newPort))
        ports.append(newPort);
}

// Remove the port from the component
void UvmComponent::removePort(NonPhysicalPort *port)
{
    ports.removeAll(port);
}

// Add a child componnent to the component, and set us as its parent
void UvmComponent::addChild(UvmComponent *newChild)
{
    if(!children.contains(newChild)) {
        children.append(newChild);
        newChild->setParent(this);
    }
}


bool UvmComponent::toGenerate() const
{
    return _toGenerate;
}

void UvmComponent::setToGenerate(bool b)
{
    _toGenerate = b;
}

// Get the ports of the component
QList<NonPhysicalPort *> UvmComponent::getPorts() const
{
    return ports;
}

QList<TlmPort*> UvmComponent::getTlmPorts() const
{
    QList<TlmPort*> resultList;
    TlmPort *selectedPort;
    foreach (UvmPort *port,ports)
    {
        selectedPort = dynamic_cast<TlmPort*>(port);
        if (selectedPort)
            resultList.append(selectedPort);
    }
    return resultList;
}

QList<FakePort*> UvmComponent::getFakePorts() const
{
    QList<FakePort*> resultList;
    FakePort *selectedPort;
    foreach (UvmPort *port,ports)
    {
        selectedPort = dynamic_cast<FakePort*>(port);
        if (selectedPort)
            resultList.append(selectedPort);
    }
    return resultList;
}



// Get the children of the component
QList<UvmComponent*> UvmComponent::getChildren() const
{
    return children;
}


// Get recursively the children of the component, inclusiv itself.
QList<UvmComponent*> UvmComponent::getAllChildren() const
{
    QList<UvmComponent *> allChildren;
    foreach(UvmComponent *c, getChildren()) {
        allChildren.append(c->getAllChildren());
    }

    allChildren.append((UvmComponent *)this);

    return allChildren;
}


// Set the relative path depending on the top of the project
void UvmComponent::setRelativePath(const QString &path)
{
    QRegExp pathValidator("^([\\w.\\s/:\\\\]+[/:\\\\]){0,1}$"); //Accept /, :, \ as path separators

    if(pathValidator.indexIn(path) != -1) {
        QString tmp(path);
        relativePath = tmp.replace(QRegExp("\\s"), "_").toLower();
    } else {
        relativePath = "";
    }
}


// Get the relative path depending on the top of the project
QString UvmComponent::getRelativePath() const
{
    return getParent()->getRelativePath() + relativePath;
}

QString UvmComponent::getOwnRelativePath() const
{
    return relativePath;
}


// Set the name of file representing the component
void UvmComponent::setClassName(const QString &name)
{
    QString tmp=name;
    this->className = tmp.replace(QRegExp("\\s"), "_");
}

void UvmComponent::setInstName(const QString &name)
{
    QString tmp=name;
    this->instName = tmp.replace(QRegExp("\\s"), "_");
}

QString UvmComponent::getInstName() const
{
    return instName;
}


QString UvmComponent::getRelativeInstName(const UvmComponent *ancestor) const
{
    QString name;
    const UvmComponent *comp=this;
    while ((comp!=0)&&(comp!=ancestor))
    {
        if (!name.isEmpty())
            name= comp->getInstName() + "." + name;
        else
            name= comp->getInstName();
        comp = comp->getParent();
    }
    if (comp == 0)
        return "";
    else
        return name;
}


//Save it and set the modified flag to true
void UvmComponent::setBodyFileName(const QString &filename)
{
    fileNameSet = true;
    this->bodyFileName = filename;
}

// Get the name of file representing the component
QString UvmComponent::getBodyFileName() const
{
    return bodyFileName;
}

//Save it and set the modified flag to true
void UvmComponent::setHeaderFileName(const QString &filename)
{
    fileNameSet = true;
    this->headerFileName = filename;
}

// Get the name of file representing the component
QString UvmComponent::getHeaderFileName() const
{
    return headerFileName;
}

void UvmComponent::addFile(const QString &filePath)
{
    for(int i=0; i<otherFilesPaths.length(); i++)
    {
        if(otherFilesPaths.at(i).compare(filePath) == 0)
            return;
    }
    otherFilesPaths.append(filePath);
    //qDebug() << "OtherFile : " << relOtherFilePath;
}

void UvmComponent::addNewFile(const QString &filePath)
{
    QDir compPath(getAbsolutePath());
    QString relOtherFilePath = compPath.relativeFilePath(filePath);

    for(int i=0; i<otherFilesPaths.length(); i++)
    {
        if(otherFilesPaths.at(i).compare(relOtherFilePath) == 0)
            return;
    }
    otherFilesPaths.append(relOtherFilePath);
}

QList<QString> UvmComponent::getOtherFiles() const
{
    return otherFilesPaths;
}

QList<QString> UvmComponent::getAbsoluteOtherFiles() const
{
    QList<QString> absoluteOtherFiles;
    foreach(QString of, otherFilesPaths)
    {
        //qDebug() << "getter : " << getAbsolutePath() + of;
        absoluteOtherFiles.append(getAbsolutePath() + of);
    }
    return absoluteOtherFiles;
}

// Get the relativ path of the file name representing the component
QString UvmComponent::getRelativeBodyFileName() const
{
    return getRelativePath() + getBodyFileName();
}


// Get the absolute path of the file name representing the component
QString UvmComponent::getAbsoluteBodyFileName() const
{
    return getAbsolutePath() + getBodyFileName();
}

// Get the relativ path of the file name representing the component
QString UvmComponent::getRelativeHeaderFileName() const
{
    return getRelativePath() + getHeaderFileName();
}


// Get the absolute path of the file name representing the component
QString UvmComponent::getAbsoluteHeaderFileName() const
{
    return getAbsolutePath() + getHeaderFileName();
}

QString UvmComponent::getConfigFileName() const
{
    for(int i=0; i<children.length(); i++)
    {
        if(dynamic_cast<UvmConfig*>(children.at(i)) != NULL)
            return children.at(i)->getAbsoluteBodyFileName();
    }
    return "";
}

QString UvmComponent::getPackageFileName() const
{
    for(int i=0; i<children.length(); i++)
    {
        if(dynamic_cast<UvmPackage*>(children.at(i)) != NULL)
            return children.at(i)->getBodyFileName();
    }
    return "";
}

QString UvmComponent::getAbsolutePackageFileName() const
{
    for(int i=0; i<children.length(); i++)
    {
        if(dynamic_cast<UvmPackage*>(children.at(i)) != NULL)
            return children.at(i)->getAbsoluteBodyFileName();
    }
    return "";
}

QString UvmComponent::getSequenceFileName() const
{
    for(int i=0; i<children.length(); i++)
    {
        if(dynamic_cast<UvmSequence*>(children.at(i)) != NULL)
            return children.at(i)->getAbsoluteBodyFileName();
    }
    return "";
}

QString UvmComponent::getSequenceItemFileName() const
{
    for(int i=0; i<children.length(); i++)
    {
        if(dynamic_cast<UvmSequenceItem*>(children.at(i)) != NULL)
            return children.at(i)->getAbsoluteBodyFileName();
    }
    return "";
}

// Get the absolute path of the file name representing the component
QString UvmComponent::getAbsolutePath() const
{
    return getProject()->getFolder() + getRelativePath();
}


// Get the project of the component
UvmProject *UvmComponent::getProject() const
{
    if (!getParent())
        return 0;
    return getParent()->getProject();
}


//Go through all the ports for each component, and see if any object corresponds
QList<UvmConnection *> UvmComponent::connectionsWith(UvmComponent *other) const
{
    //Build a list of local connections
    QList<UvmConnection*> localCons;
    foreach (UvmPort *p, this->getPorts())
        localCons.append(p->getConnections());

    //Build a list of the other component connections
    QList<UvmConnection*> otherCons;
    foreach (UvmPort *p, other->getPorts())
        otherCons.append(p->getConnections());

    //Build a list of corresponding connections (same pointers)
    QList<UvmConnection *> merge;
    foreach (UvmConnection *c1, localCons)
        foreach (UvmConnection *c2, otherCons)
            if (c1 == c2)
                merge.append(c1);

    //Remove the doubles
    for (int i=0; i<merge.size(); i++)
        for (int j=i+1; j<merge.size(); j++)
            if (merge.at(i) == merge.at(j)) {
                merge.removeAt(j);
                j--;
            }

    return merge;
}

//If here, we're not a VC ;)
QString UvmComponent::vcName() const
{
    return parent->vcName();
}


//No we're not
NameSpaceRegister *UvmComponent::nameRegister(void *caller) const
{
    return parent->nameRegister(caller);
}


// Get the parent component of the component
UvmComponent* UvmComponent::getParent() const
{
    return parent;
}


// Set the parent component of the component
void UvmComponent::setParent(UvmComponent *parent)
{
    this->parent = parent;
}

UvmTop* UvmComponent::getTop()
{
    UvmTop *top=dynamic_cast<UvmTop*>(this);
    if (top)
        return top;
    else if (getParent())
        return getParent()->getTop();
    qDebug() << "UvmComponent::getTop() returns 0. Sounds strange";
    return 0;
}

UvmPort* UvmComponent::findPortForName(const QString &name) const
{
    QStringList splitted = name.split(".");
    if (splitted.size()==1)
    {
        // The name is directly the port name
        foreach (UvmPort *port,this->getPorts())
            if (port->getName()==name)
                return port;
        const PhysicalPortList *phy=dynamic_cast<const PhysicalPortList*>(this);
        if (phy)
            foreach (PhysicalPort *port,phy->getPhysicalPorts())
                if (port->getName()==name)
                    return port;
        qCritical() << "Could not find port " << name;
        return 0;
    }

    QString rightName=name;
    foreach (UvmComponent *child,this->getChildren())
    {
        if (child->getInstName()==splitted.at(0))
            return child->findPortForName(rightName.remove(0,splitted.at(0).size()+1));
    }
    qCritical() << "Could not find port " << name;
    return 0;


    /*
    if (parent)
        return parent->findPortForName(name);
    else
        return getProject()->findPortForName(name);
        */
}

// Get the name of the component
QString UvmComponent::getShortName() const
{
    if (shortName!="")
        return shortName;
    if (dynamic_cast<const UvmVerificationComponent*>(this))
        return getClassName();
    UvmVerificationComponent *vip=getParentVip();
    if (vip)
        return getClassName();
    return "";
}


// Get the default name for a new instance (Shown in interface during creation)
QString UvmComponent::getClassName() const
{
    return className;
}


// Get the default name for a new instance (Shown in interface during creation)
QString UvmComponent::getDefaultName() const
{
    QString crtName = this->metaObject()->className();

    // three call with BusMonitor yield: BusMonitor, BusMonitor_1, BusMonitor_2
    int i=1;

    //Check the name
    while (getProject()->isNameUsed(crtName))
        crtName = QString(shortName +"_%1").arg(i++);

    return crtName;
}


// Check if the name is unique and set it
bool UvmComponent::setShortName(const QString &shortName)
{
    QString crtName = shortName;
    // three call with BusMonitor yield: BusMonitor, BusMonitor_1, BusMonitor_2
    int i=1;

    //Check the name
    if (this->getProject())
        while (!this->nameRegister(this)->registerName(crtName,this))
            crtName = QString(shortName +"_%1").arg(i++);

    //Set it
    this->shortName = crtName;

    //If 'i' incremented, the name has been changed
    return i == 1;
}

QStringList UvmComponent::getAllFilePaths() const
{
    QStringList tmp = QStringList();
    foreach(UvmComponent* child, this->children)
    {
        tmp += child->getAllFilePaths();
    }
    tmp.append(this->getAbsoluteBodyFileName());
    return tmp;
}

QStringList UvmComponent::getAllVIPFilePaths() const
{
    QStringList tmp = QStringList();
    foreach(UvmComponent* child, this->children)
    {
        tmp += child->getAllVIPFilePaths();
    }
    return tmp;
}

QStringList UvmComponent::getAllNonVIPFilePaths() const
{
    QStringList tmp = QStringList();
    foreach(UvmComponent* child, this->children)
    {
        tmp +=  child->getAllNonVIPFilePaths();
    }
    tmp.append(this->getAbsoluteBodyFileName());
    return tmp;
}


void UvmComponent::printComp() const
{
    qDebug() << "name \t\t" << getShortName();
    qDebug() << "fileName \t\t" << getBodyFileName();
    qDebug() << "absolutefileName \t" << getAbsoluteBodyFileName();
    qDebug() << "absolutePath \t" << getAbsolutePath();
    qDebug() << "relativePath \t" << getRelativePath() << endl;

    foreach(UvmComponent *comp , children)
        comp->printComp();
}

//Print our name and our components
QString UvmComponent::printTree(int deepness) const
{
    QString tree;

    //Space stuff
    for (int i=0; i<deepness; i++)
        tree.append(" ");

    deepness += 3;

    //Our name and entity name
    tree.append(this->getShortName() + "(" + this->getClassName() +")\n");

    //Append our ports
    foreach (UvmPort *p,ports)
        tree.append(p->printTree(deepness));

    //Append our childs
    foreach (UvmComponent *c,children)
        tree.append(c->printTree(deepness));

    return tree;
}


//Print our name and our components
QString UvmComponent::printHtmlTree(int deepness) const
{
    QString tree;

    //Space stuff
    for (int i=0; i<deepness; i++)
        tree.append("&nbsp;");

    deepness += 3;

    //Our name and entity name
    tree.append(this->getShortName() + "(" + this->getClassName() +")<br>\n");

    //Append our ports
    foreach (UvmPort *p,ports)
        tree.append(p->printHtmlTree(deepness));

    //Append our childs
    foreach (UvmComponent *c,children)
        tree.append(c->printHtmlTree(deepness));

    return tree;
}



UvmVerificationComponent *UvmComponent::getParentVip() const {
    if (!getProject())
        return 0;
    if (!getProject()->getTop())
        return 0;
    // Retrieve the VIP which is our father.
    foreach(UvmVerificationComponent *v, getProject()->getTop()->getVips()) {
        if (v->getAllChildren().contains((UvmComponent *)this)) {
            return v;
            break;
        }
    }
    return 0;
}

QString UvmComponent::getRelativeTemplateFileName() const
{
    return "UvmComponentGetTemplateFileName";
}

QString UvmComponent::getConfigClassName() const
{
    return this->getClassName().toLower()+"_config";
}

bool UvmComponent::hasHeaderFile() const
{
    if (headerFileName.isEmpty())
        return false;
    return QFileInfo(getAbsoluteHeaderFileName()).exists();
}

bool UvmComponent::hasPackage() const
{
    for(int i=0; i<children.length(); i++)
    {
        if(dynamic_cast<UvmPackage*>(children.at(i)) != NULL)
            return true;
    }
    return false;
}

bool UvmComponent::hasConfig() const
{
    for(int i=0; i<children.length(); i++)
    {
        if(dynamic_cast<UvmConfig*>(children.at(i)) != NULL)
            return true;
    }
    return false;
}

bool UvmComponent::hasSequenceItem() const
{
    for(int i=0; i<children.length(); i++)
    {
        if(dynamic_cast<UvmSequenceItem*>(children.at(i)) != NULL)
            return true;
    }
    return false;
}

bool UvmComponent::hasSequence() const
{
    for(int i=0; i<children.length(); i++)
    {
        if(dynamic_cast<UvmSequence*>(children.at(i)) != NULL)
            return true;
    }
    return false;
}

ComponentType UvmComponent::getConnectedComponentType(UvmPort *throughThis)
{
    return throughThis->getConnectedComponentType();
}




static int getComponentIndex(const UvmComponent *comp)
{
    if (dynamic_cast<const UvmPackage *>(comp))
        return 1;
    if (dynamic_cast<const UvmConfig *>(comp))
        return 2;
    if (dynamic_cast<const UvmInterface *>(comp))
        return 3;
    if (dynamic_cast<const UvmSequenceItem *>(comp))
        return 4;
    if (dynamic_cast<const UvmSequence *>(comp))
        return 5;
    if (dynamic_cast<const UvmMonitor *>(comp))
        return 6;
    if (dynamic_cast<const UvmSequencer *>(comp))
        return 7;
    if (dynamic_cast<const UvmVerificationComponent *>(comp))
        return 12;
    if (dynamic_cast<const UvmVirtualSequencer *>(comp))
        return 13;
    if (dynamic_cast<const UvmScoreboard *>(comp))
        return 11;
    if (dynamic_cast<const UvmAgent *>(comp))
        return 10;
    if (dynamic_cast<const UvmCollector *>(comp))
        return 8;
    if (dynamic_cast<const UvmDriver *>(comp))
        return 7;
    if (dynamic_cast<const UvmTestCase *>(comp))
        return 14;
    if (dynamic_cast<const UvmTestbench *>(comp))
        return 16;
    if (dynamic_cast<const UvmDesign *>(comp))
        return 15;
    qCritical() << "Component unknown: " << comp->getShortName();
    return 100;
}

static bool compareComponentsForSort(const UvmComponent *c1, const UvmComponent *c2)
{
    int index1 = getComponentIndex(c1);
    int index2 = getComponentIndex(c2);
    if (index1==index2)
        return c1->getInstName() < c2->getInstName();
    return getComponentIndex(c1) < getComponentIndex(c2);
}

void UvmComponent::sortChildren(bool recursive)
{
    qSort(children.begin(),children.end(),compareComponentsForSort);
    if (recursive)
        foreach(UvmComponent *child,children)
            child->sortChildren(recursive);
}
