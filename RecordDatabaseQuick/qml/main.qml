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
        Action {
            id: openRecordAction
            text: qsTr("&Open Record")
            shortcut: StandardKey.Open
            onTriggered: openRecordDialog.open()
        }
        Action {
            id: saveRecordAction
            text: qsTr("&Save Record")
            shortcut: StandardKey.Save
            onTriggered: {
                let error = recordManager.save()
                switch (error) {
                case RecordManager.NoError:
                    break
                case RecordManager.NoCurrentRecord:
                    break
                case RecordManager.FileSaveFailed:
                    errorDialog.warning(qsTr("Unable to save file"))
                    break
                default:
                    errorDialog.critical(qsTr("Unknown error occurred"))
                }
            }
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
            MenuItem {
                action: openRecordAction
            }
            MenuItem {
                action: saveRecordAction
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
            textArea.enabled = currentRecord
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

    ErrorDialog {
        id: errorDialog
    }

    AboutDialog {
        id: aboutDialog
    }

    NewRecordDialog {
        id: newRecordDialog

        onApplied: {
            let error = recordManager.create(recordId, name.toUpperCase(), surname.toUpperCase())
            switch (error) {
            case RecordManager.NoError:
                accept()
                break
            case RecordManager.RecordExists:
                errorDialog.warning(qsTr("Record exists already"))
                break
            default:
                errorDialog.critical(qsTr("Unknown error occurred"))
            }
        }
    }

    OpenRecordDialog {
        id: openRecordDialog

        onApplied: {
            let error = recordManager.open(recordId)
            switch (error) {
            case RecordManager.NoError:
                break
            case RecordManager.RecordNotExist:
                errorDialog.warning(qsTr("Record not found"))
                break
            case RecordManager.FileOpenFailed:
                errorDialog.warning(qsTr("Unable to open file"))
                break
            default:
                errorDialog.critical(qsTr("Unknown error occurred"))
            }
        }
    }
}
