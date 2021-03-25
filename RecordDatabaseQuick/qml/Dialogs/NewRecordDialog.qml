import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Dialog {
    id: root

    property alias recordId: recordIdTF.text
    property alias name: nameTF.text
    property alias surname: surnameTF.text

    title: qsTr("New Record")
    focus: true
    width: 300

    x: (mainWindow.width - width) / 2
    y: (mainWindow.height - height) / 3

    GridLayout {
        anchors.fill: parent
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
            onAccepted: standardButton(DialogButtonBox.Apply).clicked()
        }

        Label {
            text: qsTr("Name:")
        }
        TextField {
            id: nameTF
            Layout.fillWidth: true
            font.capitalization: Font.AllUppercase
            onAccepted: standardButton(DialogButtonBox.Apply).clicked()
        }

        Label {
            text: qsTr("Surname:")
        }
        TextField {
            id: surnameTF
            Layout.fillWidth: true
            font.capitalization: Font.AllUppercase
            onAccepted: standardButton(DialogButtonBox.Apply).clicked()
        }
    }

    footer: DialogButtonBox {
        standardButtons: DialogButtonBox.Apply | DialogButtonBox.Cancel
        Component.onCompleted: {
            let okButton = standardButton(DialogButtonBox.Apply)
            okButton.text = qsTr("Create")
            okButton.enabled = Qt.binding(function() {return recordIdTF.acceptableInput})
        }
    }

    onAboutToShow: {
        recordIdTF.clear()
        nameTF.clear()
        surnameTF.clear()

        recordIdTF.focus = true
    }
}
