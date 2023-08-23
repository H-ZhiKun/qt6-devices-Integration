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

    // 这样写的原因： 地址不连续，建立映射关系
    function initPower() {
        var itemq = valveListView1.itemAtIndex(0);
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
        if (itemq === null) {
            console.log("M001 no item");
        } else {
            itemq.itemName = "M001";
            itemq.stateAddr = "M001_b_12315_11";
            itemq.modeAddr = "M001_b_12318_10";
            itemq.realLocateAddr = "M001_r_12350";
            itemq.realSpeedAddr = "M001_r_12352";
        }
        if (item1 === null) {
            console.log("M002 no item");
        } else {
            item1.itemName = "M002";
            item1.stateAddr = "M002_b_12315_12";
            item1.modeAddr = "M002_b_12318_11";
            item1.realLocateAddr = "M002_r_12354";
            item1.realSpeedAddr = "M002_r_12356";
        }
        if (item2 === null) {
            console.log("M003 no item");
        } else {
            item2.itemName = "M003";
            item2.stateAddr = "M003_b_12315_13";
            item2.modeAddr = "M003_b_12318_12";
            item2.realLocateAddr = "M003_r_12358";
            item2.realSpeedAddr = "M003_r_12360";
        }
        if (item3 === null) {
            console.log("M004 no item");
        } else {
            item3.itemName = "M004";
            item3.stateAddr = "M004_b_12315_14";
            item3.modeAddr = "M004_b_12318_13";
            item3.realLocateAddr = "M004_r_12362";
            item3.realSpeedAddr = "M004_r_12364";
        }
        if (item4 === null) {
            console.log("M005 no item");
        } else {
            item4.itemName = "M005";
            item4.stateAddr = "M005_b_12315_15";
            item4.modeAddr = "M005_b_12318_14";
            item4.realLocateAddr = "M005_r_12366";
            item4.realSpeedAddr = "M005_r_12368";
        }
        if (item5 === null) {
            console.log("M006 no item");
        } else {
            item5.itemName = "M006";
            item5.stateAddr = "M006_b_12316_00";
            item5.modeAddr = "M006_b_12318_15";
            item5.realLocateAddr = "M006_r_12370";
            item5.realSpeedAddr = "M006_r_12372";
        }
        if (item6 === null) {
            console.log("M007 no item");
        } else {
            item6.itemName = "M007";
            item6.stateAddr = "M007_b_12316_01";
            item6.modeAddr = "M007_b_12319_00";
            item6.realLocateAddr = "M007_r_12374";
            item6.realSpeedAddr = "M007_r_12376";
        }
        if (item7 === null) {
            console.log("M008 no item");
        } else {
            item7.itemName = "M008";
            item7.stateAddr = "M008_b_12316_02";
            item7.modeAddr = "M008_b_12319_01";
            item7.realLocateAddr = "M008_r_12378";
            item7.realSpeedAddr = "M008_r_12380";
        }
        if (item8 === null) {
            console.log("M009 no item");
        } else {
            item8.itemName = "M009";
            item8.stateAddr = "M009_b_12316_03";
            item8.modeAddr = "M009_b_12319_02";
            item8.realLocateAddr = "M009_r_12382";
            item8.realSpeedAddr = "M009_r_12384";
        }
        if (item9 === null) {
            console.log("M010 no item");
        } else {
            item9.itemName = "M010";
            item9.stateAddr = "M010_b_12316_04";
            item9.modeAddr = "M010_b_12319_03";
            item9.realLocateAddr = "M010_r_12386";
            item9.realSpeedAddr = "M010_r_12388";
        }
        if (item10 === null) {
            console.log("M011 no item");
        } else {
            item10.itemName = "M011";
            item10.stateAddr = "M011_b_12316_05";
            item10.modeAddr = "M011_b_12319_04";
            item10.realLocateAddr = "M011_r_12390";
            item10.realSpeedAddr = "M011_r_12392";
        }
        if (item11 === null) {
            console.log("M012 no item");
        } else {
            item11.itemName = "M012";
            item11.stateAddr = "M012_b_12316_06";
            item11.modeAddr = "M012_b_12319_05";
            item11.realLocateAddr = "M012_r_12394";
            item11.realSpeedAddr = "M012_r_12396";
        }
        if (item12 === null) {
            console.log("M013 no item");
        } else {
            item12.itemName = "M013";
            item12.stateAddr = "M013_b_12316_07";
            item12.modeAddr = "M013_b_12319_06";
            item12.realLocateAddr = "M013_r_12398";
            item12.realSpeedAddr = "M013_r_12400";
        }
        if (item13 === null) {
            console.log("M014 no item");
        } else {
            item13.itemName = "M014";
            item13.stateAddr = "M014_b_12316_08";
            item13.modeAddr = "M014_b_12319_07";
            item13.realLocateAddr = "M014_r_12402";
            item13.realSpeedAddr = "M014_r_12404";
        }
        if (item14 === null) {
            console.log("M015 no item");
        } else {
            item14.itemName = "M015";
            item14.stateAddr = "M015_b_12316_09";
            item14.modeAddr = "M015_b_12319_08";
            item14.realLocateAddr = "M015_r_12406";
            item14.realSpeedAddr = "M015_r_12408";
        }
        if (item15 === null) {
            console.log("M016 no item");
        } else {
            item15.itemName = "M016";
            item15.stateAddr = "M016_b_12316_10";
            item15.modeAddr = "M016_b_12319_09";
            item15.realLocateAddr = "M016_r_12410";
            item15.realSpeedAddr = "M016_r_12412";
        }
        if (item16 === null) {
            console.log("M017 no item");
        } else {
            item16.itemName = "M017";
            item16.stateAddr = "M017_b_12316_11";
            item16.modeAddr = "M017_b_12319_10";
            item16.realLocateAddr = "M017_r_12414";
            item16.realSpeedAddr = "M017_r_12416";
        }
        if (item17 === null) {
            console.log("M018 no item");
        } else {
            item17.itemName = "M018";
            item17.stateAddr = "M018_b_12316_12";
            item17.modeAddr = "M018_b_12319_11";
            item17.realLocateAddr = "M018_r_12418";
            item17.realSpeedAddr = "M018_r_12420";
        }
        if (item18 === null) {
            console.log("M019 no item");
        } else {
            item18.itemName = "M019";
            item18.stateAddr = "M019_b_12316_13";
            item18.modeAddr = "M019_b_12319_12";
            item18.realLocateAddr = "M019_r_12422";
            item18.realSpeedAddr = "M019_r_12424";
        }
        if (item19 === null) {
            console.log("M020 no item");
        } else {
            item19.itemName = "M020";
            item19.stateAddr = "M020_b_12316_14";
            item19.modeAddr = "M020_b_12319_13";
            item19.realLocateAddr = "M020_r_12426";
            item19.realSpeedAddr = "M020_r_12428";
        }
        if (item20 === null) {
            console.log("M021 no item");
        } else {
            item20.itemName = "M021";
            item20.stateAddr = "M021_b_12316_15";
            item20.modeAddr = "M021_b_12319_14";
            item20.realLocateAddr = "M021_r_12430";
            item20.realSpeedAddr = "M021_r_12432";
        }
        if (item21 === null) {
            console.log("M022 no item");
        } else {
            item21.itemName = "M022";
            item21.stateAddr = "M022_b_12317_00";
            item21.modeAddr = "M022_b_12319_15";
            item21.realLocateAddr = "M022_r_12434";
            item21.realSpeedAddr = "M022_r_12436";
        }
        if (item22 === null) {
            console.log("M023 no item");
        } else {
            item22.itemName = "M023";
            item22.stateAddr = "M023_b_12317_01";
            item22.modeAddr = "M023_b_12320_00";
            item22.realLocateAddr = "M023_r_12438";
            item22.realSpeedAddr = "M023_r_12440";
        }
        if (item23 === null) {
            console.log("M024 no item");
        } else {
            item23.itemName = "M024";
            item23.stateAddr = "M024_b_12317_02";
            item23.modeAddr = "M024_b_12320_01";
            item23.realLocateAddr = "M024_r_12442";
            item23.realSpeedAddr = "M024_r_12444";
        }
        if (item24 === null) {
            console.log("M051 no item");
        } else {
            item24.itemName = "M051";
            item24.stateAddr = "M051_b_12315_06";
            item24.modeAddr = "M051_b_12318_05";
            item24.realLocateAddr = "M051_r_12494";
            item24.realSpeedAddr = "M051_r_12496";
        }
        if (item25 === null) {
            console.log("M056 no item");
        } else {
            item25.itemName = "M056";
            item25.stateAddr = "M056_b_12315_07";
            item25.modeAddr = "M056_b_12318_06";
            item25.realLocateAddr = "M056_r_12498";
            item25.realSpeedAddr = "M056_r_12500";
        }
        if (item26 === null) {
            console.log("M061 no item");
        } else {
            item26.itemName = "M061";
            item26.stateAddr = "M061_b_12315_02";
            item26.modeAddr = "M061_b_12318_01";
            item26.realLocateAddr = "M061_r_12330";
            item26.realSpeedAddr = "M061_r_12332";
        }
        if (item27 === null) {
            console.log("M062 no item");
        } else {
            item27.itemName = "M062";
            item27.stateAddr = "M062_b_12315_03";
            item27.modeAddr = "M062_b_12318_02";
            item27.realLocateAddr = "M062_r_12334";
            item27.realSpeedAddr = "M062_r_12336";
        }
        if (item28 === null) {
            console.log("M063 no item");
        } else {
            item28.itemName = "M063";
            item28.stateAddr = "M063_b_12315_04";
            item28.modeAddr = "M063_b_12318_03";
            item28.realLocateAddr = "M063_r_12338";
            item28.realSpeedAddr = "M063_r_12340";
        }
        if (item29 === null) {
            console.log("M066 no item");
        } else {
            item29.itemName = "M066";
            item29.stateAddr = "M066_b_12315_05";
            item29.modeAddr = "M066_b_12318_04";
            item29.realLocateAddr = "M066_r_12342";
            item29.realSpeedAddr = "M066_r_12344";
        }
        if (item30 === null) {
            console.log("M071 no item");
        } else {
            item30.itemName = "M071";
            item30.stateAddr = "M071_b_12315_00";
            item30.modeAddr = "M071_b_12317_15";
            item30.realLocateAddr = "M071_r_12322";
            item30.realSpeedAddr = "M071_r_12324";
        }
        if (item31 === null) {
            console.log("M081 no item");
        } else {
            item31.itemName = "M081";
            item31.stateAddr = "M081_b_12315_09";
            item31.modeAddr = "M081_b_12318_08";
            item31.realLocateAddr = "M081_r_12502";
            item31.realSpeedAddr = "M081_r_12504";
        }
        if (item32 === null) {
            console.log("M082 no item");
        } else {
            item32.itemName = "M082";
            item32.stateAddr = "M082_b_12315_10";
            item32.modeAddr = "M082_b_12318_09";
            item32.realLocateAddr = "M082_r_12506";
            item32.realSpeedAddr = "M082_r_12508";
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
