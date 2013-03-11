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
#ifndef XMLHIGHLIGHTER_H
#define XMLHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class XmlHighlighter : public QSyntaxHighlighter
{
public:
    XmlHighlighter(QTextDocument *parent=0);

protected:
    virtual void highlightBlock(const QString &text);

private:
    struct highlightRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<highlightRule> highlightRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;
    QTextCharFormat tagFormat;
    QTextCharFormat attributeFormat;
    QTextCharFormat attributeContentFormat;
    QTextCharFormat commentFormat;
};

#endif // XMLHIGHLIGHTER_H
