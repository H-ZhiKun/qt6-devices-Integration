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
    property int curPage: 0

    //    modality: Qt.WindowModal
    //    //固定窗口大小
    //    minimumWidth: 1110
    //    maximumWidth: 1110
    //    minimumHeight: 640
    //    maximumHeight: 640

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
            height: parent.height - 70
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

            // 实际参数展示栏
            GroupBox {
                id: actruePowerParam
                width: 200
                height: 90
                objectName: "actruePowerParam"
                title: "实际参数"
                property string acturePosition: ""  // 0~360
                property string actureSpeed: ""  // 0~3000
                y: 10
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
                    text: "实际速度："
                    font.pointSize: 11
                    y: 3
                }

                Text {
                    text: "转/分钟"
                    font.pointSize: 11
                    x: 120
                    y: 3
                }

                Text {
                    text: actruePowerParam.actureSpeed
                    font.pointSize: 11
                    y: 3
                    x: 76
                }
            }

            // 电机参数修改栏
            GroupBox {
                id: setPowerParam
                width: 200
                height: 90
                objectName: "setPowerParam"
                title: "设置参数"
                font.pointSize: 11
                y: 10
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
                    text: "速度："
                    font.pointSize: 11
                    y: 3
                }

                Text {
                    text: "转/分钟"
                    font.pointSize: 11
                    x: 120
                    y: 3
                }

                // 速度设置
                TextArea {
                    id: setSpeedParam  // 0~3000
                    x: 40
                    y: 0
                    text: actruePowerParam.actureSpeed
                    width: 70
                    height: 28
                }
            }

            // 电机方向设置
            GroupBox {
                id: powerDirection
                width: 200
                height: 80
                y: 120
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
                }

                Text {
                    text: qsTr("自动")
                    font.pointSize: 11
                    x: switchPowerDirection.x - 45
                    y: switchPowerDirection.y
                }

                Text {
                    text: qsTr("点动")
                    font.pointSize: 11
                    x: switchPowerDirection.x + 78
                    y: switchPowerDirection.y
                }
            }

            // 电机模式设置

            // 电机报警信息
            GroupBox {
                id: powerAlart
                objectName: "powerAlart"
                width: 200
                height: 90
                y: 245
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
                width: 200
                height: 90
                x: 220
                y: 245
                background: Rectangle {
                    anchors.fill: parent
                    anchors.topMargin: 30
                    border.color: "gray"
                    border.width: 0
                    radius: 10
                    color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
                }
                Button {
                    id: resetButton
                    width: 110
                    y: 27
                    x: 10
                    text: qsTr("故障复位")
                    onClicked: {
                        var resetAddr = singalComponent.curItem.resetAddr;
                        var json = {
                            [resetAddr]: "1"
                        };
                        var jsRet = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.WritePLC, JSON.stringify(json));
                        var result = JSON.parse(jsRet);
                        if (result.ok === true) {
                            setInfo.text = "复位成功！";
                            setInfo.color = "green";
                        } else {
                            setInfo.text = "复位失败！";
                            setInfo.color = "red";
                        }
                    }
                }
            }

            ToolSeparator {
                id: toolSeparator
                x: 190
                y: 14
                height: 430
                rotation: 90
            }
        }

        Button {
            id: saveButton
            width: 110
            y: 445
            x: 150
            text: qsTr("保存设置")
            icon.source: "file:///" + appdir + "/ico/baocun.png"
            font.pointSize: 12
            onClicked: {
                var handMoveAddr = singalComponent.curItem.handMoveAddr;
                if (Number(setSpeedParam.text) <= 3000 && Number(setSpeedParam.text) >= 0) {
                    var setSpeedAddr = singalComponent.curItem.setSpeedAddr;
                } else {
                    setInfo.text = "速度参数超出阈值！";
                    return;
                }
                var json = {
                    [handMoveAddr]: switchPowerDirection.checked ? "1" : "0",
                    [setSpeedAddr]: setSpeedParam.text
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
            x: 264
            y: 453
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

        StackLayout {
            //栈布局管理器
            id: stacklaout
            objectName: "stacklaout"
            width: parent.width
            y: 60
            x: 0
            height: parent.height - 100
            Rectangle {
                // anchors.fill: stacklaout
                color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
                ComponentList {
                    id: valveListView1
                    anchors.leftMargin: 0
                    baseIndex: 1
                    listName: valveControllWin.objName
                    singleWidth: 300
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

    function requirePowerState() {
        var myJsonObject = {};
        var i = 0;
        var j = 0;
        for (i = 0; i < 5; i++) {
            if (valveListView1.itemAtIndex(i) !== null) {
                myJsonObject[valveListView1.itemAtIndex(i).enableAddr] = j.toString();
                j = j + 1;
            }
        }
        var strSend = JSON.stringify(myJsonObject);
        var jsRet = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.ReadPLC, strSend);
        var result = JSON.parse(jsRet);
        if (result.ok === true) {
            var keys = Object.values(result.details);
            for (var k = 0; k < keys.length; k++) {
                var key = keys[k];
                if (k < 5) {
                    //console.log("keys: ", key);
                    if (key === "0") {
                        valveListView1.itemAtIndex(k).imageSource = "file:///" + appdir + "/ico/red.png";
                    } else {
                        valveListView1.itemAtIndex(k).imageSource = "file:///" + appdir + "/ico/green.png";
                    }
                }
            }
        }
    }
    // 这样写的原因： 地址不连续，建立映射关系
    function initPower() {
        var item0 = valveListView1.itemAtIndex(0);
        var item1 = valveListView1.itemAtIndex(1);
        var item2 = valveListView1.itemAtIndex(2);
        var item3 = valveListView1.itemAtIndex(3);
        var item4 = valveListView1.itemAtIndex(4);
        if (item0 === null) {
            console.log("M001 no item");
        } else {
            item0.itemName = "进料网带电机";
            item0.handMoveAddr = "M001_n_0024";
            item0.realSpeedAddr = "M001_di_0027";
            item0.setSpeedAddr = "M001_di_0027";
            item0.enableAddr = "M001_n_0029"; // 驱动已使能
            item0.failureAddr = "M001_n_0031"; // 驱动故障
            item0.resetAddr = "M001_n_0032"; // 故障复位
        }
        if (item1 === null) {
            console.log("M002 no item");
        } else {
            item1.itemName = "分距夹持网带电机1";
            item1.handMoveAddr = "M001_n_0033";
            item1.realSpeedAddr = "M001_di_0036";
            item1.setSpeedAddr = "M001_di_0036";
            item1.enableAddr = "M001_n_0038"; // 驱动已使能
            item1.failureAddr = "M001_n_0040"; // 驱动故障
            item1.resetAddr = "M001_n_0041"; // 故障复位
        }
        if (item2 === null) {
            console.log("M003 no item");
        } else {
            item2.itemName = "分距夹持网带电机2";
            item2.handMoveAddr = "M001_n_0042";
            item2.realSpeedAddr = "M001_di_0045";
            item2.setSpeedAddr = "M001_di_0045";
            item2.enableAddr = "M001_n_0047"; // 驱动已使能
            item2.failureAddr = "M001_n_0049"; // 驱动故障
            item2.resetAddr = "M001_n_0050"; // 故障复位
        }
        if (item3 === null) {
            console.log("M004 no item");
        } else {
            item3.itemName = "姿态保持夹持网带电机1";
            item3.handMoveAddr = "M001_n_0051";
            item3.realSpeedAddr = "M001_di_0054";
            item3.setSpeedAddr = "M001_di_0054";
            item3.enableAddr = "M001_n_0056"; // 驱动已使能
            item3.failureAddr = "M001_n_0058"; // 驱动故障
            item3.resetAddr = "M001_n_0059"; // 故障复位
        }
        if (item4 === null) {
            console.log("M005 no item");
        } else {
            item4.itemName = "姿态保持夹持网带电机2";
            item4.handMoveAddr = "M001_n_0060";
            item4.realSpeedAddr = "M001_di_0063";
            item4.setSpeedAddr = "M001_di_0063";
            item4.enableAddr = "M001_n_0065"; // 驱动已使能
            item4.failureAddr = "M001_n_0067"; // 驱动故障
            item4.resetAddr = "M001_n_0068"; // 故障复位
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
