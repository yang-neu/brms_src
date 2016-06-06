import QtQuick 2.0

Item {
    width: 280
    height: 43

    property string complexText: "";
    property string complexSubText: "";
    property int complexRectangleWidth: 262;
    Text{
        width: 50;
        height: 17;
        text: complexText;
        font.pixelSize: 14
    }
    Rectangle{
        x: 10
        y: 23
        width: complexRectangleWidth;
        height: 20;
        color: "white"
        Rectangle{
            anchors.fill: parent;
            color: "white"
        }
        Text{
            anchors.fill: parent;
            text: complexSubText;
            font.pixelSize: 14
        }
    }
}

