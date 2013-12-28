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
#include "vcdialogview.h"
#include "ui_vcdialogview.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QStringList>

#include "agentdialogview.h"
#include "newprojectwizard.h"

//Setup the GUI
VCDialogView::VCDialogView(NewProjectWizard *parent) :
    QWidget(parent),
    ui(new Ui::VCDialogView),
    _project(parent)
{
    ui->setupUi(this);

    //Restrict the input in some fields
    QRegExp rx("^[A-Za-z][0-9a-zA-Z_]*");
    ui->vcname->setValidator(new QRegExpValidator(rx, this));

    //Setup the list
    ui->agents->setTitle(tr("Agents :"));
    ui->agents->hideLoad(true);
    ui->agents->hideAdd(false);

    //Create the model and setup the ports list
    _ports = new CheckableStringListModel();
    ui->connections->setModel(_ports);


    _comparatorTypeGroup = new QButtonGroup(this);
    _comparatorTypeGroup->addButton(ui->radioButtonComparator0,0);
    _comparatorTypeGroup->addButton(ui->radioButtonComparator1,1);
    _comparatorTypeGroup->addButton(ui->radioButtonComparator2,2);
    _comparatorTypeGroup->setExclusive(true);

    //New agent
    CONNECT(ui->agents, SIGNAL(addClicked()), this, SLOT(newAgent()));
    //Pop-up agent
    CONNECT(ui->agents, SIGNAL(activated(int)), this, SLOT(showAgent(int)));
    CONNECT(ui->agents, SIGNAL(removeName(QString)), this, SLOT(removeName(QString)));

    CONNECT(ui->scoreboard, SIGNAL(toggled(bool)), this, SLOT(checkScoreboard()));

    //Name collision check
    CONNECT(ui->vcname, SIGNAL(textChanged(QString)), this, SLOT(checkName()));

    //To check if the sequencer link checkbox should be enabled
    CONNECT(ui->sequencer, SIGNAL(toggled(bool)), this, SLOT(checkSequencer()));
    CONNECT(ui->sequencer, SIGNAL(toggled(bool)), this, SLOT(checkAgentSequenceLinks()));

    //All/None and select connections
    CONNECT(_ports, SIGNAL(checked(bool,int)), this, SLOT(checked()));
    CONNECT(ui->all, SIGNAL(clicked(bool)), this, SLOT(checkAll(bool)));
    CONNECT(ui->none, SIGNAL(clicked(bool)), this, SLOT(unCheckAll(bool)));
}

VCDialogView::~VCDialogView()
{
    delete ui;
    delete _ports;
}

//Set the fields
void VCDialogView::setModel(VCViewData *vc, TestBenchViewData *parent, VHDLFileContent *dut,const QString &dutEntityName)
{
    //Set to true if model exists
    bool firstLoad = vc->name == NULL;

    //Save the models
    _vc = vc;
    _parent = parent;
    _dut = dut;
    _dutEntityName = dutEntityName;

    //Name & description
    ui->vcname->setText(_vc->name);
    ui->description->clear();
    ui->description->appendPlainText(_vc->description);

    //Modules
    ui->sequencer->setChecked(_vc->virtualSequencer);
    ui->scoreboard->setChecked(_vc->scoreboard);
    switch(_vc->scoreboardComparatorType)
    {
    case 0: ui->radioButtonComparator0->setChecked(true);break;
    case 1: ui->radioButtonComparator1->setChecked(true);break;
    case 2: ui->radioButtonComparator2->setChecked(true);break;
    }

    ui->monitor->setChecked(_vc->busMonitor);

    //Link between virtual sequencer
    ui->linkSequencer->setChecked(_vc->virtualSequencerLink);

    //Agent list
    foreach (AgentViewData *agent, _vc->agents)
        ui->agents->append(agent->name,(void*)agent);

    // Construct the ports list
    QStringList list = _dut->getPortNames(_dutEntityName);
    _ports->setStringList(list);

    //Check the selected ones
    foreach (QString name, _vc->interConnects) {
        QModelIndex index = _ports->index(list.indexOf(name));
        _ports->setData(index,QVariant(Qt::Checked),Qt::CheckStateRole);
    }

    //Default checks
    checkSequencer();

    checkScoreboard();

    //By default, check all the signals
    if (firstLoad)
        checkAll(true);
}

