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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>

#include "mdichild.h"

#include "graphicalprojectview.h"
#include "helpwidget.h"         // Window to show some help

#include "uvmtop.h"
#include "uvmtestbench.h"
#include "uvmverificationcomponent.h"
#include "uvmagent.h"
#include "uvmsequencer.h"
#include "uvmvirtualsequencer.h"
#include "uvmsequenceitem.h"
#include "uvmdriver.h"
#include "uvminterface.h"
#include "uvmmonitor.h"
#include "uvmcomponent.h"
#include "uvmconfig.h"
#include "uvmproject.h"

//Parser
#include "messagewidget.h"
#include "vhdlparser.h"
#include "filetreewidgetitem.h"
#include "connect.h"
#include "processconsole.h"
#include "filetreewidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    UvmProject* getProject() const;

public slots:

    void openFile(const QString &file = "");
    void search(QString term, bool caseSensitive, bool regex);
    void replace(QString term, QString newTerm, bool caseSensitive, QString inFile, int atLine, int atIndex, bool activeFileOnly);
    void fileAdded();
protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newFile();
    void save();
    void saveAs();
    void cut();
    void copy();
    void paste();
    void about();
    void update();
    void help();
    void updateMenus();
    void updateWindowMenu();
    void updateViewMenu();
    MdiChild *createMdiChild();
    void setActiveSubWindow(QWidget *window);
    void onActivated(QModelIndex index);
    void newProject();
    void openProject();
    void closeProject();
    void wizard();
    void goToLine(int, QString);
    void goToSearchResult(int, QString, int, QString);
    void regenerate();
    void regenerateProjects();
    void simulateInUve();
    void simulateExt();

    void saveProject(const UvmProject *project);
    void exportAsPNG();

    void changeActiveComponent();

    void simFinished ( int exitCode, QProcess::ExitStatus exitStatus );

    void openRecentFile();
    void openRecentProject();

signals:
    void updateMessages(QList<QList<QString> >);
    void updateHighlighter();
    void newActiveComponent(QString path);
    void closeAllWithoutSaving();
    void closeAllAndSave();

private:
    void createActions();
    void createDocks();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();

    void setCurrentFile(const QString &fileName);
    void setCurrentProject(const QString &fileName);
    void updateRecentFileActions();
    void updateRecentProjectActions();

    enum { MaxRecentProjects = 5 };
    QAction *recentProjectActs[MaxRecentProjects];

    enum { MaxRecentFiles = 5 };
    QAction *recentFileActs[MaxRecentFiles];

    QString strippedName(const QString &fullFileName) const;
    void openProject(const QString &file);


    bool removeDir (const QString &dir);

    MdiChild *activeMdiChild();
    QMdiSubWindow *findMdiChild(const QString &fileName);

    QMdiArea *mdiArea;
    QSignalMapper *windowMapper;

    QDockWidget *graphicFileDockWidget;
    GraphicalProjectView *projectView;

    QDockWidget *helpDockWidget;    // dock widget for the help document
    HelpWidget  *helpWidget;        // widget for the help document

    QDockWidget *fileDockWidget;
    FileTreeWidget *fileTreeWidget;

    QDockWidget *searchDockWidget;
    MessageWidget *messageWidget;

    ProcessConsole *consoleSim;
    QDockWidget *consoleDockWidget;


    QMenu *fileMenu;
    QMenu *projectMenu;
    QMenu *editMenu;
    QMenu *toolMenu;
    QMenu *viewMenu;
    QMenu *windowMenu;
    QMenu *helpMenu;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *projectToolBar;
    QToolBar *viewToolBar;

    QAction *newAct;
    QAction *newProjectAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *closeAct;
    QAction *closeAllAct;
    QAction *closeProjectAct;
    QAction *exportAsPNGAct;
    /*QAction *tileAct;
    QAction *cascadeAct;*/
    QAction *nextAct;
    QAction *previousAct;
    QAction *separatorAct;
    QAction *helpAct;
    QAction *updateAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *closeFileAction;
    QAction *openProjectAct;
    QAction *regenerateAct;
    QAction *generateProjectsAct;
    QAction *simulateInUveAct;
    QAction *simulateExtAct;

    UvmProject *currentProject;
    QString currentFile;

    bool aFileWasAdded;

    //! Process launched for a simulation
    //! The output of the process is displayed in the console dock widget
    QProcess *simProcess;
};

#endif
