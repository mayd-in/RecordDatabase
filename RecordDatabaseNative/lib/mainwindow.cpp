#include "mainwindow.h"
#include "dialogs.h"
#include "recordmanager.h"

#include <QtCore>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_textEditor = new QTextEdit(this);
    setCentralWidget(m_textEditor);

    m_recordManager = new RecordManager(m_textEditor->document(), this);
    connect(m_recordManager, &RecordManager::currentRecordChanged, this, &MainWindow::updateWindowProperties);

    m_newRecordDialog = new NewRecordDialog(this);
    connect(m_newRecordDialog, &NewRecordDialog::recordCreated, this, &MainWindow::recordNew);

    setupMenus();

    updateWindowProperties();
}

void MainWindow::setupMenus()
{
    auto fileMenu = new QMenu(tr("&File"), this);
    auto helpMenu = new QMenu(tr("&Help"), this);
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(helpMenu);

    // FILE MENU
    auto newRecordAction = new QAction(tr("&New Record"), this);
    newRecordAction->setShortcut(QKeySequence(QKeySequence::New));
    connect(newRecordAction, &QAction::triggered, m_newRecordDialog, &NewRecordDialog::open);

    fileMenu->addAction(newRecordAction);

    // HELP MENU
    helpMenu->addAction(tr("&About"), this, [&]() {
        QMessageBox::about(this, QApplication::applicationDisplayName(),
                           tr("<p><b>Record Database Editor</b> allows storing "
                              "per contact information in files using a database.</p>"));
    });
    helpMenu->addAction(tr("About Qt"), qApp, &QApplication::aboutQt);
}

void MainWindow::recordNew(QString recordId, QString name, QString surname) const
{
    m_recordManager->create(recordId, name, surname);
    m_newRecordDialog->accept();
}

void MainWindow::updateWindowProperties()
{
    auto currentRecord = m_recordManager->currentRecord();
    if (currentRecord) {
        m_textEditor->setEnabled(true);
        setWindowTitle(QString("%1 %2[*] - %3")
                       .arg(currentRecord->name, currentRecord->surname, QApplication::applicationDisplayName()));
    }
    else {
        m_textEditor->setEnabled(false);
        setWindowTitle(QString("[*]%1").arg(QApplication::applicationDisplayName()));
    }
}
