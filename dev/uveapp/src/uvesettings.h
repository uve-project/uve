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
#ifndef UVESETTINGS_H
#define UVESETTINGS_H

#include <QString>
#include <QApplication>
#include <QDir>
#include <QSettings>

class UVESettings : public QSettings
{
private:
    UVESettings();
    static UVESettings *instance;
public:
    static UVESettings *getInstance();

    static QString generator_todo_file() {return "todo.txt";}
    static QString generator_main_file() {return "main.do";}
    static QString generator_compile_vhdl_file() {return "compile_vhdl.do";}
    static QString generator_compile_sv_file() {return "compile_sv.do";}
    static QString generator_wave_file() {return "wave.do";}
    static QString generator_run_bat_file() {return "run.bat";}
    static QString generator_run_sh_file() {return "run.sh";}

    static QString generator_template_folder() { return QApplication::applicationDirPath() + QDir::separator() +
                    QString("..") + QDir::separator() +
                    QString("templates") + QDir::separator();}

    static QString generator_gve_folder() { return
                      //generator_template_folder() +
                      QString("gve") + QDir::separator();}

    static QString generator_gtb_folder() { return
                      //generator_gve_folder() +
                      QString("gtb") + QDir::separator();}

    static QString generator_gvip_folder() { return
                      //generator_gve_folder() +
                      QString("gvip") + QDir::separator();}
};

#endif // UVESETTINGS_H
