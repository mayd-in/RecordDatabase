import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15

ApplicationWindow {
    width: Screen.desktopAvailableWidth / 2
    height: Screen.desktopAvailableHeight / 3 * 2
    x: Screen.width / 2 - width / 2
    y: Screen.height / 2 - height / 2
    visible: true
    title: Qt.application.displayName
}
