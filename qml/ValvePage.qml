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
    minimumWidth: 1000
    maximumWidth: 1000
    minimumHeight: 620
    maximumHeight: 620
    */
    width: 1110  //Screen.desktopAvailableWidth
    height: 640  //Screen.desktopAvailableHeight

    property string objName: "阀门"
    visible: true
    background: Rectangle {
        anchors.fill: parent
        border.color: "gray"
        border.width: 1
        radius: 10
        color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
    }

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
            border.color: "gray"
            border.width: 1
            radius: 10
            anchors.topMargin: 50
            color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
        }
        GroupBox {
            id: singalComponent
            objectName: "singalComponent"
            y: 20
            x: 0
            width: parent.width
            height: parent.height - 200
            title: "当前未选中" + valveControllWin.objName
            property int curentSensor: -1
            font.pointSize: 12
            background: Rectangle {
                anchors.fill: parent
                border.color: "gray"
                border.width: 1
                radius: 10
                anchors.topMargin: 30
                color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
            }
            GroupBox {
                anchors.fill: parent
                anchors.bottomMargin: 100
                background: Rectangle {
                    anchors.fill: parent
                    border.color: "gray"
                    border.width: 1
                    radius: 10
                    color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
                }
                Text {
                    text: qsTr("模式选择")
                    font.pointSize: 12
                }
                RadioButton {
                    id: valveAuto
                    x: 10
                    y: 40
                    text: qsTr("自动模式")
                    focusPolicy: Qt.NoFocus
                    enabled: singalComponent.curentSensor === -1 ? false : true
                }

                RadioButton {
                    id: valveHandMove
                    x: 200
                    y: 40
                    text: qsTr("手动模式")
                    icon.height: 30
                    icon.width: 30
                    icon.source: "file:///" + appdir + "/ico/shoudong.png"
                    focusPolicy: Qt.NoFocus
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
            }

            Switch {
                id: valveSwitch
                anchors.horizontalCenter: parent.horizontalCenter
                y: 180
                text: checked ? "开启" : "关闭"
                focusPolicy: Qt.NoFocus
                enabled: valveHandMove.checked ? true : false
            }
        }

        Button {
            id: allAutoButton
            anchors.horizontalCenter: parent.horizontalCenter
            y: 400
            width: 150
            height: 50
            text: qsTr("全部自动")
            font.pointSize: 12

            property bool autoState: false
            layer.mipmap: true
            focusPolicy: Qt.StrongFocus
            icon.height: 40
            icon.width: 40
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
                    valveAuto.checked = true;
                    allAutoButton.text = "全部自动中";
                    //plc逻辑
                }
            }
        }
    }
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
            border.color: "gray"
            border.width: 1
            radius: 10
            anchors.topMargin: 50
            color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
        }
        TabBar {
            //点击相应的按钮实现切换
            id: bar
            width: parent.width
            TabButton {
                id: volveTab1
                text: qsTr("1~20")
                font.pointSize: 18
                contentItem: IconLabel {
                    spacing: volveTab1.spacing
                    mirrored: volveTab1.mirrored
                    display: volveTab1.display

                    icon: volveTab1.icon
                    text: volveTab1.text
                    font: volveTab1.font
                    color: volveTab1.palette.windowText
                }
                background: Rectangle {
                    id: volveTab1Rec
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
                        volveTab1Rec.border.color = "lightblue";
                    }
                    onExited: {
                        volveTab1Rec.border.color = Qt.rgba(220 / 255, 220 / 255, 220 / 255, 1);
                    }
                }
            }
            TabButton {
                id: volveTab2
                text: qsTr("21~40")
                font.pointSize: 18
                contentItem: IconLabel {
                    spacing: volveTab2.spacing
                    mirrored: volveTab2.mirrored
                    display: volveTab2.display

                    icon: volveTab2.icon
                    text: volveTab2.text
                    font: volveTab2.font
                    color: volveTab2.palette.windowText
                }
                background: Rectangle {
                    id: volveTab2Rec
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
                        volveTab2Rec.border.color = "lightblue";
                    }
                    onExited: {
                        volveTab2Rec.border.color = Qt.rgba(220 / 255, 220 / 255, 220 / 255, 1);
                    }
                }
            }
            /*TabButton {
                    text: qsTr("41~60")
                    font.pointSize: 18
                    background: Rectangle { color: "#bc948e8e" }
                }
                TabButton {
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
            height: parent.height - 50
            currentIndex: bar.currentIndex  //当前视图的索引
            Rectangle {
                // anchors.fill: stacklaout
                color: Qt.rgba(248 / 255, 248 / 255, 248 / 255, 1)
                ComponentList {
                    id: valveListView1
                    anchors.leftMargin: 0
                    objectName: "valveListView1"
                    baseIndex: 1
                    listName: valveControllWin.objName
                }
                ComponentList {
                    id: valveListView2
                    anchors.leftMargin: 135
                    objectName: "valveListView2"
                    baseIndex: 6
                    listName: valveControllWin.objName
                }
                ComponentList {
                    id: valveListView3
                    anchors.leftMargin: 270
                    objectName: "valveListView3"
                    baseIndex: 11
                    listName: valveControllWin.objName
                }
                ComponentList {
                    id: valveListView4
                    anchors.leftMargin: 405
                    objectName: "valveListView4"
                    baseIndex: 16
                    listName: valveControllWin.objName
                }
            }
            Rectangle {
                // anchors.fill: stacklaout
                color: Qt.rgba(248 / 255, 248 / 255, 248 / 255, 1)
                ComponentList {
                    id: valveListView5
                    anchors.leftMargin: 0
                    objectName: "valveListView5"
                    baseIndex: 21
                    listName: valveControllWin.objName
                }
                ComponentList {
                    id: valveListView6
                    anchors.leftMargin: 135
                    objectName: "valveListView6"
                    baseIndex: 26
                    listName: valveControllWin.objName
                }
                ComponentList {
                    id: valveListView7
                    anchors.leftMargin: 270
                    objectName: "valveListView7"
                    baseIndex: 31
                    listName: valveControllWin.objName
                }
                ComponentList {
                    id: valveListView8
                    anchors.leftMargin: 405
                    objectName: "valveListView8"
                    baseIndex: 36
                    listName: valveControllWin.objName
                }
            }
            /*Rectangle  {
                anchors.fill: stacklaout
                ComponentList{
                    anchors.leftMargin: 0
                    id: valveListView9
                    baseIndex: 41
                    listName: valveControllWin.objName
                }
                ComponentList{
                    anchors.leftMargin: 140
                    id: valveListView10
                    baseIndex:46
                    listName: valveControllWin.objectName
                }
                ComponentList{
                    anchors.leftMargin: 280
                    id: valveListView11
                    baseIndex:51
                    listName: valveControllWin.objectName
                }
                ComponentList{
                    anchors.leftMargin: 420
                    id: valveListView12
                    baseIndex:56
                    listName: valveControllWin.objectName
                }
            }
            Rectangle  {
                anchors.fill: stacklaout
                ComponentList{
                    anchors.leftMargin: 0
                    id: valveListView13
                    baseIndex: 61
                    listName: valveControllWin.objectName
                }
                ComponentList{
                    anchors.leftMargin: 140
                    id: valveListView14
                    baseIndex:66
                    listName: valveControllWin.objectName
                }
                ComponentList{
                    anchors.leftMargin: 280
                    id: valveListView15
                    baseIndex:71
                    listName: valveControllWin.objectName
                }
                ComponentList{
                    anchors.leftMargin: 420
                    id: valveListView16
                    baseIndex:76
                    listName: valveControllWin.objectName
                }
            }*/
        }
    }
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
        if (item0 === null) {
            console.log("XY001 no item");
        } else {
            item0.itemName = "XY001";
            item0.stateAddr = "XY001_b_12566_00";
            item0.handMoveAddr = "XY001_b_12945_00";
            item0.startAddr = "XY001_b_12945_01";
        }
        if (item1 === null) {
            console.log("XY002 no item");
        } else {
            item1.itemName = "XY002";
            item1.stateAddr = "XY002_b_12566_01";
            item1.handMoveAddr = "XY002_b_12945_02";
            item1.startAddr = "XY002_b_12945_03";
        }
        if (item2 === null) {
            console.log("XY003 no item");
        } else {
            item2.itemName = "XY003";
            item2.stateAddr = "XY003_b_12566_02";
            item2.handMoveAddr = "XY003_b_12945_04";
            item2.startAddr = "XY003_b_12945_05";
        }
        if (item3 === null) {
            console.log("XY004 no item");
        } else {
            item3.itemName = "XY004";
            item3.stateAddr = "XY004_b_12566_03";
            item3.handMoveAddr = "XY004_b_12945_06";
            item3.startAddr = "XY004_b_12945_07";
        }
        if (item4 === null) {
            console.log("XY005 no item");
        } else {
            item4.itemName = "XY005";
            item4.stateAddr = "XY005_b_12566_04";
            item4.handMoveAddr = "XY005_b_12945_08";
            item4.startAddr = "XY005_b_12945_09";
        }
        if (item5 === null) {
            console.log("XY006 no item");
        } else {
            item5.itemName = "XY006";
            item5.stateAddr = "XY006_b_12566_05";
            item5.handMoveAddr = "XY006_b_12945_10";
            item5.startAddr = "XY006_b_12945_11";
        }
        if (item6 === null) {
            console.log("XY007 no item");
        } else {
            item6.itemName = "XY007";
            item6.stateAddr = "XY007_b_12566_06";
            item6.handMoveAddr = "XY007_b_12945_12";
            item6.startAddr = "XY007_b_12945_13";
        }
        if (item7 === null) {
            console.log("XY008 no item");
        } else {
            item7.itemName = "XY008";
            item7.stateAddr = "XY008_b_12566_07";
            item7.handMoveAddr = "XY008_b_12945_14";
            item7.startAddr = "XY008_b_12945_15";
        }
        if (item8 === null) {
            console.log("XY009 no item");
        } else {
            item8.itemName = "XY009";
            item8.stateAddr = "XY009_b_12566_08";
            item8.handMoveAddr = "XY009_b_12946_00";
            item8.startAddr = "XY009_b_12946_01";
        }
        if (item9 === null) {
            console.log("XY010 no item");
        } else {
            item9.itemName = "XY010";
            item9.stateAddr = "XY010_b_12566_09";
            item9.handMoveAddr = "XY010_b_12946_02";
            item9.startAddr = "XY010_b_12946_03";
        }
        if (item10 === null) {
            console.log("XY011 no item");
        } else {
            item10.itemName = "XY011";
            item10.stateAddr = "XY011_b_12566_10";
            item10.handMoveAddr = "XY011_b_12946_04";
            item10.startAddr = "XY011_b_12946_05";
        }
        if (item11 === null) {
            console.log("XY012 no item");
        } else {
            item11.itemName = "XY012";
            item11.stateAddr = "XY012_b_12566_11";
            item11.handMoveAddr = "XY012_b_129456_06";
            item11.startAddr = "XY012_b_129456_07";
        }
        if (item12 === null) {
            console.log("XY013 no item");
        } else {
            item12.itemName = "XY013";
            item12.stateAddr = "XY013_b_12566_12";
            item12.handMoveAddr = "XY013_b_12946_08";
            item12.startAddr = "XY013_b_12946_09";
        }
        if (item13 === null) {
            console.log("XY014 no item");
        } else {
            item13.itemName = "XY014";
            item13.stateAddr = "XY014_b_12566_13";
            item13.handMoveAddr = "XY014_b_12946_10";
            item13.startAddr = "XY014_b_12946_11";
        }
        if (item14 === null) {
            console.log("XY015 no item");
        } else {
            item14.itemName = "XY015";
            item14.stateAddr = "XY015_b_12566_14";
            item14.handMoveAddr = "XY015_b_12946_12";
            item14.startAddr = "XY015_b_12946_13";
        }
        if (item15 === null) {
            console.log("XY016 no item");
        } else {
            item15.itemName = "XY016";
            item15.stateAddr = "XY016_b_12566_15";
            item15.handMoveAddr = "XY016_b_12946_14";
            item15.startAddr = "XY016_b_12946_15";
        }
        if (item16 === null) {
            console.log("XY017 no item");
        } else {
            item16.itemName = "XY017";
            item16.stateAddr = "XY017_b_12567_00";
            item16.handMoveAddr = "XY017_b_12947_00";
            item16.startAddr = "XY017_b_12947_01";
        }
        if (item17 === null) {
            console.log("XY018 no item");
        } else {
            item17.itemName = "XY018";
            item17.stateAddr = "XY018_b_12567_01";
            item17.handMoveAddr = "XY018_b_12947_02";
            item17.startAddr = "XY018_b_12947_03";
        }
        if (item18 === null) {
            console.log("XY019 no item");
        } else {
            item18.itemName = "XY019";
            item18.stateAddr = "XY019_b_12567_02";
            item18.handMoveAddr = "XY019_b_12947_04";
            item18.startAddr = "XY019_b_12947_05";
        }
        if (item19 === null) {
            console.log("XY020 no item");
        } else {
            item19.itemName = "XY020";
            item19.stateAddr = "XY020_b_12567_03";
            item19.handMoveAddr = "XY020_b_12947_06";
            item19.startAddr = "XY020_b_12947_07";
        }
        if (item20 === null) {
            console.log("XY021 no item");
        } else {
            item20.itemName = "XY021";
            item20.stateAddr = "XY021_b_12567_04";
            item20.handMoveAddr = "XY021_b_12947_08";
            item20.startAddr = "XY021_b_12947_09";
        }
        if (item21 === null) {
            console.log("XY022 no item");
        } else {
            item21.itemName = "XY022";
            item21.stateAddr = "XY022_b_12567_05";
            item21.handMoveAddr = "XY022_b_12947_10";
            item21.startAddr = "XY022_b_12947_11";
        }
        if (item22 === null) {
            console.log("XY023 no item");
        } else {
            item22.itemName = "XY023";
            item22.stateAddr = "XY023_b_12567_06";
            item22.handMoveAddr = "XY023_b_12947_12";
            item22.startAddr = "XY023_b_12947_13";
        }
        if (item23 === null) {
            console.log("XY024 no item");
        } else {
            item23.itemName = "XY024";
            item23.stateAddr = "XY024_b_12567_07";
            item23.handMoveAddr = "XY024_b_12947_14";
            item23.startAddr = "XY024_b_12947_15";
        }
        if (item24 === null) {
            console.log("XY061 no item");
        } else {
            item24.itemName = "XY061";
            item24.stateAddr = "XY061_b_12568_04";
            item24.handMoveAddr = "XY061_b_12949_08";
            item24.startAddr = "XY061_b_12949_09";
        }
        if (item25 === null) {
            console.log("XY062 no item");
        } else {
            item25.itemName = "XY062";
            item25.stateAddr = "XY062_b_12568_05";
            item25.handMoveAddr = "XY062_b_12949_10";
            item25.startAddr = "XY062_b_12949_11";
        }
        if (item26 === null) {
            console.log("XY063 no item");
        } else {
            item26.itemName = "XY063";
            item26.stateAddr = "XY063_b_12568_06";
            item26.handMoveAddr = "XY063_b_12949_12";
            item26.startAddr = "XY063_b_12949_13";
        }
        if (item27 === null) {
            console.log("XY064 no item");
        } else {
            item27.itemName = "XY064";
            item27.stateAddr = "XY064_b_12568_07";
            item27.handMoveAddr = "XY064_b_12949_14";
            item27.startAddr = "XY064_b_12949_15";
        }
        if (item28 === null) {
            console.log("XY065 no item");
        } else {
            item28.itemName = "XY065";
            item28.stateAddr = "XY065_b_12568_08";
            item28.handMoveAddr = "XY065_b_12950_00";
            item28.startAddr = "XY065_b_12950_01";
        }
    }

    Connections {
        target: appMetaFlash // C++ 对象实例
        function onPageValveChange(value) {
            // 执行其他操作...
            var jsonData = JSON.parse(value);
            var val = jsonData.valve;
        }
    }
}
