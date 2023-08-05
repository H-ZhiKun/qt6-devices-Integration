import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5

Rectangle {
    id: loginPage
    width: 400
    height: 360
    //flags: Qt.FramelessWindowHint
    color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
    states: [
        // 将PageA的属性y赋值为0，opacity赋值为1以实现显示的效果
        State {
            name: "show"; PropertyChanges { target: loginPage; y: 0; opacity: 1 }
       },
        // 将PageA的属性y赋值为-height，opaticy赋值为0以实现窗口向上移动并消失的效果
        State {
            name: "hide"; PropertyChanges { target: loginPage; y: -height; opacity: 0 }
       }
    ]
    state: "show"
    transitions: Transition {
               PropertyAnimation { properties: "y,opacity"; duration: 800; easing.type: Easing.InOutBack }
    }
    Timer {
        id: timer_button;
        interval: 800;//设置定时器定时时间为500ms,默认1000ms
        repeat: false //是否重复定时,默认为false
        running: false //是否开启定时，默认是false，当为true的时候，进入此界面就开始定时
        triggeredOnStart: false // 是否开启定时就触发onTriggered，一些特殊用户可以用来设置初始值。
        onTriggered: {
            root.visible = false
            loginPage.visible = false
            root.changeFlag = 1
        }
    }

    Text {
        text:"登录";
        anchors.horizontalCenter: parent.horizontalCenter
        y: 40

        font.pixelSize:30; //像素
    }

    Rectangle {
        anchors.horizontalCenter: parent.horizontalCenter
        y: 290
        width: 330
        height: 52
        color: "#06bcfc"
        Text{
            anchors.horizontalCenter: parent.horizontalCenter
            y: 12
            text: "登录"
            font.pointSize: 15
            width: 40
        }
        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
            onEntered: {
                parent.color = "#169bd5"
            }
            onExited: {
                parent.color = "#06bcfc"
            }
            onPressed: {

                parent.color = "#0b6b95"
            }
            onReleased: {
                parent.color = "#169bd5"
            }
            onClicked: {
                // var json = {"name": txteName.text}
                // var strSend = JSON.stringify(json)
                // var jsRet = appMetaFlash.qmlCallExpected(MainWindow.ExpectedFunction.SelectUser, strSend)
                // var result = JSON.parse(jsRet)
                // if(result.ok === true && result.details.password === txtePassword.text){
                //     loginPage.state = "hide"
                //     timer_button.start()
                // }
                if(loginPage.state === "show"){
                    loginPage.state = "hide"
                    timer_button.start()
                }
                else{
                    labelInfo.text = "用户名或密码错误！"
                }
            }
        }
    }

    Rectangle {
        anchors.horizontalCenter: parent.horizontalCenter
        y:110
        width: 330
        height: 54
        radius: 10

        TextField {
             id: txteName
             anchors.fill: parent
             text: ""
             font.pixelSize: 18
             wrapMode: TextField.Wrap
             leftPadding: 50
             placeholderText: "输入用户名"
            // echoMode: TextInput.Password
        }
        Image {
             id: name24
             x: 10
             y: 12
             width: 30
             height: 30
             source: "file:///" + appdir + "/ico/denglu.png"
             mipmap: true
        }
    }

    Rectangle {
        anchors.horizontalCenter: parent.horizontalCenter
        y: 190
        width: 330
        height: 54
        TextField {
            id: txtePassword
            anchors.fill: parent
            text: ""
            font.pixelSize: 18
            wrapMode: TextField.Wrap
            placeholderText: "输入密码"
            echoMode: TextInput.Password
            leftPadding: 50
        }
        Image {
            id: name25
            x: 10
            y: 12
            width: 30
            height: 30
            source: "file:///" + appdir + "/ico/mima.png"
            mipmap: true
        }
    }

    CheckBox {
        id: checkBox
        x: 35
        y: 250
        text: qsTr("记住我")
    }

    ToolSeparator {
        id: toolSeparator
        x: 184
        y: -105
        rotation: 90
        height: 400
    }

    // Rectangle{
    //     width: 40
    //     height: 40
    //     x: 360
    //     color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
    //     Image {
    //         anchors.fill: parent
    //         anchors.margins: 5
    //         source: "file:///" + appdir + "/ico/guanbi.png"
    //         mipmap: true
    //     }
    //     MouseArea {
    //         anchors.fill: parent
    //         hoverEnabled: true
    //         onEntered: {
    //             parent.color = ("lightgray")
    //         }
    //         onExited: {
    //             parent.color = Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
    //         }
    //         onClicked: {
    //             close()
    //         }
    //         onPressed: {
    //             parent.color = ("gray")
    //         }
    //     }
    // }

    Image {
        id: iconImage
        source: "file:///" + appdir + "/ico/DTFSlogo.png"
        width: 140
        height: 30
        x: 10
        y: 10
        mipmap: true
    }

    Label {
        id: labelInfo
        x: 230
        y: 256
        text: qsTr("")
        color: "red"
    }
}

