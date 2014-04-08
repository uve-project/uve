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
#include "testgenerator.h"

#include "viewdata.h"

#include "newprojectwizard.h"
#include "svexport.h"
#include "uvexmlexport.h"
#include "uvexmlimport.h"
#include "uveconstants.h"
#include "uvesettings.h"

TestGenerator::TestGenerator()
{
    testInputDir=QCoreApplication::applicationDirPath()+"/../../tests/";
}

#define UPPERCASE false
#define LOWERCASE true


void TestGenerator::testFull()
{
    for(int i=0;i<(1<<15);i++) {
        QString dir;
        dir=QString("test_nb_%1").arg(i);
        test(dir,"reg4","reg4.vhd",LOWERCASE,1,1,1);
    }
}

void TestGenerator::testAll(const QString &outputPath)
{

    curDir = outputPath+"/";

    QFile::remove(QString("%1testResult.txt").arg(curDir));


    test("test_1vip_2cagent_1agent","reg4","reg4.vhd",UPPERCASE,1,2,1);


    testWithVip("testVip","reg4","reg4b.vhd","/home/ythoma/docs/uve/git/lib/memory/FlashMem/flashmem.uvc",UPPERCASE);

    test("test_1vip_1cagent_1agent","reg4","reg4.vhd",UPPERCASE,1,1,1,0xFFFF);


    testWithVip("testImport","reg4","reg4b.vhd","/home/ythoma/docs/uve/git/tests14/test_1vip_1cagent_1agent/monvc0/MonVC0.uvc",UPPERCASE);

 //   return;

 //   test("test_2vip_1cagent_1agent","reg4","reg4.vhd",UPPERCASE,2,1,1,0xFFFF);
 //   return;


 //   testFull();


    test("test_listport","reg4","reg4b.vhd",UPPERCASE,1,1,1);

    test("test_1vip_1cagent_2agent","reg4","reg4.vhd",UPPERCASE,1,1,2);
    test("test_1vip_1cagent_3agent","reg4","reg4.vhd",UPPERCASE,1,1,3);
    test("test_1vip_1cagent_4agent","reg4","reg4.vhd",UPPERCASE,1,1,4);

    test("test_1vip_1cagent_1agent_low","reg4","reg4.vhd",LOWERCASE,1,1,1);
    test("test_1vip_1cagent_2agent_low","reg4","reg4.vhd",LOWERCASE,1,1,2);
    test("test_1vip_1cagent_3agent_low","reg4","reg4.vhd",LOWERCASE,1,1,3);
    test("test_1vip_1cagent_4agent_low","reg4","reg4.vhd",LOWERCASE,1,1,4);


    test("test_1vip_2cagent_1agent","reg4","reg4.vhd",UPPERCASE,1,2,1);
    test("test_1vip_2cagent_2agent","reg4","reg4.vhd",UPPERCASE,1,2,2);
    test("test_1vip_2cagent_3agent","reg4","reg4.vhd",UPPERCASE,1,2,3);
    test("test_1vip_2cagent_4agent","reg4","reg4.vhd",UPPERCASE,1,2,4);

    test("test_1vip_2cagent_1agent_low","reg4","reg4.vhd",LOWERCASE,1,2,1);
    test("test_1vip_2cagent_2agent_low","reg4","reg4.vhd",LOWERCASE,1,2,2);
    test("test_1vip_2cagent_3agent_low","reg4","reg4.vhd",LOWERCASE,1,2,3);
    test("test_1vip_2cagent_4agent_low","reg4","reg4.vhd",LOWERCASE,1,2,4);

    test("test_2vip_1cagent_1agent","reg4","reg4.vhd",UPPERCASE,2,1,1);
    test("test_2vip_1cagent_2agent","reg4","reg4.vhd",UPPERCASE,2,1,2);
    test("test_2vip_1cagent_3agent","reg4","reg4.vhd",UPPERCASE,2,1,3);
    test("test_2vip_1cagent_4agent","reg4","reg4.vhd",UPPERCASE,2,1,4);

    test("test_2vip_1cagent_1agent_low","reg4","reg4.vhd",LOWERCASE,2,1,1);
    test("test_2vip_1cagent_2agent_low","reg4","reg4.vhd",LOWERCASE,2,1,2);
    test("test_2vip_1cagent_3agent_low","reg4","reg4.vhd",LOWERCASE,2,1,3);
    test("test_2vip_1cagent_4agent_low","reg4","reg4.vhd",LOWERCASE,2,1,4);

    test("test_2vip_2cagent_1agent","reg4","reg4.vhd",UPPERCASE,2,2,1);
    test("test_2vip_2cagent_2agent","reg4","reg4.vhd",UPPERCASE,2,2,2);
    test("test_2vip_2cagent_3agent","reg4","reg4.vhd",UPPERCASE,2,2,3);
    test("test_2vip_2cagent_4agent","reg4","reg4.vhd",UPPERCASE,2,2,4);

    test("test_2vip_2cagent_1agent_low","reg4","reg4.vhd",LOWERCASE,2,2,1);
    test("test_2vip_2cagent_2agent_low","reg4","reg4.vhd",LOWERCASE,2,2,2,0xFFFF);
    test("test_2vip_2cagent_3agent_low","reg4","reg4.vhd",LOWERCASE,2,2,3);
    test("test_2vip_2cagent_4agent_low","reg4","reg4.vhd",LOWERCASE,2,2,4);

    test("test9","counter","counter.vhd",UPPERCASE,1,1,1);

//    test("test10","counter","counter.vhd",LOWERCASE,1);
}


