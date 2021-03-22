#ifndef DIALOGS_H
#define DIALOGS_H

#include <QDialog>

class QLineEdit;

class NewRecordDialog : public QDialog
{
    Q_OBJECT

public:
    NewRecordDialog(QWidget *parent = nullptr);

    void open() override;

    void beforeAccept();

signals:
    void recordCreated(QString recordId, QString name, QString surname);

private:
    QLineEdit* m_idLineEdit;
    QLineEdit* m_nameLineEdit;
    QLineEdit* m_surnameLineEdit;
};

class OpenRecordDialog : public QDialog
{
    Q_OBJECT

public:
    OpenRecordDialog(QWidget *parent = nullptr);

    void open() override;

signals:
    void recordSelected(QString recordId);

private:
    QLineEdit* m_idLineEdit;
};

#endif // DIALOGS_H
