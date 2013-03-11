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
#include "quicksearchwidget.h"

QuickSearchWidget::QuickSearchWidget(QWidget *parent) :
    QWidget(parent)
{
    layout = new QHBoxLayout();
    layout->setSpacing(0);
    layout->setMargin(0);
    setLayout(layout);
    btn = new QPushButton("+");
    btn->setObjectName("tag");
    active = false;
    btn->setStyleSheet("QPushButton#tag {color: black; border-image: url(:/images/greybutton.png) 3 10 3 10; border-top: 3px; border-bottom: 3px; border-right: 10px; border-left: 10px;}"
                  "QPushButton#tag:hover{color: black; border-image: url(:/images/greenbutton.png) 3 10 3 10; border-top: 3px; border-bottom: 3px; border-right: 10px; border-left: 10px;}"
                  "QPushButton#tag:click{color: black; border-image: url(:/images/greenbuttonclicked.png) 3 10 3 10; border-top: 3px; border-bottom: 3px; border-right: 10px; border-left: 10px;}");
    CONNECT(btn, SIGNAL(clicked()), this, SLOT(hasBeenClicked()));
    layout->addWidget(btn);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
}

QuickSearchWidget::QuickSearchWidget(QString term, bool closable, QWidget *parent) :
    QWidget(parent)
{
    arg = term;
    layout = new QHBoxLayout();
    layout->setSpacing(0);
    setLayout(layout);
    btn = new QPushButton(term);
    CONNECT(btn, SIGNAL(clicked()), this, SLOT(hasBeenClicked()));
    btn->setObjectName("tag");
    layout->addWidget(btn);
    active = true;
    hasBeenActivated();
    if(closable)
    {

        btn->setStyleSheet("QPushButton#tag {color: black; border-image: url(:/images/greyrightchopped.png) 3 10 3 10; border-top: 3px; border-bottom: 3px; border-right: 10px; border-left: 10px;}"
                           "QPushButton#tag:hover{color: black; border-image: url(:/images/greenbuttonrightchopped.png) 3 10 3 10; border-top: 3px; border-bottom: 3px; border-right: 10px; border-left: 10px;}"
                           "QPushButton#tag:click{color: black; border-image: url(:/images/greenbuttonrightchoppedclicked.png) 3 10 3 10; border-top: 3px; border-bottom: 3px; border-right: 10px; border-left: 10px;}");
        btn->setMinimumWidth(0);
        close = new QPushButton("x");
        close->setObjectName("close");
        layout->addWidget(close);
        close->setStyleSheet("QPushButton#close {color: black; border-image: url(:/images/greyleftchopped.png) 3 10 3 10; border-top: 3px; border-bottom: 3px; border-right: 10px; border-left: 10px;}"
                             "QPushButton#close:hover{color: black; border-image: url(:/images/redbuttonleftchopped.png) 3 10 3 10; border-top: 3px; border-bottom: 3px; border-right: 10px; border-left: 10px;}"
                             "QPushButton#tag:click{color: black; border-image: url(:/images/redbuttonleftchoppedclicked.png) 3 10 3 10; border-top: 3px; border-bottom: 3px; border-right: 10px; border-left: 10px;}");

        CONNECT(close, SIGNAL(clicked()), this, SLOT(close()));
        close->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }
    else
    {
        btn->setStyleSheet("QPushButton#tag {color: black; border-image: url(:/images/greybutton.png) 3 10 3 10; border-top: 3px; border-bottom: 3px; border-right: 10px; border-left: 10px;}"
                           "QPushButton#tag:hover{color: black; border-image: url(:/images/greenbutton.png) 3 10 3 10; border-top: 3px; border-bottom: 3px; border-right: 10px; border-left: 10px;}"
                           "QPushButton#tag:click{color: black; border-image: url(:/images/greenbuttonclicked.png) 3 10 3 10; border-top: 3px; border-bottom: 3px; border-right: 10px; border-left: 10px;}");
    }

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void QuickSearchWidget::activateWithTerm(QString term, bool closable)
{
    arg = term;
    active = true;
    hasBeenActivated();
    btn->setText(arg);
    if(closable)
    {

        btn->setStyleSheet("QPushButton#tag {color: black; border-image: url(:/images/greyrightchopped.png) 3 10 3 10; border-top: 3px; border-bottom: 3px; border-right: 10px; border-left: 10px;}"
                           "QPushButton#tag:hover{color: black; border-image: url(:/images/greenbuttonrightchopped.png) 3 10 3 10; border-top: 3px; border-bottom: 3px; border-right: 10px; border-left: 10px;}"
                           "QPushButton#tag:click{color: black; border-image: url(:/images/greenbuttonrightchoppedclicked.png) 3 10 3 10; border-top: 3px; border-bottom: 3px; border-right: 10px; border-left: 10px;}");
        close = new QPushButton("x");
        close->setObjectName("close");
        layout->addWidget(close);
        close->setStyleSheet("QPushButton#close {color: black; border-image: url(:/images/greyleftchopped.png) 3 10 3 10; border-top: 3px; border-bottom: 3px; border-right: 10px; border-left: 10px;}"
                             "QPushButton#close:hover{color: black; border-image: url(:/images/redbuttonleftchopped.png) 3 10 3 10; border-top: 3px; border-bottom: 3px; border-right: 10px; border-left: 10px;}"
                             "QPushButton#tag:click{color: black; border-image: url(:/images/redbuttonleftchoppedclicked.png) 3 10 3 10; border-top: 3px; border-bottom: 3px; border-right: 10px; border-left: 10px;}");

        CONNECT(close, SIGNAL(clicked()), this, SLOT(close()));
        close->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }
    else
    {
        btn->setStyleSheet("QPushButton#tag {color: black; border-image: url(:/images/greybutton.png) 3 10 3 10; border-top: 3px; border-bottom: 3px; border-right: 10px; border-left: 10px;}"
                           "QPushButton#tag:hover{color: black; border-image: url(:/images/greenbutton.png) 3 10 3 10; border-top: 3px; border-bottom: 3px; border-right: 10px; border-left: 10px;}"
                           "QPushButton#tag:click{color: black; border-image: url(:/images/greenbuttonclicked.png) 3 10 3 10; border-top: 3px; border-bottom: 3px; border-right: 10px; border-left: 10px;}");
    }

    btn->setMinimumWidth(0);
}

QString QuickSearchWidget::searchTerm()
{
    return btn->text();
}

bool QuickSearchWidget::isActive()
{
    return active;
}

void QuickSearchWidget::hasBeenClicked()
{
    if(active)
    {
        search(arg);
    }
    else
    {
        bool ok = false;
        arg = QInputDialog::getText(this, "Quick Search", "What term do you want to search for?", QLineEdit::Normal, QString(), &ok);
        if (ok && !(arg.isEmpty()))
        {
            checkForDuplicate(arg, this);
        }
    }
}
