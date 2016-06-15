import QtQuick 2.0

Text {
    id: graphBar00
    text: qsTr("0.0")
    Column {
        spacing: 0
        anchors.horizontalCenter: parent.horizontalCenter;
        anchors.bottom: parent.top
        anchors.bottomMargin: 11

        Image {
            id: coution00;
            visible: false;
            anchors.horizontalCenter: parent.horizontalCenter;
            source: "res/Caution.png" ;
        }
        Rectangle {
            id:oldTrips00;
            color: "steelblue";
            width: 10;
            height: 0;
            anchors.horizontalCenter: parent.horizontalCenter;
        }
        Rectangle {
            id:thisTrip00;
            color: "orange";
            width: 10;
            height: 0 ;
            anchors.horizontalCenter: parent.horizontalCenter;

            onHeightChanged: {
                animation00.running = true
            }

            PropertyAnimation {
                id: animation00;
                target: thisTrip00;
                property: "color";
                from: "red"
                to: thisTrip00.color;
                running: false;
                duration: 1000;
                //loops: 5 //Animation.Infinite;
            }

        }
    }

    function update(state, count, caution) {
        if("old" === state) {
            oldTrips00.height= count;
        }else if("this" === state){
            thisTrip00.height= count;
            if("caution" === caution) {
                coution00.visible = true;
            }
        }
    }

}