//Save the fields in the model
void VCDialogView::save()
{
    //Save the name
    _parent->unRegisterName(_vc->name);
    _vc->name = ui->vcname->text();
    _parent->registerName(_vc->name);

    //Description
    _vc->description = ui->description->toPlainText();

    //Modules
    _vc->busMonitor = ui->monitor->isChecked();
    _vc->virtualSequencer = ui->sequencer->isChecked();
    _vc->scoreboard = ui->scoreboard->isChecked();
    _vc->scoreboardComparatorType = _comparatorTypeGroup->checkedId();

    //Sequencer link
    _vc->virtualSequencerLink = ui->linkSequencer->isChecked();

    //Save the VC (remove the old, add the new)
    _vc->agents.clear();
    foreach (void *obj, ui->agents->values())
        _vc->agents.append((AgentViewData*)obj);

    //Save the connected signals list
    _vc->interConnects.clear();
    for (int i=0; i<_ports->rowCount(); i++)
        if (_ports->data(_ports->index(i),Qt::CheckStateRole).toInt() == Qt::Checked)
            _vc->interConnects.append(_ports->data(_ports->index(i),Qt::DisplayRole).toString());
}

//Save and check the name
bool VCDialogView::validate()
{
    //Name check
    if (!checkName()) {
        QMessageBox::critical(this,
                              tr("Invalid name"),
                              tr("The name of this verification component is already used by another verification component in the same test bench or is empty.") + "\n"
                              + tr("Please choose another name"));
        return false;
    }

    if (ui->agents->size()==0) {
        QMessageBox::critical(this,
                              tr("No agent"),
                              tr("There should be at least one agent in a VIP"));
        return false;
    }

    //Save
    this->save();

    emit validated();
    return true;
}

// Show the dialog, and append the value to the list
void VCDialogView::newAgent()
{
    //Create a new VC
    AgentViewData *agent = new AgentViewData;

    //Default values
    agent->monitor = true;
    agent->sequencer = true;
    agent->driver = true;

    //Run the dialog, and save the resulting VC
    if (createDialog(agent,tr("New Agent")) == QDialog::Accepted)
        ui->agents->append(agent->name,(void*)agent);
    else
        delete agent;
}


// Show the VC to the user, and refresh the list if accepted
void VCDialogView::showAgent(int place)
{
    //Fetch and copy the selected VC
    AgentViewData *agent = (AgentViewData*)ui->agents->value(place);
    AgentViewData *nagent = new AgentViewData;
    *nagent = *agent;

    //Show the selected VC
    if (createDialog(nagent,tr("Agent")) == QDialog::Accepted) {
        //Replace in the list
        ui->agents->replace(agent->name,nagent->name,(void*)nagent);
    } else
        delete nagent;
}

//Unckech both all and none
void VCDialogView::checked()
{
    ui->all->setChecked(false);
    ui->none->setChecked(false);
}

//Loop through each ports and check them
void VCDialogView::checkAll(bool val)
{
    if (val)
        for (int i=0; i<_ports->rowCount(); i++)
            _ports->setData(_ports->index(i),QVariant(Qt::Checked),Qt::CheckStateRole);

    ui->none->setChecked(false);
    ui->all->setChecked(true);
}

//Loop through each ports and uncheck them
void VCDialogView::unCheckAll(bool val)
{
    if (val)
        for (int i=0; i<_ports->rowCount(); i++)
            _ports->setData(_ports->index(i),QVariant(Qt::Unchecked),Qt::CheckStateRole);


    ui->all->setChecked(false);
    ui->none->setChecked(true);
}


