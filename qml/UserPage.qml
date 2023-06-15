import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window
import QtQuick.Layouts
import Qt.labs.qmlmodels 1.0
Window {
    id: userPageWin
    objectName: "userPageWin"

    modality: Qt.WindowModal
    //固定窗口大小
    minimumWidth: 620
    maximumWidth: 620
    minimumHeight: 520
    maximumHeight: 520

    Rectangle {
        anchors.fill: parent
        anchors.leftMargin: 20
        anchors.topMargin: 20

        Button{
            id: addUser
            objectName: "addUser"
            text:"添加用户"
            icon.height: 20
            icon.width: 20
            icon.source: "file:./ico/addUser.png"
            x:465
            y:435
            height: 35
            width: 110
            font.pointSize: 12

        }

        Rectangle{
            y:0
            id: userManage
            objectName: "userManage"
            width: parent.width
            height: 40

            Row{
                spacing: 0

                Repeater{
                    model: ["用户账号","用户名称","操作"]

                    Rectangle{
                        width: {
                            var w = 0
                            switch(index){
                            case 0: w = 140;break;
                            case 1: w = 140;break;
                            case 2: w = 300;break;
                            }
                            return w
                        }
                        height: userManage.height
                        color: "white"
                        border.width: 1
                        border.color: "gray"
                        Text {
                            text: modelData
                            anchors.centerIn: parent
                            font.pointSize: 12
                        }
                    }
                }
            }
        }
        TableView{
            id: userTableView
            objectName: "userTableView"
            width: parent.width
            anchors.top:userManage.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 90
            focus: true
            clip: true
            boundsBehavior: Flickable.OvershootBounds

            ScrollBar.vertical: ScrollBar {
                anchors.right:parent.right
                anchors.rightMargin: 0
                visible: userTableView.rowCount > 5
                background: Rectangle{
                    color:"gray"
                }
                onActiveChanged: {
                    active = true;
                }
                contentItem: Rectangle
                {
                    implicitWidth  : 6
                    implicitHeight : 30
                    radius : 3
                    color  : "white"
                }
            }


            model: TableModel {
                id: userTableModel
                TableModelColumn{display: "id"}
                TableModelColumn{display: "name"}
                TableModelColumn{display: "option"}
            }
            delegate: DelegateChooser{

                DelegateChoice{
                    column: 0
                    delegate: Rectangle{
                        id: idIndex
                        color: "white"
                        implicitWidth: 140
                        implicitHeight: 32
                        border.width: 1
                        border.color: "gray"
                        MouseArea{
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
                DelegateChoice{
                    column: 1
                    delegate: Rectangle{
                        color: "white"
                        implicitWidth: 140
                        implicitHeight: 32
                        border.width: 1
                        border.color: "gray"

                        Text {
                            text: display
                            anchors.centerIn: parent
                            font.pointSize: 12
                            color: "black"
                        }
                    }
                }

                DelegateChoice{
                    column: 2
                    delegate: Rectangle{

                        color: "white"
                        implicitWidth: 300
                        implicitHeight: 32
                        border.width: 1
                        border.color: "gray"

                        Button{
                            id: roleSelectBtn
                            objectName: "roleSelectBtn"
                            x: 30
                            y: -4
                            height: 40
                            width: 60
                            background: Rectangle{
                                width: 0
                                color: "white"
                            }
                            Text{
                                x: 3
                                y: 10
                                text:"信息修改"
                                font.pointSize: 12
                                color: "#f0446c"
                            }

                            MouseArea{
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    roleSelectBtn.x= roleSelectBtn.x+1
                                }
                            }
                        }

                        Button{
                            id: userPermissionBtn
                            objectName: "userPermissionBtn"
                            x: 120
                            y: -4
                            height: 40
                            width: 60
                            background: Rectangle{
                                width: 0
                                color: "white"
                            }
                            Text{
                                x: 5
                                y: 10
                                text: "权限设置"
                                font.pointSize: 12
                                color: "green"
                            }

                            MouseArea{
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    roleSelectBtn.x= roleSelectBtn.x+1
                                }
                            }
                        }

                        Button{
                            id: userDeleteBtn
                            objectName: "userDeleteBtn"
                            x:210
                            y:-4
                            height:40
                            width:60
                            background: Rectangle{
                                width: 0
                                color: "white"
                            }
                            Text{
                                x:18
                                y:10
                                text:"删除"
                                font.pointSize: 12
                                color: "red"
                            }

                            MouseArea{
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                   userTableModel.removeRow(index - (userTableView.rows * 2))
                                }
                            }

                        }
                    }
                }
            }


            Component.onCompleted: {
                userTableModel.appendRow({"id":"1001","name":"张三","option":true})
                userTableModel.appendRow({"id":"1002","name":"李四","option":true})
                userTableModel.appendRow({"id":"1003","name":"王五","option":true})
                userTableModel.appendRow({"id":"1004","name":"陈6","option":true})
                userTableModel.appendRow({"id":"1005","name":"但7","option":true})
                userTableModel.appendRow({"id":"1006","name":"候8","option":true})
                userTableModel.appendRow({"id":"1001","name":"张三","option":true})
                userTableModel.appendRow({"id":"1002","name":"李四","option":true})
                userTableModel.appendRow({"id":"1003","name":"王五","option":true})
                userTableModel.appendRow({"id":"1004","name":"陈6","option":true})
                userTableModel.appendRow({"id":"1005","name":"但7","option":true})
                userTableModel.appendRow({"id":"1006","name":"候8","option":true})
                userTableModel.appendRow({"id":"1002","name":"李四","option":true})
                userTableModel.appendRow({"id":"1003","name":"王五","option":true})
                userTableModel.appendRow({"id":"1004","name":"陈6","option":true})
                userTableModel.appendRow({"id":"1005","name":"但7","option":true})
                userTableModel.appendRow({"id":"1006","name":"候8","option":true})
            }
        }

    }

}
