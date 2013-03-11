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
/*

UbiManager, an open source software for managing ubidule
configurations, simulations, and ubichip communication.
Copyright (C) 2007 Yann Thoma <name dot surname at heig-vd dot ch>
Reconfigurable and Embedded Digital Systems Institute
        http://reds.heig-vd.ch
HEIG-VD http://www.heig-vd.ch

See AUTHORS for more details about other contributors.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

*/

#include "processconsole.h"

//! Empty Constructor
/*! Sets m_process and m_numberwritten to 0
*/
ProcessConsole::ProcessConsole()
{
	m_process=0;
	m_numberwritten=0;
//	setReadOnly(true);
}

//! Sets the process to monitor
/*! \param process a pointer to the process to monitor
  */
void ProcessConsole::setProcess(QProcess *process)
{
	m_process=process;
}

//! Displays the output from the process
/*!
	This method appends the output from the process to the text displayed
	in the console.
	It is a slot that should be connected to the readyReadStandardOutput() signal
	of the process.
	If more than 100000 lines have been writtent, then it clears the console
*/
void ProcessConsole::logOutput()
{
	if (m_process==0)
		return;
	QByteArray bytes=m_process->readAllStandardOutput();
	m_numberwritten+=bytes.size();
	if (m_numberwritten>100000)
	{
		m_numberwritten=0;
		clear();
	}
	append(QString(bytes));
}
