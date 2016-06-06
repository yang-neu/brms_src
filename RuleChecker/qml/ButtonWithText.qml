import QtQuick 2.0
Item{
    property int buttonWidth: 100;
    property int buttonHeight: 80;
    property string buttonText: ""
    signal buttonPressed;
    MouseArea{
        id:fun;
        width: buttonWidth;
        height: buttonHeight;
        Rectangle{
            id:colorRect;
            anchors.fill: parent
            border.color: "#a8bed4";
            //color: "#e6eef6";
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#d9e4ef" }
                GradientStop { position: 0.45; color: "#f0f5fa" }
                GradientStop { position: 1.0; color: "#bbd2e7" }
            }
        }
        Text {
            id: text;
            text: buttonText;
            anchors.fill: parent;
            verticalAlignment: Text.AlignVCenter;
            horizontalAlignment: Text.AlignHCenter;
            font.pixelSize: 20
        }
        focus: false;
        onPressed: {
            fun.focus = true;
            buttonPressed();
        }
        onReleased: {
            fun.focus = false;
        }
        onFocusChanged:{
            if (focus == true){
                text.font.bold=true;
                colorRect.color = "#d3d3d3"
            }
            else{
                text.font.bold=false;
                colorRect.color = "#e6eef6";
            }
        }
    }
}
