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
#ifndef UVMCOMPONENT_H
#define UVMCOMPONENT_H

#include <QDomElement>
#include <QDomDocument>
#include <QString>
#include <QObject>

#include "uvmvisitor.h"
#include "xmlvisitor.h"

#include "uvmport.h"
#include "componenttype.h"


#define DIRSEPARATOR "/"

class NonPhysicalPort;
class UvmPort;
class UvmProject;
class UvmConnection;
class UvmVerificationComponent;

class NameSpaceRegister;

struct BaseConnection;


#define UVMCOMPONENT_UTILS \
    public: \
    virtual void accept(UvmVisitor *visitor){ \
        visitor->visit(this); \
        foreach(UvmComponent *child, children) { \
            child->accept(visitor); \
        } \
    } \
    virtual void accept(UvmPassiveVisitor *visitor){ \
        visitor->visit(this); \
        foreach(UvmComponent *child, children) { \
            child->accept(visitor); \
        } \
    } \
    XMLVISITOR_ACCEPT



/**
  * This class represent an UVM Component.
  */
class UvmComponent : public QObject
{
    Q_OBJECT

    public:
        UvmComponent(UvmComponent *parent=0);
        UvmComponent(QString shortName, UvmComponent *parent);

        ~UvmComponent();

        virtual void accept(UvmVisitor *visitor) = 0;
        virtual void accept(UvmPassiveVisitor *visitor) = 0;
        virtual QDomElement accept(XmlVisitor *visitor) = 0;

        QString getConfigClassName() const;

        void addPort(NonPhysicalPort *newPort);
        void removePort(NonPhysicalPort *port);

        void addChild(UvmComponent *newChild);

        QList<NonPhysicalPort*> getPorts() const;
        QList<TlmPort*> getTlmPorts() const;
        QList<FakePort*> getFakePorts() const;

        QList<UvmComponent*> getChildren() const;
        QList<UvmComponent*> getAllChildren() const;

        virtual UvmComponent* getParent() const;
        void setParent(UvmComponent *parent);

        UvmTop* getTop();

        virtual QString getDefaultName () const;

        /** Set the object's instance name
          A parent must be set before this function is called.
          The default implementation sets the name and register it in the UvmProject.
          If the name is already used, this function automatically appends "_n" to the name, where "n" is a number.
          This way we're sure that the instance name is unique along the project.
          */
        virtual bool setShortName(const QString &shortName);
        QString getShortName() const;

        virtual QString getRelativeTemplateFileName() const;

        virtual QString getRelativePath () const;
        QString getAbsolutePath () const;

        QString getOwnRelativePath() const;

        virtual void setClassName (const QString &shortName);
        virtual QString getClassName () const;

        virtual void setInstName (const QString &shortName);
        virtual QString getInstName () const;

        virtual QString getRelativeInstName(const UvmComponent *ancestor) const;

        void setBodyFileName (const QString &filename);
        QString getBodyFileName () const;

        void setHeaderFileName (const QString & filename);
        QString getHeaderFileName () const;

        void addFile (const QString &filePath);
        void addNewFile (const QString &filePath);
        QList<QString> getOtherFiles() const;
        QList<QString> getAbsoluteOtherFiles() const;

        virtual QString getAbsoluteBodyFileName () const;
        virtual QString getRelativeBodyFileName () const;

        QString getAbsoluteHeaderFileName () const;
        QString getRelativeHeaderFileName () const;

        QString getConfigFileName() const;
        virtual QString getPackageFileName() const;
        QString getAbsolutePackageFileName() const;
        virtual QString getSequenceFileName() const;
        virtual QString getSequenceItemFileName() const;

        virtual UvmProject* getProject () const;
        UvmVerificationComponent *getParentVip () const;

        QList<UvmConnection*> connectionsWith (UvmComponent *other) const;

        /** This component's VC name
          If this component is not in a VC (testcase, testbench, etc) then the string will be empty.
          @return The name of the VC containing this object, or an empty string if there is none
          */
        virtual QString vcName () const;

        /** This component's namespace
          Will try to find the name space corresponding to this element.
          It might be a VC or the project itself.
          @param caller This value might be used by the object when it's called itself (see VC)
          */
        virtual NameSpaceRegister* nameRegister (void* caller = 0) const;

        void printComp() const;

        /** Create an ASCII view of the tree
          The component and each of its sub-components and ports will printed in the tree.
          @param deepness Current deepness (used for the begining spaces)
          */
        QString printTree(int deepness = 0) const;

        /** Create an HTML view of the tree
          The component and each of its sub-components and ports will printed in the tree.
          @param deepness Current deepness (used for the begining spaces)
          */
        QString printHtmlTree(int deepness = 0) const;

        //! Returns true (the default) if the component must be generated through exportSV
        virtual bool toGenerate() const;

        //! Set the component's generation flag (see above)
        virtual void setToGenerate (bool b);

        virtual QStringList getAllFilePaths() const;
        virtual QStringList getAllVIPFilePaths() const;
        virtual QStringList getAllNonVIPFilePaths() const;

        // Used to be protected. However, XmlUveImport needs to access it
        void setRelativePath (const QString &path);

        bool hasHeaderFile() const;
        bool hasPackage() const;

        virtual bool hasConfig() const;
        virtual bool hasSequenceItem() const;
        virtual bool hasSequence() const;

        virtual void addConnection(UvmConnection *connection);
        QList<UvmConnection*> getConnections() const;

        virtual UvmPort* findPortForName(const QString &name) const;

        virtual ComponentType getComponentType() const=0;
        ComponentType getConnectedComponentType(UvmPort* throughThis);

        void sortChildren(bool recursive);

signals:
        void updateView();

    protected:

        QList<NonPhysicalPort*> ports;
        QList<UvmComponent*> children;
        QList<UvmConnection*> connections;

        UvmComponent *parent;

        QString relativePath;
        QString className;
        QString instName;
        QString bodyFileName;
        QString headerFileName;
        QList<QString> otherFilesPaths;
        bool fileNameSet;
        bool _toGenerate;

    private:

        QString shortName;

};

#endif
