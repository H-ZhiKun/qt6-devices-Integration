import QtQuick 2.15
import QtQuick.Controls 2.15

Window {
    id: cameraPageWin
    objectName: "cameraPageWin"
    modality: Qt.WindowModal
    //固定窗口大小
    minimumWidth: 500
    maximumWidth: 500
    minimumHeight: 690
    maximumHeight: 690
    color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
    property int numWidth: 0
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
            height: 130
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
                currentIndex: cameraPage.cbxLastIndex
                model: ListModel {
                    id: cameraList
                    //定义这个接口来动态添加下拉内容
                }

                onCurrentValueChanged: {
                    var json = {
                        "sn": currentCamera.currentText
                    };
                    cameraPage.cbxLastIndex = currentCamera.currentIndex;
                    var strSend = JSON.stringify(json);
                    var jsRet = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.GetCameraParam, strSend);
                    var result = JSON.parse(jsRet);
                    if (result.ok === true) {
                        var details = result.details;
                        textWidthMax.text = details.max_width;
                        textHeightMax.text = details.max_height;
                        comboBoxWindow.currentIndex = details.qml_window;
                        comboBoxTrigger.currentIndex = details.trigger_mode;
                        sliderFPS.value = details.fps;
                        sliderExpose.value = details.expose;
                        sliderExpose.to = details.max_expose;
                        sliderGain.value = details.gain;
                        sliderWidth.value = details.width;
                        sliderHeight.value = details.height;
                        sliderOffsetX.value = details.offset_x;
                        sliderOffsetY.value = details.offset_y;
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
                x: 113
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
                objectName: "textHeightMax"
                x: 315
                y: 58
                width: 80
                font.pointSize: 12
            }
        }

        GroupBox {
            x: 15
            y: 150
            width: parent.width - 30
            height: 450
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
                y: 13
                text: "绑定窗口"
                font.pointSize: 12
            }

            ComboBox {
                id: comboBoxWindow
                objectName: "textExposure"
                x: 103
                y: 5
                width: 103
                height: 34
                model: ["窗口一", "窗口二", "窗口三"]
            }

            Text {
                x: 220
                y: 13
                text: "触发模式"
                font.pointSize: 12
            }

            ComboBox {
                id: comboBoxTrigger
                objectName: "textExposure"
                x: 305
                y: 5
                width: 103
                height: 34
                model: ["软触发", "硬触发"]
            }

            Text {
                x: 18
                y: 63
                text: "FPS"
                font.pointSize: 12
            }

            Slider {
                id: sliderFPS
                x: 121
                y: 59
                from: 1
                to: 40
                stepSize: 1
                width: 210
                onValueChanged: {
                    textInputFPS.text = value;
                }
            }

            TextField {
                id: textInputFPS
                x: 340
                y: 63
                width: 70
                height: 30
                text: sliderFPS.value
                font.pointSize: 12
                clip: true
                onEditingFinished: {
                    sliderFPS.value = text;
                }
            }

            Text {
                x: 18
                y: 113
                text: "曝光时间(ms)"
                font.pointSize: 12
            }

            Slider {
                id: sliderExpose
                x: 121
                y: 109
                from: 0.1
                to: 1000
                stepSize: 0.1
                width: 210
                onValueChanged: {
                    // 将值保留两位小数，并赋给textInputExpose的text
                    textInputExpose.text = value.toFixed(1);
                }
            }

            TextField {
                id: textInputExpose
                x: 340
                y: 113
                clip: true
                width: 70
                height: 30
                text: sliderExpose.value
                font.pointSize: 12
                onEditingFinished: {
                    sliderExpose.value = parseFloat(text);
                }
            }

            Text {
                x: 18
                y: 163
                text: "增益Gain(db)"
                font.pointSize: 12
            }

            Slider {
                id: sliderGain
                x: 121
                y: 163
                from: 0
                to: 100
                stepSize: 1
                width: 210
                onValueChanged: {
                    textInputGain.text = value;
                }
            }

            TextField {
                id: textInputGain
                x: 340
                y: 163
                clip: true
                width: 70
                height: 30
                text: sliderGain.value
                font.pointSize: 12
                onEditingFinished: {
                    sliderGain.value = text;
                }
            }

            Text {
                x: 18
                y: 213
                text: "Width"
                font.pointSize: 12
            }

            Slider {
                id: sliderWidth
                x: 121
                y: 209
                from: 16
                to: textHeightMax.text
                stepSize: 16
                width: 210
                onValueChanged: {
                    textInputWidth.text = value;
                    textInputHeight.text = value;
                    sliderHeight.value = value;
                }
            }

            TextField {
                id: textInputWidth
                x: 340
                y: 213
                clip: true
                width: 70
                height: 30
                text: sliderWidth.value
                font.pointSize: 12
                onEditingFinished: {
                    numWidth = Number(text);
                    if (numWidth > Number(textHeightMax.text)) {
                        sliderHeight.value = Number(textHeightMax.text);
                        textInputHeight.text = textHeightMax.text;
                        sliderWidth.value = Number(textHeightMax.text);
                        text = textHeightMax.text;
                    } else {
                        if (numWidth < 16) {
                            sliderWidth.value = 16;
                            textInputHeight.text = "16";
                            sliderHeight.value = 16;
                            text = "16";
                        } else {
                            numWidth = numWidth / 16;
                            numWidth = 16 * numWidth;
                            sliderWidth.value = numWidth;
                            textInputHeight.text = numWidth.toString();
                            sliderHeight.value = numWidth;
                            text = numWidth.toString();
                        }
                    }
                }
            }

            Text {
                x: 18
                y: 263
                text: "Height"
                font.pointSize: 12
            }

            Slider {
                id: sliderHeight
                x: 121
                y: 259
                from: 16
                to: textHeightMax.text
                stepSize: 16
                width: 210
                onValueChanged: {
                    textInputWidth.text = value;
                    textInputHeight.text = value;
                    sliderWidth.value = value;
                }
            }

            TextField {
                id: textInputHeight
                x: 340
                y: 263
                clip: true
                width: 70
                height: 30
                text: sliderHeight.value
                font.pointSize: 12
                onEditingFinished: {
                    numWidth = Number(text);
                    if (numWidth > Number(textHeightMax.text)) {
                        sliderWidth.value = Number(textHeightMax.text);
                        textInputWidth.text = textWidthMax.text;
                        sliderHeight.value = Number(textHeightMax.text);
                        text = textHeightMax.text;
                    } else {
                        if (numWidth < 16) {
                            sliderWidth.value = 16;
                            textInputWidth.text = "16";
                            sliderHeight.value = 16;
                            text = "16";
                        } else {
                            numWidth = numWidth / 16;
                            numWidth = 16 * numWidth;
                            sliderWidth.value = numWidth;
                            textInputWidth.text = numWidth.toString();
                            sliderHeight.value = numWidth;
                            text = numWidth.toString();
                        }
                    }
                }
            }

            Text {
                x: 18
                y: 313
                text: "OffsetX"
                font.pointSize: 12
            }

            Slider {
                id: sliderOffsetX
                x: 121
                y: 309
                from: 1
                to: textWidthMax.text
                stepSize: 1
                width: 210
                onValueChanged: {
                    textInputOffsetX.text = value;
                }
            }

            TextField {
                id: textInputOffsetX
                x: 340
                y: 313
                clip: true
                width: 70
                height: 30
                text: sliderOffsetX.value
                font.pointSize: 12
                onEditingFinished: {
                    sliderOffsetX.value = text;
                }
            }

            Text {
                x: 18
                y: 363
                text: "OffsetY"
                font.pointSize: 12
            }

            Slider {
                id: sliderOffsetY
                x: 121
                y: 359
                from: 1
                to: textHeightMax.text
                stepSize: 1
                width: 210
                onValueChanged: {
                    textInputOffsetY.text = value;
                }
            }

            TextField {
                id: textInputOffsetY
                x: 340
                y: 363
                clip: true
                width: 70
                height: 30
                text: sliderOffsetY.value
                font.pointSize: 12
                onEditingFinished: {
                    sliderOffsetY.value = text;
                }
            }
        }

        Button {
            id: saveCameraSet
            objectName: "saveCameraSet"
            text: "保存设置"
            icon.source: "file:///" + appdir + "/ico/baocun.png"
            x: 353
            y: 620
            font.pointSize: 12
            onClicked: {
                if (comboBoxWindow.currentIndex === -1) {
                    saveText.text = "请绑定窗口！";
                    saveText.color = "red";
                } else {
                    var json = {
                        "sn_num": currentCamera.currentText,
                        "qml_window": comboBoxWindow.currentIndex,
                        "fps": sliderFPS.value,
                        "trigger_mode": comboBoxTrigger.currentIndex,
                        "expose": sliderExpose.value,
                        "gain": sliderGain.value,
                        "width": sliderWidth.value,
                        "height": sliderHeight.value,
                        "offset_x": sliderOffsetX.value,
                        "offset_y": sliderOffsetY.value,
                        "max_width": parseInt(textWidthMax.text),
                        "max_height": parseInt(textHeightMax.text)
                    };

                    // 将json对象转换为JSON字符串
                    var jsonString = JSON.stringify(json);
                    var jsRet = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.SetCameraParam, jsonString);
                    var result = JSON.parse(jsRet);
                    if (result.ok === false) {
                        saveText.text = "修改失败！";
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
    Component.onCompleted: {
        var jsRet = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.GetCameraList, "");
        var result = JSON.parse(jsRet);
        if (result.ok === true) {
            for (var i = 0; i < result.details.cameraId.length; i++) {
                cameraList.append({
                        "text": result.details.cameraId[i]
                    });
            }
        }
    }
}
