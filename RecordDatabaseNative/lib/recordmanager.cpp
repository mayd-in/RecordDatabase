#include "recordmanager.h"

#include <QtCore>
#include <QtWidgets>

RecordManager::RecordManager(QTextDocument* textDocument, QObject *parent) :
    QObject(parent),
    m_textDocument(textDocument),
    m_currentRecord(nullptr)
{

}

Record* RecordManager::currentRecord()
{
    return m_currentRecord;
}

void RecordManager::create(QString recordId, QString name, QString surname)
{
    auto record = new Record{recordId, name, surname};

    m_textDocument->setHtml(QString("document for %1 %2 %3").arg(recordId, name, surname));
    m_textDocument->setModified(false);
    setCurrentRecord(record);
}

void RecordManager::setCurrentRecord(Record* record)
{
    if (m_currentRecord == record)
        return;

    m_currentRecord = record;
    emit currentRecordChanged();
}
