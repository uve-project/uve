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
#include "signalsdialogview.h"
#include "ui_signalsdialogview.h"

#include <QComboBox>
#include <QMessageBox>

//------------------------------------ VIEW ------------------------------------

SignalsDialogView::SignalsDialogView(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::SignalsDialogView)
{
    //Setup the GUI
    ui->setupUi(this);

    //Create the list options
    _options << tr("Other") << tr("Clock") << tr("Reset");

    //Set the model in the list
    _tableModel = new SignalsTableModel(_options,this);
    ui->table->setModel(_tableModel);

    //Set the item delegate in the list
    ui->table->setItemDelegateForColumn(2, new ComboBoxItemDelegate(_options,this));

    //Connect the selection signal to our slot
    connect(ui->table->selectionModel(),
            SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this,
            SLOT(select(QModelIndex,QModelIndex)));

    //Connect the widget signals
    CONNECT(ui->freq, SIGNAL(valueChanged(double)), this, SLOT(setClockFrequency(double)));
    CONNECT(ui->activeOn, SIGNAL(currentIndexChanged(int)), this, SLOT(setResetActive(int)));
    CONNECT(ui->tics, SIGNAL(valueChanged(int)), this, SLOT(setResetDuration(int)));
}

SignalsDialogView::~SignalsDialogView()
{
    delete ui;
}

//Setup the list (sooo, simple)
void SignalsDialogView::setModel(SignalsViewData *model)
{
    _model = model;
    this->initializePage();
}

//Setup the list, and copy the clocks & resets
void SignalsDialogView::initializePage()
{
    //Set the model in the model? Soooo... recursive!
    _tableModel->setModel(_model);
    //Resize the list
    ui->table->resizeColumnsToContents();

    //Reset the selected indexes
    _selectedSignal = -1;
}

//Save and check the signals
bool SignalsDialogView::validatePage()
{
    //Count the number of clock and reset
    int clocks = 0, resets = 0;
    foreach (PhysicalPort *p, _model->ports)
        switch (p->getCategory()) {
            case UvmPort::CLOCK : clocks++; break;
            case UvmPort::RESET : resets++; break;
            default : break;
        }

    //Verify that at least a clock and a model have been set
    if (clocks == 0 || resets == 0) {
        if(QMessageBox::warning(this,tr("No clock or reset defined"),
                             tr("At least one clock and one reset signal should be declared. Do you still want to continue?"),
                             QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
            return false;
    }

    save();

    return true;
}

//Save, but no check
void SignalsDialogView::cleanupPage()
{
    save();
}

//Well, in the past this function did something. Now it's not...
void SignalsDialogView::save()
{
}

//Change the current signal type
void SignalsDialogView::setSignalCategory(int categ, int row)
{
    //Find the current port
    PhysicalPort *p = _model->ports.at(row);

    //What a joke...
    if (p->getCategory() == categ)
        return;

    //Set and show
    p->setCategory(UvmPort::PortCategory(categ));

    //Refresh the settings if the signal is the selected one
    if (row == _selectedSignal)
        showSettings(p);

    //Don't forget the table model
    _tableModel->setSignalCategory(_selectedSignal,categ);
}

//Save the index and give the port to showSettings()
void SignalsDialogView::select(QModelIndex index, QModelIndex)
{
    //Save the index (only the row matters)
    _selectedSignal = index.row();

    //If none selected, quit right now
    if (_selectedSignal == -1)
        return;

    //Show the port settings
    showSettings(_model->ports.at(_selectedSignal));
}

//Fill the value and activate the inputs
void SignalsDialogView::showSettings(PhysicalPort *p)
{
    //Disable all by default
    ui->activeOn->setDisabled(true);
    ui->tics->setDisabled(true);
    ui->freq->setDisabled(true);

    //Reset signal
    if (p->getCategory() == UvmPort::RESET) {
        ui->activeOn->setDisabled(false);
        ui->activeOn->setCurrentIndex( p->getResetActiveLow() ? 1:0 );
        ui->tics->setDisabled(false);
        ui->tics->setValue(p->getResetDuration());
        return;
    }

    //Clock signal
    if (p->getCategory() == UvmPort::CLOCK) {
        ui->freq->setDisabled(false);
        ui->freq->setValue(p->getClockFrequency() / 1000000.0); //Hz -> MHz conversion
        return;
    }
}

//Change the currently selected clock frequency
void SignalsDialogView::setClockFrequency(double freq)
{
    //If there is no selected clock, quit
    if (_selectedSignal == -1)
        return;

    //Change the specified value (convert it in Hz)
    _model->ports.at(_selectedSignal)->setClockFrequency(freq * 1000000);
}

//Change the currently selected reset active value
void SignalsDialogView::setResetActive(int activeLow)
{
    //If there is no selected reset, quit
    if (_selectedSignal == -1)
        return;

    //Change the specified value
    _model->ports.at(_selectedSignal)->setResetActiveLow( activeLow == 1 );
}

//Change the currently selected reset dureation
void SignalsDialogView::setResetDuration(int tics)
{
    //If there is no selected reset, quit
    if (_selectedSignal == -1)
        return;

    //Change the specified value
    _model->ports.at(_selectedSignal)->setResetDuration(tics);
}

//------------------------------------ TABLEMODEL ------------------------------------

//Simple save (booooring)
SignalsTableModel::SignalsTableModel(QStringList usedOptions, QWidget *parent) :
    QAbstractTableModel(parent),
    _options(usedOptions)
{
}

//List size
int SignalsTableModel::rowCount(const QModelIndex &/*parent*/) const
{
    return _names.size();
}

//Return 3 (Oh my god!)
int SignalsTableModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 3;
}

//Return the default, except for column 3 (editable)
Qt::ItemFlags SignalsTableModel::flags(const QModelIndex &index) const
{
    if (index.column() == 2)
        return Qt::ItemIsEditable | Qt::ItemIsEnabled;
    else
        return Qt::ItemIsEnabled;
}

//Switch for each columns, fetch in the right list
QVariant SignalsTableModel::data(const QModelIndex &index, int role) const
{
    //Display only the name, nothing else
    if (role != Qt::DisplayRole)
        return QVariant();

    //Switch between the columns
    switch (index.column()) {
        case 0: return _names.at(index.row());
        case 1: return _types.at(index.row());
        case 2: return _used.at(index.row());
    }

    //What the hell are we doing here?
    return QVariant();
}

//Set the data in the right list (never called anyway...)
bool SignalsTableModel::setData(const QModelIndex &index, const QVariant &value, int /*role*/)
{
    //Switch between the columns
    switch (index.column()) {
        case 0: _names.replace(index.row(), value.toString()); break;
        case 1: _types.replace(index.row(), value.toString()); break;
        case 2: _used.replace(index.row(), value.toString()); break;
    }

    return true;
}

//Each header have its description
QVariant SignalsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    //Only horizontal headers
    if (orientation != Qt::Horizontal)
        return QVariant();

    //Name
    if (role == Qt::DisplayRole) {
        //For each section
        switch (section) {
            case 0: return tr("Name");
            case 1: return tr("Type");
            case 2: return tr("Used as");
        }
    }

    //Descripton
    if (role == Qt::ToolTipRole) {
        //For each section
        switch (section) {
            case 0: return tr("Name of the signals");
            case 1: return tr("Type of the signals");
            case 2: return tr("What kind of signal it is") + "\n"
                          +tr("Clock : use the signal as a clock, which will be linked two the test bench signal") + "\n"
                          +tr("Reset : use as a reset signal, will be called on need by the test bench") + "\n"
                          +tr("Other : the signal is neither a clock nor a reset, and shouldn't be used by the test bench");
        }
    }

    //Again, what have we missed?
    return QVariant();
}


