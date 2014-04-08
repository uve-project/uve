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
#include "mainwindow.h"

#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QDockWidget>
#include <QStatusBar>
#include <QAction>
#include <QFileDialog>
#include <QMenu>
#include <QToolBar>
#include <QMenuBar>
#include <QFontDialog>

#include "newprojectwizard.h"
#include "renameprojectdialog.h"
#include "svexport.h"
#include "uvexmlexport.h"
#include "current_version.h"
#include "uveconstants.h"
#include "uvesettings.h"
#include "uvexmlimport.h"
#include "uvexmlvalidator.h"

MainWindow::MainWindow() :
    currentProject(0),consoleSim(0),simProcess(0)
{
    mdiArea = new QMdiArea;
    mdiArea->setViewMode(QMdiArea::TabbedView);
    mdiArea->showFullScreen();
    mdiArea->setTabsClosable(true);
    mdiArea->setTabsMovable(true);
    setCentralWidget(mdiArea);
    CONNECT(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)), this, SLOT(updateMenus()));
    CONNECT(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)), this, SLOT(changeActiveComponent()));

    windowMapper = new QSignalMapper(this);
    CONNECT(windowMapper, SIGNAL(mapped(QWidget *)),this, SLOT(setActiveSubWindow(QWidget *)));

    createDocks();
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    updateMenus();
    updateRecentProjectActions();
    readSettings();

    setUnifiedTitleAndToolBarOnMac(true);

    UVESettings::getInstance()->setValue("UVEXSD", QApplication::applicationDirPath()+"/../../../xml/uve.xsd");
    UVESettings::getInstance()->setValue("VIPXSD", QApplication::applicationDirPath()+"/../../../xml/uvc.xsd");

    aFileWasAdded = false;
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    fileDockWidget->close();
    graphicFileDockWidget->close();
    helpDockWidget->close();
    searchDockWidget->close();
    mdiArea->closeAllSubWindows();
    if (activeMdiChild()) {
        event->ignore();
    } else {
        writeSettings();
        event->accept();
    }
}


void MainWindow::newFile()
{
    MdiChild *child = createMdiChild();
    child->newFile();
    child->show();
}

void MainWindow::newProject()
{
    QMessageBox::warning(this,"TODO","Soon to be implemented");
}

void MainWindow::openFile(const QString &file)
{
    //Save the old file
    QString oldFile = currentFile;

    //If no file is given, open the file dialog
    if (file.isEmpty())
        currentFile = QFileDialog::getOpenFileName(this,QString(),QString(),tr("All supported files (*.vhd *.vhdl *.sv)"));
    else
        currentFile = file;

    //If the file is still empty, do nothing
    if (currentFile.isEmpty())
        return;

    //If the file do not exists, print a dialog and exit
    if (!QFileInfo(currentFile).exists()) {
        QMessageBox::warning(this,tr("File not found"),tr("The file : '%1' cannot be found.").arg(currentFile));
        currentFile = oldFile;
        return;
    }

    setCurrentFile(currentFile);

    //Try to open an existing tab
    QMdiSubWindow *existing = findMdiChild(currentFile);

    if (existing) {
        mdiArea->setActiveSubWindow(existing);
        //fileView->setRootIndex(fileModel->index(QFileInfo(currentFile).absolutePath())); TODO
//        fileDockWidget->show();
    } else { //Create a new tab
        MdiChild *child = createMdiChild();
        if (child->loadFile(currentFile)) {
            statusBar()->showMessage(tr("File loaded"), 2000);
            //fileView->setRootIndex(fileModel->index(QFileInfo(currentFile).absolutePath())); TODO
//            fileDockWidget->show();
            child->show();
        } else {
            currentFile = oldFile;
            child->close();
            delete child;
        }
    }
}

////Show the view and all
//void MainWindow::openProject(UvmProject *project)
//{
//    closeProject();

//    projectView->setProject(project);
//    graphicFileDockWidget->show();
//    fileTreeWidget->populateTree();
//}

void MainWindow::search(QString term, bool caseSensitive, bool regex)
{
    QList<QList<QString> > occurences;
    Qt::CaseSensitivity cs;
    if(caseSensitive)
        cs = Qt::CaseSensitive;
    else
        cs = Qt::CaseInsensitive;

    if(currentProject)
    {
        QStringList files = currentProject->getTop()->getAllFilePaths();
        for(int i=0; i<files.length(); i++)
        {
            QString fileName = files.at(i).right(files.at(i).length() - files.at(i).lastIndexOf("/") -1);
            QFile file(files.at(i));
            if (!file.open(QFile::ReadOnly | QFile::Text)) {
                qDebug() << "Cannot read file " << files.at(i) << " : " << file.errorString() << ". ";
            }

            QTextStream in(&file);

            QString line = in.readLine();

            int lineNumber = 1;
            while (!line.isNull()) {
                if(regex)
                {
                    int j = 0;

                    while ((j = line.indexOf(QRegExp(term), j)) != -1) {
                        QString result;
                        QString tmp = line.mid(j);
                        while(tmp.contains(QRegExp(term)))
                        {
                            result = tmp;
                            tmp.left(tmp.length()-1);
                        }
                        QList<QString> occurence;
                        occurence.append(fileName);
                        occurence.append(QString::number(lineNumber));
                        occurence.append(line.trimmed());
                        occurence.append(files.at(i));
                        occurence.append(QString::number(j));
                        occurence.append(result);
                        occurences.append(occurence);
                        j++;
                    }
                }
                else
                {
                    int j = 0;

                    j = line.indexOf(term, j,cs);

                    while (j > 0) {
                        QList<QString> occurence;
                        occurence.append(fileName);
                        occurence.append(QString::number(lineNumber));
                        occurence.append(line.trimmed());
                        occurence.append(files.at(i));
                        occurence.append(QString::number(j));
                        occurence.append(term);
                        occurences.append(occurence);
                        j++;
                        j = line.indexOf(term, j, cs);
                    }
                }
                line = in.readLine();
                lineNumber++;
            }
        }
    }
    updateMessages(occurences);
}

void MainWindow::replace(QString term, QString newTerm, bool caseSensitive, QString inFile, int atLine, int atIndex, bool activeFileOnly)
{
    if(!activeFileOnly)
    {
        goToLine(atLine, inFile);
        activeMdiChild()->replace(term, newTerm, atLine, atIndex, caseSensitive);
    }
    else if(inFile.compare(activeMdiChild()->currentFile()) == 0)
    {
        activeMdiChild()->replace(term, newTerm, atLine, atIndex, caseSensitive);
    }
}

