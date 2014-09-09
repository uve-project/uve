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
#include "messagewidget.h"
#include <QCheckBox>
#include <QHeaderView>


// Contructor of the component
MessageWidget::MessageWidget()
{
    global = new QVBoxLayout();
    searchArea = new QHBoxLayout();
    searchFieldsArea = new QVBoxLayout();
    searchOptionsArea = new QGridLayout();
    //tagsLines = new QVBoxLayout();
    //tagsLine1 = new QHBoxLayout();
    //tagsLine2 = new QHBoxLayout();
    global->addLayout(searchArea);
    searchArea->addLayout(searchFieldsArea);
    searchArea->addLayout(searchOptionsArea);
    //searchArea->addLayout(tagsLines);
    //tagsLines->addLayout(tagsLine1);

    tagsArea = new QScrollArea();
    scrollingTagsArea = new QWidget();
    scrollingTagsArea->setLayout(new QHBoxLayout());
    tagsArea->setWidget(scrollingTagsArea);
    tagsArea->setWidgetResizable(true);
    tagsArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tagsArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollingTagsArea->layout()->setMargin(0);
    scrollingTagsArea->layout()->setContentsMargins(5,0,5,0);
    scrollingTagsArea->layout()->addItem(new QSpacerItem(0,20, QSizePolicy::Expanding, QSizePolicy::Fixed));
    scrollingTagsArea->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    tagsArea->setMaximumHeight(50);

    tags = new QList<QuickSearchWidget*>();

    searchButton = new QPushButton("search");
    CONNECT(searchButton, SIGNAL(clicked()), this, SLOT(searchButtonClicked()));
    searchButton->setEnabled(false);

    searchField = new QLineEdit;
    CONNECT(searchField, SIGNAL(textChanged(QString)), this, SLOT(searchFieldChanged(QString)));
    CONNECT(searchField, SIGNAL(returnPressed()), this, SLOT(searchByPressingEnter()));

    replaceField = new QLineEdit;
    CONNECT(replaceField, SIGNAL(textChanged(QString)), this, SLOT(replaceFieldChanged(QString)));

    replaceButton = new QPushButton("replace");
    CONNECT(replaceButton, SIGNAL(clicked()), this, SLOT(replaceButtonClicked()));
    replaceButton->setEnabled(false);

    caseSensitiveCheckBox = new QCheckBox("Case Sensitive");
    regexCheckBox = new QCheckBox("Regex");
    CONNECT(regexCheckBox, SIGNAL(clicked()), this, SLOT(regexCheckBoxClicked()));

    replaceRules = new QComboBox();
    replaceRules->addItem("One By One");
    replaceRules->addItem("All In Active File");
    replaceRules->addItem("In All Files");
    replaceRules->setEditable(false);

    table = new QTableWidget();
    table->setColumnCount(3);
    table->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Interactive);
    table->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Interactive);
    table->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    table->setColumnWidth(0,200);
    table->setColumnWidth(1, 40);
    CONNECT(table, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(selectionChanged(QModelIndex)));
    QStringList labels;
    labels << "File" << "Line" << "Description";
    table->setHorizontalHeaderLabels(labels);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    searchFieldsArea->addWidget(searchField);
    searchOptionsArea->addWidget(searchButton, 0, 0);
    searchFieldsArea->addWidget(replaceField);
    searchOptionsArea->addWidget(replaceButton, 1, 0);
    searchOptionsArea->addWidget(caseSensitiveCheckBox, 0, 1);
    searchOptionsArea->addWidget(regexCheckBox, 0, 2);
    searchOptionsArea->addWidget(replaceRules, 1, 1);
    global->addWidget(tagsArea);
    global->addWidget(table);

    TODOpushButton = new QuickSearchWidget("TODO", false);
    CONNECT(TODOpushButton, SIGNAL(hasBeenActivated()), this, SLOT(addInactiveTag()));
    CONNECT(TODOpushButton, SIGNAL(search(QString)), this, SLOT(searchThat(QString)));
    CONNECT(TODOpushButton, SIGNAL(destroyed()), this, SLOT(tagClosing()));
    tags->append(TODOpushButton);
    scrollingTagsArea->layout()->addWidget(TODOpushButton);
    addInactiveTag();

//    balanceTagsLines();
    this->setLayout(global);

    indexOfLastActivated = -1;
}


// Slot for update messages
void MessageWidget::updateMessages(QList<QList<QString> > list)
{
    occurences = list;
    table->clear();
    QStringList labels;
    labels << "File" << "Line" << "Description";
    table->setHorizontalHeaderLabels(labels);
    table->setRowCount(list.size());
    int l = 0;
    QTableWidgetItem *item;
    foreach (QList<QString> line, list)
    {
        item=new QTableWidgetItem(line.at(0));
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        table->setItem(l,0,item);
        item=new QTableWidgetItem(line.at(1));
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        table->setItem(l,1,item);
        item=new QTableWidgetItem(line.at(2));
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        table->setItem(l,2,item);
        table->setRowHeight(l,20);
        l++;
    }
}


// Slot for selection changes
void MessageWidget::selectionChanged(QModelIndex /*index*/)
{
    if(table->selectedItems().count() != 0)
    {
        // Get all selections
        QModelIndexList indexes = table->selectionModel()->selection().indexes();

        QModelIndex index = indexes.at(0);

        indexOfLastActivated = index.row();

        QList<QString> selectedOccurence = occurences.at(indexOfLastActivated);

        int lineNumber = selectedOccurence.at(1).toInt() -1;

        QString path = selectedOccurence.at(3);

        goToSearchResult(lineNumber, path.toLatin1(), selectedOccurence.at(4).toInt(), selectedOccurence.at(5));
    }
    else indexOfLastActivated = -1;
}

