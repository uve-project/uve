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
#ifndef VIPCONNECTORDIALOG_H
#define VIPCONNECTORDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QModelIndex>

#include "uvminterface.h"
#include "uvmverificationcomponent.h"
#include "uvmport.h"
#include "uvmconnection.h"
#include "newprojectwizard.h"

namespace Ui {
class VIPConnectorDialog;
}

/** Connect a VIP signal with a DUV's interface.
 * This is done using two list as source and representing the resulting connections in a third.
 * No UVM object are added until the accept() function is called.
 */
class VIPConnectorDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit VIPConnectorDialog(QWidget *parent = 0);
    ~VIPConnectorDialog();

    // Use this VIP and this Interface to fill the dialog list.
    // Any previous data will be overwritten.
    void setup (const QList<PhysicalPort *> &vipPorts, const QList<PhysicalPort *> &duvPorts, const QString &baseName, NewProjectWizard *project);

    // Return the list of resulting connections between these two components
    QList<BaseConnection> connections () const;

    // Return the name choosen by the user for this VIP
    QString name () const;
protected slots:

    // Connect these ports in the result list (creates no connexion)
    void connectPorts (UvmPort *src, UvmPort *dst);

    // Connect the selected ports in the VIP and DUV table
    void connectPorts ();

    // Disconnect the selected port in the result list
    void disconnectPorts ();

    //Activate/deactivate the "Connect" and "Disconnect" buttons
    void vipSelected (QModelIndex i, QModelIndex p);
    void duvSelected (QModelIndex i, QModelIndex p);
    void resultSelected (QModelIndex i, QModelIndex p);

    //Check the name for conflict
    void checkName();

protected:

    // Fill a table model using a list of ports
    void fillList (QStandardItemModel *model, const QList<PhysicalPort *> &ports);

    // Return the icon corresponding this the given direction
    QIcon directionIcon (UvmPort::PortDirection direction);
    
private:
    Ui::VIPConnectorDialog *ui;

    //Backend objects
    UvmVerificationComponent *_vc;
    UvmInterface *_interface;

    //Table models
    QStandardItemModel *_vipModel, *_duvModel, *_resultModel;

    //List selection
    bool _vipSelected, _duvSelected;

    //Base project (for namespace)
    NewProjectWizard *_project;
};

#endif // VIPCONNECTORDIALOG_H
