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
#include "stringlistmodel.h"

StringListModel::StringListModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

// From the list
int StringListModel::rowCount(const QModelIndex &) const
{
    return _list.size();
}

// From the list
uint StringListModel::size() const
{
    return _list.size();
}

// If the role is DisplayRole, return the list value
QVariant StringListModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
        return _list.at(index.row());
    return QVariant();
}

// From the list
QString StringListModel::at(uint place) const
{
    return _list.at(place);
}

QStringList StringListModel::list() const
{
    return _list;
}

// Append in the list, use insert()
void StringListModel::append(QString value)
{
    uint place = _list.size();
    this->insert(place,value);
}

//Insert at this palce
void StringListModel::insert(uint place, QString value)
{
    beginInsertRows(QModelIndex(),place,place);
    _list.insert(place,value);
    endInsertRows();
}

// Simple remove
void StringListModel::remove(uint place)
{
    beginRemoveRows(QModelIndex(),place,place);
    _list.removeAt(place);
    endRemoveRows();
}

// Find the place and use the other function
void StringListModel::remove(const QString &value)
{
    int i = _list.indexOf(value);
    if (i != -1)
        remove(i);
}

// Remove it, and insert it
void StringListModel::replace(uint place, QString newValue)
{
    this->remove(place);
    this->insert(place,newValue);
}

//Move in the list, and reset
QModelIndex StringListModel::move(int from, int to)
{
    beginResetModel();
    _list.move(from,to);
    endResetModel();

    //Return the new index
    return this->createIndex(to,0);
}

//Simply clear the list
void StringListModel::clear()
{
    beginResetModel();
    _list.clear();
    endResetModel();
}



