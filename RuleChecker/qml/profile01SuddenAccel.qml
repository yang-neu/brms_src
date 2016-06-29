import QtQuick 2.1
import QtQuick.Window 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.1
import hmiapp 1.0

Window {
    id: profile01SuddenAccel;
    visible: true;
    width: 1024;
    height: 768;
    color: "#eeeeee";

    property double m_Speed: 250;
    property string driveScene:"AddSpeedRunning";

    property int accelCharacteristic: 0;

    property double aTripMax: 0;
    property int oldTripSamplingNmuK: 0;
    property int thisTripSamplingNmuK: 0;
    property int suddenAccelRate: 0;
    property double oldTripAccelAve: 0;
    property double thisTripAccelAve: 0;
    property double aTripVariance: 0;
    property int aTripOutOf3sigma: 0;

    property int maxCountNumOldTrip: 0;
    property int maxCountNumThisTrip: 0;
    property double graphDisplayRate: 1;

    property double tmpDouble1: 0
    property double tmpDouble2: 0

    property int igOffId: 0

    property string debugbarcolor: "red";
    property string comDataSize: "";

    Component.onCompleted: {
        //Nothing to do
    }

    Connections {
        target: brms;
        onSpeedChanged:m_Speed = speed.toFixed(2);
        onDriveSceneChanged: driveScene = str;

        onAccelInfoChanged: {
            //localfunc.printConsoleLog("accel=" + data + ", count=" + count);

            localfunc.setGraphDisplayRate(state, count);

            localfunc.displayGraph(state, data, count, caution);
        }
        onAnalysisResultChanged: {
            aTripMax = aMax.toFixed(2);
            oldTripSamplingNmuK = oldNum;
            thisTripSamplingNmuK = thisNum;
            suddenAccelRate = rate;
            oldTripAccelAve = oldAve.toFixed(2);
            thisTripAccelAve = thisAve.toFixed(2)
            aTripVariance = variance.toFixed(2);
            aTripOutOf3sigma = count;
        }
        onStandardRangeChanged: {
            graphStandardRange.visible = true
            tmpDouble1 = data1.toFixed(1)<=0.8 ? 0.8 : data1.toFixed(1) //0.8以下の標準範囲を0.8に丸め込む
            tmpDouble2 = data2.toFixed(1)>=2.3 ? 2.3 : data2.toFixed(1) //2.3以上の標準範囲を2.3に丸め込む
            graphStandardRange.x = horizontalAxis.x + (((tmpDouble1.toFixed(1)*10)-7) * (graphBar09.width + horizontalAxis.spacing))
            graphStandardRange.width = (((tmpDouble2.toFixed(1)*10)-(tmpDouble1.toFixed(1)*10)+1) * (graphBar09.width + horizontalAxis.spacing))
        }

        onAccelCharacteristicChanged: {
            if("急加速しない" == state) {
                stateNormalAccel.border.color = "blue"
                stateSuddenAccel.border.color = "transparent"
            }else if("急加速しがち" == state){
                stateNormalAccel.border.color = "transparent"
                stateSuddenAccel.border.color = "blue"
            }else{ //未定義
                stateNormalAccel.border.color = "transparent"
                stateSuddenAccel.border.color = "transparent"
            }
        }

        onSocket_status_changed:{
               debugBar.color = color
        }

        onComDataSizeChanged: localfunc.displayComDataSize(size);

    }

    //--------------------------------------------------
    // 画面
    //--------------------------------------------------
    Rectangle{
        id: graph01
        x:0;
        y:0;

        Column {
            id: verticalAxis
            x:10;
            y:20;
            spacing: 0;

            GraphAxis{ text: qsTr((500*graphDisplayRate).toFixed(0)); id: verticalAxisTop }
            GraphAxis{ text: qsTr((450*graphDisplayRate).toFixed(0)) }
            GraphAxis{ text: qsTr((400*graphDisplayRate).toFixed(0)) }
            GraphAxis{ text: qsTr((350*graphDisplayRate).toFixed(0)) }
            GraphAxis{ text: qsTr((300*graphDisplayRate).toFixed(0)) }
            GraphAxis{ text: qsTr((250*graphDisplayRate).toFixed(0)) }
            GraphAxis{ text: qsTr((200*graphDisplayRate).toFixed(0)) }
            GraphAxis{ text: qsTr((150*graphDisplayRate).toFixed(0)) }
            GraphAxis{ text: qsTr((100*graphDisplayRate).toFixed(0)) }
            GraphAxis{ text: qsTr((50*graphDisplayRate).toFixed(0)) }
            GraphAxis{ text: qsTr((0*graphDisplayRate).toFixed(0))
                Row {
                    id: horizontalAxis
                    x: 40;
                    y: 36;
                    spacing: 0;
                    //GraphBar {id: graphBar01; text: qsTr("0.1") }
                    //GraphBar {id: graphBar02; text: qsTr("0.2") }
                    //GraphBar {id: graphBar03; text: qsTr("0.3") }
                    //GraphBar {id: graphBar04; text: qsTr("0.4") }
                    //GraphBar {id: graphBar05; text: qsTr("0.5") }
                    //GraphBar {id: graphBar06; text: qsTr("0.6") }
                    GraphBar {id: graphBar07; text: qsTr("←") }
                    GraphBar {id: graphBar08; text: qsTr("0.8") }
                    GraphBar {id: graphBar09; text: qsTr("0.9") }
                    GraphBar {id: graphBar10; text: qsTr("1.0") }
                    GraphBar {id: graphBar11; text: qsTr("1.1") }
                    GraphBar {id: graphBar12; text: qsTr("1.2") }
                    GraphBar {id: graphBar13; text: qsTr("1.3") }
                    GraphBar {id: graphBar14; text: qsTr("1.4") }
                    GraphBar {id: graphBar15; text: qsTr("1.5") }
                    GraphBar {id: graphBar16; text: qsTr("1.6") }
                    GraphBar {id: graphBar17; text: qsTr("1.7") }
                    GraphBar {id: graphBar18; text: qsTr("1.8") }
                    GraphBar {id: graphBar19; text: qsTr("1.9") }
                    GraphBar {id: graphBar20; text: qsTr("2.0") }
                    GraphBar {id: graphBar21; text: qsTr("2.1") }
                    GraphBar {id: graphBar22; text: qsTr("2.2") }
                    GraphBar {id: graphBar23; text: qsTr("→")}
                }
                Rectangle {
                    id: graphStandardRange
                    visible: false
                    x: horizontalAxis.x + ((9-7) * (graphBar09.width + horizontalAxis.spacing)) //0.9~1.4
                    y: 30
                    width: ((14-9+1) * (graphBar09.width + horizontalAxis.spacing)) //0.9~1.4
                    height: graphBar09.height
                    border.width: 2
                    border.color: "green"
                    color: "transparent"
                    radius: 5
                }
            }


        }

        Rectangle {
            id: graphSuddenAccelBar
            x : horizontalAxis.x + ((17-6) * (graphBar17.width + horizontalAxis.spacing)) + (graphBar17.width/4); //1.7以上は急加速
            y : verticalAxis.y;
            width: 3
            height: verticalAxisTop.height/2 + (verticalAxisTop.height * 10)
            color: "purple"
            Text {
                x:10
                text: qsTr("急加速領域")
                color: parent.color
            }
        }

        ColumnLayout {
            id: graphCaption
            x: 70
            y: 610
            RowLayout {
                spacing: 10
                Rectangle {
                    width: 20
                    height: 20
                    color: "steelblue"
                }
                Text {
                    text: qsTr("これまでのドライブのデータ")
                    font.pixelSize: 14
                }
                Rectangle {
                    width: 90
                    height: 20
                    border.width: 2
                    border.color: "green"
                    color: "transparent"
                    radius: 5
                }
                Text {
                    text: qsTr("これまでのデータからの標準範囲")
                    font.pixelSize: 14
                }
            }
            RowLayout {
                spacing: 10
                Rectangle {
                    width: 20
                    height: 20
                    color: "orange"
                }
                Text {
                    text: qsTr("今回のドライブのデータ")
                    font.pixelSize: 14
                }
                Rectangle {
                    width: 20
                    height: 20
                    color: "red"
                }
                Text {
                    text: qsTr("最新のデータ")
                    font.pixelSize: 14
                }
                Image {
                    source: "res/Caution.png" ;
                }
                Text {
                    text: qsTr("今回のドライブでの異常値")
                    font.pixelSize: 14
                }
            }
        }
    }



    ColumnLayout {
        x: 680
        y: 0
        spacing: 20;

        ColumnLayout {
            Text{
                text: qsTr("■診断結果");
                //font.pixelSize: 14
            }
            Rectangle {
                id: stateSuddenAccel
                width: 320
                height: 106
                border.width: 3
                border.color: profile01SuddenAccel.color // or "blue"
                color: "transparent"
                radius: 10

                Image {
                    source: "res/state_SuddenAccel.png"
                }
                Text {
                   anchors.right: parent.right
                   anchors.bottom: parent.bottom
                   anchors.margins: 10
                   text: qsTr("急加速しがち")
                   color: "red"
                }
            }
            Rectangle {
                id: stateNormalAccel
                width: stateSuddenAccel.width
                height: stateSuddenAccel.height
                border.width: 3
                border.color: "blue" //or profile01SuddenAccel.color
                color: "transparent"
                radius: 10

                Image {
                    source: "res/state_NormalAccel.png"
                }
                Text {
                   anchors.right: parent.right
                   anchors.bottom: parent.bottom
                   anchors.margins: 10
                   text: qsTr("急加速しない")
                   color: "red"
                }
            }
        }

        Text{
            text: qsTr("■入力情報");
            //font.pixelSize: 14
        }
        NumericalData {
            displayText: qsTr("速度");
            displayData: m_Speed.toString();
            displayUnit: qsTr("km/h");
        }
        NumericalData {
            displayText: qsTr("車両状態");
            displayData: qsTr(driveScene);
            displayUnit: qsTr("状態");
        }
        Text{
            text: qsTr("■分析結果");
            //font.pixelSize: 14
        }
        NumericalData {
            displayText: qsTr("最新の加速シーンの最大値");
            displayData: aTripMax.toString();
            displayUnit: qsTr("m/s²");
        }
        NumericalData {
            displayText: qsTr("これまでのサンプル数");
            displayData: oldTripSamplingNmuK.toString();
            displayUnit: qsTr("個");
        }
        NumericalData {
            displayText: qsTr("今回のサンプル数");
            displayData: thisTripSamplingNmuK.toString();
            displayUnit: qsTr("個");
        }
        NumericalData {
            displayText: qsTr("急加速の割合");
            displayData: suddenAccelRate.toString();
            displayUnit: qsTr("%");
        }
        NumericalData {
            displayText: qsTr("これまでの平均");
            displayData: oldTripAccelAve.toString();
            displayUnit: qsTr("m/s²");
        }
        NumericalData {
            displayText: qsTr("今回の平均");
            displayData: thisTripAccelAve.toString();
            displayUnit: qsTr("m/s²");
        }
        NumericalData {
            displayText: qsTr("分散値"); //標準偏差
            displayData: aTripVariance.toString();
            displayUnit: qsTr("m/s²");
        }
        NumericalData {
            displayText: qsTr("3σ外");
            displayData: aTripOutOf3sigma.toString();
            displayUnit: qsTr("個");
        }
    }


    Item { //For Debug
        visible: true

        Rectangle {
            id: debugBar
            x:0
            y:profile01SuddenAccel.height  - igOff.buttonHeight - 20
            width: profile01SuddenAccel.width
            height: 2
            color: debugbarcolor
        }

        Text {
            x: 0
            y: debugBar.y;
            width:10
            height: 10
            verticalAlignment: Text.Center
            text: qsTr(comDataSize);
            color: "red"
            font.pixelSize: 10
        }

        ButtonWithText{
            id: igOff
            x: profile01SuddenAccel.width - buttonWidth -10
            y: profile01SuddenAccel.height - buttonHeight - 10
            buttonWidth: 100;
            buttonHeight: 40;
            buttonText: qsTr("IG OFF");
            onButtonPressed:{
                brms.switchIG = 0;
                if (igOffId == 0)
                {
                    igOffId = 1;
                    igOff.buttonText = qsTr("IG ON");
                }
                else
                {
                    igOffId = 0;
                    igOff.buttonText = qsTr("IG OFF");
                }
            }
        }
    }

    //--------------------------------------------------
    // function
    //--------------------------------------------------
    Item {
        id: localfunc
        property int i: 0

        function printConsoleLog(msg) {
            console.log(msg);
        }

        function displayComDataSize(size) {
            comDataSize="";
            for(i=0; i<size; i++) {
                comDataSize = comDataSize + "!"
            }
        }

        function displayGraph(state, data, count, caution) {
            //localfunc.printConsoleLog(m_A.toFixed(1));

            switch(data.toFixed(1)) {
            case "0.7":
                graphBar07.update(state, count, caution);
                break;

            case "0.8":
                graphBar08.update(state, count, caution);
                break;

            case "0.9":
                graphBar09.update(state, count, caution);
                break;

            case "1.0":
                graphBar10.update(state, count, caution);
                break;

            case "1.1":
                graphBar11.update(state, count, caution);
                break;

            case "1.2":
                graphBar12.update(state, count, caution);
                break;

            case "1.3":
                graphBar13.update(state, count, caution);
                break;

            case "1.4":
                graphBar14.update(state, count, caution);
                break;

            case "1.5":
                graphBar15.update(state, count, caution);
                break;

            case "1.6":
                 graphBar16.update(state, count, caution);
                 break;

            case "1.7":
                graphBar17.update(state, count, caution);
                break;

            case "1.8":
                graphBar18.update(state, count, caution);
                break;

            case "1.9":
                graphBar19.update(state, count, caution);
                break;

            case "2.0":
                graphBar20.update(state, count, caution);
                break;

            case "2.1":
                graphBar21.update(state, count, caution);
                break;

            case "2.2":
                graphBar22.update(state, count, caution);
                break;

            case "2.3":
                graphBar23.update(state, count, caution);
                break;

            default:
                localfunc.printConsoleLog("data=" + data.toFixed(1) + ": out of range")
                /*
                if(data.toFixed(1)<=0.8) {
                    localfunc.printConsoleLog("data=" + data.toFixed(1) + ": 0.8以下は0.8に表示")
                    graphBar08.update(state, count, caution);
                }else if(2.2 <= data.toFixed(1)){
                    localfunc.printConsoleLog("data=" + data.toFixed(1) + ": 2.2以上は2.2に表示")
                    graphBar22.update(state, count, caution);
                }else{
                    localfunc.printConsoleLog("data=" + data.toFixed(1) + ": Error")
                }
                */
                break;
            }

            return;
        }

        function setGraphDisplayRate(state, count) {
            //localfunc.printConsoleLog("setGraphDisplayRate: called")
            //localfunc.printConsoleLog("state="  + state + ", count=" + count)

            if("old" === state) {
                maxCountNumOldTrip = maxCountNumOldTrip > count ? maxCountNumOldTrip:count;
            }else if ("this" === state) {
                maxCountNumThisTrip = maxCountNumThisTrip > count ? maxCountNumThisTrip:count;
            }
            //localfunc.printConsoleLog("maxCountNumOldTrip=" + maxCountNumOldTrip + ", maxCountNumThisTrip=" + maxCountNumThisTrip);

            graphDisplayRate = ( (maxCountNumOldTrip + maxCountNumThisTrip) - ( (maxCountNumOldTrip + maxCountNumThisTrip)%50 )+50)/500; //50刻みで表示
            //localfunc.printConsoleLog("graphDisplayRate=" + graphDisplayRate);
            //localfunc.printConsoleLog("---");

            //Change the scale
            graphBar07.scaleRate = 1/graphDisplayRate;
            graphBar08.scaleRate = 1/graphDisplayRate;
            graphBar09.scaleRate = 1/graphDisplayRate;
            graphBar10.scaleRate = 1/graphDisplayRate;
            graphBar11.scaleRate = 1/graphDisplayRate;
            graphBar12.scaleRate = 1/graphDisplayRate;
            graphBar13.scaleRate = 1/graphDisplayRate;
            graphBar14.scaleRate = 1/graphDisplayRate;
            graphBar15.scaleRate = 1/graphDisplayRate;
            graphBar16.scaleRate = 1/graphDisplayRate;
            graphBar17.scaleRate = 1/graphDisplayRate;
            graphBar18.scaleRate = 1/graphDisplayRate;
            graphBar19.scaleRate = 1/graphDisplayRate;
            graphBar20.scaleRate = 1/graphDisplayRate;
            graphBar21.scaleRate = 1/graphDisplayRate;
            graphBar22.scaleRate = 1/graphDisplayRate;
            graphBar23.scaleRate = 1/graphDisplayRate;
        }

    }
}
