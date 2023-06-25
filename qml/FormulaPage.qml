import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQml.Models 2.15

GroupBox {

    /*
    modality: Qt.WindowModal
    //固定窗口大小
    minimumWidth: 780
    maximumWidth: 780
    minimumHeight: 600
    maximumHeight: 600
    visible: true
    */
    width: 1110  //Screen.desktopAvailableWidth
    height: 640  //Screen.desktopAvailableHeight

    id: formulaControllWin
    objectName: "formulaControllWin"

    // 和listview当前索引绑定
    property string curFormulaName:formulaListView.currentItem === null ? "" : formulaListView.currentItem.myData.formulaName  // 当前配方名字


    // 左侧配方选择页面
    GroupBox{
        id: formulaSelect
        objectName: "formulaSelect"
        y: 20
        x: 100
        width: 250
        height: 550
        title: "配方选择"
        font.pointSize: 20

        // 选择model
        ListModel {
            id: formulaListModel
            objectName: "formulaListModel"
            //ListElement { formulaName: "0" }
        }

        ListView{
            // 布局
            anchors.fill: parent
            anchors.topMargin: 10
            id: formulaListView
            objectName: "formulaListView"
            clip: true  // 超出view部分裁剪
            focus: true
            property int newIndex : -1

            model: formulaListModel

            // 单个item
            delegate: ItemDelegate {
                //highlighted: formulaListView.isCurrentItem  // 移入高亮
                property variant myData: model  // 起一个别名，外部才能和内部变量绑定
                // 大小
                height: 30
                width:140

                Rectangle{
                    id: sensorRec
                    objectName: "sensorRec"
                    anchors.fill: parent
                    height: 30
                    y:0

                    Text{
                        text: formulaName
                        font.pointSize: 11
                        y:5
                    }

                    // 选中变色
                    border.color: Qt.lighter(color, 1.1)
                    color: formulaListView.currentIndex == index ? "lightblue" :"white"

                    // 鼠标点击区域
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            formulaListView.currentIndex = index  // 更换当前索引
                            //formulaControllWin.curFormulaName = formulaName
                            //parent.forceActiveFocus()  // 获取焦点
                            buttonModifyFormula.enabled = true
                        }
                    }

                }
            }
        }
    }


    // 中间配方信息相关页面
    GroupBox{
        id: formulaDisplay
        objectName: "formulaDisplay"
        width: 280
        height: 550
        x: 420
        y: 20
        title:"配方信息"
        font.pointSize: 20
        //property string formulaName: "默认配方"
        property int codingXPosition: 0  // 打码机x轴电机位置
        property int codingYPosition: 0  // 打码机Y轴位置
        property int produceSpeed : 0  // 生产速度
        property int produceAcceleration: 0  // 生产加速度
        property int produceDeceleration: 0  // 生产降速度
        property int impurityPosition: 0  // 杂质位置
        Text{
            text:"配方名称："
            font.pointSize: 11
        }

        Text{
            text:"当前状态："
            font.pointSize: 11
            y:28
        }

        Text{
            text:"未下载"
            font.pointSize: 11
            y:28
            x:73
        }

        // 配方名字
        Rectangle{
            x:73
            height:21
            width:180
            id: formulaName
            objectName: "formulaName"
            TextInput{
                height:21
                width:180
                x:0
                y:0
                id: formulaNameInput
                objectName: "formulaNameInput"
                text:curFormulaName
                font.pointSize: 11
                readOnly: true
                clip: true
            }
        }

        // 打码机电机位置
        GroupBox{
            width: 250
            height: 118
            y:65
            title: "电机位置"
            font.pointSize: 11

            Text{
                text:"打码机X轴电机位置："
                font.pointSize: 11
                y:8
            }


            Text{
                text:"打码机Y轴电机位置："
                font.pointSize: 11
                y:38
            }

            // 打码机X轴电机位置
            Rectangle{
                y:8
                x:140
                height:21
                width:35
                id: xPosition
                objectName: "xPosition"
                TextInput{
                    anchors.fill:parent
                    id: xPositionInput
                    objectName: "xPositionInput"
                    text:"200"
                    font.pointSize: 11
                    readOnly: true
                }
            }


            // 打码机Y轴电机位置
            Rectangle{
               id: yPosition
               objectName: "yPosition"
               y:38
               x:140
               height:21
               width:35
               TextInput{
                   anchors.fill:parent
                   id: yPositionInput
                   objectName: "yPositionInput"
                   text:"100"
                   font.pointSize: 11
                   readOnly: true
               }
            }



            Text{
                text:" 毫米"
                font.pointSize: 11
                y:8
                x:170
            }
            Text{
                text:" 毫米"
                font.pointSize: 11
                y:38
                x:170
            }
        }

        // 生产速度
        GroupBox{
            width: 250
            height: 118
            y:285
            x:0
            title: "生产速度"
            font.pointSize: 11

            Text{
                text:"设备生产速度："
                font.pointSize: 11
            }
            Text{
                text:"设备生产加速速度："
                font.pointSize: 11
                y:25
            }
            Text{
                text:"设备生产降速速度："
                font.pointSize: 11
                y:50
            }

            // 设备生产速度
            Rectangle{
                x:130
                height:21
                width:40
                id: deviceProduceSpeed
                objectName: "deviceProduceSpeed"
                TextInput{
                    id: deviceProduceSpeedInput
                    objectName: "deviceProduceSpeedInput"
                    anchors.fill: parent
                    text: "300"
                    font.pointSize: 11
                    readOnly: true
                }
            }


            // 设备生产加速速度
            Rectangle{
                x:130
                y:25
                height:21
                width:40
                id: deviceproduceAcceleration
                objectName: "deviceproduceAcceleration"
                TextInput{
                    id: deviceproduceAccelerationInput
                    objectName: "deviceproduceAccelerationInput"
                    anchors.fill: parent
                    text: "300"
                    font.pointSize: 11
                    readOnly: true
                }
            }

            // 设备生产降速速度
            Rectangle{
                x:130
                y:50
                height:21
                width:40
                id: deviceproduceDeceleration
                objectName: "deviceproduceDeceleration"
                TextInput{
                    id: deviceproduceDecelerationInput
                    objectName: "deviceproduceDecelerationInput"
                    anchors.fill: parent
                    text: "300"
                    font.pointSize: 11
                    readOnly: true
                }
            }



            Text{
                text: "瓶/每秒"
                font.pointSize: 11
                x:170
            }
            Text{
                text: "瓶/每秒"
                font.pointSize: 11
                y:25
                x:170
            }
            Text{
                text: "瓶/每秒"
                font.pointSize: 11
                y:50
                x:170
            }
        }

        //杂质定位
        GroupBox{
            width: 250
            height: 70
            y:200
            title: "杂质定位"
            font.pointSize: 11

            Text{
                anchors.verticalCenter: parent.verticalCenter
                text:"杂质定位位置："
                font.pointSize: 11
                y:8
            }

            // 杂质定位位置
            Rectangle{
                y:0
                x:100
                height:21
                width:40
                id: impurityPositionSite
                objectName: "impurityPositionSite"
                TextInput{
                    id: impurityPositionSiteInput
                    objectName: "impurityPositionSiteInput"
                    anchors.fill: parent
                    text: "300"
                    font.pointSize: 11
                    readOnly: true
                }
            }

            Text{
                anchors.verticalCenter: parent.verticalCenter
                text:"毫米"
                font.pointSize: 11
                y:8
                x:140
            }
        }
    }

    // 右边配方文件相关页面
    GroupBox{
        id: formulaControll
        objectName: "formulaControll"
        width: 180
        height: 550
        x: 770
        y: 20
        title:"配方配置"
        font.pointSize: 20

        Button {
            id: buttonNewFormula
            objectName: "buttonNewFormula"
            anchors.horizontalCenter: parent.horizontalCenter
            y: 10
            width:120
            text: qsTr("新建配方")
            icon.source: "file:./ico/xinjian.png"
            font.pointSize: 11
            onClicked: {
                // 插入到list
                formulaListModel.insert(0, {formulaName: "配方" + Math.random()})
                formulaListView.currentIndex = 0

                // 可编辑
                xPositionInput.readOnly = false
                yPositionInput.readOnly = false
                deviceProduceSpeedInput.readOnly = false
                deviceproduceAccelerationInput.readOnly = false
                deviceproduceDecelerationInput.readOnly = false
                impurityPositionSiteInput.readOnly = false
                formulaNameInput.readOnly = false

                // 改变背景框颜色
                xPosition.color = "lightgrey"
                xPosition.border.color = "grey"
                yPosition.color = "lightgrey"
                yPosition.border.color = "grey"
                deviceProduceSpeed.color = "lightgrey"
                deviceProduceSpeed.border.color = "grey"
                deviceproduceAcceleration.color = "lightgrey"
                deviceproduceAcceleration.border.color = "grey"
                deviceproduceDeceleration.color = "lightgrey"
                deviceproduceDeceleration.border.color = "grey"
                impurityPositionSite.color = "lightgrey"
                impurityPositionSite.border.color = "grey"
                formulaName.color = "lightgrey"
                formulaName.border.color = "grey"

                // 高亮
                buttonNewFormula.highlighted = true

                // 其他按钮不可选取
                buttonModifyFormula.enabled = false
                buttonDownloadFormula.enabled = false
                buttonDeleteFormula.enabled = false
                buttonSaveFormula.enabled = true
                enabled = false

            }
        }

        Button {
            id: buttonModifyFormula
            objectName: "buttonModifyFormula"
            anchors.horizontalCenter: parent.horizontalCenter
            y: 90
            width:120
            text: qsTr("修改配方")
            icon.source: "file:./ico/bianji.png"

            enabled: false
            font.pointSize: 11
            onClicked: {
                // 可编辑
                xPositionInput.readOnly = false
                yPositionInput.readOnly = false
                deviceProduceSpeedInput.readOnly = false
                deviceproduceAccelerationInput.readOnly = false
                deviceproduceDecelerationInput.readOnly = false
                impurityPositionSiteInput.readOnly = false
                formulaNameInput.readOnly = false

                // 改变背景框颜色
                xPosition.color = "lightgrey"
                xPosition.border.color = "grey"
                yPosition.color = "lightgrey"
                yPosition.border.color = "grey"
                deviceProduceSpeed.color = "lightgrey"
                deviceProduceSpeed.border.color = "grey"
                deviceproduceAcceleration.color = "lightgrey"
                deviceproduceAcceleration.border.color = "grey"
                deviceproduceDeceleration.color = "lightgrey"
                deviceproduceDeceleration.border.color = "grey"
                impurityPositionSite.color = "lightgrey"
                impurityPositionSite.border.color = "grey"
                formulaName.color = "lightgrey"
                formulaName.border.color = "grey"

                // 高亮
                buttonModifyFormula.highlighted = true

                // 其他按钮不可选取
                buttonNewFormula.enabled = false
                buttonDownloadFormula.enabled = false
                buttonDeleteFormula.enabled = false
                buttonSaveFormula.enabled = true
            }
        }

        Button {
            id: buttonSaveFormula
            objectName: "buttonSaveFormula"
            anchors.horizontalCenter: parent.horizontalCenter
            y: 170
            width:120
            text: qsTr("保存配方")
            icon.source: "file:./ico/baocun.png"
            font.pointSize: 11
            enabled: false
            onClicked: {
                // 不可编辑
                xPositionInput.readOnly = true
                yPositionInput.readOnly = true
                deviceProduceSpeedInput.readOnly = true
                deviceproduceAccelerationInput.readOnly = true
                deviceproduceDecelerationInput.readOnly = true
                impurityPositionSiteInput.readOnly = true
                formulaNameInput.readOnly = true

                // 改变背景框颜色
                xPosition.color = "white"
                xPosition.border.color = "white"
                yPosition.color = "white"
                yPosition.border.color = "white"
                deviceProduceSpeed.color = "white"
                deviceProduceSpeed.border.color = "white"
                deviceproduceAcceleration.color = "white"
                deviceproduceAcceleration.border.color = "white"
                deviceproduceDeceleration.color = "white"
                deviceproduceDeceleration.border.color = "white"
                impurityPositionSite.color = "white"
                impurityPositionSite.border.color = "white"
                formulaName.color = "white"
                formulaName.border.color = "white"

                // 其他按钮可以使用
                buttonModifyFormula.highlighted = false
                buttonNewFormula.highlighted = false
                buttonNewFormula.enabled = true
                buttonModifyFormula.enabled = true
                buttonDownloadFormula.enabled = true
                buttonDeleteFormula.enabled = true
                buttonNewFormula.enabled = true

                enabled = false
            }
        }

        Button {
            id: buttonDownloadFormula
            objectName: "buttonDownloadFormula"
            anchors.horizontalCenter: parent.horizontalCenter
            y: 250
            width:120
            text: qsTr("下载配方")
            icon.source: "file:./ico/xiazai.png"
            font.pointSize: 11
        }

        Button {
            id: buttonDeleteFormula
            objectName: "buttonDeleteFormula"
            anchors.horizontalCenter: parent.horizontalCenter
            y: 330
            width:120
            text: qsTr("删除配方")
            icon.source: "file:./ico/shanchu.png"
            font.pointSize: 11
            onClicked: {
                formulaListModel.remove(formulaListView.currentIndex)
            }
        }
    }
}
