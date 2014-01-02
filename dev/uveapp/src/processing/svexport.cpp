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


#include "svexport.h"
#include "uvmtop.h"
#include "uvesettings.h"
#include "uvmsequence.h"
#include "uvmpackage.h"
#include "current_version.h"


const QString SVExport::templateID = "1";

#define tab QString("    ")

SVExport::SVExport()
{
    int_error=0;
    errorString="";
}

QString SVExport::getErrorString() const {
    return errorString;
}





#include <iostream>
#include "uvexmlimport.h"

bool SVExport::regenerateDir(const QString &dirPath)
{
    QDir dir(dirPath);
    UvmProject *project=0;

    dir.setFilter(QDir::Files);
    {
        QFileInfoList list = dir.entryInfoList();
        for (int i = 0; i < list.size(); ++i) {
            QFileInfo fileInfo = list.at(i);
            if (fileInfo.fileName().indexOf(".uve")!=-1) {
                qDebug() << qPrintable(QString("Regenerating %2")
                                        .arg(fileInfo.fileName()));
                if (project)
                    delete project;
                project = new UvmProject();
                UveXmlImport importer;
                project = importer.loadProject(fileInfo.absoluteFilePath());

                //If there was any errors, show them
                if (!project) {
                    QString erString = "Errors on project load: "+fileInfo.absoluteFilePath()+". Error: "+importer.getErrorString();
                    errorString += erString;
                    qCritical() << erString;
                    return false;
                }
                else {
                    SVExport exporter;
                    exporter.exportProject(project);
                }

            }
        }
    }

    // Recursively look at subdirectories
    dir.setFilter(QDir::Dirs|QDir::NoDotAndDotDot);
    {
        QFileInfoList list = dir.entryInfoList();
        for (int i = 0; i < list.size(); ++i) {
            QFileInfo fileInfo = list.at(i);
            regenerateDir(fileInfo.absoluteFilePath());
        }

    }
    delete project;
    return true;
}




bool SVExport::generateFile(const QString &srcFilename,
                            const QString &dstFilename,
                            QMap<QString,QString> replace)
{
    QFile srcFile(srcFilename);
    QFile dstFile(dstFilename);

    if(!dstFile.open(QIODevice::WriteOnly)) {
        errorString += "Error opening destination file " + dstFile.fileName() + "\n";
        return false;
    }

    if(!srcFile.open(QIODevice::ReadOnly)) {
        errorString += "Error opening source file" + srcFile.fileName() + "\n";
        return false;
    }

    // Truncate the file
    dstFile.resize(0);

    // Write the file, while replacing tags, depending
    while(!srcFile.atEnd()) {
        QString line = srcFile.readLine();
        dstFile.write(parseInLineTags(line,replace).toUtf8());
    }

    srcFile.close();
    dstFile.close();

    return true;
}

#include <iostream>

QString& SVExport::parseInLineTags(QString &line,QMap<QString,QString> replace) {
    // Manage offset, this enable us to skip unknown (here) tags
    int offset = 0;

    // $@XXX@$
    // XXX See below in if (tag)
    QRegExp tag("\\$@(.+)@\\$");
    tag.setMinimal(true);


//    QRegExp tagline("(\\s*)\\$@\\s*(.+)\n");

    QRegExp tagline("(\\s*)\\$@\\s*([\\w_]+)");
//    QRegExp tagline("(\\s*)\\$@\\s*scoreboard_filename\n");


    // $@
    QRegExp spaces("(\\s*)\\$@");
    QString linestart;
    if (spaces.indexIn(line)!=-1)
        linestart=spaces.capturedTexts()[1];

    while((offset = tag.indexIn(line, offset)) != -1) {
        QString key = tag.capturedTexts()[1];

        if (replace.contains(key)) {
            QString newstring=replace.value(key);
//            newstring.replace("\n","\n"+linestart);
            line = line.replace(tag.capturedTexts()[0], newstring );
        }
        else {
            errorString += QString("Did not find the key %1\n").arg(key);
        }

        offset++;
    }

    offset=0;

    while((offset = tagline.indexIn(line, offset)) != -1) {
        QString key = tagline.capturedTexts()[2];
        linestart=tagline.capturedTexts()[1];

        if (replace.contains(key)) {
            QString newstring=replace.value(key);
            newstring=newstring.trimmed();
            newstring.replace("\n","\n"+linestart);
            newstring += "\n";
            line = linestart + newstring;
        }
        else {
            errorString += QString("Did not find the key %1\n").arg(key);
        }

        offset++;
    }

    return line;
}



