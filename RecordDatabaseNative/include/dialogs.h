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
    void recordCreated(QString, QString, QString);

private:
    QLineEdit* m_idLineEdit;
    QLineEdit* m_nameLineEdit;
    QLineEdit* m_surnameLineEdit;
};

#endif // DIALOGS_H
