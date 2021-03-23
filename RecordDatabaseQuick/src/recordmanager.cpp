#include "recordmanager.h"

#include <QtCore>
#include <QQuickTextDocument>

RecordManager::RecordManager(QObject *parent)
    : QObject(parent)
    , m_document(nullptr)
    , m_textDocument(nullptr)
{
}

QQuickTextDocument* RecordManager::document() const
{
    return m_document;
}

void RecordManager::setDocument(QQuickTextDocument* document)
{
    if (m_document == document)
        return;

    m_document = document;
    m_textDocument = document->textDocument();
    emit documentChanged();
}

Record RecordManager::currentRecord() const
{
    return m_currentRecord;
}

void RecordManager::setCurrentRecord(Record record)
{
    if (m_currentRecord == record)
        return;

    m_currentRecord = record;
    emit currentRecordChanged();
}


bool RecordManager::create(QString recordId, QString name, QString surname)
{
    auto record = Record{recordId, name, surname};

    m_textDocument->setHtml(QString("document for %1 %2 %3").arg(recordId, name, surname));
    m_textDocument->setModified(false);
    setCurrentRecord(record);
    return true;
}
