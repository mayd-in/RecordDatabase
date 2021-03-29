import sys
import unittest
from PyQt5 import QtTest

from PyQt5.QtWidgets import QApplication
from PyQt5.QtTest import QTest

from recorddatabase.dialogs import NewRecordDialog

app = QApplication(sys.argv)

class TestNewRecordDialog(unittest.TestCase):
    def setUp(self):
        self.dialog = NewRecordDialog()

    def test_construction(self):
        self.assertIsNotNone(self.dialog.idLineEdit, "LineEdit ID not created")
        self.assertIsNotNone(self.dialog.nameLineEdit, "LineEdit name not created")
        self.assertIsNotNone(self.dialog.surnameLineEdit, "LineEdit surname not created")

        self.assertIsNotNone(self.dialog.createRecordButton, "Create record button not created")

    def test_fields(self):
        validId = "123123"
        invalidId1 = "123"
        invalidId2 = "123asd"

        QTest.keyClicks(self.dialog.idLineEdit, validId)
        self.assertEqual(self.dialog.idLineEdit.text(), validId)

        self.dialog.idLineEdit.clear()
        QTest.keyClicks(self.dialog.idLineEdit, invalidId1)
        self.assertEqual(self.dialog.idLineEdit.text(), invalidId1)

        self.dialog.idLineEdit.clear()
        QTest.keyClicks(self.dialog.idLineEdit, invalidId2)
        self.assertEqual(self.dialog.idLineEdit.text(), "123")

    def test_acceptButton(self):
        validId = "123123"
        invalidId1 = "123"

        self.assertFalse(self.dialog.createRecordButton.isEnabled(), "Button is enabled at beginning")

        QTest.keyClicks(self.dialog.idLineEdit, invalidId1)
        self.assertFalse(self.dialog.createRecordButton.isEnabled(), "Button is enabled after invalid input")

        self.dialog.idLineEdit.clear()
        QTest.keyClicks(self.dialog.idLineEdit, validId)
        self.assertTrue(self.dialog.createRecordButton.isEnabled(), "Button is enabled after valid input")


if __name__ == "__main__":
    unittest.main()