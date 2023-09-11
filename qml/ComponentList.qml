import QtQuick 2.15
import QtQuick.Controls 2.15

ListView {
    id: sensorListView1
    // 布局
    anchors.fill: parent
    anchors.topMargin: 30
    objectName: "sensorListView1"
    property int baseIndex: 1  // 从第几个开始
    property string listName: "传感器"
    clip: true  // 超出view部分裁剪
    model: 5  // 每个listview包含几个item
    focus: true
    interactive: false

    // 单个item
    delegate: ItemDelegate {
        id: singleItem
        // highlighted: sensorListView1.isCurrentItem  // 移入高亮
        property alias imageSource: imageState.source
        property string itemName: ""
        property string stateAddr: "" // 状态
        property string modeAddr: "" // 读：手动模式，1为手动、0为自动
        property string locateAddr: "" // 定位，1为定位模式，0为速度模式
        property string handMoveAddr: ""// 写：手动模式，1为手动、0为自动
        property string startAddr: "" // 启动，1为启动，0为停止，按下为1松开为0
        property string jogAddr: "" // 点动模式
        property string directAddr: "" // 电机正向，1为正向，0为反向
        property string setLocateAddr: "" // 设置位置
        property string setSpeedAddr: "" // 设置速度
        property string realLocateAddr: "" // 实际位置
        property string realSpeedAddr: "" // 实际速度
        // 大小
        height: 70
        width: 130

        // background: Qt.rgba(245/255,248/255,245/255,1)

        Rectangle {
            id: sensorRec
            objectName: "sensorRec"
            anchors.fill: parent
            height: 70
            radius: 10
            y: 0
            Text {
                text: "     " + itemName
                font.pointSize: 15
                y: 20
            }

            color: activeFocus ? "lightblue" : Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1) // 选中的颜色变化
            border.color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
            border.width: 2

            // 鼠标点击区域
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true

                onClicked: {
                    sensorListView1.currentIndex = index;  // 更换当前索引
                    parent.forceActiveFocus();  // 获取焦点

                    // parent.color = "lightblue"

                    //lastIndex == -1 ? "" : sensorListView1.itemAtIndex(lastIndex).color = "white"
                    //lastIndex = index
                    // 数据传输到传感器控制栏
                    singalComponent.title = "当前选中： " + itemName;
                    singalComponent.curentSensor = (modelData + baseIndex);
                    singalComponent.curItem = singleItem;
                    if (singleItem.startAddr === "") {
                        // 传感器
                        return;
                    }
                    if (singleItem.locateAddr === "") {
                        // 阀门
                        return;
                    } else {
                        // 电机
                        var json = {
                            [singleItem.modeAddr]: "0",
                            [singleItem.realLocateAddr]: "1",
                            [singleItem.realSpeedAddr]: "2"
                        };
                        var strSend = JSON.stringify(json);
                        var jsRet = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.ReadPLC, strSend);
                        var result = JSON.parse(jsRet);
                        if (result.ok === true) {
                            var modeRes = result.details[singleItem.modeAddr];
                            if (modeRes === "0") {
                                powerAuto.checked = true;
                            }
                            if (modeRes === "1") {
                                powerHandMove.checked = true;
                            }
                            actruePowerParam.acturePosition = result.details[singleItem.realLocateAddr];
                            //console.log("acturePosition: ", result.details.two);
                            actruePowerParam.actureSpeed = result.details[singleItem.realSpeedAddr];
                        } else {
                            setInfo.text = "读取失败！";
                            setInfo.color = "red";
                        }
                    }
                }
                onEntered: {
                    sensorRec.border.color = "#ddabd3ed";
                }
                onExited: {
                    sensorRec.border.color = Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1);
                }
            }

            Image {
                id: imageState
                width: 30
                height: 30
                y: 20
                x: 2
                source: "file:///" + appdir + "/ico/green.png"
                fillMode: Image.PreserveAspectFit
            }

            onFocusChanged: {
                color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1);
            }
        }
    }
}