bool SVExport::exportProject(const UvmProject *project) {

    errorString.clear();
    int_error=0;

    if(!validateTemplate(project))
    {
        errorString = "Templates could not be validated. Export failed. ";
        return false;
    }

    project->getTop()->sortChildren(true);

    QDir().mkpath(project->getFolder());
    QDir().mkpath(project->getTop()->getAbsolutePath());
    foreach (UvmVerificationComponent *vc, project->getTop()->getVips()) {
        QDir().mkpath(project->getFolder() + vc->getRelativePath());
        foreach (UvmAgent *agent, vc->getAgents()) {
            if (agent->toGenerate())
                QDir().mkpath(project->getFolder() + agent->getRelativePath());
        }
    }


    QString srcFile, dstFile;

    QMap<QString,QString> map;

    // todo.txt
    srcFile = QString(project->getTemplatePath() + UVESettings::generator_todo_file());
    dstFile = QString(project->getFolder() + UVESettings::generator_todo_file());
    if(!generateFile(srcFile, dstFile,map))
        return false;

    map.clear();
    map.insert("set_top","set tb_top "+ project->getTop()->getClassName());
    map.insert("set_testcase","set testcase "+ project->getTop()->getTestCase()->getClassName());
    map.insert("set_duv","set duv_lib "+ project->getTop()->getDesign()->getClassName().toLower()+"_lib"+
               "\nset duv_folder \""+project->getTop()->getDesign()->getRelativePath().replace('\\','/')+"\"");

    // main.do
    srcFile = QString(project->getTemplatePath() + UVESettings::generator_main_file());
    dstFile = QString(project->getFolder() + UVESettings::generator_main_file());
    if(!generateFile(srcFile, dstFile,map))
        return false;

    map.clear();

    {
        QString line = "";

        foreach(QString file, project->getVHDLFiles()) {
            QString dstFile("");
/*
            // 1. Find Common part of file path
            QStringList rootComponents = project->getFolder().split(QRegExp("[/:\\\\]"));
            QStringList fileComponents = file.split(QRegExp("[/:\\\\]"));
            int nbCommon = 0;

            while(rootComponents[nbCommon] == fileComponents[nbCommon]) {
                nbCommon++;
            }

            // 2. Prepend "../" as needed to go back from rootComponents to first common folder
            for(int i = nbCommon + 1; i < rootComponents.size(); i++) {
                dstFile += "../";
            }

            // 3. Add File Specific path components
            for(int i = nbCommon; i < fileComponents.size(); i++) {
                dstFile += fileComponents[i];

                if (i + 1 < fileComponents.size()) {
                    dstFile += "/";
                }
            }
*/
            dstFile = file;
            line += "vcom -work $duv_lib \"" + dstFile.replace('\\','/') + "\" -mixedsvvh";
            line += "\n";
        }

        map.insert("vcom",line);
    }

    // compile_vhdl.do
    srcFile = QString(project->getTemplatePath() + UVESettings::generator_compile_vhdl_file());
    dstFile = QString(project->getFolder() + UVESettings::generator_compile_vhdl_file());
    if(!generateFile(srcFile, dstFile,map))
        return false;

    map.clear();
    {
        QString newLine;
        newLine = "vlog -work $tb_lib ";

        foreach(UvmVerificationComponent *vip, project->getTop()->getVips()) {
            newLine += " +incdir+" + vip->getRelativePath().replace('\\','/');
            foreach(UvmAgent *agent, vip->getAgents()) {
                newLine += " +incdir+" + agent->getRelativePath().replace('\\','/');
            }
        }
        newLine += " +incdir+";
        newLine += project->getTop()->getRelativePath().replace('\\','/') + " ";

        QString line;

        line = newLine;

        line += project->getTop()->getRelativeBodyFileName().replace('\\','/');
        line += "\n";

        line += newLine;
        line += project->getTop()->getTestCase()->getRelativeBodyFileName().replace('\\','/');
        line += "\n";

        map.insert("vlog",line);

    }

    // compile_sv.do
    srcFile = QString(project->getTemplatePath() + UVESettings::generator_compile_sv_file());
    dstFile = QString(project->getFolder() + UVESettings::generator_compile_sv_file());
    if(!generateFile(srcFile, dstFile,map))
        return false;

    map.clear();

    {
        QString line;

        foreach(UvmVerificationComponent *vip, project->getTop()->getVips()) {

            UvmInterface *interface = vip->getInterface();
            QString interfaceName = vip->getClassName() + "_if";

            // Divider
            line += "add wave -noupdate -divider ";
            line += vip->getClassName() + "::" + vip->getInstName();
            line += "\n";

            foreach (PhysicalPort *port,interface->getPhysicalPorts()) {
                line += addWave(project->getTop(),interfaceName, port->getName(), false);
            }
        }
        map.insert("add_wave",line);
    }

    // wave.do
    srcFile = QString(project->getTemplatePath() + UVESettings::generator_wave_file());
    dstFile = QString(project->getFolder() + UVESettings::generator_wave_file());
    if(!generateFile(srcFile, dstFile,map))
        return false;

    map.clear();
    {
        QString line;
        //Append a last / to the simulator if not already here
        QString simulatorPath = QDir::toNativeSeparators( project->getSimulator() );
        if ( ! simulatorPath.endsWith( QDir::separator() ) )
            simulatorPath += QDir::separator();

        line = "set simulator=";
        line += simulatorPath;
        map.insert("set_simulator_win_path",line);
    }


    // run.bat
    srcFile = QString(project->getTemplatePath() + UVESettings::generator_run_bat_file());
    dstFile = QString(project->getFolder() + UVESettings::generator_run_bat_file());
    if(!generateFile(srcFile, dstFile,map))
        return false;

    map.clear();
    {
        QString line;
        //Append a last / to the simulator if not already here
        QString simulatorPath = QDir::toNativeSeparators(project->getSimulator() );
        if ( ! simulatorPath.endsWith( QDir::separator() ) )
             simulatorPath += QDir::separator();

        line = "SIMULATOR=";
        line += simulatorPath;
        map.insert("set_simulator_ux_path",line);
    }
    // run.sh
    srcFile = QString(project->getTemplatePath() + UVESettings::generator_run_sh_file());
    dstFile = QString(project->getFolder() + UVESettings::generator_run_sh_file());
    if(!generateFile(srcFile, dstFile,map))
        return false;

    // Ensure run.sh is executable on a unix platform...
    QFile(dstFile).setPermissions(QFile::ExeUser|QFile::ExeGroup|QFile::ExeOther|
                                  QFile::ReadUser|QFile::ReadGroup|QFile::ReadOther|
                                  QFile::WriteUser);

    project->getTop()->accept(this);
    if ((int_error) || (!errorString.isEmpty()))
        return false;
    else
        return true;
}

const QString SVExport::addWave(UvmComponent *comp,QString vif, QString sig, bool inHexa) const {
    QString wave = "add wave -noupdate ";
    QString radixHexa = "";

    if(inHexa)
        radixHexa = "-radix hexadecimal ";

    return wave + radixHexa + comp->getClassName() + "/" + vif + "/" + sig + "\n";
}

void SVExport::initMap(QMap<QString,QString> &map,const UvmComponent *comp) const
{
    QFile dstFile(comp->getAbsoluteBodyFileName());
    QFileInfo info(dstFile);
    QString generatedFile=info.baseName().toUpper();
    map.insert("FILENAME",generatedFile + "_SV");
    map.insert("filename",comp->getBodyFileName());
    map.insert("ClassName",comp->getClassName());
    map.insert("class_name", comp->getClassName().toLower());
    map.insert("projectname",comp->getProject()->getName());
    map.insert("date",QDateTime::currentDateTime().toString(Qt::ISODate));
    map.insert("uve_version", UVE_VERSION);
    if (comp->getParentVip() != 0) {
        map.insert("vip_interface", comp->getParentVip()->getInterface()->getClassName());
        // TODO: Check why we need this check for existing VIPs
        if (comp->getParentVip()->getSequenceItem())
            map.insert("vip_transfer",comp->getParentVip()->getSequenceItem()->getClassName());
    }

    QString line;
    line="";
    foreach(UvmVerificationComponent *vip, comp->getProject()->getTop()->getVips()) {
            line += "`include \"" + vip->getInterface()->getBodyFileName() + "\"\n";
    }
    map.insert("include_vip_if",line);


    line="";
    foreach(UvmVerificationComponent *vip, comp->getProject()->getTop()->getVips()) {
            line += "`include \"" + vip->getPackageFileName() + "\"\n";
    }
    map.insert("include_vip_pkg",line);

    line = "";

    foreach(UvmVerificationComponent *vip, comp->getProject()->getTop()->getVips()) {
        line += "import " + vip->getClassName() + "_pkg::*;\n";
    }

    map.insert("import_pkg",line);

}

void SVExport::generateFile(const UvmComponent *comp,QMap<QString,QString> &map) {
    QString srcFile, dstFile;

    srcFile = comp->getProject()->getTemplatePath() + comp->getRelativeTemplateFileName();
    dstFile = comp->getAbsoluteBodyFileName();
    if(!generateFile(srcFile, dstFile,map))
        int_error=1;
}

