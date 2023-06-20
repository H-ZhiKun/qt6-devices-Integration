// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0

import QtQuick 6.2
import QtQuick.Controls 6.2
// import UntitledProject
import QtQuick.Window
import QtQuick.Layouts

Window {
    id: root
    width: 1366
    height: 668
    visible: true
/*
    function loadButton() {
        var component = Qt.createComponent("Alarm.qml");
        if (component.status === Component.Ready) {
            var button = component.createObject(root);
        }
    }*/

    function loadCameraParam() {
        var componentCameraParam = Qt.createComponent("CameraParam.qml");
        var buttonCameraParam = componentCameraParam.createObject(root);

    }

    function loadFormulaPage() {
        var component = Qt.createComponent("FormulaPage.qml");
        if (component.status === Component.Ready) {
            var button2 = component.createObject(root);
        }
    }


    //Component.onCompleted: loadButton()
    Button {
        id: button
        x: 53
        y: 68
        text: qsTr("报警页面")
        onClicked: {
            var component = Qt.createComponent("Alarm.qml")
            var window    = component.createObject(root)
            window.show()
        }
    }

    Button {
        id: button1
        x: 155
        y: 68
        text: qsTr("相机参数")
        onClicked: {
            var component = Qt.createComponent("CameraParam.qml")
            var window    = component.createObject(root)
            window.show()
        }
    }

    Button {
        id: button2
        x: 276
        y: 68
        text: qsTr("配方页面")
        onClicked: {
            loadFormulaPage()
        }
    }

    Button {
        id: button3
        x: 401
        y: 68
        text: qsTr("电机")
        onClicked: {
            var component = Qt.createComponent("PowerPage.qml")
            var window    = component.createObject(root)
            window.show()
        }
    }

    Button {
        id: button4
        x: 541
        y: 68
        text: qsTr("生产数据")
        onClicked: {
            var component = Qt.createComponent("ProduceData.qml")
            var window    = component.createObject(root)
            window.show()
        }
    }

    Button {
        id: button6
        x: 53
        y: 165
        text: qsTr("传感器")
        onClicked: {
            var component = Qt.createComponent("SensorPage.qml")
            var window    = component.createObject(root)
            window.show()
        }
    }

    Button {
        id: button7
        x: 155
        y: 165
        text: qsTr("用户管理")
        onClicked: {
            var component = Qt.createComponent("UserPage.qml")
            var window    = component.createObject(root)
            window.show()
        }
    }

    Button {
        id: button8
        x: 276
        y: 165
        text: qsTr("阀门")
        onClicked: {
            var component = Qt.createComponent("ValvePage.qml")
            var window    = component.createObject(root)
            window.show()
        }
    }


}
