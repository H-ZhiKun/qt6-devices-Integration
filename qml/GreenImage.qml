import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    width: 40
    height: 40
    property alias y: green.y
    Image {
        id: green
        width: 40
        height: 40
        y:4
        source: "file:./ico/green.png"
        fillMode: Image.PreserveAspectFit
    }
}
