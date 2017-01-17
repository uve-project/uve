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
#ifndef TESTGENERATOR_H
#define TESTGENERATOR_H

#include <QString>

#define VERBOSITY_DEBUG    1
#define VERBOSITY_INFO     2
#define VERBOSITY_WARNING  3
#define VERBOSITY_CRITICAL 4
#define VERBOSITY_FATAL    5

void testMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
void setVerbosity(int verbosity);

class TestGenerator
{
public:
    TestGenerator();

    void testAll(const QString &outputPath);

    void testFull();


    void test(const QString &subdir,
              const QString &entityName,
              const QString &vhdlFileName,
              bool lowercase,
              int nbVips,
              int nbClassAgent,
              int nbInstAgents,
              int parameters=(1<<6)|(1<<9)|(1<<13) // Monitor, Sequencer, and Driver
            );


    void testWithVip(const QString &subdir,
              const QString &entityName,
              const QString &vhdlFileName,
              const QString &vipFileName,
              bool lowercase
            );


    void testXmlProject(QString projectFileName);

    void compareFiles(const QString &fileName1,const QString &fileName2);

protected:
    QString nameCase(const QString &name,bool toLowerCase) const;

    QString curDir;
    QString testInputDir;
};

#endif // TESTGENERATOR_H
