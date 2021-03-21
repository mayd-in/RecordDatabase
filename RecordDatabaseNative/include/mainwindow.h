#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class TextEditor;
class RecordManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
};
#endif // MAINWINDOW_H
