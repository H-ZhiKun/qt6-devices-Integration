// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0

import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Window
import QtQuick.Layouts

GroupBox {
    id: valveControllWin
    objectName: "valveControllWin"

    /*
    modality: Qt.WindowModal
    //固定窗口大小
    minimumWidth: 1110
    maximumWidth: 1110
    minimumHeight: 640
    maximumHeight: 640
    */
    width: 1110  //Screen.desktopAvailableWidth
    height: 640  //Screen.desktopAvailableHeight

    property string objName: "电机"
    visible: true
    background: Rectangle {
        anchors.fill: parent
        border.color: "gray"
        border.width: 1
        radius: 10
        color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
    }
    // 右侧控制栏
    GroupBox {
        id: valveControll
        objectName: "valveControll"
        anchors.fill: parent
        anchors.margins: 10
        title: objName + "控制"
        font.pointSize: 20
        anchors.leftMargin: 610
        anchors.bottomMargin: 40
        background: Rectangle {
            anchors.fill: parent
            anchors.topMargin: 50
            border.color: "gray"
            border.width: 1
            radius: 10
            color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
        }

        // 右上角单个电机的控制栏
        GroupBox {
            id: singalComponent
            objectName: "singalComponent"
            y: 5
            x: 0
            width: parent.width
            height: parent.height - 75
            title: "当前未选中" + valveControllWin.objName
            property int curentSensor: -1
            font.pointSize: 12
            background: Rectangle {
                anchors.fill: parent
                anchors.topMargin: 30
                border.color: "gray"
                border.width: 1
                radius: 10
                color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
            }

            // 手自动切换按钮
            RadioButton {
                id: powerAuto
                x: 10
                y: 10
                text: qsTr("自动模式")
                focusPolicy: Qt.NoFocus
                enabled: singalComponent.curentSensor === -1 ? false : true
            }

            RadioButton {
                id: powerHandMove
                x: 130
                y: 10
                text: qsTr("手动模式")
                checked: false
                focusPolicy: Qt.NoFocus
                icon.height: 30
                icon.width: 30
                enabled: singalComponent.curentSensor === -1 ? false : true
                onClicked: {
                    if (checked) {
                        allAutoColor.colorState = false;
                        allAutoColor.color = Qt.rgba(204 / 255, 204 / 255, 204 / 255, 1);
                        allAutoColor.border.width = 0;
                        allAutoButton.text = "全部自动";
                    }
                }
            }

            // 电机开启关闭切换按钮
            Switch {
                id: powerSwitch
                x: 270
                y: 10
                text: checked ? "电机开启" : "电机关闭"
                focusPolicy: Qt.NoFocus
                enabled: powerHandMove.checked ? true : false
            }

            // 实际参数展示栏
            GroupBox {
                id: actruePowerParam
                width: 200
                height: 110
                objectName: "actruePowerParam"
                title: "实际参数"
                property int acturePosition: 360  // 0~360
                property int actureSpeed: 1000  // 0~3000
                y: 60
                font.pointSize: 11
                background: Rectangle {
                    anchors.fill: parent
                    anchors.topMargin: 30
                    border.color: "gray"
                    border.width: 1
                    radius: 10
                    color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
                }
                Text {
                    text: "实际位置："
                    font.pointSize: 11
                    y: 5
                }
                Text {
                    text: "实际速度："
                    font.pointSize: 11
                    y: 35
                }
                Text {
                    text: "度"
                    font.pointSize: 11
                    x: 120
                    y: 5
                }
                Text {
                    text: "转/分钟"
                    font.pointSize: 11
                    x: 120
                    y: 35
                }
                Text {
                    text: actruePowerParam.acturePosition
                    font.pointSize: 11
                    y: 5
                    x: 76
                }
                Text {
                    text: actruePowerParam.actureSpeed
                    font.pointSize: 11
                    y: 35
                    x: 76
                }
            }

            // 电机参数修改栏
            GroupBox {
                id: setPowerParam
                width: 200
                height: 110
                objectName: "setPowerParam"
                title: "设置参数"
                font.pointSize: 11
                y: 60
                x: 220
                background: Rectangle {
                    anchors.fill: parent
                    anchors.topMargin: 30
                    border.color: "gray"
                    border.width: 1
                    radius: 10
                    color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
                }
                Text {
                    text: "位置："
                    font.pointSize: 11
                    y: 5
                }
                Text {
                    text: "速度："
                    font.pointSize: 11
                    y: 35
                }
                Text {
                    text: "度"
                    font.pointSize: 11
                    x: 120
                    y: 5
                }
                Text {
                    text: "转/分钟"
                    font.pointSize: 11
                    x: 120
                    y: 35
                }

                // 速度设置
                TextArea {
                    id: setSpeedParam  // 0~360
                    x: 40
                    y: 32
                    width: 70
                    height: 28
                    enabled: powerHandMove.checked ? true : false
                }

                // 位置设置
                TextArea {
                    id: setPositionParam  // 0~360
                    x: 40
                    y: 0
                    width: 50
                    height: 28
                    enabled: powerHandMove.checked ? true : false
                }
            }

            // 电机方向设置
            GroupBox {
                id: powerDirection
                width: 200
                height: 80
                y: 180
                title: "电机方向设置"
                objectName: "powerDirection"
                font.pointSize: 11
                background: Rectangle {
                    anchors.fill: parent
                    anchors.topMargin: 30
                    border.color: "gray"
                    border.width: 1
                    radius: 10
                    color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
                }

                // 电机正反开关
                MySwitch {
                    id: switchPowerDirection
                    anchors.horizontalCenter: parent.horizontalCenter
                    y: 0
                    focusPolicy: Qt.NoFocus
                    enabled: powerHandMove.checked ? true : false
                }

                Text {
                    text: qsTr("正")
                    font.pointSize: 11
                    x: switchPowerDirection.x - 25
                    y: switchPowerDirection.y
                }

                Text {
                    text: qsTr("反")
                    font.pointSize: 11
                    x: switchPowerDirection.x + 78
                    y: switchPowerDirection.y
                }
            }

            // 电机模式设置
            GroupBox {
                id: powerMode
                width: 200
                height: 80
                x: 220
                y: 180
                title: "电机模式设置"
                objectName: "powerMode"
                font.pointSize: 11
                background: Rectangle {
                    anchors.fill: parent
                    anchors.topMargin: 30
                    border.color: "gray"
                    border.width: 1
                    radius: 10
                    color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
                }

                ComboBox {
                    id: comboBox
                    objectName: "powerMode"
                    x: 0
                    y: -4
                    model: ["速度模式", "位置模式", "点动模式"]
                    focusPolicy: Qt.NoFocus
                    enabled: powerHandMove.checked ? true : false
                    font.pointSize: 11
                }
            }

            // 电机报警信息
            GroupBox {
                id: powerAlart
                objectName: "powerAlart"
                width: 200
                height: 90
                y: 270
                title: "报警显示"
                font.pointSize: 11
                background: Rectangle {
                    anchors.fill: parent
                    anchors.topMargin: 30
                    border.color: "gray"
                    border.width: 1
                    radius: 10
                    color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
                }
                Text {
                    id: powerHandAlart
                    objectName: "powerHandAlart"
                    text: "电机手动报警"
                    font.pointSize: 11
                    x: 0
                    y: -5
                    color: "gray"
                }
                Text {
                    id: powerAutoAlart
                    objectName: "powerAutoAlart"
                    text: "电机自动报警"
                    font.pointSize: 11
                    x: 0
                    y: 20
                    color: "gray"
                }
            }

            GroupBox {
                x: 220
                y: 270
                width: 200
                height: 90
                title: "保存"

                Button {
                    id: saveButton
                    width: 110
                    text: qsTr("保存设置")
                    icon.source: "file:///" + appdir + "/ico/baocun.png"
                    enabled: powerHandMove.checked ? true : false
                    onClicked: {
                        if (0) {
                            saveText.text = "保存失败！";
                            saveText.color = "red";
                        } else {
                            saveText.text = "保存成功！";
                            saveText.color = "green";
                        }
                    }
                    onEnabledChanged: {
                        saveText.text = "";
                    }
                }

                Text {
                    id: saveText
                    x: 114
                    y: 8
                }
            }
        }

        // 一键自动按钮
        Button {
            id: allAutoButton
            x: 40
            y: 440
            width: 140
            height: 50
            text: qsTr("全部自动")
            font.pointSize: 12

            property bool autoState: false
            layer.mipmap: true
            focusPolicy: Qt.StrongFocus
            icon.height: 20
            icon.width: 20
            icon.source: "file:///" + appdir + "/ico/zidong.png"

            background: Rectangle {
                id: allAutoColor
                anchors.fill: allAutoButton
                property bool colorState: false
                color: Qt.rgba(204 / 255, 204 / 255, 204 / 255, 1)
                border.width: 0
                border.color: "gray"
            }

            MouseArea {
                id: mouseArea1
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    allAutoColor.border.width = 2;
                }
                onExited: {
                    allAutoColor.border.width = allAutoColor.colorState ? 2 : 0;
                }
                onPressed: {
                    allAutoColor.color = Qt.rgba(153 / 255, 153 / 255, 153 / 255, 1);
                }
                onClicked: {
                    allAutoColor.color = Qt.rgba(153 / 255, 153 / 255, 153 / 255, 1);
                    allAutoColor.colorState = true;
                    allAutoColor.border.width = 2;
                    powerAuto.checked = true;
                    allAutoButton.text = "全部自动中";
                    //plc逻辑
                }
            }
        }

        // 所有轴全部回零
        Button {
            id: moveAxleZero
            objectName: "moveAxleZero"
            x: 245
            y: 440
            width: 160
            height: 50
            text: "所有轴全部回零"
            //icon.height: 40
            //icon.width: 40
            icon.source: "file:///" + appdir + "/ico/backzero.png"
            highlighted: false
            flat: false
            font.pointSize: 12
            focusPolicy: Qt.NoFocus
            onClicked: {
                enabled = false;
                // 全部回0函数调用
                // appMetaflash.axiseBackZero()
                enabled = true;
            }
        }
    }

    // 左侧电机展示栏
    GroupBox {
        id: valveDisplay
        objectName: "valveDisplay"
        anchors.fill: parent
        anchors.margins: 10
        title: valveControllWin.objName + "列表"
        font.pointSize: 20
        anchors.bottomMargin: 40
        anchors.rightMargin: 510
        background: Rectangle {
            anchors.fill: parent
            anchors.topMargin: 50
            border.color: "gray"
            border.width: 1
            radius: 10
            color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
        }

        TabBar {
            //点击相应的按钮实现切换
            id: bar
            objectName: "bar"
            width: parent.width
            TabButton {
                id: powerTab1
                text: qsTr("1~20")
                font.pointSize: 18
                contentItem: IconLabel {
                    spacing: powerTab1.spacing
                    mirrored: powerTab1.mirrored
                    display: powerTab1.display

                    icon: powerTab1.icon
                    text: powerTab1.text
                    font: powerTab1.font
                    color: powerTab1.palette.windowText
                }
                background: Rectangle {
                    id: powerTab1Rec
                    border.width: 2
                    border.color: Qt.rgba(220 / 255, 220 / 255, 220 / 255, 1)
                    color: bar.currentIndex === 0 ? "lightblue" : Qt.rgba(220 / 255, 220 / 255, 220 / 255, 1)
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        bar.currentIndex = 0;
                    }
                    onEntered: {
                        powerTab1Rec.border.color = "lightblue";
                    }
                    onExited: {
                        powerTab1Rec.border.color = Qt.rgba(220 / 255, 220 / 255, 220 / 255, 1);
                    }
                }
            }
            TabButton {
                id: powerTab2
                text: qsTr("21~40")
                font.pointSize: 18
                contentItem: IconLabel {
                    spacing: powerTab2.spacing
                    mirrored: powerTab2.mirrored
                    display: powerTab2.display

                    icon: powerTab2.icon
                    text: powerTab2.text
                    font: powerTab2.font
                    color: powerTab2.palette.windowText
                }
                background: Rectangle {
                    id: powerTab2Rec
                    border.width: 2
                    border.color: Qt.rgba(220 / 255, 220 / 255, 220 / 255, 1)
                    color: bar.currentIndex === 1 ? "lightblue" : Qt.rgba(220 / 255, 220 / 255, 220 / 255, 1)
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        bar.currentIndex = 1;
                    }
                    onEntered: {
                        powerTab2Rec.border.color = "lightblue";
                    }
                    onExited: {
                        powerTab2Rec.border.color = Qt.rgba(220 / 255, 220 / 255, 220 / 255, 1);
                    }
                }
            }
            TabButton {
                id: powerTab3
                text: qsTr("41~60")
                font.pointSize: 18
                contentItem: IconLabel {
                    spacing: powerTab3.spacing
                    mirrored: powerTab3.mirrored
                    display: powerTab3.display

                    icon: powerTab3.icon
                    text: powerTab3.text
                    font: powerTab3.font
                    color: powerTab3.palette.windowText
                }
                background: Rectangle {
                    id: powerTab3Rec
                    border.width: 2

                    border.color: Qt.rgba(220 / 255, 220 / 255, 220 / 255, 1)
                    color: bar.currentIndex === 2 ? "lightblue" : Qt.rgba(220 / 255, 220 / 255, 220 / 255, 1)
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        bar.currentIndex = 2;
                    }
                    onEntered: {
                        powerTab3Rec.border.color = "lightblue";
                    }
                    onExited: {
                        powerTab3Rec.border.color = Qt.rgba(220 / 255, 220 / 255, 220 / 255, 1);
                    }
                }
            }
            /*TabButton {
                    text: qsTr("61~80")
                    font.pointSize: 18
                    anchors.top: parent.top
                    background: Rectangle { color: "#bc948e8e" }
                }*/
        }

        StackLayout {
            //栈布局管理器
            id: stacklaout
            objectName: "stacklaout"
            width: parent.width
            y: 60
            x: 0
            height: parent.height - 100
            currentIndex: bar.currentIndex  //当前视图的索引
            Rectangle {
                // anchors.fill: stacklaout
                color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
                ComponentList {
                    id: valveListView1
                    anchors.leftMargin: 0
                    baseIndex: 1
                    listName: valveControllWin.objName
                }
                ComponentList {
                    id: valveListView2
                    anchors.leftMargin: 135
                    baseIndex: 6
                    listName: valveControllWin.objName
                }
                ComponentList {
                    id: valveListView3
                    anchors.leftMargin: 270
                    baseIndex: 11
                    listName: valveControllWin.objName
                }
                ComponentList {
                    id: valveListView4
                    anchors.leftMargin: 405
                    baseIndex: 16
                    listName: valveControllWin.objName
                }
            }
            Rectangle {
                // anchors.fill: stacklaout
                color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
                ComponentList {
                    id: valveListView5
                    anchors.leftMargin: 0
                    baseIndex: 21
                    listName: valveControllWin.objName
                }
                ComponentList {
                    id: valveListView6
                    anchors.leftMargin: 135
                    baseIndex: 26
                    listName: valveControllWin.objName
                }
                ComponentList {
                    id: valveListView7
                    anchors.leftMargin: 270
                    baseIndex: 31
                    listName: valveControllWin.objName
                }
                ComponentList {
                    id: valveListView8
                    anchors.leftMargin: 405
                    baseIndex: 36
                    listName: valveControllWin.objName
                }
            }
            Rectangle {
                // anchors.fill: stacklaout
                color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
                ComponentList {
                    id: valveListView9
                    anchors.leftMargin: 0
                    baseIndex: 41
                    listName: valveControllWin.objName
                }
                ComponentList {
                    id: valveListView10
                    anchors.leftMargin: 135
                    baseIndex: 46
                    listName: valveControllWin.objName
                }
                ComponentList {
                    id: valveListView11
                    anchors.leftMargin: 270
                    baseIndex: 51
                    listName: valveControllWin.objName
                }
                ComponentList {
                    id: valveListView12
                    anchors.leftMargin: 405
                    baseIndex: 56
                    listName: valveControllWin.objName
                }
            }
            /*Rectangle  {
                anchors.fill: stacklaout
                ComponentList{
                    anchors.leftMargin: 0
                    id: valveListView13
                    baseIndex: 61
                    listName: valveControllWin.objName
                }
                ComponentList{
                    anchors.leftMargin: 140
                    id: valveListView14
                    baseIndex:66
                    listName: valveControllWin.objName
                }
                ComponentList{
                    anchors.leftMargin: 280
                    id: valveListView15
                    baseIndex:71
                    listName: valveControllWin.objName
                }
                ComponentList{
                    anchors.leftMargin: 420
                    id: valveListView16
                    baseIndex:76
                    listName: valveControllWin.objName
                }
            }*/
        }
    }

    Connections {
        target: appMetaFlash // C++ 对象实例
        function onPagePowerChange(value) {
            // 执行其他操作...
            var jsonData = JSON.parse(value);
            var val = jsonData.valve;
        }
    }
}
