import QtQuick 2.15
import QtQuick.Controls 2.15


GroupBox{
    id: produceDataWin
    width: 1110  //Screen.desktopAvailableWidth
    height: 640  //Screen.desktopAvailableHeight

    property alias positiveActiveEnergy: positiveActiveEnergy.text  // 正向有功电能，保留2位小数
    property alias reverseActiveEnergy: reverseActiveEnergy.text    // 反向有功电能，保留2位小数
    property alias aPhaseVoltage: aPhaseVoltage.text                // A相电压，保留1位小数
    property alias bPhaseVoltage: bPhaseVoltage.text                // B相电压，保留1位小数
    property alias cPhaseVoltage: cPhaseVoltage.text                // C相电压，保留1位小数
    property alias temperature: temperature.text                    // 温度，保留1位小数
    property alias totalActivePower: totalActivePower.text          // 总有功功率，保留3位小数
    property alias totalApparentPower: totalApparentPower.text      // 总视在功率，保留3位小数
    property alias totalActiveEnergy: totalActiveEnergy.text        // 总有功电能，保留2位小数
    property alias aDirectionCurrent: aDirectionCurrent.text        // A向电流，保留2位小数
    property alias bDirectionCurrent: bDirectionCurrent.text        // B向电流，保留2位小数
    property alias cDirectionCurrent: cDirectionCurrent.text        // C向电流，保留2位小数
    property alias humidity: humidity.text                          // 湿度

    property alias formulaName: formulaName.text                    // 配方名字
    property alias formulaSpeed: formulaSpeed.text                  // 配方速度
    property alias numBottlesEntered: numBottlesEntered.text                // 进瓶数
    property alias numQualifiedProducts : numQualifiedProducts .text        // 合格品数
    property alias numAllWasteProducts : numAllWasteProducts .text          // 废品总数
    property alias numPositionWasteProducts : numPositionWasteProducts .text// 定位废品数
    property alias numMarkingWasteProducts : numMarkingWasteProducts .text  // 喷码废品数
    property alias numPauseWasteProducts : numPauseWasteProducts .text      // 暂停废品数

    property alias equipmentSteps : equipmentSteps.text              // 设备步骤[未启动\条件检查\启动各轴
                                                                    //  \自动运行\暂停\点动运动\点动暂停]
    property alias producePrepareState: producePrepare.state        // 准备生产状态
    property alias produceRunState: produceRun.state                // 运行生产状态
    property alias producePauseState: producePause.state            // 暂停生产状态
    property alias produceStopState: produceStop.state              // 终止生产状态


    GroupBox{
        id: elec
        objectName: "elec"
        anchors.left: parent.left
        anchors.leftMargin: 70
        y: 50
        width: 480
        height: 325
        title: "    电能"

        Image {
            source: "file:./ico/dianneng.png"
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


        Text{
            text:"时间："
            font.pointSize: 11
            y:0
            x:0
        }

        Text{
            text:"正向有功电能："
            font.pointSize: 11
            y:30
            x:0
        }

        Text{
            text:"反向有功电能："
            font.pointSize: 11
            y:60
            x:0
        }

        Text{
            text:"A相电压："
            font.pointSize: 11
            y:140
            x:0
        }

        Text{
            text:"B相电压："
            font.pointSize: 11
            y:170
            x:0
        }

        Text{
            text:"C相电压："
            font.pointSize: 11
            y:200
            x:0
        }

        Text{
            text:"温度："
            font.pointSize: 11
            y:240
            x:0
        }

        Text{
            text:"总有功功率："
            font.pointSize: 11
            y:30
            x:240
        }

        Text{
            text:"总视在功率："
            font.pointSize: 11
            y:60
            x:240
        }

        Text{
            text:"总有功电能："
            font.pointSize: 11
            y:90
            x:240
        }

        Text{
            text:"A相电流："
            font.pointSize: 11
            y:140
            x:240
        }

        Text{
            text:"B相电流："
            font.pointSize: 11
            y:170
            x:240
        }

        Text{
            text:"C相电流："
            font.pointSize: 11
            y:200
            x:240
        }

        Text{
            text:"湿度："
            font.pointSize: 11
            y:240
            x:240
        }

        Text{
            text:""
            font.pointSize: 11
            y:0
            x:40
        }

        Text{  // 正向有功电能，保留2位小数
            id: positiveActiveEnergy
            text:""
            font.pointSize: 11
            y:30
            x:100
        }

        Text{  // 反向有功电能，保留2位小数
            id: reverseActiveEnergy
            text:""
            font.pointSize: 11
            y:60
            x:100
        }

        Text{  // A相电压，保留1位小数
            id: aPhaseVoltage
            text:""
            font.pointSize: 11
            y:140
            x:70
        }

        Text{  // B相电压，保留1位小数
            id: bPhaseVoltage
            text:""
            font.pointSize: 11
            y:170
            x:70
        }

        Text{  // C相电压，保留1位小数
            id: cPhaseVoltage
            text:""
            font.pointSize: 11
            y:200
            x:70
        }

        Text{  // 温度，保留1位小数
            id: temperature
            text:""
            font.pointSize: 11
            y:240
            x:40
        }

        Text{  // 总有功功率，保留3位小数
            id: totalActivePower
            text:""
            font.pointSize: 11
            y:30
            x:330
        }

        Text{  // 总视在功率，保留3位小数
            id: totalApparentPower
            text:""
            font.pointSize: 11
            y:60
            x:330
        }

        Text{  // 总有功电能，保留2位小数
            id: totalActiveEnergy
            text:""
            font.pointSize: 11
            y:90
            x:330
        }

        Text{  // A向电流，保留2位小数
            id: aDirectionCurrent
            text:""
            font.pointSize: 11
            y:140
            x:310
        }

        Text{  // B向电流，保留2位小数
            id: bDirectionCurrent
            text:""
            font.pointSize: 11
            y:170
            x:310
        }

        Text{  // C向电流，保留2位小数
            id: cDirectionCurrent
            text:""
            font.pointSize: 11
            y:200
            x:310
        }

        Text{  // 湿度，保留1位小数
            id: humidity
            text:""
            font.pointSize: 11
            y:240
            x:280
        }

        // 正向有功电能单位
        Text{
            text: elec.kwh
            font.pointSize: 11
            y:30
            x:170
        }

        // 反向有功电能单位
        Text{
            text: elec.kwh
            font.pointSize: 11
            y:60
            x:170
        }
        // 电压单位
        Text{
            text: elec.v
            font.pointSize: 11
            y:140
            x:115
        }

        Text{
            text: elec.v
            font.pointSize: 11
            y:170
            x:115
        }

        Text{
            text: elec.v
            font.pointSize: 11
            y:200
            x:115
        }

        // 温度单位
        Text{
            text: elec.degree
            font.pointSize: 11
            y:240
            x:75
        }

        // 总有功功率单位
        Text{
            text: elec.kw
            font.pointSize: 11
            y:30
            x:410
        }

        // 总视在功率单位
        Text{
            text: elec.kva
            font.pointSize: 11
            y:60
            x:410
        }

        // 总有功电能单位
        Text{
            text: elec.kwh
            font.pointSize: 11
            y:90
            x:410
        }

        // 电流单位
        Text{
            text: elec.an
            font.pointSize: 11
            y:140
            x:365
        }

        Text{
            text: elec.an
            font.pointSize: 11
            y:170
            x:365
        }

        Text{
            text: elec.an
            font.pointSize: 11
            y:200
            x:365
        }
    }

    GroupBox{
        id:formulaData
        x:610
        y:50
        width:400
        height:103
        title:"   配方信息"
        font.pointSize: 12

        Image {
            source: "file:./ico/peifang.png"
            mipmap: true
            width: 20
            height: 20
            x: -10
            y: -45
        }

        Text{
            text:"当前配方名字："
            font.pointSize: 11
            y:0
            x:0
        }

        Text{
            text:"当前配方速度："
            font.pointSize: 11
            y:30
            x:0
        }

        // 配方名字
        Text{
            id: formulaName
            text:""
            font.pointSize: 11
            y:0
            x:100
            width: 280
            clip: true
        }

        // 配方速度
        Text{
            id: formulaSpeed
            text:""
            font.pointSize: 11
            y:30
            x:100
        }

        // 配方速度单位
        Text{
            text:"瓶/小时"
            font.pointSize: 11
            y:30
            x:150
        }
    }

    GroupBox{
        id:countData
        x:610
        y:170
        width:400
        height:205
        title:"   计数信息"
        font.pointSize: 12
        Image {
            source: "file:./ico/shuju1.png"
            mipmap: true
            width: 24
            height: 24
            x: -10
            y: -45
        }

        Text{
            text:"进瓶数："
            font.pointSize: 11
            y:5
            x:0
        }

        Text{
            text:"合格品数："
            font.pointSize: 11
            y:35
            x:0
        }

        Text{
            text:"废品总数："
            font.pointSize: 11
            y:65
            x:0
        }

        Text{
            text:"定位废品数："
            font.pointSize: 11
            y:5
            x:180
        }

        Text{
            text:"喷码废品数："
            font.pointSize: 11
            y:35
            x:180
        }

        Text{
            text:"暂停/终止废品数："
            font.pointSize: 11
            y:65
            x:180
        }

        // 进瓶数
        Text{
            id: numBottlesEntered
            text:""
            font.pointSize: 11
            y:5
            x:75
        }

        // 合格品数
        Text{
            id: numQualifiedProducts
            text:""
            font.pointSize: 11
            y:35
            x:75
        }

        // 废品总数
        Text{
            id: numAllWasteProducts
            text:""
            font.pointSize: 11
            y:65
            x:75
        }

        // 定位废品数
        Text{
            id: numPositionWasteProducts
            text:""
            font.pointSize: 11
            y:5
            x:300
        }

        // 喷码废品数
        Text{
            id: numMarkingWasteProducts
            text:""
            font.pointSize: 11
            y:35
            x:300
        }

        // 暂停废品数
        Text{
            id: numPauseWasteProducts
            text:""
            font.pointSize: 11
            y:65
            x:300
        }

        Text{
            text:"瓶"
            font.pointSize: 11
            y:5
            x:140
        }

        Text{
            text:"瓶"
            font.pointSize: 11
            y:35
            x:140
        }

        Text{
            text:"瓶"
            font.pointSize: 11
            y:65
            x:140
        }

        Text{
            text:"瓶"
            font.pointSize: 11
            y:5
            x:365
        }

        Text{
            text:"瓶"
            font.pointSize: 11
            y:35
            x:365
        }

        Text{
            text:"瓶"
            font.pointSize: 11
            y:65
            x:365
        }

        Button{
            y:100
            x:0
            width: 130
            text:"计数清零"
            icon.color: "#ddfb0a0a"
            icon.height: 35
            icon.width: 35
            icon.source: "file:./ico/zerored.png"
            font.pointSize: 11
        }
    }

    Text {
        text: qsTr("生产状态：")
        font.pointSize: 11
        y:520
        x:70
    }

    Text {
        text: qsTr("准备      运行      暂停      终止")
        font.pointSize: 11
        y:520
        x:160
    }

    Text {
        text: qsTr("设备步骤：")
        font.pointSize: 11
        y:560
        x:70
    }

    Text {
        id: equipmentSteps
        text: qsTr("未启动")
        font.pointSize: 11
        y:560
        x:160
    }

    Image {
        id: producePrepare
        width: 30
        height: 30
        y:490
        x:160
        property int state: 0
        mipmap: true
        source: state === 0 ? "file:./ico/gray.png" : "file:./ico/green.png"
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id:produceRun
        width: 30
        height: 30
        y:490
        x:214
        property int state: 0
        mipmap: true
        source: state === 0 ? "file:./ico/gray.png" : "file:./ico/green.png"
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: producePause
        width: 30
        height: 30
        y:490
        x:268
        property int state: 0
        mipmap: true
        source: state === 0 ? "file:./ico/gray.png" : "file:./ico/green.png"
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: produceStop
        width: 30
        height: 30
        y:490
        x:322
        property int state: 0
        mipmap: true
        source: state === 0 ? "file:./ico/gray.png" : "file:./ico/green.png"
        fillMode: Image.PreserveAspectFit
    }
}


