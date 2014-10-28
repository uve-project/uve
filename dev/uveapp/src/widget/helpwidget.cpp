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
#include "helpwidget.h"
#include "uveconstants.h"

#include <QHBoxLayout>

/** constructor [public].
 * Constructs a new help window.
 */
HelpWidget::HelpWidget(QWidget *parent) :
    QWidget(parent)
{ 
    setWindowTitle( tr("Help") );

    _view = new QWebView(this);
    QUrl myUrl = QUrl(UVE_MANUAL);
    myUrl.setScheme("file");
    _view->setUrl(myUrl);
    _view->show();

    QHBoxLayout *layout = new QHBoxLayout( this );
    layout->addWidget( _view );
}

/** destructor [public].
 * Free dynamically allocated memory
 */
HelpWidget::~HelpWidget()
{
    delete _view;
}
