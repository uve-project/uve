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
#include <QStandardItem>

#include "vipconnectordialog.h"
#include "ui_vipconnectordialog.h"
#include "connect.h"

//Create and connect the models
VIPConnectorDialog::VIPConnectorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VIPConnectorDialog),
    _vipSelected(false),
    _duvSelected(false)
{
    ui->setupUi(this);

    //Create the models
    _vipModel = new QStandardItemModel(this);
    _duvModel = new QStandardItemModel(this);
    _resultModel = new QStandardItemModel(this);

    //Connect them
    ui->vip->setModel(_vipModel);
    ui->duv->setModel(_duvModel);
    ui->result->setModel(_resultModel);

    //Selection
    CONNECT( ui->vip->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(vipSelected(QModelIndex,QModelIndex)) );
    CONNECT( ui->duv->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(duvSelected(QModelIndex,QModelIndex)) );
    CONNECT( ui->result->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(resultSelected(QModelIndex,QModelIndex)) );

    //Name check
    CONNECT( ui->name, SIGNAL(textChanged(QString)), this, SLOT(checkName()) );

    //Connect/disconnect buttons
    CONNECT( ui->connectSignal, SIGNAL(clicked()), this, SLOT(connectPorts()) );
    CONNECT( ui->disconnectSignal, SIGNAL(clicked()), this, SLOT(disconnectPorts()) );

    //At first, connect and disconnect are not selected
    ui->connectSignal->setEnabled(false);
    ui->disconnectSignal->setEnabled(false);
}

VIPConnectorDialog::~VIPConnectorDialog()
{
    delete ui;
}

//Clear the old lists and setup the new ones
void VIPConnectorDialog::setup(const QList<PhysicalPort*> &vipPorts, const QList<PhysicalPort*> &duvPorts, const QString &baseName, NewProjectWizard *project)
{
    //Save this
    _project = project;

    //Clear the old models
    _vipModel->clear();
    _duvModel->clear();
    _resultModel->clear();

    //Setup the VIP and DUV lists headers
    QStringList headers;
    headers << tr("Signal name") << tr("Direction") << tr("Signal type");
    _vipModel->setHorizontalHeaderLabels(headers);
    _duvModel->setHorizontalHeaderLabels(headers);

    //Setup the result list headers
    headers.clear();
    headers << tr("VIP signal") << tr("Direction") << tr("DUV signal");
    _resultModel->setHorizontalHeaderLabels(headers);

    //Fill the input lists
    this->fillList( _vipModel, vipPorts );
    this->fillList( _duvModel, duvPorts );

    //Resize the columns
    ui->vip->resizeColumnsToContents();
    ui->duv->resizeColumnsToContents();

    //Setup the name
    ui->name->setText(baseName);
}

//Fetch the list from the result list
QList<BaseConnection> VIPConnectorDialog::connections() const
{
    QList<BaseConnection> list;

    //Add the new ones, from the result list
    for (int row=0; row < _resultModel->rowCount(); row++) {
        //Fetch the ports from the model (stored in the first and third columns, hidden in UserRole)
        PhysicalPort *src = (PhysicalPort*)_resultModel->data( _resultModel->index(row,0), Qt::UserRole ).value<void*>();
        PhysicalPort *dst = (PhysicalPort*)_resultModel->data( _resultModel->index(row,2), Qt::UserRole ).value<void*>();
        //Create the connection
        BaseConnection bc;
        bc.src = src->getName();
        bc.dst = dst->getName();
        bc.name = bc.src + "To" + bc.dst;
        list << bc;
    }

    return list;
}

//Fetch it from the GUI
QString VIPConnectorDialog::name() const
{
    return ui->name->text();
}

//------------------------------ CONTROLLER ---------------------------------