void MainWindow::save()
{
    if (activeMdiChild() && activeMdiChild()->save())
        statusBar()->showMessage(tr("File saved"), 2000);
}


void MainWindow::saveAs()
{
    if (activeMdiChild() && activeMdiChild()->saveAs())
        statusBar()->showMessage(tr("File saved"), 2000);
}


void MainWindow::cut()
{
    if (activeMdiChild())
        activeMdiChild()->cut();
}


void MainWindow::copy()
{
    if (activeMdiChild())
        activeMdiChild()->copy();
}


void MainWindow::paste()
{
    if (activeMdiChild())
        activeMdiChild()->paste();
}

void MainWindow::about()
{
    // show application About dialog
    QMessageBox *about;
    about = new QMessageBox( this );
    about->setIconPixmap( QPixmap( ":/images/logo.png" ));
    about->setTextFormat( Qt::RichText );
    about->setText("<b>UVE " UVE_VERSION "</b>");
    about->setInformativeText(
                "Webpage:<br />"
                "&nbsp;&nbsp;" "<a href='" UVE_WEBPAGE "'>" UVE_WEBPAGE "</a>" "<br />"
                "<br />"
                "Please report Bugs or Feature Requests to:<br />"
                "&nbsp;&nbsp;" "<a href='" UVE_WEBPAGE_TICKETS "'>Ticketing System</a>" "<br />"
                "<br />"
                "<br />"
                UVE_COPYRIGHT_LONG_RICH );
    about->setDetailedText(
                "Project Members:" "\n"
                "    " "Christophe Bianchi    <christophe.bianchi@hevs.ch>" "\n"
                "    " "Jérémie Ecoffey    <jeremie.ecoffey@heig-vd.ch>" "\n"
                "    " "Oliver Gubler    <oliver.gubler@hevs.ch>" "\n"
                "    " "Daniel Molla    <daniel.molla@heig-vd.ch>" "\n"
                "    " "Bastien Noverraz    <>" "\n"
                "    " "Laurent Perrier    <>" "\n"
                "    " "Lionel Sambuc    <>" "\n"
                "    " "Jerôme Stadelmann    <jerome.stadelmann@heig-vd.ch>" "\n"
                "    " "Yann Thoma    <yann.thoma@heig-vd.ch>" "\n"
                );
    about->exec();
}

/** updateApplication() ***** [public slot] *************************************
*                                                                              *
* Update the application                                                       *
*  TODO: open web page with current releases                                   *
*  or                                                                          *
*  TODO: get latest release from server                                        *
*  or                                                                          *
*  TODO: open widget to allow installation from local file or the net          *
*  and                                                                         *
*  TODO: add option to check for updates at regular intervals                  *
*******************************************************************************/
void MainWindow::update()
{
  //a window showing the current version number and how to check for updates
  QMessageBox *update;
  update = new QMessageBox( this );
  update->setIconPixmap( QPixmap(":/images/logo.png") );
  update->setTextFormat( Qt::RichText );
  update->setText( "<b>Update</b>" );
  //use &nbsp; to avoid text wrapping
  //TODO: use escape() or some function to automatically transform text to html
  QString updateText;
  updateText += "Your&nbsp;installations&nbsp;at ";
  updateText += QCoreApplication::applicationDirPath();
  updateText += "<br />";
  updateText += "&nbsp;is&nbsp;at&nbsp;version&nbsp;<b>" UVE_VERSION "</b>.<br /><br />";
  updateText += "Check&nbsp;for&nbsp;newer&nbsp;versions&nbsp;at&nbsp;<a href='" UVE_WEBPAGE_DOWNLOAD "'>our&nbsp;webpage</a><br />";
  updateText += "or&nbsp;subscribe&nbsp;for&nbsp;release&nbsp;notifications&nbsp;to&nbsp;<a href='" UVE_WEBPAGE_FEED "'>our&nbsp;feed</a>";
  update->setInformativeText(updateText);
  update->exec();
}

/** toggle help panel visibility
  */
void MainWindow::help(){
    if ( helpDockWidget->isHidden() ) {
        helpDockWidget->show();
    }
    else {
        helpDockWidget->hide();
    }
}

void MainWindow::updateMenus()
{
    bool const hasMdiChild = (activeMdiChild() != 0);
    bool const hasProject = (currentProject != 0);
    bool const noSimulationRunning = (simProcess->state()==QProcess::NotRunning);
    saveAct->setEnabled(hasMdiChild);
    saveAsAct->setEnabled(hasMdiChild);
    copyAct->setEnabled(hasMdiChild);
    pasteAct->setEnabled(hasMdiChild);
    cutAct->setEnabled(hasMdiChild);
    closeAct->setEnabled(hasMdiChild);
    closeAllAct->setEnabled(hasMdiChild);
    /*tileAct->setEnabled(hasMdiChild);
    cascadeAct->setEnabled(hasMdiChild);*/
    nextAct->setEnabled(hasMdiChild);
    previousAct->setEnabled(hasMdiChild);
    separatorAct->setVisible(hasMdiChild);
    newProjectAct->setEnabled(noSimulationRunning);
    openProjectAct->setEnabled(noSimulationRunning);
    closeProjectAct->setEnabled(hasProject &&
                                noSimulationRunning);
    exportAsPNGAct->setEnabled(hasProject &&
                               noSimulationRunning);
    regenerateAct->setEnabled(hasProject &&
                              noSimulationRunning);
    simulateInUveAct->setEnabled(hasProject &&
                            noSimulationRunning &&
                            currentProject->getSimulator()!="");
    simulateExtAct->setEnabled(hasProject &&
                               noSimulationRunning &&
                               currentProject->getSimulator()!="");
    projectMenu->addSeparator();
    if (hasProject) {
        if (noSimulationRunning) {
            setWindowTitle( tr("UVE")
                            + " - "
                            + currentProject->getName()
                            + " @ "
                            + currentProject->getFolder() );
        } else {
            setWindowTitle( tr("UVE")
                            + " - "
                            + currentProject->getName()
                            + " @ "
                            + currentProject->getFolder()
                            + " - "
                            + "SIMULATING...");
        }

    } else {
        setWindowTitle(tr("UVE"));
    }
}


