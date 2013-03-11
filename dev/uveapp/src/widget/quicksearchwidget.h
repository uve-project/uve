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
#ifndef QUICKSEARCHWIDGET_H
#define QUICKSEARCHWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QDebug>
#include "connect.h"

class QuickSearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QuickSearchWidget(QWidget *parent = 0);
    QuickSearchWidget(QString term, bool closable, QWidget *parent = 0);
    void activateWithTerm(QString term, bool closable);
    QString searchTerm();
    bool isActive();
signals:
    void hasBeenActivated();
    void search(QString term);
    void checkForDuplicate(QString term, QuickSearchWidget* caller);
public slots:
    void hasBeenClicked();

private:
    QString arg;
    QPushButton* btn;
    QPushButton* close;
    bool active;
    QHBoxLayout* layout;

};

#endif // QUICKSEARCHWIDGET_H
