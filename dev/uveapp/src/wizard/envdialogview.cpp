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
#include "envdialogview.h"
#include "ui_envdialogview.h"

#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QFileDialog>

#include "uvesettings.h"
#include "uvmvirtualsequencer.h"
#include "uvmscoreboard.h"
#include "uvmproject.h"
#include "newprojectwizard.h"
#include "vipconnectordialog.h"
#include "vhdlclasses.h"
#include "connect.h"
#include "uvexmlvalidator.h"

//Library config key
static const char* conf_library = "vip_library";

EnvDialogView::EnvDialogView(NewProjectWizard *parent) :
    QWizardPage(parent),
    ui(new Ui::EnvDialogView),
    _project(parent)
{
    ui->setupUi(this);

    //Setup the list
    ui->vcs->setTitle(tr("Verification Components :"));
    ui->vcs->hideAdd(false);

    //Restrict the input in some fields
    QRegExp rx("^[A-Za-z][0-9a-zA-Z_]*");
    ui->testbench->setValidator(new QRegExpValidator(rx, this));
    ui->testcase->setValidator(new QRegExpValidator(rx, this));

    //Connections
    CONNECT(ui->vcs, SIGNAL(addClicked()), this, SLOT(newVC()));
    CONNECT(ui->vcs, SIGNAL(activated(int)), this, SLOT(showVC(int)));
    CONNECT(ui->vcs, SIGNAL(loadClicked()), this, SLOT(loadVC()));
    CONNECT(ui->vcs, SIGNAL(removeName(QString)), this, SLOT(removeName(QString)));

    //Names check
    CONNECT(ui->testbench, SIGNAL(textChanged(QString)), this, SLOT(checkTestBenchName()));
    CONNECT(ui->testcase, SIGNAL(textChanged(QString)), this, SLOT(checkTestCaseName()));

    CONNECT(ui->sequencer, SIGNAL(toggled(bool)), this, SLOT(checkVCSequencerLink()));

}

EnvDialogView::~EnvDialogView()
{
    delete ui;
}


//Save them, and set the fields with their value
void EnvDialogView::setModels(TestBenchViewData *testbench, TestCaseViewData *testcase, VHDLFileContent *dut, const QString &dutEntityName)
{
    //Save
    _testbench = testbench;
    _testcase = testcase;
    _dut = dut;
    _dutEntityName = dutEntityName;

    //Names
    ui->testbench->setText(_testbench->name);
    ui->testcase->setText(_testcase->name);

    //TestBench modules
    ui->sequencer->setChecked(_testbench->virtualSequencer);
    ui->scoreboard->setChecked(_testbench->scoreboard);

    //VC list
    foreach (VCViewData *vc, _testbench->vcs)
        ui->vcs->append(vc->name,(void*)vc);
}


// Save in the models and check for the mandatory fields
bool EnvDialogView::validatePage()
{
    //Check for the names
    QString msg = checkTestCaseName() + "\n" + checkTestBenchName();

    //If there is a problem, display it
    if ( msg != "\n" ) {
        QMessageBox::critical(this,tr("Invalid name"),msg);
        return false;
    }

    if (ui->vcs->values().size()==0) {
        QMessageBox::critical(this,
                              tr("No VIP"),
                              tr("The project does not have any VIP. Please add at least one VIP"));
        return false;
    }

    //Save
    this->save();

    return true;
}

// Save in the models
void EnvDialogView::save()
{

    //Save the names
    _project->unRegisterNameAtProjectScope(_testcase->name);
    _testcase->name = ui->testcase->text();
    _project->registerNameAtProjectScope(_testcase->name);
    _project->unRegisterNameAtProjectScope(_testbench->name);
    _testbench->name = ui->testbench->text();
    _project->registerNameAtProjectScope(_testbench->name);

    //Modules
    _testbench->scoreboard = ui->scoreboard->isChecked();
    _testbench->virtualSequencer = ui->sequencer->isChecked();

    //Save the VC (remove the old, add the new)
    _testbench->vcs.clear();
    foreach (void *obj, ui->vcs->values())
        _testbench->vcs.append((VCViewData*)obj);
}

// Show the dialog, and append the value to the list
void EnvDialogView::newVC()
{
    //Create a new VC
    VCViewData *vc = new VCViewData;

    //Run the dialog, and save the resulting VC
    if (createDialog(vc,tr("New Verification Component")) == QDialog::Accepted)
        ui->vcs->append(vc->name,(void*)vc);
    else
        delete vc;
}


// Show the VC to the user, and refresh the list if accepted
void EnvDialogView::showVC(int place)
{
    //Fetch and copy the selected VC
    VCViewData *vc = (VCViewData*)ui->vcs->value(place);
    VCViewData *nvc = new VCViewData;
    *nvc = *vc;

    //Show the selected VC
    if (createDialog(nvc,tr("Verification Component")) == QDialog::Accepted) {
        //Replace in the list
        ui->vcs->replace(vc->name,nvc->name,(void*)nvc);
    } else
        delete nvc;
}

#include "uvexmlimport.h"
#include "uveconstants.h"

