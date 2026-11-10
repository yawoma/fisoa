import QtQuick
import QtQuick.Window

Window{
    visible: true
    width: 640
    height: 480
    title: qsTr("Fisoa")

    Text{
        anchors.centerIn: parent
        text: "Main window Fisoa"
        font.pixelSize: 24
    }
}