void MessageWidget::addInactiveTag()
{
    QuickSearchWidget* tmp = new QuickSearchWidget();
    tags->append(tmp);
    CONNECT(tmp, SIGNAL(hasBeenActivated()), this, SLOT(addInactiveTag()));
    CONNECT(tmp, SIGNAL(search(QString)), this, SLOT(searchThat(QString)));
    CONNECT(tmp, SIGNAL(destroyed()), this, SLOT(tagClosing()));
    CONNECT(tmp, SIGNAL(checkForDuplicate(QString,QuickSearchWidget*)), this, SLOT(checkForDuplicateQST(QString,QuickSearchWidget*)));
    scrollingTagsArea->layout()->addWidget(tmp);
    //balanceTagsLines();
}

void MessageWidget::searchThat(QString term)
{
    searchField->setText(term);
    search(term, caseSensitiveCheckBox->isChecked(), regexCheckBox->isChecked());
}

void MessageWidget::searchByPressingEnter()
{
    if(!(searchField->text().isEmpty()))
    {
        search(searchField->text(), caseSensitiveCheckBox->isChecked(), regexCheckBox->isChecked());
    }
}

void MessageWidget::searchButtonClicked()
{
    search(searchField->text(), caseSensitiveCheckBox->isChecked(), regexCheckBox->isChecked());
    replaceButton->setEnabled(searchField->text().compare("") !=0 && replaceField->text().compare("") !=0 && occurences.length() > 0 && !regexCheckBox->isChecked());
}

void MessageWidget::replaceButtonClicked()
{
    switch (replaceRules->currentIndex())
    {
    case 0: //one by one
    {
        if(indexOfLastActivated == -1)
        {
            if(occurences.length() > 0)
                indexOfLastActivated =0;
        }
        else
        {
            QList<QString> selectedOccurence = occurences.at(indexOfLastActivated);

            emit replace(searchField->text(), replaceField->text(), caseSensitiveCheckBox->isChecked(), selectedOccurence.at(3).toLatin1(), selectedOccurence.at(1).toInt() -1, selectedOccurence.at(4).toInt(), false);

            if(occurences.length()-1 > indexOfLastActivated)
                indexOfLastActivated++;
            else
                indexOfLastActivated = -1;
        }
        if(indexOfLastActivated >= 0)
        {
            table->selectRow(indexOfLastActivated);

            QList<QString> selectedOccurence = occurences.at(indexOfLastActivated);

            int lineNumber = selectedOccurence.at(1).toInt() -1;

            QString path = selectedOccurence.at(3);

            goToLine(lineNumber, path.toLatin1());
        }

        break;
    }
    case 1: //in active File
    {
        for(int i =0; i< table->rowCount(); i++)
        {
            QList<QString> selectedOccurence = occurences.at(i);

            emit replace(searchField->text(), replaceField->text(), caseSensitiveCheckBox->isChecked(), selectedOccurence.at(3).toLatin1(), selectedOccurence.at(1).toInt() -1, selectedOccurence.at(4).toInt(), true);
        }
        break;
    }
    case 2: //everywhere
    {
        for(int i =0; i< table->rowCount(); i++)
        {
            QList<QString> selectedOccurence = occurences.at(i);

            emit replace(searchField->text(), replaceField->text(), caseSensitiveCheckBox->isChecked(), selectedOccurence.at(3).toLatin1(), selectedOccurence.at(1).toInt() -1, selectedOccurence.at(4).toInt(), false);
        }
        break;
    }
    default:
    {
        break;
    }
    }
}

void MessageWidget::searchFieldChanged(QString term)
{
    searchButton->setEnabled(term.compare("") !=0);
    replaceButton->setEnabled(term.compare("") !=0 && occurences.length() > 0 && replaceField->text().compare("") !=0 && !regexCheckBox->isChecked());
}

void MessageWidget::replaceFieldChanged(QString term)
{
    replaceButton->setEnabled(term.compare("") !=0 && occurences.length() > 0 && !regexCheckBox->isChecked());
}

void MessageWidget::regexCheckBoxClicked()
{
    replaceButton->setEnabled(replaceField->text().compare("") !=0 && occurences.length() > 0 && !regexCheckBox->isChecked());
}

void MessageWidget::checkForDuplicateQST(QString term, QuickSearchWidget *caller)
{
    for(int i=0; i < tags->length(); i++)
    {
        QuickSearchWidget* tag = tags->at(i);
        if(tag->isActive() && tag->searchTerm().compare(term, Qt::CaseSensitive) == 0)
        {
            QMessageBox::critical(this, "Error", "A Quick Search Tag already exists with this term. ");
            return;
        }
    }
    caller->activateWithTerm(term, true);
}

void MessageWidget::tagClosing()
{
    balanceTagsLines();
    for(int i=0; i< tags->length(); i++)
    {
        if(tags->at(i) == NULL)
        {
            tags->removeAt(i);
            i--;
        }
    }
}

// Receive widget resize events which are passed in the event parameter
void MessageWidget::resizeEvent(QResizeEvent */*e*/)
{
    //QTableWidget::resizeEvent(e);
    return;

    // Jamais execute???
    QRect cr = contentsRect();
    int width0=table->columnWidth(0);
    int width=cr.width()-width0;
    table->setColumnWidth(1,width);
}

void MessageWidget::balanceTagsLines()
{
    for(int i=0; i<tags->length(); i++)
    {
        searchArea->addWidget(tags->at(i));
    }
}
