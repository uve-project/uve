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
#ifndef WIZARDDATACONVERTER_H
#define WIZARDDATACONVERTER_H

#include "viewdata.h"

class WizardDataConverter
{
public:
    WizardDataConverter();

    bool data2Project(
            //View models
            const ProjectViewData *d_project,
            const TestCaseViewData *d_testcase,
            const TestBenchViewData *d_testbench,
            const SignalsViewData *d_signals,
            UvmProject *project);

    bool project2Data(
            const UvmProject *project,
            ProjectViewData *d_project,
            TestCaseViewData *d_testcase,
            TestBenchViewData *d_testbench,
            SignalsViewData *d_signals);


    QString linkName(UvmComponent *from, UvmComponent *to) const;
    QString getErrorString() const;
private:
    QString errorString;

};

#endif // WIZARDDATACONVERTER_H
