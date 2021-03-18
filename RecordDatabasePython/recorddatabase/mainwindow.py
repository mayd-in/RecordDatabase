from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import QKeySequence

from .recordmanager import RecordManager
from .dialogs import NewRecordDialog, OpenRecordDialog


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        textEditor = QTextEdit()
        self.setCentralWidget(textEditor)

        recordManager = RecordManager(textEditor.document(), self)
        recordManager.currentRecordChanged.connect(self.updateWindowProperties)

        dialogNewRecord = NewRecordDialog(self)
        dialogNewRecord.recordCreated.connect(self.recordNew)

        dialogOpenRecord = OpenRecordDialog(self)
        dialogOpenRecord.recordSelected.connect(self.recordOpen)

        # PROPERTIES
        self.textEditor = textEditor
        self.recordManager = recordManager
        self.dialogNewRecord = dialogNewRecord
        self.dialogOpenRecord = dialogOpenRecord

        # SETUP
        self.setupMenus()

        self.updateWindowProperties()

    def setupMenus(self):
        fileMenu = QMenu(self.tr("&File"), self)
        helpMenu = QMenu(self.tr("&Help"), self)
        self.menuBar().addMenu(fileMenu)
        self.menuBar().addMenu(helpMenu)

        # FILE MENU
        newRecordAction = QAction(self.tr("&New Record"), self)
        newRecordAction.setShortcut(QKeySequence(QKeySequence.New))
        newRecordAction.triggered.connect(self.dialogNewRecord.open)

        openRecordAction = QAction(self.tr("&Open Record"), self)
        openRecordAction.setShortcut(QKeySequence(QKeySequence.Open))
        openRecordAction.triggered.connect(self.dialogOpenRecord.open)

        saveRecordAction = QAction(self.tr("&Save Record"), self)
        saveRecordAction.setShortcut(QKeySequence(QKeySequence.Save))
        saveRecordAction.triggered.connect(self.recordSave)

        fileMenu.addAction(newRecordAction)
        fileMenu.addAction(openRecordAction)
        fileMenu.addAction(saveRecordAction)

        # HELP MENU
        helpMenu.addAction(self.tr("&Help"), lambda: QMessageBox.about(self, QApplication.applicationDisplayName(),
                    self.tr("<p><b>Record Database Editor</b> allows storing "
                       "per contact information in files using a database.</p>")))
        helpMenu.addAction(self.tr("About Qt"), QApplication.aboutQt)

    def recordNew(self, recordId, name, surname):
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

    def updateWindowProperties(self):
        currentRecord = self.recordManager.currentRecord
        if currentRecord:
            self.textEditor.setEnabled(True)
            self.setWindowTitle(f"{currentRecord.name} {currentRecord.surname}[*] - " + QApplication.applicationDisplayName())
        else:
            self.textEditor.setEnabled(False)
            self.setWindowTitle("[*]" + QApplication.applicationDisplayName())
