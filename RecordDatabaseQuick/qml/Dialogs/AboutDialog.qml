import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Dialog {
    id: root
    focus: true
    standardButtons: Dialog.Close
    title: qsTr("About")
    width: 500

    x: (mainWindow.width - width) / 2
    y: (mainWindow.height - height) / 3

    Text {
        text: qsTr("<p><b>Record Database Editor</b> allows storing " +
                   "per contact information in files using a database.</p>")
        width: parent.width
        wrapMode: Text.Wrap
    }
}
