import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15

import quick.recorddatabase 1.0

import "./Dialogs"

ApplicationWindow {
    id: mainWindow
    width: Screen.desktopAvailableWidth / 2
    height: Screen.desktopAvailableHeight / 3 * 2
    x: Screen.width / 2 - width / 2
    y: Screen.height / 2 - height / 2
    visible: true
    title: Qt.application.displayName

    Item {
        id: actions

        Action {
            id: newRecordAction
            text: qsTr("&New Record")
            shortcut: StandardKey.New
            onTriggered: newRecordDialog.open()
        }
    }

    menuBar: MenuBar {
        id: menuBar
        Layout.fillWidth: true

        Menu {
            title: qsTr("&File")

            MenuItem {
                action: newRecordAction
            }
        }

        Menu {
            title: qsTr("&Help")

            MenuItem {
                text: qsTr("&About")
                onTriggered: aboutDialog.open()
            }
        }
    }

    RecordManager {
        id: recordManager

        document: textArea.textDocument
        onCurrentRecordChanged: {
            mainWindow.title = currentRecord.name + " " + currentRecord.surname + " - " + Qt.application.displayName
        }
    }

    Flickable {
        id: flickable
        flickableDirection: Flickable.VerticalFlick
        anchors.fill: parent

        TextArea.flickable: TextArea {
            id: textArea
            textFormat: Qt.RichText
            wrapMode: TextArea.Wrap
            focus: true
            enabled: false
            selectByMouse: true
            persistentSelection: true

            leftPadding: 6
            rightPadding: 6
        }

        ScrollBar.vertical: ScrollBar {}
    }

    AboutDialog {
        id: aboutDialog
    }

    NewRecordDialog {
        id: newRecordDialog

        onAccepted: {
            recordManager.create(recordId.toUpperCase(), name.toUpperCase(), surname.toUpperCase())
        }
    }
}
