#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>

class TextEditor;

class RecordManager;
class NewRecordDialog;
class OpenRecordDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum Theme {
        Default,
        Light,
        Dark,
    };
    Q_ENUM(Theme)

    MainWindow(QWidget *parent = nullptr);

signals:
    void themeChanged(Theme theme);

private:
    void setupMenus();
    void setupToolBars();

    void recordNew(QString recordId, QString name, QString surname);
    void recordOpen(QString recordId);
    bool recordSave();
    bool maybeSave();

    void closeEvent(QCloseEvent *event) override;

    void updateWindowProperties();
    void setLanguage(QLocale locale);
    void setTheme(Theme theme);

    TextEditor* m_textEditor;
    RecordManager* m_recordManager;

    NewRecordDialog* m_newRecordDialog;
    OpenRecordDialog* m_openRecordDialog;

    Theme m_theme;

    QTranslator m_translator;
    QTranslator m_translatorQt;
};
#endif // MAINWINDOW_H
