import QtQuick 2.0

Text {
    text: qsTr("Default")
    width: 30
    horizontalAlignment: Text.AlignRight

    Rectangle {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.right
        anchors.leftMargin: 10
        width: 600
        height: 1
        color: "gray"
    }
}
