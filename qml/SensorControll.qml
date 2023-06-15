import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window
Window {
    id: sensorControll
    objectName: "sensorControll"
    property int name
    modality: Qt.WindowModal
    //固定窗口大小
    minimumWidth: 500
    maximumWidth: 500
    minimumHeight: 300
    maximumHeight: 300

    visible: true
    title: "传感器" + name
    Button {
        id: buttonStart
        objectName: "buttonStart"
        x: 224
        y: 201
        width: 120
        height: 65
        text: qsTr("       启动")
        font.pointSize: 16
        Image {
            width: 40
            height: 40
            y:14
            x:5
            source: "file:./ico/14.png"
            fillMode: Image.PreserveAspectFit
        }
    }

    /*关闭按钮*/
    Button {
        id: buttonClose
        objectName: "buttonClose"
        x: 359
        y: 201
        width: 120
        height: 65
        text: qsTr("       关闭")
        font.pointSize: 16
        Image {
            width: 40
            height: 40
            y:14
            x:5
            source: "file:./ico/13.png"
            fillMode: Image.PreserveAspectFit
        }
    }

    Switch {
        id: switchAuto
        objectName: "switchAuto"
        x: 80
        y: 200
        Image {
            width: 40
            height: 40
            y:6
            x:60
            source: "file:./ico/b10.png"
            fillMode: Image.PreserveAspectFit
        }
    }

    Text {
        x:switchAuto.x-38
        y:switchAuto.y+50
        text:qsTr("手动")
        font.pixelSize: 20
    }

    Image {
        width: 40
        height: 40
        x:switchAuto.x-38
        y:switchAuto.y+7
        source: "file:./ico/ico20170422/2.png"
        fillMode: Image.PreserveAspectFit
    }

    Text {
        x:switchAuto.x+58
        y:switchAuto.y+50
        text:qsTr("自动")
        font.pixelSize: 20
    }

    Text {
        id: textState
        x: 121
        y: 76
        text: qsTr("当前状态：")
        font.pixelSize: 30
    }

    MouseArea{
        anchors.fill: sensorControllWin
        onClicked: close();
    }

    Text {
        id: text1
        x: 282
        y: 76
        color: "#0eed55"
        text: qsTr("开启")
        font.pixelSize: 30
    }

}
