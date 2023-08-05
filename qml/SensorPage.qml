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
    Connections {
        target: appMetaFlash // C++ 对象实例
        function onPageSensorChange(value) {
            // 执行其他操作...
            var jsonData = JSON.parse(value);
            var val = jsonData.valve;
        }
    }
}
