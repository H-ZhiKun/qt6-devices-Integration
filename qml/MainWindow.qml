import QtQuick 2.15
import QtQuick.Controls 2.15

Window {
    id: root
    width: 400
    height: 430
    visible: true
    color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
    screen: Qt.application.screens[0]
    maximumHeight: 705
    title: "德图福思"
    property int changeFlag: 0

    enum ExpectedFunction {
        DominoConnect,    //0 多米诺设备连接
        DeleteFormula,    //1 删除某个配方数据
        SelectFormula,    //2 查询某个配方数据
        ModifyFormula,    //3 修改某个配方数据
        InsertFormula,    //4 插入某个配方数据
        GetCameraParam,   //5 查询某个相机数据
        SetCameraParam,   //6 修改相机数据
        GetCameraList,    //7 获取相机列表
        InsertUser,       //8 新建用户
        SelectUserID,     //9 查询用户ID
        SelectUser,       //10 查询用户
        DeleteUser,       //11 删除用户
        ModifyUser,       //12 修改用户
        SelectAlert,      //13 选择报警信息
        CollectImage,     //14 采集图像
        ReadPLC,          //15 读取PLC
        WritePLC,         //16 写入PLC
        RefreshMainPage,  //17 刷新主页面数据
        RefreshPowerPage,  //18 刷新电能数据
        RefreshElecData, //19 刷新直线式电能数据
        RefreshStrightMainPage,
        ZeroClearing,
        StrightZeroClearing
    }

    Login {
        id: loginPage
    }
    Master {
        id: master
        visible: false
        states: [
            // 将PageA的属性y赋值为0，opacity赋值为1以实现显示的效果
            State {
                name: "show"
                PropertyChanges {
                    target: master
                    y: 0
                    opacity: 1
                }
            },
            // 将PageA的属性y赋值为-height，opaticy赋值为0以实现窗口向上移动并消失的效果
            State {
                name: "hide"
                PropertyChanges {
                    target: master
                    y: -height
                    opacity: 0.5
                }
            }
        ]
        state: "hide"
        transitions: Transition {
            PropertyAnimation {
                properties: "y, opacity"
                duration: 1000
                easing.type: Easing.InOutBack
            }
        }
    }
    Timer {
        id: timerCheckout
        interval: 600//设置定时器定时时间为500ms,默认1000ms
        repeat: false //是否重复定时,默认为false
        running: false //是否开启定时，默认是false，当为true的时候，进入此界面就开始定时
        triggeredOnStart: false // 是否开启定时就触发onTriggered，一些特殊用户可以用来设置初始值。
        onTriggered: {
            root.visible = true;
            if (master.state === "hide") {
                master.state = "show";
            }
        }
    }

    onChangeFlagChanged: {
        //        loginPage.visible = false
        //        mainPage.visible = true
        if (root.changeFlag === 1) {
            master.visible = true;
            root.height = 768;
            root.width = 1366;
            root.x = (Screen.width - root.width) / 2;
            root.y = (Screen.height - root.height) / 2;
            timerCheckout.start();
        }
    }
}
