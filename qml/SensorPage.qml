// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0

import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Window
import QtQuick.Layouts

GroupBox {
    id: sensorControllWin
    objectName: "sensorControllWin"
    /*
    modality: Qt.WindowModal
    //固定窗口大小
    minimumWidth: 670
    maximumWidth: 670
    minimumHeight: 620
    maximumHeight: 620
    */
    width: 1110  //Screen.desktopAvailableWidth
    height: 640  //Screen.desktopAvailableHeight
    background: Rectangle {
        anchors.fill: parent
        border.color: "gray"
        border.width: 1
        radius: 10
        color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
    }

    property string objName: "传感器"
    visible: true

    /*GroupBox{
        id: sensorControll
        anchors.fill: parent
        anchors.margins: 30
        title: objectName + "控制"
        font.pointSize: 20
        anchors.leftMargin: 640
        anchors.bottomMargin: 40

        GroupBox{
            id: singalComponent
            y:20
            x:0
            width: parent.width
            height: parent.height-180
            title: "当前未选中" + sensorControllWin.objectName
            property int curentSensor: -1
            font.pointSize: 10

            Button {
                id: buttonAuto
                anchors.horizontalCenter: parent.horizontalCenter
                y: 52
                width: 235
                height: 71
                property int switchState: 0 //0:手动 1:自动
                icon.height: 40
                icon.width: 40
                // 图标、文字随状态发生改变
                icon.source: switchState === 0 ? "file:///" + appdir + "/ico/shoudong.png" : "file:///" + appdir + "/ico/zidong_1.png"
                text: switchState === 0 ? qsTr("手动模式中") : qsTr("自动模式中")
                background: Rectangle {
                    id: backcolor
                    color: buttonAuto.switchState === 0 ? "#2a87d7" : "#64fa32"
                    border.width: 1
                    border.color: "black"
                }

                MouseArea{
                    anchors.fill:buttonAuto
                    hoverEnabled: true  // 检测鼠标的进出
                    onEntered: {
                        backcolor.color = "#dda4a4a4" // 进入变灰
                    }

                    onExited: {
                        backcolor.color = buttonAuto.switchState === 0 ? "#2a87d7" : "#64fa32" // 退出变回本来颜色
                    }

                    onClicked: {
                        buttonAuto.switchState = !buttonAuto.switchState // 点击后状态反转
                        buttonAuto.switchState === 0 ? buttonSwitch.enabled = true : buttonSwitch.enabled = false  //自动状态开启后，关闭开启按钮不可选
                        backcolor.color = buttonAuto.switchState === 0 ? "#2a87d7" : "#64fa32"  // 点击后变色
                    }
                }

            }


            Button {
                id: buttonSwitch
                property int switchState: 0 //0:关闭 1:开启
                anchors.horizontalCenter: parent.horizontalCenter
                y: 152
                width: 235
                height: 71
                text: switchState === 0 ? qsTr("已关闭") : qsTr("已开启")
                icon.height: 40
                icon.width: 40
                icon.source: switchState === 0 ? "file:///" + appdir + "/ico/guanbi.png" : "file:///" + appdir + "/ico/kaiqi.png"
                background: Rectangle {
                    id: switchColor
                    color: buttonSwitch.switchState === 0 ? "#d6d7d7" : "#64fa32"
                    border.width: 1
                    border.color: "black"
                }
                MouseArea{
                    anchors.fill:buttonSwitch
                    hoverEnabled: true  // 检测鼠标的进出
                    onEntered: {
                        switchColor.color = "#dda4a4a4"  // 进入变灰色
                    }

                    onExited: {
                        switchColor.color = buttonSwitch.switchState === 0 ? "#d6d7d7" : "#64fa32"
                    }
                    onClicked: {
                        buttonSwitch.switchState = !buttonSwitch.switchState
                        switchColor.color = buttonSwitch.switchState === 0 ? "#d6d7d7" : "#64fa32"  // 点击后灰色变色
                    }
                }


            }
        }

        Button{
            id: buttonAllAuto
            anchors.horizontalCenter: parent.horizontalCenter
            y:350
            width:160
            height:75
            text: "      一键自动"
            highlighted: false
            flat: false
            font.pointSize: 16
            property int allAutoState: 0  // 0:未一键自动 1:一键自动 2:自动中
            Image {
                width: 45
                height: 45
                y:17
                x:5
                source: "file:///" + appdir + "/ico/zidong.png"
                fillMode: Image.PreserveAspectFit
            }

            background: Rectangle {
                id: allAutoColor
                color: buttonAllAuto.allAutoState === 0 ? "#d6d7d7" : "#64fa32"
                border.width: 1
                border.color: "black"
            }

            MouseArea{
                anchors.fill:buttonAllAuto
                hoverEnabled: true  // 检测鼠标的进出
                onEntered: {
                    allAutoColor.color = "#dda4a4a4" // 进入变灰
                }

                onExited: {
                    allAutoColor.color = buttonAllAuto.allAutoState === 0 ? "#d6d7d7" : "#64fa32" // 退出变回本来颜色
                }

                onClicked: {
                    buttonAllAuto.allAutoState = !buttonAllAuto.allAutoState // 点击后状态反转
                    allAutoColor.color = buttonAllAuto.allAutoState === 0 ? "#d6d7d7" : "#64fa32"  // 点击后变色
                }
            }
        }

    }*/

    GroupBox {
        id: sensorDisplay
        objectName: "sensorDisplay"
        anchors.fill: parent
        anchors.margins: 30
        title: sensorControllWin.objName + "列表"
        font.pointSize: 20
        anchors.bottomMargin: 40
        //anchors.rightMargin: 380
        background: Rectangle {
            anchors.fill: parent
            anchors.topMargin: 45
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
                id: sensorTab1
                text: qsTr("1~20")
                font.pointSize: 18
                contentItem: IconLabel {
                    spacing: sensorTab1.spacing
                    mirrored: sensorTab1.mirrored
                    display: sensorTab1.display

                    icon: sensorTab1.icon
                    text: sensorTab1.text
                    font: sensorTab1.font
                    color: sensorTab1.palette.windowText
                }
                background: Rectangle {
                    id: sensorTab1Rec
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
                        sensorTab1Rec.border.color = "lightblue";
                    }
                    onExited: {
                        sensorTab1Rec.border.color = Qt.rgba(220 / 255, 220 / 255, 220 / 255, 1);
                    }
                }
            }
            TabButton {
                id: sensorTab2
                text: qsTr("21~40")
                font.pointSize: 18
                contentItem: IconLabel {
                    spacing: sensorTab2.spacing
                    mirrored: sensorTab2.mirrored
                    display: sensorTab2.display

                    icon: sensorTab2.icon
                    text: sensorTab2.text
                    font: sensorTab2.font
                    color: sensorTab2.palette.windowText
                }
                background: Rectangle {
                    id: sensorTab2Rec
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
                        sensorTab2Rec.border.color = "lightblue";
                    }
                    onExited: {
                        sensorTab2Rec.border.color = Qt.rgba(220 / 255, 220 / 255, 220 / 255, 1);
                    }
                }
            }
            TabButton {
                id: sensorTab3
                text: qsTr("41~60")
                font.pointSize: 18
                contentItem: IconLabel {
                    spacing: sensorTab3.spacing
                    mirrored: sensorTab3.mirrored
                    display: sensorTab3.display

                    icon: sensorTab3.icon
                    text: sensorTab3.text
                    font: sensorTab3.font
                    color: sensorTab3.palette.windowText
                }
                background: Rectangle {
                    id: sensorTab3Rec
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
                        sensorTab3Rec.border.color = "lightblue";
                    }
                    onExited: {
                        sensorTab3Rec.border.color = Qt.rgba(220 / 255, 220 / 255, 220 / 255, 1);
                    }
                }
            }
            TabButton {
                id: sensorTab4
                text: qsTr("61~80")
                font.pointSize: 18
                contentItem: IconLabel {
                    spacing: sensorTab4.spacing
                    mirrored: sensorTab4.mirrored
                    display: sensorTab4.display

                    icon: sensorTab4.icon
                    text: sensorTab4.text
                    font: sensorTab4.font
                    color: sensorTab4.palette.windowText
                }
                background: Rectangle {
                    id: sensorTab4Rec
                    border.width: 2
                    border.color: Qt.rgba(220 / 255, 220 / 255, 220 / 255, 1)
                    color: bar.currentIndex === 3 ? "lightblue" : Qt.rgba(220 / 255, 220 / 255, 220 / 255, 1)
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        bar.currentIndex = 3;
                    }
                    onEntered: {
                        sensorTab4Rec.border.color = "lightblue";
                    }
                    onExited: {
                        sensorTab4Rec.border.color = Qt.rgba(220 / 255, 220 / 255, 220 / 255, 1);
                    }
                }
            }
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
                color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
                ComponentList {
                    id: sensorListView1
                    anchors.leftMargin: 100
                    objectName: "sensorListView1"
                    baseIndex: 1
                    listName: sensorControllWin.objName
                }
                ComponentList {
                    id: sensorListView2
                    anchors.leftMargin: 300
                    objectName: "sensorListView2"
                    baseIndex: 6
                    listName: sensorControllWin.objName
                }
                ComponentList {
                    id: sensorListView3
                    anchors.leftMargin: 500
                    objectName: "sensorListView3"
                    baseIndex: 11
                    listName: sensorControllWin.objName
                }
                ComponentList {
                    id: sensorListView4
                    anchors.leftMargin: 700
                    objectName: "sensorListView4"
                    baseIndex: 16
                    listName: sensorControllWin.objName
                }
            }
            Rectangle {
                // anchors.fill: stacklaout
                color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
                ComponentList {
                    id: sensorListView5
                    anchors.leftMargin: 100
                    objectName: "sensorListView5"
                    baseIndex: 21
                    listName: sensorControllWin.objName
                }
                ComponentList {
                    id: sensorListView6
                    anchors.leftMargin: 300
                    objectName: "sensorListView6"
                    baseIndex: 26
                    listName: sensorControllWin.objName
                }
                ComponentList {
                    id: sensorListView7
                    anchors.leftMargin: 500
                    objectName: "sensorListView7"
                    baseIndex: 31
                    listName: sensorControllWin.objName
                }
                ComponentList {
                    id: sensorListView8
                    anchors.leftMargin: 700
                    objectName: "sensorListView8"
                    baseIndex: 36
                    listName: sensorControllWin.objName
                }
            }
            Rectangle {
                // anchors.fill: stacklaout
                color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
                ComponentList {
                    id: sensorListView9
                    anchors.leftMargin: 100
                    objectName: "sensorListView9"
                    baseIndex: 41
                    listName: sensorControllWin.objName
                }
                ComponentList {
                    id: sensorListView10
                    anchors.leftMargin: 300
                    objectName: "sensorListView10"
                    baseIndex: 46
                    listName: sensorControllWin.objName
                }
                ComponentList {
                    id: sensorListView11
                    anchors.leftMargin: 500
                    objectName: "sensorListView11"
                    baseIndex: 51
                    listName: sensorControllWin.objName
                }
                ComponentList {
                    id: sensorListView12
                    anchors.leftMargin: 700
                    objectName: "sensorListView12"
                    baseIndex: 56
                    listName: sensorControllWin.objName
                }
            }
            Rectangle {
                // anchors.fill: stacklaout
                color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
                ComponentList {
                    id: sensorListView13
                    anchors.leftMargin: 100
                    objectName: "sensorListView13"
                    baseIndex: 61
                    listName: sensorControllWin.objName
                }
                ComponentList {
                    id: sensorListView14
                    anchors.leftMargin: 300
                    objectName: "sensorListView14"
                    baseIndex: 66
                    listName: sensorControllWin.objName
                }
                ComponentList {
                    id: sensorListView15
                    anchors.leftMargin: 500
                    objectName: "sensorListView15"
                    baseIndex: 71
                    listName: sensorControllWin.objName
                }
                ComponentList {
                    id: sensorListView16
                    anchors.leftMargin: 700
                    objectName: "sensorListView16"
                    baseIndex: 76
                    listName: sensorControllWin.objName
                }
            }
        }
    }
    function initPower() {
        var item0 = sensorListView1.itemAtIndex(0);
        var item1 = sensorListView1.itemAtIndex(1);
        var item2 = sensorListView1.itemAtIndex(2);
        var item3 = sensorListView1.itemAtIndex(3);
        var item4 = sensorListView1.itemAtIndex(4);
        var item5 = sensorListView2.itemAtIndex(0);
        var item6 = sensorListView2.itemAtIndex(1);
        var item7 = sensorListView2.itemAtIndex(2);
        var item8 = sensorListView2.itemAtIndex(3);
        var item9 = sensorListView2.itemAtIndex(4);
        var item10 = sensorListView3.itemAtIndex(0);
        var item11 = sensorListView3.itemAtIndex(1);
        var item12 = sensorListView3.itemAtIndex(2);
        var item13 = sensorListView3.itemAtIndex(3);
        var item14 = sensorListView3.itemAtIndex(4);
        var item15 = sensorListView4.itemAtIndex(0);
        var item16 = sensorListView4.itemAtIndex(1);
        var item17 = sensorListView4.itemAtIndex(2);
        var item18 = sensorListView4.itemAtIndex(3);
        var item19 = sensorListView4.itemAtIndex(4);
        var item20 = sensorListView5.itemAtIndex(0);
        var item21 = sensorListView5.itemAtIndex(1);
        var item22 = sensorListView5.itemAtIndex(2);
        var item23 = sensorListView5.itemAtIndex(3);
        var item24 = sensorListView5.itemAtIndex(4);
        var item25 = sensorListView6.itemAtIndex(0);
        var item26 = sensorListView6.itemAtIndex(1);
        var item27 = sensorListView6.itemAtIndex(2);
        var item28 = sensorListView6.itemAtIndex(3);
        var item29 = sensorListView6.itemAtIndex(4);
        var item30 = sensorListView7.itemAtIndex(0);
        var item31 = sensorListView7.itemAtIndex(1);
        var item32 = sensorListView7.itemAtIndex(2);
        var item33 = sensorListView7.itemAtIndex(3);
        var item34 = sensorListView7.itemAtIndex(4);
        var item35 = sensorListView8.itemAtIndex(0);
        var item36 = sensorListView8.itemAtIndex(1);
        var item37 = sensorListView8.itemAtIndex(2);
        var item38 = sensorListView8.itemAtIndex(3);
        var item39 = sensorListView8.itemAtIndex(4);
        var item40 = sensorListView9.itemAtIndex(0);
        var item41 = sensorListView9.itemAtIndex(1);
        var item42 = sensorListView9.itemAtIndex(2);
        var item43 = sensorListView9.itemAtIndex(3);
        var item44 = sensorListView9.itemAtIndex(4);
        var item45 = sensorListView10.itemAtIndex(0);
        var item46 = sensorListView10.itemAtIndex(1);
        var item47 = sensorListView10.itemAtIndex(2);
        var item48 = sensorListView10.itemAtIndex(3);
        var item49 = sensorListView10.itemAtIndex(4);
        var item50 = sensorListView11.itemAtIndex(0);
        var item51 = sensorListView11.itemAtIndex(1);
        var item52 = sensorListView11.itemAtIndex(2);
        var item53 = sensorListView11.itemAtIndex(3);
        var item54 = sensorListView11.itemAtIndex(4);
        var item55 = sensorListView12.itemAtIndex(0);
        var item56 = sensorListView12.itemAtIndex(1);
        var item57 = sensorListView12.itemAtIndex(2);
        var item58 = sensorListView12.itemAtIndex(3);
        var item59 = sensorListView12.itemAtIndex(4);
        var item60 = sensorListView13.itemAtIndex(0);
        var item61 = sensorListView13.itemAtIndex(1);
        var item62 = sensorListView13.itemAtIndex(2);
        var item63 = sensorListView13.itemAtIndex(3);
        var item64 = sensorListView13.itemAtIndex(4);
        var item65 = sensorListView14.itemAtIndex(0);
        var item66 = sensorListView14.itemAtIndex(1);
        var item67 = sensorListView14.itemAtIndex(2);
        var item68 = sensorListView14.itemAtIndex(3);
        var item69 = sensorListView14.itemAtIndex(4);
        var item70 = sensorListView15.itemAtIndex(0);
        var item71 = sensorListView15.itemAtIndex(1);
        if (item0 === null) {
            console.log("YS001 no item");
        } else {
            item0.itemName = "YS001";
            item0.stateAddr = "YS001_b_12558_00";
        }
        if (item1 === null) {
            console.log("YS002 no item");
        } else {
            item1.itemName = "YS002";
            item1.stateAddr = "YS002_b_12558_01";
        }
        if (item2 === null) {
            console.log("YS003 no item");
        } else {
            item2.itemName = "YS003";
            item2.stateAddr = "YS003_b_12558_02";
        }
        if (item3 === null) {
            console.log("YS004 no item");
        } else {
            item3.itemName = "YS004";
            item3.stateAddr = "YS004_b_12558_03";
        }
        if (item4 === null) {
            console.log("YS005 no item");
        } else {
            item4.itemName = "YS005";
            item4.stateAddr = "YS005_b_12558_04";
        }
        if (item5 === null) {
            console.log("YS006 no item");
        } else {
            item5.itemName = "YS006";
            item5.stateAddr = "YS006_b_12558_05";
        }
        if (item6 === null) {
            console.log("YS007 no item");
        } else {
            item6.itemName = "YS007";
            item6.stateAddr = "YS007_b_12558_06";
        }
        if (item7 === null) {
            console.log("YS008 no item");
        } else {
            item7.itemName = "YS008";
            item7.stateAddr = "YS008_b_12558_07";
        }
        if (item8 === null) {
            console.log("YS009 no item");
        } else {
            item8.itemName = "YS009";
            item8.stateAddr = "YS009_b_12558_08";
        }
        if (item9 === null) {
            console.log("YS010 no item");
        } else {
            item9.itemName = "YS010";
            item9.stateAddr = "YS010_b_12558_09";
        }
        if (item10 === null) {
            console.log("YS011 no item");
        } else {
            item10.itemName = "YS011";
            item10.stateAddr = "YS011_b_12558_10";
        }
        if (item11 === null) {
            console.log("YS012 no item");
        } else {
            item11.itemName = "YS012";
            item11.stateAddr = "YS012_b_12558_11";
        }
        if (item12 === null) {
            console.log("YS013 no item");
        } else {
            item12.itemName = "YS013";
            item12.stateAddr = "YS013_b_12558_12";
        }
        if (item13 === null) {
            console.log("YS014 no item");
        } else {
            item13.itemName = "YS014";
            item13.stateAddr = "YS014_b_12558_13";
        }
        if (item14 === null) {
            console.log("YS015 no item");
        } else {
            item14.itemName = "YS015";
            item14.stateAddr = "YS015_b_12558_14";
        }
        if (item15 === null) {
            console.log("YS016 no item");
        } else {
            item15.itemName = "YS016";
            item15.stateAddr = "YS016_b_12558_15";
        }
        if (item16 === null) {
            console.log("YS017 no item");
        } else {
            item16.itemName = "YS017";
            item16.stateAddr = "YS017_b_12559_00";
        }
        if (item17 === null) {
            console.log("YS018 no item");
        } else {
            item17.itemName = "YS018";
            item17.stateAddr = "YS018_b_12559_01";
        }
        if (item18 === null) {
            console.log("YS019 no item");
        } else {
            item18.itemName = "YS019";
            item18.stateAddr = "YS019_b_12559_02";
        }
        if (item19 === null) {
            console.log("YS020 no item");
        } else {
            item19.itemName = "YS020";
            item19.stateAddr = "YS020_b_12559_03";
        }
        if (item20 === null) {
            console.log("YS021 no item");
        } else {
            item20.itemName = "YS021";
            item20.stateAddr = "YS021_b_12559_04";
        }
        if (item21 === null) {
            console.log("YS022 no item");
        } else {
            item21.itemName = "YS022";
            item21.stateAddr = "YS022_b_12559_05";
        }
        if (item22 === null) {
            console.log("YS023 no item");
        } else {
            item22.itemName = "YS023";
            item22.stateAddr = "YS023_b_12559_06";
        }
        if (item23 === null) {
            console.log("YS024 no item");
        } else {
            item23.itemName = "YS024";
            item23.stateAddr = "YS024_b_12559_07";
        }
        if (item24 === null) {
            console.log("YS025 no item");
        } else {
            item24.itemName = "YS025";
            item24.stateAddr = "YS025_b_12559_08";
        }
        if (item25 === null) {
            console.log("YS026 no item");
        } else {
            item25.itemName = "YS026";
            item25.stateAddr = "YS026_b_12559_09";
        }
        if (item26 === null) {
            console.log("YS027 no item");
        } else {
            item26.itemName = "YS027";
            item26.stateAddr = "YS027_b_12559_10";
        }
        if (item27 === null) {
            console.log("YS028 no item");
        } else {
            item27.itemName = "YS028";
            item27.stateAddr = "YS028_b_12559_11";
        }
        if (item28 === null) {
            console.log("YS029 no item");
        } else {
            item28.itemName = "YS029";
            item28.stateAddr = "YS029_b_12559_12";
        }
        if (item29 === null) {
            console.log("YS030 no item");
        } else {
            item29.itemName = "YS030";
            item29.stateAddr = "YS030_b_12559_13";
        }
        if (item30 === null) {
            console.log("YS031 no item");
        } else {
            item30.itemName = "YS031";
            item30.stateAddr = "YS031_b_12559_14";
        }
        if (item31 === null) {
            console.log("YS032 no item");
        } else {
            item31.itemName = "YS032";
            item31.stateAddr = "YS032_b_12559_15";
        }
        if (item32 === null) {
            console.log("YS033 no item");
        } else {
            item32.itemName = "YS033";
            item32.stateAddr = "YS033_b_12560_00";
        }
        if (item33 === null) {
            console.log("YS034 no item");
        } else {
            item33.itemName = "YS034";
            item33.stateAddr = "YS034_b_12560_01";
        }
        if (item34 === null) {
            console.log("YS035 no item");
        } else {
            item34.itemName = "YS035";
            item34.stateAddr = "YS035_b_12560_02";
        }
        if (item35 === null) {
            console.log("YS036 no item");
        } else {
            item35.itemName = "YS036";
            item35.stateAddr = "YS036_b_12560_03";
        }
        if (item36 === null) {
            console.log("YS037 no item");
        } else {
            item36.itemName = "YS037";
            item36.stateAddr = "YS037_b_12560_04";
        }
        if (item37 === null) {
            console.log("YS038 no item");
        } else {
            item37.itemName = "YS038";
            item37.stateAddr = "YS038_b_12560_05";
        }
        if (item38 === null) {
            console.log("YS039 no item");
        } else {
            item38.itemName = "YS039";
            item38.stateAddr = "YS039_b_12560_06";
        }
        if (item39 === null) {
            console.log("YS040 no item");
        } else {
            item39.itemName = "YS040";
            item39.stateAddr = "YS040_b_12560_07";
        }
        if (item40 === null) {
            console.log("YS041 no item");
        } else {
            item40.itemName = "YS041";
            item40.stateAddr = "YS041_b_12560_08";
        }
        if (item41 === null) {
            console.log("YS042 no item");
        } else {
            item41.itemName = "YS042";
            item41.stateAddr = "YS042_b_12560_09";
        }
        if (item42 === null) {
            console.log("YS043 no item");
        } else {
            item42.itemName = "YS043";
            item42.stateAddr = "YS043_b_12560_10";
        }
        if (item43 === null) {
            console.log("YS044 no item");
        } else {
            item43.itemName = "YS044";
            item43.stateAddr = "YS044_b_12560_11";
        }
        if (item44 === null) {
            console.log("YS045 no item");
        } else {
            item44.itemName = "YS045";
            item44.stateAddr = "YS045_b_12560_12";
        }
        if (item45 === null) {
            console.log("YS046 no item");
        } else {
            item45.itemName = "YS046";
            item45.stateAddr = "YS046_b_12560_13";
        }
        if (item46 === null) {
            console.log("YS047 no item");
        } else {
            item46.itemName = "YS047";
            item46.stateAddr = "YS047_b_12560_14";
        }
        if (item47 === null) {
            console.log("YS048 no item");
        } else {
            item47.itemName = "YS048";
            item47.stateAddr = "YS048_b_12560_15";
        }
        if (item48 === null) {
            console.log("YS101 no item");
        } else {
            item47.itemName = "YS101";
            item47.stateAddr = "YS101_b_12562_08";
        }
        if (item49 === null) {
            console.log("YS102 no item");
        } else {
            item49.itemName = "YS102";
            item49.stateAddr = "YS102_b_12562_09";
        }
        if (item50 === null) {
            console.log("YS103 no item");
        } else {
            item50.itemName = "YS103";
            item50.stateAddr = "YS103_b_12562_10";
        }
        if (item51 === null) {
            console.log("YS104 no item");
        } else {
            item51.itemName = "YS104";
            item51.stateAddr = "YS104_b_12562_11";
        }
        if (item52 === null) {
            console.log("YS105 no item");
        } else {
            item52.itemName = "YS105";
            item52.stateAddr = "YS105_b_12562_12";
        }
        if (item53 === null) {
            console.log("YS106 no item");
        } else {
            item53.itemName = "YS106";
            item53.stateAddr = "YS106_b_12562_13";
        }
        if (item54 === null) {
            console.log("YS107 no item");
        } else {
            item54.itemName = "YS107";
            item54.stateAddr = "YS107_b_12562_14";
        }
        if (item55 === null) {
            console.log("YS108 no item");
        } else {
            item55.itemName = "YS108";
            item55.stateAddr = "YS108_b_12562_15";
        }
        if (item56 === null) {
            console.log("YS109 no item");
        } else {
            item56.itemName = "YS109";
            item56.stateAddr = "YS109_b_12563_00";
        }
        if (item57 === null) {
            console.log("YS110 no item");
        } else {
            item57.itemName = "YS110";
            item57.stateAddr = "YS110_b_12563_01";
        }
        if (item58 === null) {
            console.log("YS111 no item");
        } else {
            item58.itemName = "YS111";
            item58.stateAddr = "YS111_b_12563_02";
        }
        if (item59 === null) {
            console.log("YS112 no item");
        } else {
            item59.itemName = "YS112";
            item59.stateAddr = "YS112_b_12563_03";
        }
        if (item60 === null) {
            console.log("YS113 no item");
        } else {
            item60.itemName = "YS113";
            item60.stateAddr = "YS113_b_12563_04";
        }
        if (item61 === null) {
            console.log("YS114 no item");
        } else {
            item61.itemName = "YS114";
            item61.stateAddr = "YS114_b_12563_05";
        }
        if (item62 === null) {
            console.log("YS120 no item");
        } else {
            item62.itemName = "YS120";
            item62.stateAddr = "YS120_b_12563_06";
        }
        if (item63 === null) {
            console.log("YS121 no item");
        } else {
            item63.itemName = "YS121";
            item63.stateAddr = "YS121_b_12563_07";
        }
        if (item64 === null) {
            console.log("YS122 no item");
        } else {
            item64.itemName = "YS122";
            item64.stateAddr = "YS122_b_12563_08";
        }
        if (item65 === null) {
            console.log("YS151 no item");
        } else {
            item65.itemName = "YS151";
            item65.stateAddr = "YS151_b_12563_11";
        }
        if (item66 === null) {
            console.log("YS170 no item");
        } else {
            item66.itemName = "YS170";
            item66.stateAddr = "YS170_b_12563_09";
        }
        if (item67 === null) {
            console.log("YS171 no item");
        } else {
            item67.itemName = "YS171";
            item67.stateAddr = "YS171_b_12563_10";
        }
        if (item68 === null) {
            console.log("YS172 no item");
        } else {
            item68.itemName = "YS172";
            item68.stateAddr = "YS172_b_12563_12";
        }
        if (item69 === null) {
            console.log("YS173 no item");
        } else {
            item69.itemName = "YS173";
            item69.stateAddr = "YS173_b_12563_13";
        }
        if (item70 === null) {
            console.log("YS175 no item");
        } else {
            item70.itemName = "YS175";
            item70.stateAddr = "YS175_b_12563_14";
        }
        if (item71 === null) {
            console.log("YS176 no item");
        } else {
            item71.itemName = "YS176";
            item71.stateAddr = "YS176_b_12563_15";
        }
    }
    Connections {
        target: appMetaFlash // C++ 对象实例
        function onPageSensorChange(value) {
            // 执行其他操作...
            var jsonData = JSON.parse(value);
            var val = jsonData.valve;
        }
    }
}
