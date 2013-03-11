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
#ifndef UVM_ENV_H
#define UVM_ENV_H

#include "uvmcomponent.h"
#include "uvmagent.h"
#include "uvmscoreboard.h"
#include "uvmvirtualsequencer.h"

/**
  * This class represent an UVM Environment.
  */
class UvmEnv: public UvmComponent
{
    public:
        UvmEnv(QString name, UvmComponent *parent=0);

        void addAgent(UvmAgent *agent);
        void addScoreboard(UvmScoreboard *scoreboard);
        void addSequencer(UvmVirtualSequencer *virtualSequencer);

        void removeAgent(UvmAgent *agent);
        void removeScoreboard(UvmScoreboard *scoreboard);
        void removeSequencer(UvmVirtualSequencer *virtualSequencer);

        QList<UvmScoreboard*> getScoreboards();
        QList<UvmAgent*> getAgents();
        QList<UvmVirtualSequencer*> getVirtualSequencers();
};

#endif // UVM_ENV_H
