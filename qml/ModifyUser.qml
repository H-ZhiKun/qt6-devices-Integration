import QtQuick 2.15
import QtQuick.Controls 2.15

Window {
    width: 500
    height: 280
    property string userName: userPageWin.curname
    property var permissionArray: new Array
    modality: Qt.WindowModal
    color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
    title: "权限设置"
    GroupBox{
        title: "权限设置: " + userName
        anchors.fill: parent
        anchors.margins: 30

        anchors.bottomMargin: 70

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
        y: 230
        width: 80
        text: qsTr("确定")
        onClicked: {
            // 连接数据库，更新数据
            var json = {"name": userName, "camera_permission": cameraCheckBox.checked ? 1 : 0, "data_permission": dataCheckBox.checked ? 1 : 0,
            "alarm_permission": alarmCheckBox.checked ? 1 : 0, "formula_permission": formulaCheckBox.checked ? 1 : 0,
            "sensor_permission": sensorCheckBox.checked ? 1 : 0, "valve_permission": valveCheckBox.checked ? 1 : 0,
            "power_permission": powerCheckBox.checked ? 1 : 0, "log_permission": logCheckBox.checked ? 1 : 0,
            "user_manage_permission": 0}
            var strSend = JSON.stringify(json);
            var jsRet = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.ModifyUser, strSend);
            var result = JSON.parse(jsRet);
            if(result.ok){
                close()
            }else{
                okLabel.text = "修改失败！"
            }
        }
    }

    Button {
        id: button1
        x: 302
        y: 230
        width: 80
        text: qsTr("取消")
        onClicked: {
            close()
        }
    }

    Component.onCompleted: {
        var json = {"name": userName}
        var strSend = JSON.stringify(json);
        var jsRet = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.SelectUser, strSend);
        var result = JSON.parse(jsRet);
        if (result.ok === true) {
            cameraCheckBox.checked = result.details.camera_permission === "1" ? true : false;
            dataCheckBox.checked = result.details.data_permission === "1" ? true : false;
            alarmCheckBox.checked = result.details.alarm_permission === "1" ? true : false;
            formulaCheckBox.checked = result.details.formula_permission === "1" ? true : false;
            sensorCheckBox.checked = result.details.sensor_permission === "1" ? true : false;
            valveCheckBox.checked = result.details.valve_permission === "1" ? true : false;
            powerCheckBox.checked = result.details.power_permission === "1" ? true : false;
            logCheckBox.checked = result.details.log_permission === "1" ? true : false;
        }
    }
}
