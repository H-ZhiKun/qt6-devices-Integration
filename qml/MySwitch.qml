import QtQuick 2.0
import QtQuick.Controls 2.0

Switch {
    id: mySwitch
    objectName: "mySwitch"
    property color checkedColor: "#0ACF97"

    indicator: Rectangle {
        width: 70
        height: 25
        radius: height / 2
        color: mySwitch.checked ? checkedColor : "#2a87d7"
        border.width: 2
        border.color: mySwitch.checked ? checkedColor : "#E5E5E5"

        Rectangle {
            x: mySwitch.checked ? parent.width - width - 2 : 1
            width: parent.height - 2
            height: width
            radius: width / 2
            anchors.verticalCenter: parent.verticalCenter
            color: "white"
            border.color: "#D5D5D5"

            Behavior on x {
                NumberAnimation { duration: 200 }
            }
        }
    }
}
