from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import QRegExpValidator


validatorId = QRegExpValidator(QRegExp("^\\d{6,}$"))


class NewRecordDialog(QDialog):
    recordCreated = pyqtSignal(str, str, str)

    def __init__(self, parent):
        super().__init__(parent)

        idLabel = QLabel(self.tr("Record ID:"))
        idLineEdit = QLineEdit()
        idLineEdit.setValidator(validatorId)

        nameLabel = QLabel(self.tr("Name:"))
        nameLineEdit = QLineEdit()

        surnameLabel = QLabel(self.tr("Surname:"))
        surnameLineEdit = QLineEdit()

        formLayout = QFormLayout()
        formLayout.addRow(idLabel, idLineEdit)
        formLayout.addRow(nameLabel, nameLineEdit)
        formLayout.addRow(surnameLabel, surnameLineEdit)

        createRecordButton = QPushButton(self.tr("Create Record"))
        createRecordButton.setEnabled(False)

        buttonBox = QDialogButtonBox(Qt.Horizontal)
        buttonBox.addButton(createRecordButton, QDialogButtonBox.AcceptRole)
        buttonBox.setStandardButtons(QDialogButtonBox.Cancel)

        layout = QVBoxLayout()
        layout.addLayout(formLayout)
        layout.addWidget(buttonBox)

        self.setLayout(layout)

        # CONNECTIONS
        idLineEdit.textChanged.connect(lambda: createRecordButton.setEnabled(idLineEdit.hasAcceptableInput()))
        nameLineEdit.textEdited.connect(lambda text: nameLineEdit.setText(QLocale().toUpper(text)))
        surnameLineEdit.textEdited.connect(lambda text: surnameLineEdit.setText(QLocale().toUpper(text)))

        buttonBox.accepted.connect(self.beforeAccept)
        buttonBox.rejected.connect(self.reject)

        # PROPERTIES
        self.idLineEdit = idLineEdit
        self.nameLineEdit = nameLineEdit
        self.surnameLineEdit = surnameLineEdit

    def beforeAccept(self):
        recordId = self.idLineEdit.text()
        name = self.nameLineEdit.text()
        surname = self.surnameLineEdit.text()

        self.recordCreated.emit(recordId, name, surname)

    def open(self):
        self.idLineEdit.clear()
        self.nameLineEdit.clear()
        self.surnameLineEdit.clear()

        super().open()


class OpenRecordDialog(QDialog):
    recordSelected = pyqtSignal(str)

    def __init__(self, parent):
        super().__init__(parent)

        # DIALOG DEFINITION
        idLabel = QLabel(self.tr("Record ID:"))
        idLineEdit = QLineEdit()
        idLineEdit.setValidator(validatorId)

        openButton = QPushButton(self.tr("Open"))

        formLayout = QFormLayout()
        formLayout.addRow(idLabel, idLineEdit)

        layout = QVBoxLayout()
        layout.addLayout(formLayout)
        layout.addWidget(openButton)

        self.setLayout(layout)

        # CONNECTIONS
        openButton.clicked.connect(lambda: self.recordSelected.emit(idLineEdit.text()))

        # PROPERTIES
        self.idLineEdit = idLineEdit

    def open(self):
        self.idLineEdit.clear()

        super().open()
