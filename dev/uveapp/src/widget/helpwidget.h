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
#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <QWidget>
#include <QtWebKit/QWebView>

/** HelpWidget.
 * This widget contains the application help in form of a html page.
 */
class HelpWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HelpWidget(QWidget *parent = 0);
    ~HelpWidget();
    
private:
    QWebView * _view;

};

#endif // HELPWIDGET_H