void SVExport::visit(const UvmTop *comp){
    if (!comp->toGenerate())
        return;
    QMap<QString,QString> map;
    initMap(map,comp);

    map.insert("include_testcase",
               QString("`include \"%1\";").arg(comp->getTestCase()->getBodyFileName().replace('\\','/')));


    QList<PhysicalPort*> ports = comp->getDesign()->getPhysicalPorts();

    QList<PhysicalPort*> multiConnectPorts;
    foreach(PhysicalPort *p, ports) {
        if ((p->getMode() == UvmPort::DUT) || (p->getMode() == UvmPort::GLOBAL)) {
            if ((p->getDirection() == UvmPort::IN) &&
                    ((p->getCategory() != UvmPort::CLOCK)&&
                     (p->getCategory() != UvmPort::RESET)))
                continue;
            if (p->getConnections().size()>1)
            {
                multiConnectPorts << p;
            }
        }
    }

    QString line;

    line = "";

    QList<PhysicalPort*> signalsToDeclare;
    foreach(PhysicalPort *p, multiConnectPorts)
        signalsToDeclare << p;
    foreach(PhysicalPort *p, ports)
        if (p->getConnections().size() == 0)
             signalsToDeclare << p;

    foreach(PhysicalPort *p, signalsToDeclare) {
        if ((p->getType().compare("std_logic")==0) ||
                (p->getType().compare("std_ulogic")==0) ||
                (p->getType().compare("std_logic_vector")==0) ||
                (p->getType().compare("std_ulogic_vector")==0) ||
                (p->getType().compare("")==0))
        {
            line += "logic ";

            if (p->getSize() > 1) {
                line += QString("[%1:0] ").arg(p->getSize() - 1);
            }
        }
        else {
            line+="/*---------------------------------------------------------------------------\n";
            line+=" * @TODO : Define this type \""+p->getType()+"\" in a package\n";
            line+=" * This type is used in the VHDL design, but should be declared as being\n";
            line+=" * compatible with SystemVerilog\n";
            line+=" *--------------------------------------------------------------------------*/\n";
            line += p->getType()+" ";
        }

        line += p->getName() + ";\n";
    }
    map.insert("decl_signals",line);

    line="";
    line += comp->getDesign()->getClassName() + " duv(\n";

    // SV Is picky about comma, so we have to skip the first one!
    bool first_port = true;

    foreach(PhysicalPort *p, ports) {
        if ((p->getMode() == UvmPort::DUT) || (p->getMode() == UvmPort::GLOBAL)) {

            if (!first_port) {
                line += ",\n";
            }

            first_port = false;

            line += tab;
            line += "." + p->getName();
            if (signalsToDeclare.contains(p))
            {
                line += " ("+p->getName()+")";
            }
            else {
                UvmPort *otherPort=p->getConnections().at(0)->getOtherPort(p);
                line += " (";
                line += otherPort->getParent()->getParentVip()->getClassName() + "_if." + otherPort->getName();
                line += ")";
            }
        }
    }

    line += "\n";
    line += ");\n";

    map.insert("duv_connection",line);

    line = "";


    foreach(PhysicalPort *p, multiConnectPorts) {
        foreach (UvmConnection *connection, p->getConnections())
        {
            UvmPort *otherPort = connection->getOtherPort(p);
            line += "assign " + otherPort->getParent()->getParentVip()->getClassName() + "_if" + "." + otherPort->getName() +
                    " = " + p->getName()
                    + ";\n";
        }
    }
    map.insert("interface_connection",line);


    line = "";

    // Generate Clock blocks
    foreach(UvmVerificationComponent *vip, comp->getVips()) {
        UvmInterface *interface = vip->getInterface();
        QString interfaceName = vip->getClassName() + "_if";
        foreach(PhysicalPort *clock, interface->getClocks()) {
            if (clock->getConnections().size()==0)
            {
                line += "initial begin\n";

                line += tab;
                line += interfaceName + "." + clock->getName() + " <= 1'b0;\n";

                line += tab;
                line +="forever\n";

                line += tab;
                line += tab;
                line += "#" + QString("%1").arg(clock->getClockHalfPeriod()) + " ";
                line += interfaceName + "." + clock->getName();
                line += " = ~" + interfaceName + "." + clock->getName() + ";\n";

                line += "end\n\n";
            }
        }
    }

    // Generate Clock blocks
    foreach(PhysicalPort *port, ports) {
        if (port->getCategory() != UvmPort::CLOCK)
            continue;
        QString name;
        if (signalsToDeclare.contains(port))
            name = port->getName();
        else
            name = port->getConnections().at(0)->getOtherPort(port)->getParent()->getParentVip()->getClassName() + "_if"
                    + "." + port->getName();
        line += "initial begin\n";

        line += tab;
        line += name + " <= 1'b0;\n";

        line += tab;
        line +="forever\n";

        line += tab;
        line += tab;
        line += "#" + QString("%1").arg(port->getClockHalfPeriod()) + " ";
        line +=name + " = ~" + name + ";\n";

        line += "end\n\n";
    }

    // Generate Reset blocks
    foreach(UvmVerificationComponent *vip, comp->getVips()) {
        UvmInterface *interface = vip->getInterface();
        foreach(PhysicalPort *reset, interface->getResets()) {
            if (reset->getConnections().size()==0) {
                // Initial state
                line += "initial begin\n";
                line += tab;
                line += interface->getInstName() + "." + reset->getName() + " <= 1'b";
                line += (reset->getResetActiveLow() ? "0" : "1");
                line += ";\n";

                // Change state after delay
                line += tab;
                line += "#" + QString("%1").arg(reset->getResetDuration()) + " ";
                line += interface->getInstName() + "." + reset->getName() + " <= 1'b";
                line += (reset->getResetActiveLow() ? "1" : "0");
                line += ";\n";

                line += "end\n\n";
            }
        }
    }

    // Generate Reset blocks
    foreach(PhysicalPort *reset, ports) {
        if (reset->getCategory() != UvmPort::RESET)
            continue;
        QString name;
        if (signalsToDeclare.contains(reset))
            name = reset->getName();
        else
            name = reset->getConnections().at(0)->getOtherPort(reset)->getParent()->getParentVip()->getClassName() + "_if"
                    + "." + reset->getName();

        // Initial state
        line += "initial begin\n";
        line += tab;
        line += name + " <= 1'b";
        line += (reset->getResetActiveLow() ? "0" : "1");
        line += ";\n";

        // Change state after delay
        line += tab;
        line += "#" + QString("%1").arg(reset->getResetDuration()) + " ";
        line += name + " <= 1'b";
        line += (reset->getResetActiveLow() ? "1" : "0");
        line += ";\n";

        line += "end\n\n";
    }


    map.insert("clockAndReset",line);

    line = "";
    foreach(UvmVerificationComponent *vip, comp->getVips()) {
        line +=QString("uvm_config_db#(virtual %1)::set(uvm_root::get(), \"*\", \"%1\", %2_if);\n").arg(vip->getInterface()->getClassName()).arg(vip->getClassName());
    }
    map.insert("set_interface",line);

    line="";


    foreach(UvmVerificationComponent *vip, comp->getVips()) {
        line +=QString("%1 %2_if();\n").arg(vip->getInterface()->getClassName()).arg(vip->getClassName());
    }
    map.insert("decl_interface",line);

    generateFile(comp,map);
}

