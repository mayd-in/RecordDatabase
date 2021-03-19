import enum

from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import QKeySequence, QColor, QPalette

from .recordmanager import RecordManager
from .dialogs import NewRecordDialog, OpenRecordDialog


class MainWindow(QMainWindow):
    Theme = enum.Enum('Theme', 'Light Dark')
    themeChanged = pyqtSignal(Theme)

    def __init__(self):
        super().__init__()

        textEditor = QTextEdit()
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

        self.updateWindowProperties()
        self.setTheme(MainWindow.Theme.Dark)

    def setupMenus(self):
        fileMenu = QMenu(self.tr("&File"), self)
        helpMenu = QMenu(self.tr("&Help"), self)
        self.menuBar().addMenu(fileMenu)
        self.menuBar().addMenu(helpMenu)

        # FILE MENU
        # Theme
        actionThemeLight = QAction(self.tr("Light"), self)
        actionThemeLight.setCheckable(True)
        actionThemeLight.triggered.connect(lambda: self.setTheme(MainWindow.Theme.Light))
        self.themeChanged.connect(lambda theme: actionThemeLight.setChecked(theme == MainWindow.Theme.Light))

        actionThemeDark = QAction(self.tr("Dark"), self)
        actionThemeDark.setCheckable(True)
        actionThemeDark.triggered.connect(lambda: self.setTheme(MainWindow.Theme.Dark))
        self.themeChanged.connect(lambda theme: actionThemeDark.setChecked(theme == MainWindow.Theme.Dark))

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

    def setTheme(self, theme):
        if theme == MainWindow.Theme.Light:
            defaultPalette = QPalette()
            self.setPalette(defaultPalette)  # Editor palette
            QApplication.setPalette(defaultPalette)
            self.themeChanged.emit(MainWindow.Theme.Light)

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
            self.themeChanged.emit(MainWindow.Theme.Dark)
