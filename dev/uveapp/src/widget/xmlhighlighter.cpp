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
#include "xmlhighlighter.h"

XmlHighlighter::XmlHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    highlightRule rule;
    // tag format
    tagFormat.setForeground(Qt::darkBlue);
    tagFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegExp("(<[a-zA-Z:]+\\b|<\\?[a-zA-Z:]+\\b|\\?>|>|/>|</[a-zA-Z:]+>)");
    rule.format = tagFormat;
    highlightRules.append(rule);
    // attribute format
    attributeFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("[a-zA-Z:]+=");
    rule.format = attributeFormat;
    highlightRules.append(rule);
    // attribute content format
    attributeContentFormat.setForeground(Qt::red);
    rule.pattern = QRegExp("(\"[^\"]*\"|'[^']*')");
    rule.format = attributeContentFormat;
    highlightRules.append(rule);
    commentFormat.setForeground(Qt::lightGray);
    commentFormat.setFontItalic(true);
    commentStartExpression = QRegExp("<!--");
    commentEndExpression = QRegExp("-->");
}
void XmlHighlighter::highlightBlock(const QString &text)
{
    foreach (const highlightRule &rule, highlightRules) {
        QRegExp expression(rule.pattern);
        int index = text.indexOf(expression);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = text.indexOf(expression, index + length);
        }
    }
    setCurrentBlockState(0);
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);
    while (startIndex >= 0) {
        int endIndex = text.indexOf(commentEndExpression, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, commentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