void MainWindow::updateWindowMenu()
{
    windowMenu->clear();
    windowMenu->addAction(closeAct);
    windowMenu->addAction(closeAllAct);
    windowMenu->addSeparator();
    /*windowMenu->addAction(tileAct);
    windowMenu->addAction(cascadeAct);
    windowMenu->addSeparator();*/
    windowMenu->addAction(nextAct);
    windowMenu->addAction(previousAct);
    windowMenu->addAction(separatorAct);

    QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
    separatorAct->setVisible(!windows.isEmpty());

    for (int i = 0; i < windows.size(); ++i) {
        MdiChild *child = qobject_cast<MdiChild *>(windows.at(i)->widget());

        QString text;
        if (i < 9) {
            text = tr("&%1 %2").arg(i + 1).arg(child->userFriendlyCurrentFile());
        } else {
            text = tr("%1 %2").arg(i + 1).arg(child->userFriendlyCurrentFile());
        }
        QAction *action  = windowMenu->addAction(text);
        action->setCheckable(true);
        action ->setChecked(child == activeMdiChild());
        CONNECT(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
        windowMapper->setMapping(action, windows.at(i));
    }
}

void MainWindow::updateViewMenu()
{
    viewMenu->clear();
    viewMenu->addAction(fileDockWidget->toggleViewAction());
    viewMenu->addAction(graphicFileDockWidget->toggleViewAction());
    viewMenu->addAction(searchDockWidget->toggleViewAction());
    viewMenu->addAction(consoleDockWidget->toggleViewAction());
    viewMenu->addSeparator();
    QMenu* toolBarViewSubMenu = new QMenu("&Toolbars");
    viewMenu->addMenu(toolBarViewSubMenu);
    toolBarViewSubMenu->addAction(fileToolBar->toggleViewAction());
    toolBarViewSubMenu->addAction(editToolBar->toggleViewAction());
    toolBarViewSubMenu->addAction(projectToolBar->toggleViewAction());
    toolBarViewSubMenu->addAction(viewToolBar->toggleViewAction());

    /*QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
    separatorAct->setVisible(!windows.isEmpty());

    for (int i = 0; i < windows.size(); ++i) {
        MdiChild *child = qobject_cast<MdiChild *>(windows.at(i)->widget());

        QString text;
        if (i < 9) {
            text = tr("&%1 %2").arg(i + 1).arg(child->userFriendlyCurrentFile());
        } else {
            text = tr("%1 %2").arg(i + 1).arg(child->userFriendlyCurrentFile());
        }
        QAction *action  = viewMenu->addAction(text);
        action->setCheckable(true);
        action ->setChecked(child == activeMdiChild());
        CONNECT(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
        windowMapper->setMapping(action, windows.at(i));
    }*/
}


void MainWindow::onActivated(QModelIndex /*index*/)
{
    QString fileName = ""; //fileModel->filePath(index); TODO

    if (!fileName.isEmpty()) {
        QMdiSubWindow *existing = findMdiChild(fileName);
        if (existing) {
            mdiArea->setActiveSubWindow(existing);
            return;
        }

        MdiChild *child = createMdiChild();
        if (child->loadFile(fileName)) {
            statusBar()->showMessage(tr("File loaded"), 2000);
            child->show();
        } else {
            child->close();
        }
    }
}


MdiChild *MainWindow::createMdiChild()
{
    MdiChild *child = new MdiChild();
    QMdiSubWindow* tmp = mdiArea->addSubWindow(child);
    QMenu* menu = tmp->systemMenu();
    /*while(menu->actions().count() > 0)
    {
        menu->removeAction(menu->actions().at(0));
    }*/
    QAction* closeTabAct = new QAction(tr("Close tab"), menu);
    closeTabAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_W));
    CONNECT(closeTabAct, SIGNAL(triggered()), tmp, SLOT(close()));
    menu->addAction(closeTabAct);
    QAction* closeAllTabsAct = new QAction(tr("Close all tabs"), menu);
    CONNECT(closeAllTabsAct, SIGNAL(triggered()), mdiArea, SLOT(closeAllSubWindows()));
    menu->addAction(closeAllTabsAct);

    CONNECT(child, SIGNAL(copyAvailable(bool)), cutAct, SLOT(setEnabled(bool)));
    CONNECT(child, SIGNAL(copyAvailable(bool)), copyAct, SLOT(setEnabled(bool)));
    CONNECT(this, SIGNAL(closeAllWithoutSaving()), child, SLOT(closeWithoutSaving()));
    CONNECT(this, SIGNAL(closeAllAndSave()), child, SLOT(closeAndSave()));
    return child;
}

void MainWindow::openProject(const QString &file)
{
    closeProject();

    //Load the XML
    QString projectFile = file;

    UvmProject *oldProject = currentProject;

    UveXmlImport xmlImport;
    currentProject = xmlImport.loadProject(projectFile);

    //If there was any errors, show them
    if (currentProject == 0) {
        QStringList faultyFiles = xmlImport.getFaultyFiles();
        if(faultyFiles.isEmpty())
        {
            QMessageBox::critical(this,tr("Errors on project load"),tr("There were errors while loading the project :\n%1").arg(xmlImport.getErrorString()));
            return;
        }
        foreach(QString faultyFile, faultyFiles)
        {
            UveXmlValidator v;
            v.setXML(faultyFile);
            v.exec();
        }
        return;
    }

    //Setup the view
    projectView->setProject(currentProject);

    // This has to be done after setProject, in order to let the view correctly close
    if (oldProject) //Remove the old one, if any
        delete oldProject;

    //Show the panes
    fileTreeWidget->populateTree();
    fileDockWidget->show();
    graphicFileDockWidget->show();

    setCurrentProject(file);

    updateMenus();

    aFileWasAdded = false;
}

// Open the uve.xml file and load the project
void MainWindow::openProject()
{
    //Uve.xml file
    QFileDialog dialog(this,tr("Open the project file"),
                       UVESettings::getInstance()->value(CONF_LASTPROJECTFOLDER,
                                                         QDir::currentPath()).toString(),
                       "Uve project (*.uve)");
    dialog.setFileMode(QFileDialog::ExistingFile);

    if (!dialog.exec())
        return;

    QFileInfo fileInfo(dialog.selectedFiles().at(0));
    UVESettings::getInstance()->setValue(CONF_LASTPROJECTFOLDER,fileInfo.absolutePath());
    //Load the XML
    openProject(dialog.selectedFiles().at(0));

    aFileWasAdded = false;
}