//Setup the dialog and set the agent inside it
int VCDialogView::createDialog(AgentViewData *agent, const QString &title)
{
    //Save our data
    this->save();

    //Create a dialog from the VCDialogView
    AgentDialogView *ad = new AgentDialogView(_project);
    QDialog *d = new QDialog(this);
    d->setWindowTitle(title);
    QVBoxLayout *l = new QVBoxLayout(d);
    l->addWidget(ad);

    //Add the Save/Cancel buttons
    QDialogButtonBox *buttons = new QDialogButtonBox();
    buttons->addButton(QDialogButtonBox::Save);
    buttons->addButton(QDialogButtonBox::Cancel);
    l->addWidget(buttons);
    CONNECT(buttons, SIGNAL(rejected()), d, SLOT(reject()));
    //Save the data in the model if the user clicks on "Save"
    CONNECT(buttons, SIGNAL(accepted()), ad, SLOT(validate()));
    //If the view is validated, accept and quit
    CONNECT(ad, SIGNAL(validated()), d, SLOT(accept()));

    //Set the Agent in the dialog
    ad->setModel(agent,_vc);

    //Launch it
    return d->exec();
}

//Check if any other component has the same name
bool VCDialogView::checkName()
{
    //Use the project namespace (ignore our own name)
    if (_parent->isNameUsed(ui->vcname->text()) && ui->vcname->text() !=_vc->name) {
        ui->vcname->setStyleSheet("color:red;");
        return false;
    } else {
        ui->vcname->setStyleSheet("color:black;");
        //Last check : is the name empty? It shouldn't
        return !ui->vcname->text().isEmpty();
    }
}

//Check if the two necessary components are present
bool VCDialogView::checkSequencer()
{
    bool ok = ui->sequencer->isChecked() && _parent->virtualSequencer;
    //Disable if one is missing
    ui->linkSequencer->setEnabled(ok);
    if(!ok)
        ui->linkSequencer->setChecked(false);
    return ok;
}

void VCDialogView::removeName(QString name)
{
    _vc->unRegisterName(name);
}

void VCDialogView::checkAgentSequenceLinks()
{
    if(!ui->sequencer->isChecked())
    {
        bool hasIncompatibleAgent = false;
        for(int i=0; i<ui->agents->values().length(); i++)
        {
            void* obj = ui->agents->value(i);
            if(static_cast<AgentViewData*>(obj)->sequencerLink)
            {
                ui->agents->paintItemRed(i, true);
                hasIncompatibleAgent = true;
            }
        }
        if(hasIncompatibleAgent)
        {
            QMessageBox::warning(this, "Warning", "One or more agent has a link to the Virtual Sequencer. Please tick the Virtual Sequencer, or adapt the agents (signaled in red). ");
        }
    }
    else
    {
        for(int i=0; i<ui->agents->values().length(); i++)
        {
            ui->agents->paintItemRed(i, false);
        }
    }
}

void VCDialogView::checkScoreboard()
{
    if (ui->scoreboard->isChecked())
    {
        ui->radioButtonComparator0->setEnabled(true);
        ui->radioButtonComparator1->setEnabled(true);
        ui->radioButtonComparator2->setEnabled(true);
    }
    else
    {
        ui->radioButtonComparator0->setEnabled(false);
        ui->radioButtonComparator1->setEnabled(false);
        ui->radioButtonComparator2->setEnabled(false);
    }
}

//-------------------------- STRING MODEL -----------------------------

//We are checkable
Qt::ItemFlags CheckableStringListModel::flags(const QModelIndex &) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
}

QVariant CheckableStringListModel::data( const QModelIndex& index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    if (role == Qt::CheckStateRole)
        return _checked.value(index.row(),Qt::Unchecked);
    else
        return QStringListModel::data(index,role);
}


bool CheckableStringListModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
    if ( !index.isValid() )
        return false;

    if ( role == Qt::CheckStateRole ) {

        _checked.insert(index.row(),Qt::CheckState(value.toInt()));
        emit checked(value.toBool(), index.row());
        emit dataChanged(index,index);
    } else
        return QStringListModel::setData(index,value,role);

    return true;
}