void SVExport::visit(const UvmTestCase *comp){
    if (!comp->toGenerate())
        return;
    QMap<QString,QString> map;
    initMap(map,comp);

    map.insert("gtb_tb_filename",comp->getTestbench()->getBodyFileName());
    map.insert("gtb_tb_class",comp->getTestbench()->getClassName());
    map.insert("gtb_tb_inst",comp->getTestbench()->getInstName());

    QString line;
    line = "";
    foreach (UvmVerificationComponent *vip,comp->getProject()->getTop()->getVips()) {
        line += "uvm_config_db#(uvm_object_wrapper)::set\n"
          "   (this,\"*."+vip->getInstName() + ".*.sequencer.run_phase\",\"default_sequence\",\n"
          +"    "+ vip->getClassName() + "_simple_seq_do::type_id::get());\n";
    }
    map.insert("def_sequences",line);

    line = "";
    foreach (UvmVerificationComponent *vip,comp->getProject()->getTop()->getVips()) {
        line += "begin\n";
        line += tab + "// Create the configuration for " + vip->getConfigClassName() + "\n";
        line += tab + vip->getConfigClassName() + " " + "cfg;\n";
        line += tab + "cfg = " + vip->getConfigClassName() + "::type_id::create(\"cfg_"+vip->getConfigClassName()+"\");\n";
        if (!vip->getScoreboards().isEmpty())
        {
            line += tab + "// VIP Scoreboard attributes\n";
            line += tab + "cfg.disable_scoreboard = 0;\n";
        }
        if (vip->getBusMonitor())
        {
            line += tab + "cfg.has_bus_monitor = 1;\n";
            line += tab + "cfg.bus_monitor_checks_enable = 1;\n";
            line += tab + "cfg.bus_monitor_coverage_enable = 1;\n";
        }
        foreach (UvmAgent *agent,vip->getAgents()) {
            line += tab + "// Create the array of agents configurations for " + agent->getClassName().toLower() + "\n";
            line += tab + "cfg.nb_" + agent->getClassName().toLower() + "_config = " + QString::number(agent->getNbAgents()) + ";\n";
            line += tab + "cfg." + agent->getClassName().toLower() + "_cfg = new[" + QString::number(agent->getNbAgents()) + "];\n";
            for(int i=0;i<agent->getNbAgents();i++)
            {
                line += tab + "// Create the configuration for " + agent->getClassName().toLower() + "[" + QString::number(i) + "]\n";
                QString startOfLine = tab + "cfg." + agent->getClassName().toLower() + "_cfg[" + QString::number(i) + "]";
                line += startOfLine + " = " + agent->getConfigClassName() + "::type_id::create(\"cfg_" + agent->getConfigClassName()+"_"+QString::number(i) + "\");\n";

                line += tab + "// Monitor fields\n";
                line += startOfLine + ".monitor_checks_enable = 1;\n";
                line += startOfLine + ".monitor_coverage_enable = 1;\n";

                if (agent->getCollector() != NULL) {
                    line += tab + "// Collector fields\n";
                    line += startOfLine + ".collector_checks_enable = 1;\n";
                    line += startOfLine + ".collector_coverage_enable = 1;\n";
                }
            }
        }

        line += tab + "// Apply the configuration to the VIP\n";
        line += tab + "uvm_config_db#(" + vip->getConfigClassName()+ ")::set\n"
          + tab + tab + "(this,\"*."+vip->getInstName() + "\",\"config\",\n"
          + tab + tab + "cfg);\n";
        line += "end\n";
    }
    map.insert("def_configs",line);


    line = "";
    foreach (UvmVerificationComponent *vip,comp->getProject()->getTop()->getVips()) {
        UvmMonitor *busMonitor = vip->getBusMonitor();
        if (busMonitor)
        line += "if (" + comp->getTestbench()->getInstName() + "." + vip->getInstName() + "."+busMonitor->getInstName()+" != null)\n"
          +"    "+comp->getTestbench()->getInstName() + "." + vip->getInstName() + "."+busMonitor->getInstName()+".set_report_verbosity_level(UVM_FULL);\n";
    }

    map.insert("bus_monitor_verbosity",line);

    generateFile(comp,map);
}


void SVExport::visit(const UvmTestbench *comp){
    if (!comp->toGenerate())
        return;
    QMap<QString,QString> map;
    initMap(map,comp);


    QString line;

    line  = "";
    foreach(UvmScoreboard *sc, comp->getScoreboards()) {
        line += QString("`include \"%1\"\n").arg(sc->getBodyFileName());
    }
    map.insert("include_scoreboard",line);

    line  = "";

    int i=0;
    foreach(UvmScoreboard *sc, comp->getScoreboards()) {
        line += sc->getClassName() + " " + sc->getInstName() + ";\n";
        i++;
    }

    map.insert("decl_tb_components",line);

    line  = "";
    i=0;
    foreach(UvmScoreboard *sc, comp->getScoreboards()) {
        line += "// Build scoreboard\n";
        line += sc->getInstName() + " = ";
        line += sc->getClassName();
        line += "::type_id::create(\"" + sc->getInstName() + "\", this);\n";
        i++;
    }
    map.insert("tb_build_components",line);

    line = "";
    if (comp->getScoreboards().size()!=0)
    {
        foreach (UvmVerificationComponent *vip, comp->getVerificationComponents()) {
            foreach (UvmAgent *agent,vip->getAgents()) {
                for(int i=0;i<agent->getNbAgents();i++) {
                    line += vip->getInstName() + "." + agent->getInstName()+"["+QString("%1").arg(i)+"].monitor.item_collected_port.connect("+comp->getScoreboards().at(0)->getInstName()+"."+scoreboardAgentPortName(agent,i)+"_port);\n";
                }
            }
        }
    }

    map.insert("connect_components",line);

    line = "";
    foreach (UvmVerificationComponent *vip, comp->getVerificationComponents()) {
        line += vip->getClassName() + " " + vip->getInstName() + ";\n";
    }
    map.insert("decl_vips",line);


    line = "";
    foreach (UvmVerificationComponent *vip, comp->getVerificationComponents()) {
        line += vip->getInstName() + " = " + vip->getClassName() + "::type_id::create(\"" +
                vip->getInstName() + "\", this);\n";
    }
    map.insert("create_vips",line);

    generateFile(comp,map);
}

