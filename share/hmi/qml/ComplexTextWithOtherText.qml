import QtQuick 2.0

Item {
    width: 550
    height: 43

    property string complexTextWT: "";
    property string complexSubTextWT: "";
    property string complexSubTextWT_Stats: "";
    property string complexSubTextWT_Number: "";
    ComplexText{
        complexText: complexTextWT;
        complexSubText: complexSubTextWT;
        complexRectangleWidth: 300;
    }
    Rectangle{
        x: 349
        y: 18
        width: 100;
        height: 17;
        color: "white"
    }
    Text{
        x: 349
        y: 18
        width: 100;
        height: 17;
        text: complexSubTextWT_Stats;
        font.pixelSize: 14
    }
    Rectangle{
        x: 460
        y: 18
        width: 50;
        height: 17;
        color: "white"
    }
    Text{
        x: 460
        y: 18
        width: 50;
        height: 17;
        text: complexSubTextWT_Number;
        font.pixelSize: 14
    }
}

