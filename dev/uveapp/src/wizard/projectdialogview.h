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
#ifndef PROJECTDIALOGVIEW_H
#define PROJECTDIALOGVIEW_H

#include <QWizardPage>
#include <QList>

#include "uvmproject.h"
#include "viewdata.h"
#include "uvmproject.h"
#include "connect.h"

namespace Ui {
class ProjectDialogView;
}

/** Displays the global project's data
  In this widget are shown the project-related data, such as :
  \li The project name
  \li The project folder
  \li The associated simulator
  \li The DUT file

  These fields are dynamic, as they fill themselves from data either from the settings, or from other fields.

  From the MVC standpoint, this object is a controller, and its UI object is the view.
  */
class ProjectDialogView : public QWizardPage
{
    Q_OBJECT

public:
    //! Creates the GUI
    explicit ProjectDialogView(QWidget *parent = 0);
    ~ProjectDialogView();

    /** Set a model in the view
      Setting a model here will fill the fields from data from the project.
      If some fields are blank, they will be filled with default values.
      */
    void setModel (ProjectViewData *model);

    //! Return false until all the fields have been filled
    bool isEmpty();
    //! Return false until all mandatory fields have been filled
    bool isComplete();

    //! Return the same result as isComplete(), but save the GUI data in the model
    bool validatePage();

    //! Return false if the template field is empty or invalid.
    bool hasTemplates();

private slots:

    // Check if all the mandatory fields are filled, called when an input field changes
    void filled ();

    // Fill the folder field if the name or DUT file changed, but not if it has been modified manually
    void autoFolder ();

    // A file should be added to the list, show the dialog
    void addFile ();

    // A file has been removed - remove its entity from the list
    void fileRemoved (void* name);

private:

    void addFiles(const QStringList &files);

    //GUI
    Ui::ProjectDialogView *ui;
    //Model
    ProjectViewData *_model;
    //Automatic folder field value (Used to know when it manually changed)
    QString _autoFolder;
    //List of file for each combo-box entries
    QList<VHDLFileContent *> _files;

    friend class NewProjectWizardTester;
};

#endif // PROJECTDIALOGVIEW_H