void SVExport::visit(const UvmScoreboard *comp){
    if (!comp->toGenerate())
        return;
    QMap<QString,QString> map;
    initMap(map,comp);

    QString line;


    // If the scoreboard is a VIP scoreboard, then we do not import the VIPs packages
    // It basically erase what was put in initMap(map,comp)
    if (comp->getParentVip())
        map.insert("import_pkg","");

    line = "";
    if (comp->getParentVip())
    {
        line += "// Try to get a configuration object\n";
        line += "if (!uvm_config_db#("+comp->getParentVip()->getConfigClassName()+
                ")::get(this, \"\", \"config\", cfg))\n";
        line += tab +"`uvm_fatal(\"GETCONFIGFAIL\", \"Failed to get the scoreboard configuration\");\n";
        line += "disable_scoreboard = cfg.disable_scoreboard;\n";
    }
    map.insert("get_config",line);

    line = "";
    if (comp->getParentVip())
    {
        line += comp->getParentVip()->getConfigClassName()+ " cfg;\n";
    }

    map.insert("decl_config",line);

    QList<UvmVerificationComponent *> vips;
    if (comp->getParentVip())
        vips.append(comp->getParentVip());
    else
        vips=comp->getProject()->getTop()->getVips();


    bool hasTransformer=false;
    QString seq_item_class1="";
    QString seq_item_class2="";
    foreach (UvmVerificationComponent *vip, vips) {
        foreach (UvmAgent *agent, vip->getAgents()) {
            for (int i=0;i<agent->getNbAgents();i++) {
                if (seq_item_class1.isEmpty())
                    seq_item_class1 = vip->getSequenceItem()->getClassName();
                if (seq_item_class1 != vip->getSequenceItem()->getClassName())
                    if (seq_item_class2.isEmpty()) {
                        seq_item_class2 = vip->getSequenceItem()->getClassName();
                        hasTransformer = true;
                    }
            }
        }
    }



    line = "";

    if (comp->getComparatorType()!=UvmScoreboard::ONLYCOMPARATOR)
    {
        int agent_nb = 0;
        foreach (UvmVerificationComponent *vip, vips) {
            foreach (UvmAgent *agent, vip->getAgents()) {
                for (int i=0;i<agent->getNbAgents();i++) {
                    line += "// Implementation of the write method of the ";
                    line += vip->getClassName() + "_port\n";
                    line += "virtual function void write_"+scoreboardAgentPortName(agent,i)+"_pkt("+vip->getSequenceItem()->getClassName()+" trans);\n";
                    line += tab + "if(!disable_scoreboard) begin\n";
                    line += tab + "/*---------------------------------------------------------------------------\n";
                    line += tab + "* @TODO : Implement functional checks. The bit sbd_error must be set to 1\n";
                    line += tab + "* if an error occur. The test cannot be succeed if this bit is set to 1.\n";
                    line += tab + "*--------------------------------------------------------------------------*/\n";
                    line += tab + "// Display transfer\n";
                    line += tab + "`uvm_info(get_type_name(),\n";
                    line += tab + "$psprintf(\"Transfer collected :\\n\%s\", trans.sprint()),\n";
                    line += tab + "          UVM_HIGH)\n";
                    if (comp->hasComparator())
                    {
                        line += tab + "\n";
                        if (agent_nb==0)
                        {
                            line += tab + "// Send transfer to the comparator\n";
                            line += tab + "comparator.before_export.write(trans);\n\n";
                        }
                        else if (agent_nb == 1)
                        {
                            line += tab + "// Send transfer to the comparator\n";
                            line += tab + "comparator.after_export.write(trans);\n\n";

                        }
                        else
                        {
                            line += tab + "// TODO: Send transfer to the comparator\n";
                            line += tab + "// comparator.before_export.write(trans);\n\n";
                            line += tab + "// comparator.before_export.write(trans);\n\n";
                        }
                        agent_nb ++;
                    }

                    line += tab + "end\n";
                    line += "endfunction : write_"+scoreboardAgentPortName(agent,i)+"_pkt\n\n";
                }
            }
        }
    }
    map.insert("write_methods",line);

    line = "";
    foreach (UvmVerificationComponent *vip, vips) {
        foreach (UvmAgent *agent, vip->getAgents()) {
            for (int i=0;i<agent->getNbAgents();i++) {
                QString name = scoreboardAgentPortName(agent,i) + "_port";
                line +=  name + " = "
                        "new(\""+name+"\", this);\n";
            }
        }
    }
    if (hasTransformer) {
        line += "transformer = new;\n";
        if (comp->hasComparator()) {
            line += "comparator = new(\"Comparator\",this,transformer);\n";
        }
    }
    else if (comp->hasComparator()) {
        line += "comparator = new(\"Comparator\",this);\n";
    }
    map.insert("create_ports",line);




    line = "";
    if (comp->getComparatorType()!=UvmScoreboard::ONLYCOMPARATOR)
        foreach (UvmVerificationComponent *vip, vips) {
            foreach (UvmAgent *agent, vip->getAgents()) {
                for (int i=0;i<agent->getNbAgents();i++) {
                    QString name = scoreboardAgentPortName(agent,i);
                    line += "`uvm_analysis_imp_decl(_"+name+"_pkt)\n";
                }
            }
        }
    map.insert("decl_ports_types",line);


    line = "";

    foreach (UvmVerificationComponent *vip, vips) {
        foreach (UvmAgent *agent, vip->getAgents()) {
            for (int i=0;i<agent->getNbAgents();i++) {
                if (comp->getComparatorType()!=UvmScoreboard::ONLYCOMPARATOR)
                    line += "uvm_analysis_imp_"+scoreboardAgentPortName(agent,i)+"_pkt#("+vip->getSequenceItem()->getClassName()+", "+comp->getClassName()+") "+scoreboardAgentPortName(agent,i)+"_port;\n";
                else
                    line += "uvm_analysis_export#("+vip->getSequenceItem()->getClassName()+") "+scoreboardAgentPortName(agent,i)+"_port;\n";
            }
        }
    }

    if (comp->hasComparator()) {
        line += "\n";

        if (hasTransformer) {

            line += "class transformer_t;\n";
            line += tab + "// TODO : We assumed an input and an output class, but you have to choose the \n";
            line += tab + "//        right direction\n";
            line += tab + "function "+seq_item_class2+ " transform ("+seq_item_class1+" trans);\n";
            line += tab + tab + seq_item_class2 + " result=new;\n";
            line += tab + tab + "// TODO Implement the translation\n";
            line += tab + tab + "return result;\n";
            line += tab + "endfunction : transform\n";
            line += "endclass : transformer_t\n";
            line += "\n";
            line += "transformer_t transformer;\n";
            line += "\n";
            line += "typedef uvm_algorithmic_comparator#("+seq_item_class1+","+seq_item_class2+",transformer_t) comparator_t;\n";
            line += "comparator_t comparator;\n";
        }
        else
        {

            line += "typedef uvm_in_order_class_comparator#("+seq_item_class1+") comparator_t;\n";
            line += "comparator_t comparator;\n";

        }
    }

    map.insert("decl_ports",line);


    line = "";
    if (comp->getComparatorType()==UvmScoreboard::ONLYCOMPARATOR) {
        line += "function void connect_phase(uvm_phase phase);\n";

        int number = 0;
        foreach (UvmVerificationComponent *vip, vips) {
            foreach (UvmAgent *agent, vip->getAgents()) {
                for (int i=0;i<agent->getNbAgents();i++) {
                    if (number == 0)
                        line += tab + scoreboardAgentPortName(agent,i) + "_port"+".connect(comparator.before_export);\n";
                    else if (number == 1)
                        line += tab + scoreboardAgentPortName(agent,i) + "_port"+".connect(comparator.after_export);\n";
                    else {
                        line += tab + "// It seems that you have more than two analysis ports. You should choose how\n";
                        line += tab + "// to connect them to the comparator\n";
                    }
                    number ++;
                }
            }
        }
        line += "endfunction\n";

    }

    map.insert("connect_phase",line);

    generateFile(comp,map);
}

QString SVExport::scoreboardAgentPortName(UvmAgent *agent,int index) const
{
    return agent->getParentVip()->getClassName().toLower() + "_" +
                            agent->getClassName().toLower() +
                            QString("%1").arg(index);
}


bool SVExport::cpDir(const QString &srcPath, const QString &dstPath)
{
 //   rmDir(dstPath);
    if (!QDir().mkpath(dstPath))
        return false;

    QDir srcDir(srcPath);
    foreach(const QFileInfo &info, srcDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
        QString srcItemPath = srcPath + "/" + info.fileName();
        QString dstItemPath = dstPath + "/" + info.fileName();
        if (info.isDir()) {
            if (!cpDir(srcItemPath, dstItemPath)) {
                return false;
            }
        } else if (info.isFile()) {
            if (QFile::exists(dstItemPath))
                QFile::remove(dstItemPath);
            if (!QFile::copy(srcItemPath, dstItemPath)) {
                return false;
            }
        } else {
            qDebug() << "Unhandled item" << info.filePath() << "in cpDir";
        }
    }
    return true;
}

