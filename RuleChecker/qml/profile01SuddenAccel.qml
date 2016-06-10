import QtQuick 2.1
import QtQuick.Window 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.1
import hmiapp 1.0

Window {
    id: profile01SuddenAccel;
    visible: true;
    width: 1920;
    height: 720;
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

    property string suddenAccel:"Profile1_Text";
    property string suddenAccelState:"StatsNotJudge";
    property int suddenAccelCount:0;

    Component.onCompleted: {
        //Nothing to do
        suddenAccelBar.x = (17 * (graphBar01.width + verticalAxis.spacing)) + 32; //1.7以上は急加速
        suddenAccelBar.y = -15;
    }

    Connections {
        target: brms;
        onSpeedChanged:m_Speed = speed.toFixed(2);
        onAChanged: {
            m_A = a.toFixed(2);
            m_A1 = m_A>0 ? m_A: 0-m_A;
        }
        onSuddenAccelChanged: suddenAccel = str;
        onSuddenAccelStateChanged: suddenAccelState = str;
        onSuddenAccelCountChanged: suddenAccelCount = count;
        onAccelInfoChanged: {
            //localfunc.printConsoleLog("accel=" + data + ", count=" + count);
            localfunc.displayGraph(state, data, count, caution);
        }

    }

    //--------------------------------------------------
    // 画面
    //--------------------------------------------------
    Column {
        x: 8
        y: 0
        height: 300
        spacing: 20;

        Text{
            text: qsTr("■入力情報");
            font.pixelSize: 14
        }
        ProgressBarWithText {
            progressText: qsTr("Speed");
            progressDate: m_Speed.toString();//qsTr("52.39");
            progressUnit: qsTr("km/h");
            progressValue: m_Speed;
            progressMinimumValue: 0;
            progressMaximumValue: speedMax;
        }
        ProgressBarWithText {
            progressText: qsTr("a");
            progressDate: m_A.toString();//qsTr("0.81");
            progressUnit: qsTr("m/s²");
            progressValue: m_A1;
            progressMinimumValue: 0.0;
            progressMaximumValue: 9.0;
        }

        Text{
            text: qsTr("■分析結果");
            font.pixelSize: 14
        }
        ProgressBarWithText {
            progressText: qsTr("加速シーンの最大値");
            progressDate: aTripMax.toString();
            progressUnit: qsTr("m/s²");
            progressValue: aTripMax;
            progressMinimumValue: 0;
            progressMaximumValue: speedMax;
        }
        Text {
            text: qsTr("これまでのサンプル数");
            font.pixelSize: 14
            Text {
                x: 159
                y: 0;
                text: samplingNmuK.toString();
                font.pixelSize: 14
            }
            Text {
                x: 238
                y: 0
                text: qsTr("個");
                font.pixelSize: 14
            }
        }
        Text {
            text: qsTr("今回のサンプル数");
            font.pixelSize: 14
            Text {
                x: 159
                y: 0;
                text: samplingNmuK.toString();
                font.pixelSize: 14
            }
            Text {
                x: 238
                y: 0
                text: qsTr("個");
                font.pixelSize: 14
            }
        }
        ProgressBarWithText {
            progressText: qsTr("急加速の割合");
            progressDate: (0).toString();
            progressUnit: qsTr("%");
            progressValue: 0;
            progressMinimumValue: 0;
            progressMaximumValue: 500;
        }
        ProgressBarWithText {
            progressText: qsTr("これまでの平均");
            progressDate: aTripAve.toString();
            progressUnit: qsTr("m/s²");
            progressValue: aTripAve;
            progressMinimumValue: 0;
            progressMaximumValue: 500;
        }
        ProgressBarWithText {
            progressText: qsTr("今回の平均");
            progressDate: aTripAve.toString();
            progressUnit: qsTr("m/s²");
            progressValue: aTripAve;
            progressMinimumValue: 0;
            progressMaximumValue: 500;
        }
        Text {
            text: qsTr("分算値");
            font.pixelSize: 14
            Text {
                x: 159
                y: 0;
                text: aTripVariance.toString();
                font.pixelSize: 14
            }
            Text {
                x: 238
                y: 0
                text: qsTr("(m/s²)²");
                font.pixelSize: 14
            }
        }
        Text {
            text: qsTr("3σ外");
            font.pixelSize: 14
            Text {
                x: 159
                y: 0;
                text: (0).toString();
                font.pixelSize: 14
            }
            Text {
                x: 238
                y: 0
                text: qsTr("個");
                font.pixelSize: 14
            }
        }
    }

    Column {
        x: 290;
        y: 0;
        height: 300;
        spacing: 20;

        Text{
            text: qsTr("プロファイル１　急加速診断");
            font.pixelSize: 14;
        }
        ComplexTextWithOtherText{
            complexTextWT: qsTr("Profile1 急加速");
            complexSubTextWT: qsTr(suddenAccel);//qsTr("Profile1_Text");
            complexSubTextWT_Stats: qsTr(suddenAccelState);//qsTr("Different from always");
            complexSubTextWT_Number: suddenAccelCount;//"114";
        }
    }

    Rectangle{
        id: graph01
        x:290;
        y:120;

        Text{
            x:0;
            y:0
            text: qsTr("■Graph");
        }

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
                    GraphBar {id: graphBar01; text: qsTr("0.1") }
                    GraphBar {id: graphBar02; text: qsTr("0.2") }
                    GraphBar {id: graphBar03; text: qsTr("0.3") }
                    GraphBar {id: graphBar04; text: qsTr("0.4") }
                    GraphBar {id: graphBar05; text: qsTr("0.5") }
                    GraphBar {id: graphBar06; text: qsTr("0.6") }
                    GraphBar {id: graphBar07; text: qsTr("0.7") }
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
            Rectangle {
                //anchors.bottom: parent.top
                id: suddenAccelBar
                width: 3
                height: 350
                color: "purple"
                Text {
                    x:10
                    text: qsTr("急加速領域")
                    color: parent.color
                }
            }

        }
    }

    Rectangle {
        x:10
        y:516
        width: 1100
        height: 2
        color: "blue"
    }

    Text{
        x: 820
        y: 635
        width: 56
        height: 12
        text: qsTr("Save");
        font.pixelSize: 14
    }
    property int igOffId: 0
    ButtonWithText{
        id: igOff
        x: 820
        y: 660
        buttonWidth: 100;
        buttonHeight: 40;
        buttonText: qsTr("IG OFF");
        onButtonPressed:{
            brms.switchIG = 0;
            if (igOffId == 0)
            {
                igOffId = 1;
                igOff.buttonText = qsTr("IG ON");
                localfunc.saveTripInfo();
            }
            else
            {
                igOffId = 0;
                igOff.buttonText = qsTr("IG OFF");
                localfunc.loadOldTripsInfo();
            }
        }
    }
    Rectangle{
        id: listViewBackground
        x: 1150;
        y: 50;
        width: 730;
        height: 650;
        color:"white";
        border.color: "#a8bed4";
        visible: true;
    }

    property int clickNo: 0;
    property string message: "";
    property string outputmessage: "";
    property string currentLineMessage: "";
    property double messageLength: 0;
    property double m_PositionX : 0
    ListView {
        id: logListView;

        anchors.fill: listViewBackground;
        anchors.topMargin: 20;
        anchors.bottomMargin: 40;
        z: 999

        clip: true;
        interactive: false;
        model: debugsModel
        delegate: Component {
            Text {
                anchors.leftMargin: 4;
                text: textOutput;
                font.pixelSize: 14;
                color: "black"
            }
        }
        ListModel { id: debugsModel }
        Connections {
            target: brms;
            onUpdateMessage:logListView.logOutput(str)
        }
        function logOutput(message) {
            debugsModel.append( { textOutput: message} );
            logListView.positionViewAtEnd();
        }
    }
    FlickableScrollBar  {
        id:flickableScrollBarVertical
        scrollArea: logListView;
        orientation: Qt.Vertical;
        height: logListView.height;
        width: 20;
        anchors.left: logListView.right;
        anchors.top: logListView.top;
    }
    FlickableScrollBar  {
        id:flickableScrollBarHorizontal
        scrollArea: logListView;
        orientation: Qt.Horizontal;
        height: 20;
        width: logListView.width;
        anchors.left: logListView.left;
        anchors.top: logListView.bottom;
        anchors.topMargin: 20;
    }
    Rectangle {
        id:up
        width:flickableScrollBarVertical.width;
        height:20;
        anchors.left: flickableScrollBarVertical.left;
        anchors.bottom: flickableScrollBarVertical.top;
        color:"#eeeeee"
        border.color: "#a8bed4";
        Canvas {
            anchors.fill: parent;
            onPaint: {
                var ctx = getContext("2d");
                ctx.save();
                ctx.strokeStyle = "black";
                ctx.fillStyle = "black"
                ctx.lineWidth = 1;
                ctx.beginPath();
                ctx.moveTo(10, 6);
                ctx.lineTo(5, 14);
                ctx.lineTo(15, 14);
                ctx.lineTo(10, 6);
                ctx.stroke();
                ctx.restore();
                ctx.fill();
            }
        }
    }
    Rectangle {
        id:down
        width:flickableScrollBarVertical.width;
        height:20;
        anchors.left: flickableScrollBarVertical.left;
        anchors.top: flickableScrollBarVertical.bottom;
        color:"#eeeeee"
        border.color: "#a8bed4";
        Canvas {
            anchors.fill: parent;
            onPaint: {
                var ctx = getContext("2d");
                ctx.save();
                ctx.strokeStyle = "black";
                ctx.fillStyle = "black"
                ctx.lineWidth = 1;
                ctx.beginPath();
                ctx.moveTo(5, 6);
                ctx.lineTo(10, 14);
                ctx.lineTo(15, 6);
                ctx.lineTo(5, 6);
                ctx.stroke();
                ctx.restore();
                ctx.fill();
            }
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
