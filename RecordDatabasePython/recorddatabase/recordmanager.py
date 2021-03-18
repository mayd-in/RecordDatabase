import enum

from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import QTextDocument, QTextDocumentWriter
from PyQt5.QtSql import QSqlDatabase, QSqlQuery


class Record():
    def __init__(self, recordId, fileName, isSaved):

        self.recordId = recordId
        self.name = ""
        self.surname = ""
        self.fileName = fileName
        self.isSaved = isSaved


class RecordManager(QObject):
    currentRecordChanged = pyqtSignal()

    class ErrorCodes(enum.IntEnum):
        NoError = 0
        RecordExists = enum.auto()
        RecordNotExist = enum.auto()
        NoCurrentRecord = enum.auto()
        FileOpenFailed = enum.auto()
        FileSaveFailed = enum.auto()

    def __init__(self, textDocument: QTextDocument, parent):
        super().__init__(parent)

        self.textDocument = textDocument

        self.currentRecord = None

        # FILE DIRECTORY PATH
        writeDir = QDir(QStandardPaths.writableLocation(QStandardPaths.AppDataLocation))
        if not writeDir.mkpath('.'):
            qFatal(f"Failed to create writable directory at {writeDir.absolutePath()}")
        self.absolutePath = writeDir.absolutePath()

        self.setupDatabase()

    def setupDatabase(self):
        database = QSqlDatabase.database()
        if not database.isValid():
            database = QSqlDatabase.addDatabase("QSQLITE")
            if not database.isValid():
                qFatal(f"Cannot add database {database.lastError().text()}")

        databasePath = self.absolutePath + "/db.sqlite3"
        database.setDatabaseName(databasePath)
        if not database.open():
            qFatal(f"Cannot open database {database.lastError().text()}")

        if not database.tables().count("Records"):
            query = QSqlQuery()
            if not query.exec(
                """
                CREATE TABLE IF NOT EXISTS 'Records' (
                    'record_id' TEXT PRIMARY KEY,
                    'name' TEXT NOT NULL,
                    'surname' TEXT NOT NULL,
                    'filename' TEXT,
                    'created' DATETIME DEFAULT (datetime(CURRENT_TIMESTAMP, 'localtime')),
                    'last_modified' DATETIME DEFAULT (datetime(CURRENT_TIMESTAMP, 'localtime'))
                )
                """):
                qFatal(f"Failed to query database {database.lastError().text()}")

    def setCurrentRecord(self, record):
        if self.currentRecord == record:
            return

        self.currentRecord = record
        self.currentRecordChanged.emit()

    def create(self, recordId, name, surname):
        if self.findRecord(recordId):
            return RecordManager.ErrorCodes.RecordExists

        fileName = f'{recordId}.htm'
        record = Record(recordId, fileName, isSaved=False)
        record.name = name
        record.surname = surname

        # UPDATE DOCUMENT
        self.textDocument.setHtml(f"document for {recordId} {name} {surname}")
        self.textDocument.setModified(False)
        self.setCurrentRecord(record)
        return RecordManager.ErrorCodes.NoError

    def open(self, recordId):
        record = self.findRecord(recordId)
        if not record:
            return RecordManager.ErrorCodes.RecordNotExist

        # FILE READ
        file = QFile(f'{self.absolutePath}/{record.fileName}')
        if not file.open(QFile.ReadOnly):
            return RecordManager.ErrorCodes.FileOpenFailed

        data = file.readAll()
        codec = QTextCodec.codecForHtml(data)
        text = codec.toUnicode(data)

        # UPDATE DOCUMENT
        self.textDocument.setHtml(text)
        self.textDocument.setModified(False)
        self.setCurrentRecord(record)
        return RecordManager.ErrorCodes.NoError

    def save(self):
        record = self.currentRecord
        if not record:
            return RecordManager.ErrorCodes.NoCurrentRecord

        # FILE WRITE
        writer = QTextDocumentWriter(f'{self.absolutePath}/{record.fileName}')
        success = writer.write(self.textDocument)
        if not success:
            return RecordManager.ErrorCodes.FileSaveFailed

        # DATABASE WRITE
        query = QSqlQuery()
        if record.isSaved:
            q = f'UPDATE Records SET \
            last_modified = (datetime(CURRENT_TIMESTAMP, "localtime")) \
            WHERE record_id="{record.recordId}"'
            query.prepare(q)
        else:
            q = f'INSERT INTO Records (record_id, name, surname, filename) \
            VALUES ("{record.recordId}", :name, :surname, "{record.fileName}")'
            query.prepare(q)
            query.bindValue(":name", record.name)
            query.bindValue(":surname", record.surname)

        success = query.exec()
        if not success:
            return RecordManager.ErrorCodes.FileSaveFailed

        # UPDATE DOCUMENT
        self.textDocument.setModified(False)
        return RecordManager.ErrorCodes.NoError

    def findRecord(self, recordId):
        query = QSqlQuery(f'SELECT record_id, name, surname, filename FROM Records WHERE record_id="{recordId}"')
        if query.next():
            name = query.value(1)
            surname = query.value(2)
            fileName = query.value(3)
            record = Record(recordId, fileName, isSaved=True)
            record.name = name
            record.surname = surname
            return record
        else:
            return None
