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

    // 单个item
    delegate: ItemDelegate {
        // highlighted: sensorListView1.isCurrentItem  // 移入高亮
        property alias imageSource: imageState.source
        property string itemName: ""
        property string stateAddr: ""
        property string modeAddr: ""
        property string realLocateAddr: ""
        property string realSpeedAddr: ""
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
