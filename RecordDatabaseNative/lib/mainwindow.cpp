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
    connect(m_textEditor->document(), &QTextDocument::modificationChanged, this, &QMainWindow::setWindowModified);

    m_recordManager = new RecordManager(m_textEditor->document(), this);
    connect(m_recordManager, &RecordManager::currentRecordChanged, this, &MainWindow::updateWindowProperties);

    m_newRecordDialog = new NewRecordDialog(this);
    connect(m_newRecordDialog, &NewRecordDialog::recordCreated, this, &MainWindow::recordNew);

    m_openRecordDialog = new OpenRecordDialog(this);
    connect(m_openRecordDialog, &OpenRecordDialog::recordSelected, this, &MainWindow::recordOpen);

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

    auto openRecordAction = new QAction(tr("&Open Record"), this);
    openRecordAction->setShortcut(QKeySequence(QKeySequence::Open));
    connect(openRecordAction, &QAction::triggered, m_openRecordDialog, &OpenRecordDialog::open);

    auto saveRecordAction = new QAction(tr("&Save Record"), this);
    saveRecordAction->setShortcut(QKeySequence(QKeySequence::Save));
    connect(saveRecordAction, &QAction::triggered, this, &MainWindow::recordSave);

    fileMenu->addAction(newRecordAction);
    fileMenu->addAction(openRecordAction);
    fileMenu->addAction(saveRecordAction);

    // HELP MENU
    helpMenu->addAction(tr("&About"), this, [&]() {
        QMessageBox::about(this, QApplication::applicationDisplayName(),
                           tr("<p><b>Record Database Editor</b> allows storing "
                              "per contact information in files using a database.</p>"));
    });
    helpMenu->addAction(tr("About Qt"), qApp, &QApplication::aboutQt);
}

void MainWindow::recordNew(QString recordId, QString name, QString surname)
{
    if (!maybeSave())
        return;

    auto error = m_recordManager->create(recordId, name, surname);

    switch (error) {
    case RecordManager::NoError:
        m_newRecordDialog->accept();
        break;
    case RecordManager::RecordExists:
        QMessageBox::warning(this, QApplication::applicationDisplayName(), tr("Record exists already"));
        break;
    default:
        QMessageBox::critical(this, QApplication::applicationDisplayName(), tr("Unknown error occurred"));
    }
}

void MainWindow::recordOpen(QString recordId)
{
    if (!maybeSave())
        return;

    auto error = m_recordManager->open(recordId);

    switch (error) {
    case RecordManager::NoError:
        break;
    case RecordManager::RecordNotExist:
        QMessageBox::warning(this, QApplication::applicationDisplayName(), tr("Record not found"));
        break;
    case RecordManager::FileOpenFailed:
        QMessageBox::warning(this, QApplication::applicationDisplayName(), tr("Unable to open file"));
        break;
    default:
        QMessageBox::critical(this, QApplication::applicationDisplayName(), tr("Unknown error occurred"));
    }
}

bool MainWindow::recordSave()
{
    auto error = m_recordManager->save();
    switch (error) {
    case RecordManager::NoError:
        return true;
    case RecordManager::NoCurrentRecord:
        return true;
    case RecordManager::FileSaveFailed:
        QMessageBox::warning(this, QApplication::applicationDisplayName(), tr("Unable to save file"));
        break;
    default:
        QMessageBox::critical(this, QApplication::applicationDisplayName(), tr("Unknown error occurred"));
    }
    return false;
}

bool MainWindow::maybeSave()
{
    if(!m_textEditor->document()->isModified())
        return true;

    auto ret = QMessageBox::warning(this, QApplication::applicationDisplayName(),
                             tr("There are unsaved changes.\n"
                                "Do you want to save your changes?"),
                             QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (ret == QMessageBox::Save)
        return recordSave();
    else if (ret == QMessageBox::Cancel)
        return false;
    return true;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (maybeSave())
        event->accept();
    else
        event->ignore();
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