void MainWindow::closeProject()
{
    //ask to save?
    bool somethingHasChanged = false;

    for(int i=0; i<mdiArea->subWindowList().length(); i++)
    {
        QMdiSubWindow* sw = mdiArea->subWindowList().at(i);
        MdiChild* kid = dynamic_cast<MdiChild*>(sw->widget());
        somethingHasChanged = somethingHasChanged || kid->isModified();
    }

    //check if modified.TODO
    if(somethingHasChanged)
    {
        QMessageBox saveProMB;
        saveProMB.setText("Do you want to save the current project before closing?");
        saveProMB.setStandardButtons(QMessageBox::Abort | QMessageBox::Discard | QMessageBox::Save);
        saveProMB.setDefaultButton(QMessageBox::Save);
        int ret = saveProMB.exec();
        if(ret == QMessageBox::Abort)
        {
            return;
        }
        else if(ret == QMessageBox::Discard)
        {
            closeAllWithoutSaving();
        }
        else
        {
            closeAllAndSave();
        }
    }

    if(aFileWasAdded)
    {
        QMessageBox saveProMB;
        saveProMB.setText("Files were added to some components. Would you like to export it again?");
        saveProMB.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        saveProMB.setDefaultButton(QMessageBox::Yes);
        int ret = saveProMB.exec();
        if(ret == QMessageBox::Yes)
        {
            UveXmlExport exporter;
            exporter.saveProject(currentProject);
        }
    }

    mdiArea->closeAllSubWindows();

    //clear the docks
    fileTreeWidget->clear();
    fileDockWidget->hide();
    if(projectView!=0)
    {
        delete projectView;
        projectView = new GraphicalProjectView(this);
        graphicFileDockWidget->setWidget(projectView);
    }
    graphicFileDockWidget->hide();
    consoleSim->clear();
    consoleDockWidget->hide();
    searchDockWidget->hide();

    //delete current Project.
    if(currentProject!=0)
    {
        delete currentProject;
        currentProject =0;
    }

    updateMenus();

    aFileWasAdded = false;
}