QString TestGenerator::nameCase(const QString &name,bool toLowerCase) const
{
    if (toLowerCase)
        return name.toLower();
    else
        return name;
}

void TestGenerator::test(const QString &subdir,
                         const QString &entityName,
                         const QString &vhdlFileName,
                         bool lowercase,
                         int nbVips,
                         int nbClassAgent,
                         int nbInstAgents,
                         int parameters)
{
    NewProjectWizard *wizard = new NewProjectWizard();
    wizard->setupProject();


    //View models
    ProjectViewData *d_project=wizard->d_project;
    d_project->dutEntityName=entityName;
    d_project->name=nameCase("MonProjet",lowercase);
    d_project->folder=curDir+subdir+"/";
    d_project->templatePath=UVESettings::generator_gve_folder();

    //Fetch the simulator in the settings
    QString simulator = UVESettings::getInstance()->value(CONF_SIMULATOR,QVariant("")).toString();

    d_project->simulator=simulator;
    d_project->files << testInputDir+vhdlFileName;

    wizard->setupEnv();

    TestCaseViewData *d_testcase=wizard->d_testcase;
    d_testcase->name=nameCase("MonTestCase",lowercase);

    TestBenchViewData *d_testbench=wizard->d_testbench;
    d_testbench->name=nameCase("MonTestBench",lowercase);
    d_testbench->virtualSequencer=parameters&(1<<0);
    d_testbench->scoreboard=parameters&(1<<1);

    for(int v=0;v<nbVips;v++)
    {
        VCViewData *vc;
        vc=new VCViewData();
        vc->name=nameCase(QString("MonVC%1").arg(v),lowercase);
        vc->file="";
        vc->description="C'est mon VC à moi";
        vc->virtualSequencer=parameters&(1<<2);
        vc->virtualSequencerLink=parameters&(1<<3);
        vc->scoreboard=parameters&(1<<4);
        vc->busMonitor=parameters&(1<<5);
        for(int a=0;a<nbClassAgent;a++)
        {
            AgentViewData *agent;
            agent = new AgentViewData();
            agent->name=nameCase(QString("MonAgent%1").arg(a),lowercase);
            agent->nbAgents=nbInstAgents;
            agent->monitor=parameters&(1<<6);
            agent->monitorCoverage=parameters&(1<<7);
            agent->monitorLink=parameters&(1<<8); //Sequencer to monitor link
            agent->sequencer=parameters&(1<<9);
            agent->sequencerCoverage=parameters&(1<<10);
            agent->sequencerLink=parameters&(1<<11); //VC Virtual sequencer to sequencer link
            agent->collector=parameters&(1<<12);
            agent->driver=parameters&(1<<13);
            agent->driver2monitorLink=parameters&(1<<14); // Driver to monitor link

            vc->agents << agent;
        }
        // Pour un VC:
    //    QList<BaseConnection> connections;
    //    QStringList interConnects;


        d_testbench->vcs << vc;
    }



    wizard->setupSignals();

    //Save the connected signals list
    foreach (VCViewData *vc,d_testbench->vcs)
    {
        vc->interConnects.clear();
        foreach(UvmPort *port,d_project->duvPorts)
            vc->interConnects.append(port->getName());
    }

    wizard->createProject();


    UveXmlExport xmlExporter;
    if (!xmlExporter.saveProject(wizard->getProject()))
        qCritical() << xmlExporter.getErrorString();

    SVExport exporter;
    if (!exporter.exportProject(wizard->_result))
        qCritical() << qPrintable(exporter.getErrorString());

    QString fullFilename= wizard->getProject()->getFolder() + wizard->getProject()->getProjectFileName();

    testXmlProject(fullFilename);


    foreach(VCViewData *vc,d_testbench->vcs)
    {
        foreach (AgentViewData *agent,vc->agents)
            delete agent;
        delete vc;
    }
    delete wizard;
}





