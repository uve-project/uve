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
#ifndef STRINGLISTMODEL_H
#define STRINGLISTMODEL_H

#include <QAbstractListModel>
#include <QStringList>

/** Simple QString list model for a QListView
  Implemented for the NameList object.
  Adding and removing items is possible.
  */
class StringListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit StringListModel(QObject *parent = 0);

    //! The list size
    int rowCount (const QModelIndex &parent) const;

    //! The list size
    uint size () const;

    //! Display the corresponding string if the role is DisplayRole
    QVariant data (const QModelIndex &index, int role) const;

    //! Return the name at this place
    QString at (uint place) const;

    //! Return a list of all the items (same order as intern)
    QStringList list () const;

    //! Append in the list
    void append (QString value);

    //! Insert in the list
    void insert (uint place, QString value);

    //! Remove from the list
    void remove (uint place);

    //! Search and remove from the list
    void remove (const QString &value);

    //! Replace an value in the list
    void replace (uint place, QString newValue);

    //! Move an item in the list
    QModelIndex move(int from, int to);

    //! Remove all the elements from the list
    void clear ();
    
signals:
    
public slots:

private:
    //Internal storing object
    QStringList _list;
    
};

#endif // STRINGLISTMODEL_H
