import QtQuick 2.15
import QtQuick.Controls 2.15


ListView{
    // 布局
    anchors.fill: parent
    anchors.topMargin: 30
    id: sensorListView1
    objectName: "sensorListView1"
    property int baseIndex: 1  // 从第几个开始
    property string listName : "传感器"
    clip: true  // 超出view部分裁剪
    model: 5  // 每个listview包含几个item
    focus: true

    // 单个item
    delegate: ItemDelegate {
        highlighted: sensorListView1.isCurrentItem  // 移入高亮

        // 大小
        height: 70
        width:140

        Rectangle{
            id: sensorRec
            objectName: "sensorRec"
            anchors.fill: parent
            height: 70
            y:0

            Text{
                text:"       " + sensorListView1.listName + (modelData + baseIndex)
                font.pointSize: 16
                y:20
            }

            color: activeFocus ? "lightblue" : "white" // 选中的颜色变化
            border.color: Qt.lighter(color, 1.1)


            // 鼠标点击区域
            MouseArea{

                anchors.fill: parent

                onClicked: {
                    sensorListView1.currentIndex = index  // 更换当前索引
                    parent.forceActiveFocus()  // 获取焦点

                   // parent.color = "lightblue"

                    //lastIndex == -1 ? "" : sensorListView1.itemAtIndex(lastIndex).color = "white"
                    //lastIndex = index
                    // 数据传输到传感器控制栏
                    singalComponent.title = "当前选中的" + sensorListView1.listName + "：" + sensorListView1.listName + (modelData + baseIndex)
                    singalComponent.curentSensor = (modelData + baseIndex)
                }


            }

            Image {
                width: 30
                height: 30
                y:20
                x:8
                source: "file:./ico/green.png"
                fillMode: Image.PreserveAspectFit
            }

            onFocusChanged: {
                color:"white"
            }

        }

    }
}


