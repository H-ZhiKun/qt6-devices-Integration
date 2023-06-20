// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0

import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Window
import QtQuick.Layouts

Window {
    id: root
    width: 1366
    height: 768
    visible: true
    title: "德图福思"

    TabBar {
        id: bar
        width: cameraBtn.width
        TabButton {
            id: cameraBtn
            text: qsTr("  相    机  ")
            width: root.width/8
            height: root.height/9
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            icon.source: "file:./ico/xiangji.png"
            icon.height: 40
            icon.width: 40
            font.pointSize: 14
        }
        TabButton {
            id: produceDataBtn
            text: qsTr("生产数据")
            font.pointSize: 14
            width: root.width/8
            height: root.height/9
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: cameraBtn.bottom
            icon.source: "file:./ico/shuju.png"
            icon.height: 40
            icon.width: 40
        }
        TabButton {
            id: alarmBtn
            text: qsTr("报警显示")
            font.pointSize: 14
            width: root.width/8
            height: root.height/9
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: produceDataBtn.bottom
            icon.source: "file:./ico/alarm.png"
            icon.height: 40
            icon.width: 40
        }
        TabButton {
            id: formulaBtn
            text: qsTr("  配    方  ")
            font.pointSize: 14
            width: root.width/8
            height: root.height/9
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: alarmBtn.bottom
            icon.source: "file:./ico/peifang.png"
            icon.height: 40
            icon.width: 40
        }
        TabButton {
            id: sensorBtn
            text: qsTr(" 传 感 器 ")
            font.pointSize: 14
            width: root.width/8
            height: root.height/9
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: formulaBtn.bottom
            icon.source: "file:./ico/chuanganqi.png"
            icon.height: 40
            icon.width: 40
        }
        TabButton {
            id: valveBtn
            text: qsTr("  阀    门  ")
            font.pointSize: 14
            width: root.width/8
            height: root.height/9
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: sensorBtn.bottom
            icon.source: "file:./ico/famen.png"
            icon.height: 40
            icon.width: 40
        }
        TabButton {
            id: powerBtn
            text: qsTr("  电    机  ")
            font.pointSize: 14
            width: root.width/8
            height: root.height/9
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: valveBtn.bottom
            icon.source: "file:./ico/dianji.png"
            icon.height: 40
            icon.width: 40
        }
        TabButton {
            id: thirdBtn
            text: qsTr("用户管理")
            font.pointSize: 14
            width: root.width/8
            height: root.height/9
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: powerBtn.bottom
            icon.source: "file:./ico/yonghuguanli.png"
            icon.height: 40
            icon.width: 40
        }
        TabButton {
            id: operateBtn
            text: qsTr("操作日志")
            font.pointSize: 14
            width: root.width/8
            height: root.height/9
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: thirdBtn.bottom
            icon.source: "file:./ico/caozuorizhi.png"
            icon.height: 40
            icon.width: 40
        }

    }


    StackLayout {
        id: view
        width: parent.width
        currentIndex: bar.currentIndex
        // 相机
        Item {
            id: cameraTab
            Master{
                x:190
                y:50
            }
        }
        // 生产数据
        Item {
            id: produceDataTab
            ProduceData{
                x:190
                y:50
            }
        }

        // 报警显示
        Item {
            id: alarmTab
            Alarm{
                x:190
                y:50
            }
        }
        // 配方
        Item {
            id: formulaPageTab
            FormulaPage{
                x:190
                y:50
            }
        }
        // 传感器
        Item {
            id: sensorPageTab
            SensorPage{
                x:190
                y:50
            }
        }
        // 阀门
        Item {
            id: valvePageTab
            ValvePage{
                x:190
                y:50
            }
        }
        // 电机
        Item {
            id: powerPageTab
            PowerPage{
                x:190
                y:50
            }
        }
        // 用户管理
        Item {
            id: userPageTab
            UserPage{
                x:190
                y:50
            }
        }
        // 操作日志
        Item {
            id: nonepage
            Text{
                text:" "
            }
        }

    }

}