void SVExport::visit(const UvmVerificationComponent *comp){
    if (!comp->toGenerate()) {
        // This is a VIP from a library. Simply copy the folder
        if (!cpDir(comp->getOriginalVipPath(),
                   comp->getProject()->getFolder() + comp->getVipRelativePath())) {
            errorString += "Can not copy the VIP folder\n";
            return;
        }
        return;
    }

    QMap<QString,QString> map;
    initMap(map,comp);

    QString line;

    line  = "";

    if (comp->getBusMonitor() != NULL) {
        line += comp->getBusMonitor()->getClassName() + " bus_monitor;\n";
    }

    foreach(UvmScoreboard *scoreboard,comp->getScoreboards()) {
        line += scoreboard->getClassName() + " " + scoreboard->getInstName() + ";\n";
    }

    foreach(UvmAgent *agent, comp->getAgents()) {
        line += agent->getClassName() + " "+ agent->getInstName() + "[];\n";
    }

    map.insert("vip_components",line);

    line="";
    foreach(UvmAgent *agent, comp->getAgents()) {
        line += "protected int nb_agent_"+ agent->getInstName() + " = 0;\n";
    }

    map.insert("numbers",line);

    line="";
    if (comp->getParentVip()) {
        line = QString("%1").arg(comp->getParentVip()->getAgents().count());
    }
    else
        line="0";

    map.insert("nb_agents",line);

    line="";
    foreach(UvmAgent *agent, comp->getAgents()) {
        line += "`uvm_field_int(nb_agent_"+ agent->getInstName() + ", UVM_DEFAULT)\n";
    }

    map.insert("comp_utils",line);


    line  = "";
    foreach(UvmAgent *agent, comp->getAgents()) {
        QString agent_num = "nb_agent_"+ agent->getInstName();
        line += QString(agent_num + " = %1;\n").arg(agent->getNbAgents());
        line += agent_num + " = cfg.nb_" + agent->getConfigClassName() + ";\n";
//        line += "// Get number of agents of type " + agent->getClassName() + "\n";
//        line += "void'(uvm_config_db#(int)::get(this, \"\", \""+agent_num+"\", "+agent_num+"));\n";
//        line += "\n";

        line += agent->getInstName() + " = new["+ agent_num + "];\n";

        line += "// Build agent(s) of type "+ agent->getClassName() + "\n";
        line += "for(int i = 0; i < "+ agent_num +"; i++) begin\n";
        line += tab+"$sformat(inst_name, \""+ agent->getInstName()+"[%0d]\", ";
        line += "i);\n";
        line += tab + agent->getInstName()+"[";
        line += "i] = ";
        line += agent->getClassName();
        line += "::type_id::create(inst_name,this);\n";
        line += tab + "void'(uvm_config_db#(int)::set(this,{inst_name,\"*\"}, \"master_id\", i));\n";
        line += tab + "void'(uvm_config_db#("+agent->getConfigClassName()+")::set(this,{inst_name,\"*\"}, \"config\", cfg." + agent->getClassName().toLower() + "_cfg[i]));\n";
        line += "end\n";
        line += "\n";
    }

    line += "has_bus_monitor = cfg.has_bus_monitor;\n";
    if (comp->getBusMonitor()!=0) {
        line += "// Build bus monitor\n";
        line += "if(has_bus_monitor == 1) begin\n";
        line += tab + "bus_monitor = ";
        line += comp->getBusMonitor()->getClassName();
        line += "::type_id::create(\"bus_monitor\", this);\n";
        line += "end\n";

    }

    foreach(UvmScoreboard *scoreboard,comp->getScoreboards()) {
        line += "// Build scoreboard\n";
        line += scoreboard->getInstName() +" = ";
        line += scoreboard->getClassName();
        line += "::type_id::create(\""+scoreboard->getInstName()+"\", this);\n";
        line += "void'(uvm_config_db#("+comp->getConfigClassName()+")::set(this,\""+scoreboard->getInstName()+"\", \"config\", cfg));\n";
    }

    if (comp->getBusMonitor()!=0)
    {
        line += "void'(uvm_config_db#(" + comp->getConfigClassName()+ ")::set(this,\"bus_monitor\",\"config\",cfg));\n";

    }

    map.insert("vip_build_components",line);


    line = "";

    if (!comp->getScoreboards().isEmpty()) {
        foreach (UvmAgent *agent,comp->getAgents()) {
            for(int i=0;i<agent->getNbAgents();i++)
                line += agent->getInstName()+"["+QString("%1").arg(i)+"].monitor.item_collected_port.connect("+comp->getScoreboards().at(0)->getInstName()+"."+scoreboardAgentPortName(agent,i)+"_port);\n";
        }
    }

    map.insert("vip_connect_phase",line);

    generateFile(comp,map);



    exportConfig(comp);

}


void SVExport::visit(const UvmAgent *comp){
    if (!comp->toGenerate())
        return;

    QMap<QString,QString> map;
    initMap(map,comp);

    QString line;

    line  = "";

    if (comp->getSequencer() != NULL) {
        line += comp->getSequencer()->getClassName() + " " + comp->getSequencer()->getInstName() + ";\n";
    }

    if (comp->getDriver() != NULL) {
        line += comp->getDriver()->getClassName() + " " + comp->getDriver()->getInstName() + ";\n";
    }

    if (comp->getMonitor() != NULL) {
        line += comp->getMonitor()->getClassName() + " " + comp->getMonitor()->getInstName() + ";\n";
    }

    if (comp->getCollector() != NULL) {
        line += comp->getCollector()->getClassName() + " " + comp->getCollector()->getInstName() + ";\n";
    }

    map.insert("decl_components",line);


    line  = "";
    if ((comp->getSequencer() != 0) && (comp->getDriver() != 0)){
        line += "if (cfg.is_active == UVM_ACTIVE)\n";
        line += "begin\n";
        if (comp->getSequencer() != NULL) {
            line += tab + "sequencer = ";
            line += comp->getSequencer()->getClassName();
            line += "::type_id::create(\"sequencer\", this);\n";
        }

        if (comp->getDriver() != NULL) {
            line += tab + "driver = ";
            line += comp->getDriver()->getClassName();
            line += "::type_id::create(\"driver\", this);\n";
        }
        line += "end\n";
    }

    if (comp->getMonitor() != NULL) {
        line += "monitor = ";
        line += comp->getMonitor()->getClassName();
        line += "::type_id::create(\"monitor\", this);\n";
    }

    if (comp->getCollector() != NULL) {
        line += "collector = ";
        line += comp->getCollector()->getClassName();
        line += "::type_id::create(\"collector\", this);\n";
    }

    map.insert("build_components",line);

    line  = "";

    if((comp->getCollector() != NULL) && (comp->getMonitor() != NULL)) {
        line += "collector.item_collected_port.connect(monitor.coll_mon_port);\n";
    }

    if(comp->getType() == UvmAgent::ACTIV) {
        line += "\n";

        line += "if (cfg.is_active == UVM_ACTIVE)\n";
        line += "begin\n";
        line += tab + "// This is an Activ agent, so connect sequencer and monitor\n";

        if((comp->getDriver() != NULL) && (comp->getSequencer() != NULL)) {
            line += tab + "driver.seq_item_port.connect(sequencer.seq_item_export);\n";
        }

        if((comp->getSequencer() != NULL) && (comp->getMonitor() != NULL)) {
            line += tab + "sequencer.seq_item_port.connect(monitor.seq_item_imp);\n";
        }

        if (comp->getDriver() && comp->getMonitor()) {
            line += tab + "if (driver.send_to_monitor)\n";
            line += tab + tab + "driver.mon_item_port.connect(monitor.mon_item_imp);\n";
        }
        line += "end\n";
    }

    map.insert("connect_components",line);

    generateFile(comp,map);

}