// Open the new project wizard
void MainWindow::wizard()
{
    //Fetch the simulator in the settings
    QString simulator = UVESettings::getInstance()->value(CONF_SIMULATOR,QVariant("")).toString();

    QString mainFile = "";
    //Open the new project wizard
    if(activeMdiChild() && ((currentFile.contains(".vhd"))||(currentFile.contains(".vhdl"))))
    {
        QMessageBox *whichMain = new QMessageBox(QMessageBox::Question, QString("Use current file?"), QString("Do you want to use the current file as main VHDL?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Abort);
        int ret = whichMain->exec();
        if(ret == QMessageBox::Abort)
            return;
        if(ret == QMessageBox::Yes)
            mainFile = currentFile;
    }
    NewProjectWizard wizard(simulator, mainFile, this);

    //Quit if the wizard failed or has been quitted
    if (wizard.exec() != QDialog::Accepted)
        return;

    //Close already open projects
    closeProject();

    // The project needs to be saved in order to let the view
    // correctly identify the generated files
    this->saveProject(wizard.getProject());
    // Open the saved project
    this->openProject( wizard.getProject()->getFolder() +  wizard.getProject()->getProjectFileName());

    aFileWasAdded = false;
}

void MainWindow::simFinished ( int exitCode, QProcess::ExitStatus /*exitStatus*/ )
{
    QMessageBox::warning(0, "UVE",
                         QString("End of simulation. Exit code: %1").arg(exitCode));

    updateMenus();
}

void MainWindow::simulateExt()
{
#ifdef TARGET_WIN
    QString script=currentProject->getFolder() + UVESettings::generator_run_bat_file();
#else
    QString script=currentProject->getFolder() + UVESettings::generator_run_sh_file();
#endif

    QStringList arguments;
    arguments << "all";
    if (!QProcess::startDetached(script,arguments,currentProject->getFolder()))
    {
        QMessageBox::warning(0, "UVE",
                             "QuestaSim could not be started. Please check that the executable vsim.exe is accessible by the PATH environment variable.");
        return;
    }

    updateMenus();
}

void MainWindow::simulateInUve()
{
#ifdef TARGET_WIN
    QString script=currentProject->getFolder() + UVESettings::generator_run_bat_file();
#else
    QString script=currentProject->getFolder() + UVESettings::generator_run_sh_file();
#endif
    simProcess->setWorkingDirectory(currentProject->getFolder());

    consoleSim->clear();
    consoleSim->setProcess(simProcess);

    consoleDockWidget->show();

    QStringList arguments;
    arguments << "all";
    simProcess->start(script,arguments);
    if (!simProcess->waitForStarted(3000))
    {
        QMessageBox::warning(0, "UVE",
                             "QuestaSim could not be started. Please check that the executable vsim.exe is accessible by the PATH environment variable.");
        return;
    }

    updateMenus();
}

void MainWindow::regenerateProjects()
{
    // Generate self

    QString olddir = QString(UVESettings::getInstance()->value("regenerateDir","/home").toString());

    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    olddir,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    UVESettings::getInstance()->setValue("regenerateDir",dir);
    SVExport exporter;
    if (!exporter.regenerateDir(dir))
    {
        QMessageBox::warning(this,"Error",exporter.getErrorString());
    }
    if (activeMdiChild() != 0)
        activeMdiChild()->checkModifiedFromOutside();
}

void MainWindow::regenerate()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Regenerate Project");
    msgBox.setText("You are about to regenerate the project. Do you want to overwrite the current project, or write in another folder?");
    QPushButton* abort = msgBox.addButton(tr("Cancel"), QMessageBox::RejectRole);
    QPushButton* overwrite = msgBox.addButton(tr("Overwrite"), QMessageBox::AcceptRole);
    QPushButton* saveInFolder = msgBox.addButton(tr("Save in folder..."), QMessageBox::ActionRole);

    msgBox.exec();

     if (msgBox.clickedButton() == abort)
     {
         return;
     }
     else if (msgBox.clickedButton() == overwrite)
     {
         SVExport exporter;
         exporter.exportProject(currentProject);


         if (activeMdiChild() != 0)
             activeMdiChild()->checkModifiedFromOutside();

         updateMenus();
     }
     else if (msgBox.clickedButton() == saveInFolder)
     {
         QFileDialog fd;
         fd.setFileMode(QFileDialog::DirectoryOnly);
         fd.setOption(QFileDialog::ShowDirsOnly);
         fd.setDirectory(currentProject->getFolder());


         QString newExportFolder;
         if(fd.exec())
             newExportFolder = fd.selectedFiles().at(0);
         else
             return;

         currentProject->setFolder(newExportFolder);

         UveXmlExport otherExporter;
         otherExporter.saveProject(currentProject);

         SVExport exporter;
         exporter.exportProject(currentProject);

         if (activeMdiChild() != 0)
             activeMdiChild()->checkModifiedFromOutside();

         updateMenus();
     }
}

//Displays a message if a project already exists in the same directory, and save the whole
void MainWindow::saveProject(const UvmProject *project)
{
    //Quit if there is nothing to save
    if (!project)
        return;

    //Check for the directory. Does it exist? Is it empty?
    bool shouldRename = QFileInfo(project->getFolder()).isDir();
    if (shouldRename) {
        QDir dir(project->getFolder());
        if (dir.entryList(QDir::NoDotAndDotDot).count()==0)
            shouldRename = false;

    }
    if ( shouldRename ) {
        RenameProjectDialog dialog(project->getFolder(),this);

        //Move the directory
        if (dialog.exec()) {
            if ( ! QDir().rename(project->getFolder(),dialog.newProjectName()) ) {
                if ( ! QMessageBox::warning(this, tr("File operation error"),
                                            tr("An error occured while moving '%1' to '%2'").arg(project->getFolder(),dialog.newProjectName()) + "\n" +
                                            tr("Do you want to continue anyway?")) )
                    return;
            }
        }

        //Remove the old one
        else
            if ( ! this->removeDir(project->getFolder()) )
                QMessageBox::warning(this, tr("File operation error"),
                                     tr("An error occured while removing '%1'").arg(project->getFolder()) + "\n" +
                                     tr("Continuing anyway."));
    }

    //Generate the SV and save the project XML
    UveXmlExport xmlExporter;
    if (!xmlExporter.saveProject(project))
        QMessageBox::warning(this,tr("Errors on project save"),tr("There were errors while saving the project :\n%1").arg(xmlExporter.getErrorString()));
    SVExport exporter;
    if (!exporter.exportProject(project))
        QMessageBox::warning(this,tr("Errors on project SV export"),tr("There were errors while exporting the project to SystemVerilog:\n%1").arg(exporter.getErrorString()));
}

void MainWindow::exportAsPNG()
{

    // ask for image location and name
    QString fileName = QFileDialog::getSaveFileName( this ,
                                                     "Export as PNG..." ,
                                                     currentProject->getFolder() ,
                                                     "PNG Images (*.png)" );
    if ( QFileInfo( fileName ).suffix() != "png" )
    {
      fileName.append( QString( "." ) + "png" );
    }

    QPixmap image = projectView->getPixmap();

    // and save it to a PNG file
    if (!image.save( fileName, "PNG" , 100 )) {


        QMessageBox::critical( this, tr("Export as PNG") , tr("There were errors while exporting the project to PNG."));
    }
}

void MainWindow::changeActiveComponent()
{
    if(activeMdiChild() != 0)
    {
        projectView->changeActiveComponent(activeMdiChild()->currentFile());
        newActiveComponent(activeMdiChild()->currentFile());
    }
    else
    {
        projectView->noActiveComponent();
    }
}

// Go to the line of the active Mdi Child
void MainWindow::goToLine(int line, QString inFile)
{
    openFile(inFile);

    if (activeMdiChild())
        activeMdiChild()->goToLine(line);
}

void MainWindow::goToSearchResult(int line, QString inFile, int index, QString term)
{
    openFile(inFile);

    if(activeMdiChild())
        activeMdiChild()->goToSearchResult(line, index, term);
}


// creation of widgets of the application
void MainWindow::createDocks()
{
    /* file view widget */
    fileTreeWidget = new FileTreeWidget(this,this);

    fileDockWidget = new QDockWidget(tr("Project Files"), this);
    fileDockWidget->setWidget(fileTreeWidget);
    fileDockWidget->close();
    fileDockWidget->setFeatures(QDockWidget::AllDockWidgetFeatures);
    addDockWidget(Qt::LeftDockWidgetArea, fileDockWidget);
    //CONNECT(fileView, SIGNAL(activated(QModelIndex)), this, SLOT(onActivated(QModelIndex)));

    /* graphic file view widget */
    projectView = new GraphicalProjectView(this);
    graphicFileDockWidget = new QDockWidget(tr("Project View"), this);
    graphicFileDockWidget->setFeatures(QDockWidget::AllDockWidgetFeatures);
    graphicFileDockWidget->close();
    graphicFileDockWidget->setWidget(projectView);
    addDockWidget(Qt::RightDockWidgetArea, graphicFileDockWidget);

    /* help widget */
    helpWidget = new HelpWidget(this);
    helpDockWidget = new QDockWidget(tr("Help"), this);
    helpDockWidget->setFeatures(QDockWidget::AllDockWidgetFeatures);
    helpDockWidget->setTitleBarWidget( 0 );
    helpDockWidget->setWidget(helpWidget);
    addDockWidget(Qt::RightDockWidgetArea, helpDockWidget);
    helpDockWidget->hide();

    /* project-wide search widget */
    messageWidget = new MessageWidget();
    searchDockWidget = new QDockWidget(tr("Project-wide Search"), this);
    searchDockWidget->setFeatures(QDockWidget::AllDockWidgetFeatures);
    searchDockWidget->setTitleBarWidget( 0 );
    searchDockWidget->setWidget(messageWidget);
    addDockWidget(Qt::BottomDockWidgetArea, searchDockWidget);
    searchDockWidget->hide();

    /* simulator console widget */
    consoleSim = new ProcessConsole();
    consoleDockWidget = new QDockWidget(tr("Simulation Output"), this);
    consoleDockWidget->setFeatures(QDockWidget::AllDockWidgetFeatures);
    consoleDockWidget->setTitleBarWidget( 0 );
    consoleDockWidget->setWidget(consoleSim);
    addDockWidget(Qt::BottomDockWidgetArea, consoleDockWidget);
    consoleDockWidget->hide();

    /* global dock settings */
    tabifyDockWidget(searchDockWidget, consoleDockWidget);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    /* signals */
    CONNECT(messageWidget, SIGNAL(goToLine(int, QString)), this, SLOT(goToLine(int, QString)));
    CONNECT(messageWidget, SIGNAL(goToSearchResult(int, QString, int, QString)), this, SLOT(goToSearchResult(int,QString,int,QString)));
    CONNECT(this, SIGNAL(updateMessages(QList<QList<QString> >)) ,messageWidget, SLOT(updateMessages(QList<QList<QString> >)));
    CONNECT(messageWidget, SIGNAL(search(QString, bool, bool)), this, SLOT(search(QString, bool, bool)));
    CONNECT(messageWidget, SIGNAL(replace(QString,QString,bool,QString,int,int, bool)), this, SLOT(replace(QString,QString,bool,QString,int,int, bool)));
    simProcess=new QProcess();
    CONNECT(simProcess,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(simFinished(int,QProcess::ExitStatus)));
    CONNECT(simProcess,SIGNAL(readyReadStandardOutput()),consoleSim,SLOT(logOutput()));
}


// actions creation of the application
void MainWindow::createActions()
{
    // File
    newAct = new QAction(QIcon(":/images/fileNew.png"), tr("&New file"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    CONNECT(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open file..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    CONNECT(openAct, SIGNAL(triggered()), this, SLOT(openFile()));

    saveAct = new QAction(QIcon(":/images/saveFile.png"), tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    CONNECT(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(QIcon(":/images/save.png"), tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    CONNECT(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAct = new QAction(QIcon::fromTheme("application-exit"), tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit the application"));
    CONNECT(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        recentFileActs[i] = new QAction(QIcon(":/images/fileIcon.png"), tr(""), this);
        recentFileActs[i]->setVisible(false);
        CONNECT(recentFileActs[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
    }

    // Edit
    cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the clipboard"));
    CONNECT(cutAct, SIGNAL(triggered()), this, SLOT(cut()));

    copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the clipboard"));
    CONNECT(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current selection"));
    CONNECT(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));


    fontAct = new QAction(QIcon(), tr("&Font selection"), this);
    fontAct->setStatusTip(tr("Choose the editor font"));
    CONNECT(fontAct, SIGNAL(triggered()), this, SLOT(fontDialog()));

    // Project
    newProjectAct = new QAction(QIcon(":/images/projectNew.png"),tr("&New Project"),this);
    newProjectAct->setShortcut(Qt::CTRL + Qt::ALT + Qt::Key_N);
    newProjectAct->setStatusTip(tr("Create a new project"));
    CONNECT(newProjectAct, SIGNAL(triggered()), this, SLOT(wizard()));

    openProjectAct = new QAction(QIcon(":/images/projectOpen.png"),tr("&Open Project..."), this);
    openProjectAct->setShortcut(Qt::CTRL + Qt::ALT + Qt::Key_O);
    openProjectAct->setStatusTip(tr("Open a project"));
    CONNECT(openProjectAct, SIGNAL(triggered()), this, SLOT(openProject()));

    closeProjectAct = new QAction(QIcon(":/images/projectClose.png"), tr("&Close Project"), this);
    closeProjectAct->setStatusTip(tr("Close the current project"));
    closeProjectAct->setShortcut(Qt::CTRL + Qt::ALT + Qt::Key_W);
    CONNECT(closeProjectAct, SIGNAL(triggered()), this, SLOT(closeProject()));

    exportAsPNGAct = new QAction(tr("&Export as PNG"), this);
    exportAsPNGAct->setStatusTip(tr("Export Project View as Image"));
    exportAsPNGAct->setIcon(QIcon(":/images/exportpng.png")); // TODO different ICON.
    CONNECT(exportAsPNGAct, SIGNAL(triggered()), this, SLOT(exportAsPNG()));

//    generateProjectsAct = new QAction(tr("&Regenerate project"), this);
//    generateProjectsAct->setShortcut(tr("Ctrl+D"));
//    generateProjectsAct->setStatusTip(tr("Regenerate a set of projects"));
//    CONNECT(generateProjectsAct, SIGNAL(triggered()), this, SLOT(regenerateProjects()));

    simulateInUveAct = new QAction(QIcon(":/images/startInUVE.png"),tr("Simulate in UVE"), this);
    //simulateAct->setShortcut(tr("Ctrl+D"));
    simulateInUveAct->setStatusTip(tr("Starts the simulation of the current project"));
    CONNECT(simulateInUveAct, SIGNAL(triggered()), this, SLOT(simulateInUve()));

    simulateExtAct = new QAction(QIcon(":/images/startInSimulator.png"),tr("Simulate in simulator"), this);
    //simulateAct->setShortcut(tr("Ctrl+D"));
    simulateExtAct->setStatusTip(tr("Starts the simulation of the current project in the simulator GUI"));
    CONNECT(simulateExtAct, SIGNAL(triggered()), this, SLOT(simulateExt()));

    for (int i = 0; i < MaxRecentProjects; ++i)
    {
        recentProjectActs[i] = new QAction(QIcon(":/images/projectIcon.png"), tr(""), this);
        recentProjectActs[i]->setVisible(false);
        CONNECT(recentProjectActs[i], SIGNAL(triggered()),
                this, SLOT(openRecentProject()));
    }

    // Tools
    regenerateAct = new QAction(QIcon(":/images/regenerate.png"),tr("&Regenerate"), this);
    regenerateAct->setShortcut(tr("Ctrl+R"));
    regenerateAct->setStatusTip(tr("Regenerate the testbench"));
    CONNECT(regenerateAct, SIGNAL(triggered()), this, SLOT(regenerate()));

    // View
//    fileDockWidget->toggleViewAction()->setIcon(QIcon(":/images/TODO"));
//    fileDockWidget->toggleViewAction()->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT +TODO));
    fileDockWidget->toggleViewAction()->setCheckable(true);
//    graphicFileDockWidget->toggleViewAction()->setIcon(QIcon(":/images/TODO"));
//    graphicFileDockWidget->toggleViewAction()->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + TODO));
    graphicFileDockWidget->toggleViewAction()->setCheckable(true);
//    searchDockWidget->toggleViewAction()->setIcon(QIcon(":/images/search.png"));
    searchDockWidget->toggleViewAction()->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_F));
    searchDockWidget->toggleViewAction()->setCheckable(true);
//    consoleDockWidget->toggleViewAction()->setIcon(QIcon(":/images/TODO"));
//    consoleDockWidget->toggleViewAction()->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + TODO));
    consoleDockWidget->toggleViewAction()->setCheckable(true);

    // Window
    closeAct = new QAction(QIcon(":/images/close.png"), tr("Cl&ose"), this);
    closeAct->setShortcut(tr("Ctrl+F4"));
    closeAct->setStatusTip(tr("Close the active window"));
    CONNECT(closeAct, SIGNAL(triggered()), mdiArea, SLOT(closeActiveSubWindow()));

    closeAllAct = new QAction(tr("Close &All"), this);
    closeAllAct->setStatusTip(tr("Close all the windows"));
    CONNECT(closeAllAct, SIGNAL(triggered()), mdiArea, SLOT(closeAllSubWindows()));

/*
    tileAct = new QAction(tr("&Tile"), this);
    tileAct->setStatusTip(tr("Tile the windows"));
    CONNECT(tileAct, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));

    cascadeAct = new QAction(tr("&Cascade"), this);
    cascadeAct->setStatusTip(tr("Cascade the windows"));
    CONNECT(cascadeAct, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));
*/

    nextAct = new QAction(tr("Ne&xt"), this);
    nextAct->setShortcuts(QKeySequence::NextChild);
    nextAct->setStatusTip(tr("Move the focus to the next window"));
    CONNECT(nextAct, SIGNAL(triggered()), mdiArea, SLOT(activateNextSubWindow()));

    previousAct = new QAction(tr("Pre&vious"), this);
    previousAct->setShortcuts(QKeySequence::PreviousChild);
    previousAct->setStatusTip(tr("Move the focus to the previous  window"));
    CONNECT(previousAct, SIGNAL(triggered()), mdiArea, SLOT(activatePreviousSubWindow()));

    separatorAct = new QAction(this);
    separatorAct->setSeparator(true);

    // Help
    helpAct = new QAction(QIcon(":/images/help-contents.png"), tr("&Help"), this);
    helpAct->setShortcut(QKeySequence::HelpContents);
    helpAct->setStatusTip(tr("Show the help"));
    helpAct->setCheckable(true);
    CONNECT(helpAct, SIGNAL(triggered()), this, SLOT(help()));

    updateAct = new QAction(QIcon(":/images/updateIcon.png"), tr("&Update..."), this);
    updateAct->setStatusTip(tr("Show the application's About box"));
    CONNECT(updateAct, SIGNAL(triggered()), this, SLOT(update()));

    aboutAct = new QAction(QIcon(":/images/aboutIcon.png"), tr("&About..."), this);
    aboutAct->setStatusTip(tr("Show installed version and update information"));
    CONNECT(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(QIcon(":/images/qt-logo.png"),tr("About &Qt..."), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    CONNECT(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

}


// menus creation of the application
void MainWindow::createMenus()
{
    /** File menu */
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
        fileMenu->addAction(recentFileActs[i]);
    fileMenu->addSeparator();
    updateRecentFileActions();
    fileMenu->addAction(exitAct);

    /** Edit menu */
    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addSeparator();
    editMenu->addAction(fontAct);

    /** Project menu */
    projectMenu = menuBar()->addMenu(tr("&Project"));
    projectMenu->addAction(newProjectAct);
    projectMenu->addAction(openProjectAct);
    projectMenu->addAction(closeProjectAct);
    projectMenu->addSeparator();
    projectMenu->addAction(exportAsPNGAct);
    projectMenu->addAction(regenerateAct);
    projectMenu->addAction(simulateInUveAct);
    projectMenu->addAction(simulateExtAct);
    projectMenu->addSeparator();
    for (int i = 0; i < MaxRecentProjects; ++i)
        projectMenu->addAction(recentProjectActs[i]);
    projectMenu->addSeparator();

    /** Tools menu */
//    toolMenu = menuBar()->addMenu(tr("&Tools"));
//    toolMenu->addAction(generateProjectsAct);

    /** View menu */
    viewMenu = menuBar()->addMenu(tr("&View"));
    CONNECT(viewMenu, SIGNAL(aboutToShow()), this, SLOT(updateViewMenu()));

    /** Window menu */
    windowMenu = menuBar()->addMenu(tr("&Window"));
    CONNECT(windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

    menuBar()->addSeparator();

    /** Help menu */
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(helpAct);
    helpMenu->addSeparator();
    helpMenu->addAction(updateAct);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}


// toolbars creation of the application
void MainWindow::createToolBars()
{
    /** File toolbar */
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->hide();

    /** Edit toolbar */
    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);
    editToolBar->addAction(cutAct);
    editToolBar->hide();

    /** Project toolbar */
    projectToolBar = addToolBar(tr("Project"));
    projectToolBar->addAction(newProjectAct);
    projectToolBar->addAction(openProjectAct);
    projectToolBar->addAction(closeProjectAct);
    projectToolBar->addSeparator();
    projectToolBar->addAction(exportAsPNGAct);
//    projectToolBar->addAction(regenerateAct);
    projectToolBar->addSeparator();
    projectToolBar->addAction(simulateInUveAct);
    projectToolBar->addAction(simulateExtAct);
    // only show text for simulate actions
    newProjectAct->setPriority(QAction::LowPriority);
    openProjectAct->setPriority(QAction::LowPriority);
    closeProjectAct->setPriority(QAction::LowPriority);
    exportAsPNGAct->setPriority(QAction::LowPriority);
    projectToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    /** View toolbar */
    viewToolBar = new QToolBar(tr("View"));
    viewToolBar->addAction(fileDockWidget->toggleViewAction());
    viewToolBar->addSeparator();
    viewToolBar->addAction(graphicFileDockWidget->toggleViewAction());
    viewToolBar->addSeparator();
    viewToolBar->addAction(searchDockWidget->toggleViewAction());
    viewToolBar->addSeparator();
    viewToolBar->addAction(consoleDockWidget->toggleViewAction());
    viewToolBar->addSeparator();
    viewToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    addToolBar(Qt::BottomToolBarArea, viewToolBar);
}

// statusbar creation of the application
void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Welcome to UVE. Enjoy your stay."));
}

void MainWindow::readSettings()
{

    UVESettings::getInstance()->beginGroup("mainwindow");
    QPoint pos = UVESettings::getInstance()->value("pos", QPoint(100, 100)).toPoint();
    QSize size = UVESettings::getInstance()->value("size", QSize(800, 600)).toSize();
    bool maximized = UVESettings::getInstance()->value("maximized", false).toBool();
    UVESettings::getInstance()->endGroup();

    if (maximized) {
        showMaximized();
    } else {
        move(pos);
        resize(size);
        show();
    }
}

void MainWindow::writeSettings()
{

    UVESettings::getInstance()->beginGroup("mainwindow");
    UVESettings::getInstance()->setValue("pos", pos());
    UVESettings::getInstance()->setValue("size", size());
    UVESettings::getInstance()->setValue("maximized",isMaximized());
    UVESettings::getInstance()->endGroup();
}

//Recursively remove the directory
bool MainWindow::removeDir(const QString &dir)
{
    QDir srcDir(dir);
    foreach(QFileInfo info, srcDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
        if (info.isDir()) {
            if (!removeDir(info.absoluteFilePath()))
                return false;
        } else if (info.isFile()) {
            if (!QFile::remove(info.absoluteFilePath()))
                return false;
        } else
            return false;
    }
    return QDir().rmdir(dir);
}

// Return the active MdiChild
MdiChild *MainWindow::activeMdiChild()
{
    if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
        return dynamic_cast<MdiChild *>(activeSubWindow->widget());
    return 0;
}

// Return an MdiChild corresponding
QMdiSubWindow *MainWindow::findMdiChild(const QString &fileName)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
        MdiChild *mdiChild = qobject_cast<MdiChild *>(window->widget());
        if (mdiChild->currentFile() == canonicalFilePath)
            return window;
    }
    return 0;
}

void MainWindow::setActiveSubWindow(QWidget *window)
{
    if(!window)
        return;
    mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

#include <QFont>

void MainWindow::fontDialog()
{

    bool ok;

    QVariant var=UVESettings::getInstance()->value(CONF_FONT,DEFAULT_FONT);
    QFont font=var.value<QFont>();

    font = QFontDialog::getFont(
                     &ok, font, this);
    if (ok) {
        foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
            MdiChild *mdiChild = qobject_cast<MdiChild *>(window->widget());
            mdiChild->setLexerFont(font);
        }
        UVESettings::getInstance()->setValue(CONF_FONT,font);
    }
}

void MainWindow::setCurrentFile(const QString &fileName)
{


    QStringList files = UVESettings::getInstance()->value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MaxRecentFiles)
        files.removeLast();

    UVESettings::getInstance()->setValue("recentFileList", files);

    foreach (QWidget *widget, QApplication::topLevelWidgets())
    {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin)
            mainWin->updateRecentFileActions();
    }
}

void MainWindow::updateRecentFileActions()
{

    QStringList files = UVESettings::getInstance()->value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), static_cast<int>(MaxRecentFiles));

    for (int i = 0; i < numRecentFiles; ++i)
    {
        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setToolTip(files[i]);
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);

    //    fileSeparatorAct->setVisible(numRecentFiles > 0);
}

void MainWindow::setCurrentProject(const QString &fileName)
{


    QStringList files = UVESettings::getInstance()->value("recentProjectList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MaxRecentProjects)
        files.removeLast();

    UVESettings::getInstance()->setValue("recentProjectList", files);

    foreach (QWidget *widget, QApplication::topLevelWidgets())
    {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin)
            mainWin->updateRecentProjectActions();
    }
}

void MainWindow::updateRecentProjectActions()
{

    QStringList files = UVESettings::getInstance()->value("recentProjectList").toStringList();

    int numRecentProjects = qMin(files.size(), static_cast<int>(MaxRecentProjects));

    for (int i = 0; i < numRecentProjects; ++i)
    {
//        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        if (QFileInfo(files[i]).exists()) {
            //UveXmlImport xmlImport;
            //UvmProject* temp_project = xmlImport.loadProject(files[i]);
            //QString text = tr("%1").arg(temp_project->getName());
            QString text = tr("%1").arg(strippedName(files[i]));
            recentProjectActs[i]->setText(text);
            recentProjectActs[i]->setData(files[i]);
            recentProjectActs[i]->setToolTip(files[i]);
            recentProjectActs[i]->setVisible(true);
        } else {
            QStringList recentProjects = UVESettings::getInstance()->value("recentProjectList").toStringList();
            recentProjects.removeAll(files[i]);
            UVESettings::getInstance()->setValue("recentProjectList", recentProjects);
            QMessageBox::information(this,tr("Project not found"),
                                     tr("The project file %1 could not be found and the project was removed from the recents list.").arg(files[i]));
        }
    }
    for (int j = numRecentProjects; j < MaxRecentProjects; ++j)
        recentProjectActs[j]->setVisible(false);
}


void MainWindow::openRecentProject()
{
    QAction *action = qobject_cast<QAction *>(sender());

    if (action) {
        QString file = action->data().toString();
        if(!QFileInfo(file).exists())      {

            QStringList recentProjects = UVESettings::getInstance()->value("recentProjectList").toStringList();
            recentProjects.removeAll(file);
            UVESettings::getInstance()->setValue("recentProjectList", recentProjects);

            updateRecentProjectActions();

            QMessageBox::information(this,tr("Project not found"),
                                     tr("The project file %1 could not be found and the project was removed from the recents list.").arg(file));

            return;
        }

        openProject(file);
    }
}

void MainWindow::openRecentFile()
{

    QAction *action = qobject_cast<QAction *>(sender());
    if (action) {
        QString file = action->data().toString();
        if(!QFileInfo(file).exists())
        {

            QStringList recentFiles = UVESettings::getInstance()->value("recentFileList").toStringList();
            recentFiles.removeAll(file);
            UVESettings::getInstance()->setValue("recentFileList", recentFiles);

            updateRecentFileActions();

            QMessageBox::information(this,tr("File not found"),
                                     tr("The file %1 could not be found and was therefore removed from the recents list.").arg(file));

            return;
        }
        openFile(file);
    }
}


QString MainWindow::strippedName(const QString &fullFileName) const
{
    return QFileInfo(fullFileName).fileName();
}

UvmProject* MainWindow::getProject() const
{
    return currentProject;
}

void MainWindow::fileAdded()
{
    fileTreeWidget->populateTree();
    aFileWasAdded = true;
}
