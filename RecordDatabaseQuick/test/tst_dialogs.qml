import QtTest 1.15
import QtQuick 2.15
import QtQuick.Controls 2.15

import "../qml/Dialogs"

TestCase {
    name: "test_dialogs"

    property NewRecordDialog newRecordDialog: null

    Component {
        id: factory

        NewRecordDialog {

        }
    }

    function initTestCase() {
        // Called before everything else
    }

    function cleanupTestCase() {
        // Called after tests run
    }

    function init() {
        // Called before each case
        newRecordDialog = factory.createObject(parent)
    }

    function cleanup() {
        // Called after each case
        newRecordDialog.destroy()
        newRecordDialog = null
    }

    function test_fields() {

    }

    function test_acceptButton() {
        let validId = "123123"
        let invalidId1 = "123"

        let createButton = newRecordDialog.footer.standardButton(DialogButtonBox.Apply)
        verify(!createButton.enabled, "Button is enabled at beginning")
    }

}
