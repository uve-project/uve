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
#include "projectdialogview.h"
#include "ui_projectdialogview.h"

#include <QFileDialog>
#include <QStringList>
#include <QFileInfo>
#include <QRegExp>
#include <QMessageBox>
#include <QValidator>
#include <uvesettings.h>

#include "vhdlparser.h"
#include "uveconstants.h"

//Setup the GUI
ProjectDialogView::ProjectDialogView(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ProjectDialogView)
{
    //UI setup
    ui->setupUi(this);

    //Project folder
    ui->folder->setMode(QFileDialog::Directory);
    ui->folder->setAcceptMode(QFileDialog::AcceptOpen);
    ui->folder->setDialogTitle(tr("Project folder"));
    ui->folder->setFileName(UVESettings::getInstance()->value(CONF_PROJECTFOLDER,"").toString());

    ui->templates->setMode(QFileDialog::Directory);
    ui->templates->setFileName(UVESettings::getInstance()->value(CONF_TEMPLATEFOLDER,UVESettings::generator_gve_folder()).toString());
    ui->templates->setDialogTitle(tr("Template path"));

    //The simulator
    ui->simulator->setMode(QFileDialog::Directory);
    ui->simulator->setDialogTitle(tr("Simulator directory"));
    ui->simulator->setFileName(UVESettings::getInstance()->value(CONF_SIMULATOR,"vsim").toString());

    //The File list
    ui->fileList->hideUpDown(false);
    ui->fileList->setTitle(tr("VHDL files"));

    //The name only contains character, numbers and underscore
    ui->name->setValidator(new QRegExpValidator(QRegExp("^[A-Za-z][0-9a-zA-Z_]*"), this));

    //Connect each field to the filled slot, for the completed status to change
    CONNECT(ui->folder, SIGNAL(fileNameChanged(QString)), this, SLOT(filled()));
    CONNECT(ui->simulator, SIGNAL(fileNameChanged(QString)), this, SLOT(filled()));
    CONNECT(ui->name, SIGNAL(textChanged(QString)), this, SLOT(filled()));

    //When the list or name changes, update the folder field
    //CONNECT(ui->name, SIGNAL(textChanged(QString)), this, SLOT(autoFolder()));
    //CONNECT(ui->fileList, SIGNAL(appened(void*)), this, SLOT(autoFolder()));
    CONNECT(ui->fileList, SIGNAL(loadClicked()), this, SLOT(addFile()));
    CONNECT(ui->fileList, SIGNAL(removed(void*)), this, SLOT(fileRemoved(void*)));
}

ProjectDialogView::~ProjectDialogView()
{
    delete ui;
}

//Save the model, and setup the fields
void ProjectDialogView::setModel(ProjectViewData *model)
{
    _model = model;

    //Parse the given files to fill the main class combo-box and the file list
    VHDLParser parser;
    foreach (QString file, _model->files) {
        if (parser.read(file) != 1) {
            VHDLFileContent *content=parser.getVHDLFileContent();

            foreach (QString entityName,content->getEntitiesName())
                ui->classes->addItem(entityName);
            _files.append(content);
        }
        //File list
        ui->fileList->append(file,0);
    }

    //Simulator
    ui->simulator->setFileName(_model->simulator);

    //Name : if the name is empty but the entities are not empty, use the first entity name plus "-tester"
    if (_model->dutEntityName.isEmpty() && !_files.isEmpty()) {
        ui->name->setText(ui->classes->currentText() + "-tester");

    //Simply use the given name
    } else
        ui->name->setText(_model->name);

    //Folder
    //ui->folder->setFileName(_model->folder);

    //Reset the automatic name, and calculate it
    _autoFolder = "";
    autoFolder();
}

//Check the all fields
bool ProjectDialogView::isEmpty()
{
    if (ui->folder->getFileName().isEmpty())
        return false;
    if (ui->simulator->getFileName().isEmpty())
        return false;
    if (ui->name->text().isEmpty())
        return false;
    if (!(ui->fileList->size() > 0))
        return false;
    return !ui->folder->getFileName().isEmpty() &&
           !ui->simulator->getFileName().isEmpty() &&
           !ui->name->text().isEmpty() &&
            ui->fileList->size() > 0;
}

