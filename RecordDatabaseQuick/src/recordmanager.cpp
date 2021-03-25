#include "recordmanager.h"

#include <QtCore>
#include <QQuickTextDocument>
#include <QTextDocumentWriter>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

RecordManager::RecordManager(QObject *parent)
    : QObject(parent)
    , m_document(nullptr)
    , m_textDocument(nullptr)
{
    // FILE DIRECTORY PATH
    const QDir writeDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (!writeDir.mkpath("."))
        qFatal("Failed to create writable directory at %s", qPrintable(writeDir.absolutePath()));
    m_absolutePath = writeDir.absolutePath();

    setupDatabase();
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

RecordManager::Error RecordManager::create(QString recordId, QString name, QString surname)
{
    if (findRecord(recordId))
        return Error::RecordExists;

    auto fileName = QString("%1.htm").arg(recordId);
    auto record = Record{recordId, name, surname, fileName, false};

    m_textDocument->setHtml(QString("document for %1 %2 %3").arg(recordId, name, surname));
    m_textDocument->setModified(false);
    setCurrentRecord(record);
    return Error::NoError;
}

RecordManager::Error RecordManager::open(QString recordId)
{
    auto record = findRecord(recordId);
    if (!record)
        return Error::RecordNotExist;

    // FILE READ
    QFile file(m_absolutePath + "/" + record.fileName);
    if (!file.open(QFile::ReadOnly)) {
        qCritical("File open failed: %s", qPrintable(file.errorString()));
        return Error::FileOpenFailed;
    }

    auto data = file.readAll();
    auto codec = QTextCodec::codecForHtml(data);
    auto text = codec->toUnicode(data);

    // UPDATE DOCUMENT
    m_textDocument->setHtml(text);
    m_textDocument->setModified(false);
    setCurrentRecord(record);
    return Error::NoError;
}

RecordManager::Error RecordManager::save()
{
    auto record = m_currentRecord;
    if (!record)
        return Error::NoCurrentRecord;

    // FILE WRITE
    QTextDocumentWriter writer(m_absolutePath + "/" + record.fileName);
    bool success = writer.write(m_textDocument);
    if (!success) {
        qCritical("File write failed");
        return Error::FileSaveFailed;
    }

    // DATABASE WRITE
    QSqlQuery query;
    if (record.isSaved) {
        auto q = QString(
            "UPDATE Records SET "
            "last_modified = (datetime(CURRENT_TIMESTAMP, 'localtime')) "
            "WHERE record_id = :recordId"
        );
        query.prepare(q);
        query.bindValue(":recordId", record.recordId);
    }
    else {
        auto q = QString(
            "INSERT INTO Records (record_id, name, surname, filename) "
            "VALUES (:recordId, :name, :surname, :fileName)"
        );
        query.prepare(q);
        query.bindValue(":recordId", record.recordId);
        query.bindValue(":name", record.name);
        query.bindValue(":surname", record.surname);
        query.bindValue(":fileName", record.fileName);
    }

    success = query.exec();
    if (!success) {
        qCritical("Cannot save record to database: %s", qPrintable(query.lastError().text()));
        return Error::FileSaveFailed;
    }

    // UPDATE DOCUMENT
    m_textDocument->setModified(false);
    return Error::NoError;
}

void RecordManager::setupDatabase()
{
    QSqlDatabase database = QSqlDatabase::database();
    if (!database.isValid()) {
        database = QSqlDatabase::addDatabase("QSQLITE");
        if (!database.isValid())
            qFatal("Cannot add database: %s", qPrintable(database.lastError().text()));
    }

    const auto databasePath = m_absolutePath + "/db.sqlite3";
    database.setDatabaseName(databasePath);
    if (!database.open()) {
        qFatal("Cannot open database: %s", qPrintable(database.lastError().text()));
    }

    if (database.tables().contains(QStringLiteral("Records"))) {
        QSqlQuery query;
        if (!query.exec(
            "CREATE TABLE IF NOT EXISTS 'Records' ("
            "   'record_id' TEXT PRIMARY KEY,"
            "   'name' TEXT NOT NULL,"
            "   'surname' TEXT NOT NULL,"
            "   'created' DATETIME DEFAULT (datetime(CURRENT_TIMESTAMP, 'localtime')),"
            "   'last_modified' DATETIME DEFAULT (datetime(CURRENT_TIMESTAMP, 'localtime'))"
            ")")) {
            qFatal("Failed to create database: %s", qPrintable(query.lastError().text()));
        }
    }
}

Record RecordManager::findRecord(const QString& recordId) const
{
    auto q = QString("SELECT record_id, name, surname, filename FROM Records WHERE record_id='%1'").arg(recordId);
    QSqlQuery query(q);
    if (query.next()) {
        auto name = query.value(1).toString();
        auto surname = query.value(2).toString();
        auto fileName = query.value(3).toString();
        auto record = Record{recordId, name, surname, fileName, true};
        return record;
    }
    return Record();
}
