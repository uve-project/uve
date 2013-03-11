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
#ifndef UVMVERIFICATIONCOMPONENT_H
#define UVMVERIFICATIONCOMPONENT_H

#include "uvmcomponent.h"
#include "uvmagent.h"
#include "uvmscoreboard.h"
#include "uvmvirtualsequencer.h"
#include "uvmsequenceitem.h"
#include "uvmmonitor.h"
#include "uvminterface.h"
#include "namespaceregister.h"

// Forward declaration as we are included in UvmTestBench
class UvmTestCase;

/**
  * This class represent an UVM Environment.
  */
class UvmVerificationComponent: public UvmComponent, public NameSpaceRegister
{

    UVMCOMPONENT_UTILS

    public:
        UvmVerificationComponent(UvmComponent *parent=0);
        UvmVerificationComponent(QString shortName, UvmComponent *parent=0);

        virtual QString getRelativePath () const;

        //! Description of this VIP
        void setDescription (const QString &desc);
        QString getDescription () const;

        void addAgent(UvmAgent *agent);
        void removeAgent(UvmAgent *agent);
        QList<UvmAgent*> getAgents() const;

        void addVirtualSequencer(UvmVirtualSequencer *virtualSequencer);
        void removeVirtualSequencer(UvmVirtualSequencer *virtualSequencer);
        QList<UvmVirtualSequencer*> getVirtualSequencers() const;

        void addScoreboard(UvmScoreboard *scoreboard);
        void removeScoreboard(UvmScoreboard *scoreboard);
        QList<UvmScoreboard*> getScoreboards();

        void setBusMonitor(UvmMonitor *env);
        UvmMonitor* getBusMonitor() const;

        void setConfig(UvmConfig *config);
        UvmConfig* getConfig();

        UvmInterface* getInterface() const;
        void setInterface(UvmInterface * interface);

        void setClockName(QString shortName);
        QString getClockName();

        void setResetName(QString shortName);
        QString getResetName();

        QString getPackageFileName() const;

        UvmSequenceItem* getSequenceItem() const;

        UvmTestCase* getTestCase() const;

        void setVipFileName(const QString &fileName);
        QString getVipFileName() const;

        /** Returns our name
          */
        QString vcName () const;

        //! Return us, or return our parent's if we're calling ourselves (VC names belongs in the main register)
        NameSpaceRegister* nameRegister (void* caller = 0) const;

        virtual QStringList getAllVIPFilePaths();
        virtual QStringList getAllNonVIPFilePaths();

        virtual QString getRelativeTemplateFileName() const;
        QString getConfigTemplateFileName() const;

        void setOriginalVipPath(QString path);
        QString getOriginalVipPath() const;


        QString getVipRelativePath() const;


        void setVipRelativePath(const QString &path);


        void setConfigFileName(const QString &fileName);
        QString getConfigFileName() const;


        virtual UvmPackage* getPackage() const;
        virtual ComponentType getComponentType();

        bool getGenerated() const;
        void setGenerated(bool generated);

    protected:

    private:

        QString vipFileName;
        QString vipRelativePath;
        QString configFileName;

        // For an existing VIP, contains the path to the VIP.
        // This field does not need to be saved in the XML
        QString originalVipPath;

        //Simple helper to retrieve the text content of a balise
        QString valueOf (const QString &tag, const QDomElement &root) const;

        QString clockName;
        QString resetName;
        QString description;

        bool generated;
};

#endif // UVMVERIFICATIONCOMPONENT_H
