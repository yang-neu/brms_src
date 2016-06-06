import QtQuick 2.1
import QtQuick.Window 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.1
import hmiapp 1.0

Window {
    id:mainW;
    visible: true;
    width: 1920;
    height: 720;
    color: "#eeeeee";

//    void speedChanged(double speed);
//    void distanceChanged(double distance);
//    void accelChanged(double accel);
//    void ttcChanged(double ttc);
//    void roadClassSigChanged(QString str);
//    void aChanged(double a);

//    void roadKindChanged(QString str);
//    void frontCarChanged(QString str);
//    void driveSceneChanged(QString str);
//    void suddenAccelChanged(QString str);
//    void suddenAccelStateChanged(QString str);
//    void suddenAccelCountChanged(int count);

    property double speedMax: 250;
    property double aMax: 3;
    property double distanceMax: 255;
    property double accelMax: 100;
    property double ttcMax: 1000

    property double m_Speed: 250;
    property double m_A: 3;
    property double m_A1: 3;
    property double m_Distance: 255;
    property double m_Accel: 1;
    property double m_TTC: 10;
    property string roadClassSig: "NormalRoad(Trunk)";

    property string roadKind:"Normal Road";
    property string frontCar:"TBD";
    property string driveScene:"AddSpeedRunning";

    property string suddenAccel:"Profile1_Text";
    property string suddenAccelState:"StatsNotJudge";
    property int suddenAccelCount:0;

    property string suddenDecel:"Profile2_Text";
    property string suddenDecelState:"StatsNotJudge";
    property int suddenDecelCount:0;

    property string decelStart:"Profile3_Text";
    property string decelStartState:"StatsNotJudge";
    property int decelStartCount:0;

    property string ttcRisk:"Profile4_Text";
    property string ttcRiskState:"StatsNotJudge";
    property int ttcRiskCount:0;

    property string disOnExpressWay:"Profile5_Text";
    property string disOnExpressWayState:"StatsNotJudge";
    property int disOnExpressWayCount:0;

    property string disOnOpenRoad:"Profile6_Text";
    property string disOnOpenRoadState:"StatsNotJudge";
    property int disOnOpenRoadCount:0;

    property string followStop:"Profile7_Text";
    property string followStopState:"StatsNotJudge";
    property int followStopCount:0;

    Connections {
        target: brms;
        onSpeedChanged:m_Speed = speed.toFixed(2);
        onDistanceChanged:m_Distance = distance.toFixed(2);
        onAccelChanged:m_Accel = accel.toFixed(2);
        onAChanged: {
            m_A = a.toFixed(2);
            m_A1 = m_A>0 ? m_A: 0-m_A;
        }
        onTtcChanged: m_TTC = ttc.toFixed(2);
        onRoadClassSigChanged: roadClassSig = str;

        onRoadKindChanged: roadKind = str;
        onFrontCarChanged: frontCar = str;
        onDriveSceneChanged: driveScene = str;

        onSuddenAccelChanged: suddenAccel = str;
        onSuddenAccelStateChanged: suddenAccelState = str;
        onSuddenAccelCountChanged: suddenAccelCount = count;

        onSuddenDecelChanged: suddenDecel = str;
        onSuddenDecelStateChanged: suddenDecelState = str;
        onSuddenDecelCountChanged: suddenDecelCount = count;

        onDecelStartChanged: decelStart = str;
        onDecelStartStateChanged: decelStartState = str;
        onDecelStartCountChanged: decelStartCount = count;

        onTtcRiskChanged: ttcRisk = str;
        onTtcRiskStateChanged: ttcRiskState = str;
        onTtcRiskCountChanged: ttcRiskCount = count;

        onDisOnExpressWayChanged:disOnExpressWay = str;
        onDisOnExpressWayStateChanged:disOnExpressWayState = str;
        onDisOnExpressWayCountChanged:disOnExpressWayCount = count;

        onDisOnOpenRoadChanged:disOnOpenRoad = str;
        onDisOnOpenRoadStateChanged:disOnOpenRoadState = str;
        onDisOnOpenRoadCountChanged:disOnOpenRoadCount = count;

        onFollowStopChanged:followStop = str;
        onFollowStopStateChanged:followStopState = str;
        onFollowStopCountChanged:followStopCount = count;
    }

    Text{
        x: 8
        y: 0
        text: qsTr("CanDate");
        font.pixelSize: 14
    }
    Column {
        x: 8
        y: 21
        height: 300
        spacing: 20;
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
        ProgressBarWithText {
            progressText: qsTr("Distance");
            progressDate: m_Distance.toString();//qsTr("21");
            progressUnit: qsTr("m");
            progressValue: m_Distance;
            progressMinimumValue: 0;
            progressMaximumValue: distanceMax;
        }
        ProgressBarWithText {
            progressText: qsTr("du");
            progressDate: m_Accel.toString();//qsTr("33.0");
            progressUnit: qsTr("%");
            progressValue: m_Accel;
            progressMinimumValue: 0;
            progressMaximumValue: accelMax;
        }
        ProgressBarWithText {
            progressText: qsTr("TTC");
            progressDate: m_TTC.toString();//qsTr("999.0");
            progressUnit: qsTr("sec");
            progressValue: m_TTC;
            progressMinimumValue: 0;
            progressMaximumValue: ttcMax;
        }
        ComplexText {
            complexText: qsTr("RoadKindJudgeSignal");
            complexSubText: qsTr(roadClassSig);//qsTr("NormalRoad(Trunk)");
            complexRectangleWidth: 262;
        }
    }
    Text{
        x: 300
        y: 0
        text: qsTr("CanDateInfo");
        font.pixelSize: 14
    }
    Column {
        x: 300
        y: 71
        height: 300
        spacing: 20;
        ComplexText {
            complexText: qsTr("Run");
            complexSubText: qsTr(driveScene);//qsTr("AddSpeedRunning");
            complexRectangleWidth: 180;
        }
        ComplexText {
            complexText: qsTr("Front Car");
            complexSubText: qsTr(frontCar);//qsTr("TBD");
            complexRectangleWidth: 180;
        }
        ComplexText {
            complexText: qsTr("RunningRoadKind");
            complexSubText: qsTr(roadKind);//qsTr("Normal Road");
            complexRectangleWidth: 180;
        }
    }
    Text{
        x: 580
        y: 0
        text: qsTr("User");
        font.pixelSize: 14
    }
    Column {
        x: 580
        y: 50
        height: 300
        spacing: 20;
        ComplexTextWithOtherText{
            complexTextWT: qsTr("Profile1");
            complexSubTextWT: qsTr(suddenAccel);//qsTr("Profile1_Text");
            complexSubTextWT_Stats: qsTr(suddenAccelState);//qsTr("Different from always");
            complexSubTextWT_Number: suddenAccelCount;//"114";
        }
        ComplexTextWithOtherText{
            complexTextWT: qsTr("Profile2");
            complexSubTextWT: qsTr(suddenDecel);
            complexSubTextWT_Stats: qsTr(suddenDecelState);
            complexSubTextWT_Number: suddenDecelCount;
        }
        ComplexTextWithOtherText{
            complexTextWT: qsTr("Profile3");
            complexSubTextWT: qsTr(decelStart);
            complexSubTextWT_Stats: qsTr(decelStartState);
            complexSubTextWT_Number: decelStartCount;
        }
        ComplexTextWithOtherText{
            complexTextWT: qsTr("Profile4");
            complexSubTextWT: qsTr(ttcRisk);
            complexSubTextWT_Stats: qsTr(ttcRiskState);
            complexSubTextWT_Number: ttcRiskCount;
        }
   	ComplexTextWithOtherText{
            complexTextWT: qsTr("Profile5");
            complexSubTextWT: qsTr(disOnOpenRoad);
            complexSubTextWT_Stats: qsTr(disOnOpenRoadState);
            complexSubTextWT_Number: disOnOpenRoadCount;
        }
        ComplexTextWithOtherText{
            complexTextWT: qsTr("Profile6");
            complexSubTextWT: qsTr(disOnExpressWay);
            complexSubTextWT_Stats: qsTr(disOnExpressWayState);
            complexSubTextWT_Number: disOnExpressWayCount;
        }
     
        ComplexTextWithOtherText{
            complexTextWT: qsTr("Profile7");
            complexSubTextWT: qsTr(followStop);
            complexSubTextWT_Stats: qsTr(followStopState);
            complexSubTextWT_Number: followStopCount;
        }
    }
    Rectangle {
        x:10
        y:486
        width: 1100
        height: 2
        color: "blue"
    }
    Text{
        x: 8
        y: 500
        width: 56
        height: 12
        text: qsTr("Function");
        font.pixelSize: 14
    }
    property int invalidId: 0
    ButtonWithText{
        id: invalid
        x: 8
        y: 550
        buttonWidth: 100;
        buttonHeight: 80;
        buttonText: qsTr("Invalid");
        onButtonPressed:{
            brms.switchInput = 0;
            if (invalidId == 0){
                invalidId = 1;
                invalid.buttonText = qsTr("Valid");
            }
            else{
                invalidId = 0;
                invalid.buttonText = qsTr("Invalid");
            }
        }
    }
    Column {
        x: 200
        y: 500
        height: 300
        spacing: 15;
        ButtonWithTextInput{
            id: speedChange
            buttonWInput_Text: qsTr("speedChange");
            inputText: "0.0";
            textUnit: qsTr("km/h");
            onInputButtonPressed:brms.speedDebug = speedChange.inputText;
        }
        ButtonWithTextInput{
            id: distanceChange
            buttonWInput_Text: qsTr("distanceChange");
            inputText: qsTr("255");
            textUnit: qsTr("m");
            onInputButtonPressed:brms.distanceDebug = distanceChange.inputText;
        }
        ButtonWithTextInput{
            id: duChange
            buttonWInput_Text: qsTr("duChange");
            inputText: qsTr("0");
            textUnit: qsTr("%");
            onInputButtonPressed:brms.accelDebug = duChange.inputText;
        }
        ButtonWithTextInput{
            buttonWInput_Text: qsTr("roadChange");
            onInputButtonPressed:brms.openRoadKindDebug =listView.currentIndex;
        }
    }
    ComboBox {
        x: -1//426
        y: -1//666
        width: 200;
        height: 35
        visible: false
        model: ["National Rd" , "Provincial Rd" , "County Rd"];
        style: ComboBoxStyle {
            dropDownButtonWidth: 6;
            background: Rectangle {
                implicitHeight: 6;
                border.width: 1;
                border.color: "#a8bed4";
                color: "#eeeeee";
                Rectangle{
                    width: 20;
                    height: 35;
                    anchors.right: parent.right;
                    border.width: 1;
                    border.color: "#a8bed4";
                    color: "#e6eef6";
                    Canvas {
                        anchors.fill: parent;
                        onPaint: {
                            var ctx = getContext("2d");
                            ctx.save();
                            ctx.strokeStyle = "black";
                            ctx.fillStyle = "black"
                            ctx.lineWidth = 2;
                            ctx.beginPath();
                            ctx.moveTo(4, 15);
                            ctx.lineTo(10, 21);
                            ctx.lineTo(16, 15);
                            ctx.lineTo(4, 15);
                            ctx.stroke();
                            ctx.restore();
                            ctx.fill();
                        }
                    }
                }
            }
            label:Text {
                anchors.left: parent.left;
                anchors.leftMargin: 2;
                width: parent.width - 22;
                height: parent.height;
                verticalAlignment: Text.AlignVCenter;
                text: control.currentIndex == 0 ? qsTr("National Rd"):
                                                  control.currentIndex == 1 ? qsTr("Provincial Rd"):
                                                                              control.currentIndex == 2 ? qsTr("County Rd"):qsTr("National Rd");
                font.pixelSize: 14
                color: "black";
            }
        }
    }
    Rectangle {
        id:comboBox
        property variant items: [qsTr("National Rd") , qsTr("Provincial Rd") , qsTr("County Rd")]
        property alias selectedItem: chosenItemText.text;
        property alias selectedIndex: listView.currentIndex;
        signal comboClicked;
        x: 426;
        y: 666;
        width: 200;
        height: 35;
        color: "#eeeeee";
        border.color: "#a8bed4";
        smooth:true;
        Rectangle {
            id:chosenItem
            width:parent.width;
            height:comboBox.height;
            smooth:true;
            Text {
                anchors.top: parent.top;
                anchors.left: parent.left;
                anchors.margins: 3;
                id:chosenItemText
                text:comboBox.items[0];
                font.family: "Arial"
                font.pixelSize: 14;
                smooth:true
            }
        }
        Rectangle {
            id:dropDown
            width:20;
            height:comboBox.height
            anchors.left: chosenItem.right;
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#d9e4ef" }
                GradientStop { position: 0.45; color: "#f0f5fa" }
                GradientStop { position: 1.0; color: "#bbd2e7" }
            }
            Canvas {
                anchors.fill: parent;
                onPaint: {
                    var ctx = getContext("2d");
                    ctx.save();
                    ctx.strokeStyle = "black";
                    ctx.fillStyle = "black"
                    ctx.lineWidth = 2;
                    ctx.beginPath();
                    ctx.moveTo(4, 15);
                    ctx.lineTo(10, 21);
                    ctx.lineTo(16, 15);
                    ctx.lineTo(4, 15);
                    ctx.stroke();
                    ctx.restore();
                    ctx.fill();
                }
            }
        }
        MouseArea{
            //anchors.fill: dropDown
            anchors.top: chosenItem.top;
            anchors.left: chosenItem.left;
            height: chosenItem.height;
            width: chosenItem.width + dropDown.width;
            hoverEnabled: true;
            onClicked: {
                listView.visible = true;
            }
        }
        ListView {
            id:listView
            height: 200;
            visible: false;
            width: chosenItem.width + dropDown.width;
            anchors.left: chosenItem.left
            anchors.top: chosenItem.bottom
            model: comboBox.items
            currentIndex: 0
            delegate: Item{
                width:listView.width;
                height: comboBox.height;
                Rectangle {
                    id: mouseMoveHighLight
                    width:listView.width;
                    height:comboBox.height;
                    color: listView.currentIndex == index ? "lightblue" : "white";
                }
                Text {
                    text: modelData
                    anchors.top: parent.top;
                    anchors.left: parent.left;
                    anchors.margins: 5;
                    font.pixelSize: 14;
                }
                MouseArea {
                    anchors.fill: parent;
                    hoverEnabled: true
                    onClicked: {
                        var preSelection = chosenItemText.text
                        chosenItemText.text = modelData
                        if(chosenItemText.text != preSelection){
                            comboBox.comboClicked();
                        }
                        listView.currentIndex = index;
                    }
                    onReleased:listView.visible = false;
                }
            }
        }
    }
    Rectangle {
        x: 600;
        y: 520;
        width: 100;
        height: 100;
        color: "#eeeeee";
        ExclusiveGroup {
            id: radioGroup;
        }
        Component {
            id: radioStyle;
            RadioButtonStyle {
                indicator: Rectangle {
                    implicitWidth: 12;
                    implicitHeight: 12;
                    radius: 10;
                    border.color: control.hovered ? "darkblue" : "gray";
                    border.width: 1;
                    Rectangle {
                        anchors.fill: parent;
                        visible: control.checked;
                        color: "#0000A0";
                        radius: 5;
                        anchors.margins: 3;
                    }
                }
                label: Text {
                    color: control.activeFocus ? "blue" : "black";
                    text: control.text;
                    font.pixelSize: 14
                }
            }
        }
        RadioButton {
            id: radio1;
            text: qsTr("Not Change");
            exclusiveGroup: radioGroup;
            anchors.top: parent.top;
            anchors.left: parent.left;
            checked: true;
            focus: true;
            activeFocusOnPress: true;
            style: radioStyle;
            onClicked:;
        }
        RadioButton {
            id: radio2;
            text: qsTr("Slowly Near");
            exclusiveGroup: radioGroup;
            anchors.top: radio1.bottom;
            anchors.topMargin: 10;
            anchors.left: parent.left;
            activeFocusOnPress: true;
            style: radioStyle;
            onClicked:;
        }
        RadioButton {
            id: radio3;
            text: qsTr("Slowly Away");
            exclusiveGroup: radioGroup;
            anchors.top: radio2.bottom;
            anchors.topMargin: 10;
            anchors.left: parent.left;
            activeFocusOnPress: true;
            style: radioStyle;
            onClicked:;
        }
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
            }
            else
            {
                igOffId = 0;
                igOff.buttonText = qsTr("IG OFF");
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
//        MouseArea {
//            anchors.fill: parent
//            onClicked: {
//                clickNo = clickNo + 1;
//                message = message + ", " + Math.sqrt(clickNo).toString();
//                console.log(message);
//                outputmessage = message;

//                messageLength = outputmessage.length;

//                while (messageLength > 50)
//                {
//                    currentLineMessage = outputmessage.substring(0, 50);
//                    logListView.logOutput(logListView.count + " " + currentLineMessage);

//                    outputmessage = outputmessage.substring(50, messageLength);
//                    messageLength = outputmessage.length;
//                }

//                logListView.logOutput(logListView.count + " " + outputmessage);

//                logListView.positionViewAtEnd();

//            }
//        }
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

}
