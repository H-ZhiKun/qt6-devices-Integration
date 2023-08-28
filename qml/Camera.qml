import AppFrame 1.0
import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects

GroupBox {
    //x:0
    //y:0
    id: cameraPage
    width: 1110  //Screen.desktopAvailableWidth
    height: 640  //Screen.desktopAvailableHeight
    visible: true
    property int cbxLastIndex: 0
    background: Rectangle {
        anchors.fill: parent
        border.color: "gray"
        border.width: 1
        radius: 10
        color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
    }
    Rectangle {
        id: frameLocatinCamera
        x: 47
        y: 21
        width: 290
        height: 290
        radius: 5

        // title: "定位相机"
        Rectangle {
            id: rectLocatinCamera
            width: parent.width - 10
            height: parent.height - 10
            anchors.centerIn: parent
            color: "#f7f7f7"
            radius: 5
        }

        DropShadow {
            anchors.fill: rectLocatinCamera
            radius: 8.0
            samples: 16
            color: "#dd000000"
            source: rectLocatinCamera
        }
        ImagePainter {
            id: locationCamera
            objectName: "locationCamera"
            anchors.fill: parent
            anchors.margins: 8
            //anchors.topMargin: 20
        }

        Text {
            x: 10
            y: -20
            text: "定位相机"
            font.pointSize: 10
        }
    }

    Rectangle {
        id: frame1
        x: 47
        y: 328
        width: 290
        height: 290
        radius: 5
        // title: "定位复核相机"
        Rectangle {
            id: rectframe1
            width: parent.width - 10

            height: parent.height - 10
            anchors.centerIn: parent
            color: "#f7f7f7"
            radius: 5
        }

        DropShadow {
            anchors.fill: rectframe1
            radius: 8.0
            samples: 16
            color: "#dd000000"
            source: rectframe1
        }

        ImagePainter {
            id: locateCheckCamera
            objectName: "locateCheckCamera"
            anchors.fill: parent
            anchors.margins: 8
            //anchors.topMargin: 35
        }

        Text {
            x: 10
            y: -20
            text: "定位复核相机"
            font.pointSize: 10
        }
    }

    Rectangle {
        id: frame2
        x: 425
        y: 21
        width: 290
        height: 290
        radius: 5
        // title: "打码复核相机"
        Rectangle {
            id: rectframe2
            width: parent.width - 10
            height: parent.height - 10
            anchors.centerIn: parent
            color: "#f7f7f7"
            radius: 5
        }

        DropShadow {
            anchors.fill: rectframe2
            radius: 8.0
            samples: 16
            color: "#dd000000"
            source: rectframe2
        }
        ImagePainter {
            id: codeCheckCamera
            objectName: "codeCheckCamera"
            anchors.fill: parent
            anchors.margins: 8
            //anchors.topMargin: 35
        }

        Text {
            x: 10
            y: -20
            text: "打码复核相机"
            font.pointSize: 10
        }
    }

    GroupBox {
        id: formulaData
        x: 798
        y: 119
        width: 255
        height: 103
        font.pointSize: 12
        background: Rectangle {
            anchors.fill: parent
            anchors.topMargin: 35
            border.color: "gray"
            border.width: 1
            radius: 10
            color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
        }
        Image {
            x: -10
            y: -45
            width: 20
            height: 20
            mipmap: true
            source: "file:///" + appdir + "/ico/peifang.png"
        }

        Text {
            x: 0
            y: 0
            text: "当前配方名字："
            font.pointSize: 11
        }

        Text {
            x: 0
            y: 30
            text: "当前配方速度："
            font.pointSize: 11
        }

        Text {
            id: curFormulaName
            x: 100
            y: 0
            width: 280
            text: master.formulaName
            font.pointSize: 11
            clip: true
        }

        Text {
            id: curFormulaSpeed
            x: 100
            y: 30
            text: master.formulaSpeed
            font.pointSize: 11
        }

        Text {
            x: 184
            y: 30
            text: "瓶/小时"
            font.pointSize: 11
        }
        title: "   配方信息"
    }

    GroupBox {
        id: countData
        x: 798
        y: 272
        width: 255
        height: 303
        font.pointSize: 12
        background: Rectangle {
            anchors.fill: parent
            anchors.topMargin: 35
            border.color: "gray"
            border.width: 1
            radius: 10
            color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
        }
        Image {
            x: -10
            y: -45
            width: 24
            height: 24
            mipmap: true
            source: "file:///" + appdir + "/ico/shuju1.png"
        }

        Text {
            x: 0
            y: 5
            text: "进瓶数："
            font.pointSize: 11
        }

        Text {
            x: 0
            y: 35
            text: "合格品数："
            font.pointSize: 11
        }

        Text {
            x: 0
            y: 65
            text: "废品总数："
            font.pointSize: 11
        }

        Text {
            x: 0
            y: 95
            text: "定位废品数："
            font.pointSize: 11
        }

        Text {
            x: 0
            y: 125
            text: "喷码废品数："
            font.pointSize: 11
        }

        Text {
            x: 0
            y: 155
            text: "暂停/终止废品数："
            font.pointSize: 11
        }

        Text {
            id: countAll
            x: 75
            y: 5
            text: textCountAll
            font.pointSize: 11
        }

        Text {
            id: countPass
            x: 75
            y: 35
            text: textCountPass
            font.pointSize: 11
        }

        Text {
            id: countWaste
            x: 75
            y: 65
            text: textCountWaste
            font.pointSize: 11
        }

        Text {
            id: countLocateWaste
            x: 122
            y: 95
            text: textCountLocateWaste
            font.pointSize: 11
        }

        Text {
            id: countCodeWaste
            x: 122
            y: 125
            text: textCountCodeWaste
            font.pointSize: 11
        }

        Text {
            id: countPauseWaste
            x: 122
            y: 155
            text: textCountPauseWaste
            font.pointSize: 11
        }

        Text {
            x: 187
            y: 5
            text: "\u74f6"
            font.pointSize: 11
        }

        Text {
            x: 187
            y: 35
            text: "\u74f6"
            font.pointSize: 11
        }

        Text {
            x: 187
            y: 65
            text: "\u74f6"
            font.pointSize: 11
        }

        Text {
            x: 187
            y: 95
            text: "\u74f6"
            font.pointSize: 11
        }

        Text {
            x: 187
            y: 125
            text: "\u74f6"
            font.pointSize: 11
        }

        Text {
            x: 187
            y: 155
            text: "瓶"
            font.pointSize: 11
        }

        Button {
            x: 0
            y: 196
            width: 130
            text: "计数清零"
            icon.width: 35
            icon.source: "file:///" + appdir + "/ico/zerored.png"
            font.pointSize: 11
            icon.color: "#ddfb0a0a"
            icon.height: 35
        }
        title: "   计数信息"
    }

    Image {
        id: image
        x: 798
        y: -4
        width: 255
        height: 105
        mipmap: true
        source: "file:///" + appdir + "/ico/DTFSlogo.png"
        fillMode: Image.PreserveAspectFit
    }

    Text {
        x: 425
        y: 373
        text: qsTr("生产状态：")
        font.pointSize: 13
    }

    Text {
        id: textPrepare
        x: 515
        y: 389
        text: qsTr("准备")
        color: textProduceState === 0 ? "green" : "gray"
        font.pointSize: 11
    }

    Text {
        id: textRun
        x: 572
        y: 389
        text: qsTr("运行")
        color: textProduceState === 1 ? "green" : "gray"
        font.pointSize: 11
    }

    Text {
        id: textPause
        x: 629
        y: 389
        text: qsTr("暂停")
        color: textProduceState === 2 ? "green" : "gray"
        font.pointSize: 11
    }

    Text {
        id: textStop
        x: 686
        y: 389
        text: qsTr("终止")
        color: textProduceState === 3 ? "red" : "gray"
        font.pointSize: 11
    }

    Text {
        x: 425
        y: 450
        text: qsTr("设备步骤：")
        font.pointSize: 13
    }

    Text {
        id: equipmentSteps
        x: 515
        y: 451
        text: textEquipmentSteps
        font.pointSize: 11
    }

    Image {
        id: producePrepare
        x: 515
        y: 359
        width: 30
        height: 30
        property int state: 0
        mipmap: true
        source: "file:///" + appdir + "/ico/zhunbeizhong.png"
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: produceRun
        x: 572
        y: 359
        width: 30
        height: 30
        mipmap: true
        source: "file:///" + appdir + "/ico/yunxing.png"
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: producePause
        x: 629
        y: 359
        width: 30
        height: 30
        mipmap: true
        source: "file:///" + appdir + "/ico/zanting.png"
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: produceStop
        x: 686
        y: 359
        width: 30
        height: 30
        mipmap: true
        source: textProduceState === 3 ? "file:///" + appdir + "/ico/tingzhi.png" : "file:///" + appdir + "/ico/zhongzhiblack.png"
        fillMode: Image.PreserveAspectFit
    }

    Button {
        id: button
        x: 425
        y: 513
        width: 111
        height: 37
        text: qsTr("相机参数设置")
        onClicked: {
            var component = Qt.createComponent("CameraParam.qml");
            var window = component.createObject(cameraPage);
            window.show();
        }
    }

    Button {
        id: button1
        x: 560
        y: 512
        width: 108
        height: 38
        text: qsTr("保存图像")
        onClicked: {
            var json = {
                "name": "保存图像"
            };
            var jsRet = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.CollectImage, JSON.stringify(json));
            var result = JSON.parse(jsRet);
            if (result.ok === true) {
                // 保存成功
                console.log("save success");
            } else {
                console.log("save failed");
            }
        }
    }

    Button {
        id: button2
        x: 424
        y: 570
        width: 112
        height: 32
        text: qsTr("点动模式")
        property bool pointMode: false
        onClicked: {
            pointMode = !pointMode;
            var json = {};
            if (pointMode) {
                json["point_b_12992_00"] = "1";
                text = "点动中";
            } else {
                json["point_b_12992_00"] = "0";
                text = "点动模式";
            }
            var jsRet = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.WritePLC, JSON.stringify(json));
        }
    }

    Connections {
        target: appMetaFlash // C++ 对象实例
        function onPageMainChange(value) {
            // 执行其他操作...
            var jsonData = JSON.parse(value);
            var val = jsonData.valve;
            textCountAll = jsonData.count_all;
            textCountPass = jsonData.count_pass;
            textCountWaste = jsonData.count_waste;
            textCountLocateWaste = jsonData.count_locate_waste;
            textCountCodeWaste = jsonData.count_code_waste;
            textCountPauseWaste = jsonData.count_pause_waste;
            textEquipmentSteps = jsonData.equipmentSteps;
            textProduceState = jsonData.produceState;
        }
    }
}
