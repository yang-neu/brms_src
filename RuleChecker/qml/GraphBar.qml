import QtQuick 2.0

Text {
    id: graphBar00
    text: qsTr("0.0")
    width: 35
    height: 30
    horizontalAlignment: Text.AlignHCenter

    property double scaleRate: 1

    Column {
        spacing: 0
        anchors.horizontalCenter: parent.horizontalCenter;
        anchors.bottom: parent.top
        anchors.bottomMargin: 11

        transform: Scale {
            origin.y: coution00.height + oldTrips00.height + thisTrip00.height + dummy00.height
            yScale: scaleRate
        }

        Image {
            id: coution00;
            visible: true;
            opacity: 0;
            anchors.horizontalCenter: parent.horizontalCenter;
            source: "res/Caution.png" ;

            transform: Scale {
                origin.y: coution00.height
                yScale: 1/scaleRate
            }
        }

        Rectangle {
            id:oldTrips00;
            color: "steelblue";
            border.color: "transparent"
            //border.width: 2
            width: 18;
            height: 0;
            anchors.horizontalCenter: parent.horizontalCenter;
        }

        Rectangle {
            id:thisTrip00;
            color: "orange";
            width: oldTrips00.width;
            height: 0 ;
            anchors.horizontalCenter: parent.horizontalCenter;

            onHeightChanged: {
                animation00.running = true
                animation01.running = true
            }

            PropertyAnimation {
                id: animation00;
                target: thisTrip00;
                property: "color";
                from: "red"
                to: thisTrip00.color;
                running: false;
                duration: 10000;
                loops: 1 //Animation.Infinite;
            }
        }

        Rectangle {
            id: dummy00;
            color: "transparent";
            width: 18;
            height: 0
            anchors.horizontalCenter: parent.horizontalCenter;

            SequentialAnimation {
                 id: animation01;
                 running: false;

                 NumberAnimation {
                    target: dummy00;
                    property: "height";
                    from: 0
                    to: ((500/scaleRate)-((oldTrips00.height+thisTrip00.height)))/2;
                    running: false;
                    duration: 1000;
                    loops: 1 //Animation.Infinite;
                    easing.type: Easing.OutQuart
                  }
                 NumberAnimation {
                    target: dummy00;
                    property: "height";
                    from: ((500/scaleRate)-((oldTrips00.height+thisTrip00.height)))/2
                    to: 0;
                    running: false;
                    duration: 3000;
                    loops: 1 //Animation.Infinite;
                    easing.type: Easing.OutBounce
                  }
            }
        }

        PropertyAnimation {
            //ボーダー色変更
            id: animation04;
            target: oldTrips00;
            property: "border.color";
            from: "red"
            to: "transparent";
            running: false;
            duration: 1000;
            loops: 10 //Animation.Infinite;
        }

    }

    function update(state, count, caution) {
        if("old" === state) {
            oldTrips00.height= count;
        }else if("this" === state){
            thisTrip00.height= count;
            if("caution" === caution) {
                coution00.visible = true;
                coution00.opacity = 1;
            }
        }
    }

}