void TestGenerator::testWithVip(const QString& subdir,
                                const QString& entityName,
                                const QString& vhdlFileName,
                                const QString& vipFileName,
                                bool lowercase)
{
    NewProjectWizard *wizard = new NewProjectWizard();
    wizard->setupProject();


    //View models
    ProjectViewData *d_project=wizard->d_project;
    d_project->dutEntityName=entityName;
    d_project->name=nameCase("MonProjet",lowercase);
    d_project->folder=curDir+subdir+"/";
    d_project->templatePath=UVESettings::generator_gve_folder();

    //Fetch the simulator in the settings
    QString simulator = UVESettings::getInstance()->value(CONF_SIMULATOR,QVariant("")).toString();

    d_project->simulator=simulator;
    d_project->files << testInputDir+vhdlFileName;

    wizard->setupEnv();

    TestCaseViewData *d_testcase=wizard->d_testcase;
    d_testcase->name=nameCase("MonTestCase",lowercase);

    TestBenchViewData *d_testbench=wizard->d_testbench;
    d_testbench->name=nameCase("MonTestBench",lowercase);
    d_testbench->virtualSequencer=0;//parameters&(1<<0);
    d_testbench->scoreboard=0;//parameters&(1<<1);

    for(int v=0;v<1;v++)
    {
        VCViewData *vc;
        vc=new VCViewData();
        vc->name="FlashMem";//nameCase(QString("MonVC%1").arg(v),lowercase);
        vc->file = vipFileName;
        d_testbench->vcs << vc;
    }



    wizard->setupSignals();

    //Save the connected signals list
    foreach (VCViewData *vc,d_testbench->vcs)
    {
        vc->interConnects.clear();
        foreach(UvmPort *port,d_project->duvPorts)
            vc->interConnects.append(port->getName());
    }

    wizard->createProject();
    if (!wizard->_result)
    {
        qCritical() << "The project could not be created";
        return;
    }


    UveXmlExport xmlExporter;
    if (!xmlExporter.saveProject(wizard->getProject()))
        qCritical() << xmlExporter.getErrorString();

    SVExport exporter;
    if (!exporter.exportProject(wizard->_result))
        qCritical() << qPrintable(exporter.getErrorString());


    if (!xmlExporter.saveProject(wizard->getProject()))
        qCritical() << xmlExporter.getErrorString();

    QString fullFilename= wizard->getProject()->getFolder() + wizard->getProject()->getProjectFileName();

    testXmlProject(fullFilename);


    foreach(VCViewData *vc,d_testbench->vcs)
    {
        foreach (AgentViewData *agent,vc->agents)
            delete agent;
        delete vc;
    }
    delete wizard;
}







