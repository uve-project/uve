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
#include "newprojectwizard.h"

#include <QHBoxLayout>
#include <QMessageBox>
#include <QFile>
#include <QByteArray>
#include <QRegExp>

#include "uvmtop.h"
#include "uvmdesign.h"
#include "uvmport.h"
#include "uvmconnection.h"
#include "uvmtestbench.h"
#include "uvmtestcase.h"
#include "uvmsequenceitem.h"
#include "uvmsequence.h"
#include "uvmpackage.h"
#include "vhdlparser.h"

#include "viewdata.h"

//Setup the pages
NewProjectWizard::NewProjectWizard(const QString defaultSimulator, const QString &defaultFile, QWidget *parent) :
    QWizard(parent),
    _defaultSimulator(defaultSimulator),
    _defaultFile(defaultFile),
    d_project(0),
    _result(0),
    d_testcase(0),
    d_testbench(0),
    d_signals(0),
    m_testcase(0),
    m_project(0),
    m_testbench(0),
    m_top(0),
    m_design(0)
{
    //Set our title
    this->setWindowTitle(tr("Project Creator"));

    //Project page
    _project = new ProjectDialogView(this);
    _project->setTitle(tr("Project"));
    _project->setSubTitle(tr("The project's global data."));
    this->addPage(_project);

    //Environment page
    _env = new EnvDialogView(this);
    _env->setTitle(tr("Environment"));
    _env->setSubTitle(tr("The project's global structure."));
    //this->addPage(_env);

    //Signals page
    _signals = new SignalsDialogView(this);
    _signals->setTitle(tr("Signals"));
    _signals->setSubTitle(tr("Definition of the clocks and resets signals.") + "\n" +
                          tr("Here only the DUT input signals are shown."));
    this->addPage(_signals);
    this->addPage(_env);
    //Connect our finished signal to the project creation function
    CONNECT(this, SIGNAL(accepted()), this, SLOT(createProject()));
}

NewProjectWizard::~NewProjectWizard()
{
    delete _project;
    delete _env;
    delete _signals;

    delete d_project;
    delete d_testcase;
    delete d_testbench;
    delete d_signals;
}

UvmProject *NewProjectWizard::getProject() const
{
    return _result;
}


//Check in the list
bool NewProjectWizard::isNameUsedAtProjectScope(const QString &name) const
{
    return _names.contains(name);
}

//Save it in the list
void NewProjectWizard::registerNameAtProjectScope(const QString &name)
{
    _names.append(name);
}

//Remove from the list
void NewProjectWizard::unRegisterNameAtProjectScope(const QString &name)
{
    _names.removeAll(name);
}


//Simple redirection to the specific functions
void NewProjectWizard::initializePage(int id)
{
    switch (id) {
    case ProjectPage :   setupProject();  break;
    case SignalsPage :   setupSignals();  break;
    case EnvPage :       setupEnv();      break;
    default : break; //ARRG! This shouldn't happen!
    }
}

#include "uvexmlimport.h"
#include "wizarddataconverter.h"

//This function is called at the end
//These object are already created : UvmProject, UvmTop, UvmTestCase and UvmTestbench
void NewProjectWizard::createProject()
{
    _result = new UvmProject();

    WizardDataConverter converter;
    if (!converter.data2Project(d_project,d_testcase,d_testbench,d_signals,_result))
    {
        QMessageBox::warning(this,"Error while exporting the project: ",converter.getErrorString());
        delete _result;
        _result = 0;
        return;
    }

}

//------------------------- PAGES FUNCTIONS -------------------------

//Create a projet view data if none exists, and give it to the view
void NewProjectWizard::setupProject()
{
    //Create if none
    if (!d_project) {
        d_project = new ProjectViewData;
        d_project->dut = 0;
        //Default simulator value
        d_project->simulator = _defaultSimulator;
        //Default file name (if any)
        if (!_defaultFile.isEmpty())
            d_project->files << _defaultFile;
    }

    //Setup the view
    _project->setModel(d_project);
}

//Create the test bench / test case if not present
void NewProjectWizard::setupEnv()
{
    //Create the project (name only)
    if (!m_project) {
        m_project = new UvmProject();
        m_project->setName(d_project->name);
    }

    //Create the top object
    if (!m_top) {
        m_top = new UvmTop("Top",m_project);
        m_project->setTop(m_top);
    }

    //Create the test case&bench
    if (!m_testcase) {
        m_testcase = new UvmTestCase(m_top);
        m_top->setTestCase(m_testcase);
    }
    if (!m_testbench) {
        m_testbench = new UvmTestbench(m_testcase);
        m_testcase->setTestbench(m_testbench);
    }

    //Test Case
    if (!d_testcase) {
        d_testcase = new TestCaseViewData;
        d_testcase->name = m_testcase->getDefaultName();
    }

    //Test bench
    if (!d_testbench) {
        d_testbench = new TestBenchViewData;
        d_testbench->scoreboard = true;
        d_testbench->name = m_testbench->getDefaultName();
    }

    //DUV
    /*if (!m_design) {


        //Add each file in the list, parse it and add its class name
        VHDLParser parser;
        foreach (QString file, d_project->files) {
            //Parse
            if (parser.read(file) == -1)
                continue;
            VHDLFileContent *content=parser.getVHDLFileContent();
            if (content->getEntitiesName().contains(d_project->dutEntityName))
                d_project->dut = content;
            else
                delete content;
        }

        //Create the design unit
        if (!m_design) {
            m_design = new UvmDesign(d_project->dutEntityName,m_top);
            //d_project->dut->getPorts(d_project->dutEntityName,m_design); //Will be added to the design
        }
    }*/

    //Setup the view
    _env->setModels(d_testbench,d_testcase,d_project->dut,d_project->dutEntityName);
}

//Load the DUT and fetch the ports from the design if necessary
void NewProjectWizard::setupSignals()
{
    //DUV
    if (!m_design) {


        //Add each file in the list, parse it and add its class name
        VHDLParser parser;
        foreach (QString file, d_project->files) {
            //Parse
            if (parser.read(file) == -1)
                continue;
            VHDLFileContent *content=parser.getVHDLFileContent();
            if (content->getEntitiesName().contains(d_project->dutEntityName))
                d_project->dut = content;
            else
                delete content;
        }

        //Create the design unit
        m_design = new UvmDesign(d_project->dutEntityName,m_top);
        d_project->dut->getPorts(d_project->dutEntityName,m_design); //Will be added to the design
    }

    //Do nothing if the model is already here
    if (!d_signals) {
        //Fill the ports (only input ports)
        d_project->duvPorts = m_design->getPhysicalPorts();
        d_signals = new SignalsViewData;
        foreach (PhysicalPort *p,d_project->duvPorts) {
            if (p->getDirection() == UvmPort::PORT_IN) {
                //Try to find out if the signal is a clock
                QRegExp rc (".*[Cc][Ll][oO]?[Cc]?[Kk].*");
                if (rc.indexIn(p->getName()) != -1)
                    p->setCategory(UvmPort::CLOCK);
                //Same for the reset
                QRegExp rr (".*[Rr][Ee]?[Ss][Ee]?[Tt].*");
                if (rr.indexIn(p->getName()) != -1)
                    p->setCategory(UvmPort::RESET);
                //Add it in the view model
                d_signals->ports.append(p);
            }
            else {
                // We also add the output and in/out signals
                d_signals->ports.append(p);
            }
        }

        //Save the ports in the view
        _signals->setModel(d_signals);
    }
}
