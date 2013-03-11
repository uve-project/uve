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
#include "newprojectwizardtester.h"


//#include <QTest>

#include "newprojectwizard.h"
#include "ui_projectdialogview.h"
#include "svexport.h"

NewProjectWizardTester::NewProjectWizardTester()
{
}

void NewProjectWizardTester::test()
{
    /*
    NewProjectWizard *wizard=new NewProjectWizard("monsimulateur", "/home/ythoma/docs/uve/git/dev/gui/test.vhd",0);

    wizard->initializePage(NewProjectWizard::ProjectPage);
    QTest::keyClicks(wizard->_project->ui->simulator,"monsimulateur");
    QTest::keyClicks(wizard->_project->ui->folder,"/home/ythoma/docs/uve/git/dev/gui/MyTest/");
//    QStringList files;
//    files << "/home/ythoma/docs/uve/git/dev/gui/test.vhd";
//    wizard->_project->addFiles(files);
    QTest::keyClicks(wizard->_project->ui->name,"MonProjet");

    wizard->_project->validatePage();

    wizard->initializePage(NewProjectWizard::EnvPage);

    wizard->_env->validatePage();

    wizard->initializePage(NewProjectWizard::SignalsPage);

    wizard->_signals->validatePage();

    wizard->createProject();

    SVExport exporter;
    exporter.exportProject(wizard->m_project);
*/
}
