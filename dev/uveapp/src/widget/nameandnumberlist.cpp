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

#include <QVBoxLayout>
#include <QHeaderView>
#include <QSpinBox>

#include "nameandnumberlist.h"
//#include "uvmagent.h"

NameAndNumberList::NameAndNumberList(QWidget *parent) :
    QWidget(parent)
{
    //Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    //Horizontal layout for the title and button
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->setStretch(0,1);
    mainLayout->addLayout(hLayout);

    //Title (empty)
    _title = new QLabel();
    hLayout->addWidget(_title);

    //Spacer
    QSpacerItem *spacer = new QSpacerItem(0,0);
    hLayout->addSpacerItem(spacer);
    hLayout->setStretch(1,1);

    //Buttons
    b_load = new QToolButton();
    b_load->setIcon(QIcon(":/images/open.png"));
    b_load->setToolTip(QString("Load existing"));
    b_up = new QToolButton();
    b_up->setIcon(QIcon::fromTheme("go-up",QIcon(":images/up.png")));
    b_down = new QToolButton();
    b_down->setIcon(QIcon::fromTheme("go-down",QIcon(":images/down.png")));
    b_add = new QToolButton();
    b_add->setIcon(QIcon::fromTheme("list-add",QIcon(":images/add.png")));
    b_add->setToolTip(QString("Add new"));
    b_remove = new QToolButton();
    b_remove->setIcon(QIcon::fromTheme("list-remove",QIcon(":images/delete.png")));
    b_remove->setToolTip(QString("Remove"));
    hLayout->addWidget(b_up);
    hLayout->addWidget(b_down);
    hLayout->addWidget(b_load);
    hLayout->addWidget(b_add);
    hLayout->addWidget(b_remove);

    //The list and its model
    _table = new QTableWidget();
    _table->setColumnCount(2);
    _table->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Interactive);
    _table->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    _table->setColumnWidth(0,200);
    _table->setSelectionBehavior(QAbstractItemView::SelectRows);
    _table->setMinimumHeight(100);

    QStringList header;
    header<<"Agent Name"<<"Number";
    _table->setHorizontalHeaderLabels(header);
    mainLayout->addWidget(_table);

    //Connect the buttons
    CONNECT(b_up, SIGNAL(clicked()), this, SLOT(up()));
    CONNECT(b_down, SIGNAL(clicked()), this, SLOT(down()));
    CONNECT(b_add, SIGNAL(clicked()), this, SIGNAL(addClicked()));
    CONNECT(b_load, SIGNAL(clicked()), this, SIGNAL(loadClicked()));
    CONNECT(b_remove, SIGNAL(clicked()), this, SLOT(removeCurrent()));

    //Enable/disables the buttons
    CONNECT(this, SIGNAL(appened(void*)), this, SLOT(checkSize()));
    CONNECT(this, SIGNAL(removed(void*)), this, SLOT(checkSize()));
    CONNECT(this, SIGNAL(appened(void*)), this, SLOT(checkUpDown()));
    CONNECT(this, SIGNAL(removed(void*)), this, SLOT(checkUpDown()));
    CONNECT(_table, SIGNAL(clicked(QModelIndex)), this, SLOT(checkUpDown()));
    CONNECT(_table->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(checkUpDown()));

    //Redirect list signals
    CONNECT(_table, SIGNAL(activated(QModelIndex)), this, SLOT(activateIndex(QModelIndex)));
    CONNECT(_table, SIGNAL(clicked(QModelIndex)), this, SLOT(selectIndex(QModelIndex)));

    //By default, do not show all buttons
    hideUpDown(true);
    hideAdd(true);
    //And they are disables
    b_up->setEnabled(false);
    b_down->setEnabled(false);
    //The remove button too
    b_remove->setEnabled(false);

    //Lower our minimal height and change our vertical policy
    this->setMinimumHeight(100);
    this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
}

// Append to the map/model
void NameAndNumberList::append(const QString &name, void *object)
{
    //Exit if the item name is already here (no duplicate)
    if (_objects.contains(name))
        return;

    //Append in the map
    _objects.insert(name,object);

    int l = _table->rowCount();
    _table->setRowCount(_table->rowCount() +1);

    AgentViewData* agent = static_cast<AgentViewData*>(object);
    //Append in the model
    //_model->append(name);
    QTableWidgetItem* item = new QTableWidgetItem(name);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    _table->setItem(l,0,item);
    QSpinBox* spinBox = new QSpinBox();
    spinBox->setMinimum(1);
    spinBox->setValue(agent->nbAgents);
    CONNECT(spinBox, SIGNAL(valueChanged(int)), this, SLOT(aSpinBoxHasChanged()));
    _table->setCellWidget(l, 1, spinBox);

    //Send the signal
    emit appened(object);
}

// Remove from the map/model
void NameAndNumberList::remove(const QString &name)
{
    //If there is nothing to remove, quit
    if (_objects.isEmpty())
        return;

    //Remove from the model
    //_model->remove(name);
    for(int i=0; i<_table->rowCount(); i++)
    {
        if(_table->item(i, 0)->text().compare(name) == 0)
        {
            _table->removeRow(i);
            break;
        }
    }

    //Remove from the map
    void* obj = _objects.value(name);
    _objects.remove(name);

    //Emit the signal
    emit removed(obj);
}

