#ifndef RECORDMANAGER_H
#define RECORDMANAGER_H

#include <QObject>

class QTextDocument;

struct Record
{
    QString recordId;
    QString name;
    QString surname;
};

class RecordManager : public QObject
{
    Q_OBJECT

public:
    explicit RecordManager(QTextDocument* textDocument, QObject *parent = nullptr);

    Record* currentRecord();

    void create(QString recordId, QString name, QString surname);

signals:
    void currentRecordChanged();

private:
    void setCurrentRecord(Record* record);

    QTextDocument* m_textDocument;
    Record* m_currentRecord;
};

#endif // RECORDMANAGER_H
