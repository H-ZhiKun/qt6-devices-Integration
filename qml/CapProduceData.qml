import QtQuick 2.15
import QtQuick.Controls 2.15

GroupBox {
    id: produceDataWin
    width: 1110  //Screen.desktopAvailableWidth
    height: 640  //Screen.desktopAvailableHeight

    property alias producePrepareState: producePrepare.state        // 准备生产状态
    property alias produceRunState: produceRun.state                // 运行生产状态
    property alias produceEMERGENCYSTOP: produceEMERGENCYSTOP.state                //急停生产
    property alias producePauseState: producePause.state            // 暂停生产状态
    property alias produceStopState: produceStop.state              // 终止生产状态
    property alias timeTask: timeProduce

    background: Rectangle {
        anchors.fill: parent
        border.color: "gray"
        border.width: 1
        radius: 10
        color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
    }

    GroupBox {
        id: elec
        objectName: "elec"
        anchors.left: parent.left
        anchors.leftMargin: 70
        y: 50
        width: 483
        height: 434
        title: "    电能"
        background: Rectangle {
            anchors.fill: parent
            anchors.topMargin: 35
            border.color: "gray"
            border.width: 1
            radius: 10
            color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
        }

        Image {
            source: "file:///" + appdir + "/ico/dianneng.png"
            width: 35
            height: 40
            mipmap: true
            x: -15
            y: -54
        }
        font.pointSize: 12
        property string kw: "千瓦"
        property string kwh: "千瓦时"
        property string kva: "千伏安"
        property string an: "安"
        property string v: "伏"
        property string degree: "度"

        //property alias name: value

        Text {
            text: "正向有功电能："
            font.pointSize: 11
            y: 40
            x: 10
        }

        Text {
            text: "反向有功电能："
            font.pointSize: 11
            y: 100
            x: 10
        }

        Text {
            text: "A相电压："
            font.pointSize: 11
            y: 160
            x: 10
        }

        Text {
            text: "B相电压："
            font.pointSize: 11
            y: 205
            x: 10
        }

        Text {
            text: "C相电压："
            font.pointSize: 11
            y: 250
            x: 10
        }

        Text {
            text: "温度："
            font.pointSize: 11
            y: 320
            x: 10
        }

        Text {
            text: "总有功功率："
            font.pointSize: 11
            y: 40
            x: 240
        }

        Text {
            text: "总视在功率："
            font.pointSize: 11
            y: 70
            x: 240
        }

        Text {
            text: "组合有功电能："
            font.pointSize: 11
            y: 100
            x: 240
        }

        Text {
            text: "A相电流："
            font.pointSize: 11
            y: 160
            x: 240
        }

        Text {
            text: "B相电流："
            font.pointSize: 11
            y: 205
            x: 240
        }

        Text {
            text: "C相电流："
            font.pointSize: 11
            y: 250
            x: 240
        }

        Text {
            text: "湿度："
            font.pointSize: 11
            y: 320
            x: 240
        }

        Text {
            text: ""
            font.pointSize: 11
            y: 0
            x: 40
        }

        Text {
            // 正向有功电能，保留2位小数
            id: positiveActiveEnergy
            text: ""
            font.pointSize: 11
            y: 40
            x: 120
        }

        Text {
            // 反向有功电能，保留2位小数
            id: reverseActiveEnergy
            text: ""
            font.pointSize: 11
            y: 100
            x: 120
        }

        Text {
            // A相电压，保留1位小数
            id: aPhaseVoltage
            text: ""
            font.pointSize: 11
            y: 160
            x: 80
        }

        Text {
            // B相电压，保留1位小数
            id: bPhaseVoltage
            text: ""
            font.pointSize: 11
            y: 205
            x: 80
        }

        Text {
            // C相电压，保留1位小数
            id: cPhaseVoltage
            text: ""
            font.pointSize: 11
            y: 250
            x: 80
        }

        Text {
            // 温度，保留1位小数
            id: temperature
            text: ""
            font.pointSize: 11
            y: 320
            x: 60
        }

        Text {
            // 总有功功率，保留3位小数
            id: totalActivePower
            text: ""
            font.pointSize: 11
            y: 40
            x: 330
        }

        Text {
            // 总视在功率，保留3位小数
            id: totalApparentPower
            text: ""
            font.pointSize: 11
            y: 70
            x: 330
        }

        Text {
            // 组合有功电能，保留2位小数
            id: combinedActiveEnergy
            text: ""
            font.pointSize: 11
            y: 100
            x: 330
        }

        Text {
            // A向电流，保留2位小数
            id: aDirectionCurrent
            text: ""
            font.pointSize: 11
            y: 160
            x: 310
        }

        Text {
            // B向电流，保留2位小数
            id: bDirectionCurrent
            text: ""
            font.pointSize: 11
            y: 205
            x: 310
        }

        Text {
            // C向电流，保留2位小数
            id: cDirectionCurrent
            text: ""
            font.pointSize: 11
            y: 250
            x: 310
        }

        Text {
            // 湿度，保留1位小数
            id: humidity
            text: ""
            font.pointSize: 11
            y: 320
            x: 280
        }

        // 正向有功电能单位
        Text {
            text: elec.kwh
            font.pointSize: 11
            y: 40
            x: 170
        }

        // 反向有功电能单位
        Text {
            text: elec.kwh
            font.pointSize: 11
            y: 100
            x: 170
        }
        // 电压单位
        Text {
            text: elec.v
            font.pointSize: 11
            y: 160
            x: 115
        }

        Text {
            text: elec.v
            font.pointSize: 11
            y: 205
            x: 115
        }

        Text {
            text: elec.v
            font.pointSize: 11
            y: 250
            x: 115
        }

        // 温度单位
        Text {
            text: elec.degree
            font.pointSize: 11
            y: 320
            x: 115
        }

        // 总有功功率单位
        Text {
            text: elec.kw
            font.pointSize: 11
            y: 40
            x: 400
        }

        // 总视在功率单位
        Text {
            text: elec.kva
            font.pointSize: 11
            y: 70
            x: 400
        }

        // 总有功电能单位
        Text {
            text: elec.kwh
            font.pointSize: 11
            y: 100
            x: 400
        }

        // 电流单位
        Text {
            text: elec.an
            font.pointSize: 11
            y: 160
            x: 365
        }

        Text {
            text: elec.an
            font.pointSize: 11
            y: 205
            x: 365
        }

        Text {
            text: elec.an
            font.pointSize: 11
            y: 250
            x: 365
        }
    }

    GroupBox {
        id: countData
        x: 579
        y: 138
        width: 459
        height: 258
        title: "   计数信息"
        font.pointSize: 12
        background: Rectangle {
            anchors.fill: parent
            anchors.topMargin: 35
            border.color: "gray"
            border.width: 1
            radius: 10
            color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
        }

        Text {
            text: "进料数量："
            font.pointSize: 11
            y: 5
            x: 15
        }

        Text {
            text: "检测数量："
            font.pointSize: 11
            y: 52
            x: 15
        }

        Text {
            text: "总剔除数量："
            font.pointSize: 11
            y: 93
            x: 13
        }

        Text {
            text: "物流码剔除数量："
            font.pointSize: 11
            y: 136
            x: 13
        }

        // 进瓶数
        Text {
            id: countAll
            text: textCountAll
            font.pointSize: 11
            y: 5
            x: 166
        }

        // 检测数量
        Text {
            id: countInspection
            text: textCountInspection
            font.pointSize: 11
            y: 52
            x: 156
        }

        // 总剔除数
        Text {
            id: countEliminate
            text: textCountEliminate
            font.pointSize: 11
            y: 93
            x: 156
        }

        // 物流吗剔除数
        Text {
            id: countLogisticscode
            text: textCountLogisticscode
            font.pointSize: 11
            y: 136
            x: 154
        }

        Text {
            text: "瓶"
            font.pointSize: 11
            y: 5
            x: 394
        }

        Text {
            text: "瓶"
            font.pointSize: 11
            y: 52
            width: 0
            height: 20
            x: 394
        }

        Text {
            text: "瓶"
            font.pointSize: 11
            y: 93
            x: 394
        }

        Text {
            text: "瓶"
            font.pointSize: 11
            y: 136
            x: 394
        }

        Button {
            y: 162
            x: 10
            width: 130
            text: "计数清零"
            icon.color: "#ddfb0a0a"
            icon.height: 35
            icon.width: 35
            icon.source: "file:///" + appdir + "/ico/zerored.png"
            font.pointSize: 11
        }
    }

    Text {
        text: qsTr("下位机状态：")
        font.pointSize: 11
        y: 520
        x: 70
    }

    Text {
        text: qsTr("待启动         运行         急停          终止            暂停      ")
        font.pointSize: 11
        y: 520
        width: 316
        height: 20
        x: 205
    }

    Image {
        id: producePrepare
        width: 20
        height: 20
        y: 520
        x: 240
        property int state: 0
        mipmap: true
        source: textProduceState === "1" ? "file:///" + appdir + "/ico/green.png" : "file:///" + appdir + "/ico/gray.png"
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: produceRun
        width: 20
        height: 20
        y: 520
        x: 310
        property int state: 0
        mipmap: true
        source: textProduceState === "2" ? "file:///" + appdir + "/ico/green.png" : "file:///" + appdir + "/ico/gray.png"
        fillMode: Image.PreserveAspectFit
    }
    Image {
        id: produceEMERGENCYSTOP
        width: 20
        height: 20
        y: 520
        x: 380
        property int state: 0
        mipmap: true
        source: textProduceState === "3" ? "file:///" + appdir + "/ico/green.png" : "file:///" + appdir + "/ico/gray.png"
        fillMode: Image.PreserveAspectFit
    }
    Image {
        id: producePause
        width: 20
        height: 20
        y: 520
        x: 450
        property int state: 0
        mipmap: true
        source: textProduceState === "5" ? "file:///" + appdir + "/ico/green.png" : "file:///" + appdir + "/ico/gray.png"
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: produceStop
        width: 20
        height: 20
        y: 520
        x: 540
        property int state: 0
        mipmap: true
        source: textProduceState === "4" ? "file:///" + appdir + "/ico/green.png" : "file:///" + appdir + "/ico/gray.png"
        fillMode: Image.PreserveAspectFit
    }

    Timer {
        id: timeProduce
        interval: 100//设置定时器定时时间为500ms,默认1000ms
        repeat: true //是否重复定时,默认为false
        running: false //是否开启定时，默认是false，当为true的时候，进入此界面就开始定时
        triggeredOnStart: false // 是否开启定时就触发onTriggered，一些特殊用户可以用来设置初始值。
        onTriggered: {
            refreshProduce();
        }
    }

    function refreshProduce() {
        var jsRet = appMetaFlash.qmlCallExpected(StrightMainWindow.ExpectedFunction.RefreshElecData, "");
        var result = JSON.parse(jsRet);
        if (result.ok) {
            positiveActiveEnergy.text = result.details.positive_active_energy;
            reverseActiveEnergy.text = result.details.reverse_active_energy;
            aPhaseVoltage.text = result.details.a_phase_voltage;
            bPhaseVoltage.text = result.details.b_phase_voltage;
            cPhaseVoltage.text = result.details.c_phase_voltage;
            temperature.text = result.details.temperature;
            totalActivePower.text = result.details.total_active_power;//总有功功率
            totalApparentPower.text = result.details.total_apparent_power;//总视在功率

            aDirectionCurrent.text = result.details.a_direction_current;
            bDirectionCurrent.text = result.details.b_direction_current;
            cDirectionCurrent.text = result.details.c_direction_current;
            humidity.text = result.details.humidity;
            textCountAll = result.details.textCountAll;//进料数
            textCountInspection = result.details.textCountInspection;//检测数量
            textCountEliminate = result.details.textCountEliminate;//总剔除数量
            textCountLogisticscode = result.details.textCountLogisticscode;//物流码剔除数量
            //combinedActiveEnergy.text = result.details.combined_active_energy;//组合有功电能
            combinedActiveEnergy.text = result.details.combined_active_energy;
            textProduceState = result.details.text_produce_state;
        }
    }
}
