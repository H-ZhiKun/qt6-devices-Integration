import AppFrame 1.0
import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects

GroupBox {
    //x:0
    //y:0
    id: cameraPage
    width: 1110 //Screen.desktopAvailableWidth
    height: 640 //Screen.desktopAvailableHeight
    visible: true
    property int cbxLastIndex: 0
    property alias timeTask: timeCamera
    background: Rectangle {
        anchors.fill: parent
        border.color: "gray"
        border.width: 1
        radius: 10
        color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
    }

    Rectangle {
        id: frame2
        x: 22
        y: 95
        width: 450
        height: 450
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
            anchors.rightMargin: 0
            anchors.bottomMargin: 0
        }

        ImagePainter {
            id: ocr
            objectName: "OCR"
            anchors.fill: parent
            anchors.margins: 8
            //anchors.topMargin: 35
        }

        Text {
            x: 40
            y: -20
            text: "打码复核相机"
            font.pointSize: 10
        }

        Image {
            id: imageLocate
            x: 10
            y: -25
            width: 28
            height: 26
            mipmap: true
            fillMode: Image.PreserveAspectFit
            source: "file:///" + appdir + "/ico/red.png"
        }
    }

    GroupBox {
        id: countData
        x: 771
        y: 228
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
        x: 782
        y: 144
        text: qsTr("设备步骤：")
        font.pointSize: 13
    }

    Text {
        id: equipmentSteps
        x: 872
        y: 145
        text: textEquipmentSteps
        font.pointSize: 11
    }

    // Button {
    //     id: button
    //     x: 549
    //     y: 367
    //     width: 111
    //     height: 37
    //     text: qsTr("相机参数设置")
    //     onClicked: {
    //         var component = Qt.createComponent("StrightCameraParam.qml");
    //         var window = component.createObject(cameraPage);
    //         window.show();
    //     }
    // }

    Button {
        id: button1
        x: 548
        y: 483
        width: 112
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
        x: 548
        y: 424
        width: 112
        height: 32
        text: qsTr("点动模式")
        property bool pointMode: false
        onClicked: {
            pointMode = !pointMode;
            var json = {};
            if (pointMode) {
                json["point_di_0001"] = "1";
                autoPointMode = 1;
                text = "点动中";
            } else {
                json["point_di_0001"] = "2";
                autoPointMode = 2;
                text = "点动模式";
            }
            var jsRet = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.WritePLC, JSON.stringify(json));
        }
    }

    Text {
        id: textPrepare1
        x: 585
        y: 160
        width: 38
        height: 19
        text: qsTr("PLC:")
        font.pointSize: 11
    }

    Text {
        id: textPrepare2
        x: 537
        y: 199
        width: 94
        height: 19
        text: qsTr("激光打码机:")
        font.pointSize: 11
    }

    Text {
        id: textPrepare3
        x: 538
        y: 243
        width: 90
        height: 19
        text: qsTr("普什服务器:")
        font.pointSize: 11
    }

    Text {
        id: textPrepare4
        x: 565
        y: 286
        width: 63
        height: 19
        text: qsTr("扫码机:")
        font.pointSize: 11
    }

    Image {
        id: imageplc
        x: 638
        y: 157
        width: 28
        height: 26
        fillMode: Image.PreserveAspectFit
        mipmap: true
    }

    Image {
        id: imagedomino
        x: 638
        y: 196
        width: 28
        height: 26
        fillMode: Image.PreserveAspectFit
        mipmap: true
    }

    Image {
        id: imagepermission
        x: 638
        y: 243
        width: 28
        height: 26
        fillMode: Image.PreserveAspectFit
        mipmap: true
    }

    Image {
        id: imagecognex
        x: 638
        y: 282
        width: 28
        height: 26
        fillMode: Image.PreserveAspectFit
        mipmap: true
    }

    Timer {
        id: timeCamera
        interval: 1000//设置定时器定时时间为500ms,默认1000ms
        repeat: true //是否重复定时,默认为false
        running: true //是否开启定时，默认是false，当为true的时候，进入此界面就开始定时
        triggeredOnStart: false // 是否开启定时就触发onTriggered，一些特殊用户可以用来设置初始值。
        onTriggered: {
            refreshItemState();
        }
    }

    function refreshItemState() {
        var jsRet = appMetaFlash.qmlCallExpected(StrightMainWindow.ExpectedFunction.RefreshStrightMainPage, "");
        var result = JSON.parse(jsRet);
        if (result.ok) {
            result.details.dominoState === "1" ? imagedomino.source = "file:///" + appdir + "/ico/green.png" : imagedomino.source = "file:///" + appdir + "/ico/red.png";
            result.details.cognexState === "1" ? imagecognex.source = "file:///" + appdir + "/ico/green.png" : imagecognex.source = "file:///" + appdir + "/ico/red.png";
            result.details.permissionState === "1" ? imagepermission.source = "file:///" + appdir + "/ico/green.png" : imagepermission.source = "file:///" + appdir + "/ico/red.png";
            result.details.plcState === "1" ? imageplc.source = "file:///" + appdir + "/ico/green.png" : imageplc.source = "file:///" + appdir + "/ico/red.png";
            result.details.image0 === "1" ? imageLocate.source = "file:///" + appdir + "/ico/green.png" : imageplc.source = "file:///" + appdir + "/ico/red.png";
            textEquipmentSteps = result.details.textEquipmentSteps;
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
