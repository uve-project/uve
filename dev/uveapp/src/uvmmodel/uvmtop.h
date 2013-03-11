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
#ifndef UVMTOP_H
#define UVMTOP_H

#include "uvmdesign.h"
#include "uvmtestcase.h"
#include "uvmproject.h"

#include "namespaceregister.h"

/**
  * This class represent an UVM top.
  */
class UvmTop: public UvmComponent
{
    Q_OBJECT

    UVMCOMPONENT_UTILS


    public:
        UvmTop(UvmProject *project);
        UvmTop(QString shortName, UvmProject *project);

        ~UvmTop();

        QString getRelativeTemplateFileName() const;

        virtual QString getRelativePath () const;

        UvmDesign* getDesign() const;
        void setDesign(UvmDesign *design);

        UvmTestCase* getTestCase() const;
        void setTestCase(UvmTestCase *design);

        UvmInterface* getInterface() const;
        void setInterface(UvmInterface * interface);

        /**
          * Re-implement this function because we have no father, but a project
          */
        UvmProject *getProject() const;

        //! We're not a VC, so return an empty string
        QString vcName () const;

        //! Return the project's register
        NameSpaceRegister* nameRegister (void* = 0) const;

        const QList<UvmVerificationComponent*> getVips() const;

        virtual ComponentType getComponentType();

    private:
        UvmProject *project;

};

#endif // UVMTOP_H
