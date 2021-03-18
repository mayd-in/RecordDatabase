from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import QTextDocument


class Record():
    def __init__(self, recordId):

        self.recordId = recordId
        self.name = ""
        self.surname = ""


class RecordManager(QObject):
    currentRecordChanged = pyqtSignal()

    def __init__(self, textDocument: QTextDocument, parent):
        super().__init__(parent)

        self.textDocument = textDocument

        self.currentRecord = None

    def setCurrentRecord(self, record):
        if self.currentRecord == record:
            return

        self.currentRecord = record
        self.currentRecordChanged.emit()

    def create(self, recordId, name, surname):
        record = Record(recordId)
        record.name = name
        record.surname = surname

        self.textDocument.setHtml(f"document for {recordId} {name} {surname}")
        self.textDocument.setModified(False)
        self.setCurrentRecord(record)