//Check the mandatory fields
bool ProjectDialogView::isComplete()
{
    return !ui->folder->getFileName().isEmpty() &&
           !ui->name->text().isEmpty()&&
            ui->fileList->size() > 0&&
            ui->classes->currentText()!="";
}

bool ProjectDialogView::hasTemplates()
{
    if(ui->templates->getFileName().isEmpty())
        return false;
    return QFile(ui->templates->getFileName()).exists();
}

//Save the field in the model and in the settings, and return isComplete
bool ProjectDialogView::validatePage()
{
    //If template is empty or invalid, pop-up an error
    if (!hasTemplates()) {
        QMessageBox::critical(this,tr("Templates missing or invalid"),
                              tr("Templates path field must be filled with valid data."));
        return false;
    }
    //If mandatory empty, pop-up an error
    if (!isComplete()) {
        QMessageBox::critical(this,tr("Invalid field"),
                              tr("All mandatory fields must be filled with valid data and the file list should contain at least one item."));
        return false;
    }
    //If some empty, pop-up a warning
    else if(!isEmpty()) {
        if(QMessageBox::warning(this,tr("Empty field"),
                                    tr("All fields should be filled with valid data.\n") +
                                    tr("Do you want to continue anyway?"),
                                       QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
            return false;
        }
    }

    //Save in the model
    _model->dutEntityName = ui->classes->currentText();
    _model->folder = ui->folder->getFileName();
    _model->name = ui->name->text();
    _model->simulator = ui->simulator->getFileName();
    _model->files = ui->fileList->names();
    _model->dut = 0;
    foreach(VHDLFileContent *content,_files)
    {
        if (content->getEntity(ui->classes->currentText()))
            _model->dut=content;
    }
    _model->templatePath = ui->templates->getFileName();
    if (_model->templatePath.at(_model->templatePath.length()-1)!=QDir::separator())
        _model->templatePath += QDir::separator();

    // Update the settings
    UVESettings::getInstance()->setValue(CONF_PROJECTFOLDER,ui->folder->getFileName());
    UVESettings::getInstance()->setValue(CONF_SIMULATOR,ui->simulator->getFileName());
    UVESettings::getInstance()->setValue(CONF_TEMPLATEFOLDER,ui->templates->getFileName());

    return true;
}

//Uses isComplete()
void ProjectDialogView::filled()
{
    if (isComplete())
        emit completeChanged();
}

//Automatically fill the folder field
void ProjectDialogView::autoFolder()
{
    //Quit if the field has been manually changed
    if (_autoFolder != ui->folder->getFileName())
        return;

    //Calculate the new value (use the list last file, probably the DUV, or the home directory if none)
//    if (ui->fileList->size() > 0)
//        _autoFolder = QFileInfo(ui->fileList->names().last()).path() + "/" + ui->name->text();
//    else
        _autoFolder = QDir::homePath() + "/" + ui->name->text();
    ui->folder->setFileName(_autoFolder);
}

//Open the file selection dialog, and append all the given files
void ProjectDialogView::addFile()
{
    //If there is a file in the list, use its directory as base, or use the home folder
    QString dir = UVESettings::getInstance()->value(CONF_VHDLFOLDER).toString();

    //Open the dialog
    QStringList files = QFileDialog::getOpenFileNames(this,
                                                      tr("Open DUT files"),
                                                      dir,
                                                      tr("VHDL files (*.vhdl *.vhd)"));

    // If no file has been selecet (cancel)
    if (files.empty()) {
        return;
    }

    addFiles(files);
    UVESettings::getInstance()->setValue(CONF_VHDLFOLDER, QFileInfo(files.last()).path());
}

void ProjectDialogView::addFiles(const QStringList &files)
{
    //Add each file in the list, parse it and add its class name
    VHDLParser parser;
    foreach (QString file, files) {
        //Parse
        if (parser.read(file) == -1)
            continue;
        VHDLFileContent *content=parser.getVHDLFileContent();
        //To the file list
        ui->fileList->append(file,content);
        //To the class list
        foreach (QString entityName,content->getEntitiesName())
            ui->classes->addItem(entityName);
        _files.append(content);
    }
}

//Remove this value from the class list
void ProjectDialogView::fileRemoved(void *name)
{
    QString *entity = (QString*)name;
    int i = ui->classes->findText(*entity);
    if (i != -1) { //Check that the name exists (not all file have an entity name)
        ui->classes->removeItem(i);
        _files.removeAt(i);
    }
}