void SVExport::visit(const UvmDriver *comp){
    if (!comp->toGenerate())
        return;
    QMap<QString,QString> map;
    initMap(map,comp);

    QString line;

    map.insert("agent_class_name",comp->getParent()->getClassName().toLower());

    line = "";
    foreach(PhysicalPort *p, comp->getParentVip()->getInterface()->getPhysicalPorts()) {
        if (p->getCategory()==UvmPort::CLOCK) {
            line += "@(posedge vif.";
            line += p->getName();
            line += ");\n";
        }
    }


    foreach(PhysicalPort *p, comp->getParentVip()->getInterface()->getPhysicalPorts()) {
        if ((p->getCategory()!=UvmPort::CLOCK) &&
                (p->getCategory()!=UvmPort::RESET) &&
                (p->getDirection()!=UvmPort::IN)) {
            line += "vif.";
            line += p->getName();
            line += " = ";
            line += "trans.";
            line += p->getName();
            line += ";\n";
        }
    }

    map.insert("driver_drive",line);

    line = "";
    bool hasReset = false;
    foreach(PhysicalPort *p, comp->getParentVip()->getInterface()->getPhysicalPorts()) {
        if (p->getCategory()==UvmPort::RESET) {
            line += tab + "@(posedge vif.";
            line += p->getName();
            line += ");\n";
            hasReset = true;
        }
    }
    QString theLine = "";
    if (hasReset) {
        theLine += "forever begin\n";
        theLine += line;
        theLine +=tab + "/*---------------------------------------------------------------------------\n";
        theLine +=tab + "* @TODO : reset all signals while reset signal is active\n";
        theLine +=tab + "*--------------------------------------------------------------------------*/\n";
        theLine +=tab + "\n";
        foreach(PhysicalPort *p, comp->getParentVip()->getInterface()->getPhysicalPorts()) {
            if ((p->getCategory()!=UvmPort::CLOCK) &&
                    (p->getCategory()!=UvmPort::RESET) &&
                    (p->getDirection()!=UvmPort::IN)) {
                theLine += tab + "vif.";
                theLine += p->getName();
                theLine += " = ";
                theLine += "0;\n";
            }
        }
        line += "\n";
        theLine += "end\n";
    }

    map.insert("driver_resets",theLine);

    generateFile(comp,map);

}

void SVExport::visit(const UvmMonitor *comp){
    if (!comp->toGenerate())
        return;
    QMap<QString,QString> map;
    initMap(map,comp);

    map.insert("agent_class_name",comp->getParent()->getClassName().toLower());
    map.insert("vip_class_name",comp->getParentVip()->getClassName().toLower());

    QString line;

    line = "";
    foreach(PhysicalPort *p, comp->getParentVip()->getInterface()->getPhysicalPorts()) {
        if (p->getCategory()==UvmPort::CLOCK) {
            line += "@(posedge vif.";
            line += p->getName();
            line += ");\n";
        }
    }


    foreach(PhysicalPort *p, comp->getParentVip()->getInterface()->getPhysicalPorts()) {
        if ((p->getCategory()!=UvmPort::CLOCK) && (p->getCategory()!=UvmPort::RESET)) {
            line += "trans_collected.";
            line += p->getName();
            line += " = ";
            line += "vif.";
            line += p->getName();
            line += ";\n";
        }
    }

    map.insert("monitor_collect",line);


    generateFile(comp,map);
}


void SVExport::visit(const UvmCollector *comp){
    if (!comp->toGenerate())
        return;
    QMap<QString,QString> map;
    initMap(map,comp);


    QString line;

    map.insert("agent_class_name",comp->getParent()->getClassName().toLower());

    line = "";
    foreach(PhysicalPort *p, comp->getParentVip()->getInterface()->getPhysicalPorts()) {
        if (p->getCategory()==UvmPort::CLOCK) {
            line += "@(posedge vif.";
            line += p->getName();
            line += ");\n";
        }
    }


    foreach(PhysicalPort *p, comp->getParentVip()->getInterface()->getPhysicalPorts()) {
        if ((p->getCategory()!=UvmPort::CLOCK) && (p->getCategory()!=UvmPort::RESET)) {
            line += "trans_collected.";
            line += p->getName();
            line += " = ";
            line += "vif.";
            line += p->getName();
            line += ";\n";
        }
    }

    map.insert("monitor_collect",line);

    generateFile(comp,map);
}


void SVExport::visit(const UvmDesign *comp){
    return;
    QMap<QString,QString> map;
    initMap(map,comp);
    generateFile(comp,map);
}

void SVExport::visit(const UvmSequencer *comp){
    if (!comp->toGenerate())
        return;
    QMap<QString,QString> map;
    initMap(map,comp);
    generateFile(comp,map);
}


void SVExport::visit(const UvmSequenceItem *comp){
    if (!comp->toGenerate())
        return;
    QMap<QString,QString> map;
    initMap(map,comp);
    map.insert("class_name", comp->getClassName().toLower());

    QString line;

    line = "";

    foreach(PhysicalPort *p, comp->getParentVip()->getInterface()->getPhysicalPorts()) {
        if ((p->getCategory()!=UvmPort::CLOCK) && (p->getCategory()!=UvmPort::RESET)) {
            if (p->getDirection() != UvmPort::IN)
                line += "rand ";
            line += "logic ";

            if (p->getSize() > 1) {
                line += QString("[%1:0] ").arg(p->getSize() - 1);
            }

            line += p->getName() + ";\n";
        }
    }
    map.insert("transfer_variables",line);


    line = "";

    foreach(PhysicalPort *p, comp->getParentVip()->getInterface()->getPhysicalPorts()) {
        if ((p->getCategory()!=UvmPort::CLOCK) && (p->getCategory()!=UvmPort::RESET)) {
            line += "`uvm_field_int (";
            line += p->getName();
            line += ",  UVM_DEFAULT)\n";
        }
    }
    map.insert("transfer_var_macro",line);

    generateFile(comp,map);
}

void SVExport::visit(const UvmConfig *comp){
    return;


    if (!comp->toGenerate())
        return;
    QMap<QString,QString> map;
    initMap(map,comp);
    generateFile(comp,map);
}

void SVExport::visit(const UvmInterface *comp){
    if (!comp->toGenerate())
        return;

    // Generate Interface file only if we are the interface of a VIP
    if (dynamic_cast<UvmVerificationComponent *>(comp->getParent()) == 0)
        return;

    QMap<QString,QString> map;
    initMap(map,comp);

    QString line;
    line = "";

    foreach(PhysicalPort *p, comp->getParentVip()->getInterface()->getPhysicalPorts()) {
        if ((p->getType().compare("std_logic")==0) ||
                (p->getType().compare("std_ulogic")==0) ||
                (p->getType().compare("std_logic_vector")==0) ||
                (p->getType().compare("std_ulogic_vector")==0) ||
                (p->getType().compare("")==0))
        {
            line += "logic ";

            if (p->getSize() > 1) {
                line += QString("[%1:0] ").arg(p->getSize() - 1);
            }
        }
        else {
            line+="/*---------------------------------------------------------------------------\n";
            line+=" * @TODO : Define this type \""+p->getType()+"\" in a package\n";
            line+=" * This type is used in the VHDL design, but should be declared as being\n";
            line+=" * compatible with SystemVerilog\n";
            line+=" *--------------------------------------------------------------------------*/\n";
            line += p->getType()+" ";
        }

        line += p->getName() + ";\n";
    }

    map.insert("interface_signals",line);

    generateFile(comp,map);
}

void SVExport::visit(const UvmVirtualSequencer *comp){
    if (!comp->toGenerate())
        return;
    QMap<QString,QString> map;
    initMap(map,comp);

    QString line;

    line = "";

    foreach(UvmPort *p, comp->getSubSequencersPorts()) {
        if (p->getConnections().size()>0) {
            line += p->getConnections()[0]->getDstPort()->getParent()->getClassName();
            line += " ";
            line += p->getConnections()[0]->getDstPort()->getParent()->getClassName();
            line += "_seqr;\n";
        }
    }

    map.insert("subsequencers",line);

    generateFile(comp,map);
}

void SVExport::visit(const UvmSequence *comp){
    if (!comp->toGenerate())
        return;
    QMap<QString,QString> map;
    initMap(map,comp);
    UvmVerificationComponent* vipParent = comp->getParentVip();
    map.insert("vip_name", vipParent->getClassName());
    generateFile(comp,map);
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
    return getComponentIndex(c1) < getComponentIndex(c2);
}

