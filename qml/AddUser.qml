import QtQuick 2.15
import QtQuick.Controls 2.15

Window {
    width: 500
    height: 350
    property var permissionArray: new Array
    modality: Qt.WindowModal
    color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
    title: "添加用户"
    Label {
        id: label
        x: 40
        y: 30
        text: qsTr("用户名称：")
    }

    Label {
        id: label1
        x: 40
        y: 70
        text: qsTr("用户密码：")
    }

    GroupBox{
        title: "权限设置"
        anchors.fill: parent
        anchors.margins: 30
        anchors.topMargin: 110
        anchors.bottomMargin: 65

        CheckBox {
            id: cameraCheckBox
            x: 19
            y: 0
            text: qsTr("相机模块")
            checked: true
            enabled: false
        }
        CheckBox {
            id: dataCheckBox
            x: 19
            y: 30
            text: qsTr("生产数据模块")
            checked: true
            enabled: false
        }
        CheckBox {
            id: alarmCheckBox
            x: 19
            y: 60
            text: qsTr("报警显示模块")
            checked: true
            enabled: false
        }
        CheckBox {
            id: formulaCheckBox
            x: 19
            y: 90
            text: qsTr("配方模块")
            checked: true
            enabled: false
        }
        CheckBox {
            id: sensorCheckBox
            x: 200
            y: 0
            text: qsTr("传感器模块")
        }
        CheckBox {
            id: valveCheckBox
            x: 200
            y: 30
            text: qsTr("阀门模块")
        }
        CheckBox {
            id: powerCheckBox
            x: 200
            y: 60
            text: qsTr("电机模块")
        }
        CheckBox {
            id: logCheckBox
            x: 200
            y: 90
            text: qsTr("操作日志模块")
        }

        CheckBox {
            id: checkBox
            x: 349
            y: 90
            text: qsTr("全选")
            onCheckedChanged: {
                cameraCheckBox.checked = true
                if(checked){
                    sensorCheckBox.checked = true
                    valveCheckBox.checked = true
                    powerCheckBox.checked = true
                    logCheckBox.checked = true
                }else{
                    sensorCheckBox.checked = false
                    valveCheckBox.checked = false
                    powerCheckBox.checked = false
                    logCheckBox.checked = false
                }
            }
        }
    }

    Button {
        id: button
        x: 120
        y: 306
        width: 80
        text: qsTr("确定")
        onClicked: {
            if(accountName.text === ""){
                nameLabel.text = "名称不得为空！"
            }else{
                if(accountPassword.text === ""){
                    passwordLabel.text = "密码不得为空！"
                }else{
                    // 连接数据库，插入数据
                    var json = {"name": accountName.text, "password": accountPassword.text,
                                "camera_permission": cameraCheckBox.checked ? 1 : 0, 
                                "data_permission": dataCheckBox.checked ? 1 : 0,
                                "alarm_permission": alarmCheckBox.checked ? 1 : 0, "formula_permission": formulaCheckBox.checked ? 1 : 0,
                                "sensor_permission": sensorCheckBox.checked ? 1 : 0, "valve_permission": valveCheckBox.checked ? 1 : 0,
                                "power_permission": powerCheckBox.checked ? 1 : 0, "log_permission": logCheckBox.checked ? 1 : 0,
                                "user_manage_permission": 0
                    }
                    var strSend = JSON.stringify(json)
                    var jsRet = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.InsertUser, strSend)
                    var result = JSON.parse(jsRet);
                    if(result.ok){
                        var jsonName = {"name": accountName.text}
                        var strIDSend = JSON.stringify(jsonName)
                        var jsRetID = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.SelectUserID, strIDSend)
                        var resultID = JSON.parse(jsRetID)
                        if (resultID.ok === true) {
                            var iD = resultID.details.id
                            userTableModel.appendRow( {"id": iD, "name": accountName.text, "option":true});
                            close()
                        }
                        okLabel.text = "新建用户失败！" 
                    }else{
                        okLabel.text = "新建用户失败！"
                    }
                }
            }
        }
    }

    Button {
        id: button1
        x: 302
        y: 306
        width: 80
        text: qsTr("取消")
        onClicked: {
            close()
        }
    }

    TextField {
        id: accountName
        x: 130
        y: 25
        width: 150
        placeholderText: qsTr("请输入名称")
        onTextEdited: {
            nameLabel.text = ""
        }
    }

    TextField {
        id: accountPassword
        x: 130
        y: 65
        width: 150
        placeholderText: qsTr("请输入密码")
        echoMode: TextInput.Password
        onTextEdited: {
            passwordLabel.text = ""
        }
    }

    Label {
        id: nameLabel
        x: 290
        y: 30
        color: "red"
        text: ""
        font.pointSize: 10
    }

    Label {
        id: passwordLabel
        x: 290
        y: 70
        color: "red"
        text: ""
        font.pointSize: 10
    }

    Label {
        id: okLabel
        x: 210
        y: 312
        text: ""
        color: "red"
        font.pointSize: 10
    }
}
