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
#ifndef AGENTDIALOGVIEW_H
#define AGENTDIALOGVIEW_H

#include <QWidget>

#include "uvmagent.h"
#include "uvmverificationcomponent.h"
#include "viewdata.h"
#include "uvmproject.h"
#include "newprojectwizard.h"
#include "connect.h"

namespace Ui {
class AgentDialogView;
}

/** Display an agent's data
  The agent's name is derived from its parent VC name.
  For the module, some module options are only available if their corresponding module are activated.
  */
class AgentDialogView : public QWidget
{
    Q_OBJECT

public:
    //! Setup the GUI
    explicit AgentDialogView(NewProjectWizard *parent);
    ~AgentDialogView();

    //! Set the model and update the GUI fields
    void setModel (AgentViewData *agent, VCViewData *parent);

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
    // Update the monitor-to-sequencer checkbox avaibility
    void checkMonToSeq ();

    // Update the virtual sequencer-to-sequencer checkbox avaibility
    void checkVSToSeq ();

    void checkSequencer();

    void checkCollector();

    void checkDriverToMonitor ();

    // Update the monitor coverage checkbox avaibility
    void checkMonCov ();

    // Check if the agent name is not already taken
    bool checkName ();

private:
    //GUI
    Ui::AgentDialogView *ui;
    //Model
    AgentViewData *_agent;
    //The model's parent
    VCViewData *_parent;
    //Our project
    NewProjectWizard *_project;
};

#endif // AGENTDIALOGVIEW_H