void TestGenerator::testXmlProject(QString projectFileName)
{
    UveXmlImport importer;
    UvmProject *project=importer.loadProject(projectFileName);
    if (!project)
    {
        qCritical() << importer.getErrorString();
        return;
    }


    QFileInfo info(project->getFolder());
    QString oldPath=project->getFolder();
    QString newPath=info.dir().absolutePath()+"_regen";
    QDir().mkpath(newPath);
    project->setFolder(newPath+"/");
    UveXmlExport exporter;
    if (!exporter.saveProject(project))
        qCritical() << exporter.getErrorString();

    // We regenerate the entire SystemVerilog files
    SVExport svexporter;
    if (!svexporter.exportProject(project))
        qCritical() << qPrintable(svexporter.getErrorString());

    /*
    QString newFileName= project->getFolder() + project->getBodyFileName();

    compareFiles(projectFileName,newFileName);


    QStringList vipFileNames1;
    foreach (UvmVerificationComponent *vc,project->getTop()->getVips())
        vipFileNames1 << project->getFolder() + vc->getRelativePath()+ vc->getVipFilename();

    QStringList vipFileNames2;
    foreach (UvmVerificationComponent *vc,project->getTop()->getVips())
        vipFileNames2 << project->getFolder() + vc->getRelativePath()+ vc->getVipFilename();


    if (vipFileNames1.size()!=vipFileNames2.size())
        qCritical() << "The number of VIPs do not match";

    for(int i=0;i<vipFileNames1.size();i++)
        compareFiles(vipFileNames1.at(i),vipFileNames2.at(i));

    foreach(QString fileName,project->getTop()->getAllVIPFilePaths())
        qDebug() << "VipFilePath: " << fileName;
    */

    int ret=QProcess::execute(QString("%1compare.sh %2 %3 %4testResult.txt").arg(testInputDir).arg(oldPath).arg(newPath).arg(curDir));
    if (ret==-2)
        qCritical() << "Can not start the folders comparison";
}

void TestGenerator::compareFiles(const QString &fileName1, const QString &fileName2)
{
    QFile file0(fileName1);
    if (!file0.open(QIODevice::ReadOnly | QIODevice::Text))
        qCritical() << "Can not open the file " << fileName1 << " for comparison";
    QFile file1(fileName2);
    if (!file1.open(QIODevice::ReadOnly | QIODevice::Text))
        qCritical() << "Can not open the file " << fileName2 << " for comparison";

    QTextStream in0(&file0);
    QTextStream in1(&file1);
    bool isError=false;
    while (!in0.atEnd()) {
        if (in0.readLine().compare(in1.readLine()))
        {
            qCritical() << "Files not identical: " << fileName1 << " and " << fileName2;
            isError=true;
        }
    }
    if (!in1.atEnd())
    {
        qCritical() << "Files not identical: " << fileName1 << " and " << fileName2;
        isError=true;
    }
    if (!isError)
        qDebug() << "Files identical: "  << fileName1 << " and " << fileName2;
}

static int testVerbosity = VERBOSITY_DEBUG;

void testMessageOutput(QtMsgType type, const QMessageLogContext &/*context*/, const QString &msg)
 {
     switch (type) {
     case QtDebugMsg:
         if (testVerbosity<=VERBOSITY_DEBUG)
             fprintf(stderr, "Debug: %s\n", msg.toLatin1().constData());
         break;
     case QtWarningMsg:
         if (testVerbosity<=VERBOSITY_WARNING)
             fprintf(stderr, "Warning: %s\n", msg.toLatin1().constData());
         break;
     case QtCriticalMsg:
         if (testVerbosity<=VERBOSITY_CRITICAL)
             fprintf(stderr, "Critical: %s\n", msg.toLatin1().constData());
         break;
     case QtFatalMsg:
         if (testVerbosity<=VERBOSITY_FATAL)
             fprintf(stderr, "Fatal: %s\n", msg.toLatin1().constData());
         abort();
     }
 }


void setVerbosity(int verbosity)
{
    testVerbosity=verbosity;
}
