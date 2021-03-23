import QtQuick 2.15
import QtQuick.Controls 2.15

Dialog {
    id: root

    function warning(message) {
        errorMessage.text = message
        open()
    }

    function critical(message) {
        errorMessage.text = message
        open()
    }

    title: qsTr("Error")
    standardButtons: Dialog.Close
    contentWidth: 300
    x: (mainWindow.width - width) / 2
    y: (mainWindow.height - height) / 2

    Text {
        id: errorMessage
        width: parent.width
        wrapMode: Text.Wrap
    }
}
