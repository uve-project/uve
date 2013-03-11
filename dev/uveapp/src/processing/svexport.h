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
#ifndef SVEXPORT_H
#define SVEXPORT_H

#include "uvmvisitor.h"

#include "uvmproject.h"
#include "svexportabstract.h"

class SVExport : public UvmPassiveVisitor, SVExportAbstract
{
protected:
    int int_error;
    QString errorString;
    QString generatedFile;
public:
    SVExport();

    QString getErrorString() const;


    /** Regenerate all projects in a directory
      All the project files recursively scanned from a directory are
      open and regenerated.
      */
    bool regenerateDir(const QString &dirPath);

    bool exportProject(const UvmProject *project);
    bool generateFile(const QString &srcFilename,
                      const QString &dstFilename,
                      QMap<QString,QString> replace);

    void generateFile(const UvmComponent *comp,QMap<QString,QString> &map);

    QString& parseInLineTags(QString &line,QMap<QString,QString> replace);

    const QString addWave(UvmComponent *comp,QString vif, QString sig, bool inHexa) const;

    void initMap(QMap<QString,QString> &map,const UvmComponent *comp) const;

    virtual void visit(const UvmTop *comp);
    virtual void visit(const UvmTestCase *comp);
    virtual void visit(const UvmTestbench *comp);
    virtual void visit(const UvmScoreboard *comp);

    virtual void visit(const UvmDesign *comp);

    virtual void visit(const UvmVerificationComponent *comp);
    virtual void visit(const UvmVirtualSequencer *comp);
    virtual void visit(const UvmAgent *comp);
    virtual void visit(const UvmCollector *comp);
    virtual void visit(const UvmDriver *comp);
    virtual void visit(const UvmMonitor *comp);
    virtual void visit(const UvmSequencer *comp);
    virtual void visit(const UvmSequence *comp);
    virtual void visit(const UvmSequenceItem *comp);

    virtual void visit(const UvmPackage *comp);
    virtual void visit(const UvmConfig *comp);
    virtual void visit(const UvmInterface *comp);

    virtual void exportConfig(const UvmVerificationComponent *comp);

    bool cpDir(const QString &srcPath, const QString &dstPath);

    virtual bool validateTemplate(const UvmProject* project);

    QString scoreboardAgentPortName(UvmAgent *agent,int index) const;

private:
    static const QString templateID;

};


#endif // SVEXPORT_H
