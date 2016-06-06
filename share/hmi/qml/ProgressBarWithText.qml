import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
Item {
    width: 280
    height: 43
    property string progressText: "Speed";
    property string progressDate: "52.39";
    property string progressUnit: "km/h";
    property double progressValue: 40;
    property double progressMinimumValue: 0;
    property double progressMaximumValue: 100;

    Text{
        width: 50;
        height: 17;
        text:progressText;
        font.pixelSize: 14
    }
    Text{
        x: 159
        y: 0;
        width: 42
        height: 17;
        text:progressDate;
        font.pixelSize: 14
    }
    Text{
        x: 238
        y: 0
        width: 42;
        height: 17;
        text:progressUnit;
        font.pixelSize: 14
    }
    ProgressBar {
        x: 10
        y: 23
        value: progressValue;
        minimumValue: progressMinimumValue;
        maximumValue: progressMaximumValue;
        width: 262
        height: 20;
    }
}