// Load the internal lists from the model
void SignalsTableModel::setModel(SignalsViewData *model)
{
    beginResetModel();

    //Save it
    _model = model;

    //Clear the old stuff
    _names.clear();
    _types.clear();
    _used.clear();

    //Fill the lists
    foreach (PhysicalPort *port, _model->ports) {
        QString name = port->getName();
        _names.append(name);
        _types.append(port->getType());
        //Search for its type
        _used.append(_options.at(port->getCategory()));
    }

    endResetModel();
}

//Change the used list
void SignalsTableModel::setSignalCategory(int row, int categ)
{
    _used.replace(row,_options.at(categ));
}

//------------------------------------ COMBOBOX ------------------------------------

//Save it and connect our signal
RowComboBox::RowComboBox(int row, QWidget *parent) :
    QComboBox(parent)
{
    _row = row;
    CONNECT(this, SIGNAL(currentIndexChanged(int)), this, SLOT(changed(int)));
}

//Redirect it
void RowComboBox::changed(int categ)
{
    emit valueChanged(categ,_row);
}

//------------------------------------ ITEMDELEGATE ------------------------------------

//Create the options
ComboBoxItemDelegate::ComboBoxItemDelegate(QStringList options, SignalsDialogView *parent) :
    QStyledItemDelegate(parent),
    _options(options),
    _parent(parent)
{
}

//Setup the combobox
QWidget *ComboBoxItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &index) const
{
    //Create the box
    RowComboBox *box = new RowComboBox(index.row(),parent);
    box->addItems(_options);
    box->setFocusPolicy(Qt::StrongFocus);

    //Connect its change signal (faster than waiting on the setEditorData function)
    CONNECT(box, SIGNAL(valueChanged(int,int)), _parent, SLOT(setSignalCategory(int,int)));

    return box;
}

// Nothing to do
void ComboBoxItemDelegate::setModelData(QWidget *, QAbstractItemModel *, const QModelIndex &) const
{
}

//From the model
void ComboBoxItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *box = static_cast<QComboBox*>(editor);
    QString d = index.model()->data(index,Qt::DisplayRole).toString();
    box->setCurrentIndex(_options.indexOf(d));
}

//Fits in the box
void ComboBoxItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}
