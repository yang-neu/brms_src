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

    property double speedMax: 250;
    property double aMax: 3;

    property int samplingNmuK: 0;
    property double aTripMax: 0;
    property double aTripAve: 0;
    property double aTripAve2: 0;
    property double aTripVariance: 0;

    property double m_Speed: 250;
    property double m_A: 3;
    property double m_A1: 3;


    Component.onCompleted: {
        //Nothing to do
    }

    Connections {
        target: brms;
        onSpeedChanged:m_Speed = speed.toFixed(2);
        onAChanged: {
            m_A = a.toFixed(2);
            m_A1 = m_A>0 ? m_A: 0-m_A;
        }
        onAccelInfoChanged: {
            //localfunc.printConsoleLog("accel=" + data + ", count=" + count);
            localfunc.displayGraph(state, data, count, caution);
        }

    }

    //--------------------------------------------------
    // 画面
    //--------------------------------------------------
    Rectangle{
        id: graph01
        x:0;
        y:0;

        Column {
            x:10;
            y:20;
            spacing: 32;

            GraphAxis{ text: qsTr("300 ") }
            GraphAxis{ text: qsTr("250 ") }
            GraphAxis{ text: qsTr("200 ") }
            GraphAxis{ text: qsTr("150 ") }
            GraphAxis{ text: qsTr("100 ") }
            GraphAxis{ text: qsTr(" 50 ") }
            GraphAxis{ text: qsTr("  0 ")
                Row {
                    id: verticalAxis
                    x: 40;
                    y: 20;
                    spacing: 10;
                    //GraphBar {id: graphBar01; text: qsTr("0.1") }
                    //GraphBar {id: graphBar02; text: qsTr("0.2") }
                    //GraphBar {id: graphBar03; text: qsTr("0.3") }
                    //GraphBar {id: graphBar04; text: qsTr("0.4") }
                    //GraphBar {id: graphBar05; text: qsTr("0.5") }
                    //GraphBar {id: graphBar06; text: qsTr("0.6") }
                    //GraphBar {id: graphBar07; text: qsTr("0.7") }
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
                }
            }
        }

        Rectangle {
            id: suddenAccelBar
            x : ((17-7) * (graphBar17.width + verticalAxis.spacing)) + 45; //1.7以上は急加速
            y : 15;
            width: 3
            height: 350
            color: "purple"
            Text {
                x:10
                text: qsTr("急加速領域")
                color: parent.color
            }
        }
        Rectangle {
            id: standardRange
            x : ((9-7) * (graphBar09.width + verticalAxis.spacing)) + 5; //0.9~1.4
            y : 360;
            width: ((14-9+1) * (graphBar09.width + verticalAxis.spacing)) + 5 //0.9~1.4
            height: 29
            border.width: 2
            border.color: "green"
            color: "transparent"
            radius: 5
        }
    }

    ColumnLayout {
        x: 50
        y: 440
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
                text: qsTr("今回でのドライブのデータ")
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

    ColumnLayout {
        x: 700
        y: 0
        spacing: 20;

        ColumnLayout {
            Text{
                text: qsTr("■診断結果");
                font.pixelSize: 14
            }
            Rectangle {
                width: 300
                height: 100
                border.width: 3
                border.color: profile01SuddenAccel.color // or "blue"
                color: "transparent"
                radius: 10

                Image {
                    source: "res/state_SuddenAccel.png"

                    Text {
                       anchors.left: parent.right
                       anchors.bottom: parent.bottom
                       text: qsTr("急加速しがち")
                       color: "red"
                    }
                }
            }
            Rectangle {
                width: 300
                height: 100
                border.width: 3
                border.color: "blue" //or profile01SuddenAccel.color
                color: "transparent"
                radius: 10

                Image {
                    source: "res/state_NormalAccel.png"

                    Text {
                       anchors.left: parent.right
                       anchors.bottom: parent.bottom
                       text: qsTr("急加速しない")
                       color: "red"
                    }
                }
            }
        }

        Text{
            text: qsTr("■入力情報");
            font.pixelSize: 14
        }
        NumericalData {
            displayText: qsTr("速度");
            displayData: m_Speed.toString();
            displayUnit: qsTr("km/h");
        }
        NumericalData {
            displayText: qsTr("車両状態");
            displayData: (0).toString();
            displayUnit: qsTr("");
        }
        Text{
            text: qsTr("■分析結果");
            font.pixelSize: 14
        }
        NumericalData {
            displayText: qsTr("加速シーンの最大値");
            displayData: aTripMax.toString();
            displayUnit: qsTr("m/s²");
        }
        NumericalData {
            displayText: qsTr("これまでのサンプル数");
            displayData: samplingNmuK.toString();
            displayUnit: qsTr("個");
        }
        NumericalData {
            displayText: qsTr("今回のサンプル数");
            displayData: samplingNmuK.toString();
            displayUnit: qsTr("個");
        }
        NumericalData {
            displayText: qsTr("急加速の割合");
            displayData: (0).toString();
            displayUnit: qsTr("%");
        }
        NumericalData {
            displayText: qsTr("これまでの平均");
            displayData: aTripAve.toString();
            displayUnit: qsTr("m/s²");
        }
        NumericalData {
            displayText: qsTr("今回の平均");
            displayData: aTripAve.toString();
            displayUnit: qsTr("m/s²");
        }
        NumericalData {
            displayText: qsTr("分算値");
            displayData: aTripVariance.toString();
            displayUnit: qsTr("(m/s²)²");
        }
        NumericalData {
            displayText: qsTr("3σ外");
            displayData: (0).toString();
            displayUnit: qsTr("個");
        }
    }

    //--------------------------------------------------
    // function
    //--------------------------------------------------
    Item {
        id: localfunc

        function printConsoleLog(msg) {
            console.log(msg);
        }

        function displayGraph(state, data, count, caution) {
            //localfunc.printConsoleLog(m_A.toFixed(1));

            switch(data.toFixed(1)) {
            case "0.1":
                graphBar01.update(state, count, caution);
                break;

            case "0.2":
                graphBar02.update(state, count, caution);
                break;

            case "0.3":
                graphBar03.update(state, count, caution);
                break;

            case "0.4":
                graphBar04.update(state, count, caution);
                break;

            case "0.5":
                graphBar05.update(state, count, caution);
                break;

            case "0.6":
                 graphBar06.update(state, count, caution);
                 break;

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

            default:
                break;
            }

            return;
        }

    }
}
