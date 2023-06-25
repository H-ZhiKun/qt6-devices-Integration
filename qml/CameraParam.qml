import QtQuick 2.15
import QtQuick.Controls 2.15

Window {

    id: cameraPageWin
    objectName: "cameraPageWin"
    modality: Qt.WindowModal
    //固定窗口大小
    minimumWidth: 500
    maximumWidth: 500
    minimumHeight: 490
    maximumHeight: 490

    title: "相机参数设置"

    Rectangle{
        anchors.fill: parent
        anchors.margins: 10

        GroupBox{
            x: 15
            y: 0
            width: parent.width - 30
            height: 130
            title: "相机选择"
            Text{
                text: "当前相机"
                y: 11
                x: 20
                font.pointSize: 12
            }
            ComboBox {
                id: currentCamera
                objectName: "currentCamera"
                x: 115
                y: 3
                width: 144
                height: 34
            }
            Text {
                x: 18
                y: 58
                text: qsTr("WidthMax：")
                font.pointSize: 12
            }

            Label {
                id: textWidthMax
                objectName: "textWidthMax"
                x: 113
                y: 58
                width: 80
                text: "1080"
                font.pointSize: 12
            }

            Text {
                x: 220
                y: 58
                text: qsTr("HeightMax：")
                font.pointSize: 12
            }

            Label {
                id: textHeightMax
                objectName: "textHeightMax"
                x: 315
                y: 58
                width: 80
                text: "1080"
                font.pointSize: 12
            }
        }

        GroupBox{
            x: 15
            y: 150
            width: parent.width - 30
            height: 260
            title: "参数"

            Text{
                x: 18
                y: 13
                text: "实时FPS"
                font.pointSize: 12
            }

            TextArea {
                id: textFPS
                objectName: "textFPS"
                x: 103
                y: 5
                width: 80
                text: "40"
                font.pointSize: 12
            }

            Text{
                x: 18
                y: 63
                text: "曝光时间"
                font.pointSize: 12
            }

            TextArea {
                id: textExposure
                objectName: "textExposure"
                x: 103
                y: 55
                width: 80
                text: "40"
                font.pointSize: 12
            }

            Text{
                x: 220
                y: 13
                text: "触发模式"
                font.pointSize: 12
            }

            ComboBox {
                id: comboBox
                objectName: "textExposure"
                x: 305
                y: 5
                width: 103
                height: 34
                model:["连续模式", "点动模式"]
            }

            Text{
                x: 220
                y: 63
                text: "增益Gain"
                font.pointSize: 12
            }

            TextArea {
                id: textGain
                objectName: "textGain"
                x: 305
                y: 55
                width: 80
                text: "40"
                font.pointSize: 12
            }

            Text{
                x: 18
                y: 113
                text: "Width"
                font.pointSize: 12
            }

            TextArea {
                id: textWidth
                objectName: "textWidth"
                x: 103
                y: 105
                width: 80
                text: "40"
                font.pointSize: 12
            }

            Text{
                x: 220
                y: 113
                text: "Height"
                font.pointSize: 12
            }

            TextArea {
                id: textHeight
                objectName: "textHeight"
                x: 305
                y: 105
                width: 80
                text: "40"
                font.pointSize: 12
            }

            Text{
                x: 18
                y: 163
                text: "OffsetX"
                font.pointSize: 12
            }

            TextArea {
                id: textOffsetX
                objectName: "textOffsetX"
                x: 103
                y: 155
                width: 80
                text: "40"
                font.pointSize: 12
            }

            Text{
                x: 220
                y: 163
                text: "OffsetY"
                font.pointSize: 12
            }

            TextArea {
                id: textOffsetY
                objectName: "textOffsetY"
                x: 305
                y: 155
                width: 80
                text: "40"
                font.pointSize: 12
            }
        }

        Button{
            id: saveCameraSet
            objectName: "saveCameraSet"
            text: "保存设置"
            icon.source: "file:./ico/baocun.png"
            x: 353
            y: 420
            font.pointSize: 12
        }
    }
}
