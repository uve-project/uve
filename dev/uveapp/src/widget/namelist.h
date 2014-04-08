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
#ifndef NAMELIST_H
#define NAMELIST_H

#include <QWidget>
#include <QList>
#include <QString>
#include <QMultiMap>
#include <QLabel>
#include <QToolButton>
#include <QListView>
#include <QAbstractListModel>
#include <QTableWidget>

#include "stringlistmodel.h"
#include "uvmcomponent.h"
#include "connect.h"

/** Widget holding a list of QString assocciated with a value.
  This class acts like a list, but with a visual representation.
  This visual representation for each element is a string given when the object is added.
  To add an object, one must call the append() function. No duplicate in the names are allowed.
  The "Add" button is not automatic, one must use the addClicked() signal to know when the button has been clicked, and then append a value using append().
  However, the "Remove", "Up" and "Down" buttons works automatically, and the item is removed/upped/downed in the list.
  By default, the "Up" and "Down" buttons are hidden.
  !!! NOT A GENERAL PURPOSE CLASS, MAY HAVE INVALID BEHAVIOR !!!
  */
class NameList : public QWidget
{
    Q_OBJECT
public:
    /** Creates the GUI with an empty list
      */
    explicit NameList (QWidget *parent = 0);

    /** Append an object to the list
      To be displayed, an object needs a name.
      This name is given here, along with the object it's associated with.
      */
    void append (const QString &name, void *object);

    /** Remove the value associated with this name
      If more than one value has this name, only the first one is removed.
      */
    void remove (const QString &name);

    /** Remove the value at this place
      */
    void removeAt (uint place);

    /** Retrieve a specific object
      @param place Place of the wanted object in the list
      */
    void* value (uint place) const;

    /** Retrieve a specific object
      @param name The name of the wanted object
      */
    void* value (const QString &name) const;

    /** Retrieve a specific name
      @param place Place of the wanted name in the list
      */
    QString name (uint place) const;

    /** Retrieve all the objects
      All the objects given through append() can be retrieven here.
      The objects are not ordered as the list (yet, ask for BNZ if wanted)
      */
    QList<void*> values () const;

    /** Retrieve all the names
      All the names given through append() can be retrieven here.
      */
    QStringList names() const;

    /** Current list size
      */
    uint size () const;

    //! Remove all the elements from the list
    void clear ();

    //! Replace an element's name and value
    void replace (const QString &oldName, const QString &newName, void *newValue);

    /** Title above the list
      In the list view, the title is above the list and on the left of the add/remove buttons.
      This value does not corresponds to the widget name or window title.
      */
    void setTitle (const QString &title);
    QString getTitle () const;

    /** Show or hide the up/down buttons
      The up and down buttons let the user choose the place of an element on the list.
      */
    void hideUpDown (bool hide);
    void hideLoad (bool hide);
    void hideAdd (bool hide);
    bool isUpDownHidden () const;
    bool isLoadHidden () const;
    bool isAddHidden () const;

    void paintItemRed(uint place, bool red);

signals:

    //! The "Add" button has been clicked
    void addClicked ();

    //! The "load" button has been clicked
    void loadClicked ();

    void removeName(QString name);

    //! An item has been selected
    void selected (int place);

    //! An item has been double-clicked
    void activated (int place);

    //! An item has been added
    void appened (void *object);

    //! An item has been removed
    void removed (void *object);

private slots:

    //Remove the currently selected item
    void removeCurrent ();

    //Up the selection
    void up ();
    //Down the selection
    void down ();

    //Enable or disable the "Remove" button if there is nothing to remove
    void checkSize ();
    //Enable or disable the up/down buttons
    void checkUpDown ();

    //ModelIndex to row conversions
    void selectIndex (QModelIndex i);
    void activateIndex (QModelIndex i);

private:

    //Map of objects, both the names and the objects themselves
    QMap<QString,void*> _objects;

    //GUI objects
    QLabel *_title;
    QToolButton *b_load, *b_add, *b_remove, *b_up, *b_down;
    //QListView *_list;
    //StringListModel *_model;
    QTableWidget *_table;
};

#endif // NAMELIST_H
