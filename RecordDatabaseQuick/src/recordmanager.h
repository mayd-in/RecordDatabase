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
    QString fileName;
    bool isSaved;

    bool operator==(const Record& rhs) const {
        return recordId == rhs.recordId;
    }
    operator bool() const {
        return recordId != "";
    }
};

Q_DECLARE_METATYPE(Record)

class RecordManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QQuickTextDocument* document READ document WRITE setDocument NOTIFY documentChanged)
    Q_PROPERTY(Record currentRecord READ currentRecord NOTIFY currentRecordChanged)

public:
    enum Error {
            NoError,
            RecordExists,
            RecordNotExist,
            NoCurrentRecord,
            FileOpenFailed,
            FileSaveFailed,
        };
    Q_ENUM(Error)

    explicit RecordManager(QObject *parent = nullptr);

    QQuickTextDocument* document() const;
    void setDocument(QQuickTextDocument* document);

    Record currentRecord() const;
    void setCurrentRecord(Record record);

public slots:
    Error create(QString recordId, QString name, QString surname);
    Error open(QString recordId);
    Error save();

signals:
    void documentChanged();
    void currentRecordChanged();

private:
    void setupDatabase();
    Record findRecord(const QString& recordId) const;

    QQuickTextDocument* m_document;
    QTextDocument* m_textDocument;
    Record m_currentRecord;

    QString m_absolutePath;
};

#endif // RECORDMANAGER_H
