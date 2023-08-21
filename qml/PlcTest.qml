import QtQuick 2.15
import QtQuick.Controls 2.15

GroupBox {
    width: 1110
    height: 640
    GroupBox{
        anchors.fill: parent
        anchors.leftMargin: 200
        anchors.topMargin: 50
        anchors.rightMargin: 300
        anchors.bottomMargin: 200

        Label {
            id: label1
            x: 22
            y: 20
            text: "传入float数据："
        }
        Label {
            id: label2
            x: 22
            y: 120
            text: "传入int数据："
        }
        Label {
            id: label3
            x: 22
            y: 220
            text: "传入bool数据："
        }

        TextField {
            id: floatInput
            x: 145
            y: 10
            width: 118
            height: 40
            font.pixelSize: 12
        }

        TextField {
            id: intInput
            x: 145
            y: 110
            width: 118
            height: 40
            text: "56"
            font.pixelSize: 12
        }

        TextField {
            id: boolInput
            x: 145
            y: 210
            width: 118
            height: 40
            font.pixelSize: 12
        }

        TextField {
            id: floatAddr
            x: 290
            y: 10
            width: 118
            height: 40
            font.pixelSize: 12
        }

        TextField {
            id: intAddr
            x: 290
            y: 110
            width: 118
            height: 40
            text: "12301"
            font.pixelSize: 12
        }

        TextField {
            id: boolAddr
            x: 290
            y: 210
            width: 118
            height: 40
            font.pixelSize: 12
        }

        Button {
            id: buttonfloat
            x: 465
            y: 14
            width: 89
            height: 32
            text: qsTr("确定")
            onClicked: {
                var json = {"addr": floatAddr.text, "values": floatInput.text}
                var strSend = JSON.stringify(json)
                var jsRet = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.WritePlcRealData, strSend)
                var result = JSON.parse(jsRet);
                if(result.ok){
                    labelInfo.text = "插入real数据成功！"
                    labelInfo.color = "green"
                }else{
                    labelInfo.text = "插入real数据失败！"
                    labelInfo.color = "red"
                }
            }
        }

        Button {
            id: buttonint
            x: 465
            y: 114
            width: 89
            height: 32
            text: qsTr("确定")
            onClicked: {
                var json = {"addr": intAddr.text, "values": intInput.text}
                var strSend = JSON.stringify(json)
                var jsRet = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.WritePlcData, strSend)
                var result = JSON.parse(jsRet);
                if(result.ok){
                    labelInfo.text = "插入int数据成功！"
                    labelInfo.color = "green"
                }else{
                    labelInfo.text = "插入int数据失败！"
                    labelInfo.color = "red"
                }
            }
        }

        Button {
            id: buttonbool
            x: 465
            y: 214
            width: 89
            height: 32
            text: qsTr("确定")
            onClicked: {
                var json = {"addr": boolAddr.text, "values": boolInput.text}
                var strSend = JSON.stringify(json)
                var jsRet = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.WritePlcBoolData, strSend)
                var result = JSON.parse(jsRet);
                if(result.ok){
                    labelInfo.text = "插入bool数据成功！"
                    labelInfo.color = "green"
                }else{
                    labelInfo.text = "插入bool数据失败！"
                    labelInfo.color = "red"
                }
            }
        }
        
        Label {
            id: label
            x: 145
            y: 56
            text: qsTr("值")
        }
        
        Label {
            id: label4
            x: 290
            y: 56
            text: qsTr("地址")
        }
        
        Label {
            id: label5
            x: 145
            y: 156
            text: qsTr("值")
        }
        
        Label {
            id: label6
            x: 290
            y: 156
            text: qsTr("地址")
        }
        Label {
            id: labe7
            x: 145
            y: 256
            text: qsTr("值")
        }
        
        Label {
            id: label8
            x: 290
            y: 256
            text: qsTr("地址")
        }

        Label {
            id: labelInfo
            x: 31
            y: 319
            text: ""
        }
    }
}
