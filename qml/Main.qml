import AppFrame 1.0
import QtQuick
import QtQuick.Window
import QtQuick.Controls

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("test test")
    Button {
        id: myText
        objectName: "button1"
        text: appMetaFlash.strValue1
        x: 200
        y: 220
        onClicked: {
            var ip = "127.0.0.1";
            var port = 11110;
            var arguments = [ip, port]; // 传递多个参数作为数组



            var success = appMetaFlash.qmlDominoConnect(ip, port);
            if (success) {
                console.log("C++ function called successfully");
            } else {
                console.error("Failed to call C++ function");
            }
        }

    }
    Button {
        objectName: "button2"
        text: appMetaFlash.strValue2
        x: 400
        y: 220
        onClicked: {
            var ip = "192.168.101.54";
            var port = 11110;
            var arguments = [ip, port]; // 传递多个参数作为数组

            var success = appMetaFlash.qmlDominoConnect(ip, port);
            if (success) {
                console.log("C++ function called successfully");
            } else {
                console.error("Failed to call C++ function");
            }
        }
    }
}
