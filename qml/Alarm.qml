import QtQuick 6.2
import QtQuick.Controls 6.2
import Qt.labs.qmlmodels 1.0

GroupBox {
    id: alarmWin
    objectName: "alarmWin"
    property alias itemCount: pageCompo.itemCount
    property alias alarmList: alarmTable.model
    /*
    modality: Qt.WindowModal
    //固定窗口大小
    minimumWidth: 670
    maximumWidth: 670
    minimumHeight: 620
    maximumHeight: 620
    //visible: true
    */
    width: 1110  //Screen.desktopAvailableWidth
    height: 640  //Screen.desktopAvailableHeight
    background: Rectangle {
        anchors.fill: parent
        border.color: "gray"
        border.width: 1
        radius: 10
        color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
    }
    title: "报警信息"

    GroupBox {
        id: alarmGroup
        objectName: "alarmGroup"
        anchors.fill: parent
        anchors.margins: 30
        font.pixelSize: 20
        anchors.bottomMargin: 80
        background: Rectangle {
            anchors.fill: parent
            border.color: "gray"
            border.width: 1
            radius: 10
            color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
        }

        Rectangle {
            width: parent.width + 20
            height: 35
            color: "lightgray"
            radius: 10
            y: -10
            x: -10
        }

        Text {
            text: "开始时间"
            font.pointSize: 13
            x: 65
            y: -4
        }

        Text {
            text: "结束时间"
            font.pointSize: 13
            x: 270
            y: -4
        }

        Text {
            text: "报警信息"
            font.pointSize: 13
            x: 670
            y: -4
        }

        TableView {
            id: alarmTable
            anchors.fill: parent
            anchors.topMargin: 30
            columnSpacing: 1
            rowSpacing: 1
            boundsBehavior: Flickable.StopAtBounds
            //focus: true
            clip: true
            objectName: "alarmTable"

            // 设置列的宽
            property var columnWidths: [200, 200, 600]
            columnWidthProvider: function (column) {
                return columnWidths[column];
            }
            ScrollBar.vertical: ScrollBar {
                id: alarmScrollBar
                width: 15
            }

            model: TableModel {
                TableModelColumn {
                    display: "startDate"
                }
                TableModelColumn {
                    display: "endDate"
                }
                TableModelColumn {
                    display: "alarmText"
                }
                //rowsInserted:
            }

            selectionModel: ItemSelectionModel {
                model: alarmTable.model
            }

            delegate: Rectangle {
                implicitWidth: 290
                implicitHeight: 30
                //border.width: 1
                color: row % 2 === 0 ? Qt.rgba(238 / 255, 238 / 255, 238 / 255, 1) : "lightblue"

                Text {
                    text: display
                    anchors.centerIn: parent
                }
            }
        }

        PageComponent {
            id: pageCompo
            anchors.horizontalCenter: parent.horizontalCenter
            y: 411
            height: 40
            //width: parent.width-20
            border.color: "gray"
            radius: 4

            pageCurrent: 1
            itemCount: 360
            //触发信号后去请求该页数据，并更新当前页码和总数等
            onRequestPage: {
                pageCurrent = page;
                alarmTable.model.clear();
                var json = {
                    "pageSize": 12,
                    "pageNumber": page
                };
                var strSend = JSON.stringify(json);
                var jsRet = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.SelectAlert, strSend);
                var result = JSON.parse(jsRet);
                if (result.ok === true) {
                    itemCount = result.details.num;
                } else {
                    console.log("require alarm faile");
                }
            }
        }
    }

    Button {
        id: saveDoor
        y: 535
        x: 363
        text: "屏蔽安全门"
        icon.source: "file:///" + appdir + "/ico/pingbi.png"
        property bool saveState: false
        width: 120
        onClicked: {
            if (!saveState) {
                text = "开启安全门";
                saveState = true;
                icon.color = "red";
            } else {
                text = "屏蔽安全门";
                saveState = false;
                icon.color = "black";
            }
        }
    }

    Button {
        id: violent
        y: 535
        x: 603
        text: "静音"
        icon.source: "file:///" + appdir + "/ico/yinliangda.png"
        property bool voiceState: false
        width: 120
        onClicked: {
            if (!voiceState) {
                text = "取消静音";
                voiceState = true;
                icon.source = "file:///" + appdir + "/ico/jingyin.png";
            } else {
                text = "静音";
                voiceState = false;
                icon.source = "file:///" + appdir + "/ico/yinliangda.png";
            }
        }
    }

    Connections {
        target: appMetaFlash // C++ 对象实例
        function onPageAlarmChange(value) {
            var jsonData = JSON.parse(value);
            var num = jsonData.num;
            var dataAlarm = {
                "startDate": jsonData.created_time,
                "endDate": jsonData.updated_time,
                "alarmText": jsonData.content
            };
            alarmTable.model.appendRow(dataAlarm);
        }
    }
}
