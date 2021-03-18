from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import QKeySequence

from .recordmanager import RecordManager
from .dialogs import NewRecordDialog


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        textEditor = QTextEdit()
        self.setCentralWidget(textEditor)

        recordManager = RecordManager(textEditor.document(), self)
        recordManager.currentRecordChanged.connect(self.updateWindowProperties)

        dialogNewRecord = NewRecordDialog(self)
        dialogNewRecord.recordCreated.connect(self.recordNew)

        # PROPERTIES
        self.textEditor = textEditor
        self.recordManager = recordManager
        self.dialogNewRecord = dialogNewRecord

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

        fileMenu.addAction(newRecordAction)

        # HELP MENU
        helpMenu.addAction(self.tr("&Help"), lambda: QMessageBox.about(self, QApplication.applicationDisplayName(),
                    self.tr("<p><b>Record Database Editor</b> allows storing "
                       "per contact information in files using a database.</p>")))
        helpMenu.addAction(self.tr("About Qt"), QApplication.aboutQt)

    def recordNew(self, recordId, name, surname):
        self.recordManager.create(recordId, name, surname)
        self.dialogNewRecord.accept()

    def updateWindowProperties(self):
        currentRecord = self.recordManager.currentRecord
        if currentRecord:
            self.textEditor.setEnabled(True)
            self.setWindowTitle(f"{currentRecord.name} {currentRecord.surname}[*] - " + QApplication.applicationDisplayName())
        else:
            self.textEditor.setEnabled(False)
            self.setWindowTitle("[*]" + QApplication.applicationDisplayName())
