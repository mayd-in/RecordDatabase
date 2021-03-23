import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Dialog {
    id: root

    property alias recordId: recordIdTF.text
    property alias name: nameTF.text
    property alias surname: surnameTF.text

    title: qsTr("New Record")

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

        Label {
            text: qsTr("Name:")
        }
        TextField {
            id: nameTF
            font.capitalization: Font.AllUppercase
        }

        Label {
            text: qsTr("Surname:")
        }
        TextField {
            id: surnameTF
            font.capitalization: Font.AllUppercase
        }
    }

    footer: DialogButtonBox {
        standardButtons: DialogButtonBox.Ok | DialogButtonBox.Cancel
        Component.onCompleted: {
            let okButton = standardButton(DialogButtonBox.Ok)
            okButton.text = qsTr("Create")
            okButton.enabled = Qt.binding(function() {return recordIdTF.acceptableInput})
        }
    }

    onOpened: {
        recordIdTF.clear()
        nameTF.clear()
        surnameTF.clear()
    }
}