//Open the file dialog, load the designated VC, and open the connector dialog
void EnvDialogView::loadVC()
{
    //VIP library
    QString projectPath = _dut->getFileName();
    projectPath.truncate(_dut->getFileName().lastIndexOf("/") +1);
    QString library = UVESettings::getInstance()->value(conf_library,projectPath).toString();

    //File dialog
    QFileDialog dialog(this,tr("Open a VIP"),library,"VIP XML (*.uvc)");
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setDirectory(UVESettings::getInstance()->value(CONF_VIPFOLDER,UVE_LIB).toString());
    if ( dialog.exec() == QDialog::Rejected )
        return;

    //Create some needed objects
    QString error;
    UvmProject p;
    UvmTop top("top",&p);

    QList<PhysicalPort*> dutPorts = _dut->getPorts(_dutEntityName,0);

    //Load the VC
    QString vipFile = dialog.selectedFiles().at(0);
    UveXmlImport importer;
    UvmVerificationComponent* vc = importer.loadVerificationComponent(vipFile,0,false);
    if ( ! vc ) {


        QStringList faultyFiles = importer.getFaultyFiles();
        if(faultyFiles.isEmpty())
        {
            QMessageBox::critical(this,tr("Error while loading the VIP %1").arg(vipFile),importer.getErrorString());
            return;
        }
        foreach(QString faultyFile, faultyFiles)
        {
            UveXmlValidator v;
            v.setXML(faultyFile);
            v.exec();
        }

        return;
    }

    //Open the connection dialog, and fetch the result in a new VCViewData
    VIPConnectorDialog vipDialog (this);
    vipDialog.setup(vc->getInterface()->getPhysicalPorts(), dutPorts, vc->getShortName(), _project);
    if ( vipDialog.exec() == QDialog::Accepted ) {
        VCViewData *data = new VCViewData();
        data->file = vipFile;
        data->name = vipDialog.name();
        data->connections = vipDialog.connections();
        //Also append in the list
        ui->vcs->append(data->name,(void*)data);
    }

    UVESettings::getInstance()->setValue(CONF_VIPFOLDER, dialog.directory().path());
}


//Setup the dialog and set the VC inside it
int EnvDialogView::createDialog(VCViewData *vc, const QString &title)
{
    //Save us, for our data to be up-to-date
    this->save();

    //Create a dialog from the VCDialogView
    VCDialogView *vcd = new VCDialogView(_project);
    QDialog *d = new QDialog(this);
    d->setWindowTitle(title);
    QVBoxLayout *l = new QVBoxLayout(d);
    l->addWidget(vcd);

    //Add the Save/Cancel buttons
    QDialogButtonBox *buttons = new QDialogButtonBox();
    buttons->addButton(QDialogButtonBox::Save);
    buttons->addButton(QDialogButtonBox::Cancel);
    l->addWidget(buttons);
    CONNECT(buttons, SIGNAL(rejected()), d, SLOT(reject()));
    //Save the data in the model if the user clicks on "Save"
    CONNECT(buttons, SIGNAL(accepted()), vcd, SLOT(validate()));
    //If the view is validated, accept and quit
    CONNECT(vcd, SIGNAL(validated()), d, SLOT(accept()));

    //Set the VC in the dialog
    vcd->setModel(vc,_testbench,_dut,_dutEntityName);

    //Launch it
    return d->exec();
}

//Check the namespace and the test bench name
QString EnvDialogView::checkTestCaseName()
{
    //Empty name
    if (ui->testcase->text().isEmpty())
        return  tr("The test case name is empty");

    //Use the project namespace
    if ((_project->isNameUsedAtProjectScope(ui->testcase->text()) && ui->testcase->text() != _testcase->name)
            || (ui->testbench->text() == ui->testcase->text())) {
        ui->testcase->setStyleSheet("color:red;");
        return tr("The test case name is already used by another component") +"\n";
    } else
        ui->testcase->setStyleSheet("color:black;");

    return QString();
}

//Check the namespace and the test case name
QString EnvDialogView::checkTestBenchName()
{
    //Empty name
    if (ui->testbench->text().isEmpty())
        return tr("The testbench name is empty") + "\n";

    //Use the project namespace
    if ((_project->isNameUsedAtProjectScope(ui->testbench->text()) && ui->testbench->text() != _testbench->name)
            || (ui->testbench->text() == ui->testcase->text())) {
        ui->testbench->setStyleSheet("color:red;");
        return tr("The testbench name is already used by another component") +"\n";
    } else
        ui->testbench->setStyleSheet("color:black;");

    return QString();
}

void EnvDialogView::removeName(QString name)
{
    _testbench->unRegisterName(name);
}

void EnvDialogView::checkVCSequencerLink()
{

    if(!ui->sequencer->isChecked())
    {
        bool hasIncompatibleVC = false;
        for(int i=0; i<ui->vcs->values().length(); i++)
        {
            void* obj = ui->vcs->value(i);
            if(static_cast<VCViewData*>(obj)->virtualSequencerLink)
            {
                ui->vcs->paintItemRed(i,true);
                hasIncompatibleVC = true;
            }
        }
        if(hasIncompatibleVC)
        {
            QMessageBox::warning(this, "Warning", "One or more Verification Components has a link to the Virtual Sequencer. Please tick the Virtual Sequencer, or adapt the VCs (signaled in red). ");
        }
    }
    else
    {
        for(int i=0; i<ui->vcs->values().length(); i++)
        {
            ui->vcs->paintItemRed(i,false);
        }
    }
}
