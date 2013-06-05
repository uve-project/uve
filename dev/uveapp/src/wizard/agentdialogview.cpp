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
#include "agentdialogview.h"
#include "ui_agentdialogview.h"

#include <QMessageBox>

#include "uvmvirtualsequencer.h"

//Setup the GUI
AgentDialogView::AgentDialogView(NewProjectWizard *parent) :
    QWidget(parent),
    ui(new Ui::AgentDialogView),
    _project(parent)
{
    //GUI
    ui->setupUi(this);

    //Restrict the input in some fields
    QRegExp rx("^[A-Za-z][0-9a-zA-Z_]*");
    ui->name->setValidator(new QRegExpValidator(rx, this));

    //No collector without a monitor
    CONNECT(ui->monitor, SIGNAL(toggled(bool)), this, SLOT(checkCollector()));

    //Monitor-to-sequence check needs the monitor and the sequencer
    CONNECT(ui->sequencer, SIGNAL(toggled(bool)), this, SLOT(checkMonToSeq()));
    CONNECT(ui->monitor, SIGNAL(toggled(bool)), this, SLOT(checkMonToSeq()));

    //Monitor coverage check needs the monitor
    CONNECT(ui->monitor, SIGNAL(toggled(bool)), this, SLOT(checkMonCov()));

    //Virtual sequencer to sequencer check needs the sequencer
    CONNECT(ui->sequencer, SIGNAL(toggled(bool)), this, SLOT(checkVSToSeq()));

    CONNECT(ui->driver, SIGNAL(toggled(bool)), this, SLOT(checkDriverToMonitor()));
    CONNECT(ui->monitor, SIGNAL(toggled(bool)), this, SLOT(checkDriverToMonitor()));

    CONNECT(ui->driver, SIGNAL(toggled(bool)), this, SLOT(checkSequencer()));

    //Name collision check
    CONNECT(ui->name, SIGNAL(textChanged(QString)), this, SLOT(checkName()));
}

AgentDialogView::~AgentDialogView()
{
    _parent->unRegisterName(_agent->name);
    delete ui;
}

//Set the model data in the GUI fields
void AgentDialogView::setModel(AgentViewData *agent, VCViewData *parent)
{
    //Save them
    _agent = agent;
    _parent = parent;

    //Name
    ui->name->setText(_agent->name);

    //Modules
    ui->monitor->setChecked(_agent->monitor);
    ui->sequencer->setChecked(_agent->sequencer);
    ui->collector->setChecked(_agent->collector);
    ui->driver->setChecked(_agent->driver);

    //Links
    ui->monitorToSequencer->setChecked(_agent->monitorLink);
    ui->virtualToSequencer->setChecked(_agent->sequencerLink);
    ui->driverToMonitor->setChecked(_agent->driver2monitorLink);

    //Coverage
    ui->monitorCoverage->setChecked(_agent->monitorCoverage);

    //Enable/disable the necessary values
    checkMonToSeq();
    checkVSToSeq();
    checkMonCov();
    checkDriverToMonitor();
    checkCollector();
}

//Save the data in the model
void AgentDialogView::save()
{
    //Name
    _parent->unRegisterName(_agent->name);
    _agent->name = ui->name->text();
    _parent->registerName(_agent->name);

    //Modules
    _agent->monitor = ui->monitor->isChecked();
    _agent->sequencer = ui->sequencer->isChecked();
    _agent->collector = ui->collector->isChecked() && ui->collector->isEnabled();
    _agent->driver = ui->driver->isChecked() && ui->driver->isEnabled();

    //Links
    _agent->monitorLink = ui->monitorToSequencer->isChecked();
    _agent->sequencerLink = ui->virtualToSequencer->isChecked();
    _agent->driver2monitorLink = ui->driverToMonitor->isChecked();

    //Coverage
    _agent->monitorCoverage = ui->monitorCoverage->isChecked();
}

//Save and check the name
bool AgentDialogView::validate()
{
    //Show a message box if the agent name is already used
    if (!checkName()) {
        QMessageBox::critical(this,
                              tr("Invalid name"),
                              tr("The name of this agent is already used by another agent in the same verification component.") + "\n"
                              + tr("Please choose another name"));
        return false;
    }

    //Save
    this->save();

    emit validated();
    return true;
}

//If the monitor or the sequencer are not here, disable the checkbox
void AgentDialogView::checkMonToSeq()
{
    ui->monitorToSequencer->setEnabled( ui->monitor->isChecked() && ui->sequencer->isChecked() );
    if(!ui->monitorToSequencer->isEnabled())
        ui->monitorToSequencer->setChecked(false);
}

//If the sequencer is not here, disable the checkbox
void AgentDialogView::checkVSToSeq()
{
    ui->virtualToSequencer->setEnabled( ui->sequencer->isChecked() && _parent->virtualSequencer);
    if(!ui->virtualToSequencer->isEnabled())
        ui->virtualToSequencer->setChecked(false);
}

void AgentDialogView::checkDriverToMonitor()
{
    ui->driverToMonitor->setEnabled( ui->driver->isChecked() && ui->monitor->isChecked());
    if(!ui->driverToMonitor->isEnabled())
        ui->driverToMonitor->setChecked(false);
}

//If the monitor is not here, disable the checkbox
void AgentDialogView::checkMonCov()
{
    ui->monitorCoverage->setEnabled( ui->monitor->isChecked() );
    if(!ui->monitorCoverage->isEnabled())
        ui->monitorCoverage->setChecked(false);
}

void AgentDialogView::checkSequencer()
{
    ui->sequencer->setEnabled( ui->driver->isChecked() );
    if(!ui->sequencer->isEnabled())
        ui->sequencer->setChecked(false);
}

void AgentDialogView::checkCollector()
{
    ui->collector->setEnabled(ui->monitor->isChecked());
    if(!ui->collector->isEnabled())
        ui->collector->setChecked(false);
}

//Check if any other agent have the same name, and display the name in red if it is
bool AgentDialogView::checkName()
{
    //Use the project namespace (ignore our own name)
    if (_parent->isNameUsed(ui->name->text()) && ui->name->text() != _agent->name) {
        ui->name->setStyleSheet("color:red;");
        return false;
    } else {
        ui->name->setStyleSheet("color:black;");
        //Last check : is the name empty? It shouldn't
        return !ui->name->text().isEmpty();
    }
}
