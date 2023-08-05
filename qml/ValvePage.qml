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
    Connections {
        target: appMetaFlash // C++ 对象实例
        function onPageValveChange(value) {
            // 执行其他操作...
            var jsonData = JSON.parse(value);
            var val = jsonData.valve;
        }
    }
}
