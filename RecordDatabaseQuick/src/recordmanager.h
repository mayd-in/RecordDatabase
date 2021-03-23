#ifndef RECORDMANAGER_H
#define RECORDMANAGER_H

#include <QObject>

class QQuickTextDocument;
class QTextDocument;

struct Record
{
    Q_GADGET

    Q_PROPERTY(QString recordId MEMBER recordId)
    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(QString surname MEMBER surname)

public:
    QString recordId;
    QString name;
    QString surname;

    bool operator==(const Record& rhs) const {
        return recordId == rhs.recordId;
    }
};

Q_DECLARE_METATYPE(Record)

class RecordManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QQuickTextDocument* document READ document WRITE setDocument NOTIFY documentChanged)
    Q_PROPERTY(Record currentRecord READ currentRecord NOTIFY currentRecordChanged)

public:
    explicit RecordManager(QObject *parent = nullptr);

    QQuickTextDocument* document() const;
    void setDocument(QQuickTextDocument* document);

    Record currentRecord() const;
    void setCurrentRecord(Record record);

public slots:
    bool create(QString recordId, QString name, QString surname);

signals:
    void documentChanged();
    void currentRecordChanged();

private:
    QQuickTextDocument* m_document;
    QTextDocument* m_textDocument;
    Record m_currentRecord;
};

#endif // RECORDMANAGER_H
