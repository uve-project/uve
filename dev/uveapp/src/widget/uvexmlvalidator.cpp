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
#include <QtXmlPatterns>
#include "uvexmlvalidator.h"
#include "ui_uvexmlvalidator.h"

#include "xmlhighlighter.h"
#include "uvesettings.h"

class MessageHandler : public QAbstractMessageHandler
{
public:
    MessageHandler()
        : QAbstractMessageHandler(0)
    {
    }

    QString statusMessage() const
    {
        return m_description;
    }

    int line() const
    {
        return m_sourceLocation.line();
    }

    int column() const
    {
        return m_sourceLocation.column();
    }

protected:
    virtual void handleMessage(QtMsgType type, const QString &description,
                               const QUrl &identifier, const QSourceLocation &sourceLocation)
    {
        Q_UNUSED(type);
        Q_UNUSED(identifier);

        m_messageType = type;
        m_description = description;
        m_sourceLocation = sourceLocation;
    }

private:
    QtMsgType m_messageType;
    QString m_description;
    QSourceLocation m_sourceLocation;
};


UveXmlValidator::UveXmlValidator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UveXmlValidator)
{
    ui->setupUi(this);

    new XmlHighlighter(ui->XSDFileBrowser->document());
    new XmlHighlighter(ui->XMLFileBrowser->document());

    connect(ui->Close, SIGNAL(clicked()), this, SLOT(close()));

    ui->status->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
}

UveXmlValidator::~UveXmlValidator()
{
    delete ui;
}

void UveXmlValidator::setXML(QString fileName)
{
    QFile xmlFile(fileName);
    xmlFile.open(QIODevice::ReadOnly);
    const QString xmlText(QString::fromUtf8(xmlFile.readAll()));
    ui->XMLFileBrowser->setPlainText(xmlText);

    QString XSDfile;

    if(fileName.endsWith("uve"))
    {
        XSDfile = UVESettings::getInstance()->value("UVEXSD").toString();
    }
    if(fileName.endsWith("uvc"))
    {
        XSDfile = UVESettings::getInstance()->value("VIPXSD").toString();
    }
    QFile schemaFile(XSDfile);
    schemaFile.open(QIODevice::ReadOnly);
    const QString schemaText(QString::fromUtf8(schemaFile.readAll()));
    ui->XSDFileBrowser->setPlainText(schemaText);

    validate();
}

void UveXmlValidator::validate()
{
    const QByteArray schemaData = ui->XSDFileBrowser->toPlainText().toUtf8();
    const QByteArray instanceData = ui->XMLFileBrowser->toPlainText().toUtf8();

    MessageHandler messageHandler;

    QXmlSchema schema;
    schema.setMessageHandler(&messageHandler);

    schema.load(schemaData);

    bool errorOccurred = false;
    if (!schema.isValid()) {
        errorOccurred = true;
    } else {
        QXmlSchemaValidator validator(schema);
        if (!validator.validate(instanceData))
            errorOccurred = true;
    }

    if (errorOccurred) {
        ui->status->setText(messageHandler.statusMessage());
        moveCursor(messageHandler.line(), messageHandler.column());
    } else {
        ui->status->setText(tr("validation successful"));
    }

    const QString styleSheet = QString("QLabel {background: %1; padding: 3px}")
            .arg(errorOccurred ? QColor(Qt::red).lighter(160).name() :
                                 QColor(Qt::green).lighter(160).name());
    ui->status->setStyleSheet(styleSheet);
}

void UveXmlValidator::moveCursor(int line, int column)
{
    ui->XMLFileBrowser->moveCursor(QTextCursor::Start);
    for (int i = 1; i < line; ++i)
        ui->XMLFileBrowser->moveCursor(QTextCursor::Down);

    for (int i = 1; i < column; ++i)
        ui->XMLFileBrowser->moveCursor(QTextCursor::Right);

    QList<QTextEdit::ExtraSelection> extraSelections;
    QTextEdit::ExtraSelection selection;

    const QColor lineColor = QColor(Qt::red).lighter(160);
    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = ui->XMLFileBrowser->textCursor();
    selection.cursor.clearSelection();
    extraSelections.append(selection);

    ui->XMLFileBrowser->setExtraSelections(extraSelections);

    ui->XMLFileBrowser->setFocus();
}

void UveXmlValidator::textChanged()
{
    ui->XMLFileBrowser->setExtraSelections(QList<QTextEdit::ExtraSelection>());
}
