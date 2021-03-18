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

    def __init__(self, model, parent):
        super().__init__(parent)

        # DIALOG DEFINITION
        idLabel = QLabel(self.tr("Record ID:"))
        idLineEdit = QLineEdit()
        idLineEdit.setValidator(validatorId)

        nameLabel = QLabel(self.tr("Name:"))
        nameLineEdit = QLineEdit()

        surnameLabel = QLabel(self.tr("Surname:"))
        surnameLineEdit = QLineEdit()

        # Date widgets
        currentDate = QDate.currentDate()

        startDateLabel = QLabel(self.tr("Start Date:"))
        startDateEdit = QDateEdit(currentDate.addMonths(-3))
        startDateEdit.setDisplayFormat("dd MMMM yyyy")
        startDateEdit.setCalendarPopup(True)

        endDateLabel = QLabel(self.tr("End Date:"))
        endDateEdit = QDateEdit(currentDate)
        endDateEdit.setDisplayFormat("dd MMMM yyyy")
        endDateEdit.setCalendarPopup(True)

        startDateEdit.setMaximumDate(endDateEdit.date())
        endDateEdit.setMinimumDate(startDateEdit.date())

        startDateEdit.dateChanged.connect(endDateEdit.setMinimumDate)
        endDateEdit.dateChanged.connect(startDateEdit.setMaximumDate)

        findRecordButton = QPushButton(self.tr("Find Records"))

        formLayout = QFormLayout()
        formLayout.addRow(idLabel, idLineEdit)
        formLayout.addRow(nameLabel, nameLineEdit)
        formLayout.addRow(surnameLabel, surnameLineEdit)
        formLayout.addRow(startDateLabel, startDateEdit)
        formLayout.addRow(endDateLabel, endDateEdit)

        # Model view
        proxyModel = QSortFilterProxyModel(self)
        proxyModel.setSourceModel(model)
        proxyModel.setSortLocaleAware(True)

        tableView = QTableView()
        tableView.setModel(proxyModel)
        tableView.setSelectionBehavior(QTableView.SelectRows)
        tableView.setSelectionMode(QTableView.SingleSelection)
        tableView.setSortingEnabled(True)
        tableView.verticalHeader().hide()
        tableView.horizontalHeader().setStretchLastSection(True)
        tableView.setSizeAdjustPolicy(QAbstractScrollArea.SizeAdjustPolicy.AdjustToContents)
        tableView.resizeColumnsToContents()
        tableView.sortByColumn(3, Qt.SortOrder.DescendingOrder)  # Last modified column

        layout = QVBoxLayout()
        layout.addLayout(formLayout)
        layout.addWidget(findRecordButton)
        layout.addWidget(tableView)

        self.setLayout(layout)

        # CONNECTIONS
        nameLineEdit.textEdited.connect(lambda text: nameLineEdit.setText(QLocale().toUpper(text)))
        surnameLineEdit.textEdited.connect(lambda text: surnameLineEdit.setText(QLocale().toUpper(text)))

        tableView.doubleClicked.connect(self.rowSelected)
        findRecordButton.clicked.connect(self.updateModel)

        # PROPERTIES
        self.idLineEdit = idLineEdit
        self.nameLineEdit = nameLineEdit
        self.surnameLineEdit = surnameLineEdit
        self.startDateEdit = startDateEdit
        self.endDateEdit = endDateEdit

        self.model = model

    def rowSelected(self, index):
        selectedRecordId = index.model().index(index.row(), 0).data()  # 0 -> record_id column
        self.recordSelected.emit(selectedRecordId)

    def updateModel(self):
        recordId = self.idLineEdit.text()
        name = self.nameLineEdit.text()
        surname = self.surnameLineEdit.text()
        startDate = self.startDateEdit.dateTime()
        endDate = self.endDateEdit.dateTime().addDays(1)

        self.model.filterResults(recordId, name, surname, startDate, endDate)

    def open(self):
        self.idLineEdit.clear()
        self.nameLineEdit.clear()
        self.surnameLineEdit.clear()

        self.updateModel()

        super().open()
