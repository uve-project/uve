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
#ifndef NEWPROJECTWIZARD_H
#define NEWPROJECTWIZARD_H

#include <QWizard>

#include "projectdialogview.h"
#include "envdialogview.h"
#include "signalsdialogview.h"
#include "uvmproject.h"
#include "uvmdesign.h"
#include "uvmtestbench.h"
#include "uvmtestcase.h"
#include "uvmpackage.h"
#include "uvmtop.h"
#include "connect.h"

class NewProjectWizardTester;
class TestGenerator;

/** New project wizard
  Each page represents a view with a title and some informations.
  The each view is filled with one or more relevant model object.
  */
class NewProjectWizard : public QWizard
{
   Q_OBJECT
public:
   /** Pages creation
     Each used view is inserted in a QWizardPage, and a title/subtitle is set.
     */
   explicit NewProjectWizard(const QString defaultSimulator = "", const QString &defaultFile = "", QWidget *parent = 0);

    ~NewProjectWizard();

    /** Fetch the created project
      At the end, if everything has worked, the project tree is created and can be fetched here.
      If something went wrong, or the user quit before the end, this function returns 0.
      */
    UvmProject* getProject () const;

    /** Check if a name is already used
      If the name has already been registered, return false
      */
    bool isNameUsedAtProjectScope (const QString &name) const;

    /** Register a name
      The name is saved, but only for the isNameUsed function to work.
      */
    void registerNameAtProjectScope (const QString &name);

    /** Un-register a name
      The name is removed from the name list
      */
    void unRegisterNameAtProjectScope (const QString &name);

signals:

public slots:

   // Each of the following slot is called before a page is showed

   void setupProject ();

   void setupEnv ();

   void setupSignals ();

protected:

   //Redirect the calls to the above functions
   void	initializePage ( int id );

private slots:

   //Called when the widget ended, create the project tree
   void createProject ();

private:

   //Enumerate of the possible pages
   enum DialogPage {
      ProjectPage,
      SignalsPage,
      EnvPage
   };

   //The widget's internal views
   ProjectDialogView *_project;
   EnvDialogView *_env;
   SignalsDialogView *_signals;

   //Default data
   QString _defaultSimulator, _defaultFile;

   //View models
   ProjectViewData *d_project;
   TestCaseViewData *d_testcase;
   TestBenchViewData *d_testbench;
   SignalsViewData *d_signals;

   //Models
   UvmTestCase *m_testcase;
   UvmProject *m_project, *_result;
   UvmTestbench *m_testbench;
   UvmTop *m_top;
   UvmDesign *m_design;

   //Name list
   QStringList _names;

   friend class NewProjectWizardTester;
   friend class TestGenerator;
};


#endif // NEWPROJECTWIZARD_H
