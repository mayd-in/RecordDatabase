#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTextEdit;

class RecordManager;
class NewRecordDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    void setupMenus();

    void recordNew(QString recordId, QString name, QString surname) const;

    void updateWindowProperties();

    QTextEdit* m_textEditor;
    RecordManager* m_recordManager;
    NewRecordDialog* m_newRecordDialog;
};
#endif // MAINWINDOW_H
