import QtQuick 2.0

Text {
    property string displayText: "Speed";
    property string displayData: "52.39";
    property string displayUnit: "km/h";

    text: displayText;
    font.pixelSize: 14

    Text {
        x: 180;
        y: 0;
        width: 50;
        text: displayData;
        font.pixelSize: 14
        horizontalAlignment: Text.AlignRight
    }
    Text {
        x: 238;
        y: 0;
        text: displayUnit;
        font.pixelSize: 14
    }

}
