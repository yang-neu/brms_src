import QtQuick 2.0

Item  {
    id: container
    property variant scrollArea
    property int orientation: Qt.Vertical

    function position(){
        var pos = 0;
        if (container.orientation === Qt.Vertical)
            pos = scrollArea.visibleArea.yPosition * (container.height-2) + 1;
        else
            pos = 1;
        return pos;
    }
    function size(){
        var size;
        if (container.orientation === Qt.Vertical)
            size = scrollArea.visibleArea.heightRatio * (container.height-2);
        else
            size = container.width - 2;
        return size;
    }

    Rectangle  {
        id: bar
        anchors.fill: parent
        color: "#eeeeee"
        border.color: "#a8bed4";
        Rectangle {
            id: button
            x: orientation == Qt.Vertical ? 1 : position()
            y: orientation == Qt.Vertical ? position() : 1
            width: orientation == Qt.Vertical ? (parent.width-2) : size()
            height: orientation == Qt.Vertical ? (size() < 20 ? 20 : size()) : (parent.height-2)
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#d9e4ef" }
                GradientStop { position: 0.45; color: "#f0f5fa" }
                GradientStop { position: 1.0; color: "#bbd2e7" }
            }
            border.color: "#a8bed4";
            Rectangle{
                id:line1
                color: "#809acb"
                visible: orientation == Qt.Vertical ? true : false
                height: 2;
                width: parent.width - 4;
                anchors.horizontalCenter: button.horizontalCenter;
                anchors.bottom: line2.top;
                anchors.bottomMargin: 2;
            }
            Rectangle{
                id:line2
                color: "#809acb"
                visible: orientation == Qt.Vertical ? true : false
                height: 2;
                width: parent.width - 4;
                anchors.horizontalCenter: button.horizontalCenter;
                anchors.verticalCenter: button.verticalCenter;
            }
            Rectangle{
                id:line3
                color: "#809acb"
                visible: orientation == Qt.Vertical ? true : false
                height: 2;
                width: parent.width - 4;
                anchors.horizontalCenter: button.horizontalCenter;
                anchors.top: line2.bottom;
                anchors.topMargin: 2;
            }
            Rectangle{
                id:line4
                color: "#809acb"
                visible: orientation == Qt.Vertical ? false : true
                height: parent.height - 4;
                width: 2;
                anchors.verticalCenter: button.verticalCenter;
                anchors.right: line5.left;
                anchors.rightMargin: 2;
            }
            Rectangle{
                id:line5
                color: "#809acb"
                visible: orientation == Qt.Vertical ? false : true
                height: parent.height - 4;
                width: 2;
                anchors.horizontalCenter: button.horizontalCenter;
                anchors.verticalCenter: button.verticalCenter;
            }
            Rectangle{
                id:line6
                color: "#809acb"
                visible: orientation == Qt.Vertical ? false : true
                height: parent.height - 4;
                width: 2;
                anchors.verticalCenter: button.verticalCenter;
                anchors.left: line5.right;
                anchors.leftMargin: 2;
            }
            MouseArea {
                id: ma_button
                anchors.fill: button
                hoverEnabled: true
                drag.target: button
                drag.axis: Qt.Vertical ? Drag.YAxis : Drag.XAxis
                drag.minimumY: 0
                drag.maximumY: Qt.Vertical ? container.height - button.height : 0
                drag.minimumX: 0
                drag.maximumX: Qt.Vertical ? 0 : container.width - button.width

                onMouseYChanged: {
                    if(ma_button.pressed) {
                        scrollArea.contentY = button.y / container.height * scrollArea.contentHeight;
                    }
                }
            }
        }
    }
}
