import QtQuick 2.0

Text {
    id: graphBar00
    text: qsTr("0.0")
    Column {
        x:8
        y:0
        spacing: 0
        anchors.bottom: parent.top
        anchors.bottomMargin: 11
        Rectangle { id:oldTrips00; color: "steelblue"; width: 10; height: 0 }
        Rectangle { id:thisTrip00; color: "orange"; width: 10; height: 0 }
        Rectangle { id:nowAccel00; color: "red"; width: 10; height: 0 }
    }

    function update(state, count) {
        if("old" === state) {
            oldTrips00.height= count;
        }else if("this" === state){
            thisTrip00.height= count;
        }
    }
}
