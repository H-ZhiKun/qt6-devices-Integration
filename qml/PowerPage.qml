// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0

import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Window
import QtQuick.Layouts

GroupBox {
    id: valveControllWin
    objectName: "valveControllWin"
    property bool initFlag: true
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
    property alias curPage: bar.currentIndex
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
            property var curItem: valveListView1.itemAtIndex(0)
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
                property bool sendFlag: true
                focusPolicy: Qt.NoFocus
                enabled: singalComponent.curentSensor === -1 ? false : true
                onCheckedChanged: {
                    if (checked) {
                        if (sendFlag) {
                            var handMoveAddr = singalComponent.curItem.handMoveAddr;
                            var json = {
                                [handMoveAddr]: "0"
                            };
                            var jsRet = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.WritePLC, JSON.stringify(json));
                            var result = JSON.parse(jsRet);
                            if (result.ok === true) {
                                setInfo.text = "自动成功！";
                                setInfo.color = "green";
                            } else {
                                setInfo.text = "自动失败！";
                                setInfo.color = "red";
                            }
                        } else {
                            sendFlag = false;
                        }
                    }
                }
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
                    id: setSpeedParam  // 0~3000
                    x: 40
                    y: 32
                    text: actruePowerParam.actureSpeed
                    width: 70
                    height: 28
                    enabled: powerHandMove.checked ? true : false
                }

                // 位置设置
                TextArea {
                    id: setPositionParam  // 0~360
                    x: 40
                    y: 0
                    text: actruePowerParam.acturePosition
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
                    model: ["速度模式", "定位模式"]
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
                        var handMoveAddr = singalComponent.curItem.handMoveAddr;
                        if (Number(setPositionParam.text) <= 360 && Number(setPositionParam.text) >= 0) {
                            var setLocateAddr = singalComponent.curItem.setLocateAddr;
                        } else {
                            setInfo.text = "位置参数超出阈值！";
                            return;
                        }
                        if (Number(setSpeedParam.text) <= 3000 && Number(setSpeedParam.text) >= 0) {
                            var setSpeedAddr = singalComponent.curItem.setSpeedAddr;
                        } else {
                            setInfo.text = "速度参数超出阈值！";
                            return;
                        }
                        var directAddr = singalComponent.curItem.directAddr;
                        var locateAddr = singalComponent.curItem.locateAddr;
                        var json = {
                            [handMoveAddr]: "1",
                            [setLocateAddr]: setPositionParam.text,
                            [setSpeedAddr]: setSpeedParam.text,
                            [directAddr]: switchPowerDirection.checked ? "0" : "1",
                            [locateAddr]: comboBox.currentIndex === 1 ? "1" : "0"
                        };
                        var jsRet = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.WritePLC, JSON.stringify(json));
                        var result = JSON.parse(jsRet);
                        if (result.ok === true) {
                            setInfo.text = "保存成功！";
                            setInfo.color = "green";
                        } else {
                            setInfo.text = "保存失败！";
                            setInfo.color = "red";
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
                    property bool pageFlag: true
                    onClicked: {
                        bar.currentIndex = 1;
                        if (pageFlag) {
                            initPower();
                            pageFlag = false;
                        }
                    }
                    onEntered: {
                        powerTab2Rec.border.color = "lightblue";
                    }
                    onExited: {
                        powerTab2Rec.border.color = Qt.rgba(220 / 255, 220 / 255, 220 / 255, 1);
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
                    model: 3
                }
            }
        }
    }

    Label {
        id: setInfo
        x: 610
        y: 590
        text: qsTr("")
        color: "red"
    }

    // 这样写的原因： 地址不连续，建立映射关系
    function initPower() {
        var item0 = valveListView1.itemAtIndex(0);
        var item1 = valveListView1.itemAtIndex(1);
        var item2 = valveListView1.itemAtIndex(2);
        var item3 = valveListView1.itemAtIndex(3);
        var item4 = valveListView1.itemAtIndex(4);
        var item5 = valveListView2.itemAtIndex(0);
        var item6 = valveListView2.itemAtIndex(1);
        var item7 = valveListView2.itemAtIndex(2);
        var item8 = valveListView2.itemAtIndex(3);
        var item9 = valveListView2.itemAtIndex(4);
        var item10 = valveListView3.itemAtIndex(0);
        var item11 = valveListView3.itemAtIndex(1);
        var item12 = valveListView3.itemAtIndex(2);
        var item13 = valveListView3.itemAtIndex(3);
        var item14 = valveListView3.itemAtIndex(4);
        var item15 = valveListView4.itemAtIndex(0);
        var item16 = valveListView4.itemAtIndex(1);
        var item17 = valveListView4.itemAtIndex(2);
        var item18 = valveListView4.itemAtIndex(3);
        var item19 = valveListView4.itemAtIndex(4);
        var item20 = valveListView5.itemAtIndex(0);
        var item21 = valveListView5.itemAtIndex(1);
        var item22 = valveListView5.itemAtIndex(2);
        var item23 = valveListView5.itemAtIndex(3);
        var item24 = valveListView5.itemAtIndex(4);
        var item25 = valveListView6.itemAtIndex(0);
        var item26 = valveListView6.itemAtIndex(1);
        var item27 = valveListView6.itemAtIndex(2);
        var item28 = valveListView6.itemAtIndex(3);
        var item29 = valveListView6.itemAtIndex(4);
        var item30 = valveListView7.itemAtIndex(0);
        var item31 = valveListView7.itemAtIndex(1);
        var item32 = valveListView7.itemAtIndex(2);
        if (item0 === null) {
            console.log("M001 no item");
        } else {
            item0.itemName = "M001";
            item0.stateAddr = "M001_b_12315_11";
            item0.modeAddr = "M001_b_12318_10";
            item0.realLocateAddr = "M001_r_12350";
            item0.realSpeedAddr = "M001_r_12352";
            item0.locateAddr = "M001_b_12693_07";
            item0.handMoveAddr = "M001_b_12693_08";
            item0.startAddr = "M001_b_12693_09";
            item0.jogAddr = "M001_b_12693_10";
            item0.directAddr = "M001_b_12693_11";
            item0.setLocateAddr = "M001_r_12737";
            item0.setSpeedAddr = "M001_r_12739";
        }
        if (item1 === null) {
            console.log("M002 no item");
        } else {
            item1.itemName = "M002";
            item1.stateAddr = "M002_b_12315_12";
            item1.modeAddr = "M002_b_12318_11";
            item1.realLocateAddr = "M002_r_12354";
            item1.realSpeedAddr = "M002_r_12356";
            item1.locateAddr = "M002_b_12693_12";
            item1.handMoveAddr = "M002_b_12693_13";
            item1.startAddr = "M002_b_12693_14";
            item1.jogAddr = "M002_b_12693_15";
            item1.directAddr = "M002_b_12694_00";
            item1.setLocateAddr = "M002_r_12741";
            item1.setSpeedAddr = "M002_r_12743";
        }
        if (item2 === null) {
            console.log("M003 no item");
        } else {
            item2.itemName = "M003";
            item2.stateAddr = "M003_b_12315_13";
            item2.modeAddr = "M003_b_12318_12";
            item2.realLocateAddr = "M003_r_12358";
            item2.realSpeedAddr = "M003_r_12360";
            item2.locateAddr = "M003_b_12694_01";
            item2.handMoveAddr = "M003_b_12694_02";
            item2.startAddr = "M003_b_12694_03";
            item2.jogAddr = "M003_b_12694_04";
            item2.directAddr = "M003_b_12694_05";
            item2.setLocateAddr = "M003_r_12745";
            item2.setSpeedAddr = "M003_r_12747";
        }
        if (item3 === null) {
            console.log("M004 no item");
        } else {
            item3.itemName = "M004";
            item3.stateAddr = "M004_b_12315_14";
            item3.modeAddr = "M004_b_12318_13";
            item3.realLocateAddr = "M004_r_12362";
            item3.realSpeedAddr = "M004_r_12364";
            item3.locateAddr = "M004_b_12694_06";
            item3.handMoveAddr = "M004_b_12694_07";
            item3.startAddr = "M004_b_12694_08";
            item3.jogAddr = "M004_b_12694_09";
            item3.directAddr = "M004_b_12694_10";
            item3.setLocateAddr = "M004_r_12749";
            item3.setSpeedAddr = "M004_r_12751";
        }
        if (item4 === null) {
            console.log("M005 no item");
        } else {
            item4.itemName = "M005";
            item4.stateAddr = "M005_b_12315_15";
            item4.modeAddr = "M005_b_12318_14";
            item4.realLocateAddr = "M005_r_12366";
            item4.realSpeedAddr = "M005_r_12368";
            item4.locateAddr = "M005_b_12694_11";
            item4.handMoveAddr = "M005_b_12694_12";
            item4.startAddr = "M005_b_12694_13";
            item4.jogAddr = "M005_b_12694_14";
            item4.directAddr = "M005_b_12694_15";
            item4.setLocateAddr = "M005_r_12753";
            item4.setSpeedAddr = "M005_r_12755";
        }
        if (item5 === null) {
            console.log("M006 no item");
        } else {
            item5.itemName = "M006";
            item5.stateAddr = "M006_b_12316_00";
            item5.modeAddr = "M006_b_12318_15";
            item5.realLocateAddr = "M006_r_12370";
            item5.realSpeedAddr = "M006_r_12372";
            item5.locateAddr = "M006_b_12695_00";
            item5.handMoveAddr = "M006_b_12695_01";
            item5.startAddr = "M006_b_12695_02";
            item5.jogAddr = "M006_b_12695_03";
            item5.directAddr = "M006_b_12695_04";
            item5.setLocateAddr = "M006_r_12757";
            item5.setSpeedAddr = "M006_r_12759";
        }
        if (item6 === null) {
            console.log("M007 no item");
        } else {
            item6.itemName = "M007";
            item6.stateAddr = "M007_b_12316_01";
            item6.modeAddr = "M007_b_12319_00";
            item6.realLocateAddr = "M007_r_12374";
            item6.realSpeedAddr = "M007_r_12376";
            item6.locateAddr = "M007_b_12695_05";
            item6.handMoveAddr = "M007_b_12695_06";
            item6.startAddr = "M007_b_12695_07";
            item6.jogAddr = "M007_b_12695_08";
            item6.directAddr = "M007_b_12695_09";
            item6.setLocateAddr = "M007_r_12761";
            item6.setSpeedAddr = "M007_r_12763";
        }
        if (item7 === null) {
            console.log("M008 no item");
        } else {
            item7.itemName = "M008";
            item7.stateAddr = "M008_b_12316_02";
            item7.modeAddr = "M008_b_12319_01";
            item7.realLocateAddr = "M008_r_12378";
            item7.realSpeedAddr = "M008_r_12380";
            item7.locateAddr = "M008_b_12695_10";
            item7.handMoveAddr = "M008_b_12695_11";
            item7.startAddr = "M008_b_12695_12";
            item7.jogAddr = "M008_b_12695_13";
            item7.directAddr = "M008_b_12695_14";
            item7.setLocateAddr = "M008_r_12765";
            item7.setSpeedAddr = "M008_r_12767";
        }
        if (item8 === null) {
            console.log("M009 no item");
        } else {
            item8.itemName = "M009";
            item8.stateAddr = "M009_b_12316_03";
            item8.modeAddr = "M009_b_12319_02";
            item8.realLocateAddr = "M009_r_12382";
            item8.realSpeedAddr = "M009_r_12384";
            item8.locateAddr = "M009_b_12695_15";
            item8.handMoveAddr = "M009_b_12696_00";
            item8.startAddr = "M009_b_12696_01";
            item8.jogAddr = "M009_b_12696_02";
            item8.directAddr = "M009_b_12696_03";
            item8.setLocateAddr = "M009_r_12769";
            item8.setSpeedAddr = "M009_r_12771";
        }
        if (item9 === null) {
            console.log("M010 no item");
        } else {
            item9.itemName = "M010";
            item9.stateAddr = "M010_b_12316_04";
            item9.modeAddr = "M010_b_12319_03";
            item9.realLocateAddr = "M010_r_12386";
            item9.realSpeedAddr = "M010_r_12388";
            item9.locateAddr = "M010_b_12696_04";
            item9.handMoveAddr = "M010_b_12696_05";
            item9.startAddr = "M010_b_12696_06";
            item9.jogAddr = "M010_b_12696_07";
            item9.directAddr = "M010_b_12696_08";
            item9.setLocateAddr = "M010_r_12773";
            item9.setSpeedAddr = "M010_r_12775";
        }
        if (item10 === null) {
            console.log("M011 no item");
        } else {
            item10.itemName = "M011";
            item10.stateAddr = "M011_b_12316_05";
            item10.modeAddr = "M011_b_12319_04";
            item10.realLocateAddr = "M011_r_12390";
            item10.realSpeedAddr = "M011_r_12392";
            item10.locateAddr = "M011_b_12696_09";
            item10.handMoveAddr = "M011_b_12696_10";
            item10.startAddr = "M011_b_12696_11";
            item10.jogAddr = "M011_b_12696_12";
            item10.directAddr = "M011_b_12696_13";
            item10.setLocateAddr = "M011_r_12777";
            item10.setSpeedAddr = "M011_r_12779";
        }
        if (item11 === null) {
            console.log("M012 no item");
        } else {
            item11.itemName = "M012";
            item11.stateAddr = "M012_b_12316_06";
            item11.modeAddr = "M012_b_12319_05";
            item11.realLocateAddr = "M012_r_12394";
            item11.realSpeedAddr = "M012_r_12396";
            item11.locateAddr = "M012_b_12696_14";
            item11.handMoveAddr = "M012_b_12696_15";
            item11.startAddr = "M012_b_12697_00";
            item11.jogAddr = "M012_b_12697_01";
            item11.directAddr = "M012_b_12697_02";
            item11.setLocateAddr = "M012_r_12781";
            item11.setSpeedAddr = "M012_r_12783";
        }
        if (item12 === null) {
            console.log("M013 no item");
        } else {
            item12.itemName = "M013";
            item12.stateAddr = "M013_b_12316_07";
            item12.modeAddr = "M013_b_12319_06";
            item12.realLocateAddr = "M013_r_12398";
            item12.realSpeedAddr = "M013_r_12400";
            item12.locateAddr = "M013_b_12697_03";
            item12.handMoveAddr = "M013_b_12697_04";
            item12.startAddr = "M013_b_12697_05";
            item12.jogAddr = "M013_b_12697_06";
            item12.directAddr = "M013_b_12697_07";
            item12.setLocateAddr = "M013_r_12785";
            item12.setSpeedAddr = "M013_r_12787";
        }
        if (item13 === null) {
            console.log("M014 no item");
        } else {
            item13.itemName = "M014";
            item13.stateAddr = "M014_b_12316_08";
            item13.modeAddr = "M014_b_12319_07";
            item13.realLocateAddr = "M014_r_12402";
            item13.realSpeedAddr = "M014_r_12404";
            item13.locateAddr = "M014_b_12697_08";
            item13.handMoveAddr = "M014_b_12697_09";
            item13.startAddr = "M014_b_12697_10";
            item13.jogAddr = "M014_b_12697_11";
            item13.directAddr = "M014_b_12697_12";
            item13.setLocateAddr = "M014_r_12789";
            item13.setSpeedAddr = "M014_r_12791";
        }
        if (item14 === null) {
            console.log("M015 no item");
        } else {
            item14.itemName = "M015";
            item14.stateAddr = "M015_b_12316_09";
            item14.modeAddr = "M015_b_12319_08";
            item14.realLocateAddr = "M015_r_12406";
            item14.realSpeedAddr = "M015_r_12408";
            item14.locateAddr = "M015_b_12697_13";
            item14.handMoveAddr = "M015_b_12697_14";
            item14.startAddr = "M015_b_12697_15";
            item14.jogAddr = "M015_b_12698_00";
            item14.directAddr = "M015_b_12698_01";
            item14.setLocateAddr = "M015_r_12793";
            item14.setSpeedAddr = "M015_r_12795";
        }
        if (item15 === null) {
            console.log("M016 no item");
        } else {
            item15.itemName = "M016";
            item15.stateAddr = "M016_b_12316_10";
            item15.modeAddr = "M016_b_12319_09";
            item15.realLocateAddr = "M016_r_12410";
            item15.realSpeedAddr = "M016_r_12412";
            item15.locateAddr = "M016_b_12698_02";
            item15.handMoveAddr = "M016_b_12698_03";
            item15.startAddr = "M016_b_12698_04";
            item15.jogAddr = "M016_b_12698_05";
            item15.directAddr = "M016_b_12698_06";
            item15.setLocateAddr = "M016_r_12797";
            item15.setSpeedAddr = "M016_r_12799";
        }
        if (item16 === null) {
            console.log("M017 no item");
        } else {
            item16.itemName = "M017";
            item16.stateAddr = "M017_b_12316_11";
            item16.modeAddr = "M017_b_12319_10";
            item16.realLocateAddr = "M017_r_12414";
            item16.realSpeedAddr = "M017_r_12416";
            item16.locateAddr = "M017_b_12698_07";
            item16.handMoveAddr = "M017_b_12698_08";
            item16.startAddr = "M017_b_12698_09";
            item16.jogAddr = "M017_b_12698_10";
            item16.directAddr = "M017_b_12698_11";
            item16.setLocateAddr = "M017_r_12801";
            item16.setSpeedAddr = "M017_r_12803";
        }
        if (item17 === null) {
            console.log("M018 no item");
        } else {
            item17.itemName = "M018";
            item17.stateAddr = "M018_b_12316_12";
            item17.modeAddr = "M018_b_12319_11";
            item17.realLocateAddr = "M018_r_12418";
            item17.realSpeedAddr = "M018_r_12420";
            item17.locateAddr = "M018_b_12698_12";
            item17.handMoveAddr = "M018_b_12698_13";
            item17.startAddr = "M018_b_12698_14";
            item17.jogAddr = "M018_b_12698_15";
            item17.directAddr = "M018_b_12699_00";
            item17.setLocateAddr = "M018_r_12805";
            item17.setSpeedAddr = "M018_r_12807";
        }
        if (item18 === null) {
            console.log("M019 no item");
        } else {
            item18.itemName = "M019";
            item18.stateAddr = "M019_b_12316_13";
            item18.modeAddr = "M019_b_12319_12";
            item18.realLocateAddr = "M019_r_12422";
            item18.realSpeedAddr = "M019_r_12424";
            item18.locateAddr = "M019_b_12699_01";
            item18.handMoveAddr = "M019_b_12699_02";
            item18.startAddr = "M019_b_12699_03";
            item18.jogAddr = "M019_b_12699_04";
            item18.directAddr = "M019_b_12699_05";
            item18.setLocateAddr = "M019_r_12809";
            item18.setSpeedAddr = "M019_r_12811";
        }
        if (item19 === null) {
            console.log("M020 no item");
        } else {
            item19.itemName = "M020";
            item19.stateAddr = "M020_b_12316_14";
            item19.modeAddr = "M020_b_12319_13";
            item19.realLocateAddr = "M020_r_12426";
            item19.realSpeedAddr = "M020_r_12428";
            item19.locateAddr = "M020_b_12699_06";
            item19.handMoveAddr = "M020_b_12699_07";
            item19.startAddr = "M020_b_12699_08";
            item19.jogAddr = "M020_b_12699_09";
            item19.directAddr = "M020_b_12699_10";
            item19.setLocateAddr = "M020_r_12813";
            item19.setSpeedAddr = "M020_r_12815";
        }
        if (item20 === null) {
            console.log("M021 no item");
        } else {
            item20.itemName = "M021";
            item20.stateAddr = "M021_b_12316_15";
            item20.modeAddr = "M021_b_12319_14";
            item20.realLocateAddr = "M021_r_12430";
            item20.realSpeedAddr = "M021_r_12432";
            item20.locateAddr = "M021_b_12699_11";
            item20.handMoveAddr = "M021_b_12699_12";
            item20.startAddr = "M021_b_12699_13";
            item20.jogAddr = "M021_b_12699_14";
            item20.directAddr = "M021_b_12699_15";
            item20.setLocateAddr = "M021_r_12817";
            item20.setSpeedAddr = "M021_r_12819";
        }
        if (item21 === null) {
            console.log("M022 no item");
        } else {
            item21.itemName = "M022";
            item21.stateAddr = "M022_b_12317_00";
            item21.modeAddr = "M022_b_12319_15";
            item21.realLocateAddr = "M022_r_12434";
            item21.realSpeedAddr = "M022_r_12436";
            item21.locateAddr = "M022_b_12700_00";
            item21.handMoveAddr = "M022_b_12700_01";
            item21.startAddr = "M022_b_12700_02";
            item21.jogAddr = "M022_b_12700_03";
            item21.directAddr = "M022_b_12700_04";
            item21.setLocateAddr = "M022_r_12821";
            item21.setSpeedAddr = "M022_r_12823";
        }
        if (item22 === null) {
            console.log("M023 no item");
        } else {
            item22.itemName = "M023";
            item22.stateAddr = "M023_b_12317_01";
            item22.modeAddr = "M023_b_12320_00";
            item22.realLocateAddr = "M023_r_12438";
            item22.realSpeedAddr = "M023_r_12440";
            item22.locateAddr = "M023_b_12700_05";
            item22.handMoveAddr = "M023_b_12700_06";
            item22.startAddr = "M023_b_12700_07";
            item22.jogAddr = "M023_b_12700_08";
            item22.directAddr = "M023_b_12700_09";
            item22.setLocateAddr = "M023_r_12825";
            item22.setSpeedAddr = "M023_r_12827";
        }
        if (item23 === null) {
            console.log("M024 no item");
        } else {
            item23.itemName = "M024";
            item23.stateAddr = "M024_b_12317_02";
            item23.modeAddr = "M024_b_12320_01";
            item23.realLocateAddr = "M024_r_12442";
            item23.realSpeedAddr = "M024_r_12444";
            item23.locateAddr = "M024_b_12700_10";
            item23.handMoveAddr = "M024_b_12700_11";
            item23.startAddr = "M024_b_12700_12";
            item23.jogAddr = "M024_b_12700_13";
            item23.directAddr = "M024_b_12700_14";
            item23.setLocateAddr = "M024_r_12829";
            item23.setSpeedAddr = "M024_r_12831";
        }
        if (item24 === null) {
            console.log("M051 no item");
        } else {
            item24.itemName = "M051";
            item24.stateAddr = "M051_b_12315_06";
            item24.modeAddr = "M051_b_12318_05";
            item24.realLocateAddr = "M051_r_12494";
            item24.realSpeedAddr = "M051_r_12496";
            item24.locateAddr = "M051_b_12691_14";
            item24.handMoveAddr = "M051_b_12691_15";
            item24.startAddr = "M051_b_12692_00";
            item24.jogAddr = "M051_b_12692_01";
            item24.directAddr = "M051_b_12692_02";
            item24.setLocateAddr = "M051_r_12881";
            item24.setSpeedAddr = "M051_r_12883";
        }
        if (item25 === null) {
            console.log("M056 no item");
        } else {
            item25.itemName = "M056";
            item25.stateAddr = "M056_b_12315_07";
            item25.modeAddr = "M056_b_12318_06";
            item25.realLocateAddr = "M056_r_12498";
            item25.realSpeedAddr = "M056_r_12500";
            item25.locateAddr = "M056_b_12692_03";
            item25.handMoveAddr = "M056_b_12692_04";
            item25.startAddr = "M056_b_12692_05";
            item25.jogAddr = "M056_b_12692_06";
            item25.directAddr = "M056_b_12692_07";
            item25.setLocateAddr = "M056_r_12885";
            item25.setSpeedAddr = "M056_r_12887";
        }
        if (item26 === null) {
            console.log("M061 no item");
        } else {
            item26.itemName = "M061";
            item26.stateAddr = "M061_b_12315_02";
            item26.modeAddr = "M061_b_12318_01";
            item26.realLocateAddr = "M061_r_12330";
            item26.realSpeedAddr = "M061_r_12332";
            item26.locateAddr = "M061_b_12690_10";
            item26.handMoveAddr = "M061_b_12690_11";
            item26.startAddr = "M061_b_12690_12";
            item26.jogAddr = "M061_b_12690_13";
            item26.directAddr = "M061_b_12690_14";
            item26.setLocateAddr = "M061_r_12717";
            item26.setSpeedAddr = "M061_r_12719";
        }
        if (item27 === null) {
            console.log("M062 no item");
        } else {
            item27.itemName = "M062";
            item27.stateAddr = "M062_b_12315_03";
            item27.modeAddr = "M062_b_12318_02";
            item27.realLocateAddr = "M062_r_12334";
            item27.realSpeedAddr = "M062_r_12336";
            item27.locateAddr = "M062_b_12690_15";
            item27.handMoveAddr = "M062_b_12691_00";
            item27.startAddr = "M062_b_12691_01";
            item27.jogAddr = "M062_b_12691_02";
            item27.directAddr = "M062_b_12691_03";
            item27.setLocateAddr = "M062_r_12721";
            item27.setSpeedAddr = "M062_r_12723";
        }
        if (item28 === null) {
            console.log("M063 no item");
        } else {
            item28.itemName = "M063";
            item28.stateAddr = "M063_b_12315_04";
            item28.modeAddr = "M063_b_12318_03";
            item28.realLocateAddr = "M063_r_12338";
            item28.realSpeedAddr = "M063_r_12340";
            item28.locateAddr = "M063_b_12691_04";
            item28.handMoveAddr = "M063_b_12691_05";
            item28.startAddr = "M063_b_12691_06";
            item28.jogAddr = "M063_b_12691_07";
            item28.directAddr = "M063_b_12691_08";
            item28.setLocateAddr = "M063_r_12725";
            item28.setSpeedAddr = "M063_r_12727";
        }
        if (item29 === null) {
            console.log("M066 no item");
        } else {
            item29.itemName = "M066";
            item29.stateAddr = "M066_b_12315_05";
            item29.modeAddr = "M066_b_12318_04";
            item29.realLocateAddr = "M066_r_12342";
            item29.realSpeedAddr = "M066_r_12344";
            item29.locateAddr = "M066_b_12691_09";
            item29.handMoveAddr = "M066_b_12691_10";
            item29.startAddr = "M066_b_12691_11";
            item29.jogAddr = "M066_b_12691_12";
            item29.directAddr = "M066_b_12691_13";
            item29.setLocateAddr = "M066_r_12729";
            item29.setSpeedAddr = "M066_r_12731";
        }
        if (item30 === null) {
            console.log("M071 no item");
        } else {
            item30.itemName = "M071";
            item30.stateAddr = "M071_b_12315_00";
            item30.modeAddr = "M071_b_12317_15";
            item30.realLocateAddr = "M071_r_12322";
            item30.realSpeedAddr = "M071_r_12324";
            item30.locateAddr = "M071_b_12690_00";
            item30.handMoveAddr = "M071_b_12690_01";
            item30.startAddr = "M071_b_12690_02";
            item30.jogAddr = "M071_b_12690_03";
            item30.directAddr = "M071_b_12690_04";
            item30.setLocateAddr = "M071_r_12709";
            item30.setSpeedAddr = "M071_r_12711";
        }
        if (item31 === null) {
            console.log("M081 no item");
        } else {
            item31.itemName = "M081";
            item31.stateAddr = "M081_b_12315_09";
            item31.modeAddr = "M081_b_12318_08";
            item31.realLocateAddr = "M081_r_12502";
            item31.realSpeedAddr = "M081_r_12504";
            item31.locateAddr = "M081_b_12692_13";
            item31.handMoveAddr = "M081_b_12692_14";
            item31.startAddr = "M081_b_12692_15";
            item31.jogAddr = "M081_b_12693_00";
            item31.directAddr = "M081_b_12693_01";
            item31.setLocateAddr = "M081_r_12889";
            item31.setSpeedAddr = "M081_r_12891";
        }
        if (item32 === null) {
            console.log("M082 no item");
        } else {
            item32.itemName = "M082";
            item32.stateAddr = "M082_b_12315_10";
            item32.modeAddr = "M082_b_12318_09";
            item32.realLocateAddr = "M082_r_12506";
            item32.realSpeedAddr = "M082_r_12508";
            item32.locateAddr = "M082_b_12693_02";
            item32.handMoveAddr = "M082_b_12693_03";
            item32.startAddr = "M082_b_12693_04";
            item32.jogAddr = "M082_b_12693_05";
            item32.directAddr = "M082_b_12693_06";
            item32.setLocateAddr = "M082_r_12893";
            item32.setSpeedAddr = "M082_r_12895";
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
