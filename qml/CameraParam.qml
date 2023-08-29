import QtQuick 2.15
import QtQuick.Controls 2.15

Window {
    id: cameraPageWin
    objectName: "cameraPageWin"
    modality: Qt.WindowModal
    //固定窗口大小
    minimumWidth: 420
    maximumWidth: 420
    minimumHeight: 690
    maximumHeight: 690
    color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
    property int numWidth: 0
    property int widthIncrement: 1
    property int heightIncrement: 1
    property int offsetxIncrement: 1
    property int offsetyIncrement: 1
    property int maxExpose: 1
    title: "相机参数设置"

    Rectangle {
        anchors.fill: parent
        anchors.margins: 10
        color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
        radius: 10
        GroupBox {
            x: 15
            y: 0
            width: parent.width - 30
            height: 210
            title: "相机选择"
            background: Rectangle {
                anchors.fill: parent
                anchors.topMargin: 30
                border.color: "gray"
                border.width: 1
                radius: 10
                color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
            }
            Text {
                text: "当前相机"
                y: 11
                x: 20
                font.pointSize: 12
            }
            ComboBox {
                id: currentCamera
                editable: true
                objectName: "currentCamera"
                x: 115
                y: 3
                width: 144
                height: 34
                currentIndex: 0
                model: ["定位相机", "打码复核相机", "定位复核相机"]

                onCurrentValueChanged: {
                    var json = {
                        "DisplayWindows": currentCamera.currentIndex
                    };
                    var strSend = JSON.stringify(json);
                    var jsRet = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.GetCameraParam, strSend);
                    var result = JSON.parse(jsRet);
                    if (result.ok === true) {
                        var details = result.details;
                        textWidthMax.text = details.max_width;
                        textHeightMax.text = details.max_height;
                        comboBoxTrigger.currentIndex = details.trigger_mode;
                        widthIncrement = details.width_increment;
                        heightIncrement = details.height_increment;
                        offsetxIncrement = details.offsetx_increment;
                        offsetyIncrement = details.offsety_increment;
                        textInputExpose.text = details.expose;
                        maxExpose = details.max_expose;
                        textInputGain.text = details.gain;
                        textInputWidth.text = details.width;
                        textInputHeight.text = details.height;
                        textInputOffsetX.text = details.offset_x;
                        textInputOffsetY.text = details.offset_y;
                    } else {
                        saveText.text = result.description;
                        saveText.color = "red";
                    }
                }
            }
            Text {
                x: 18
                y: 58
                text: qsTr("WidthMax: ")
                font.pointSize: 12
            }

            Label {
                id: textWidthMax
                objectName: "textWidthMax"
                x: 120
                y: 58
                width: 80
                font.pointSize: 12
            }

            Text {
                x: 220
                y: 58
                text: qsTr("HeightMax: ")
                font.pointSize: 12
            }

            Label {
                id: textHeightMax
                x: 323
                y: 58
                width: 80
                font.pointSize: 12
            }

            Text {
                x: 18
                y: 98
                text: qsTr("Width 步进: ")
                font.pointSize: 12
            }

            Label {
                id: textWidthIncrement
                x: 120
                y: 98
                width: 80
                text: widthIncrement
                font.pointSize: 12
            }

            Text {
                x: 220
                y: 98
                text: qsTr("Height 步进: ")
                font.pointSize: 12
            }

            Label {
                id: textHeightIncrement
                x: 323
                y: 98
                width: 80
                text: heightIncrement
                font.pointSize: 12
            }

            Text {
                x: 18
                y: 138
                text: qsTr("OffsetX 步进: ")
                font.pointSize: 12
            }

            Label {
                id: textOffsetxIncrement
                x: 120
                y: 138
                width: 80
                text: offsetxIncrement
                font.pointSize: 12
            }

            Text {
                x: 220
                y: 138
                text: qsTr("OffsetY 步进: ")
                font.pointSize: 12
            }

            Label {
                id: textOffsetyIncrement
                x: 323
                y: 138
                width: 80
                text: offsetyIncrement
                font.pointSize: 12
            }
        }

        GroupBox {
            x: 15
            y: 230
            width: parent.width - 30
            height: 350
            title: "参数"
            background: Rectangle {
                anchors.fill: parent
                anchors.topMargin: 30
                border.color: "gray"
                border.width: 1
                radius: 10
                color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
            }

            Text {
                x: 18
                y: 10
                text: "触发模式"
                font.pointSize: 12
            }

            ComboBox {
                id: comboBoxTrigger
                objectName: "textExposure"
                x: 215
                y: -3
                width: 105
                height: 34
                model: ["连续采集", "触发采集"]
            }

            Text {
                x: 18
                y: 50
                text: "曝光时间(ms)"
                font.pointSize: 12
            }

            TextField {
                id: textInputExpose
                x: 215
                y: 45
                clip: true
                width: 105
                height: 30
                font.pointSize: 12
            }

            Text {
                x: 18
                y: 90
                text: "增益Gain(db)"
                font.pointSize: 12
            }

            TextField {
                id: textInputGain
                x: 215
                y: 85
                clip: true
                width: 105
                height: 30
                font.pointSize: 12
            }

            Text {
                x: 18
                y: 130
                text: "Width"
                font.pointSize: 12
            }

            TextField {
                id: textInputWidth
                x: 215
                y: 125
                clip: true
                width: 105
                height: 30
                font.pointSize: 12
            }

            Text {
                x: 18
                y: 170
                text: "Height"
                font.pointSize: 12
            }

            TextField {
                id: textInputHeight
                x: 215
                y: 165
                clip: true
                width: 105
                height: 30
                font.pointSize: 12
            }

            Text {
                x: 18
                y: 210
                text: "OffsetX"
                font.pointSize: 12
            }

            TextField {
                id: textInputOffsetX
                x: 215
                y: 205
                clip: true
                width: 105
                height: 30
                font.pointSize: 12
            }

            Text {
                x: 18
                y: 250
                text: "OffsetY"
                font.pointSize: 12
            }

            TextField {
                id: textInputOffsetY
                x: 215
                y: 245
                clip: true
                width: 105
                height: 30
                font.pointSize: 12
            }
        }

        Button {
            id: saveCameraSet
            objectName: "saveCameraSet"
            text: "保存设置"
            icon.source: "file:///" + appdir + "/ico/baocun.png"
            x: 280
            y: 620
            font.pointSize: 12
            height: 40
            onClicked: {
                if (comboBoxWindow.currentIndex === -1) {
                    saveText.text = "请绑定窗口！";
                    saveText.color = "red";
                } else {
                    if (Number(textInputExpose.text) > maxExpose || Number(textInputExpose.text) < 0) {
                        saveText.text = "曝光参数超出阈值！";
                        saveText.color = "red";
                        return;
                    }
                    if (Number(textInputWidth.text) > Number(textWidthMax.text) || Number(textInputWidth.text) < 0) {
                        saveText.text = "宽度参数超出阈值！";
                        saveText.color = "red";
                        return;
                    }
                    if (Number(textInputHeight.text) > Number(textHeightMax.text) || Number(textInputHeight.text) < 0) {
                        saveText.text = "长度参数超出阈值！";
                        saveText.color = "red";
                        return;
                    }
                    if (Number(textInputOffsetX.text) % offsetxIncrement != 0) {
                        saveText.text = "offsetx参数不符合步进！";
                        saveText.color = "red";
                        return;
                    }
                    if (Number(textInputOffsetY.text) % offsetyIncrement != 0) {
                        saveText.text = "offsety参数不符合步进！";
                        saveText.color = "red";
                        return;
                    }
                    var json = {
                        "DisplayWindows": currentCamera.currentIndex,
                        "trigger_mode": comboBoxTrigger.currentIndex,
                        "expose": textInputExpose.text,
                        "gain": textInputGain.text,
                        "width": textInputWidth.text,
                        "height": textInputHeight.text,
                        "offset_x": textInputOffsetX.text,
                        "offset_y": textInputOffsetY.text
                    };

                    // 将json对象转换为JSON字符串
                    var jsonString = JSON.stringify(json);
                    var jsRet = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.SetCameraParam, jsonString);
                    var result = JSON.parse(jsRet);
                    if (result.ok === false) {
                        saveText.text = result.description;
                        saveText.color = "red";
                    } else {
                        saveText.text = "保存成功！";
                        saveText.color = "green";
                    }
                }
            }
        }

        Text {
            id: saveText
            x: 18
            y: 625
            font.pointSize: 11
        }
    }
}
