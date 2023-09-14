// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0

import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Window
import QtQuick.Layouts

GroupBox {
    id: valveControllWin
    objectName: "valveControllWin"
    width: 1110  //Screen.desktopAvailableWidth
    height: 640  //Screen.desktopAvailableHeight
    property string objName: "阀门"
    property int curPage: 0
    property bool openFlag: true
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
            property var curItem: valveListView1.itemAtIndex(0)
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

            MySwitch {
                id: switchModel
                anchors.horizontalCenter: parent.horizontalCenter
                y: 126
                anchors.horizontalCenterOffset: 0
                focusPolicy: Qt.NoFocus
                onClicked: {
                    var handMoveAddr = singalComponent.curItem.handMoveAddr;
                    var json = {
                        [handMoveAddr]: switchModel.checked ? "0" : "1"
                    };
                    var jsRet = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.WritePLC, JSON.stringify(json));
                    var result = JSON.parse(jsRet);
                    if (result.ok === true) {
                        setInfo.text = "保存失败！";
                        setInfo.color = "red";
                    } else {
                        setInfo.text = "保存成功！";
                        setInfo.color = "green";
                    }
                }
            }

            Text {
                text: qsTr("自动")
                font.pointSize: 11
                x: switchModel.x - 50
                y: switchModel.y
            }

            Text {
                text: qsTr("点动")
                font.pointSize: 11
                x: switchModel.x + 83
                y: switchModel.y
            }
        }

        Label {
            id: setInfo
            x: 10
            y: 467
            text: qsTr("")
            font.pointSize: 15
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

        StackLayout {
            //栈布局管理器
            id: stacklaout
            objectName: "stacklaout"
            width: parent.width
            y: 60
            x: 0
            height: parent.height - 50
            Rectangle {
                color: Qt.rgba(248 / 255, 248 / 255, 248 / 255, 1)
                //anchors.fill: stacklaout
                ComponentList {
                    id: valveListView1
                    anchors.leftMargin: 130
                    objectName: "valveListView1"
                    baseIndex: 1
                    listName: valveControllWin.objName
                }
                ComponentList {
                    id: valveListView2
                    anchors.leftMargin: 265
                    objectName: "valveListView2"
                    baseIndex: 6
                    listName: valveControllWin.objName
                    model: 1
                }
            }
        }
    }

    function requireValveState() {
        var myJsonObject = {};
        var i = 0;
        var j = 0;
        for (i = 0; i < 5; i++) {
            if (valveListView1.itemAtIndex(i) !== null) {
                myJsonObject[valveListView1.itemAtIndex(i).stateAddr] = j.toString();
                j = j + 1;
            }
        }
        for (i = 0; i < 5; i++) {
            if (valveListView2.itemAtIndex(i) !== null) {
                myJsonObject[valveListView2.itemAtIndex(i).stateAddr] = j.toString();
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
                if (k >= 5 && k < 10) {
                    if (key === "0") {
                        valveListView2.itemAtIndex(k - 5).imageSource = "file:///" + appdir + "/ico/red.png";
                    } else {
                        valveListView2.itemAtIndex(k - 5).imageSource = "file:///" + appdir + "/ico/green.png";
                    }
                }
            }
        }
    }

    function initValve() {
        var item0 = valveListView1.itemAtIndex(0);
        var item1 = valveListView1.itemAtIndex(1);
        var item2 = valveListView1.itemAtIndex(2);
        var item3 = valveListView1.itemAtIndex(3);
        var item4 = valveListView1.itemAtIndex(4);
        var item5 = valveListView2.itemAtIndex(0);
        if (item0 === null) {
            console.log("阀1 no item");
        } else {
            item0.itemName = "阀1";
            item0.handMoveAddr = "XY001_di_0070";
        }
        if (item1 === null) {
            console.log("阀2 no item");
        } else {
            item1.itemName = "阀2";
            item1.handMoveAddr = "XY002_di_0072";
        }
        if (item2 === null) {
            console.log("阀3 no item");
        } else {
            item2.itemName = "阀3";
            item2.handMoveAddr = "XY003_di_0074";
        }
        if (item3 === null) {
            console.log("阀4 no item");
        } else {
            item3.itemName = "阀4";
            item3.handMoveAddr = "XY004_di_0076";
        }
        if (item4 === null) {
            console.log("阀5 no item");
        } else {
            item4.itemName = "阀5";
            item4.handMoveAddr = "XY005_di_0078";
        }
        if (item5 === null) {
            console.log("阀6 no item");
        } else {
            item5.itemName = "阀6";
            item5.handMoveAddr = "XY006_di_0080";
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
