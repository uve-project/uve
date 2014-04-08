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
#ifndef SIGNALSDIALOGVIEW_H
#define SIGNALSDIALOGVIEW_H

#include <QWizardPage>
#include <QAbstractItemDelegate>
#include <QAbstractTableModel>
#include <QStyledItemDelegate>
#include <QComboBox>

#include "viewdata.h"
#include "uvmproject.h"
#include "connect.h"

class ComboBoxItemDelegate;
class SignalsTableModel;

namespace Ui {
class SignalsDialogView;
}

/** Displays a list of signals
  Each signal can be set as being a clock, a reset or nothing.
  Only the IN signals are shown
  */
class SignalsDialogView : public QWizardPage
{
    Q_OBJECT

public:
    explicit SignalsDialogView(QWidget *parent = 0);
    ~SignalsDialogView();

    //! Set the model and fill the list
    void setModel (SignalsViewData *model);

    //! On init, re-set the model in the list
    void initializePage();

    //! Check and save in the model
    bool validatePage ();

    //! Also save in the model
    void cleanupPage ();

    //! Save in the model
    void save ();

public slots:

    //! Set the signal type (in the model and in the table model)
    void setSignalCategory (int categ, int row);

private slots:

    //Find the selected port and use showSettings()
    void select (QModelIndex index, QModelIndex);

    //Fill the settings part and enable/disable the buttons
    void showSettings (PhysicalPort *p);

    //Change the clock frequency
    void setClockFrequency (double freq);
    //Change the reset active value
    void setResetActive (int activeLow);
    //Change the reset minimal duration
    void setResetDuration (int tics);

private:
    //GUI
    Ui::SignalsDialogView *ui;
    //Model
    SignalsViewData *_model;
    //Table model
    SignalsTableModel *_tableModel;
    //Currently selected signal (in the model's ports)
    int _selectedSignal;
    //Type options
    QStringList _options;
};



/** Model for the table view
  Contains three rows : a signal name, a signal type, and its assignement.
  The assignement can be edited as a combobox, with the choice of being
  */
class SignalsTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit SignalsTableModel(QStringList usedOptions, QWidget *parent = 0);

    //! Return internal list size
    int rowCount (const QModelIndex &parent) const;

    //! Returns 3 (What a function!)
    int columnCount (const QModelIndex &parent) const;

    //! Enables the modification on the "Used as" column
    Qt::ItemFlags flags(const QModelIndex &index) const;

    //! Only the data can be (<= I'm not sure what this mean...?)
    QVariant data (const QModelIndex &index, int role) const;
    bool setData (const QModelIndex &index, const QVariant &value, int role);

    //! Returns the columns headers only
    QVariant headerData (int section, Qt::Orientation orientation, int role) const;

    //! Fills the list using this data
    void setModel (SignalsViewData *model);

    //! Set the signal category of the given row
    void setSignalCategory (int row, int categ);

private:
    //Lists for each column
    QStringList _names, _types, _used;
    //Our model (or sub-model, or whatever)
    SignalsViewData *_model;
    //List of combobox options
    QStringList _options;
};


/** A combobox storing its corresponding row in the model
  */
class RowComboBox : public QComboBox
{
    Q_OBJECT

public:
    //! Redefined to save the row
    RowComboBox(int row, QWidget *parent = 0);

signals:
    //! Is sent when the value changed
    void valueChanged (int categ, int row);

private slots:
    // To redirect the signal
    void changed (int categ);

private:
    // Our row
    int _row;
};


/** Item delegate showing a ComboBox
  To edit the widget type, a combobox must be shown.
  This delegate show a combobox, a setup it to fits the table
  */
class ComboBoxItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ComboBoxItemDelegate(QStringList options, SignalsDialogView *parent = 0);

    //! Creates the combobox
    QWidget* createEditor (QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    //! Set the combobox value in the model
    void setModelData (QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    //! Set the model value in the editor -- NOPE do nothing now
    void setEditorData (QWidget *editor, const QModelIndex &index) const;

    //! Fits the editor in the table
    void updateEditorGeometry (QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    //List of combobox options
    QStringList _options;
    //The controller
    SignalsDialogView *_parent;
};

#endif // SIGNALSDIALOGVIEW_H
