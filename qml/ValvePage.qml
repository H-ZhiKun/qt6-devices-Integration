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

    GroupBox{
        id: valveControll
        objectName: "valveControll"
        anchors.fill: parent
        anchors.margins: 30
        title: objName + "控制"
        font.pointSize: 20
        anchors.leftMargin: 640
        anchors.bottomMargin: 40

        GroupBox{

            id: singalComponent
            objectName: "singalComponent"
            y:20
            x:0
            width: parent.width
            height: parent.height-180
            title: "当前未选中" + valveControllWin.objName
            property int curentSensor: -1
            font.pointSize: 15

            Button {
                // TODO:手动后一键自动按钮变色
                id: buttonAuto
                objectName: "buttonAuto"
                anchors.horizontalCenter: parent.horizontalCenter
                y: 52
                width: 155
                height: 30
                property int switchState: 0 //0:自动 1:手动
                icon.height: 30
                icon.width: 30
                font.pointSize: 15
                // 图标、文字随状态发生改变
                icon.source: switchState === 1 ? "file:./ico/shoudong.png" : "file:./ico/zidong_1.png"
                text: switchState === 1 ? qsTr("手动模式中") : qsTr("自动模式中")
                background: Rectangle {
                    id: backcolor
                    color: buttonAuto.switchState === 1 ? "#2a87d7" : "#64fa32"
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
                        backcolor.color = buttonAuto.switchState === 1 ? "#2a87d7" : "#64fa32" // 退出变回本来颜色
                    }

                    onClicked: {
                        buttonAuto.switchState = !buttonAuto.switchState // 点击后状态反转
                        //buttonAuto.switchState === 0 ? buttonSwitch.enabled = true : buttonSwitch.enabled = false  //自动状态开启后，关闭开启按钮不可选
                        backcolor.color = buttonAuto.switchState === 1 ? "#2a87d7" : "#64fa32"  // 点击后变色
                    }
                }

            }


            Button {
                id: buttonSwitch
                objectName: "buttonSwitch"
                property int switchState: 0 //0:关闭 1:开启
                anchors.horizontalCenter: parent.horizontalCenter
                y: 152
                width: 155
                height: 30
                text: switchState === 0 ? qsTr("已关闭") : qsTr("已开启")
                icon.height: 30
                icon.width: 30
                icon.source: switchState === 0 ? "file:./ico/guanbi.png" : "file:./ico/kaiqi.png"
                font.pointSize: 15
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
            objectName: "buttonAllAuto"
            anchors.horizontalCenter: parent.horizontalCenter
            y:400
            width:155
            height:30
            text: "    全部自动"
            highlighted: false
            flat: false
            font.pointSize: 15
            property int allAutoState: 0  // 0:一键自动 1:未一键自动 2:自动中
            Image {
                width: 25
                height: 25
                y: 3
                x: 5
                source: "file:./ico/zidong.png"
                fillMode: Image.PreserveAspectFit
            }

            background: Rectangle {
                id: allAutoColor
                color: buttonAllAuto.allAutoState === 1 ? "#d6d7d7" : "#64fa32"
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
                    allAutoColor.color = buttonAllAuto.allAutoState === 1 ? "#d6d7d7" : "#64fa32" // 退出变回本来颜色
                }

                onClicked: {
                    buttonAllAuto.allAutoState = !buttonAllAuto.allAutoState // 点击后状态反转
                    allAutoColor.color = buttonAllAuto.allAutoState === 1 ? "#d6d7d7" : "#64fa32"  // 点击后变色
                }
            }
        }

    }

    GroupBox{
        id: valveDisplay
        objectName: "valveDisplay"
        anchors.fill: parent
        anchors.margins: 30
        title: valveControllWin.objName + "列表"
        font.pointSize: 20
        anchors.bottomMargin: 40
        anchors.rightMargin: 480

        TabBar {    //点击相应的按钮实现切换
                id: bar
                width: parent.width
                TabButton {
                    text: qsTr("1~20")
                    font.pointSize: 18
                    background: Rectangle { color: "#bc948e8e" }
                }
                TabButton {
                    text: qsTr("21~40")
                    font.pointSize: 18
                    background: Rectangle { color: "#bc948e8e" }
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

        StackLayout {   //栈布局管理器
            id:stacklaout
            objectName: "stacklaout"
            width: parent.width
            y:60
            x:0
            height: parent.height-100
            currentIndex: bar.currentIndex  //当前视图的索引
            Rectangle  {
                anchors.fill: stacklaout
                ComponentList{
                    anchors.leftMargin: 0
                    id: valveListView1
                    objectName: "valveListView1"
                    baseIndex: 1
                    listName: valveControllWin.objName
                }
                ComponentList{
                    anchors.leftMargin: 140
                    id: valveListView2
                    objectName: "valveListView2"
                    baseIndex:6
                    listName: valveControllWin.objName
                }
                ComponentList{
                    anchors.leftMargin: 280
                    id: valveListView3
                    objectName: "valveListView3"
                    baseIndex:11
                    listName: valveControllWin.objName
                }
                ComponentList{
                    anchors.leftMargin: 420
                    id: valveListView4
                    objectName: "valveListView4"
                    baseIndex:16
                    listName: valveControllWin.objName
                }
            }
            Rectangle  {
                anchors.fill: stacklaout
                ComponentList{
                    anchors.leftMargin: 0
                    id: valveListView5
                    objectName: "valveListView5"
                    baseIndex: 21
                    listName: valveControllWin.objName
                }
                ComponentList{
                    anchors.leftMargin: 140
                    id: valveListView6
                    objectName: "valveListView6"
                    baseIndex:26
                    listName: valveControllWin.objName
                }
                ComponentList{
                    anchors.leftMargin: 280
                    id: valveListView7
                    objectName: "valveListView7"
                    baseIndex:31
                    listName: valveControllWin.objName
                }
                ComponentList{
                    anchors.leftMargin: 420
                    id: valveListView8
                    objectName: "valveListView8"
                    baseIndex:36
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


}
