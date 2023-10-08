import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window
import QtQuick.Layouts
import Qt.labs.qmlmodels 1.0

GroupBox {
    id: userPageWin
    objectName: "userPageWin"
    property string curname: ""
    background: Rectangle {
        color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
        border.color: "black"
    }
    /*
    modality: Qt.WindowModal
    //固定窗口大小
    minimumWidth: 620
    maximumWidth: 620
    minimumHeight: 520
    maximumHeight: 520
    */
    width: 1110  //Screen.desktopAvailableWidth
    height: 640  //Screen.desktopAvailableHeight

    Rectangle {
        anchors.fill: parent
        anchors.leftMargin: 20
        anchors.topMargin: 20
        color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)

        Rectangle {
            id: userManage
            y: 0
            x: 50
            objectName: "userManage"
            width: parent.width - 50
            height: 40
            color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)

            Row {
                spacing: 0

                Repeater {
                    model: ["用户账号", "用户名称", "操作"]

                    Rectangle {
                        width: {
                            var w = 0;
                            switch (index) {
                            case 0:
                                w = 220;
                                break;
                            case 1:
                                w = 220;
                                break;
                            case 2:
                                w = 500;
                                break;
                            }
                            return w;
                        }
                        height: userManage.height
                        color: "lightgray"
                        border.width: 0
                        Text {
                            text: modelData
                            anchors.centerIn: parent
                            font.pointSize: 12
                        }
                    }
                }
            }
        }
        TableView {
            id: userTableView
            objectName: "userTableView"
            width: parent.width
            anchors.top: userManage.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 90
            anchors.leftMargin: 50
            boundsBehavior: Flickable.StopAtBounds
            focus: true
            clip: true

            ScrollBar.vertical: ScrollBar {
                id: alarmScrollBar
                width: 15
            }

            model: TableModel {
                id: userTableModel
                TableModelColumn {
                    display: "id"
                }
                TableModelColumn {
                    display: "username"
                }
                TableModelColumn {
                    display: "option"
                }
            }
            delegate: DelegateChooser {

                DelegateChoice {
                    column: 0
                    delegate: Rectangle {
                        id: idIndex
                        color: row % 2 === 0 ? Qt.rgba(238 / 255, 238 / 255, 238 / 255, 1) : "lightblue"
                        implicitWidth: 220
                        implicitHeight: 40
                        border.width: 1
                        border.color: "white"
                        MouseArea {
                            anchors.fill: parent
                        }
                        Text {
                            text: display
                            anchors.centerIn: parent
                            font.pointSize: 12
                            color: "black"
                        }
                    }
                }
                DelegateChoice {
                    column: 1
                    delegate: Rectangle {
                        color: row % 2 === 0 ? Qt.rgba(238 / 255, 238 / 255, 238 / 255, 1) : "lightblue"
                        implicitWidth: 220
                        implicitHeight: 40
                        border.width: 1
                        border.color: "white"

                        Text {
                            text: display
                            anchors.centerIn: parent
                            font.pointSize: 12
                            color: "black"
                        }
                    }
                }

                DelegateChoice {
                    column: 2
                    delegate: Rectangle {

                        color: row % 2 === 0 ? Qt.rgba(238 / 255, 238 / 255, 238 / 255, 1) : "lightblue"
                        implicitWidth: 500
                        implicitHeight: 40
                        border.width: 1
                        border.color: "white"

                        Button {
                            id: roleSelectBtn
                            objectName: "roleSelectBtn"
                            x: 50
                            y: 0
                            height: 40
                            width: 60
                            background: Rectangle {
                                width: 0
                                color: "white"
                            }
                            Text {
                                x: 3
                                y: 10
                                text: "信息修改"
                                font.pointSize: 12
                                color: "#f0446c"
                            }

                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    curname = userTableModel.getRow(index - (userTableView.rows * 2)).username;
                                    var component = Qt.createComponent("ModifyUserInfo.qml");
                                    var window = component.createObject(userPageWin);
                                    window.show();
                                }
                            }
                        }

                        Button {
                            id: userPermissionBtn
                            objectName: "userPermissionBtn"
                            x: 200
                            y: 0
                            height: 40
                            width: 60
                            background: Rectangle {
                                width: 0
                                color: "white"
                            }
                            Text {
                                x: 5
                                y: 10
                                text: "权限设置"
                                font.pointSize: 12
                                color: "green"
                            }

                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    curname = userTableModel.getRow(index - (userTableView.rows * 2)).username;
                                    var component = Qt.createComponent("ModifyUser.qml");
                                    var window = component.createObject(userPageWin);
                                    window.show();
                                }
                            }
                        }

                        Button {
                            id: userDeleteBtn
                            objectName: "userDeleteBtn"
                            x: 350
                            y: 0
                            height: 40
                            width: 60
                            background: Rectangle {
                                width: 0
                                color: "white"
                            }
                            Text {
                                x: 18
                                y: 10
                                text: "删除"
                                font.pointSize: 12
                                color: "red"
                            }

                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    var deletrID = userTableModel.getRow(index - (userTableView.rows * 2)).id;
                                    var json = {
                                        "id": deletrID
                                    };
                                    var strSend = JSON.stringify(json);
                                    var success = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.DeleteUser, strSend);
                                    ;
                                    if (success) {
                                        userTableModel.removeRow(index - (userTableView.rows * 2));
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // Component.onCompleted: {
            //     userTableModel.appendRow({"id":"1001","name":"张三","option":true})
            //     userTableModel.appendRow({"id":"1002","name":"李四","option":true})
            //     userTableModel.appendRow({"id":"1003","name":"王五","option":true})
            //     userTableModel.appendRow({"id":"1004","name":"陈6","option":true})
            //     userTableModel.appendRow({"id":"1005","name":"但7","option":true})
            //     userTableModel.appendRow({"id":"1006","name":"候8","option":true})
            //     userTableModel.appendRow({"id":"1001","name":"张三","option":true})
            //     userTableModel.appendRow({"id":"1002","name":"李四","option":true})
            //     userTableModel.appendRow({"id":"1003","name":"王五","option":true})
            //     userTableModel.appendRow({"id":"1004","name":"陈6","option":true})
            //     userTableModel.appendRow({"id":"1005","name":"但7","option":true})
            //     userTableModel.appendRow({"id":"1006","name":"候8","option":true})
            //     userTableModel.appendRow({"id":"1002","name":"李四","option":true})
            //     userTableModel.appendRow({"id":"1003","name":"王五","option":true})
            //     userTableModel.appendRow({"id":"1004","name":"陈6","option":true})
            //     userTableModel.appendRow({"id":"1005","name":"但7","option":true})
            //     userTableModel.appendRow({"id":"1006","name":"候8","option":true})
            // }
        }
    }
    Button {
        id: addUser
        objectName: "addUser"
        text: "添加用户"
        icon.height: 20
        icon.width: 20
        icon.source: "file:///" + appdir + "/ico/addUser.png"
        x: 800
        y: 560
        height: 35
        width: 120
        font.pointSize: 13
        onClicked: {
            var component = Qt.createComponent("AddUser.qml");
            var window = component.createObject(userPageWin);
            window.show();
        }
    }

    Connections {
        target: appMetaFlash // C++ 对象实例
        function onPageUserChange(value) {
            // 执行其他操作...
            var jsonData = JSON.parse(value);
            userTableModel.appendRow({
                    "id": jsonData.id,
                    "username": jsonData.username,
                    "option": true
                });
        }
    }
}