// Remove from the model, and use the name at this place to remove it from the map
void NameAndNumberList::removeAt(uint place)
{
    //Fetch the name from the place
    QString name = _table->item(place, 0)->text();

    //Use the other function
    remove(name);
}

// Return the value in the map, whose name is found in the model
void* NameAndNumberList::value(uint place) const
{
    return _objects.value(_table->item(place, 0)->text());
}

// Directly return from the map
void *NameAndNumberList::value(const QString &name) const
{
    return _objects.value(name);
}

// Use the model to find this value
QString NameAndNumberList::name(uint place) const
{
    return _table->item(place, 0)->text();
}

// The map values
QList<void *> NameAndNumberList::values() const
{
    return _objects.values();
}

// The map keys
QStringList NameAndNumberList::names() const
{
    QStringList result = QStringList();
    for(int i=0;i<_table->rowCount(); i++)
    {
        result.append(_table->item(i, 0)->text());
    }
    return result;
}

// Use the model size
uint NameAndNumberList::size() const
{
    return _table->rowCount();
}

// Empty the model, map and index
void NameAndNumberList::clear()
{
    _table->clear();
    _objects.clear();
}

// Find the old name, and replace it in the list and the map
void NameAndNumberList::replace(const QString &oldName, const QString &newName, void *newValue)
{
    //Simple search
    int i;
    for (i=0; i<_table->rowCount(); i++)
        if (_table->item(i, 0)->text().compare(oldName) == 0)
            break;

    //If not found, exit
    if (i == _table->rowCount())
        return;

    //Replace at this place
    _table->item(i, 0)->setText(newName);
    static_cast<QSpinBox*>(_table->cellWidget(i,1))->setValue((static_cast<AgentViewData*>(newValue))->nbAgents);

    //Replace in the objects map (remove, add)
    _objects.remove(oldName);
    _objects.insert(newName,newValue);
}

// Apply the value in the GUI
void NameAndNumberList::setTitle(const QString &title)
{
    _title->setText(title);
}

// Fetch from the GUI
QString NameAndNumberList::getTitle() const
{
    return _title->text();
}

// Show/hide the up/down buttons
void NameAndNumberList::hideUpDown(bool hide)
{
    b_up->setHidden(hide);
    b_down->setHidden(hide);
}

void NameAndNumberList::hideLoad(bool hide)
{
    b_load->setHidden(hide);
}

void NameAndNumberList::hideAdd(bool hide)
{
    b_add->setHidden(hide);
}

// Fetch directly from the GUI, hmmm, fresh!
bool NameAndNumberList::isUpDownHidden() const
{
    return b_up->isHidden();
}

bool NameAndNumberList::isLoadHidden() const
{
    return b_load->isHidden();
}

bool NameAndNumberList::isAddHidden() const
{
    return b_add->isHidden();
}

//Search for the currently selected and erase it
void NameAndNumberList::removeCurrent()
{
    //Remove the selected row (if the current is valid)
    int row = _table->currentIndex().row();
    if (row != -1)
    {
        removeName(_table->item(row, 0)->text());
        removeAt(row);
    }
}

//Move the item up in the model and in the index map
void NameAndNumberList::up()
{
    //Fetch the row ID
    int row = _table->currentRow();

    //Ignore if the row is on the top or invalid
    if (row <= 0 ) return;

    //Move it in the model
    _table->move(row, row-1);
}

//Move the item down in the model and in the index map
void NameAndNumberList::down()
{
    //Fetch the row ID
    int row = _table->currentRow();

    //Ignore if the row is on the bottom or invalid
    if (row >= _objects.size()-1 || row == -1) return;

    //Move it in the model
    _table->move(row,row+1);
}

//If the list is empty, disable the button
void NameAndNumberList::checkSize()
{
    b_remove->setDisabled(_objects.isEmpty());
}

//By the way, enable or disable the up/down buttons when on top/bottom
void NameAndNumberList::checkUpDown()
{
    //Fetch the row ID
    int row = _table->currentIndex().row();

    //If on top
    b_up->setEnabled(row != 0 && row != -1);

    //If on the bottom
    b_down->setEnabled(row < _objects.size()-1 && row != -1);
}

//These two just translates an index to a row

void NameAndNumberList::selectIndex(QModelIndex i)
{
    emit selected(i.row());
}

void NameAndNumberList::activateIndex(QModelIndex i)
{
    emit activated(i.row());
}

void NameAndNumberList::aSpinBoxHasChanged()
{
    for(int i=0; i< _table->rowCount(); i++)
    {
        AgentViewData* currentAgent = static_cast<AgentViewData*>(_objects.value(_table->item(i, 0)->text()));
        currentAgent->nbAgents = static_cast<QSpinBox*>(_table->cellWidget(i,1))->value();
    }
}

void NameAndNumberList::paintItemRed(uint place, bool red)
{
    if(red)
    {
        _table->item(place, 0)->setTextColor(Qt::red);
    }
    else
    {
        _table->item(place, 0)->setTextColor(Qt::black);
    }
}
