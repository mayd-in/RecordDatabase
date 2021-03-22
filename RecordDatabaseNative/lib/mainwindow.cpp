#include "mainwindow.h"
#include "dialogs.h"
#include "recordmanager.h"
#include "texteditor.h"

#include <QtCore>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_textEditor = new TextEditor(this);
    setCentralWidget(m_textEditor);
    connect(m_textEditor->document(), &QTextDocument::modificationChanged, this, &QMainWindow::setWindowModified);

    m_recordManager = new RecordManager(m_textEditor->document(), this);
    connect(m_recordManager, &RecordManager::currentRecordChanged, this, &MainWindow::updateWindowProperties);

    m_newRecordDialog = new NewRecordDialog(this);
    connect(m_newRecordDialog, &NewRecordDialog::recordCreated, this, &MainWindow::recordNew);

    m_openRecordDialog = new OpenRecordDialog(this);
    connect(m_openRecordDialog, &OpenRecordDialog::recordSelected, this, &MainWindow::recordOpen);

    setupMenus();
    setupToolBars();

    updateWindowProperties();
    setTheme(Theme::Dark);
}

void MainWindow::setupMenus()
{
    auto fileMenu = new QMenu(tr("&File"), this);
    auto helpMenu = new QMenu(tr("&Help"), this);
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(helpMenu);

    // FILE MENU
    // Theme
    auto lightThemeAction = new QAction(tr("Light"), this);
    lightThemeAction->setCheckable(true);
    connect(lightThemeAction, &QAction::triggered, this, [this](){setTheme(Theme::Light);});

    auto darkThemeAction = new QAction(tr("Dark"), this);
    darkThemeAction->setCheckable(true);
    connect(darkThemeAction, &QAction::triggered, this, [this](){setTheme(Theme::Dark);});

    connect(this, &MainWindow::themeChanged, lightThemeAction, [lightThemeAction, darkThemeAction](Theme theme){
        lightThemeAction->setChecked(theme == Theme::Light);
        darkThemeAction->setChecked(theme == Theme::Dark);
    });

    auto themeMenu = new QMenu(tr("&Theme"), this);
    themeMenu->addAction(lightThemeAction);
    themeMenu->addAction(darkThemeAction);

    // Records
    auto newRecordAction = new QAction(tr("&New Record"), this);
    newRecordAction->setShortcut(QKeySequence(QKeySequence::New));
    connect(newRecordAction, &QAction::triggered, m_newRecordDialog, &NewRecordDialog::open);

    auto openRecordAction = new QAction(tr("&Open Record"), this);
    openRecordAction->setShortcut(QKeySequence(QKeySequence::Open));
    connect(openRecordAction, &QAction::triggered, m_openRecordDialog, &OpenRecordDialog::open);

    auto saveRecordAction = new QAction(tr("&Save Record"), this);
    saveRecordAction->setShortcut(QKeySequence(QKeySequence::Save));
    connect(saveRecordAction, &QAction::triggered, this, &MainWindow::recordSave);

    fileMenu->addMenu(themeMenu);
    fileMenu->addSeparator();
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

void MainWindow::setupToolBars()
{
    // FONT
    {
        auto toolBar = addToolBar(tr("Font"));
        toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);

        auto comboFontFamily = new QFontComboBox();
        connect(comboFontFamily, &QFontComboBox::textActivated, m_textEditor, &TextEditor::setFontFamily);

        auto comboFontSize = new QComboBox();
        comboFontSize->setEditable(true);
        auto sizes = QFontDatabase::standardSizes();
        for (auto size : sizes) {
            comboFontSize->addItem(QString::number(size));
        }
        comboFontSize->setCurrentIndex(sizes.indexOf(QApplication::font().pointSize()));
        connect(comboFontSize, &QFontComboBox::textActivated, m_textEditor, [this](QString text) {
            m_textEditor->setFontSize(text.toInt());
        });

        connect(m_textEditor, &QTextEdit::currentCharFormatChanged, this,
            [comboFontFamily, comboFontSize](QTextCharFormat format) {
                auto font = format.font();
                comboFontFamily->setCurrentIndex(comboFontFamily->findText(QFontInfo(font).family()));
                comboFontSize->setCurrentIndex(comboFontSize->findText(QString::number(font.pointSize())));
        });

        toolBar->addWidget(comboFontFamily);
        toolBar->addWidget(comboFontSize);
        toolBar->addAction(m_textEditor->actionFontSizeIncrease);
        toolBar->addAction(m_textEditor->actionFontSizeDecrease);
    }

    // FORMAT
    {
        auto toolBar = addToolBar(tr("Format"));
        toolBar->addAction(m_textEditor->actionTextBold);
        toolBar->addAction(m_textEditor->actionTextItalic);
        toolBar->addAction(m_textEditor->actionTextUnderline);

        toolBar->addSeparator();
        toolBar->addAction(m_textEditor->actionFontColor);

        toolBar->addSeparator();
        auto *alignGroup = new QActionGroup(this);
        if (QApplication::isLeftToRight()) {
            alignGroup->addAction(m_textEditor->actionAlignLeft);
            alignGroup->addAction(m_textEditor->actionAlignCenter);
            alignGroup->addAction(m_textEditor->actionAlignRight);
        }
        else {
            alignGroup->addAction(m_textEditor->actionAlignRight);
            alignGroup->addAction(m_textEditor->actionAlignCenter);
            alignGroup->addAction(m_textEditor->actionAlignLeft);
        }
        alignGroup->addAction(m_textEditor->actionAlignJustify);
        toolBar->addActions(alignGroup->actions());

        toolBar->addSeparator();
        toolBar->addAction(m_textEditor->actionIndentMore);
        toolBar->addAction(m_textEditor->actionIndentLess);
    }
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

void MainWindow::setTheme(MainWindow::Theme theme)
{
    if (theme == Theme::Default || theme == Theme::Light) {
        QPalette defaultPalette;
        setPalette(defaultPalette);  // Editor palette
        QApplication::setPalette(defaultPalette);  // Application palette

        themeChanged(Theme::Light);
    }
    else if (theme == Theme::Dark) {
        QColor windowColor(53,53,53);

        // Editor color palette
        QPalette editorPalette = this->palette();
        editorPalette.setColor(QPalette::Base, Qt::lightGray);
        editorPalette.setColor(QPalette::Text, Qt::black);  // Otherwise editor text becomes white
        setPalette(editorPalette);

        // Application color palette
        QPalette palette = QApplication::palette();
        palette.setColor(QPalette::Window, windowColor);
        palette.setColor(QPalette::WindowText, Qt::white);
        palette.setColor(QPalette::Base, windowColor.darker(150));
        palette.setColor(QPalette::AlternateBase, windowColor);
        palette.setColor(QPalette::ToolTipBase, windowColor);
        palette.setColor(QPalette::ToolTipText, Qt::white);
        palette.setColor(QPalette::Text, Qt::white);
        palette.setColor(QPalette::Button, windowColor);
        palette.setColor(QPalette::ButtonText, Qt::white);
        palette.setColor(QPalette::BrightText, Qt::red);
        palette.setColor(QPalette::HighlightedText, Qt::black);
        palette.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);
        palette.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);
        QApplication::setPalette(palette);

        themeChanged(Theme::Dark);
    }
}
