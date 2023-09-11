import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQml.Models 2.15

GroupBox {
    id: formulaControllWin

    /*
    modality: Qt.WindowModal
    //固定窗口大小
    minimumWidth: 780
    maximumWidth: 780
    minimumHeight: 600
    maximumHeight: 600
    visible: true
    */
    width: 1110  //Screen.desktopAvailableWidth
    height: 640  //Screen.desktopAvailableHeight

    objectName: "formulaControllWin"
    background: Rectangle {
        anchors.fill: parent
        border.color: "gray"
        border.width: 1
        radius: 10
        color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
    }
    // 和listview当前索引绑定
    property string curFormulaName: formulaListView.currentItem === null ? "" : formulaListView.currentItem.myData.formulaName  // 当前配方名字
    property string downloadName: ""
    property int modifyFlag: 0
    Connections {
        target: appMetaFlash // C++ 对象实例
        function onPageFormulaChange(value) {
            // 执行其他操作...
            var jsonData = JSON.parse(value);
            formulaListModel.insert(0, {
                    "formulaName": jsonData.name
                });
            formulaListView.currentIndex = 0;
            xPositionInput.text = jsonData.code_x_position;
            yPositionInput.text = jsonData.code_y_position;
            impurityPositionSiteInput.text = jsonData.impurity_locate;
            deviceProduceSpeedInput.text = jsonData.speed_produce;
            deviceproduceAccelerationInput.text = jsonData.acceleration_produce;
            deviceproduceDecelerationInput.text = jsonData.deceleration_produce;
            buttonModifyFormula.enabled = true;
        }
    }
    // 左侧配方选择页面
    GroupBox {
        id: formulaSelect
        objectName: "formulaSelect"
        y: 20
        x: 100
        width: 250
        height: 550
        title: "配方选择"
        font.pointSize: 20

        background: Rectangle {
            anchors.fill: parent
            border.color: "gray"
            border.width: 1
            radius: 10
            anchors.topMargin: 50
            color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
        }

        // 选择model
        ListModel {
            id: formulaListModel
            objectName: "formulaListModel"
            //ListElement { formulaName: "0" }
        }

        ListView {
            id: formulaListView
            // 布局
            anchors.fill: parent
            anchors.topMargin: 10
            objectName: "formulaListView"
            clip: true  // 超出view部分裁剪
            focus: true
            property int newIndex: -1

            model: formulaListModel

            // 单个item
            delegate: ItemDelegate {
                //highlighted: formulaListView.isCurrentItem  // 移入高亮
                property variant myData: model  // 起一个别名，外部才能和内部变量绑定
                // 大小
                height: 30
                width: 220
                background: Rectangle {
                    id: sensorRec
                    objectName: "sensorRec"
                    anchors.fill: parent
                    height: 30
                    y: 0
                    radius: 10

                    Text {
                        text: formulaName
                        font.pointSize: 11
                        y: 5
                        x: 8
                    }

                    // 选中变色
                    //border.color: Qt.lighter(color, 1.1)
                    border.width: 2
                    border.color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
                    color: formulaListView.currentIndex == index ? "lightblue" : Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
                }

                // 鼠标点击区域
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        formulaListView.currentIndex = index;  // 更换当前索引
                        buttonModifyFormula.enabled = true;
                        var json = {
                            "name": curFormulaName
                        };
                        var strSend = JSON.stringify(json);
                        var jsRet = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.SelectFormula, strSend);
                        var result = JSON.parse(jsRet);
                        if (result.ok === true) {
                            xPositionInput.text = result.details.code_x_position;
                            yPositionInput.text = result.details.code_y_position;
                            impurityPositionSiteInput.text = result.details.impurity_locate;
                            deviceProduceSpeedInput.text = result.details.speed_produce;
                            deviceproduceAccelerationInput.text = result.details.acceleration_produce;
                            deviceproduceDecelerationInput.text = result.details.deceleration_produce;
                        }

                        // 配方状态
                        formulaDisplay.downloadState = downloadName === curFormulaName ? "已下载" : "未下载";
                        saveText.text = "";
                    }
                    onEntered: {
                        sensorRec.border.color = "lightblue";
                    }
                    onExited: {
                        sensorRec.border.color = Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1);
                    }
                }
            }
        }
    }

    // 中间配方信息相关页面
    GroupBox {
        id: formulaDisplay
        objectName: "formulaDisplay"
        width: 310
        height: 550
        x: 405
        y: 20
        title: "配方信息"
        font.pointSize: 20
        background: Rectangle {
            anchors.fill: parent
            border.color: "gray"
            border.width: 1
            radius: 10
            anchors.topMargin: 50
            color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
        }
        //property string formulaName: "默认配方"
        property int codingXPosition: 0  // 打码机x轴电机位置
        property int codingYPosition: 0  // 打码机Y轴位置
        property int produceSpeed: 0  // 生产速度
        property int produceAcceleration: 0  // 生产加速度
        property int produceDeceleration: 0  // 生产降速度
        property int impurityPosition: 0  // 杂质位置
        property alias downloadState: downloadStateText.text

        Text {
            text: "配方名称："
            font.pointSize: 11
        }

        Text {
            text: "当前状态："
            font.pointSize: 11
            y: 28
        }

        Text {
            id: downloadStateText
            text: "未下载"
            font.pointSize: 11
            y: 28
            x: 73
        }

        // 配方名字
        Rectangle {
            id: formulaName
            x: 73
            height: 25
            width: 180
            objectName: "formulaName"
            color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
            TextInput {
                id: formulaNameInput
                width: parent.width - 2
                anchors.topMargin: 2
                height: 23
                x: 0
                y: 0
                objectName: "formulaNameInput"
                text: curFormulaName
                font.pointSize: 11
                readOnly: true
                clip: true
                onTextChanged: {
                    if (formulaControllWin.modifyFlag === 2) {
                        var json = {
                            "name": curFormulaName
                        };
                        var strSend = JSON.stringify(json);
                        var jsRet = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.SelectFormula, strSend);
                        var result = JSON.parse(jsRet);
                        if (result.ok === true) {
                            xPositionInput.text = result.code_x_position;
                            yPositionInput.text = result.code_y_position;
                            impurityPositionSiteInput.text = result.impurity_locate;
                            var impurity_locate = result.impurity_locate;
                            deviceProduceSpeedInput.text = result.speed_produce;
                            deviceproduceAccelerationInput.text = result.acceleration_produce;
                            deviceproduceDecelerationInput.text = result.deceleration_produce;
                        }
                        // 配方状态
                        formulaDisplay.downloadState = downloadName === curFormulaName ? "已下载" : "未下载";
                        formulaControllWin.modifyFlag = 0;
                    }
                    if (formulaControllWin.modifyFlag === 1) {
                        formulaControllWin.modifyFlag = 2;
                    }
                }
            }
        }

        // 打码机电机位置
        GroupBox {
            width: 280
            height: 118
            y: 65
            title: "电机位置"
            font.pointSize: 11

            Text {
                text: "打码机X轴电机位置："
                font.pointSize: 11
                y: 8
            }

            Text {
                text: "打码机Y轴电机位置："
                font.pointSize: 11
                y: 38
            }

            // 打码机X轴电机位置
            Rectangle {
                id: xPosition
                y: 8
                x: 140
                height: 25
                width: 70
                objectName: "xPosition"
                color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
                TextInput {
                    id: xPositionInput
                    width: parent.width - 4
                    anchors.topMargin: 2
                    anchors.fill: parent
                    anchors.leftMargin: 4
                    objectName: "xPositionInput"
                    text: "0"
                    font.pointSize: 11
                    validator: RegularExpressionValidator {
                        regularExpression: /^[+]?[0-9]+\.[0-9]{2}$/
                    }
                    readOnly: true
                    clip: true
                }
            }

            // 打码机Y轴电机位置
            Rectangle {
                id: yPosition
                objectName: "yPosition"
                y: 38
                x: 140
                height: 25
                width: 70
                color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
                TextInput {
                    id: yPositionInput
                    width: parent.width - 4
                    anchors.topMargin: 2
                    anchors.fill: parent
                    anchors.leftMargin: 4
                    objectName: "yPositionInput"
                    text: "0"
                    validator: RegularExpressionValidator {
                        regularExpression: /^[+]?[0-9]+\.[0-9]{2}$/
                    }
                    font.pointSize: 11
                    readOnly: true
                    clip: true
                }
            }

            Text {
                text: " 毫米"
                font.pointSize: 11
                y: 8
                x: 212
            }
            Text {
                text: " 毫米"
                font.pointSize: 11
                y: 38
                x: 212
            }
        }

        // 生产速度
        GroupBox {
            width: 280
            height: 125
            y: 285
            x: 0
            title: "生产速度"
            font.pointSize: 11

            Text {
                text: "设备生产速度："
                font.pointSize: 11
            }
            Text {
                text: "设备生产加速速度："
                font.pointSize: 11
                y: 25
            }
            Text {
                text: "设备生产降速速度："
                font.pointSize: 11
                y: 50
            }

            // 设备生产速度
            Rectangle {
                id: deviceProduceSpeed
                x: 130
                height: 25
                width: 70
                objectName: "deviceProduceSpeed"
                color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
                TextInput {
                    id: deviceProduceSpeedInput
                    width: parent.width - 4
                    anchors.fill: parent
                    anchors.topMargin: 2
                    anchors.leftMargin: 4
                    objectName: "deviceProduceSpeedInput"
                    validator: RegularExpressionValidator {
                        regularExpression: /^[+]?[0-9]+\.[0-9]{2}$/
                    }
                    font.pointSize: 11
                    readOnly: true
                    clip: true
                }
            }

            // 设备生产加速速度
            Rectangle {
                id: deviceproduceAcceleration
                x: 130
                y: 25
                height: 25
                width: 70
                objectName: "deviceproduceAcceleration"
                color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
                TextInput {
                    id: deviceproduceAccelerationInput
                    width: parent.width - 4
                    anchors.topMargin: 2
                    objectName: "deviceproduceAccelerationInput"
                    anchors.fill: parent
                    anchors.leftMargin: 4
                    validator: RegularExpressionValidator {
                        regularExpression: /^[+]?[0-9]+\.[0-9]{2}$/
                    }
                    font.pointSize: 11
                    readOnly: true
                    clip: true
                }
            }

            // 设备生产降速速度
            Rectangle {
                id: deviceproduceDeceleration
                x: 130
                y: 50
                height: 25
                width: 70
                objectName: "deviceproduceDeceleration"
                color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
                TextInput {
                    id: deviceproduceDecelerationInput
                    width: parent.width - 4
                    anchors.topMargin: 2
                    objectName: "deviceproduceDecelerationInput"
                    anchors.fill: parent
                    anchors.leftMargin: 4
                    text: "0"
                    validator: RegularExpressionValidator {
                        regularExpression: /^[+]?[0-9]+\.[0-9]{2}$/
                    }
                    font.pointSize: 11
                    readOnly: true
                    clip: true
                }
            }

            Text {
                text: "瓶/每秒"
                font.pointSize: 11
                x: 205
            }
            Text {
                text: "瓶/每秒"
                font.pointSize: 11
                y: 25
                x: 205
            }
            Text {
                text: "瓶/每秒"
                font.pointSize: 11
                y: 50
                x: 205
            }
        }

        //杂质定位
        GroupBox {
            width: 280
            height: 70
            y: 200
            title: "杂质定位"
            font.pointSize: 11

            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: "杂质定位位置："
                font.pointSize: 11
                y: 8
            }

            // 杂质定位位置
            Rectangle {
                id: impurityPositionSite
                y: -5
                x: 100
                height: 25
                width: 70
                objectName: "impurityPositionSite"
                color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
                TextInput {
                    id: impurityPositionSiteInput
                    width: parent.width - 4
                    anchors.fill: parent
                    anchors.topMargin: 2
                    anchors.leftMargin: 4
                    text: "0"
                    validator: RegularExpressionValidator {
                        regularExpression: /^[+]?[0-9]+\.[0-9]{2}$/
                    }
                    font.pointSize: 11
                    readOnly: true
                    clip: true
                }
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: "毫米"
                font.pointSize: 11
                y: 8
                x: 174
            }
        }
    }

    // 右边配方文件相关页面
    GroupBox {
        id: formulaControll
        objectName: "formulaControll"
        width: 180
        height: 550
        x: 770
        y: 20
        title: "配方配置"
        font.pointSize: 20
        background: Rectangle {
            anchors.fill: parent
            border.color: "gray"
            border.width: 1
            radius: 10
            anchors.topMargin: 50
            color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
        }

        Button {
            id: buttonNewFormula
            objectName: "buttonNewFormula"
            anchors.horizontalCenter: parent.horizontalCenter
            y: 10
            width: 120
            text: qsTr("新建配方")
            icon.source: "file:///" + appdir + "/ico/xinjian.png"
            font.pointSize: 11
            onClicked: {
                // 插入到list
                formulaListModel.insert(0, {
                        "formulaName": "配方" + Math.floor(Math.random() * 100000)
                    });
                formulaListView.currentIndex = 0;

                // 可编辑
                xPositionInput.readOnly = false;
                yPositionInput.readOnly = false;
                deviceProduceSpeedInput.readOnly = false;
                deviceproduceAccelerationInput.readOnly = false;
                deviceproduceDecelerationInput.readOnly = false;
                impurityPositionSiteInput.readOnly = false;
                formulaNameInput.readOnly = false;

                // 改变背景框颜色
                //xPosition.color = "lightgray"
                xPosition.border.color = "gray";
                //yPosition.color = "lightgray"
                yPosition.border.color = "gray";
                //deviceProduceSpeed.color = "lightgray"
                deviceProduceSpeed.border.color = "gray";
                //deviceproduceAcceleration.color = "lightgray"
                deviceproduceAcceleration.border.color = "gray";
                //deviceproduceDeceleration.color = "lightgray"
                deviceproduceDeceleration.border.color = "gray";
                //impurityPositionSite.color = "lightgray"
                impurityPositionSite.border.color = "gray";
                //formulaName.color = "lightgray"
                formulaName.border.color = "gray";

                // 高亮
                buttonNewFormula.highlighted = true;

                // 其他按钮不可选取
                buttonModifyFormula.enabled = false;
                buttonDownloadFormula.enabled = false;
                buttonDeleteFormula.enabled = false;
                buttonSaveFormula.enabled = true;
                enabled = false;

                // 保存按钮使用新建数据api
                buttonSaveFormula.saveFlag = 1;
            }
        }

        Button {
            id: buttonModifyFormula
            objectName: "buttonModifyFormula"
            anchors.horizontalCenter: parent.horizontalCenter
            y: 90
            width: 120
            text: qsTr("修改配方")
            icon.source: "file:///" + appdir + "/ico/bianji.png"

            enabled: false
            font.pointSize: 11
            onClicked: {
                // 可编辑
                xPositionInput.readOnly = false;
                yPositionInput.readOnly = false;
                deviceProduceSpeedInput.readOnly = false;
                deviceproduceAccelerationInput.readOnly = false;
                deviceproduceDecelerationInput.readOnly = false;
                impurityPositionSiteInput.readOnly = false;
                //formulaNameInput.readOnly = false

                // 改变背景框颜色
                //xPosition.color = "lightgray"
                xPosition.border.color = "gray";
                //yPosition.color = "lightgray"
                yPosition.border.color = "gray";
                //deviceProduceSpeed.color = "lightgray"
                deviceProduceSpeed.border.color = "gray";
                //deviceproduceAcceleration.color = "lightgray"
                deviceproduceAcceleration.border.color = "gray";
                //deviceproduceDeceleration.color = "lightgray"
                deviceproduceDeceleration.border.color = "gray";
                //impurityPositionSite.color = "lightgray"
                impurityPositionSite.border.color = "gray";

                // 高亮
                buttonModifyFormula.highlighted = true;

                // 其他按钮不可选取
                buttonNewFormula.enabled = false;
                buttonDownloadFormula.enabled = false;
                buttonDeleteFormula.enabled = false;
                buttonSaveFormula.enabled = true;

                // 保存按钮使用修改数据api
                buttonSaveFormula.saveFlag = 0;
            }
        }

        Button {
            id: buttonSaveFormula
            objectName: "buttonSaveFormula"
            anchors.horizontalCenter: parent.horizontalCenter
            y: 170
            width: 120
            text: qsTr("保存配方")
            icon.source: "file:///" + appdir + "/ico/baocun.png"
            font.pointSize: 11
            enabled: false
            property int saveFlag: 0
            onClicked: {
                // 不可编辑
                xPositionInput.readOnly = true;
                yPositionInput.readOnly = true;
                deviceProduceSpeedInput.readOnly = true;
                deviceproduceAccelerationInput.readOnly = true;
                deviceproduceDecelerationInput.readOnly = true;
                impurityPositionSiteInput.readOnly = true;
                formulaNameInput.readOnly = true;

                // 改变背景框颜色
                xPosition.color = Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1);
                xPosition.border.color = Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1);
                yPosition.color = Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1);
                yPosition.border.color = Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1);
                deviceProduceSpeed.color = Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1);
                deviceProduceSpeed.border.color = Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1);
                deviceproduceAcceleration.color = Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1);
                deviceproduceAcceleration.border.color = Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1);
                deviceproduceDeceleration.color = Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1);
                deviceproduceDeceleration.border.color = Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1);
                impurityPositionSite.color = Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1);
                impurityPositionSite.border.color = Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1);
                formulaName.color = Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1);
                formulaName.border.color = Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1);

                // 其他按钮可以使用
                buttonModifyFormula.highlighted = false;
                buttonNewFormula.highlighted = false;
                buttonNewFormula.enabled = true;
                buttonModifyFormula.enabled = true;
                buttonDownloadFormula.enabled = true;
                buttonDeleteFormula.enabled = true;
                buttonNewFormula.enabled = true;
                if (saveFlag === 0) {
                    // 修改
                    var json = {
                        "name": curFormulaName,
                        "code_x_position": xPositionInput.text,
                        "code_y_position": yPositionInput.text,
                        "impurity_locate": impurityPositionSiteInput.text,
                        "speed_produce": deviceProduceSpeedInput.text,
                        "acceleration_produce": deviceproduceAccelerationInput.text,
                        "deceleration_produce": deviceproduceDecelerationInput.text
                    };
                    var jsRet = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.ModifyFormula, JSON.stringify(json));
                    var result = JSON.parse(jsRet);
                    if (result.ok === true) {
                        saveText.text = "保存成功！";
                        saveText.color = "green";
                    } else {
                        saveText.text = "修改失败！";
                        saveText.color = "red";
                    }
                }
                if (saveFlag === 1) {
                    // 新建
                    var json = {
                        "name": formulaNameInput.text,
                        "code_x_position": xPositionInput.text,
                        "code_y_position": yPositionInput.text,
                        "impurity_locate": impurityPositionSiteInput.text,
                        "speed_produce": deviceProduceSpeedInput.text,
                        "acceleration_produce": deviceproduceAccelerationInput.text,
                        "deceleration_produce": deviceproduceDecelerationInput.text
                    };
                    var jsRet = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.InsertFormula, JSON.stringify(json));
                    var result = JSON.parse(jsRet);
                    if (result.ok === true) {
                        formulaListModel.setProperty(0, "formulaName", formulaNameInput.text);
                        saveText.text = "保存成功！";
                        saveText.color = "green";
                    } else {
                        saveText.text = "新建失败！";
                        saveText.color = "red";
                        formulaListModel.remove(formulaListView.currentIndex);
                        formulaControllWin.modifyFlag = 1;
                    }
                }
                enabled = false;
            }
        }

        Button {
            id: buttonDownloadFormula
            objectName: "buttonDownloadFormula"
            anchors.horizontalCenter: parent.horizontalCenter
            y: 250
            width: 120
            text: qsTr("下载配方")
            icon.source: "file:///" + appdir + "/ico/xiazai.png"
            font.pointSize: 11
            onClicked: {
                var json = {
                    "speed_r_12952": deviceProduceSpeedInput.text,
                    "add_r_12954": deviceproduceAccelerationInput.text,
                    "reduce_r_12596": deviceproduceDecelerationInput.text
                    "xPosision_r_12960": xPositionInput.text
                    "yPosision_r_12962": yPositionInput.text
                    
                };
                var strSend = JSON.stringify(json);
                var jsRet = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.WritePLC, strSend);
                var result = JSON.parse(jsRet);
                if (result.ok) {
                    // 发送给PLC 成功则做以下动作
                    downloadName = curFormulaName;
                    formulaDisplay.downloadState = "已下载";
                    master.formulaName = curFormulaName;
                    master.formulaSpeed = deviceProduceSpeedInput.text;
                    saveText.text = "下载成功！";
                    saveText.color = "green";
                }
            }
        }

        Button {
            id: buttonDeleteFormula
            objectName: "buttonDeleteFormula"
            anchors.horizontalCenter: parent.horizontalCenter
            y: 330
            width: 120
            text: qsTr("删除配方")
            icon.source: "file:///" + appdir + "/ico/shanchu.png"
            font.pointSize: 11
            onClicked: {
                var json = {
                    "name": curFormulaName
                };
                var strSend = JSON.stringify(json);
                var jsRet = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.DeleteFormula, strSend);
                var result = JSON.parse(jsRet);
                if (result.ok === true) {
                    if (curFormulaName === downloadName) {
                        master.formulaName = "无";
                        master.formulaSpeed = "";
                    }
                    formulaListModel.remove(formulaListView.currentIndex);
                    formulaControllWin.modifyFlag = 1;
                    saveText.text = "删除成功！";
                    saveText.color = "green";
                } else {
                    saveText.text = "删除失败！";
                    saveText.color = "red";
                }
            }
        }

        Text {
            id: saveText
            anchors.horizontalCenter: parent.horizontalCenter
            y: 455
            font.pointSize: 11
        }
    }
}
