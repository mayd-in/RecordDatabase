from PyQt5.QtWidgets import *


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        textEditor = QTextEdit()
        self.setCentralWidget(textEditor)

        self.setupMenus()

        # PROPERTIES
        self.textEditor = textEditor

    def setupMenus(self):
        # HELP MENU
        helpMenu = QMenu(self.tr("&Help"), self)
        self.menuBar().addMenu(helpMenu)

        # CONNECTIONS
        helpMenu.addAction(self.tr("&Help"), lambda: QMessageBox.about(self, QApplication.applicationDisplayName(),
                    self.tr("<p><b>Record Database Editor</b> allows storing "
                       "per contact information in files using a database.</p>")))
        helpMenu.addAction(self.tr("About Qt"), QApplication.aboutQt)
