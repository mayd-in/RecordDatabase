import enum

from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import QFontDatabase, QFontInfo, QKeySequence, QColor, QPalette

from .texteditor import TextEditor
from .recordmanager import RecordManager
from .dialogs import NewRecordDialog, OpenRecordDialog


class MainWindow(QMainWindow):
    Theme = enum.Enum('Theme', 'Default Light Dark', start=0)
    themeChanged = pyqtSignal()

    def __init__(self):
        super().__init__()

        self.translator = QTranslator(self)
        self.translatorQt = QTranslator(self)

        settings = QSettings()
        self.setLanguage(settings.value("language"))
        self.setTheme(settings.value("theme", MainWindow.Theme.Light, type=int))

        QApplication.setApplicationDisplayName(QApplication.translate("app", "Record Database Editor"))

        textEditor = TextEditor(self)
        textEditor.document().modificationChanged.connect(self.setWindowModified)
        self.setCentralWidget(textEditor)

        recordManager = RecordManager(textEditor.document(), self)
        recordManager.currentRecordChanged.connect(self.updateWindowProperties)

        dialogNewRecord = NewRecordDialog(self)
        dialogNewRecord.recordCreated.connect(self.recordNew)

        dialogOpenRecord = OpenRecordDialog(recordManager.recordModel, self)
        dialogOpenRecord.recordSelected.connect(self.recordOpen)

        # PROPERTIES
        self.textEditor = textEditor
        self.recordManager = recordManager
        self.dialogNewRecord = dialogNewRecord
        self.dialogOpenRecord = dialogOpenRecord

        # SETUP
        self.setupMenus()
        self.setupToolbars()

        self.updateWindowProperties()

    def setupMenus(self):
        fileMenu = QMenu(self.tr("&File"), self)
        helpMenu = QMenu(self.tr("&Help"), self)
        self.menuBar().addMenu(fileMenu)
        self.menuBar().addMenu(helpMenu)

        # FILE MENU
        # Language
        languageMenu = QMenu(self.tr("&Language"), self)

        def changeLanguage(language):
            settings = QSettings()
            settings.setValue("language", QLocale(language))
            QMessageBox.information(self, QApplication.applicationDisplayName(),
                self.tr("The language change will take effect after restart."))

        languages = QDir('translations/qm').entryList(['*.qm'])
        for language in languages:
            if language.startswith('qt'):
                continue
            language = language[:language.rfind('.')]  # en.qm -> en
            locale = QLocale(language)
            languageName = locale.nativeLanguageName() if language != 'en' else "English"
            action = languageMenu.addAction(languageName, lambda language=language: changeLanguage(language))
            action.setCheckable(True)
            action.setChecked(locale == QLocale())

        # Theme
        actionThemeLight = QAction(self.tr("Light"), self)
        actionThemeLight.setCheckable(True)
        actionThemeLight.triggered.connect(lambda: self.setTheme(MainWindow.Theme.Light))
        self.themeChanged.connect(lambda: actionThemeLight.setChecked(self.theme == MainWindow.Theme.Light))

        actionThemeDark = QAction(self.tr("Dark"), self)
        actionThemeDark.setCheckable(True)
        actionThemeDark.triggered.connect(lambda: self.setTheme(MainWindow.Theme.Dark))
        self.themeChanged.connect(lambda: actionThemeDark.setChecked(self.theme == MainWindow.Theme.Dark))

        self.themeChanged.emit()

        themeGroup = QActionGroup(self)
        themeGroup.addAction(actionThemeLight)
        themeGroup.addAction(actionThemeDark)

        themeMenu = QMenu(self.tr("&Theme"), self)
        themeMenu.addActions(themeGroup.actions())

        # Records
        newRecordAction = QAction(self.tr("&New Record"), self)
        newRecordAction.setShortcut(QKeySequence(QKeySequence.New))
        newRecordAction.triggered.connect(self.dialogNewRecord.open)

        openRecordAction = QAction(self.tr("&Open Record"), self)
        openRecordAction.setShortcut(QKeySequence(QKeySequence.Open))
        openRecordAction.triggered.connect(self.dialogOpenRecord.open)

        saveRecordAction = QAction(self.tr("&Save Record"), self)
        saveRecordAction.setShortcut(QKeySequence(QKeySequence.Save))
        saveRecordAction.triggered.connect(self.recordSave)

        fileMenu.addMenu(languageMenu)
        fileMenu.addMenu(themeMenu)
        fileMenu.addSeparator()
        fileMenu.addAction(newRecordAction)
        fileMenu.addAction(openRecordAction)
        fileMenu.addAction(saveRecordAction)

        # HELP MENU
        helpMenu.addAction(self.tr("&Help"), lambda: QMessageBox.about(self, QApplication.applicationDisplayName(),
                    self.tr("<p><b>Record Database Editor</b> allows storing "
                       "per contact information in files using a database.</p>")))
        helpMenu.addAction(self.tr("About Qt"), QApplication.aboutQt)

    def setupToolbars(self):
        # FONT
        toolbar = self.addToolBar(self.tr("Font"))
        toolbar.setAllowedAreas(Qt.ToolBarArea.TopToolBarArea | Qt.ToolBarArea.BottomToolBarArea)

        comboFontFamily = QFontComboBox()
        comboFontFamily.textActivated.connect(self.textEditor.setFontFamily)

        comboFontSize = QComboBox()
        comboFontSize.setEditable(True)
        sizes = QFontDatabase.standardSizes()
        for size in sizes:
            comboFontSize.addItem(str(size))
        comboFontSize.setCurrentIndex(sizes.index(QApplication.font().pointSize()))
        comboFontSize.textActivated.connect(self.textEditor.setFontSize)

        def onChanged(format):
            font = format.font()
            comboFontFamily.setCurrentIndex(comboFontFamily.findText(QFontInfo(font).family()))
            comboFontSize.setCurrentIndex(comboFontSize.findText(str(font.pointSize())))
        self.textEditor.currentCharFormatChanged.connect(onChanged)

        toolbar.addWidget(comboFontFamily)
        toolbar.addWidget(comboFontSize)
        toolbar.addAction(self.textEditor.actionFontSizeIncrease)
        toolbar.addAction(self.textEditor.actionFontSizeDecrease)

        # FORMAT
        toolbar = self.addToolBar(self.tr("Format"))
        toolbar.addAction(self.textEditor.actionTextBold)
        toolbar.addAction(self.textEditor.actionTextItalic)
        toolbar.addAction(self.textEditor.actionTextUnderline)

        toolbar.addSeparator()
        toolbar.addAction(self.textEditor.actionFontColor)

        toolbar.addSeparator()
        alignGroup = QActionGroup(self)
        if (QApplication.isLeftToRight()):
            alignGroup.addAction(self.textEditor.actionAlignLeft)
            alignGroup.addAction(self.textEditor.actionAlignCenter)
            alignGroup.addAction(self.textEditor.actionAlignRight)
        else:
            alignGroup.addAction(self.textEditor.actionAlignRight)
            alignGroup.addAction(self.textEditor.actionAlignCenter)
            alignGroup.addAction(self.textEditor.actionAlignLeft)
        alignGroup.addAction(self.textEditor.actionAlignJustify)
        toolbar.addActions(alignGroup.actions())

        toolbar.addSeparator()
        toolbar.addAction(self.textEditor.actionIndentMore)
        toolbar.addAction(self.textEditor.actionIndentLess)

    def recordNew(self, recordId, name, surname):
        if not self.maybeSave():
            return

        error = self.recordManager.create(recordId, name, surname)
        if not error:
            self.dialogNewRecord.accept()
        elif error == RecordManager.ErrorCodes.RecordExists:
            QMessageBox.warning(self, QApplication.applicationDisplayName(),
                self.tr("Record exists already"))
        else:
            QMessageBox.critical(self, QApplication.applicationDisplayName(),
                self.tr("Unknown error occurred"))

    def recordOpen(self, recordId):
        if not self.maybeSave():
            return

        error = self.recordManager.open(recordId)
        if not error:
            return
        elif error == RecordManager.ErrorCodes.RecordNotExist:
            QMessageBox.warning(self, QApplication.applicationDisplayName(),
                self.tr("Record not found"))
        elif error == RecordManager.ErrorCodes.FileOpenFailed:
            QMessageBox.warning(self, QApplication.applicationDisplayName(),
                self.tr("Unable to open file"))
        else:
            QMessageBox.critical(self, QApplication.applicationDisplayName(),
                self.tr("Unknown error occurred"))

    def recordSave(self):
        error = self.recordManager.save()
        if not error:
            return True
        elif error == RecordManager.ErrorCodes.NoCurrentRecord:
            return True
        elif error == RecordManager.ErrorCodes.FileSaveFailed:
            QMessageBox.warning(self, QApplication.applicationDisplayName(),
                self.tr("Unable to save file"))
        else:
            QMessageBox.critical(self, QApplication.applicationDisplayName(),
                self.tr("Unknown error occurred"))
        return False

    def maybeSave(self):
        if not self.textEditor.document().isModified():
            return True

        ret = QMessageBox.warning(self, QApplication.applicationDisplayName(),
            self.tr("There are unsaved changes.\nDo you want to save your changes?"),
            QMessageBox.Save | QMessageBox.Discard | QMessageBox.Cancel
        )

        if ret == QMessageBox.Save:
            return self.recordSave()
        elif ret == QMessageBox.Cancel:
            return False
        return True

    def closeEvent(self, event):
        if self.maybeSave():
            event.accept()
        else:
            event.ignore()

    def updateWindowProperties(self):
        currentRecord = self.recordManager.currentRecord
        if currentRecord:
            self.textEditor.setEnabled(True)
            self.setWindowTitle(f"{currentRecord.name} {currentRecord.surname}[*] - " + QApplication.applicationDisplayName())
        else:
            self.textEditor.setEnabled(False)
            self.setWindowTitle("[*]" + QApplication.applicationDisplayName())

    def setLanguage(self, locale: QLocale):
        QLocale.setDefault(locale)
        language = locale.name()  # en_US
        language = language[:language.rfind('_')]  # en

        QApplication.removeTranslator(self.translator)
        QApplication.removeTranslator(self.translatorQt)

        self.translator.load(f"translations/qm/{language}.qm")
        self.translatorQt.load(f"translations/qm/qtbase_{language}.qm")
        QApplication.installTranslator(self.translator)
        QApplication.installTranslator(self.translatorQt)

    def setTheme(self, theme):
        theme = MainWindow.Theme(theme)

        settings = QSettings()

        if theme == MainWindow.Theme.Light or theme == MainWindow.Theme.Default:
            defaultPalette = QPalette()
            self.setPalette(defaultPalette)  # Editor palette
            QApplication.setPalette(defaultPalette)

            self.theme = theme
            self.themeChanged.emit()
            settings.setValue("theme", self.theme.value)

        elif theme == MainWindow.Theme.Dark:
            windowColor = QColor(53,53,53)

            # Editor color palette
            editorPalette = self.palette()
            editorPalette.setColor(QPalette.Base, Qt.lightGray)
            editorPalette.setColor(QPalette.Text, Qt.black)  # Otherwise editor text becomes white
            self.setPalette(editorPalette)

            # Application color palette
            palette = QApplication.palette()
            palette.setColor(QPalette.Window, windowColor)
            palette.setColor(QPalette.WindowText, Qt.white)
            palette.setColor(QPalette.Base, windowColor.darker(150))
            palette.setColor(QPalette.AlternateBase, windowColor)
            palette.setColor(QPalette.ToolTipBase, windowColor)
            palette.setColor(QPalette.ToolTipText, Qt.white)
            palette.setColor(QPalette.Text, Qt.white)
            palette.setColor(QPalette.Button, windowColor)
            palette.setColor(QPalette.ButtonText, Qt.white)
            palette.setColor(QPalette.BrightText, Qt.red)
            palette.setColor(QPalette.HighlightedText, Qt.black)
            palette.setColor(QPalette.Disabled, QPalette.Text, Qt.darkGray)
            palette.setColor(QPalette.Disabled, QPalette.ButtonText, Qt.darkGray)
            QApplication.setPalette(palette)

            self.theme = theme
            self.themeChanged.emit()
            settings.setValue("theme", self.theme.value)
