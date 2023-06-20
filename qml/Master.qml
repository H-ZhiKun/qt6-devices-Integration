import QtQuick 2.15
import QtQuick.Controls 2.15


GroupBox  {
    x:0
    y:0
    width: 1110  //Screen.desktopAvailableWidth
    height: 640  //Screen.desktopAvailableHeight


    GroupBox {
        id: frameLocatinCamera
        x: 47
        y: 56
        width: 295
        height: 197
        title: "定位相机"
        Image {
            id: imageLocatinCamera
            objectName: imageLocatinCamera
            anchors.fill: parent
        }
    }

    GroupBox {
        id: frame1
        x: 47
        y: 333
        width: 295
        height: 197
        title: "定位复核相机"
    }

    GroupBox {
        id: frame2
        x: 425
        y: 56
        width: 295
        height: 197
        title: "打码复核相机"
    }

    GroupBox {
        id: formulaData
        x: 798
        y: 119
        width: 255
        height: 103
        font.pointSize: 12
        Image {
            x: -10
            y: -45
            width: 20
            height: 20
            mipmap:true
            source: "file:./ico/peifang.png"
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
            x: 100
            y: 0
            width: 280
            text: ""
            font.pointSize: 11
            clip: true
        }

        Text {
            x: 100
            y: 30
            text: ""
            font.pointSize: 11
        }

        Text {
            x: 150
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
        Image {
            x: -10
            y: -45
            width: 24
            height: 24
            mipmap:true
            source: "file:./ico/shuju1.png"
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
            x: 75
            y: 5
            text: ""
            font.pointSize: 11
        }

        Text {
            x: 75
            y: 35
            text: ""
            font.pointSize: 11
        }

        Text {
            x: 75
            y: 65
            text: ""
            font.pointSize: 11
        }

        Text {
            x: 300
            y: 5
            text: ""
            font.pointSize: 11
        }

        Text {
            x: 300
            y: 35
            text: ""
            font.pointSize: 11
        }

        Text {
            x: 300
            y: 65
            text: ""
            font.pointSize: 11
        }

        Text {
            x: 140
            y: 5
            text: "\u74f6"
            font.pointSize: 11
        }

        Text {
            x: 140
            y: 35
            text: "\u74f6"
            font.pointSize: 11
        }

        Text {
            x: 140
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
            icon.source: "file:./ico/zerored.png"
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
        source: "file:./ico/DTFSlogo.png"
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
        color: producePrepare.state === 0 ? "gray" : "green"
        font.pointSize: 11
    }

    Text {
        id: textRun
        x: 572
        y: 389
        text: qsTr("运行")
        color: produceRun.state === 0 ? "gray" : "green"
        font.pointSize: 11
    }

    Text {
        id: textPause
        x: 629
        y: 389
        text: qsTr("暂停")
        color: producePause.state === 0 ? "gray" : "green"
        font.pointSize: 11
    }

    Text {
        id: textStop
        x: 686
        y: 389
        text: qsTr("终止")
        color: produceStop.state === 0 ? "gray" : "red"
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
        text: qsTr("未启动")
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
        source: "file:./ico/zhunbeizhong.png"
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: produceRun
        x: 572
        y: 359
        width: 30
        height: 30
        property int state: 0
        mipmap: true
        source: "file:./ico/yunxing.png"
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: producePause
        x: 629
        y: 359
        width: 30
        height: 30
        property int state: 0
        mipmap: true
        source: "file:./ico/zanting.png"
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: produceStop
        x: 686
        y: 359
        width: 30
        height: 30
        property int state: 1
        mipmap: true
        source: state === 0 ? "file:./ico/zhongzhiblack.png" : "file:./ico/tingzhi.png"
        fillMode: Image.PreserveAspectFit
    }
}


