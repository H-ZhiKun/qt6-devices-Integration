import QtQuick 2.15
import QtQuick.Controls 2.15

Window {
    width: 500
    height: 250
    property string userName: userPageWin.curname
    property var permissionArray: new Array
    modality: Qt.WindowModal
    color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
    Label {
        id: label
        x: 110
        y: 30
        text: qsTr("用户名称：")
    }

    Label {
        id: label1
        x: 110
        y: 80
        text: qsTr("重新设置密码：")
    }

    Button {
        id: button
        x: 120
        y: 180
        width: 80
        text: qsTr("确定")
        onClicked: {
            if(accountPassword.text === ""){
                passwordLabel.text = "密码不得为空！"
            }else{
                var json = {"name": userName, "password": accountPassword.text}
                var strSend = JSON.stringify(json)
                var jsRet = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.ModifyUser, strSend);
                var result = JSON.parse(jsRet);
                if(result.ok){
                    close()
                }else{
                    okLabel.text = "修改密码失败！"
                }
            }

        }
    }

    Button {
        id: button1
        x: 302
        y: 180
        width: 80
        text: qsTr("取消")
        onClicked: {
            close()
        }
    }

    Text {
        id: accountName
        x: 220
        y: 30
        width: 150
        text: userName
        font.pointSize: 11
    }

    TextField {
        id: accountPassword
        x: 220
        y: 75
        width: 150
        placeholderText: qsTr("请输入新的密码")
        echoMode: TextInput.Password
        onTextEdited: {
            passwordLabel.text = ""
        }
    }

    Label {
        id: passwordLabel
        x: 380
        y: 80
        color: "red"
        text: ""
        font.pointSize: 10
    }

    Label {
        id: okLabel
        x: 210
        y: 188
        text: ""
        color: "red"
        font.pointSize: 10
    }
}