//Add them in the result list
void VIPConnectorDialog::connectPorts(UvmPort *src, UvmPort *dst)
{
    QList<QStandardItem*> row;

    //Source Name and pointer
    QStandardItem *item = new QStandardItem(src->getName());
    item->setData(QVariant::fromValue((void*)src), Qt::UserRole);
    row.append( item );

    //Signal direction
    item = new QStandardItem();
    item->setIcon(this->directionIcon(src->getDirection()));
    row.append( item );

    //Destination Name and pointer
    item = new QStandardItem(dst->getName());
    item->setData(QVariant::fromValue((void*)dst), Qt::UserRole);
    row.append( item );

    //Add in the model
    _resultModel->appendRow(row);
}

//Fetch the two selected and connect them
void VIPConnectorDialog::connectPorts()
{
    //No guard, this "connect" button is disabled when no selection are present

    //Get the selected index, but use the first column
    QModelIndex srcIndex = _vipModel->index( ui->vip->selectionModel()->currentIndex().row(), 0);
    QModelIndex dstIndex = _duvModel->index( ui->duv->selectionModel()->currentIndex().row(), 0);

    //Fetch the ports from the model (stored in the first column, hidden in UserRole)
    UvmPort *src = (UvmPort*)_vipModel->data( srcIndex, Qt::UserRole ).value<void*>();
    UvmPort *dst = (UvmPort*)_duvModel->data( dstIndex, Qt::UserRole ).value<void*>();

    //Check on those thought
    if (!src || !dst)
        return;

    //Connect them
    this->connectPorts(src,dst);
}

//Simple remove the row from the list
void VIPConnectorDialog::disconnectPorts()
{
    //No guard, this "disconnect" button is disabled when no selection are present
    _resultModel->removeRow( ui->result->selectionModel()->currentIndex().row() );
}

// Enable/disable the connect and disconnect buttons
void VIPConnectorDialog::vipSelected(QModelIndex i, QModelIndex )
{
    _vipSelected = i.isValid();
    ui->connectSignal->setEnabled( _vipSelected && _duvSelected);
}

void VIPConnectorDialog::duvSelected(QModelIndex i, QModelIndex )
{
    _duvSelected = i.isValid();
    ui->connectSignal->setEnabled( _vipSelected && _duvSelected);
}

void VIPConnectorDialog::resultSelected(QModelIndex i, QModelIndex )
{
    ui->disconnectSignal->setEnabled(i.isValid());
}

//Check the namespace for the VIP name
void VIPConnectorDialog::checkName()
{
    //Empty name
    if (ui->name->text().isEmpty())
        return;

    //Use the project namespace
    if (_project->isNameUsedAtProjectScope(ui->name->text())) {
        ui->name->setStyleSheet("color:red;");
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    } else {
        ui->name->setStyleSheet("color:black;");
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}

//------------------------------ UTILITIES ---------------------------------

// Add each port as a row in the list, and place its pointer in the first column in UserType
void VIPConnectorDialog::fillList(QStandardItemModel *model, const QList<PhysicalPort *> &ports)
{
    //Add each port
    foreach (PhysicalPort *p, ports) {
        QList<QStandardItem*> row;

        //Name and pointer
        QStandardItem *item = new QStandardItem(p->getName());
        item->setData(QVariant::fromValue((void*)p), Qt::UserRole);
        row.append( item );

        //Signal direction
        item = new QStandardItem();
        item->setIcon(this->directionIcon(p->getDirection()));
        row.append( item );

        //Signal type
        row.append( new QStandardItem(p->getType()));

        //Add in the model
        model->appendRow(row);
    }
}

// Simple switch between the possible values
QIcon VIPConnectorDialog::directionIcon(UvmPort::PortDirection direction)
{
    switch (direction) {
        case UvmPort::PORT_IN : return QIcon(":images/arrow-right");
        case UvmPort::PORT_OUT : return QIcon(":images/arrow-left");
        case UvmPort::PORT_INOUT : return QIcon(":images/arrow-both");
        default : return QIcon();
    }
}
