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
#include "wizarddataconverter.h"

#include "uvmproject.h"
#include "uvmtop.h"
#include "uvmsequence.h"
#include "uvmpackage.h"
#include "vhdlclasses.h"
#include "uvexmlimport.h"

#include "namesetter.h"

WizardDataConverter::WizardDataConverter()
{
}

bool WizardDataConverter::data2Project(const ProjectViewData *d_project, const TestCaseViewData *d_testcase, const TestBenchViewData *d_testbench, const SignalsViewData *d_signals, UvmProject *project)
{
    errorString="";

    if (project==0)
    {
        errorString="Project equals to 0";
        return false;
    }

    UvmTop *m_top = 0;
    UvmTestCase *m_testcase = 0;
    UvmTestbench *m_testbench = 0;
    UvmDesign *m_design = 0;

    //Create the top object
    m_top = new UvmTop("Top",project);
    project->setTop(m_top);

    //Create the test case&bench
    m_testcase = new UvmTestCase(m_top);
    m_top->setTestCase(m_testcase);

    m_testbench = new UvmTestbench(m_testcase);
    m_testcase->setTestbench(m_testbench);


    //Create the design unit
    if (!m_design) {
        m_design = new UvmDesign(d_project->dutEntityName,m_top);
        m_design->setBodyFileName(d_project->dut->getFileName());
        foreach(PhysicalPort *port,d_signals->ports)
        {
            PhysicalPort *p=new PhysicalPort(port,m_design);
            if (!p)
                qCritical() << "Dramatic error in file " << __FILE__ << " line " << __LINE__;
        }
    }

    //Setup the project
    project->setDut(d_project->dut->getFileName());
    project->setFolder(d_project->folder);
    project->setName(d_project->name);
    project->setSimulator(d_project->simulator);
    project->setVHDLFiles(d_project->files);
    project->setTemplatePath(d_project->templatePath);

    //Create the interface
    UvmInterface *m_interface = new UvmInterface("Interface", m_top);
    m_interface->setBodyFileName(m_top->getBodyFileName());

    //DUV -> Interface
    new UvmConnection (UvmPort::newFakePort(m_design->getShortName()+"AllSignals", UvmPort::DUT_INTERFACE, UvmPort::PORT_OUT, m_design),
                       UvmPort::newFakePort(m_interface->getShortName()+"DUTAllSignals", UvmPort::DUT_INTERFACE, UvmPort::PORT_IN, m_interface),
                       linkName(m_design,m_interface),
                       m_top);

    //Setup the TestCase
    m_testcase->setShortName(d_testcase->name);

    //Setup the TestBench
    m_testbench->setShortName(d_testbench->name);

    //Scoreboard
    UvmScoreboard *m_testbenchScoreboard = 0;
    if (d_testbench->scoreboard) {
        m_testbenchScoreboard = new UvmScoreboard("Scoreboard",m_testbench);
        m_testbenchScoreboard->setComparatorType((UvmScoreboard::ComparatorType)d_testbench->scoreboardComparatorType);
        m_testbench->addScoreboard(m_testbenchScoreboard);
    }

    //Virtual sequencer
    UvmVirtualSequencer *m_testbenchSequencer = 0;
    if (d_testbench->virtualSequencer) {
        m_testbenchSequencer = new UvmVirtualSequencer("VirtualSeq",m_testbench);
        m_testbench->addVirtualSequencer(m_testbenchSequencer);
    }

    //Create the VCs
    foreach (VCViewData *d_vc, d_testbench->vcs) {

        UvmVerificationComponent *m_vc = 0;

        //If the VC is from a file (loaded from library), reload it
        if ( ! d_vc->file.isEmpty() ) {

            //Load it and setup its name
            QList<BaseConnection> connList;
            UveXmlImport importer;
            m_vc = importer.loadVerificationComponent(d_vc->file,m_testbench,false);

            //On error, quit
            if (!m_vc) {
                errorString=QObject::tr("Error while loading the VIP: %1").arg(importer.getErrorString());
                return false;
            }





            //Create the VC's agents
            foreach (UvmAgent *m_agent,m_vc->getAgents()) {

                //Monitor
                UvmMonitor *m_agentMonitor=m_agent->getMonitor();
                if (m_agentMonitor) {

                    //Monitor to scoreboard
                    if (d_testbench->scoreboard)
                        new UvmConnection ( UvmPort::newTlmPort(m_agentMonitor->getShortName()+"ScoreboardOut", UvmPort::UVM_ANALYSIS_PORT, m_agentMonitor),
                                            UvmPort::newTlmPort(m_testbenchScoreboard->getShortName()+"Agent"+m_agentMonitor->getShortName(), UvmPort::UVM_ANALYSIS_IMP, m_testbenchScoreboard),
                                            linkName(m_agentMonitor,m_testbenchScoreboard),
                                            m_testbench);
                }

                //Sequencer
                UvmSequencer *m_agentSequencer=m_agent->getSequencer();
                if (m_agentSequencer) {

                    //Virtual sequencer to sequencer
                    if (false) {
//                    if (d_agent->sequencerLink && d_vc->virtualSequencer) {
                        new UvmConnection ( UvmPort::newTlmPort(m_vc->getVirtualSequencers()[0]->getShortName()+"Agent"+m_agentSequencer->getShortName(), UvmPort::VIRT_SEQ_TO_SEQ_OUT, m_vc->getVirtualSequencers()[0]),
                                            UvmPort::newTlmPort(m_agentSequencer->getShortName()+"VirtualSequencerIn", UvmPort::VIRT_SEQ_TO_SEQ_IN, m_agentSequencer),
                                            linkName(m_vc->getVirtualSequencers()[0],m_agentSequencer),
                                            m_testbench);
                    }

                    // Testbench virtual sequencer to sequencer link, if there is no VIP virtual sequencer
                    //               if (d_agent->sequencerLink && (!d_vc->virtualSequencer) && m_testbenchSequencer)
                    //                   new UvmConnection ( new UvmPort(m_testbenchSequencer->getName()+"Agent"+m_agentSequencer->getName(), UvmPort::TLM_ROUND, UvmPort::OUT, "", m_testbenchSequencer),
                    //                                       new UvmPort(m_agentSequencer->getName()+"VirtualSequencerIn", UvmPort::TLM_SQUARE, UvmPort::IN, "", m_agentSequencer),
                    //                                       linkName(m_testbenchSequencer,m_agentSequencer));
                }
            }



            //Create the connections from/to the DUV interface
            foreach (BaseConnection bc, d_vc->connections) {

                //Create the two ports
                PhysicalPort *from = 0, *to = 0;

                //Search for the port in the DUV interface
                foreach (PhysicalPort *port, m_design->getPhysicalPorts()) {

                    //If in the DUV
                    if (port->getName() == bc.dst) {

                        //Define which is the source and which is the destination
                        QString duvPortName;
                        QString vipPortName;
                        duvPortName = bc.dst;
                        vipPortName = bc.src;

                        //Find the direction
                        switch (port->getDirection()) {
                        case UvmPort::PORT_OUT : //Interface to VC
                            from = port;
                            to = m_vc->getInterface()->getPortByName(vipPortName);
                            break;
                        case UvmPort::PORT_IN : //VC to Interface
                            from = m_vc->getInterface()->getPortByName(vipPortName);
                            to = port;
                            break;
                        default: //VC <-> Interface
                            from = port;
                            to = m_vc->getInterface()->getPortByName(vipPortName);
                            break;
                        }
                    }
                }

                //Add the connection
                new UvmConnection (from,to,bc.name,m_top);
            }

            //Visual signal between the VC interface and the interface
            new UvmConnection ( UvmPort::newFakePort(m_vc->getInterface()->getShortName()+"_interinterport", UvmPort::VCTODUT_INTERFACE, UvmPort::PORT_INOUT, m_vc->getInterface()),
                                UvmPort::newFakePort(m_interface->getShortName()+"_interinterport", UvmPort::VC_INTERFACE, UvmPort::PORT_INOUT, m_interface),
                                linkName(m_vc->getInterface(),m_interface),
                                m_top);

            m_vc->setVipRelativePath(m_vc->getClassName().toLower()+"/");
            m_vc->setShortName(m_vc->getClassName());

        }

        //Else, its an user-added VC that must be setup
        else {

            //Setup the VC
            m_vc = new UvmVerificationComponent(d_vc->name,m_testbench);
            m_vc->setVipRelativePath(m_vc->getRelativePath());
            m_vc->setDescription(d_vc->description);
            UvmConfig *config=new UvmConfig(m_vc->getShortName().toLower()+"_config",m_vc);
            config->setBodyFileName(m_vc->getShortName().toLower()+"_config.sv");
            m_vc->setConfig(config);

            //VC Interface
            UvmInterface *m_vcInterface = new UvmInterface("VC_Interface", m_vc);

            // Sequence item
            new UvmSequenceItem("Transfer", m_vc);

            // Sequence
            new UvmSequence("Sequence", m_vc);

            // Package
            new UvmPackage("pkg",m_vc);

            //VC bus monitor
            UvmMonitor *m_vcMonitor = 0;
            if (d_vc->busMonitor) {
                m_vcMonitor = new UvmMonitor("BusMonitor",m_vc);
                m_vc->setBusMonitor(m_vcMonitor);

                //To the interface
                new UvmConnection ( UvmPort::newFakePort(m_vcInterface->getShortName()+"BusMonitorOut", UvmPort::VC_INTERFACE, UvmPort::PORT_OUT, m_vcInterface),
                                    UvmPort::newFakePort(m_vcMonitor->getShortName()+"InterfaceIn", UvmPort::VC_INTERFACE, UvmPort::PORT_IN, m_vcMonitor),
                                    linkName(m_vcInterface,m_vcMonitor),
                                    m_vc);
            }


            UvmVirtualSequencer *m_vcSequencer = 0;
            if (d_vc->virtualSequencer) {
                m_vcSequencer = new UvmVirtualSequencer("VirtualSequencer",m_vc);
                m_vc->addVirtualSequencer(m_vcSequencer);

                // TODO: Adding link to the testbench virtual sequencer

                // Testbench virtual sequencer to sequencer link, if there is no VIP virtual sequencer
                if (d_vc->virtualSequencerLink &&  m_testbenchSequencer)
                    new UvmConnection ( UvmPort::newTlmPort(m_vc->getShortName()+"_"+ m_vcSequencer->getShortName(), UvmPort::VIRT_SEQ_TO_SEQ_OUT, m_testbenchSequencer),
                                        UvmPort::newTlmPort(m_vc->getShortName()+m_vcSequencer->getShortName()+"VirtualSequencerIn", UvmPort::VIRT_SEQ_TO_SEQ_IN, m_vcSequencer),
                                        linkName(m_testbenchSequencer,m_vcSequencer),
                                        m_testbench);
            }

            //VC Interface connections
            foreach (QString name, d_vc->interConnects) {

                //Create the two ports
                PhysicalPort *from = 0,*to = 0;
                QString conName;

                //Find the name's port
                foreach (PhysicalPort *port, m_design->getPhysicalPorts())
                    if (port->getName() == name) {

                        //Find the direction
                        switch (port->getDirection()) {
                        case UvmPort::PORT_OUT : //Interface to VC
                            from = port;
                            to = UvmPort::newPhysicalPort(name/*+"InterfaceIn"*/, UvmPort::GLOBAL, UvmPort::PORT_IN, port->getType(), m_vcInterface);
                            to->setSize(port->getSize());
                            to->setCategory(port->getCategory());
                            conName = linkName(m_design,m_vc);
                            break;
                        case UvmPort::PORT_IN : //VC to Interface
                            from = UvmPort::newPhysicalPort(name/*+"InterfaceOut"*/, UvmPort::GLOBAL, UvmPort::PORT_OUT, port->getType(), m_vcInterface);
                            from->setSize(port->getSize());
                            from->setCategory(port->getCategory());
                            to = port;
                            conName = linkName(m_vc,m_design);
                            break;
                        default: //VC <-> Interface
                            from = port;
                            to = UvmPort::newPhysicalPort(name/*+"InterfaceInOut"*/, UvmPort::GLOBAL, UvmPort::PORT_INOUT, port->getType(), m_vcInterface);
                            to->setSize(port->getSize());
                            to->setCategory(port->getCategory());
                            conName = linkName(m_design,m_vc);
                            break;
                        }
                    }

                //Add the connection
                new UvmConnection (from,to,conName,m_top);
            }

            //Visual signal between the VC interface and the interface
            new UvmConnection ( UvmPort::newFakePort(m_vcInterface->getShortName()+"_interinterport", UvmPort::VCTODUT_INTERFACE, UvmPort::PORT_INOUT, m_vcInterface),
                                UvmPort::newFakePort(m_interface->getShortName()+"_interinterport", UvmPort::VC_INTERFACE, UvmPort::PORT_INOUT, m_interface),
                                linkName(m_vcInterface,m_interface),
                                m_top);

            //Scoreboard
            UvmScoreboard *m_vcScoreboard = 0;
            if (d_vc->scoreboard) {
                m_vcScoreboard = new UvmScoreboard("Scoreboard",m_vc);
                m_vcScoreboard->setComparatorType((UvmScoreboard::ComparatorType)d_vc->scoreboardComparatorType);
                m_vc->addScoreboard(m_vcScoreboard);
            }

            //Create the VC's agents
            foreach (AgentViewData *d_agent, d_vc->agents) {

                //Setup the agent
                UvmAgent *m_agent = new UvmAgent(d_agent->name,m_vc);

                m_agent->setNbAgents(d_agent->nbAgents);

                UvmConfig *config=new UvmConfig(m_agent->getShortName().toLower()+"_config",m_agent);
                config->setBodyFileName("../"+m_vc->getShortName().toLower()+"_config.sv");
                m_agent->setConfig(config);


                //Monitor
                UvmMonitor *m_agentMonitor = 0;
                if (d_agent->monitor) {
                    m_agentMonitor = new UvmMonitor("Monitor",m_agent);
                    m_agent->setMonitor(m_agentMonitor);

                    TlmPort *item_collected_port = UvmPort::newTlmPort("item_collected_port", UvmPort::UVM_ANALYSIS_PORT, m_agentMonitor);
                    //Monitor to scoreboard
                    if (d_testbench->scoreboard)
                        new UvmConnection ( item_collected_port,
                                            UvmPort::newTlmPort(m_vc->getShortName()+m_agent->getShortName()+m_agentMonitor->getShortName(), UvmPort::UVM_ANALYSIS_IMP, m_testbenchScoreboard),
                                            linkName(m_agentMonitor,m_testbenchScoreboard),
                                            m_testbench);

                    //Monitor to scoreboard
                    if (d_vc->scoreboard)
                        new UvmConnection ( item_collected_port,
                                            UvmPort::newTlmPort(m_vcScoreboard->getShortName()+"Agent"+m_agentMonitor->getShortName(), UvmPort::UVM_ANALYSIS_IMP, m_vcScoreboard),
                                            linkName(m_agentMonitor,m_vcScoreboard),
                                            m_vc);
                }

                //Sequencer
                UvmSequencer *m_agentSequencer = 0;
                if (d_agent->sequencer) {
                    m_agentSequencer = new UvmSequencer("Sequencer",m_agent);
                    m_agent->setSequencer(m_agentSequencer);

                    //Monitor to sequencer
                    if (d_agent->monitorLink && d_agent->sequencer)
                        new UvmConnection ( UvmPort::newTlmPort(m_agentMonitor->getShortName()+"SequencerOut", UvmPort::UVM_BLOCKING_PEEK_IMP, m_agentMonitor),
                                            UvmPort::newTlmPort(m_agentSequencer->getShortName()+"MonitorIn", UvmPort::UVM_BLOCKING_PEEK_PORT, m_agentSequencer),
                                            linkName(m_agentMonitor,m_agentSequencer),
                                            m_agent);

                    //Virtual sequencer to sequencer
                    if (d_agent->sequencerLink && d_vc->virtualSequencer) {
                        new UvmConnection ( UvmPort::newTlmPort(m_vc->getVirtualSequencers()[0]->getShortName()+"Agent"+m_agentSequencer->getShortName(), UvmPort::VIRT_SEQ_TO_SEQ_OUT, m_vc->getVirtualSequencers()[0]),
                                            UvmPort::newTlmPort(m_agentSequencer->getShortName()+"VirtualSequencerIn", UvmPort::VIRT_SEQ_TO_SEQ_IN, m_agentSequencer),
                                            linkName(m_vc->getVirtualSequencers()[0],m_agentSequencer),
                                            m_vc);
                    }

                    // Testbench virtual sequencer to sequencer link, if there is no VIP virtual sequencer
                    //               if (d_agent->sequencerLink && (!d_vc->virtualSequencer) && m_testbenchSequencer)
                    //                   new UvmConnection ( new UvmPort(m_testbenchSequencer->getName()+"Agent"+m_agentSequencer->getName(), UvmPort::TLM_ROUND, UvmPort::OUT, "", m_testbenchSequencer),
                    //                                       new UvmPort(m_agentSequencer->getName()+"VirtualSequencerIn", UvmPort::TLM_SQUARE, UvmPort::IN, "", m_agentSequencer),
                    //                                       linkName(m_testbenchSequencer,m_agentSequencer));
                }

                //Collector
                UvmCollector *m_agentCollector = 0;
                if (d_agent->collector) {
                    m_agentCollector = new UvmCollector("Collector",m_agent);
                    m_agent->setCollector(m_agentCollector);

                    //Collector to Monitor
                    if (d_agent->monitor)
                        new UvmConnection ( UvmPort::newTlmPort(m_agentCollector->getShortName()+"MonitorIn", UvmPort::UVM_ANALYSIS_PORT, m_agentCollector),
                                            UvmPort::newTlmPort(m_agentMonitor->getShortName()+"CollectorOut", UvmPort::UVM_ANALYSIS_IMP, m_agentMonitor),
                                            linkName(m_agentCollector,m_agentMonitor),
                                            m_agent);

                    //To the interface
                    new UvmConnection ( UvmPort::newFakePort(m_vcInterface->getShortName()+"Agent"+m_agentCollector->getShortName(), UvmPort::VC_INTERFACE, UvmPort::PORT_INOUT, m_vcInterface),
                                        UvmPort::newFakePort(m_agentCollector->getShortName()+"InterfaceIn", UvmPort::VC_INTERFACE, UvmPort::PORT_IN, m_agentCollector),
                                        linkName(m_vcInterface,m_agentCollector),
                                        m_vc);
                }
                //If there is no collector but there is a monitor, then the monitor should be connected to the interface
                else if (d_agent->monitor) {
                    //Interface to monitor
                    new UvmConnection ( UvmPort::newFakePort(m_vcInterface->getShortName()+"Agent"+m_agentMonitor->getShortName(), UvmPort::VC_INTERFACE, UvmPort::PORT_INOUT, m_vcInterface),
                                        UvmPort::newFakePort(m_agentMonitor->getShortName()+"InterfaceIn", UvmPort::VC_INTERFACE, UvmPort::PORT_IN, m_agentMonitor),
                                        linkName(m_vcInterface,m_agentMonitor),
                                        m_vc);
                }

                //Driver
                UvmDriver *m_agentDriver = 0;
                if (d_agent->driver) {
                    m_agentDriver = new UvmDriver("Driver",m_agent);
                    m_agent->setDriver(m_agentDriver);

                    //Sequencer to Driver
                    if (d_agent->sequencer)
                        new UvmConnection ( UvmPort::newTlmPort(m_agentSequencer->getShortName()+"DriverOut", UvmPort::UVM_SEQ_ITEM_PULL_IMP, m_agentSequencer),
                                            UvmPort::newTlmPort(m_agentDriver->getShortName()+"SequencerIn", UvmPort::UVM_SEQ_ITEM_PULL_PORT, m_agentDriver),
                                            linkName(m_agentSequencer,m_agentDriver),
                                            m_agent);
/*
                    //Driver to Monitor
                    if(d_agent->monitor)
                        new UvmConnection (new UvmPort(m_agentDriver->getName()+"MonitorOut", UvmPort::UVM_BLOCKING_PUT_PORT, UvmPort::OUT, "", m_agentDriver),
                                           new UvmPort(m_agentMonitor->getName()+"DriverIn", UvmPort::UVM_BLOCKING_PUT_IMP, UvmPort::IN, "", m_agentMonitor),
                                           linkName(m_agentDriver, m_agentMonitor),
                                           m_agent);
*/
                    //To the interface
                    new UvmConnection ( UvmPort::newFakePort(m_vcInterface->getShortName()+"Agent"+m_agentDriver->getShortName(), UvmPort::VC_INTERFACE, UvmPort::PORT_INOUT, m_vcInterface),
                                        UvmPort::newFakePort(m_agentDriver->getShortName()+"InterfaceIn", UvmPort::VC_INTERFACE, UvmPort::PORT_IN, m_agentDriver),
                                        linkName(m_vcInterface,m_agentDriver),
                                        m_vc);
                }
                //If there is no driver but there is a sequencer, then the sequencer should be connected to the interface
                else if (d_agent->sequencer) {
                    //Sequencer to interface
                    new UvmConnection ( UvmPort::newFakePort(m_agentSequencer->getShortName()+"InterfaceIn", UvmPort::VC_INTERFACE, UvmPort::PORT_OUT, m_agentSequencer),
                                        UvmPort::newFakePort(m_vcInterface->getShortName()+"Agent"+m_agentSequencer->getShortName(), UvmPort::VC_INTERFACE, UvmPort::PORT_IN, m_vcInterface),
                                        linkName(m_agentSequencer,m_vcInterface),
                                        m_vc);
                }


                //Driver to monitor
                if (d_agent->driver2monitorLink && d_agent->monitor && d_agent->driver)
                    new UvmConnection ( UvmPort::newTlmPort(m_agentDriver->getShortName()+"MonitorOut", UvmPort::UVM_BLOCKING_PUT_PORT, m_agentDriver),
                                        UvmPort::newTlmPort(m_agentMonitor->getShortName()+"DriverIn", UvmPort::UVM_BLOCKING_PUT_IMP, m_agentMonitor),
                                        linkName(m_agentDriver,m_agentMonitor),
                                        m_agent);

                //Add the agent
                m_vc->addAgent(m_agent);
            }
        }

        //Add the VC
        m_testbench->addVerificationComponent(m_vc);
    }

    NameSetter nameSetter;
    nameSetter.setNames(project);

//    project->printTree();
    return true;
}

bool WizardDataConverter::project2Data(const UvmProject */*project*/,
                                       ProjectViewData */*d_project*/,
                                       TestCaseViewData */*d_testcase*/,
                                       TestBenchViewData */*d_testbench*/,
                                       SignalsViewData */*d_signals*/)
{
    errorString="Not yet implemented";

    return false;
}

//Use the name
QString WizardDataConverter::linkName(UvmComponent *from, UvmComponent *to) const
{
    return from->getShortName() + "To" + to->getShortName();
}


QString WizardDataConverter::getErrorString() const
{
    return errorString;
}
