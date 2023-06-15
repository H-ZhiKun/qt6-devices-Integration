// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0

import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Window
import QtQuick.Layouts


Window {
    id: powerControllWin
    objectName: "powerControllWin"

    modality: Qt.WindowModal
    //固定窗口大小
    minimumWidth: 1110
    maximumWidth: 1110
    minimumHeight: 640
    maximumHeight: 640

    property string objName: "电机"
    visible: true
    title: objName + "控制页面"


    // 右侧控制栏
    GroupBox{
        id: valveControll
        objectName: "valveControll"
        anchors.fill: parent
        anchors.margins: 30
        title: objName + "控制"
        font.pointSize: 20
        anchors.leftMargin: 610
        anchors.bottomMargin: 40
        
        // 右上角单个电机的控制栏
        GroupBox{
            id: singalComponent
            objectName: "singalComponent"
            y:5
            x:0
            width: parent.width
            height: parent.height-75
            title: "当前未选中" + powerControllWin.objName
            property int curentSensor: -1
            font.pointSize: 12
            
            // 手自动切换按钮
            Button {
                id: buttonAuto
                objectName: "buttonAuto"
                x:30
                y: 0
                width: 140
                height: 50
                property int switchState: 0 //0:自动 1:手动
                //icon.height: 40
                //icon.width: 40
                font.pointSize: 12
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
                        buttonAuto.switchState === 0 ? buttonSwitch.enabled = false : buttonSwitch.enabled = true  //自动状态开启后，关闭开启按钮不可选
                        backcolor.color = buttonAuto.switchState === 1 ? "#2a87d7" : "#64fa32"  // 点击后变色
                    }
                }

            }

            
            // 电机开启关闭切换按钮
            Button {
                id: buttonSwitch
                objectName: "buttonSwitch"
                property int switchState: 0 //0:关闭 1:开启
                enabled: false
                x:250
                y: 0
                width: 140
                height: 50
                text: switchState === 0 ? qsTr("已关闭") : qsTr("运行中")
                font.pointSize: 12
                //icon.height: 40
                //icon.width: 40
                icon.source: switchState === 0 ? "file:./ico/guanbi.png" : "file:./ico/kaiqi.png"
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

            // 实际参数展示栏
            GroupBox{
                width: 200
                height: 110
                id: actruePowerParam
                objectName: "actruePowerParam"
                title:"实际参数"
                property int acturePosition: 360  // 0~360
                property int actureSpeed: 1000  // 0~3000
                y:60
                font.pointSize: 11
                Text{
                    text:"实际位置："
                    font.pointSize: 11
                    y:5
                }
                Text{
                    text:"实际速度："
                    font.pointSize: 11
                    y:35
                }
                Text{
                    text:"度"
                    font.pointSize: 11
                    x:120
                    y:5
                }
                Text{
                    text:"转/分钟"
                    font.pointSize: 11
                    x:120
                    y:35
                }
                Text{
                    text:actruePowerParam.acturePosition
                    font.pointSize: 11
                    y:5
                    x:76
                }
                Text{
                    text:actruePowerParam.actureSpeed
                    font.pointSize: 11
                    y:35
                    x:76
                }
            }
            
            // 电机参数修改栏
            GroupBox{
                width: 200
                height:110
                id: setPowerParam
                objectName: "setPowerParam"
                title:"设置参数"
                font.pointSize: 11
                y:60
                x:220
                Text{
                    text:"设置位置："
                    font.pointSize: 11
                    y:5
                }
                Text{
                    text:"设置速度："
                    font.pointSize: 11
                    y:35
                }
                Text{
                    text:"度"
                    font.pointSize: 11
                    x:120
                    y:5
                }
                Text{
                    text:"转/分钟"
                    font.pointSize: 11
                    x:120
                    y:35
                }

                // 速度设置
                Rectangle{
                    x:76
                    y:5
                    width: 40
                    height: 21
                    id: setPositionParam  // 0~360
                    objectName: "setPositionParam"
                    color: "lightgrey"
                    border.color: "grey"
                    TextInput{
                        font.pointSize: 11
                        x:2
                        width: 38
                        height: 21
                    }
                }

                // 位置设置
                Rectangle{
                    x:76
                    y:35
                    width: 40
                    height: 21
                    id: setSpeedParam  // 0~3000
                    objectName: "setSpeedParam"
                    color: "lightgrey"
                    border.color: "grey"
                    TextInput{
                        font.pointSize: 11
                        x:2
                        width: 38
                        height: 21
                    }
                }
            }
            
            // 电机方向设置
            GroupBox{
                width: 200
                height:80
                y:180
                title:"电机方向设置"
                id: powerDirection
                objectName: "powerDirection"
                font.pointSize: 11

                // 电机正反开关
                MySwitch {
                    id: switchPowerDirection
                    anchors.horizontalCenter: parent.horizontalCenter
                    y:5
                }

                Text {
                    text: qsTr("正")
                    font.pointSize: 11
                    x:switchPowerDirection.x-25
                    y:switchPowerDirection.y
                }

                Text {
                    text: qsTr("反")
                    font.pointSize: 11
                    x:switchPowerDirection.x+78
                    y:switchPowerDirection.y
                }
            }
            
            // 电机模式设置
            GroupBox{
                width: 200
                height:80
                x:220
                y:180
                title:"电机模式设置"
                id: powerMode
                objectName: "powerMode"
                font.pointSize: 11

                ComboBox {
                    id: comboBox
                    objectName: "powerMode"
                    x: 0
                    y: -4
                    model:["速度模式","位置模式","点动模式"]
                    font.pointSize: 11
                }
            }

            // 电机报警信息
            GroupBox{
                id: powerAlart
                objectName: "powerAlart"
                width: 420
                height:90
                y:270
                title:"报警显示"
                font.pointSize: 11
                Text{
                    id: powerHandAlart
                    objectName: "powerHandAlart"
                    text: "电机手动报警"
                    font.pointSize: 11
                    x:0
                    y:0
                    color: "gray"
                }
                Text{
                    id: powerAutoAlart
                    objectName: "powerAutoAlart"
                    text: "电机自动报警"
                    font.pointSize: 11
                    x:0
                    y:25
                    color: "gray"
                }
            }
            
        }


        // 一键自动按钮
        Button {
            id: buttonAllAuto
            objectName: "buttonAllAuto"
            x:40
            y:438
            width:140
            height:50

            //icon.height: 40
            //icon.width: 40
            icon.source: "file:./ico/zidong.png"
            highlighted: false
            flat: false
            font.pointSize: 12
            property int allAutoState: 0  // 0:一键自动 1:未一键自动 2:自动中
            text: allAutoState === 1 ? "一键自动" : "全部自动中"


            background: Rectangle {
                id: allAutoColor
                objectName: "allAutoColor"
                color: buttonAllAuto.allAutoState === 1 ? "#d6d7d7" : "#64fa32"
                border.width: 1
                border.color: "black"
                //radius: 5
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

        // 所有轴全部回零
        Button{
            id:moveAxleZero
            objectName: "moveAxleZero"
            x:245
            y:438
            width:160
            height:50
            text: "所有轴全部回零"
            //icon.height: 40
            //icon.width: 40
            icon.source: "file:./ico/backzero.png"
            highlighted: false
            flat: false
            font.pointSize: 12
            property int axleState: 0

            background: Rectangle {
                id: axleZeroColor
                objectName: "axleZeroColor"
                color: moveAxleZero.axleState === 1 ? "#d6d7d7" : "#64fa32"
                border.width: 1
                border.color: "black"
            }

            MouseArea{
                anchors.fill:moveAxleZero
                hoverEnabled: true  // 检测鼠标的进出
                onEntered: {
                    axleZeroColor.color = "#dda4a4a4" // 进入变灰
                }

                onExited: {
                    axleZeroColor.color = moveAxleZero.axleState === 1 ? "#d6d7d7" : "#64fa32" // 退出变回本来颜色
                }

                onClicked: {
                    moveAxleZero.axleState = !moveAxleZero.axleState // 点击后状态反转
                    axleZeroColor.color = moveAxleZero.axleState === 1 ? "#d6d7d7" : "#64fa32"  // 点击后变色
                }
            }
        }
        
    }


    
    // 左侧电机展示栏
    GroupBox{
        id: valveDisplay
        objectName: "valveDisplay"
        anchors.fill: parent
        anchors.margins: 30
        title: powerControllWin.objName + "列表"
        font.pointSize: 20
        anchors.bottomMargin: 40
        anchors.rightMargin: 510

        TabBar {    //点击相应的按钮实现切换
                id: bar
                objectName: "bar"
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
                TabButton {
                    text: qsTr("41~60")
                    font.pointSize: 18
                    background: Rectangle { color: "#bc948e8e" }
                }
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
                    baseIndex: 1
                    listName: powerControllWin.objName
                }
                ComponentList{
                    anchors.leftMargin: 140
                    id: valveListView2
                    baseIndex:6
                    listName: powerControllWin.objName
                }
                ComponentList{
                    anchors.leftMargin: 280
                    id: valveListView3
                    baseIndex:11
                    listName: powerControllWin.objName
                }
                ComponentList{
                    anchors.leftMargin: 420
                    id: valveListView4
                    baseIndex:16
                    listName: powerControllWin.objName
                }
            }
            Rectangle  {
                anchors.fill: stacklaout
                ComponentList{
                    anchors.leftMargin: 0
                    id: valveListView5
                    baseIndex: 21
                    listName: powerControllWin.objName
                }
                ComponentList{
                    anchors.leftMargin: 140
                    id: valveListView6
                    baseIndex:26
                    listName: powerControllWin.objName
                }
                ComponentList{
                    anchors.leftMargin: 280
                    id: valveListView7
                    baseIndex:31
                    listName: powerControllWin.objName
                }
                ComponentList{
                    anchors.leftMargin: 420
                    id: valveListView8
                    baseIndex:36
                    listName: powerControllWin.objName
                }
            }
            Rectangle  {
                anchors.fill: stacklaout
                ComponentList{
                    anchors.leftMargin: 0
                    id: valveListView9
                    baseIndex: 41
                    listName: powerControllWin.objName
                }
                ComponentList{
                    anchors.leftMargin: 140
                    id: valveListView10
                    baseIndex:46
                    listName: powerControllWin.objName
                }
                ComponentList{
                    anchors.leftMargin: 280
                    id: valveListView11
                    baseIndex:51
                    listName: powerControllWin.objName
                }
                ComponentList{
                    anchors.leftMargin: 420
                    id: valveListView12
                    baseIndex:56
                    listName: powerControllWin.objName
                }
            }
            
        }
    }
    
  
}
