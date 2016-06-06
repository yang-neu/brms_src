import QtQuick 2.0

Item{
    property string buttonWInput_Text: ""
    property string inputText: ""
    property string textUnit: ""
    signal inputButtonPressed;
    width: 400;
    height: 40;
    ButtonWithText{
        buttonWidth: 200;
        buttonHeight: 40;
        buttonText: buttonWInput_Text;
        onButtonPressed:inputButtonPressed();
    }
    Rectangle{
        x: 229
        y: 8
        width: 70
        height: 28
        color: "white"
    }

    TextInput {
        x: 229
        y: 8
        width: 70
        height: 28
        text: inputText
        font.pixelSize: 14;
        focus:true;
    }
    Text{
        x: 318
        y: 14
        width: 42;
        height: 17;
        text:textUnit;
        font.pixelSize: 14
    }
}
