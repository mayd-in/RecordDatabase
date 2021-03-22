#ifndef RECORDMANAGER_H
#define RECORDMANAGER_H

#include <QObject>
#include <QSharedPointer>

class QTextDocument;

struct Record
{
    QString recordId;
    QString name;
    QString surname;
    QString fileName;
    bool isSaved;
};

class RecordManager : public QObject
{
    Q_OBJECT

public:
    enum Error {
        NoError,
        RecordExists,
        RecordNotExist,
        NoCurrentRecord,
        FileOpenFailed,
        FileSaveFailed,
    };

    explicit RecordManager(QTextDocument* textDocument, QObject *parent = nullptr);

    QSharedPointer<Record> currentRecord() const;

    Error create(QString recordId, QString name, QString surname);
    Error open(QString recordId);
    Error save();

signals:
    void currentRecordChanged();

private:
    void setupDatabase();
    void setCurrentRecord(QSharedPointer<Record> record);

    QSharedPointer<Record> findRecord(const QString& recordId) const;

    QTextDocument* m_textDocument;
    QSharedPointer<Record> m_currentRecord;
    QString m_absolutePath;
};

#endif // RECORDMANAGER_H
