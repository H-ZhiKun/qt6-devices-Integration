import QtQuick 2.12
import QtQuick.Controls 2.12

//分页组件上的页码按钮 PageButton
Button {
    id: control

    //该按钮代表的页码
    property int pageNumber: -2
    //当前选中的页码
    property int pageCurrent: -3
    property color textColor: "#666666"
    property color highlightedColor: "#1989FA"
    property alias buttonMouseArea: buttonMouseArea
    implicitWidth: implicitContentWidth+leftPadding+rightPadding
    implicitHeight: implicitContentHeight+topPadding+bottomPadding
    padding: 0
    text: pageNumber

    background: Rectangle{
        id: backRec
        radius: 4
        border.color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
        border.width: 2
        color: Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
    }
    MouseArea{
        id: buttonMouseArea
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            backRec.border.color = "lightblue"
        }
        onExited: {
            backRec.border.color = Qt.rgba(245 / 255, 248 / 255, 245 / 255, 1)
        }
    }
    contentItem: Text {
        text: control.text
        font: control.font
        color: pageNumber===pageCurrent?highlightedColor:textColor
        opacity: enabled ? 1.0 : 0.3
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
