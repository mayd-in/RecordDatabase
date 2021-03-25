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
    visible: true
    title: Qt.application.displayName

    // Do not bind, breaks resizing
    Component.onCompleted: {
        x = (Screen.width - width) / 2
        y = (Screen.height - height) / 2
    }

    function updateWindowProperties() {
        let currentRecord = recordManager.currentRecord
        if (currentRecord.recordId === "")  // Temporary
            return

        if (!documentHandler.modified)
            mainWindow.title = currentRecord.name + " " + currentRecord.surname + " - " + Qt.application.displayName
        else
            mainWindow.title = currentRecord.name + " " + currentRecord.surname + "* - " + Qt.application.displayName
        textArea.enabled = true
    }

    function save() {
        let error = recordManager.save()
        switch (error) {
        case RecordManager.NoError:
            return true
        case RecordManager.NoCurrentRecord:
            return true
        case RecordManager.FileSaveFailed:
            errorDialog.warning(qsTr("Unable to save file"))
            break
        default:
            errorDialog.critical(qsTr("Unknown error occurred"))
        }
        return false
    }

    function maybeSave(callback) {
        if (!documentHandler.modified) {
            callback()
            return
        }

        maybeSaveDialog.callback = callback
        maybeSaveDialog.open()
    }

    onClosing: {
        close.accepted = false
        maybeSave(Qt.quit)
    }

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
            onTriggered: mainWindow.save()
        }

        Action {
            id: fontIncreaseAction
            text: qsTr("Increase")
            icon.name: "format-font-size-more"
            checkable: false
            onTriggered: documentHandler.modifyFontSize(1)
        }
        Action {
            id: fontDecreaseAction
            text: qsTr("Decrease")
            icon.name: "format-font-size-less"
            checkable: false
            onTriggered: documentHandler.modifyFontSize(-1)
        }

        Action {
            id: boldAction
            text: qsTr("&Bold")
            icon.name: "format-text-bold"
            shortcut: StandardKey.Bold
            checkable: true
            checked: documentHandler.bold
            onToggled: documentHandler.bold = checked
        }
        Action {
            id: italicAction
            text: qsTr("&Italic")
            icon.name: "format-text-italic"
            shortcut: StandardKey.Italic
            checkable: true
            checked: documentHandler.italic
            onTriggered: documentHandler.italic = !documentHandler.italic
        }
        Action {
            id: underlineAction
            text: qsTr("&Underline")
            icon.name: "format-text-underline"
            shortcut: StandardKey.Underline
            checkable: true
            checked: documentHandler.underline
            onTriggered: documentHandler.underline = !documentHandler.underline
        }

        Action {
            id: colorPickerAction
            text: qsTr("Color")
            icon.name: "draw-brush"
            onTriggered: colorDialog.open()
        }

        Action {
            id: fontChooserAction
            text: qsTr("Font")
            icon.name: "gtk-select-font"
            onTriggered: fontDialog.open()
        }

        Action {
            id: alignLeftAction
            text: qsTr("&Left")
            icon.name: "format-justify-left"
            checkable: true
            checked: documentHandler.alignment == Qt.AlignLeft
            onTriggered: documentHandler.alignment = Qt.AlignLeft
        }
        Action {
            id: alignCenterAction
            text: qsTr("C&enter")
            icon.name: "format-justify-center"
            checkable: true
            checked: documentHandler.alignment == Qt.AlignCenter
            onTriggered: documentHandler.alignment = Qt.AlignCenter
        }
        Action {
            id: alignRightAction
            text: qsTr("&Right")
            icon.name: "format-justify-right"
            checkable: true
            checked: documentHandler.alignment == Qt.AlignRight
            onTriggered: documentHandler.alignment = Qt.AlignRight
        }
        Action {
            id: alignJustifyAction
            text: qsTr("&Justify")
            icon.name: "format-justify-fill"
            checkable: true
            checked: documentHandler.alignment == Qt.AlignJustify
            onTriggered: documentHandler.alignment = Qt.AlignJustify
        }

        Action {
            id: indentMoreAction
            text: qsTr("&Indent")
            icon.name: "format-indent-more"
            checkable: false
            onTriggered: documentHandler.modifyIndentation(1)
        }
        Action {
            id: indentLessAction
            text: qsTr("&Unindent")
            icon.name: "format-indent-less"
            checkable: false
            onTriggered: documentHandler.modifyIndentation(-1)
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

    header: ToolBar {
        leftPadding: 8

        Flow {
            id: root
            width: parent.width

            Row {
                id: fontRow
                ToolButton {
                    action: fontIncreaseAction
                    display: ToolButton.IconOnly
                    focusPolicy: Qt.TabFocus
                }

                ToolButton {
                    action: fontDecreaseAction
                    display: ToolButton.IconOnly
                    focusPolicy: Qt.TabFocus
                }

                ToolSeparator {
                    contentItem.visible: fontRow.y === formatRow.y
                }
            }

            Row {
                id: formatRow
                ToolButton {
                    action: boldAction
                    display: ToolButton.IconOnly
                    font.bold: true
                    focusPolicy: Qt.TabFocus
                }

                ToolButton {
                    action: italicAction
                    display: ToolButton.IconOnly
                    font.italic: true
                    focusPolicy: Qt.TabFocus
                }

                ToolButton {
                    action: underlineAction
                    display: ToolButton.IconOnly
                    font.underline: true
                    focusPolicy: Qt.TabFocus
                }

                ToolSeparator {
                    contentItem.visible: formatRow.y === miscRow.y
                }
            }

            Row {
                id: miscRow

                ToolButton {
                    id: textColorButton
                    action: colorPickerAction
                    display: ToolButton.IconOnly
                    focusPolicy: Qt.TabFocus
                }
                ToolButton {
                    id: fontChooserButton
                    action: fontChooserAction
                    display: ToolButton.IconOnly
                    focusPolicy: Qt.TabFocus
                }

                ToolSeparator {
                    contentItem.visible: miscRow.y === alignRow.y
                }
            }

            Row {
                id: alignRow
                ToolButton {
                    action: alignLeftAction
                    display: ToolButton.IconOnly
                    focusPolicy: Qt.TabFocus
                }

                ToolButton {
                    action: alignCenterAction
                    display: ToolButton.IconOnly
                    focusPolicy: Qt.TabFocus
                }

                ToolButton {
                    action: alignRightAction
                    display: ToolButton.IconOnly
                    focusPolicy: Qt.TabFocus
                }

                ToolButton {
                    action: alignJustifyAction
                    display: ToolButton.IconOnly
                    focusPolicy: Qt.TabFocus
                }

                ToolSeparator {
                    contentItem.visible: alignRow.y === indentRow.y
                }
            }

            Row {
                id: indentRow
                ToolButton {
                    action: indentMoreAction
                    display: ToolButton.IconOnly
                    focusPolicy: Qt.TabFocus
                }

                ToolButton {
                    action: indentLessAction
                    display: ToolButton.IconOnly
                    focusPolicy: Qt.TabFocus
                }
            }
        }
    }

    RecordManager {
        id: recordManager

        document: textArea.textDocument
        onCurrentRecordChanged: mainWindow.updateWindowProperties()
    }

    DocumentHandler {
        id: documentHandler
        document: textArea.textDocument
        cursorPosition: textArea.cursorPosition
        selectionStart: textArea.selectionStart
        selectionEnd: textArea.selectionEnd

        onModifiedChanged: updateWindowProperties()
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

    Dialog {
        id: maybeSaveDialog

        property var callback

        title: qsTr("Save Changes?")
        focus: true
        standardButtons: Dialog.Save | Dialog.Discard |Dialog.Cancel
        x: (mainWindow.width - width) / 2
        y: (mainWindow.height - height) / 3

        Text {
            text: qsTr("There are unsaved changes.\n"+
                       "Do you want to save your changes?")
            width: parent.width
            wrapMode: Text.Wrap
        }

        onAccepted: {
            if (mainWindow.save())
                callback()
        }
        onDiscarded: {
            close()
            callback()
        }
    }

    AboutDialog {
        id: aboutDialog
    }

    ColorDialog {
        id: colorDialog

        color: documentHandler.textColor

        onAccepted: {
            documentHandler.textColor = color
        }
    }

    FontDialog {
        id: fontDialog

        font.family: documentHandler.fontFamily
//        font.pointSize: documentHandler.fontSize

        onAccepted: {
            documentHandler.fontFamily = font.family;
            documentHandler.fontSize = font.pointSize;
        }
    }

    NewRecordDialog {
        id: newRecordDialog

        onApplied: maybeSave(createRecord)

        function createRecord() {
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

        onApplied: maybeSave(openRecord)

        function openRecord() {
            let error = recordManager.open(recordId)
            switch (error) {
            case RecordManager.NoError:
                accept()
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
