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
 Auteur        : Yann Thoma
                 Professeur HEIG-VD
                 Département TIC

 Modifications : Romain Cornaz
                 Etudiant HEIG-VD
                 Département TIC
 Modifications : Bastien Noverraz
                 Assistant HEIG-VD
                 ReDS
 Date   : 28.07.11

 */

#ifndef VHDLPARSER_H
#define VHDLPARSER_H


#include <QString>
#include <iostream>
#include "messagewidget.h"
#include "../uvmmodel/uvmport.h"
#include "../uvmmodel/uvmdesign.h"
#include "vhdlclasses.h"


class VHDLParserPrivate;

class VHDLFileContent;

/** Parse a VHDL file to fetch some data
  Right now, this parser fetches :
  \li The entity ports
  \li The entity name
  It supposes that the given VHDL file contains only one entity
  */
class VHDLParser
{
public:
    //! This object is not a singleton, but it uses one
    VHDLParser ();

    /** Read and parse a VHDL file
      The file will be parsed even if it's not a VHDL file, so be carefull.
      @param file The file to read
      @return The parser return code. No idea what it meant.
      */
    int read (const QString &file);

    //! Return a string containing a description of the error (maybe)
    QString errorString () const;

    //! Returns the content of the VHDL file
    //! This content is created every time the function is called, and so it is
    //! The responsibility of the caller to delete the object after use
    VHDLFileContent *getVHDLFileContent() const;

private:
    //THE instance of the parser
    VHDLParserPrivate *_parser;
    //Our errors
    QString _errors;

    QString curFileName;
};




//! This class should not be used by anyone but VHDLParser, use VHDLParser instead.
class VHDLParserPrivate
{
public:
    //! To use this parser one MUST use the instance returned by this function
    static VHDLParserPrivate *getCurrent ();




        /**
          Variables
          */

        char *globalText;

        int nbEntities;
        QVector<VHDLEntity*> entities;

        void initPort(VHDLPort& port);

        VHDLPort curPort;
        VHDLEntity *curEntity;

        QList<AnalyzerMessage*> m_messList;
        int m_parserError;
        int m_lineError;




        /**
          Functions
          */

        int parse (char *buf,int size);
        void setParserError (int line);
        void newMessage (int line, AnalyzerMessage::MessageType messType, QString variable);


        VHDLParserPrivate();

    protected:
        static VHDLParserPrivate *singleton;
        QMutex m_mutex;
};

#endif // VHDLPARSER_H
