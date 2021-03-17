import sys

from PyQt5.QtWidgets import QApplication

from recorddatabase.mainwindow import MainWindow

if __name__ == "__main__":
    app = QApplication([])

    app.setOrganizationName("QtProject")
    app.setApplicationName("Record Database Example")
    app.setApplicationDisplayName(app.tr("Record Database Editor"))

    w = MainWindow()

    availableGeometry = w.screen().availableGeometry()
    w.resize(availableGeometry.width() / 2, (availableGeometry.height() * 2) / 3)
    w.move((availableGeometry.width() - w.width()) / 2,
            (availableGeometry.height() - w.height()) / 2)

    w.show()

    sys.exit(app.exec())
