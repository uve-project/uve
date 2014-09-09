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
#include "vhdlparser.h"
#include <QDebug>
#include <QFile>
#include <QByteArray>

//Lazy people make lazy software
#define tr(x) QObject::tr(x)

//------------------------ PUBLIC PARSER ---------------------------

//Get the parser instance
VHDLParser::VHDLParser()
{
    _parser = VHDLParserPrivate::getCurrent();
}

//Open the file, and given the data to the parser
int VHDLParser::read(const QString &file)
{
    //Open the file
    QFile f(file);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        _errors = tr("Impossible to read the design file (DUT) \"%1\"").arg(file);
        return -1;
    }

    //Read its content
    QByteArray data = f.peek(f.size());
    f.close();

    //Parse the VHDL
    VHDLParserPrivate *parser = VHDLParserPrivate::getCurrent();
    int code = parser->parse(data.data(),data.size());

    //If something fail, add the message
    if (code != 0)
        _errors = tr("Line %1 : %2").arg(_parser->m_lineError).arg(_parser->m_parserError);

    curFileName = file;
    return code;
}

//Simple getter
QString VHDLParser::errorString() const
{
    return _errors;
}

//Create a list of ports from the parser data
QList<PhysicalPort *> VHDLFileContent::getPorts(const QString &entityName,PhysicalPortList *parent) const
{
    QList<PhysicalPort*> ports;


    VHDLEntity *entity=getEntity(entityName);
    if (!entity)
        return ports;

    for(int i=0; i<entity->nbPorts; i++)
    {
        QString str = entity->portRacine[i].mode;
        PhysicalPort *port;
        if(str.compare("IN") == 0)
            port = UvmPort::newPhysicalPort(entity->portRacine[i].name, UvmPort::DUT, UvmPort::PORT_IN, entity->portRacine[i].type, parent);
        if(str.compare("OUT") == 0)
            port = UvmPort::newPhysicalPort(entity->portRacine[i].name, UvmPort::DUT, UvmPort::PORT_OUT, entity->portRacine[i].type, parent);
        if(str.compare("INOUT") == 0)
            port = UvmPort::newPhysicalPort(entity->portRacine[i].name, UvmPort::DUT, UvmPort::PORT_INOUT, entity->portRacine[i].type, parent);
        port->setSize(entity->portRacine[i].upBit-entity->portRacine[i].lowBit+1);
        ports.append(port);
    }

    return ports;
}


QStringList VHDLFileContent::getEntitiesName() const
{
    QStringList names;
    foreach (VHDLEntity *entity,entities)
        names << entity->entityName;
    return names;
}

VHDLEntity *VHDLFileContent::getEntity(const QString &entityName) const
{
    for(int i=0;i<this->entities.size();i++)
        if (entities.at(i)->entityName == entityName)
            return entities.at(i);
    return 0;
}

//List only the names
QStringList VHDLFileContent::getPortNames(const QString &entityName) const
{
    QStringList names;
    VHDLEntity *entity=getEntity(entityName);
    if (!entity)
        return names;
    for(int i=0; i<entity->nbPorts; i++)
        names.append(entity->portRacine[i].name);
    return names;
}

QString VHDLFileContent::getFileName() const
{
    return fileName;
}


VHDLFileContent *VHDLParser::getVHDLFileContent() const
{
    VHDLFileContent *content = new VHDLFileContent();
    content->fileName = this->curFileName;
    foreach (VHDLEntity *entity,_parser->entities)
    {
        VHDLEntity *newEntity=new VHDLEntity();

        newEntity->nbPorts=entity->nbPorts;
        newEntity->entityName=entity->entityName;
        newEntity->portRacine=entity->portRacine;
        content->entities << newEntity;
    }
    return content;
}


VHDLEntity::VHDLEntity()
{
    nbPorts = 0;
}

//------------------------ PRIVATE PARSER ---------------------------
VHDLParserPrivate *VHDLParserPrivate::singleton;

extern "C" {
    int vhdlparse(char *buf,int size);
}

VHDLParserPrivate::VHDLParserPrivate()
{
    m_parserError=0;
    m_lineError=0;
    globalText=0;
    curEntity=0;
}


// Get the unique current parser
VHDLParserPrivate* VHDLParserPrivate::getCurrent()
{
    if (!singleton)
        singleton = new VHDLParserPrivate();
    return singleton;
}


void VHDLParserPrivate::initPort(VHDLPort &port)
{
    port.name="";
    port.mode="";
    port.type="";
    port.lowBit=0;
    port.direction="";
    port.upBit=0;
    port.lineNum=0;
}

// Parse a buffer of a particular size
int VHDLParserPrivate::parse(char *buf, int size)
{
    m_mutex.lock();

    m_messList.clear();
    m_parserError=0;
    foreach (VHDLEntity *entity,entities)
        delete entity;
    entities.clear();
    nbEntities=0;
    curEntity=0;

    initPort(curPort);


    int res=vhdlparse(buf,size);

    m_mutex.unlock();

    return res;
}


// Set a parser error to a specific line
void VHDLParserPrivate::setParserError(int line)
{
    this->m_parserError=1;
    this->m_lineError=line;
}


// Add a message to the analyser messages list
void VHDLParserPrivate::newMessage(int line, AnalyzerMessage::MessageType messType, QString variable)
{
    AnalyzerMessage *mess=new AnalyzerMessage(line, messType, variable);
    m_messList << mess;
}
