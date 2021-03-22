#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTextEdit;

class RecordManager;
class NewRecordDialog;
class OpenRecordDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum Theme {
        Default,
        Light,
        Dark,
    };

public:
    MainWindow(QWidget *parent = nullptr);

signals:
    void themeChanged(Theme theme);

private:
    void setupMenus();

    void recordNew(QString recordId, QString name, QString surname);
    void recordOpen(QString recordId);
    bool recordSave();
    bool maybeSave();

    void closeEvent(QCloseEvent *event) override;

    void updateWindowProperties();
    void setTheme(Theme theme);

    QTextEdit* m_textEditor;
    RecordManager* m_recordManager;

    NewRecordDialog* m_newRecordDialog;
    OpenRecordDialog* m_openRecordDialog;
};
#endif // MAINWINDOW_H
