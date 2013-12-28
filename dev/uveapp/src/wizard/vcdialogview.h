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
#ifndef VCDIALOGVIEW_H
#define VCDIALOGVIEW_H

#include <QWidget>
#include <QStringListModel>

#include "uvmverificationcomponent.h"
#include "uvmtestbench.h"
#include "uvmagent.h"
#include "viewdata.h"
#include "connect.h"

namespace Ui {
class VCDialogView;
}

struct TestBenchViewData;
class CheckableStringListModel;
class NewProjectWizard;

class VCDialogView : public QWidget
{
    Q_OBJECT

public:

    //! Setup the GUI
    explicit VCDialogView(NewProjectWizard *parent);
    ~VCDialogView();

    //! Set the model in the view
    void setModel (VCViewData *vc,
                   TestBenchViewData *parent,
                   VHDLFileContent *dut,
                   const QString &dutEntityName);

public slots:

    //! Save the GUI data in the model
    void save ();

    /** Save and check that the fields are corrects
      Emit validate() if the return value is true.
      */
    bool validate ();

signals:

    //! Is called when save() is called and returned true
    void validated ();

private slots:

    // Check if the agent name is not already taken
    bool checkName ();

    // Check if the sequencer-to-sequencer checkbox should be enabled or not
    bool checkSequencer ();

    void checkAgentSequenceLinks();

    // Check if the scoreboard comparator should be enabled or not
    void checkScoreboard();

    /* Create a new agent
     The new agent is created and its configuration dialog is shown.
     */
    void newAgent ();

    // Show an agent in a dialog
    void showAgent(int place);

    // Update the all/none checkboxes
    void checked ();

    // Check all the items in ports
    void checkAll (bool val);

    // Un-check all the items in ports
    void unCheckAll (bool val);

    void removeName(QString name);

private:
    //Create and run a dialog containing the agent and return the dialog code
    int createDialog(AgentViewData *agent, const QString &title);

    //GUI
    Ui::VCDialogView *ui;
    //Model
    VCViewData *_vc;
    //The model's parent
    TestBenchViewData *_parent;
    //Our wizard
    NewProjectWizard *_project;
    //The DUT main file
    VHDLFileContent *_dut;
    //The DUT entity name
    QString _dutEntityName;
    //The ports list
    CheckableStringListModel *_ports;

    QButtonGroup *_comparatorTypeGroup;
};


//Simple string list model object for which each data can be checked
class CheckableStringListModel : public QStringListModel
{
    Q_OBJECT
public:
    //Return some flag telling the data can be checked
    Qt::ItemFlags flags (const QModelIndex &index) const;

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

signals:
    //Emitted when a item has been checked/unchecked
    void checked (bool value, int row);

private:
    QMap<int,Qt::CheckState> _checked;
};

#endif // VCDIALOGVIEW_H
