import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3 as Dialogs

ApplicationWindow {
    id: mainWindow
    width: Screen.desktopAvailableWidth / 2
    height: Screen.desktopAvailableHeight / 3 * 2
    x: Screen.width / 2 - width / 2
    y: Screen.height / 2 - height / 2
    visible: true
    title: Qt.application.displayName

    menuBar: MenuBar {
        id: menuBar
        Layout.fillWidth: true

        Menu {
            title: qsTr("&Help")

            MenuItem {
                text: qsTr("&About")
                onTriggered: aboutDialog.open()

                Dialogs.Dialog {
                    id: aboutDialog
                    standardButtons: Dialog.Close
                    title: mainWindow.title
                    Text {
                        text: qsTr("<p><b>Record Database Editor</b> allows storing " +
                                   "per contact information in files using a database.</p>")
                    }
                }
            }
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
            selectByMouse: true
            persistentSelection: true

            leftPadding: 6
            rightPadding: 6
        }

        ScrollBar.vertical: ScrollBar {}
    }
}
