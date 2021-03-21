#include "mainwindow.h"

#include <QtCore>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_textEditor = new QTextEdit(this);
    setCentralWidget(m_textEditor);

    setupMenus();
}

void MainWindow::setupMenus()
{
    // HELP MENU
    auto helpMenu = new QMenu(tr("&Help"), this);
    menuBar()->addMenu(helpMenu);

    // CONNECTIONS
    helpMenu->addAction(tr("&About"), this, [&]() {
        QMessageBox::about(this, QApplication::applicationDisplayName(),
                           tr("<p><b>Record Database Editor</b> allows storing "
                              "per contact information in files using a database.</p>"));
    });
    helpMenu->addAction(tr("About Qt"), qApp, &QApplication::aboutQt);
}
