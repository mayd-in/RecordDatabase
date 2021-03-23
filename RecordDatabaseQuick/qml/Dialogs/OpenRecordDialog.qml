import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Dialog {
    id: root

    property alias recordId: recordIdTF.text

    title: qsTr("Open Record")

    x: (mainWindow.width - width) / 2
    y: (mainWindow.height - height) / 2

    GridLayout {
        columns: 2

        Label {
            text: qsTr("Record ID:")
        }
        TextField {
            id: recordIdTF
            Layout.fillWidth: true
            validator: RegExpValidator {
                regExp: /\d{6}/
            }
        }
    }

    footer: DialogButtonBox {
        standardButtons: DialogButtonBox.Apply | DialogButtonBox.Cancel
        Component.onCompleted: {
            let openButton = standardButton(DialogButtonBox.Apply)
            openButton.text = qsTr("Open")
            openButton.enabled = Qt.binding(function() {return recordIdTF.acceptableInput})
        }
    }

    onOpened: {
        recordIdTF.clear()
    }
}
