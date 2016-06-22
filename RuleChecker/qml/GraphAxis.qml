import QtQuick 2.0

Text {
    text: qsTr("Default")
    width: 30
    height: 50
    horizontalAlignment: Text.AlignRight
    verticalAlignment: Text.AlignVCenter

    Rectangle {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.right
        anchors.leftMargin: 10
        width: 600
        height: 1
        color: "gray"
    }
}
