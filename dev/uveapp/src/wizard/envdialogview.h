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
#ifndef ENVDIALOGVIEW_H
#define ENVDIALOGVIEW_H

#include <QWizardPage>
#include <QDialog>

#include "uvmtestbench.h"
#include "uvmtestcase.h"
#include "uvmverificationcomponent.h"
#include "vcdialogview.h"
#include "viewdata.h"

class NewProjectWizard;

namespace Ui {
class EnvDialogView;
}

class EnvDialogView : public QWizardPage
{
   Q_OBJECT
   
public:
   explicit EnvDialogView(NewProjectWizard *parent);
   ~EnvDialogView();

    //! Save the data in the models and check the fields
    bool validatePage();

    //! Set the model and show their value
    void setModels (TestBenchViewData *testbench,
                    TestCaseViewData *testcase,
                    VHDLFileContent *dut,
                    const QString &dutEntityName);

public slots:

   /** Create a VC and show its dialog
     This slot is called when the "add" button is clicked.
     */
   void newVC ();

   //! Show the corresponding VC in the list
   void showVC (int place);

   //! Load a VC from the library
   void loadVC ();

   //! Save the data in the models
   void save ();

   void checkVCSequencerLink();

private slots:

   //Check if the test case name is unique and not empty
   //Return the error message if any
   QString checkTestCaseName();

   //Check if the testbench name is unique and not empty
   //Return the error message if any
   QString checkTestBenchName();

   void removeName(QString name);
   
private:

   //Create and display a QDialog from an VC and a VCDialogView
   //Return the dialog's exec() code
   int createDialog (VCViewData *vc, const QString &title);

   //GUI
   Ui::EnvDialogView *ui;
   //Models
   TestBenchViewData *_testbench;
   TestCaseViewData *_testcase;
   //Our wizard
   NewProjectWizard *_project;
   //The project's DUT
   VHDLFileContent *_dut;
   // The project DUT entity name
   QString _dutEntityName;
};

#endif // ENVDIALOGVIEW_H
