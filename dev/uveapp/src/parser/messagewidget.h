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
#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QtGui>
#include <QTableWidget>
#include <QComboBox>
#include <QScrollArea>
#include "vhdlutils.h"
#include "quicksearchwidget.h"
#include "connect.h"
#include "QList"

class MessageWidget : public QWidget
{
    Q_OBJECT

    public:
        MessageWidget();

    public slots:
        void updateMessages(QList<QList<QString> > list);
        void selectionChanged(QModelIndex index);
        void addInactiveTag();
        void searchThat(QString term);
        void searchButtonClicked();
        void replaceButtonClicked();
        void searchFieldChanged(QString term);
        void replaceFieldChanged(QString term);
        void searchByPressingEnter();
        void tagClosing();
        void regexCheckBoxClicked();
        void checkForDuplicateQST(QString term, QuickSearchWidget* caller);

    protected:
    void resizeEvent(QResizeEvent *event);

    signals:
    void goToLine(int row, QString inFile);
    void goToSearchResult(int row, QString inFile, int atIndex, QString term);
    void search(QString term, bool caseSensitive, bool regex);
    void replace(QString term, QString newTerm, bool caseSensitive, QString inFile, int atLine, int atIndex, bool activeFileOnly);

    private:
    void balanceTagsLines();

    QTableWidget* table;
    QuickSearchWidget* TODOpushButton;
    QList<QuickSearchWidget*>* tags;
    QLineEdit* searchField;
    QPushButton* searchButton;
    QVBoxLayout* global;
    QHBoxLayout* searchArea;
    QVBoxLayout* searchFieldsArea;
    QGridLayout* searchOptionsArea;
    QVBoxLayout* tagsLines;
    QHBoxLayout* tagsLine1;
    QHBoxLayout* tagsLine2;
    QScrollArea* tagsArea;
    QWidget* scrollingTagsArea;
    QLineEdit* replaceField;
    QPushButton* replaceButton;
    QCheckBox* caseSensitiveCheckBox;
    QCheckBox* regexCheckBox;
    QComboBox* replaceRules;
    QList<QList<QString> > occurences;
    int indexOfLastActivated;
};

#endif // MESSAGEWIDGET_H