void SVExport::visit(const UvmPackage *comp)
{
    if (!comp->toGenerate())
        return;
    // VIP pkg file
    QMap<QString,QString> map;
    initMap(map,comp);

    QString line;
    UvmVerificationComponent* vipParent = comp->getParentVip();
    map.insert("vip_name", vipParent->getClassName());


    line="";
    line += "`include \"" + vipParent->getInterface()->getBodyFileName() + "\"\n";
    map.insert("include_vip_pkg_if",line);

    line = "";


    line += "`include \"";
    line += vipParent->getClassName().toLower()+"_config.sv";
    line += "\"\n";



    QList<UvmComponent *> children = vipParent->getAllChildren();

    qSort(children.begin(),children.end(),compareComponentsForSort);

    foreach(UvmComponent *c, children) {
        // Ignore Config and interface Objects which do not get
        // included in the package.
        if ((dynamic_cast<UvmConfig *>(c) == NULL) &&
                (dynamic_cast<UvmInterface *>(c) == NULL) &&
                (dynamic_cast<UvmVerificationComponent *>(c) == NULL) &&
                (dynamic_cast<UvmVirtualSequencer *>(c) == NULL) &&
                (c!=comp)) {
            line += "`include \"";
            line += c->getBodyFileName();
            line += "\"\n";
        }
    }

    foreach(UvmVirtualSequencer *seq, vipParent->getVirtualSequencers()) {
        line += "`include \"";
        line += seq->getBodyFileName();
        line += "\"\n";
    }

    line += "`include \"";
    line += vipParent->getParentVip()->getBodyFileName();
    line += "\"\n";


    map.insert("package_files",line);


    QString srcFile = QString(comp->getProject()->getTemplatePath() + UVESettings::generator_gvip_folder() + "gvip_pkg.sv");
    QString dstFile = comp->getAbsoluteBodyFileName();
    // We replace the file name
    QFileInfo info(dstFile);
    QString generatedFile=info.baseName().toUpper();
    map.insert("FILENAME",generatedFile + "_SV");


    generateFile(srcFile, dstFile,map);
}

void SVExport::exportConfig(const UvmVerificationComponent *comp) {
    if (!comp->toGenerate())
        return;
    QMap<QString,QString> map;
    initMap(map,comp);
    QString srcFile, dstFile;

    srcFile = comp->getProject()->getTemplatePath() + comp->getConfigTemplateFileName();
    dstFile = comp->getAbsolutePath()+comp->getClassName().toLower()+"_config.sv";


    QFileInfo info(dstFile);
    QString generatedFile=info.baseName().toUpper();
    // Overwrite the value defined in initMap()
    map.insert("FILENAME",generatedFile + "_SV");
    // Overwrite the value defined in initMap()
    map.insert("filename",comp->getClassName().toLower()+"_config.sv");

    QString line="";

    foreach(UvmAgent *agent,comp->getAgents()) {
        line += "class " + agent->getConfigClassName() + " extends uvm_object;\n\n";
        line += tab + "uvm_active_passive_enum is_active = UVM_ACTIVE;\n";
        line += tab + "bit driver_to_monitor = 0;\n";
        line += tab + "bit monitor_to_sequencer = 0;\n";

        line += tab + "// Monitor fields\n";
        line += tab + "bit monitor_checks_enable = 1;\n";
        line += tab + "bit monitor_coverage_enable = 1;\n";

        if (agent->getCollector() != NULL) {
            line += tab + "// Collector fields\n";
            line += tab + "bit collector_checks_enable = 1;\n";
            line += tab + "bit collector_coverage_enable = 1;\n";
        }

        line += "\n";
        line += tab + "`uvm_object_utils_begin(" + agent->getConfigClassName() + ")\n";
        line += tab + tab + "`uvm_field_enum(uvm_active_passive_enum,is_active, UVM_DEFAULT)\n";
        line += tab + tab + "`uvm_field_int(driver_to_monitor, UVM_DEFAULT)\n";
        line += tab + tab + "`uvm_field_int(monitor_to_sequencer, UVM_DEFAULT)\n";

        line += tab + tab + "// Monitor fields\n";
        line += tab + tab + "`uvm_field_int(monitor_checks_enable, UVM_DEFAULT)\n";
        line += tab + tab + "`uvm_field_int(monitor_coverage_enable, UVM_DEFAULT)\n";
        if (agent->getCollector() != NULL) {
            line += tab + tab + "// Collector fields\n";
            line += tab + tab + "`uvm_field_int(collector_checks_enable, UVM_DEFAULT)\n";
            line += tab + tab + "`uvm_field_int(collector_coverage_enable, UVM_DEFAULT)\n";
        }
        line += tab + "`uvm_object_utils_end\n\n";


        line += "endclass : " + agent->getConfigClassName() + "\n\n";
    }

    line += "class " + comp->getConfigClassName() + " extends uvm_object;\n\n";

    foreach(UvmAgent *agent,comp->getAgents()) {
        line += tab + "int nb_" + agent->getConfigClassName() + ";\n";
        line += tab + agent->getConfigClassName() + " " + agent->getClassName().toLower() +
                "_cfg[];\n";
        line += tab + "bit disable_scoreboard;\n";
        line += tab + "bit has_bus_monitor;\n";
        line += tab + "bit bus_monitor_checks_enable;\n";
        line += tab + "bit bus_monitor_coverage_enable;\n";
    }

    line += tab + "`uvm_object_utils_begin(" + comp->getConfigClassName() + ")\n";
    if (!comp->getScoreboards().empty())
        line += tab + tab + "`uvm_field_int(disable_scoreboard, UVM_DEFAULT);\n";
    if (comp->getBusMonitor()!=0) {
        line += tab + tab + "`uvm_field_int(has_bus_monitor, UVM_DEFAULT);\n";
        line += tab + tab + "`uvm_field_int(bus_monitor_checks_enable, UVM_DEFAULT);\n";
        line += tab + tab + "`uvm_field_int(bus_monitor_coverage_enable, UVM_DEFAULT);\n";
    }
    foreach(UvmAgent *agent,comp->getAgents()) {
        line += tab + tab + "`uvm_field_int(nb_" + agent->getConfigClassName() + ", UVM_DEFAULT)\n";
        line += tab + tab + "`uvm_field_array_object(" + agent->getClassName().toLower() +"_cfg, UVM_DEFAULT)\n";
    }
    line += tab + "`uvm_object_utils_end\n\n";

    line += "endclass : " + comp->getConfigClassName() + "\n\n";


    map.insert("configs",line);


    if(!generateFile(srcFile, dstFile,map))
        int_error=1;
}

bool SVExport::validateTemplate(const UvmProject *project)
{
    QString path = project->getTemplatePath();
    if(!path.endsWith(QDir::separator()))
        path.append(QDir::separator());
    QFile TID(project->getTemplatePath()+ "/" + "templateID.tid");

    QDomDocument TIDcontent("mydocument");

    if (!TID.open(QIODevice::ReadOnly))
        return false;

    if (!TIDcontent.setContent(&TID))
    {
        TID.close();
        return false;
    }
    TID.close();

    QDomElement docElem = TIDcontent.documentElement();
    QDomNode UVMversion = docElem.elementsByTagName("UVMversion").at(0);
    QString UVMversionRequested = UVMversion.attributes().namedItem("UVMversion").nodeValue().toUtf8();
    if(UVMversionRequested.compare(this->templateID) == 0)
        return true;
    return false;

}

