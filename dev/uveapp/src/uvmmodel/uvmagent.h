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
#ifndef UVMAGENT_H
#define UVMAGENT_H

#include "uvmcomponent.h"
#include "uvmdriver.h"
#include "uvmmonitor.h"
#include "uvmsequencer.h"
#include "uvmcollector.h"
#include "uvmconfig.h"

/**
  * This class represent an uvm Agent.
  */
class UvmAgent: public UvmComponent
{
    UVMCOMPONENT_UTILS

    public:
        enum AgentType {ACTIV, PASSIV};

        UvmAgent(UvmComponent *parent=0);
        UvmAgent(QString shortName, UvmComponent *parent=0);

        void setDriver(UvmDriver *driver);
        UvmDriver* getDriver() const;

        void setMonitor(UvmMonitor *monitor);
        UvmMonitor* getMonitor() const;

        void setCollector(UvmCollector *collector);
        UvmCollector* getCollector() const;

        void setSequencer(UvmSequencer *sequencer);
        UvmSequencer* getSequencer() const;

        void setConfig(UvmConfig *config);
        UvmConfig* getConfig() const;

        void setType(AgentType type);
        AgentType getType() const;

        virtual QString getRelativeTemplateFileName() const;

        int getNbAgents() const;

        virtual ComponentType getComponentType();

    public slots:
        void setNbAgents(int nbAgents);

    private:
        AgentType type;
        int nbAgents;

};

#endif
