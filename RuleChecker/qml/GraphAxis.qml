import QtQuick 2.0

Text {
    text: qsTr("Default")
    Rectangle {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.right
        width: 600
        height: 1
        color: "gray"
    }
}
