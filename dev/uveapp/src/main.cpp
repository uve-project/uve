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
#include <QApplication>
#include "window/mainwindow.h"


#include "newprojectwizardtester.h"
#include "testgenerator.h"
#include "uvexmlimport.h"
#include "uveconstants.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("REDS");
    QCoreApplication::setOrganizationDomain("heig-vd.ch");
    QCoreApplication::setApplicationName("UVE");

    //Show the help
    if (app.arguments().contains("-h") || app.arguments().contains("--help")) {
        qDebug() << "UVE, an UVM helper EDI";
        qDebug() << "Usage : uve [-h|--help] [-test] [-t] [-b] [--tree {project XML} {output}]";
        qDebug() << "Arguments :";
        qDebug() << "   -h | --help : Show this help message";
        qDebug() << "   -b          : Batch mode, quit without opening the GUI";
        qDebug() << "   -t          : Create a fake project, save it in XML and then load it";
        qDebug() << "   -test path  : Launch a series of tests to validate the software";
        qDebug() << "                 path contains a valid path into the projects will be created";
        qDebug() << "   --tree {xml} {output} : Create a tree view in ASCII from a XML project file";
        qDebug() << "";
        qDebug() << "Open File Usage : uve fileName";
        qDebug() << "Arguments :";
        qDebug() << "   fileName    : path to the .uve project file to open";
        exit(0);
    }

    //The --tree option create a tree from a project file
    int i;
    if ((i = app.arguments().indexOf("--tree")) != -1) {
        UveXmlImport importer;
        UvmProject *project=importer.loadProject(app.arguments().at(i+1));
        if (!project) {
            qDebug() << importer.getErrorString();
            exit(1);
        }
        project->printTree(app.arguments().at(i+2));
        exit(0);
    }

    if (app.arguments().contains("-test")) {
        int index=app.arguments().indexOf("-test");
        if (index>app.arguments().size()-2)
            exit(1);
        QString path=app.arguments().at(index+1);
        qInstallMsgHandler(testMessageOutput);
        setVerbosity(VERBOSITY_DEBUG);

        TestGenerator tester;
        tester.testAll(path);
        exit(0);
    }

    /*
        //If the -t flag is given, save and load the project through XML to finally display it
        if (app.arguments().contains("-t")) {
            //For the fun, save it
            p->printTree("in.txt");
            p->saveXML();
            //And then load it
            p->loadXML("Counter Project/uve.xml");
            p->printTree("out.txt");
            //Just to show it
            mainWin.openProject(p);
        }
    */

    // If the -b flag is given, quit directly
    if (app.arguments().contains("-b"))
        exit(0);

    // Else starts the GUI
    MainWindow mainWin;

    if (app.arguments().size() == 2)
    {
        mainWin.openProject(app.arguments()[1]);
    }

    return app.exec();
}
